#ifndef SENSEFX_CONFIG_H
#define SENSEFX_CONFIG_H

#include <map>
#include <string>

class config {

    private:
        std::map<std::string, std::string> parameters;

    public:
        static std::map<std::string, config> importAll(std::string dirPath, std::string mode);
        static std::map<std::string, config> importAll(std::string dirPath);

        std::string path;
        std::string mode;

        config(std::string path, std::string mode);
        explicit config(std::string path);
        config();

        std::string getParameter(std::string key);
};


#endif //SENSEFX_CONFIG_H
