/**
*@author Matej Formanek <formama9@fit.cvut.cz>
*/

#pragma once

#include <iostream>
#include <memory>

#include "CParameter.h"

/**
 * Contains download Parameters
 * Defines which type of files will be downloaded
 */
class CType : public CParameter {
public:
    CType(const std::string & htmlType, const std::string & cssType, const std::string & jsType, const std::string & imgType);

    ~CType() noexcept override = default;

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
    std::string html_type;
    std::string css_type;
    std::string js_type;
    std::string img_type;
};
