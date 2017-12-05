#include "config.h"
#include <fstream>
#include <regex>
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

config::config(std::string path, std::string mode) : path(path), mode(mode) {
    std::ifstream f(path);
    const std::regex regMode("^(\\[%([A-Z]+?)%\\])$");
    const std::regex regParam("^([A-Z]+?)\\=(.+?)$");
    std::smatch match;

    int lineNum = 0;
    for (std::string line; getline(f, line); lineNum++) {

        if (std::regex_search(line, match, regMode)) {
            if (!(match[1].compare("DEFAULT") || match[1].compare(mode))) {
                break;
            }
        }

        if (std::regex_search(line, match, regParam)) {
            this->parameters[match[1]] = match[2];
        }
    }
}

config::config(std::string path) : config(path, "DEFAULT") {}
config::config() : config("", "DEFAULT") {}

std::string config::getParameter(std::string key) {
    return this->parameters[key];
}

std::map<std::string, config> config::importAll(std::string dirPath, std::string mode) {
    std::map<std::string, config> confMap;

    const std::regex regConf(".*?([^\\/]+)\\.cfg", std::regex_constants::icase);
    std::smatch match;
    std::string confName;

    fs::path dirPathp = fs::path(dirPath);

    for (auto &p : fs::directory_iterator(dirPathp)) {
        fs::path filePathp = fs::u8path(p);
        std::string filePath = filePathp.u8string();

        if (std::regex_search(filePath, match, regConf)) {
            confName = match[1];
            config conf(filePath, mode);
            confMap[confName] = conf;
        }
    }

    return confMap;
}


std::map<std::string, config> config::importAll(std::string dirPath) {
    return importAll(dirPath, "DEFAULT");
}
