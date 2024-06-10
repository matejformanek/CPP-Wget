/**
*@author Matej Formanek <formama9@fit.cvut.cz>
*/

#include <sstream>
#include <fstream>
#include <sys/socket.h>
#include <cstring>
#include <unistd.h>

#include "CDowcss.h"
#include "CSocket.h"
#include "CDowimg.h"

#define PATH_WEB "/home/matej/pa2/sem/examples/web"
#define PATH_TMP "/home/matej/pa2/sem/examples/tmp"
#define BUFFER_SIZE 1024

//----------------------------------------------------------------------------------//

CDowcss::CDowcss(const CTemplate &src, std::map<std::string, std::string> &visited_css,
                 std::map<std::string, std::string> &visited_img) {
    templ = src.clone();
    visited = &visited_css;
    visited_IMG = &visited_img;
}

//----------------------------------------------------------------------------------//

CDowcss::CDowcss(const CTemplate &src, std::string url, std::map<std::string, std::string> &visited_css,
                 std::map<std::string, std::string> &visited_img) {
    templ = src.clone();
    templ->list_of_parameters[0]->setParameter(2, url);
    if (url.find('/') != std::string::npos)url.erase(0, url.find_first_of('/') + 1);
    links.push(url);
    visited = &visited_css;
    visited_IMG = &visited_img;
}

//----------------------------------------------------------------------------------//

CDowcss::~CDowcss() noexcept = default;

//----------------------------------------------------------------------------------//

std::unique_ptr<CDowtype> CDowcss::clone() const {
    return std::make_unique<CDowcss>(*this);
}

//----------------------------------------------------------------------------------//
//----------------------------------------------------------------------------------//

bool CDowcss::getLinks() {
    std::ifstream ifs(PATH_WEB);
    if (!ifs.is_open()) {
        std::cerr << "FATAL ERROR: Could not open system file (web), can not proceed download CSS!\n";
        return false;
    }
    std::ofstream ofs(PATH_TMP, std::ios::trunc | std::ios::out);
    if (!ofs.is_open()) {
        std::cerr << "FATAL ERROR: Could not open system file (tmp), can not proceed download CSS!\n";
        return false;
    }
    std::string hold;
    // parses css routes and rewrites them in web.html
    while (getline(ifs, hold)) {
        // looks for css in link tag
        if (hold.find("<link") != std::string::npos && hold.find(".css") != std::string::npos) {
            parseUrl(hold);
        }
        // looks for css in import tag
        if (hold.find("@import url") != std::string::npos && hold.find(".css") != std::string::npos) {
            parseUrlImp(hold);
        }
        hold += '\n';
        ofs.write(hold.c_str(), strlen(hold.c_str()));
    }
    ofs.close();
    ifs.close();

    // rewrites main html with correct local links
    std::ifstream is(PATH_TMP);
    if (!is.is_open()) {
        std::cerr << "FATAL ERROR: Could not open system file (tmp), can not update content JS!\n";
        return false;
    }
    std::ofstream os(PATH_WEB, std::ios::trunc | std::ios::out);
    if (!os.is_open()) {
        std::cerr << "FATAL ERROR: Could not open system file (web), can not update content JS!\n";
        return false;
    }

    os << is.rdbuf();
    os.close();
    is.close();

    return true;
}

//----------------------------------------------------------------------------------//

void CDowcss::parseUrl(std::string &hold) {
    std::stringstream ss(hold);
    std::string name;
    ss >> name;
    // Catches multi css on one line
    while (true) {
        // prevents infinite cycle when there is no href inside
        while (name.find("href") == std::string::npos) {
            ss >> name;
            if (ss.fail())break;
        }
        if (ss.fail())break;

        if (name.find("href=\"") != std::string::npos)name.erase(0, name.find("href=\"") + 6);
        else return;
        if (name.find('\"') != std::string::npos)name.erase(name.find_first_of('\"') + 1, name.size());
        else return;
        size_t pos = hold.find(name);
        size_t length = name.size() - 1;
        name = cleanName(name);

        // Controls link for banned, domain and already visited
        if (!controlLink(name)) {
            if (!name.empty()) {
                hold.replace(pos, length, name);
            }
            return;
        }

        // path to be given to GET /
        links.push(name);

        if (name.find('/') != std::string::npos)name.erase(0, name.find_last_of('/') + 1);
        // name of css with directory given by user (local)
        name = localPath(name);

        hold.replace(pos, length, name);
    }
}

