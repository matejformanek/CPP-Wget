/**
*@author Matej Formanek <formama9@fit.cvut.cz>
*/

#include <iostream>

#include "CPromptmsg.h"

void CPromptmsg::menuPrompt() const {
    std::cout << "\nMENU\n"
                 "Press '1' to select \"Downloader\"\n"
                 "Press '2' to go to \"Settings\"\n"
                 "Press 'h' for info and help\n"
                 "Press 'q' to quit\n"
                 "input> ";
}

void CPromptmsg::menuHelp() const {
    std::cout << "\nHELP\n"
                 "1) Lets you quickly download websites from premade templates or manualy made one\n"
                 "2) Settings let you store, create and change templates\n"
                 "You can put URL into argument when starting the program to perform quick download with your url in Default template"
              << std::endl;
}

void CPromptmsg::settingsPrompt() const {
    std::cout << "\nSETTINGS\n"
                 "Press '1' to \"Change template\"\n"
                 "Press '2' to \"Delete template\"\n"
                 "Press '3' to \"Create template\"\n"
                 "Press 'l' to \"Print list of templates\"\n"
                 "Press 'b' to return Back\n"
                 "Press 'q' to quit\n"
                 "input> ";
}

void CPromptmsg::setpreChanTemp() const {
    std::cout << "\nCHANGE TEMPLATE\n"
                 "Enter: name of template to be changed\n"
                 "Enter: 'l' to see list of templates\n"
                 "Enter: 'b' to abort and get back to Settings\n"
                 "input> ";
}


void CPromptmsg::setChangeTemp() const {
    std::cout << "\nCHANGE TEMPLATE\n"
                 "Press '1' to change URL settings\n"
                 "Press '2' to change Directory settings\n"
                 "Press '3' to change Type to download settings\n"
                 "Press 's' to save current template and quit\n"
                 "Press 'p' to show current template\n"
                 "Press 'b' to get back (Abort)\n"
                 "input> ";
}

void CPromptmsg::setDelTemp() const {
    std::cout << "DELETE TEMPLATE\n"
                 "Enter: name of a template to be deleted\n"
                 "Enter: 'l' for list of templates\n"
                 "Enter: 'b' to abort and get back to Settings\n"
                 "input> ";
}

void CPromptmsg::setListTemp() const {
    std::cout << "LIST TEMPLATES\n"
                 "Press '1' for \"List of template names (short list)\"\n"
                 "Press '2' for \"List of templates with their atributes\"\n"
                 "Press 'b' to get back\n"
                 "input> ";
}

void CPromptmsg::parUrl() const {
    std::cout << "\nSET URL\n"
                 "Press '1' to change NAME\n"
                 "Press '2' to change URL\n"
                 "Press '3' to change URL_DOMAIN\n"
                 "Press '4' to change BANNED_URL\n"
                 "Press '5' to change DEPTH\n"
                 "Press 'p' to print current state\n"
                 "Press 's' to save and quit\n"
                 "input> ";
}

void CPromptmsg::parDir() const {
    std::cout << "\nSET DIRECTORY\n"
                 "Press '1' to change DIR_HTML\n"
                 "Press '2' to change DIR_CSS\n"
                 "Press '3' to change DIR_JS\n"
                 "Press '4' to change DIR_IMG\n"
                 "Press '5' to change DIR_DEFAULT\n"
                 "Press 'p' to print current state\n"
                 "Press 's' to save and quit\n"
                 "input> ";
}

void CPromptmsg::parType() const {
    std::cout << "\nSET TYPE\n"
                 "Press '1' to change HTML\n"
                 "Press '2' to change CSS\n"
                 "Press '3' to change JS\n"
                 "Press '4' to change IMG\n"
                 "Press 'p' to print current state\n"
                 "Press 's' to save and quit\n"
                 "input> ";
}

void CPromptmsg::dowPrompt() const {
    std::cout << "\nDOWNLOADER\n"
                 "Press '1' for \"Download from list of templates\"\n"
                 "Press '2' to \"Create/Add template to list of templates for download\"\n"
                 "Press '3' to \"Delete template from list of templates for download\"\n"
                 "Press '4' to go to \"Settings\"\n"
                 "Press 'l' to see current staged list of templates\n"
                 "Press 'h' for info and help\n"
                 "Press 'q' to quit\n"
                 "input> ";
}

void CPromptmsg::dowHelp() const {
    std::cout << "HELP\n"
                 "1) Starts downloading with parameters given in the \"list of templates\"\n"
                 "2) Create or Add from premade templates to the list that will be later downloaded\n"
                 "3) Delete from your staged templates\n"
                 "4) Settings let you store, create and change templates" << std::endl;
}

void CPromptmsg::dowSetTemp() const {
    std::cout << "\nSET TEMPLATE TO STAGE AREA\n"
                 "Press '1' to Add template to the staged list\n"
                 "Press '2' to choose template from your premade templates\n"
                 "Press '3' to manually change the template\n"
                 "Press 'p' to print your current template\n"
                 "Press 'b' to go back\n"
                 "input> ";
}

void CPromptmsg::dowManChan() const {
    std::cout << "\nCHANGE TEMPLATE\n"
                 "Press '1' to change URL settings\n"
                 "Press '2' to change Directory settings\n"
                 "Press '3' to change Type to download settings\n"
                 "Press 'p' to show current template\n"
                 "Press 's' to save current template and quit\n"
                 "input> ";
}

void CPromptmsg::dowArgHelp() const {
    std::cout << "QUICK DOWNLOAD HELP\n"
                 "To change whether or not to download type write:\n"
                 "\"-h\"(html),\"-c\"(css),\"-j\"(jss),\"-i\"(img) + value [true/false]\n"
                 "Add \"d\" in front (-dh) to change directory u download into + value \"path\"\n"
                 "\"-dep\" + num changes depth search\n"
                 "\"-ban\" + url(url1,url2,...) change banned urls\n"
                 "\"-dom\" + url(url1,url2,...) change domain urls\n"
                 "After all param write \"URL\" - mandatory\n"
                 "After URL u can write new Templates\n"
                 "write \"-keep\" to keep previous template settings\n"
                 "Additional functions for work with templates = set, create, del, list\n";
}


void CPromptmsg::dowChoose(const int max)const {
    std::cout << "\nCHOOSE TEMPLATE\n"
                 "Enter: number from 1 to " << max
              << " to choose template for copy\n"
                 "Press 'l' to see the list of premade templates\n"
                 "Press 'q' to quit\n"
                 "input> ";
}


void CPromptmsg::dowDelete(const int max)const {
    std::cout << "\nDELETE STAGED TEMPLATE\n"
                 "Select position to be deleted starting from index 1 to " << max << std::endl
              << "Press 'l' to see list of staged templates\n"
                 "Press 'q' to quit\n"
                 "input> ";
}

void CPromptmsg::curlUrl() const {
    std::cout << "To make the field \"empty\" write EMPTY\n"
                 "Enter new url: ";
}

void CPromptmsg::curlDomain() const {
    std::cout << "To make the field \"empty\" write EMPTY\n"
                 "For multi urls write: url1,url2,url3 = no WS with ',' separator\n"
                 "Enter new url_domain: ";
}

void CPromptmsg::curlBan() const {
    std::cout << "To make the field \"empty\" write EMPTY\n"
                 "For multi urls write: url1,url2,url3 = no WS with ',' separator\n"
                 "Enter new banned_url: ";
}