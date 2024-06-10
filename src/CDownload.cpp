/**
*@author Matej Formanek <formama9@fit.cvut.cz>
*/

#include <unistd.h>
#include <cstring>
#include <fstream>
#include <sstream>
#include <algorithm>

#include "CDownload.h"
#include "CDowtype.h"
#include "CDowhtml.h"
#include "CDowcss.h"
#include "CDowjs.h"
#include "CDowimg.h"

#define PATH_WEB "/home/matej/pa2/sem/examples/web"
#define PATH_TMP "/home/matej/pa2/sem/examples/tmp"
#define BUFFER_SIZE 1024

//----------------------------------------------------------------------------------------//

CDownload::CDownload(const std::vector<std::unique_ptr<CTemplate>> &src) {
    depth_id = 1;
    for (auto &temp: src) {
        // Unique names for quick download
        if (!(temp->list_of_parameters[0]->getParameter(0)[0] == 'Q' &&
              temp->list_of_parameters[0]->getParameter(0)[1] == '_'))
            temp->list_of_parameters[0]->setParameter(0, std::to_string(depth_id++) +
                                                         temp->list_of_parameters[0]->getParameter(0));
        templates_download.emplace_back(temp->clone());
    }
    depth_id = 0;
}

//----------------------------------------------------------------------------------------//

CDownload::~CDownload() noexcept = default;

//----------------------------------------------------------------------------------------//
// Starts part by part functions that navigate through download
bool CDownload::Prompt() {
    if (templates_download.empty()) {
        std::cout << "!!! Missing templates for download !!!\n";
        return false;
    }

    // Cycle through templates that shall be downloaded
    for (const auto &templat: templates_download) {
        // Allows to add new templates for depth download
        deep_site.push(templat->clone());
        while (!deep_site.empty()) {
            std::string url = deep_site.front()->list_of_parameters[0]->getParameter(1);
            int sock;

            // Checks for url and that the url is not visited plus creates connection with server
            if (!processParameters(sock, url))continue;

            // Checks for server response, also navigates or downloads link, if html add to visited
            if (!processLink(sock, url))continue;

            // Make a list of files that shall be downloaded, saved here because addNewlayers changes current template
            std::string name = deep_site.front()->list_of_parameters[0]->getParameter(0);
            std::vector<std::unique_ptr<CDowtype>> download_types = downloadTypes(*(deep_site.front()));

            // if depth > 0 -> add new templates with urls from web.html and depth - 1
            int depth = std::atoi(deep_site.front()->list_of_parameters[0]->getParameter(3).c_str());
            if (depth > 0) {
                addNewlayers(*(deep_site.front()));
            }

            // Download each chosen type
            for (const auto &dow_type: download_types) {
                if (!dow_type->getLinks()) {
                    std::cerr << "System file not available, shutting download!\n";
                    return false;
                }
                dow_type->donwloadType();
            }

            deep_site.pop();
            std::cout << "TEMPLATE DOWNLOADED " << name << "\n\n";
        }
    }

    return true;
}

//----------------------------------------------------------------------------------------//
//------------------------------- PROCESS PARAMETERS -------------------------------------//
//----------------------------------------------------------------------------------------//
// Checks for url and that the url is not visited plus creates connection with server
bool CDownload::processParameters(int &sock, const std::string &url) {
    // Check that the template has url to download from
    if (!checkUrl(*(deep_site.front()))) {
        deep_site.pop();
        return false;
    }

    // Creates connection with server
    std::string canUrl = deep_site.front()->list_of_parameters[0]->getParameter(1);
    deep_site.front()->list_of_parameters[0]->setParameter(2, cannonicalUrl(canUrl));
    CSocket WSocket(deep_site.front()->list_of_parameters[0]->getParameter(1));
    if (!WSocket.Connect()) {
        deep_site.pop();
        return false;
    }

    // Visited check
    if (visited.count(url)) {
        // checks visited and that the file actually already exists
        std::ifstream tif(deep_site.front()->list_of_parameters[1]->getParameter(1) + "/" + visited[url]);
        if (tif.is_open()) {
            tif.close();
            deep_site.pop();
            std::cout << "SKIPPING COPY\n\n";
            return false;
        }
    }

    sock = WSocket.getSocket();

    return true;
}

