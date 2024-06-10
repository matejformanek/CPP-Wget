/**
*@author Matej Formanek <formama9@fit.cvut.cz>
*/

#pragma once

#include <map>
#include "CDowtype.h"

/**
 * Class that downloads js from the site
 * @parent CDowtype
 */
class CDowjs : public CDowtype{
public:
    CDowjs(const CTemplate &src,std::map<std::string,std::string> &visited_js,std::map<std::string,std::string> &visited_img);

    /**
     * Special constructor when downloading js not from the "main" html web but from purely given path
     * @param src template
     * @param url path to js
     * @param visited_js map of already visited js files (remembers through whole download thanks to pointer)
     * @param visited_img if css contains img to be able to give visited as well
     */
    CDowjs(const CTemplate &src,std::string url,std::map<std::string,std::string> &visited_js,std::map<std::string,std::string> &visited_img);

    ~CDowjs() noexcept override;

    std::unique_ptr<CDowtype> clone() const override;

    /**
    * Finds all js links and saves them inside the vector
     * Plus rewrites them to local path in web.html
     * @return false if can not open system file
    */
    bool getLinks() override;

    /**
    * Downloads js into its correct file
    * Starts up multiple functions to get links
    * From current file and save it
    */
    void donwloadType() override;

private:
    void parseUrl(std::string &hold) override;

    void parseUrlImp(std::string &hold);

    /**
     * Checks if the links is not in banned, visited or domain
     * @return false if the link is not supposed to be added
     */
    bool controlLink(std::string &name) const;

    /**
     * Controls link and its type, saves current js to tmp so that we can @function getInsidelinks as well
     * @return false if link can not be downloaded
     */
    bool processLink(int &sock);

    /**
     * Writes type into its designated file
     * @param msg path to file
     * @param sock Socket
     */
    bool writeType(const std::string &msg, int sock) const override;

    /**
     * Finds inside a js file for import tag and downloads links for later download
     * As well replaces them for local path and downloads into local file
     * @return true if new file has been made
     */
    bool getInsideLinks();

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
     * Taken by reference from CDownload to remember visited across all downloads
     */
    std::map<std::string,std::string>* visited;

    /**
     * Taken by reference from CDownload to remember visited_IMG when downloading img
     */
    std::map<std::string,std::string>* visited_IMG;
};
