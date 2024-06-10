/**
*@author Matej Formanek <formama9@fit.cvut.cz>
*/

#include <iostream>
#include <fstream>
#include <sstream>

#include "CSettings.h"

#define TEMPLATES_FILE "/home/matej/pa2/sem/examples/templates.txt"
//----------------------------------------------------------------------------------------------------------//

CSettings::CSettings() {
    // Check we have all system files needed for the run of the system
    std::ifstream ifs(TEMPLATES_FILE, std::ios::in);
    if(!ifs.is_open())throw std::logic_error("FATAL ERROR: Could not find system file (templates.txt), shutting down system!");
    std::ifstream ifs_web("/home/matej/pa2/sem/examples/web", std::ios::in);
    if(!ifs_web.is_open())throw std::logic_error("FATAL ERROR: Could not find system file (web), shutting down system!");
    else ifs_web.close();
    std::ifstream ifs_tmp("/home/matej/pa2/sem/examples/tmp", std::ios::in);
    if(!ifs_tmp.is_open())throw std::logic_error("FATAL ERROR: Could not find system file (tmp), shutting down system!");
    else ifs_tmp.close();

    std::string hold;
    size_t index = 0;
    while (getline(ifs, hold)) {
        // 14 = number of parameters -> reads and saves config files
        if (index++ % 14 == 0) {
            std::stringstream ss(hold);
            std::string tmp;
            ss >> tmp >> tmp;
            template_names.push_back(tmp);
            list_of_templates.emplace_back(CTemplate(tmp).clone());
        }
    }
    ifs.close();
}

//----------------------------------------------------------------------------------------------------------//

bool CSettings::Prompt() {
    msg.settingsPrompt();
    char choice;
    while (std::cin >> choice) {
        switch (choice) {
            case '1':
                if(!changeTemplate())return false;
                break;
            case '2':
                if(!deleteTemplprompt())return false;
                break;
            case '3':
                if(!createTemplate())return false;
                break;
            case 'l':
                listTemplates();
                break;
            case 'b':
                std::cout << "\nGOING BACK" << std::endl;
                return true;
            case 'q':
                std::cout << "\nSHUTTING DOWN" << std::endl;
                return false;
            default:
                std::cout << "\n!!! WRONG INPUT !!!" << std::endl;
        }
        msg.settingsPrompt();
    }
    return false;
}

//----------------------------------------------------------------------------------------------------------//

bool CSettings::changeTemplate() {
    msg.setpreChanTemp();

    std::string templ_name;
    CTemplate change;
    size_t pos;

    // prompt to find template name to be changed
    while (true) {
        std::cin >> templ_name;
        if (templ_name == "b")return true;
        else if (templ_name == "l")listTemplates();
        else {
            bool found = false;
            for (size_t it = 0; it < list_of_templates.size(); ++it) {
                if (templ_name == list_of_templates[it]->name) {
                    found = true;
                    change = *list_of_templates[it];
                    pos = it;
                    break;
                }
            }
            if (found)break;
            std::cout << "\n!!! NAME: \"" << templ_name << "\" IS NOT IN THE LIST !!!!\n";
        }
        msg.setpreChanTemp();
    }

    // if correct name of template was given allow setting template
    if(!setTemplate(change,pos))return false;

    return true;
}

//----------------------------------------------------------------------------------------------------------//
// Prompt for changing the template
bool CSettings::setTemplate(CTemplate &change, size_t pos) {
    msg.setChangeTemp();
    char choice;
    while (std::cin >> choice) {
        switch (choice) {
            case '1':
                change.list_of_parameters[0] = change.list_of_parameters[0]->setParameter();
                break;
            case '2':
                change.list_of_parameters[1] = change.list_of_parameters[1]->setParameter();
                break;
            case '3':
                change.list_of_parameters[2] = change.list_of_parameters[2]->setParameter();
                break;
            case 's': {
                list_of_templates.erase(list_of_templates.begin() + pos);
                list_of_templates.insert(list_of_templates.begin() + pos, change.clone());
                std::cout << "CHANGES APPLIED - GOING BACK\n";

                // overwrites the memory file thus saving it
                std::ofstream ofs(TEMPLATES_FILE, std::ios::trunc);
                if(!ofs.is_open()){
                    std::cerr << "FATAL ERROR: Could not open system file (templates.txt), shutting down!\n";
                    return false;
                }
                for (const auto &templ: list_of_templates) {
                    ofs << *templ << std::endl;
                }
                ofs.close();
                return true;
            }
            case 'p':
                std::cout << "\n================ CURRENT TEMPLATE =================\n"
                          << change << "\n================ CURRENT TEMPLATE =================" << std::endl;
                break;
            case 'b':
                std::cout << "\nGOING BACK" << std::endl;
                return true;
            default:
                std::cout << "\n!!! WRONG INPUT !!!" << std::endl;
        }
        msg.setChangeTemp();
    }

    return true;
}