//----------------------------------------------------------------------------------//

void CDowcss::parseUrlImp(std::string &hold) {
    std::string name = hold;
    if (name.find('\"') != std::string::npos)name.erase(0, name.find_first_of('\"') + 1);
    else return;
    size_t length = name.size() - 3;
    size_t pos = hold.find(name);
    if (name.find('\"') != std::string::npos)name.erase(name.find_last_of('\"'), name.size());
    if (name.find('.') != std::string::npos)name.erase(name.find_last_of('.') + 4, name.size());

    // Controls link for banned, domain and already visited
    if (!controlLink(name)) {
        if (!name.empty()) {
            hold.replace(pos, length, name);
        }
        return;
    }

    // path to be given to GET /
    links.push(name);

    if (name.find('/') != std::string::npos)name.erase(0, name.find_last_of('/') + 1);
    // name of css with directory given by user (local)
    name = localPath(name);

    hold.replace(pos, length, name);
}

//----------------------------------------------------------------------------------//
// Control visited, ban,not domain = false
bool CDowcss::controlLink(std::string &name) const {
    // do not go to banned
    if (templ->list_of_parameters[0]->getParameter(2) != "EMPTY") {
        // checks for strings of banned ban1,ban2,...
        std::string banned_url = templ->list_of_parameters[0]->getParameter(2);
        for (char &letter: banned_url)if (letter == ',')letter = ' ';
        std::stringstream ss(banned_url);
        std::string hold;
        while (ss >> hold) {
            if (templ->list_of_parameters[0]->getParameter(1).find(hold) != std::string::npos) {
                name = "";
                return false;
            }
        }
    }

    // do not go out of domain
    if (templ->list_of_parameters[0]->getParameter(4) != "EMPTY") {
        // checks for string of domains dom1,dom2,...
        std::string domain_url = templ->list_of_parameters[0]->getParameter(4);
        for (char &letter: domain_url)if (letter == ',')letter = ' ';
        std::stringstream ds(domain_url);
        std::string hold;
        bool found = false;
        while (ds >> hold) {
            if (templ->list_of_parameters[0]->getParameter(1).find(hold) != std::string::npos) {
                found = true;
                break;
            }
        }
        if (!found) {
            name = "";
            return false;
        }
    }

    std::string css_name = name;
    if (name.find('/') != std::string::npos)css_name.erase(0, css_name.find_last_of('/') + 1);
    if (visited->count(templ->list_of_parameters[0]->getParameter(1) + "/" + css_name)) {
        name = (*visited)[templ->list_of_parameters[0]->getParameter(1) + "/" + css_name];
        return false;
    }

    return true;
}

//----------------------------------------------------------------------------------//
//----------------------------------------------------------------------------------//

void CDowcss::donwloadType() {
    while (!links.empty()) {
        int sock;

        // Controls link and its type, saves to tmp so that we can get inside links as well
        if (!processLink(sock))continue;

        // Get links inside a css file + writes into new file
        if (!getInsideLinks())std::cerr << "Could not download css file: " << links.front() << std::endl;

        if (close(sock)) {
            std::cerr << "Socket was not closed correctly, css link: " << links.front() << std::endl;
        }

        links.pop();
    }
}

//----------------------------------------------------------------------------------//
// Controls link and its type, saves to tmp so that we can get inside links as well
bool CDowcss::processLink(int &sock) {
    // Separate css and image links if img send to own download
    if (links.front().find(".css") == std::string::npos) {
        if (templ->list_of_parameters[2]->getParameter(3) == "true") {
            CDowimg img(*templ, links.front(), *visited_IMG);
            img.donwloadType();
        }
        links.pop();
        return false;
    }

    // Attempts to make connection
    CSocket WSocket(templ->list_of_parameters[0]->getParameter(1));
    if (!WSocket.Connect()) {
        links.pop();
        if (close(WSocket.getSocket())) {
            std::cerr << "Socket was not closed correctly, css link: " << links.front() << std::endl;
        }
        return false;
    }

    // Requests file for download
    std::string msg = "GET /" + links.front() + "\r\n\r\n";
    if (send(WSocket.getSocket(), msg.c_str(), strlen(msg.c_str()), MSG_NOSIGNAL) < 0) {
        std::cerr << "Can not send data\n";
        links.pop();
        if (close(WSocket.getSocket())) {
            std::cerr << "Socket was not closed correctly, css link: " << links.front() << std::endl;
        }
        return false;
    }

    // Saves current css in tmp
    if (!writeType(PATH_TMP, WSocket.getSocket())) {
        links.pop();
        if (close(WSocket.getSocket())) {
            std::cerr << "Socket was not closed correctly, css link: " << links.front() << std::endl;
        }
        return false;
    }

    sock = WSocket.getSocket();

    return true;
}

