#define CATCH_CONFIG_MAIN

#include "Catch/single_include/catch2/catch.hpp"

#include "phylogenetic-fingerprints-demo/example.hpp"

TEST_CASE("Test example")
{
  REQUIRE( example() );
}
