
#include "LednickyInfo.h"

LednickyInfo::LednickyInfo(const TString& systemName,
                           Double_t lambdaParam,
                           TH2D *transformMatrix,
                           Bool_t isIdenticalPair):
  fSystemName(systemName),
  fLambdaParam(lambdaParam),
  fTransformMatrix(transformMatrix),
  fIsIdenticalPair(isIdenticalPair)
{
}

LednickyInfo::~LednickyInfo()
{
  //Transform matrices will be deleted by their lednicky eqns
}