//----------------------------------------------------------------------------------//

bool CDowcss::writeType(const std::string &msg, int sock) const {
    std::ofstream ofs(msg, std::ios::out | std::ios::trunc);
    if (!ofs.is_open()) {
        std::cerr << "Can not open file for writing: " << msg << std::endl;
        return false;
    }

    while (true) {
        char buffer[BUFFER_SIZE] = {'\0'};
        int received = recv(sock, buffer, BUFFER_SIZE, 0);
        if (received < 1 || std::strlen(buffer) == 0)break;
        // Delete transmission info if there is one
        if (buffer[0] == 'H' && buffer[1] == 'T' && buffer[2] == 'T' && buffer[3] == 'P') {
            for (size_t i = 0; i < BUFFER_SIZE; ++i) {
                if (buffer[i] == '\r' && buffer[i + 1] == '\n' && buffer[i + 2] == '\r' &&
                    buffer[i + 3] == '\n')
                    break;
                else buffer[i] = ' ';
            }
        }
        ofs.write(buffer, received);
    }
    ofs.close();

    return true;
}

//----------------------------------------------------------------------------------//
// get Links from Css to img and css
bool CDowcss::getInsideLinks() {
    if (links.front().find('/') != std::string::npos)links.front().erase(0, links.front().find_last_of('/') + 1);
    std::string msg = templ->list_of_parameters[1]->getParameter(2) == "EMPTY/" ?
                      templ->list_of_parameters[1]->getParameter(0) :
                      templ->list_of_parameters[1]->getParameter(2);
    msg += templ->list_of_parameters[0]->getParameter(0) + "_";
    msg += links.front();

    std::ofstream ofs(msg, std::ios::trunc | std::ios::out);
    if (!ofs.is_open()) {
        std::cerr << "FATAL ERROR: Could not open input file, can not update content JS!\n";
        return false;
    }

    std::ifstream ifs(PATH_TMP);
    if (!ifs.is_open()) {
        std::cerr << "FATAL ERROR: Could not open system file (tmp), can not proceed download CSS!\n";
        return false;
    }

    std::string hold;
    while (getline(ifs, hold)) {
        if (hold.find("@import url") != std::string::npos) {
            parseUrlImp(hold);
        }
        hold += '\n';
        ofs.write(hold.c_str(), strlen(hold.c_str()));
    }

    // Add to visited
    std::string path = templ->list_of_parameters[0]->getParameter(1);
    if (path[path.size() - 1] == '/')path.erase(path.size() - 1);
    (*visited)[path + "/" + links.front()] = msg;

    std::cout << "CSS DOWNLOADED\n";

    ifs.close();
    ofs.close();

    return true;
}

//----------------------------------------------------------------------------------//
//----------------------------------------------------------------------------------//

std::string CDowcss::cleanName(std::string &name) const {
    std::string url_clean = templ->list_of_parameters[0]->getParameter(1);
    if (url_clean.find('/') != std::string::npos)url_clean.erase(url_clean.find_first_of('/'), url_clean.size());

    while (name.find(url_clean) != std::string::npos)
        name.erase(0, name.find_first_of('/') + 1);

    if (name.find('\"') != std::string::npos)name.erase(name.find_last_of('\"'), name.size());
    if (name.find('.') != std::string::npos)name.erase(name.find_last_of('.') + 4, name.size());

    return name;
}

//----------------------------------------------------------------------------------//

std::string CDowcss::localPath(const std::string &name) const {
    std::string path = templ->list_of_parameters[1]->getParameter(2) == "EMPTY/" ?
                       templ->list_of_parameters[1]->getParameter(0) :
                       templ->list_of_parameters[1]->getParameter(2);
    path += templ->list_of_parameters[0]->getParameter(0) + "_";
    path += name;

    return path;
}

//----------------------------------------------------------------------------------//