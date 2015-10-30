/**
 * \file RunLednicky.cxx
 * \author Andrew Kubera
 */

#include "LednickyEqn.h"
#include "LednickyInfo.h"

#include <TFile.h>
#include <TString.h>

#include <iostream>
#include <vector>
#include <cassert>

using namespace std;


// main for full program
int main(int argc, char **argv)
{
  std::vector<TString> args(argv + 1, argv + argc);

  LednickyInfo params("SYSTEM",
                      1.0,
                      NULL,
                      false,
                      false,
                      0.139,
                      1.115,
                      0.139,
                      1.115
                    );

  LednickyEqn eq(params, 20, 0.05);
  eq.SetParameters({0.303, 1.0, 0.1, 1.8});

  TGraph *plot = eq.GetLednickyGraph();

  return 0;
}