//----------------------------------------------------------------------------------------//
// Makes sure the template has url to download from
bool CDownload::checkUrl(const CTemplate &src) {
    if (src.list_of_parameters[0]->getParameter(1).empty() || src.list_of_parameters[0]->getParameter(1) == "EMPTY") {
        std::cout << "\n!!! MISSING URL !!!\n"
                     "Add url and save to continue downloading this template\n"
                     "Save without adding url to abort this templates download\n";
        src.list_of_parameters[0]->setParameter();

        // if URL is not added even after warning prompt abort
        if (src.list_of_parameters[0]->getParameter(1).empty() ||
            src.list_of_parameters[0]->getParameter(1) == "EMPTY") {
            std::cout << "ABORTING DOWNLOAD OF TEMPLATE: " << src.list_of_parameters[0]->getParameter(0) << std::endl;
            return false;
        }
    }
    return true;
}

//----------------------------------------------------------------------------------------//
//------------------------------------ PROCESS LINK --------------------------------------//
//----------------------------------------------------------------------------------------//
// Checks for server response, also navigates or downloads link, if html add to visited
bool CDownload::processLink(int sock, const std::string &url) {
    // Checks for server response,controls that GET / is working (returning something)
    if (!checkGet(sock, url)) {
        deep_site.pop();
        return false;
    }

    // Split -> css, js and img download separately (if given absolute path to the item)
    if (!splitUrltypes(url)) {
        deep_site.pop();
        return false;
    }

    // Write into examples/web placeholder for current working website
    if (!downloadFramehtml(sock)) {
        deep_site.pop();
        return false;
    }
    // add to visited
    if (!visited.count(url))
        visited[url] = deep_site.front()->list_of_parameters[0]->getParameter(0) + "_R.html";

    return true;
}

//----------------------------------------------------------------------------------------//
// Checks the server is responding
bool CDownload::checkGet(int sock, std::string url) {
    if (url.find("https://") != std::string::npos) {
        std::cerr << "Can not read https websites\n";
        return false;
    }

    size_t found;
    if ((found = url.find("http://")) != std::string::npos)
        url.erase(found, 7);
    if ((found = url.find("www.")) != std::string::npos)
        url.erase(found, 4);
    if (url.find('/') != std::string::npos)
        url.erase(0, url.find_first_of('/') + 1);
    else url = "";

    std::string msg = "GET /" + url + "\r\n\r\n";

    // Controls server responds
    if (send(sock, msg.c_str(), strlen(msg.c_str()), MSG_NOSIGNAL) < 0) {
        std::cerr << "Can not send data\n";
        return false;
    }

    return true;
}

//----------------------------------------------------------------------------------------//
// Splits types and downloads them if the URL is explicitly asking for the specific type
bool CDownload::splitUrltypes(const std::string &url) {
    if (url.find(".css") != std::string::npos) {
        if (deep_site.front()->list_of_parameters[2]->getParameter(1) == "false") {
            std::cout << "CSS link - false\n";
            return false;
        }
        CDowcss css(*deep_site.front(), url, visited_css, visited_img);
        css.donwloadType();
    } else if (url.find(".js") != std::string::npos) {
        if (deep_site.front()->list_of_parameters[2]->getParameter(2) == "false") {
            std::cout << "JS link - false\n";
            return false;
        }
        CDowjs js(*deep_site.front(), url, visited_js, visited_img);
        js.donwloadType();
    } else if (url.find(".png") != std::string::npos || url.find(".jpg") != std::string::npos ||
               url.find(".webp") != std::string::npos || url.find(".svg") != std::string::npos) {
        if (deep_site.front()->list_of_parameters[2]->getParameter(3) == "false") {
            std::cout << "IMG link - false\n";
            return false;
        }
        CDowimg img(*deep_site.front(), url, visited_img);
        img.donwloadType();
    } else return true;

    return false;
}


//----------------------------------------------------------------------------------------//
// Downloads html page from which all links (html,css,img) will be taken
bool CDownload::downloadFramehtml(int sock) {
    std::ofstream ofs(PATH_WEB, std::ios::out | std::ios::trunc);
    if (!ofs.is_open()) {
        if (close(sock)) {
            std::cerr << "Socket was not closed correctly" << std::endl;
        }
        std::cerr << "FATAL ERROR: Could not open system file (web), can not download!\n";
        return false;
    }

    bool first = true;
    while (true) {
        char buffer[BUFFER_SIZE] = {'\0'};
        int received = recv(sock, buffer, BUFFER_SIZE, 0);
        if (received < 1 || std::strlen(buffer) == 0) {
            if (first) {
                std::cerr << "NO RESPONSE  FROM SERVER\n";
                return false;
            }
            break;
        }

        // delete beginning info if it has one
        if (first) {
            for (char &letter: buffer) {
                if (letter == '<')break;
                else letter = ' ';
            }
            first = false;
        }
        // received is number of Received characters from server
        ofs.write(buffer, received);
    }

    ofs.close();
    std::cout << "SYSTEM INITIALIZED\n";

    if (close(sock)) {
        std::cerr << "Socket was not closed correctly" << std::endl;
    }

    return true;
}

