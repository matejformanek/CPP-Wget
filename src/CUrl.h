/**
*@author Matej Formanek <formama9@fit.cvut.cz>
*/

#pragma once

#include <iostream>
#include <memory>

#include "CParameter.h"

/**
 * Contains download Parameters
 * @param name Name of the template important for uniqueness of download names
 * @param url Initial URL u want to download from
 * @param depth decides how many layers it will download
 * @param url_banned when doing deep download never download or go to this URL
 * @param url_domain when doing deep download only download links with the same domain
 */
class CUrl : public CParameter {
public:
    CUrl(const std::string &name, const std::string &url, const std::string &urlDomain,const std::string &urlBanned, const std::string &Depth);

    ~CUrl() noexcept override = default;

    std::unique_ptr <CParameter> clone() const override;

    /**
     * Prompt for setting parameters
     * @return newly set part of template
     */
    std::unique_ptr <CParameter> setParameter() override;

    /**
     * Multi setter
     * @param num which parameter
     * @param param new value
     */
    void setParameter(size_t num, const std::string &param) override;

    /**
     * Multi getter
     * @param num which parameter u want to get
     */
    std::string getParameter(size_t num) const override;

    std::ostream &print(std::ostream &out) const override;

private:
    bool addName();

    std::string name;
    std::string url;
    std::string url_banned;
    std::string depth;
    std::string url_domain;
};
