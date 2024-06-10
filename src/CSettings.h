/**
*@author Matej Formanek <formama9@fit.cvut.cz>
*/

#pragma once

#include <vector>
#include <string>

#include "CMenu.h"
#include "CTemplate.h"


/**
 * Allows setting new templates and changing the default one
 * @parent CMenu
 */
class CSettings : public CMenu{
public:
    CSettings();

    ~CSettings()noexcept override = default;

    bool Prompt() override;

    /**
     * Prompt for deletion of template from list and memory file
     * @return false when could not open system -> shuts of the whole program
     */
    bool deleteTemplprompt();

    /**
     * Deletes template locally and from system file
     * @param name of file for deletion
     * @return false if trouble occurred while working with system files
     */
    bool deleteTempl(const std::string &name);

    /**
     * Creates new Template and adds to memory file -> interactive
     * @return false when could not open system -> shuts of the whole program
     */
    bool createTemplate();

    std::string uniqueName()const;

    /**
     * prints out all current lists
     * Has options for long list or only names
     */
    void listTemplates() const;

    /**
     * Changes the Template -> interactive changing
     * @return false when could not open system -> shuts of the whole program
     */
    bool changeTemplate();

    /**
     * Allows user to set his own chosen template
     * @param change template that is being set
     * @return false if problems with system files
     */
    bool setTemplate(CTemplate &change, size_t pos);

    std::vector<std::unique_ptr<CTemplate>> list_of_templates;
    std::vector<std::string> template_names;
private:
    CPromptmsg msg;
};
