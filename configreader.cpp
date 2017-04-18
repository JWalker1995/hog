#include "configreader.h"

#include <yaml-cpp/yaml.h>

ConfigReader::ConfigReader()
{
    YAML::Node config = YAML::LoadFile("config.yaml");

    if (config["lastLogin"]) {
      std::cout << "Last logged in: " << config["lastLogin"].as<DateTime>() << "\n";
    }

    const std::string username = config["username"].as<std::string>();
    const std::string password = config["password"].as<std::string>();
    login(username, password);
    config["lastLogin"] = getCurrentDateTime();

    std::ofstream fout("config.yaml");
    fout << config;

}
