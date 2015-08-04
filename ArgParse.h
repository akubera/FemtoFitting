/**
 * \file ArgParse.h
 *
 */

#pragma once

#include <TString.h>

#include <vector>

/**
 * \class ArgParse
 * \brief Class for processing arguments given to the program.
 */
class ArgParse {
public:
  /**
   * Construct from the inputs from main
   */
  ArgParse(int argc, const char **argv);

  size_t count() const {
    return args.size();
  }

protected:
  std::vector<TString> args;
};
