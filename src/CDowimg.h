/**
*@author Matej Formanek <formama9@fit.cvut.cz>
*/

#pragma once

#include <map>
#include "CDowtype.h"

/**
 * Class that downloads img from the site
 * @parent CDowtype
 */
class CDowimg : public CDowtype {
public:
    CDowimg(const CTemplate &src, std::map<std::string, std::string> &visited_img);

    /**
     * Special constructor when downloading img not from the "main" html web but from purely given path
     * @param src template
     * @param url path to img
     * @param visited_img map of already visited img files (remembers through whole download thanks to pointer)
     */
    CDowimg(const CTemplate &src, std::string url, std::map<std::string, std::string> &visited_img);

    ~CDowimg() noexcept override;

    std::unique_ptr<CDowtype> clone() const override;

    /**
    * Finds all img links and saves them inside the vector
     * Plus rewrites path to local in web.html
     * @return false if can not open system file
    */
    bool getLinks() override;

    /**
     * Downloads img into its correct file
     */
    void donwloadType() override;

private:
    /**
     * Strips Name to the clean path
     * @return cannonical Name
     */
    std::string cleanName(std::string &name) const override;

    /**
     * Creates local path that shall be added to web.html
     * @param name name of a file
     * @return local path to be given into web.html
     */
    std::string localPath(const std::string &name) const override;

    /**
     * Checks if the links is not in banned, visited or domain
     * @return false if the link is not supposed to be added
     */
    bool controlLink(std::string &name) const;

    /**
     * Writes type into its designated file
     * @param msg path to file
     * @param sock Socket
     */
    bool writeType(const std::string &msg, int sock) const override;

    void parseUrl(std::string &hold) override;

    /**
     * Taken by reference from CDownload to remember visited across all downloads
     */
    std::map<std::string, std::string> *visited;
};
