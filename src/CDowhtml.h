/**
*@author Matej Formanek <formama9@fit.cvut.cz>
*/

#pragma once

#include "CDowtype.h"

/**
 * Class that downloads html from the site
 * @parent CDowtype
 */
class CDowhtml : public CDowtype {
public:
    explicit CDowhtml(const CTemplate &src);

    ~CDowhtml() noexcept override;

    std::unique_ptr<CDowtype> clone() const override;

    /**
     * HTML has its own getLinks in CDownload
     * Here its only empty function
     */
    bool getLinks() override;

    /**
    * Moves HTML from web to correct directory and file
    */
    void donwloadType() override;

private:
    /**
     * Virtual but without usage in this case because we dont use getLinks
     */
    std::string cleanName(std::string &name) const override;

    /**
     * Virtual but without usage in this case because we dont use getLinks
     */
    std::string localPath(const std::string &name) const override;

    /**
     * Writes type from web ("main html") into its designated file
     * @param msg path to file
     * @param sock Socket
     */
    bool writeType(const std::string &msg, int sock) const override;

    void parseUrl(std::string &hold) override;
};
