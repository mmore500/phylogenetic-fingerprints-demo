//  This file is part of Phlogenetic Fingerprints Demo
//  Copyright (C) Matthew Andres Moreno, 2021.
//  Released under MIT license; see LICENSE

#include <iostream>

#include "emp/web/web.hpp"

#include "pfd/config_setup.hpp"
#include "pfd/Config.hpp"
#include "pfd/PhyloFingerprintsPanel.hpp"

namespace UI = emp::web;

UI::Document doc("emp_base");

pfd::Config cfg;

int main()
{

  // Set up a configuration panel for web application
  setup_config_web(cfg);
  cfg.Write(std::cout);

  doc << pfd::PhyloFingerprintsPanel{};

  return 0;
}
