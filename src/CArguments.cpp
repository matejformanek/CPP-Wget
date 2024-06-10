/**
*@author Matej Formanek <formama9@fit.cvut.cz>
*/

#include <filesystem>

#include "CArguments.h"
#include "CSettings.h"

//----------------------------------------------------------------------------------------------------//

CArguments::CArguments(const std::vector<std::string> &arguments) {
    try {
        CSettings set;
    } catch (const std::logic_error &warning) {
        std::cerr << warning.what() << std::endl;
        throw std::logic_error("Missing system file");
    }
    templ = settings.list_of_templates[0]->clone();
    arguments_list = arguments;
}

//----------------------------------------------------------------------------------------------------//

CArguments::~CArguments() = default;

//----------------------------------------------------------------------------------------------------//

std::vector<std::unique_ptr<CTemplate>> CArguments::TempForDownload() {
    if (!defaultTemplate())return {};

    if (specialArg())return {};

    std::vector<std::unique_ptr<CTemplate>> downloadList;

    size_t name_id = 0;
    auto it = arguments_list.begin();
    std::unique_ptr<CTemplate> default_templ = templ->clone();
    for (; it < arguments_list.end(); ++it) {
        //Saves template to be downloaded, sets back template to default
        if (it->c_str()[0] != '-') {
            std::string name = "Q_" + std::to_string(name_id++) + "_D";
            templ->list_of_parameters[0]->setParameter(0, name);
            templ->list_of_parameters[0]->setParameter(2, *it);
            downloadList.emplace_back(templ->clone());
            if ((it + 1) != arguments_list.end() && *(it + 1) == "-keep") {
                ++it;
                continue;
            }
            templ = default_templ->clone();
            continue;
        }
        // Controls that when we get a parameter we also get a value
        if (it + 1 == arguments_list.end()) {
            std::cerr << "Parameter: " << *it << " is missing value, Aborting!\n";
            return {};
        }
            // Controls that the value and parameter are both correct
        else if (!formatTemplate(*it, *(it + 1))) {
            std::cerr << "Parameter: " << *it << " or Value: " << *(it + 1)
                      << " is incorrect try \"help\", Aborting!\n";
            return {};
        }
        ++it; // because it + 1 is value + parameter
    }

    return downloadList;
}

//----------------------------------------------------------------------------------------------------//

bool CArguments::defaultTemplate() {
    if (arguments_list.front() == "temp") {
        arguments_list.erase(arguments_list.begin());
        if (arguments_list.empty()) {
            std::cerr << "Missing template to be set as default!\n";
            return false;
        }

        size_t pos = 0;
        bool found = false;
        for (const auto &temp_name: settings.template_names) {
            if (temp_name == arguments_list.front()) {
                templ = settings.list_of_templates[pos]->clone();
                found = true;
                break;
            }
            pos++;
        }

        if (!found) {
            std::cerr << "Could not find template: " << arguments_list.front() << " to be set as default\n";
            return false;
        } else arguments_list.erase(arguments_list.begin());
    }

    return true;
}

//----------------------------------------------------------------------------------------------------//

bool CArguments::specialArg() {
    if (arguments_list.front() == "help") {
        CPromptmsg msg;
        msg.dowArgHelp();
        return true;
    } else if (arguments_list.front() == "set") {
        settings.changeTemplate();
        return true;
    } else if (arguments_list.front() == "del") {
        settings.deleteTemplprompt();
        return true;
    } else if (arguments_list.front() == "list") {
        settings.listTemplates();
        return true;
    } else if (arguments_list.front() == "create") {
        settings.createTemplate();
        return true;
    }

    return false;
}

//----------------------------------------------------------------------------------------------------//

bool CArguments::formatTemplate(const std::string &param, const std::string &value) {
    switch (param.length()) {
        // CDowtype -h -c -j -i
        case 2:
            if (value != "true" && value != "false")return false;
            if (!changeType(param, value))return false;
            return true;
            // CDowdir -dh -dc -dj -di -dd
        case 3:
            if (!changeDir(param, value))return false;
            return true;
            // CDowurl -ban -dom -dep
        case 4:
            if (!changeUrl(param, value))return false;
            return true;
        default:
            return false;
    }
}

//----------------------------------------------------------------------------------------------------//

bool CArguments::changeType(const std::string &param, const std::string &value) {
    if (param == "-h") {
        templ->list_of_parameters[2]->setParameter(0, value);
    } else if (param == "-c") {
        templ->list_of_parameters[2]->setParameter(1, value);
    } else if (param == "-j") {
        templ->list_of_parameters[2]->setParameter(2, value);
    } else if (param == "-i") {
        templ->list_of_parameters[2]->setParameter(3, value);
    } else return false;
    return true;
}

//----------------------------------------------------------------------------------------------------//

bool CArguments::changeDir(const std::string &param, const std::string &value) {
    // Control of value + reformating
    if (param == "-dd" && value == "EMPTY")return false;
    std::string reformat_value = value;
    // Make the path absolute
    if (reformat_value != "EMPTY") {
        if (reformat_value[0] == '.') {
            if (reformat_value[1] == '/') {
                reformat_value.erase(0, 1);
                reformat_value = std::filesystem::current_path().string() + reformat_value;
            } else return false;
        } else if (reformat_value[0] == '/') {
        } else reformat_value = std::filesystem::current_path().string() + "/" + reformat_value;
    }
    if (reformat_value[reformat_value.size() - 1] != '/')reformat_value += "/";
    if (reformat_value != "EMPTY/")std::filesystem::create_directory(reformat_value);

    if (param == "-dh") {
        templ->list_of_parameters[1]->setParameter(0, reformat_value);
    } else if (param == "-dc") {
        templ->list_of_parameters[1]->setParameter(1, reformat_value);
    } else if (param == "-dj") {
        templ->list_of_parameters[1]->setParameter(2, reformat_value);
    } else if (param == "-di") {
        templ->list_of_parameters[1]->setParameter(3, reformat_value);
    } else if (param == "-dd") {
        templ->list_of_parameters[1]->setParameter(4, reformat_value);
    } else return false;

    return true;
}

//----------------------------------------------------------------------------------------------------//

bool CArguments::changeUrl(const std::string &param, const std::string &value) {
    if (param == "-dep") {
        // check its viable number
        if (value[0] == '0' && value.size() == 1) {
            templ->list_of_parameters[0]->setParameter(1, value);
            return true;
        }
        if (!atoi(value.c_str()))return false;

        templ->list_of_parameters[0]->setParameter(1, value);
        return true;
    }

    if (param == "-ban") {
        templ->list_of_parameters[0]->setParameter(3, value);
    } else if (param == "-dom") {
        templ->list_of_parameters[0]->setParameter(4, value);
    } else return false;
    return true;
}

//----------------------------------------------------------------------------------------------------//