//----------------------------------------------------------------------------------------//
//---------------------------------- ADD NEW LAYERS --------------------------------------//
//----------------------------------------------------------------------------------------//
// functions -> copy template, change name,depth-- in template, add to the list, check visited domain and banned
void CDownload::addNewlayers(CTemplate src) {
    // Changes depth to one lower
    int depth = std::atoi(src.list_of_parameters[0]->getParameter(3).c_str()) - 1;
    std::string s_depth = std::to_string(depth);
    src.list_of_parameters[0]->setParameter(1, s_depth);

    // Next layer (depth) links that shall be downloaded
    std::vector<std::string> links = getLinks(src.list_of_parameters[0]->getParameter(1),
                                              src.list_of_parameters[0]->getParameter(0),
                                              src.list_of_parameters[0]->getParameter(2),
                                              src.list_of_parameters[0]->getParameter(4));
    // Cycle through next layer links
    for (const auto &link: links) {
        // Set new attributes of template and save
        src.list_of_parameters[0]->setParameter(0, src.list_of_parameters[0]->getParameter(0) +
                                                   std::to_string(depth_id++)); //unique name of template->CUrl param
        src.list_of_parameters[0]->setParameter(2, link); // set url of link
        src.name = src.list_of_parameters[0]->getParameter(0); // name of template
        visited[link] = src.list_of_parameters[0]->getParameter(0) + "_R.html";
        deep_site.push(src.clone());
        // Set name of the template back to default otherwise the numbers will stack

        src.list_of_parameters[0]->setParameter(0, src.list_of_parameters[0]->getParameter(0).
                erase(src.list_of_parameters[0]->getParameter(0).size() - std::to_string(depth_id - 1).size()));
    }
}

//----------------------------------------------------------------------------------------//
// find links (urls) pointing to other sites inside the web
std::vector<std::string> CDownload::getLinks(const std::string &src_url, const std::string &src_name,
                                             const std::string &src_url_banned, const std::string &src_url_domain) {
    std::ifstream ifs(PATH_WEB);
    if (!ifs.is_open()) {
        std::cerr << "FATAL ERROR: Could not open system file (web), can not browse more links!\n";
        return {};
    }
    std::ofstream ofs(PATH_TMP, std::ios::trunc | std::ios::out);
    if (!ifs.is_open()) {
        std::cerr << "FATAL ERROR: Could not open system file (tmp), can not browse more links!\n";
        return {};
    }

    std::string hold;
    std::vector<std::string> links;
    size_t id = depth_id;

    // search links for other pages
    while (getline(ifs, hold)) {
        if (hold.find("<a") != std::string::npos && hold.find("mailto:") == std::string::npos) {
            parseUrl(src_url, src_name, src_url_banned, src_url_domain, hold, links, id);
        }
        hold += '\n';
        ofs.write(hold.c_str(), strlen(hold.c_str()));
    }
    ifs.close();
    ofs.close();

    // save main html file with local links to other sites
    std::ifstream is(PATH_TMP);
    if (!is.is_open()) {
        std::cerr << "FATAL ERROR: Could not open system file (tmp), can not update content!\n";
        return {};
    }
    std::ofstream os(PATH_WEB, std::ios::trunc | std::ios::out);
    if (!is.is_open()) {
        std::cerr << "FATAL ERROR: Could not open system file (web), can not update content!\n";
        return {};
    }

    os << is.rdbuf();
    os.close();
    is.close();

    return links;
}

//----------------------------------------------------------------------------------------//

