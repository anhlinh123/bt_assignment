#include <backend/backend.hpp>
#include <backend/controller.hpp>
#include <drogon/drogon.h>
#include <exception>

int main(int argc, char *argv[]) {
  // parse args
  if (argc != 2) {
    std::string err = "Invalid arguments! Usage:";
    err += argv[0];
    err += " ";
    err += "<path-to-config>";
    throw std::runtime_error{err};
  }

  drogon::app().loadConfigFile(argv[1]);

  // extract data path from config file
  auto customConfig = drogon::app().getCustomConfig();
  if (!customConfig || !customConfig["data"]) {
    throw std::runtime_error{"No data config"};
  }
  std::filesystem::path configFile{argv[1]};
  auto dataFile = configFile.parent_path() / customConfig["data"].asString();

  // create a backend
  std::ifstream istrm{dataFile.c_str(), std::ios::binary};
  if (!istrm.is_open()) {
    throw std::runtime_error{"Can't open data"};
  }
  Json::CharReaderBuilder rbuilder;
  rbuilder["collectComments"] = false;
  std::string errs;
  Json::Value root;
  Json::parseFromStream(rbuilder, istrm, &root, &errs);
  auto backendPtr = std::make_shared<Backend>(root);

  drogon::app().registerController(std::make_shared<Controller>(backendPtr));
  drogon::app().run();
  return 0;
}
