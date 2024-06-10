/**
*@author Matej Formanek <formama9@fit.cvut.cz>
*/

#pragma once

#include <vector>
#include <iostream>
#include <memory>

#include "CParameter.h"

/**
 * Template it self, holds list of parameters that define the wanted outcome
 */
class CTemplate {
public:
    CTemplate();

    explicit CTemplate(const std::string &templ_name);

    /**
     * Copy constructor
     * @param src CTemplate we want to go out from
     */
    CTemplate(const CTemplate &src);

    /**
     * Operator =
     * @param src CTemplate we want to go out from
     * @return new object
     */
    CTemplate &operator=(const CTemplate &src);

    ~CTemplate();

    /**
     * return unique copy of itself
     */
    std::unique_ptr<CTemplate> clone()const;

    friend std::ostream &operator<<(std::ostream & out,const CTemplate &src);

    /**
     * The template itself storing all data
     */
    std::vector<std::shared_ptr<CParameter>> list_of_parameters;

    /**
     * name of a template aka parameter[0]
     */
    std::string name;
private:
    /**
     * Convert path to absolute path
     */
    static std::string makePath(std::string dir);
};
