//  This file is part of Phylogenetic Fingerprints Demo
//  Copyright (C) Matthew Andres Moreno, 2021.
//  Released under MIT license; see LICENSE

#include <iostream>

#include "emp/base/vector.hpp"

#include "pfd/config_setup.hpp"
#include "pfd/Config.hpp"

// This is the main function for the NATIVE version of Phylogenetic Fingerprints Demo.

pfd::Config cfg;

int main(int argc, char* argv[])
{
  // Set up a configuration panel for native application
  setup_config_native(cfg, argc, argv);
  cfg.Write(std::cout);

  std::cout << "Hello, world!" << "\n";

  return 0;
}
