/**
 * \file ArgParse.cxx
 */

#include "ArgParse.h"

#include <iostream>

ArgParse::ArgParse(int argc, const char **argv):
  args(argv, argv + argc)
{
  // std::cout << "args: " << args.size() << "\n";
}
