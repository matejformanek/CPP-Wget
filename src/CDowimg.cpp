/**
*@author Matej Formanek <formama9@fit.cvut.cz>
*/

#include <sstream>
#include <fstream>
#include <sys/socket.h>
#include <cstring>
#include <unistd.h>
#include <algorithm>

#include "CDowimg.h"
#include "CSocket.h"

#define PATH_WEB "/home/matej/pa2/sem/examples/web"
#define PATH_TMP "/home/matej/pa2/sem/examples/tmp"
#define BUFFER_SIZE 1024

//----------------------------------------------------------------------------------//

CDowimg::CDowimg(const CTemplate &src, std::map<std::string, std::string> &visited_img) {
    templ = src.clone();
    visited = &visited_img;
}

//----------------------------------------------------------------------------------//

CDowimg::CDowimg(const CTemplate &src, std::string url, std::map<std::string, std::string> &visited_img) {
    templ = src.clone();
    if (url.find('/') != std::string::npos)url.erase(0, url.find_first_of('/') + 1);
    links.push(url);
    visited = &visited_img;
}

//----------------------------------------------------------------------------------//

CDowimg::~CDowimg() noexcept = default;

//----------------------------------------------------------------------------------//

std::unique_ptr<CDowtype> CDowimg::clone() const {
    return std::make_unique<CDowimg>(*this);
}

//----------------------------------------------------------------------------------//
//----------------------------------------------------------------------------------//

bool CDowimg::getLinks() {
    std::ifstream ifs(PATH_WEB);
    if (!ifs.is_open()) {
        std::cerr << "FATAL ERROR: Could not open system file (web), can not proceed download IMG!\n";
        return false;
    }
    std::ofstream ofs(PATH_TMP, std::ios::trunc | std::ios::out);
    if (!ofs.is_open()) {
        std::cerr << "FATAL ERROR: Could not open system file (tmp), can not proceed download IMG!\n";
        return false;
    }
    std::string hold;
    // parses img routes and rewrites them in web.html
    while (getline(ifs, hold)) {
        // looks for img in img tag
        if (hold.find("<img") != std::string::npos && hold.find("src") != std::string::npos) {
            parseUrl(hold);
        }
        hold += '\n';
        ofs.write(hold.c_str(), strlen(hold.c_str()));
    }
    ofs.close();
    ifs.close();

    std::ifstream is(PATH_TMP);
    if (!is.is_open()) {
        std::cerr << "FATAL ERROR: Could not open system file (tmp), can not update content IMG!\n";
        return false;
    }
    std::ofstream os(PATH_WEB, std::ios::trunc | std::ios::out);
    if (!os.is_open()) {
        std::cerr << "FATAL ERROR: Could not open system file (web), can not update content IMG!\n";
        return false;
    }

    os << is.rdbuf();
    os.close();
    is.close();

    return true;
}

//----------------------------------------------------------------------------------//

void CDowimg::parseUrl(std::string &hold) {
    std::stringstream ss(hold);
    std::string name;
    ss >> name;
    // Catches multi img on one line
    while (true) {
        // prevents infinite cycle when there is no src inside
        while (name.find("src") == std::string::npos) {
            ss >> name;
            if (ss.fail())break;
        }
        if (ss.fail())break;

        if (name.find("src=\"") != std::string::npos)name.erase(0, name.find_first_not_of("src=\""));
        else return;
        if (name.find('\"') != std::string::npos)name.erase(name.find_first_of('\"'), name.size());
        else return;
        // position to be deleted from and length of deleted content
        size_t pos = hold.find(name);
        size_t length = name.size();
        name = cleanName(name);

        if (!controlLink(name)) {
            if (!name.empty()) {
                hold.replace(pos, length, name);
            }
            return;
        }

        // path to be given to GET /
        links.push(name);

        if (name.find('/') != std::string::npos)name.erase(0, name.find_last_of('/') + 1);
        // name of img with directory given by user (local)
        name = localPath(name);

        hold.replace(pos, length, name);
    }
}

//----------------------------------------------------------------------------------//

bool CDowimg::controlLink(std::string &name) const {
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

    std::string img_name = name;
    if (name.find('/') != std::string::npos)img_name.erase(0, img_name.find_last_of('/') + 1);
    if (visited->count(templ->list_of_parameters[0]->getParameter(1) + "/" + img_name)) {
        name = (*visited)[templ->list_of_parameters[0]->getParameter(1) + "/" + img_name];
        return false;
    }

    return true;
}

//----------------------------------------------------------------------------------//
//----------------------------------------------------------------------------------//

void CDowimg::donwloadType() {
    while (!links.empty()) {
        CSocket WSocket(templ->list_of_parameters[0]->getParameter(1));
        if (!WSocket.Connect()) {
            links.pop();
            continue;
        }

        std::string msg = "GET /" + links.front() + "\r\n\r\n";
        if (send(WSocket.getSocket(), msg.c_str(), strlen(msg.c_str()), MSG_NOSIGNAL) < 0) {
            std::cerr << "Can not send data\n";
            return;
        }

        if (links.front().find('/') != std::string::npos)links.front().erase(0, links.front().find_last_of('/') + 1);
        msg = templ->list_of_parameters[1]->getParameter(4) == "EMPTY/" ?
              templ->list_of_parameters[1]->getParameter(0) :
              templ->list_of_parameters[1]->getParameter(4);
        msg += templ->list_of_parameters[0]->getParameter(0) + "_";
        msg += links.front();

        std::string path = templ->list_of_parameters[0]->getParameter(1);
        if (path[path.size() - 1] == '/')path.erase(path.size() - 1);
        (*visited)[path + "/" + links.front()] = msg;

        writeType(msg, WSocket.getSocket());

        links.pop();
    }
}

//----------------------------------------------------------------------------------//

bool CDowimg::writeType(const std::string &msg, int sock) const {
    std::ofstream ofs(msg, std::ios::binary | std::ios::trunc);
    if (!ofs.is_open()) {
        std::cerr << "Can not open file for writing: " << msg << std::endl;
        return false;
    }

    while (true) {
        char buffer[BUFFER_SIZE] = {'\0'};
        int receive = recv(sock, buffer, BUFFER_SIZE, 0);
        if (receive < 1 || std::strlen(buffer) == 0)break;
        ofs.write(buffer, receive);
    }
    ofs.close();
    std::cout << "IMG DOWNLOADED\n";

    if (close(sock)) {
        std::cerr << "Socket was not closed correctly, img link: " << msg << std::endl;
    }

    return true;
}

//----------------------------------------------------------------------------------//
//----------------------------------------------------------------------------------//

std::string CDowimg::cleanName(std::string &name) const {
    std::string url_clean = templ->list_of_parameters[0]->getParameter(1);
    if (url_clean.find('/') != std::string::npos)url_clean.erase(url_clean.find_first_of('/'), url_clean.size());

    while (name.find(url_clean) != std::string::npos)
        name.erase(0, name.find_first_of('/') + 1);

    return name;
}

//----------------------------------------------------------------------------------//

std::string CDowimg::localPath(const std::string &name) const {
    std::string path = templ->list_of_parameters[1]->getParameter(4) == "EMPTY/" ?
                       templ->list_of_parameters[1]->getParameter(0) :
                       templ->list_of_parameters[1]->getParameter(4);
    path += templ->list_of_parameters[0]->getParameter(0) + "_";
    path += name;

    return path;
}

//----------------------------------------------------------------------------------//