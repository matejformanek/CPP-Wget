/**
*@author Matej Formanek <formama9@fit.cvut.cz>
*/

#pragma once


#include <vector>
#include <string>
#include "CTemplate.h"
#include "CSettings.h"


/**
 * Class for reading and processing arguments from user
 * Takes parameters from user and default template and creates new templates for download
 */
class CArguments {
public:
    explicit CArguments(const std::vector<std::string> & arguments);

    ~CArguments();

    /**
     * From parameter creates for each url unique template that will be downloaded
     * Parameters that are missing are taken from DEFAULT template except url which is mandatory
     * @return empty if something went wrong, list of templates for download on success
     */
    std::vector<std::unique_ptr<CTemplate>> TempForDownload();

private:
    /**
     * Decides which type of parameter we changing starts another change function
     * @param param parameter
     * @param value value to be changed to
     * @return true if all the criteria are matched
     */
    bool formatTemplate(const std::string &param,const std::string &value);

    /**
     * Allows user to set new default template for current download
     * @param found true if the new default template was found and viable
     * @return false if the call is requested but not viable template given
     */
    bool defaultTemplate();

    /**
     * Special argument meaning need to open interactive tool for changing and work with templates
     * To be separated does not contain the prefix '-'
     * @return true if the ARG contains special argument
     */
    bool specialArg();

    /**
     * Changes the template for new download
     * @return false if the parameter or value is incorrect
     */
    bool changeType(const std::string &param,const std::string &value);

    /**
     * Changes the template for new download
     * @return false if the parameter or value is incorrect
     */
    bool changeDir(const std::string &param,const std::string &value);

    /**
     * Changes the template for new download
     * @return false if the parameter or value is incorrect
     */
    bool changeUrl(const std::string &param,const std::string &value);

    CSettings settings;
    std::unique_ptr<CTemplate> templ;
    std::vector<std::string> arguments_list;
};
