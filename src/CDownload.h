/**
*@author Matej Formanek <formama9@fit.cvut.cz>
*/

#pragma once

#include <map>
#include <queue>
#include "CDownloader.h"
#include "CSocket.h"
#include "CDowtype.h"

/**
 * Working unit of download
 * Fully automated no input needed from user except when missing url
 * Downloads all links and types user has requested
 */
class CDownload : public CDownloader {
public:
    explicit CDownload(const std::vector<std::unique_ptr<CTemplate>> &src);

    ~CDownload() noexcept override;

    /**
     * Starts and operates the whole process of download
     * step by step starts all functions needed for download
     * Primary download -> when url is for html
     * Creates socket -> starts all parts of single downloads
     * @return true if it has templates for download
     */
    bool Prompt() override;

private:
    /**
     *  Checks that url has been given from user and that the url is not visited
     *  Plus creates connection with server and gives back Socket in @param sock
     * @param sock if the connection is created give value of Socket
     * @functions checkUrl, cannonicalUrl, Socket.connect (outside function)
     * @return false if some of inside functions fail
     */
    bool processParameters(int &sock, const std::string &url);

    /**
    * Checks that the template for download has url address
    * Does not check yet that the url is connectable
    * @param src Current template
    * @return false to abort download/true address was given
    */
    static bool checkUrl(const CTemplate &src);

    /**
     * Checks that the server is sending a response, if the path is purely to css,js or img navigate them elsewhere
     * if it is a html download the "main working html page" and add to visited
     * @param sock Socket
     * @functions checkGet, splitUrltypes, downloadFramehtml
     * @return false if some of inside funtion is false -> prompt needs to be "continued" = skip download of current template
     */
    bool processLink(int sock, const std::string &url);

    /**
     * Checks that server is sending some response
     * @param sock Socket
     * @return true if the server is responding
     */
    static bool checkGet(int sock, std::string url);

    /**
    * Downloads css,js,img separately because it doesnt have deep search
    * Downloads them separately if the URL is explicitly asking for specific type
    * @param url URL from user
    * @return false if @param url is of type css,img,js
    */
    bool splitUrltypes(const std::string &url);

    /**
     * Downloads html file to examples/web to be taken links from
     * Main file for downloaded, used to take all links(img,css,js,html) from as well
     * @param sock Socket
     */
    static bool downloadFramehtml(int sock);

    /**
     * When depth search is switched on (depth > 0) creates download templates for new layer links
     * @param src Template of parent that will be given to all child links with different name
     */
    void addNewlayers(CTemplate src);

    /**
     * Get links from web if we need to search deeply in more layers
     * <a> href links to other websites (.html)
     * @return vector of urls
     */
    std::vector<std::string> getLinks(const std::string &src_url, const std::string &src_name,
                                      const std::string &src_url_banned, const std::string &src_url_domain);

    /**
     * Extracts links from "web" (main html) and switches them to local
     */
    void parseUrl(const std::string &src_url, const std::string &src_name, const std::string &src_url_banned,
                  const std::string &src_url_domain, std::string &hold, std::vector<std::string> &links, size_t &id);

    /**
     * Checks that the links are not banned and also are on given domain. When the param is not EMPTY
     * @param name of url to be downloaded
     * @param src_url_banned list of banned urls
     * @param src_url_domain list of domain urls
     * @return false if the url @param name is either banned or not in domain
     */
    static bool correctUrl(const std::string &name, const std::string &src_url_banned,
                           const std::string &src_url_domain);

    /**
    * Create vector of types to be downloaded
    * @param src Template we downloading
    * @param sock Socket
    * @return vector of types we downloading
    */
    std::vector<std::unique_ptr<CDowtype>> downloadTypes(const CTemplate &src);

    /**
     * Strips address to pure form so it can be given over to @function cannonicalUrl
     * @return clean address (name)
     */
    static std::string cleanName(std::string &name, const std::string &src_url);

    /**
     * Transforms url to canonic thus visited can work
     * @param src template we changing
     */
    static std::string cannonicalUrl(std::string &url);

    /**
     * visited websites when depth searching
     * first key = url, second name of file
     */
    std::map<std::string, std::string> visited;

    /**
     * visited css websites
     * its given to CDowcss that fills it via pointer
     */
    std::map<std::string, std::string> visited_css;

    /**
     * visited js websites
     * its given to CDowjs that fills it via pointer
     */
    std::map<std::string, std::string> visited_js;

    /**
     * visited img websites
     * its given to CDowimg that fills it via pointer
     */
    std::map<std::string, std::string> visited_img;

    /**
     * Queue storing recursive (next layer links) for deep download
     */
    std::queue<std::unique_ptr<CTemplate>> deep_site;

    /**
     * Depth num, to have always unique name of files
     */
    size_t depth_id = 0;
};
