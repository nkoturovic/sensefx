#include "Config.h"
#include <fstream>
#include <regex>
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

Config::Config(std::string path, std::string mode) : path(path), mode(mode) {
    std::ifstream f(path);
    const std::regex regMode("^(\\[%([A-Z]+?)%\\])$");
    const std::regex regParam("^([A-Z_]+?)\\=(.+?)$");
    std::smatch match;

    for (std::string line; getline(f, line);) {

        if (std::regex_search(line, match, regParam)) {
            this->parameters[match[1]] = match[2];

        } else if (std::regex_search(line, match, regMode)) {
            if (!((match[2].compare("DEFAULT") == 0) || (match[2].compare(mode) == 0)))
                break;
        }
    }
}

Config::Config(std::string path) : Config(path, "DEFAULT") {}
Config::Config() { } 

std::string Config::getParameter(std::string key) {
	if (this->parameters.count(key) == 0)
		return "ERROR_NOT_EXIST";

    return this->parameters[key];
}

std::map<std::string, Config> Config::importAll(std::string dirPath, std::string mode) {
    std::map<std::string, Config> confMap;

    const std::regex regConf(".*?([^\\/]+)\\.cfg", std::regex_constants::icase);
    std::smatch match;
    std::string confName;

    fs::path dirPathp = fs::path(dirPath);

    for (auto &p : fs::directory_iterator(dirPathp)) {
        fs::path filePathp = fs::u8path(p);
        std::string filePath = filePathp.u8string();

        if (std::regex_search(filePath, match, regConf)) {
            confName = match[1];
            Config conf(filePath, mode);
            confMap[confName] = conf;
        }
    }

    return confMap;
}


std::map<std::string, Config> Config::importAll(std::string dirPath) {
    return importAll(dirPath, "DEFAULT");
}
