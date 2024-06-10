/**
*@author Matej Formanek <formama9@fit.cvut.cz>
*/

#pragma once

#include "CPromptmsg.h"

/**
 * Load menu first thing booted, allows navigating through the whole application
 * @childs CSettings and CDownloader
 */
class CMenu {
public:
    CMenu();

    virtual ~CMenu() noexcept;

    /**
     * Prompt text for user to interactively choose what he needs
     */
    virtual bool Prompt();
private:
};