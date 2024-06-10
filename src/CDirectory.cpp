/**
*@author Matej Formanek <formama9@fit.cvut.cz>
*/

#include <memory>
#include <filesystem>
#include "CDirectory.h"
#include "CPromptmsg.h"

//----------------------------------------------------------------------------//

CDirectory::CDirectory(const std::string &defaultDir, const std::string &htmlDir, const std::string &cssDir,
                       const std::string &jsDir, const std::string &imgDir)
        : default_dir(defaultDir), html_dir(htmlDir), css_dir(cssDir),
          js_dir(jsDir), img_dir(imgDir) {
    if(default_dir[default_dir.size() - 1] != '/')default_dir += "/";
    if(html_dir[html_dir.size() - 1] != '/')html_dir += "/";
    if(css_dir[css_dir.size() - 1] != '/')css_dir += "/";
    if(js_dir[js_dir.size() - 1] != '/')js_dir += "/";
    if(img_dir[img_dir.size() - 1] != '/')img_dir += "/";
}

//----------------------------------------------------------------------------//

std::unique_ptr<CParameter> CDirectory::clone() const {
    return std::make_unique<CDirectory>(*this);
}

//----------------------------------------------------------------------------//

std::unique_ptr<CParameter> CDirectory::setParameter() {
    CPromptmsg msg;
    msg.parDir();
    char choice;
    while (std::cin >> choice) {
        switch (choice) {
            case '1':
                changeType("html", html_dir);
                break;
            case '2':
                changeType("css", css_dir);
                break;
            case '3':
                changeType("js", js_dir);
                break;
            case '4':
                changeType("img", img_dir);
                break;
            case '5':
                changeDef();
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
        msg.parDir();
    }

    return this->clone();
}

//----------------------------------------------------------------------------//

void CDirectory::changeType(const std::string &type, std::string &directory) {
    std::cout << "To make the field \"empty\" write EMPTY else write path \"/path/\"\n"
                 "Enter new " << type << "_dir: ";
    while (std::cin >> directory) {
        if (directory == "EMPTY")break;
        if (directory[0] == '.') {
            if (directory[1] == '/') {
                directory.erase(0, 1);
                directory = std::filesystem::current_path().string() + directory;
                break;
            } else
                std::cout << "Wrong input!\n"
                             "To make the field \"empty\" write EMPTY else write path \"/path/\"\n"
                             "Enter new " << type << "_dir: ";
        } else if (directory[0] == '/') {
            break;
        } else {
            directory = std::filesystem::current_path().string() + "/" + directory;
            break;
        }
    }

    if (directory[directory.size() - 1] != '/')directory += "/";
    std::filesystem::create_directory(directory);

    std::cout << "New " << type << "_dir set\n";
}

//----------------------------------------------------------------------------//

void CDirectory::changeDef() {
    std::cout << "To set, write path \"/path/\"\n"
                 "Enter new dir_default: ";
    while (std::cin >> default_dir) {
        if (default_dir[0] == '.') {
            if (default_dir[1] == '/') {
                default_dir.erase(0, 1);
                default_dir = std::filesystem::current_path().string() + default_dir;
                break;
            } else
                std::cout << "Wrong input!\n"
                             "To set, write path \"/path/\"\n"
                             "Enter new dir_default: ";
        } else if (default_dir[0] == '/') {
            break;
        } else {
            default_dir = std::filesystem::current_path().string() + "/" + default_dir;
            break;
        }
    }

    if (default_dir[default_dir.size() - 1] != '/')default_dir += "/";
    std::filesystem::create_directory(default_dir);

    std::cout << "New dir_default set\n";
}

//----------------------------------------------------------------------------//

std::ostream &CDirectory::print(std::ostream &out) const {
    out << "DIR_HTML: " << html_dir << std::endl;
    out << "DIR_CSS: " << css_dir << std::endl;
    out << "DIR_JS: " << js_dir << std::endl;
    out << "DIR_IMG: " << img_dir << std::endl;
    out << "DIR_DEFAULT: " << default_dir << std::endl;
    return out;
}

//----------------------------------------------------------------------------//

std::string CDirectory::getParameter(size_t num) const {
    switch (num) {
        case 0:
            return default_dir;
        case 1:
            return html_dir;
        case 2:
            return css_dir;
        case 3:
            return js_dir;
        case 4:
            return img_dir;
        default:
            return default_dir;
    }
}

//----------------------------------------------------------------------------//

void CDirectory::setParameter(size_t num, const std::string &param) {
    switch (num) {
        case 0:
            html_dir = param;
            break;
        case 1:
            css_dir = param;
            break;
        case 2:
            js_dir = param;
            break;
        case 3:
            img_dir = param;
            break;
        case 4:
            default_dir = param;
            break;
        default:
            break;
    }
}

//----------------------------------------------------------------------------//