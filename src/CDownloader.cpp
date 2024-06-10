/**
*@author Matej Formanek <formama9@fit.cvut.cz>
*/


#include "CDownloader.h"
#include "CDownload.h"

//----------------------------------------------------------------------------------------------------------//

CDownloader::CDownloader() {
    templ = settings.list_of_templates[0]->clone();
}

//----------------------------------------------------------------------------------------------------------//

CDownloader::~CDownloader() noexcept = default;

//----------------------------------------------------------------------------------------------------------//

bool CDownloader::Prompt() {
    msg.dowPrompt();
    char choice;
    while (std::cin >> choice) {
        switch (choice) {
            case '1': {
                CDownload dld(templates_download);
                if (!dld.Prompt())return false;
                return true;
            }
            case '2': {
                setTempl();
                break;
            }
            case '3': {
                deleteTempl();
                break;
            }
            case '4': {
                if (!settings.Prompt())return false;
                break;
            }
            case 'l':
                std::cout << "========== LIST OF TEMPLATES ==========\n";
                for (const auto &temp: templates_download) {
                    std::cout << *temp << std::endl;
                }
                std::cout << "========== LIST OF TEMPLATES ==========\n";
                break;
            case 'h':
                msg.dowHelp();
                break;
            case 'q':
                std::cout << "\nSHUTTING DOWN" << std::endl;
                return false;
            default:
                std::cout << "\n!!! WRONG INPUT !!!" << std::endl;
        }
        msg.dowPrompt();
    }
    return false;
}

//----------------------------------------------------------------------------------------------------------//

void CDownloader::setTempl() {
    msg.dowSetTemp();
    char choice;
    while (std::cin >> choice) {
        switch (choice) {
            case '1':
                std::cout << "Template added to the staged list of templates\n";
                templates_download.emplace_back(templ->clone());
                break;
            case '2':
                chooseTempl();
                break;
            case '3':
                manualChange();
                break;
            case 'p':
                std::cout << "\n================ CURRENT TEMPLATE =================\n"
                          << *templ << "\n================ CURRENT TEMPLATE =================" << std::endl;
                break;
            case 'b':
                std::cout << "GOING BACK\n";
                return;
            default:
                std::cout << "\n!!! WRONG INPUT !!!" << std::endl;
        }
        msg.dowSetTemp();
    }
}

//----------------------------------------------------------------------------------------------------------//

void CDownloader::chooseTempl() {
    msg.dowChoose(settings.template_names.size());
    char choice;
    while (std::cin >> choice) {
        if (choice == 'l') {
            std::cout << "========== LIST OF TEMPLATES ==========\n";
            for (const auto &temp: settings.list_of_templates) {
                std::cout << *temp << std::endl;
            }
            std::cout << "========== LIST OF TEMPLATES ==========\n";
        } else if (choice == 'q') {
            std::cout << "GOING BACK\n";
            return;
        } else if ((int) choice - 49 >= (int) settings.list_of_templates.size() || (int) choice - 49 < 0) {
            std::cout << "!!! NUMBER OUT OF BOUNDS !!!\n";
        } else {
            templ = settings.list_of_templates[(int) choice - 49]->clone();
            std::cout << "TEMPLATE COPIED\n";
            return;
        }

        msg.dowChoose(settings.template_names.size());
    }
}

//----------------------------------------------------------------------------------------------------------//

void CDownloader::manualChange() {
    msg.dowManChan();
    char choice;
    while (std::cin >> choice) {
        switch (choice) {
            case '1':
                templ->list_of_parameters[0] = templ->list_of_parameters[0]->setParameter();
                break;
            case '2':
                templ->list_of_parameters[1] = templ->list_of_parameters[1]->setParameter();
                break;
            case '3':
                templ->list_of_parameters[2] = templ->list_of_parameters[2]->setParameter();
                break;
            case 's': {
                std::cout << "CHANGES APPLIED - GOING BACK\n";
                return;
            }
            case 'p':
                std::cout << "\n================ CURRENT TEMPLATE =================\n"
                          << *templ << "\n================ CURRENT TEMPLATE =================" << std::endl;
                break;
            default:
                std::cout << "\n!!! WRONG INPUT !!!" << std::endl;
        }
        msg.dowManChan();
    }
}

//----------------------------------------------------------------------------------------------------------//

void CDownloader::deleteTempl() {
    if (templates_download.empty()) {
        std::cout << "Staged list of templates is empty\n"
                     "Returning back\n";
        return;
    }

    msg.dowDelete(templates_download.size());

    char choice;
    while (std::cin >> choice) {
        if (choice == 'l') {
            std::cout << "========== LIST OF TEMPLATES ==========\n";
            for (const auto &temp: templates_download) {
                std::cout << *temp << std::endl;
            }
            std::cout << "========== LIST OF TEMPLATES ==========\n";
        } else if (choice == 'q') {
            std::cout << "GOING BACK\n";
            return;
        } else if ((int) choice - 49 >= (int) templates_download.size() || (int) choice - 49 < 0) {
            std::cout << "!!! NUMBER OUT OF BOUNDS !!!\n";
        } else {
            templates_download.erase(templates_download.begin() + (int) choice - 49);
            std::cout << "TEMPLATE ERASED\n";
        }

        if (templates_download.empty()) {
            std::cout << "List is empty, going back\n";
            return;
        }

        msg.dowDelete(templates_download.size());
    }
}

//----------------------------------------------------------------------------------------------------------//

bool CDownloader::quickDownload(std::vector<std::unique_ptr<CTemplate>> &download_list) {
    CDownload dld(download_list);
    if (!dld.Prompt())return false;

    return true;
}

//----------------------------------------------------------------------------------------------------------//