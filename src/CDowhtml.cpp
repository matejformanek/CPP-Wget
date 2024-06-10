/**
*@author Matej Formanek <formama9@fit.cvut.cz>
*/



#include <fstream>
#include "CDowhtml.h"

#define PATH_WEB "/home/matej/pa2/sem/examples/web"
#define BUFFER_SIZE 1024

//----------------------------------------------------------------------------------//

CDowhtml::CDowhtml(const CTemplate &src) {
    templ = src.clone();
}

//----------------------------------------------------------------------------------//

CDowhtml::~CDowhtml() noexcept = default;

//----------------------------------------------------------------------------------//

std::unique_ptr<CDowtype> CDowhtml::clone() const {
    return std::make_unique<CDowhtml>(*this);
}

//----------------------------------------------------------------------------------//

bool CDowhtml::getLinks() {
    return true;
}

//----------------------------------------------------------------------------------//

void CDowhtml::donwloadType() {
    std::ifstream is(PATH_WEB);
    //path to save to
    std::string msg = templ->list_of_parameters[1]->getParameter(1) == "EMPTY/" ?
                      templ->list_of_parameters[1]->getParameter(0) :
                      templ->list_of_parameters[1]->getParameter(1);
    msg += templ->list_of_parameters[0]->getParameter(0) + "_R.html";
    std::ofstream os(msg, std::ios::trunc | std::ios::out);
    if (!os.is_open()) {
        std::cerr << "Can not open file for writing: " << msg << std::endl;
        return;
    }

    os << is.rdbuf();
    os.close();
    is.close();
    std::cout << "HTML DOWNLOADED\n";
}

//----------------------------------------------------------------------------------//

bool CDowhtml::writeType(const std::string &msg, int sock) const {
    return true;
}


//----------------------------------------------------------------------------------//

std::string CDowhtml::cleanName(std::string &name) const {
    return "";
}

//----------------------------------------------------------------------------------//

std::string CDowhtml::localPath(const std::string &name) const {
    return "";
}

//----------------------------------------------------------------------------------//

void CDowhtml::parseUrl(std::string &hold) {
}

//----------------------------------------------------------------------------------//
