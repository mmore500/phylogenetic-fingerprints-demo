#pragma once

#include <filesystem>

#include "emp/config/ArgManager.hpp"
#include "emp/prefab/ConfigPanel.hpp"
#include "emp/web/UrlParams.hpp"
#include "emp/web/web.hpp"

#include "pfd/Config.hpp"

void use_existing_config_file(pfd::Config & config, emp::ArgManager & am) {
  if(std::filesystem::exists("phylogenetic-fingerprints-demo.cfg")) {
    std::cout << "Configuration read from phylogenetic-fingerprints-demo.cfg" << "\n";
    config.Read("phylogenetic-fingerprints-demo.cfg");
  }
  am.UseCallbacks();
  if (am.HasUnused())
    std::exit(EXIT_FAILURE);
}

void setup_config_web(pfd::Config & config)  {
  auto specs = emp::ArgManager::make_builtin_specs(&config);
  emp::ArgManager am(emp::web::GetUrlParams(), specs);
  use_existing_config_file(config, am);
}

void setup_config_native(pfd::Config & config, int argc, char* argv[]) {
  auto specs = emp::ArgManager::make_builtin_specs(&config);
  emp::ArgManager am(argc, argv, specs);
  use_existing_config_file(config, am);
}
