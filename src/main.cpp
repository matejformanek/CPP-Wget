#include <iostream>
#include <vector>
#include <filesystem>

#include "CMenu.h"
#include "CArguments.h"
#include "CDownloader.h"

int main(int argc, char **argv) {
    CMenu menu;
    if (argc > 1) {
        std::vector<std::string> arg;
        for (int argument = 1; argument < argc; ++argument)
            arg.emplace_back(argv[argument]);

        try {
            CArguments arguments(arg);
        } catch (const std::logic_error &warning) {
            return 0;
        }

        // Convert arguments into templates for download
        CArguments arguments(arg);
        std::vector<std::unique_ptr<CTemplate>> download_list = arguments.TempForDownload();
        if (download_list.empty()) {
            std::cout << "Nothing to download, shutting down!\n";
            return 1;
        }

        std::cout << "QUICK DOWNLOAD STARTED\n";
        if (!CDownloader::quickDownload(download_list))return 1;

        return 0;
    }
    if (!menu.Prompt()) {
        std::cout << "Incorrect shutdown" << std::endl;
        return 1;
    }

    return 0;
}
