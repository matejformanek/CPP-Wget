/**
*@author Matej Formanek <formama9@fit.cvut.cz>
*/

#pragma once

#include <iostream>
#include <memory>

#include "CParameter.h"

/**
 * Contains download Parameters
 * Specifies for all types where they should be downloaded to
 */
class CDirectory : public CParameter {
public:
    CDirectory(const std::string &defaultDir, const std::string &htmlDir, const std::string &cssDir,
               const std::string &jsDir, const std::string &imgDir);

    ~CDirectory() noexcept override = default;

    std::unique_ptr<CParameter> clone() const override;

    /**
     * Prompt for setting parameters
     * @return newly set part of template
     */
    std::unique_ptr<CParameter> setParameter() override;

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
    /**
     * Change path to absolute path for default directory
     */
    void changeDef();

    /**
     * Change path to absolute path for css,img, js and html
     */
    void changeType(const std::string &type, std::string &directory);

    std::string default_dir;
    std::string html_dir;
    std::string css_dir;
    std::string js_dir;
    std::string img_dir;
};
