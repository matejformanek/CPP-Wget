/**
*@author Matej Formanek <formama9@fit.cvut.cz>
*/

#pragma once

#include "CMenu.h"
#include "CSettings.h"
#include "CTemplate.h"

/**
 * Saves templates want to download and than activates
 * functions and methods to execute download
 * @child CDownload
 */
class CDownloader : public CMenu {
public:
    CDownloader();

    ~CDownloader() noexcept override;

    bool Prompt() override;

    /**
     * Gets already made "templates_download" from user arguments starts download
     * @param download_list equal to templates_download
     */
    static bool quickDownload(std::vector<std::unique_ptr<CTemplate>> &download_list);

protected:
    std::vector<std::unique_ptr<CTemplate>> templates_download = {};
private:
    /**
     * Deletes Template from staged list of templates to be downloaded (@var templates_download)
     */
    void deleteTempl();

    /**
     * Creates or adds new templates to the stage list
     */
    void setTempl();

    /**
     * Support function in @function setTempl
     * Option for fully customizable manual template
     */
    void manualChange();


    /**
     * Support function in @function setTempl
     * Lets you choose template from premade templates remembered in your memory file
     */
    void chooseTempl();

    CPromptmsg msg;
    CSettings settings = {};
    std::unique_ptr<CTemplate> templ = nullptr;
};
