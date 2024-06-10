/**
*@author Matej Formanek <formama9@fit.cvut.cz>
*/

#pragma once

#include <memory>
#include <queue>

#include "CTemplate.h"

/**
 * Parent abstract class to all child classes that download specific type from website
 * @child CDowhtml, CDowcss, CDowjs, CDowimg
 */
class CDowtype{
public:
    virtual ~CDowtype() noexcept = default;

    virtual std::unique_ptr<CDowtype> clone()const = 0;

    /**
     * Finds all links of specific type and saves them inside the queue
     * @var links = vector of links for the same type
     * @return false if can not open system file
     */
    virtual bool getLinks() = 0;

    /**
     * Downloads specific type into its file
     */
    virtual void donwloadType() = 0;

protected:
    std::queue<std::string> links;
    std::shared_ptr<CTemplate> templ;
private:
    /**
     * Strips Name to the clean path
     * @return cannonical Name
     */
    virtual std::string cleanName(std::string &name) const = 0;


    /**
     * Creates local path that shall be added to web.html
     * @param name name of a file
     * @return local path to be given into web.html
     */
    virtual std::string localPath(const std::string &name) const = 0;

    /**
     * Writes type into its designated file
     * @param msg path to file
     * @param sock Socket
     */
    virtual bool writeType(const std::string &msg, int sock) const = 0;

    virtual void parseUrl(std::string &hold) = 0;
};