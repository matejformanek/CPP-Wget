/**
*@author Matej Formanek <formama9@fit.cvut.cz>
*/

#pragma once

/**
 * stores all messages for user handling to allow more readable code
 */
class CPromptmsg {
public:
    //------------ MENU --------------//
    void menuPrompt() const;

    void menuHelp() const;

    //--------- SETTINGS ------------//
    void settingsPrompt() const;

    void setpreChanTemp() const;

    void setChangeTemp() const;

    void setDelTemp() const;

    void setListTemp() const;

    //--------- PARAMETER ----------//
    void parUrl() const;

    void parDir() const;

    void parType() const;

    //-------- DOWNLOADER ---------//
    void dowPrompt() const;

    void dowHelp() const;

    void dowSetTemp() const;

    void dowManChan() const;

    void dowArgHelp() const;

    void dowChoose(const int max) const;

    void dowDelete(const int max) const;

    //--------- CURL --------------//
    void curlUrl() const;

    void curlDomain() const;

    void curlBan() const;
};
