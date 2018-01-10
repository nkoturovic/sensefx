#ifndef SENSEFX_CONFIG_H
#define SENSEFX_CONFIG_H

#include <map>
#include <string>

class Config {

    private:
        std::map<std::string, std::string> parameters;

    public:
        static std::map<std::string, Config> importAll(std::string dirPath, std::string mode);
        static std::map<std::string, Config> importAll(std::string dirPath);

        std::string path;
        std::string mode;

        Config(std::string path, std::string mode);
        explicit Config(std::string path);
        Config();

        std::string getParameter(std::string key);
};


#endif //SENSEFX_CONFIG_H