void CDownload::parseUrl(const std::string &src_url, const std::string &src_name, const std::string &src_url_banned,
                         const std::string &src_url_domain, std::string &hold, std::vector<std::string> &links,
                         size_t &id) {
    std::stringstream ss(hold);
    std::string name;
    ss >> name;

    // cycle to make sure u get all URLs even if they are in one line
    while (true) {
        // prevents infinite cycle when there is no href inside
        while (name.find("href=") == std::string::npos) {
            ss >> name;
            if(name == "href="){
                std::string prefix = name;
                ss >> name;
                name = prefix + name;
                break;
            }
            if (ss.fail())break;
        }
        if (ss.fail())break;

        if (name.find("href=\"") != std::string::npos)name.erase(0, name.find("href=\"") + 6);
        else return;
        if (name.find('\"') != std::string::npos)name.erase(name.find_first_of('\"'), name.size());
        else return;
        // position to be deleted from and length of deleted content
        size_t position = hold.find(name);
        size_t length = name.size();
        name = cleanName(name, src_url);
        name = cannonicalUrl(name);
        if (name[name.size() - 1] == '/')name.erase(name.size() - 1);

        // Checks if the url is banned or domain
        if (!correctUrl(name, src_url_banned, src_url_domain))continue;

        // if visited give local path and do not add to links
        if (visited.count(name)) {
            hold.replace(position, length, "./" + visited[name]);
        } else if (std::binary_search(links.begin(), links.end(), name)) {
            // make sure same objects has same links because they are not yet visited
            size_t pos = std::lower_bound(links.begin(), links.end(), name) - links.begin();
            pos = links.size() - pos;
            hold.replace(position, length, "./" + src_name + std::to_string(id - pos) + "_R.html");
        } else {
            links.push_back(name);
            hold.replace(position, length, "./" + src_name + std::to_string(id++) + "_R.html");
        }
    }
}

//----------------------------------------------------------------------------------------//

bool
CDownload::correctUrl(const std::string &name, const std::string &src_url_banned, const std::string &src_url_domain) {
    // do not go to banned
    if (src_url_banned != "EMPTY") {
        // checks for strings of banned ban1,ban2,...
        std::string url = src_url_banned;
        for (char &letter: url)if (letter == ',')letter = ' ';
        std::stringstream bs(url);
        std::string banned_url;
        while (bs >> banned_url) {
            if (name.find(banned_url) != std::string::npos)return false;
        }
    }

    // do not go out of domain
    if (src_url_domain != "EMPTY") {
        // checks for string of domains dom1,dom2,...
        std::string url = src_url_domain;
        for (char &letter: url)if (letter == ',')letter = ' ';
        std::stringstream ds(url);
        std::string domain_url;
        bool found = false;
        while (ds >> domain_url) {
            if (name.find(domain_url) != std::string::npos) {
                found = true;
                break;
            }
        }
        if (!found)return false;
    }

    return true;
}

//----------------------------------------------------------------------------------------//
//--------------------------------- SUPPORT FUNCTIONS ------------------------------------//
//----------------------------------------------------------------------------------------//
// choose which types will be downloaded
std::vector<std::unique_ptr<CDowtype>> CDownload::downloadTypes(const CTemplate &src) {
    std::vector<std::unique_ptr<CDowtype>> hold;

    if (src.list_of_parameters[2]->getParameter(1) == "true")
        hold.emplace_back(CDowcss(src, visited_css, visited_img).clone());
    if (src.list_of_parameters[2]->getParameter(2) == "true")
        hold.emplace_back(CDowjs(src, visited_js, visited_img).clone());
    if (src.list_of_parameters[2]->getParameter(3) == "true")
        hold.emplace_back(CDowimg(src, visited_img).clone());
    if (src.list_of_parameters[2]->getParameter(0) == "true")
        hold.emplace_back(CDowhtml(src).clone());

    return hold;
}

//----------------------------------------------------------------------------------------//
// strips to pure address
std::string CDownload::cleanName(std::string &name, const std::string &src_url) {
    if (name.find('#') != std::string::npos)name.erase(name.find('#'), name.size());
    if (name.empty())return src_url + "/";
    if (name.find('/') != std::string::npos)name.erase(0, name.find_first_of('/') + 1);
    if (name[0] == '/') {
        return name.erase(0, 1);
    }

    return src_url + "/" + name;
}

//----------------------------------------------------------------------------------------//

std::string CDownload::cannonicalUrl(std::string &url) {
    size_t found;
    if ((found = url.find("http://")) != std::string::npos)
        url.erase(found, 7);
    if ((found = url.find("www.")) != std::string::npos)
        url.erase(found, 4);
    if ((found = url.find('?')) != std::string::npos)
        url.erase(found, url.size());
    if (url[0] == '/')url.erase(0, 1);

    return url;
}

//----------------------------------------------------------------------------------------//