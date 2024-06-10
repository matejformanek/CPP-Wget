/**
*@author Matej Formanek <formama9@fit.cvut.cz>
*/

#pragma once


/**
 * Abstract class for all the parameters that define what we will be downloading
 * @child CUrl, CDirectory, CType
 */
class CParameter{
public:
    virtual ~CParameter() noexcept = default;

    virtual std::unique_ptr<CParameter> clone()const = 0;

    /**
     * Sets or changes parametr inside class
     * @return this->clone()
     */
    virtual std::unique_ptr<CParameter> setParameter() = 0;

    /**
     * Multi Getter
     * @param num is deciding which parameter we want to get
     */
    virtual std::string getParameter(size_t num) const = 0;

    /**
     * Multi Setter for all elements number decides which
     * @param type what we changing
     * @param param new item
     */
    virtual void setParameter(size_t num,const std::string &param) {};

    /**
     * Virtual ostream print method
     * @param out stream to be printed to
     */
    virtual std::ostream &print(std::ostream &out)const = 0;

    friend std::ostream &operator<<(std::ostream &out,const CParameter &src){
        return src.print(out);
    }
};