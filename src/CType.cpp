/**
*@author Matej Formanek <formama9@fit.cvut.cz>
*/

#include "CType.h"
#include "CPromptmsg.h"

//-------------------------------------------------------------------------------//

std::unique_ptr<CParameter> CType::clone() const {
    return std::make_unique<CType>(*this);
}

//-------------------------------------------------------------------------------//

std::unique_ptr<CParameter> CType::setParameter() {
    CPromptmsg msg;
    msg.parType();
    char choice;

    while (std::cin >> choice) {
        switch (choice) {
            case '1':
                std::cout << "Enter html state [true/false]: ";
                while (std::cin >> html_type) {
                    if (html_type == "true" || html_type == "false")break;
                    std::cout << "Wrong input\n"
                                 "Enter html state [true/false]: ";
                }
                std::cout << "New html state set\n";
                break;
            case '2':
                std::cout << "Enter css state [true/false]: ";
                while (std::cin >> css_type) {
                    if (css_type == "true" || css_type == "false")break;
                    std::cout << "Wrong input\n"
                                 "Enter css state [true/false]: ";
                }
                std::cout << "New css state set\n";
                break;
            case '3':
                std::cout << "Enter new js state [true/false]: ";
                while (std::cin >> js_type) {
                    if (js_type == "true" || js_type == "false")break;
                    std::cout << "Wrong input\n"
                                 "Enter js state [true/false]: ";
                }
                std::cout << "New js state set\n";
                break;
            case '4':
                std::cout << "Enter new img state [true/false]: ";
                while (std::cin >> img_type) {
                    if (img_type == "true" || img_type == "false")break;
                    std::cout << "Wrong input\n"
                                 "Enter img state [true/false]: ";
                }
                std::cout << "New img state set\n";
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
        msg.parType();
    }

    return this->clone();
}

//-------------------------------------------------------------------------------//

std::ostream &CType::print(std::ostream &out) const {
    out << "TYPE_HTML: " << html_type << std::endl;
    out << "TYPE_CSS: " << css_type << std::endl;
    out << "TYPE_JS: " << js_type << std::endl;
    out << "TYPE_IMG: " << img_type;
    return out;
}

//-------------------------------------------------------------------------------//

CType::CType(const std::string &htmlType, const std::string &cssType, const std::string &jsType,
             const std::string &imgType) {
    if (htmlType == "0" || htmlType == "false")html_type = "false";
    else html_type = "true";
    if (cssType == "0" || cssType == "false")css_type = "false";
    else css_type = "true";
    if (jsType == "0" || jsType == "false")js_type = "false";
    else js_type = "true";
    if (imgType == "0" || imgType == "false")img_type = "false";
    else img_type = "true";
}

//-------------------------------------------------------------------------------//

std::string CType::getParameter(size_t num) const {
    switch (num) {
        case 0:
            return html_type;
        case 1:
            return css_type;
        case 2:
            return js_type;
        case 3:
            return img_type;
        default:
            return "";
    }
}

//-------------------------------------------------------------------------------//

void CType::setParameter(size_t num, const std::string &param) {
    switch (num) {
        case 0:
            html_type = param;
            break;
        case 1:
            css_type = param;
            break;
        case 2:
            js_type = param;
            break;
        case 3:
            img_type = param;
            break;
        default:
            break;
    }
}

//-------------------------------------------------------------------------------//