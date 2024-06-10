/**
*@author Matej Formanek <formama9@fit.cvut.cz>
*/

#include <sstream>
#include <fstream>
#include <vector>
#include <algorithm>

#include "CUrl.h"
#include "CPromptmsg.h"

#define TEMPLATES_FILE "/home/matej/pa2/sem/examples/templates.txt"

//------------------------------------------------------------------------//

std::unique_ptr<CParameter> CUrl::clone() const {
    return std::make_unique<CUrl>(*this);
}

//------------------------------------------------------------------------//

std::unique_ptr<CParameter> CUrl::setParameter() {
    CPromptmsg msg;
    msg.parUrl();

    char choice;
    while (std::cin >> choice) {
        switch (choice) {
            case '1':
                if (!addName())return this->clone();
                break;
            case '2':
                msg.curlUrl();
                std::cin >> url;
                std::cout << "New url set\n";
                break;
            case '3':
                msg.curlDomain();
                std::cin >> url_domain;
                std::cout << "New url_domain set\n";
                break;
            case '4':
                msg.curlBan();
                std::cin >> url_banned;
                std::cout << "New banned_url set\n";
                break;
            case '5':
                std::cout << "Enter new depth: ";
                while (std::cin >> depth) {
                    if (depth[0] == '0' && depth.size() == 1)break;
                    int num = atoi(depth.c_str());
                    if (num)break;
                    std::cout << "Wrong input!\n"
                                 "Enter new depth: ";
                }
                std::cout << "New depth set\n";
                break;
            case 'p':
                this->print(std::cout);
                break;
            case 's':
                return this->clone();
            default:
                std::cout << "!!! WRONG INPUT !!!\n";
                break;
        }
        msg.parUrl();
    }

    return this->clone();
}

//------------------------------------------------------------------------//

bool CUrl::addName() {
    std::vector<std::string> temp_names;
    if (temp_names.empty()) {
        std::ifstream ifs(TEMPLATES_FILE, std::ios::in);
        if (!ifs.is_open()) {
            std::cerr << "ERROR: opening system file \"templates.txt\", aborting\n";
            return false;
        }
        std::string hold;
        size_t index = 0;
        while (getline(ifs, hold)) {
            // 14 = number of parameters
            if (index++ % 14 == 0) {
                std::stringstream ss(hold);
                std::string tmp;
                ss >> tmp >> tmp;
                temp_names.emplace_back(tmp);
            }
        }
        ifs.close();
    }

    // Check the name is unique
    std::cout << "Enter new name: ";
    while (std::cin >> name) {
        if (!std::binary_search(temp_names.begin(), temp_names.end(), name))break;
        std::cout << "Wrong input - template name already taken\n"
                     "Enter new name: ";
    }
    std::cout << "New name set\n";

    return true;
}

//------------------------------------------------------------------------//

std::ostream &CUrl::print(std::ostream &out) const {
    out << "NAME: " << name << std::endl;
    out << "URL: " << url << std::endl;
    out << "URL_DOMAIN: " << url_domain << std::endl;
    out << "URL_BANNED: " << url_banned << std::endl;
    out << "DEPTH: " << depth << std::endl;
    return out;
}

//------------------------------------------------------------------------//

CUrl::CUrl(const std::string &name, const std::string &url, const std::string &urlDomain, const std::string &urlBanned,
           const std::string &Depth)
        : name(name), url(url), url_banned(urlBanned), depth(Depth), url_domain(urlDomain) {}

//------------------------------------------------------------------------//

std::string CUrl::getParameter(size_t num) const {
    switch (num) {
        case 0:
            return name;
        case 1:
            return url;
        case 2:
            return url_banned;
        case 3:
            return depth;
        case 4:
            return url_domain;
        default:
            return "";
    }
}

//------------------------------------------------------------------------//

void CUrl::setParameter(size_t num, const std::string &param) {
    switch (num) {
        case 0:
            name = param;
            return;
        case 1:
            depth = param;
            return;
        case 2:
            url = param;
            return;
        case 3:
            url_banned = param;
            return;
        case 4:
            url_domain = param;
            return;
        default:
            return;
    }
}

//------------------------------------------------------------------------//