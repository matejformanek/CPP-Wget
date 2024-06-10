/**
*@author Matej Formanek <formama9@fit.cvut.cz>
*/


#include <iostream>
#include "CMenu.h"
#include "CSettings.h"
#include "CDownloader.h"
#include "CPromptmsg.h"

//----------------------------------------------------------------------------------------------------------//

CMenu::CMenu() = default;

//----------------------------------------------------------------------------------------------------------//

CMenu::~CMenu() noexcept = default;

//----------------------------------------------------------------------------------------------------------//

bool CMenu::Prompt() {
    CPromptmsg msg;
    msg.menuPrompt();

    // Controls that we have all system files required for work available
    try {
        CSettings settings;
    } catch (const std::logic_error &warning){
        std::cerr << warning.what() << std::endl;
        return false;
    }
    CSettings settings;

    char choice;
    while (std::cin >> choice) {
        switch (choice) {
            case '1': {
                CDownloader default_downloader;
                if (!default_downloader.Prompt())return true;
                break;
            }
            case '2': {
                if (!settings.Prompt())return true;
                break;
            }
            case 'h':
                msg.menuHelp();
                break;
            case 'q':
                std::cout << "\nSHUTTING DOWN" << std::endl;
                return true;
            default:
                std::cout << "\n!!! WRONG INPUT !!!" << std::endl;
        }
        msg.menuPrompt();
    }
    return false;
}

//----------------------------------------------------------------------------------------------------------//