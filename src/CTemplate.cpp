/**
*@author Matej Formanek <formama9@fit.cvut.cz>
*/

#include <fstream>
#include <sstream>
#include <filesystem>

#include "CTemplate.h"
#include "CUrl.h"
#include "CDirectory.h"
#include "CType.h"

#define TEMPLATES_FILE "/home/matej/pa2/sem/examples/templates.txt"

//----------------------------------------------------------------------------------------------------------//

CTemplate::CTemplate(const std::string &templ_name) {
    // download and create template from chosen template
    std::ifstream ifs(TEMPLATES_FILE, std::ios::in);
    std::string hold_line;
    std::vector<std::string> parameter;
    std::string place_holder;
    bool found = false;

    // Read parameters from system file and save them inside program
    for (size_t param_id = 0; param_id < 14; ++param_id) {
        if (!getline(ifs, hold_line))break;
        std::stringstream st(hold_line);
        std::string element;
        st >> place_holder >> element;
        if (!found && element == templ_name) {
            this->name = templ_name;
            found = true;
        } else if (!found) {
            --param_id;
            continue;
        }
        if (param_id > 4 && param_id < 10)element = makePath(element);
        parameter.push_back(element);

        if (param_id == 4) { // CURL
            list_of_parameters.emplace_back(
                    CUrl(parameter[0], parameter[1], parameter[2], parameter[3], parameter[4]).clone());
            parameter.clear();
        } else if (param_id == 9) { // CDIRECTORY
            list_of_parameters.emplace_back(
                    CDirectory(parameter[4], parameter[0], parameter[1], parameter[2],
                               parameter[3]).clone());
            parameter.clear(); // CTYPE
        } else if (param_id == 13)
            list_of_parameters.emplace_back(
                    CType(parameter[0], parameter[1], parameter[2], parameter[3]).clone());
    }
    ifs.close();
}

//----------------------------------------------------------------------------------------------------------//

std::ostream &operator<<(std::ostream &out, const CTemplate &src) {
    for (const auto &params: src.list_of_parameters) {
        params->print(out);
    }
    return out;
}

//----------------------------------------------------------------------------------------------------------//

std::unique_ptr<CTemplate> CTemplate::clone() const {
    return std::make_unique<CTemplate>(*this);
}

//----------------------------------------------------------------------------------------------------------//

CTemplate::CTemplate(const CTemplate &src) {
    name = src.name;
    for (const auto &params: src.list_of_parameters) {
        list_of_parameters.emplace_back(params->clone());
    }
}

//----------------------------------------------------------------------------------------------------------//

CTemplate &CTemplate::operator=(const CTemplate &src) {
    if (this == &src)return *this;
    name = src.name;
    for (const auto &params: src.list_of_parameters) {
        list_of_parameters.emplace_back(params->clone());
    }
    return *this;
}

//----------------------------------------------------------------------------------------------------------//
// converts directory to absolute path
std::string CTemplate::makePath(std::string dir) {
    if (dir != "EMPTY") {
        if (dir[0] == '.') {
            if (dir[1] == '/') {
                dir.erase(0, 1);
                dir = std::filesystem::current_path().string() + dir;
            } else std::filesystem::current_path().string() + "/"; // invalid path -> give current dir
        } else if (dir[0] == '/') {
        } else dir = std::filesystem::current_path().string() + "/" + dir;
    }
    if (dir[dir.size() - 1] != '/')dir += "/";
    if(dir != "EMPTY/")std::filesystem::create_directory(dir);

    return dir;
}

//----------------------------------------------------------------------------------------------------------//

CTemplate::CTemplate() = default;

//----------------------------------------------------------------------------------------------------------//

CTemplate::~CTemplate() = default;

//----------------------------------------------------------------------------------------------------------//