//----------------------------------------------------------------------------------------------------------//

bool CSettings::deleteTemplprompt() {
    msg.setDelTemp();

    std::string templ_name;
    // Prompt to find name that shall be deleted
    while (true) {
        std::cin >> templ_name;
        if (templ_name == "b")return true;
        else if (templ_name == "l")listTemplates();
        else if(templ_name == "DEFAULT")std::cout << "Can not delete DEFAULT template!\n";
        else {
            bool found = false;
            for (const std::string &names: template_names) {
                if (templ_name == names) {
                    found = true;
                    break;
                }
            }
            if (found)break;
            std::cout << "\n!!! NAME: \"" << templ_name << "\" IS NOT IN THE LIST !!!!\n";
        }
        msg.setDelTemp();
    }

    if(!deleteTempl(templ_name))return false;

    return true;
}

//----------------------------------------------------------------------------------------------------------//

bool CSettings::deleteTempl(const std::string &name) {
    size_t index = 0;
    for (const std::string &templates: template_names) {
        if (name == templates) {
            list_of_templates.erase(list_of_templates.begin() + index);
            template_names.erase(template_names.begin() + index);
            std::string hold;

            // overwrites the memory file thus saving it
            std::ofstream ofs(TEMPLATES_FILE, std::ios::trunc);
            if(!ofs.is_open()){
                std::cerr << "FATAL ERROR: Could not open system file (templates.txt), shutting down!\n";
                return false;
            }
            for (const auto &templ: list_of_templates) {
                ofs << *templ << std::endl;
            }

            ofs.close();
            std::cout << "Template \"" << name << "\" deleted\n";
            return true;
        }
        ++index;
    }

    return true;
}


//----------------------------------------------------------------------------------------------------------//

bool CSettings::createTemplate() {
    std::cout << "\nCREATE NEW TEMPLATE";
    CTemplate newTemplate(*list_of_templates[0]);

    // make sure every name given by default to new template is unique
    std::string unique_name = uniqueName();

    // sequence of prompts for creation of new template
    newTemplate.list_of_parameters[0]->setParameter(0, unique_name);
    newTemplate.list_of_parameters[0] = newTemplate.list_of_parameters[0]->setParameter();
    newTemplate.list_of_parameters[1] = newTemplate.list_of_parameters[1]->setParameter();
    newTemplate.list_of_parameters[2] = newTemplate.list_of_parameters[2]->setParameter();

    // save template and rewrite system file
    list_of_templates.emplace_back(newTemplate.clone());
    template_names.emplace_back(newTemplate.list_of_parameters[0]->getParameter(0));
    std::ofstream ofs(TEMPLATES_FILE, std::ios::trunc);
    if(!ofs.is_open()){
        std::cerr << "FATAL ERROR: Could not open system file (templates.txt), shutting down!\n";
        return false;
    }
    for (const auto &templ: list_of_templates) {
        ofs << *templ << std::endl;
    }

    std::cout << "New template set\n";
    return true;
}

//----------------------------------------------------------------------------------------------------------//

std::string CSettings::uniqueName() const {
    std::string unique_name;
    while (true) {
        int random = rand() % 1000;
        unique_name = "NEW_TEMPL" + std::to_string(random);
        bool found = false;
        for(const std::string & temp_name : template_names){
            if(unique_name == temp_name){
                found = true;
                break;
            }
        }
        if(!found)break;
    }

    return unique_name;
}

//----------------------------------------------------------------------------------------------------------//

void CSettings::listTemplates() const {
    msg.setListTemp();
    char choice;
    while (true) {
        std::cin >> choice;
        if (choice == 'b') {
            std::cout << "GOING BACK\n";
            return;
        }
        if (choice == '1' || choice == '2')break;
        else msg.setListTemp();
    }

    std::cout << "========== LIST OF TEMPLATES ==========\n";
    if (choice == '1') {
        for (const std::string &name: template_names) {
            std::cout << name << std::endl;
        }
    } else {
        for (const auto &templates: list_of_templates) {
            std::cout << *templates << std::endl;
        }
    }
    std::cout << "========== LIST OF TEMPLATES ==========\n";
}

//----------------------------------------------------------------------------------------------------------//