//*********************************************
// Base class for Lednicky correlation function
// equation objects. Contains the parameters for
// a particular Lednicky Eqn, and can return a
// TGraph of the eqn.
// Maybe can get and use a residual correlation?
//*******************************************

#include "TString.h"
#include <TGraph.h>
#include <TH2D.h>
#include "TMath.h"
#include "TF1.h"
#include "LednickyEqn.h"
#include "LednickyInfo.h"
#include "Faddeeva.hh"
#include <iostream>
#include <complex>
#include <assert.h>


using namespace std;

LednickyEqn::LednickyEqn(const LednickyInfo &info, Int_t nBins, Double_t binWidth)
{
  fName = info.GetSystemName();
  fIsIdentical = info.GetIsIdenticalPair();
  fUseRootSScaling = info.GetUseRootSScaling();
  //If transform matrix is null, this is a primary correlation
  fTransformMatrix = info.GetTransformMatrix();
  fF0Real = 0.;
  fF0Imag = 0.;
  fD0 = 0.;
  fRadius = 0.;
  fNBins = nBins;
  fBinWidth = binWidth;
  fBaseMass1 = info.GetBaseMass1();
  fBaseMass2 = info.GetBaseMass2();
  fActualMass1 = info.GetActualMass1();
  fActualMass2 = info.GetActualMass2();

  if(fUseRootSScaling) {
    assert(fBaseMass1 > 0);
    assert(fBaseMass2 > 0);
    assert(fActualMass1 > 0);
    assert(fActualMass2 > 0);
  }
}

LednickyEqn::~LednickyEqn()
{
  if(fTransformMatrix){
    delete fTransformMatrix;
    fTransformMatrix = NULL;
  }
}

TGraph* LednickyEqn::GetBaseLednickyGraph()
{
  // Do all the math to calculate a base lednicky equation
  // in the k* frame of the parent particles. Return a
  // TGraph of the function.

  // Arrays to store X and Y values of correlation function
  Double_t *kstar = new Double_t[fNBins];
  Double_t *cf    = new Double_t[fNBins];
  const complex<double> I(0.0, 1.0);

  cout << fRadius << " " << fD0 << " " << fF0Real << " " <<fF0Imag << "\n";

  for(Int_t iBin = 0; iBin < fNBins; iBin++)
  {
    // Calculate the value of the correlation function at each bin

    // Use the center of the bin
    kstar[iBin] = fBinWidth * (1. * iBin + 0.5);

    // We might use sqrt(s) scaling of the scattering amplitude.
    // If so, we do this, we account for the scaling with a
    // modified kstar value.
    Double_t kstarPrime = kstar[iBin];

    if (fUseRootSScaling) {
      Double_t s = pow( pow( pow(fActualMass1, 2) + pow(kstar[iBin],2), 0.5)
			+ pow( pow(fActualMass2, 2) + pow(kstar[iBin],2), 0.5), 2); //mandelstam s

      kstarPrime = pow( (s*s + pow(fBaseMass1, 4) + pow(fBaseMass2, 4)
			- 2.*s*(pow(fBaseMass1, 2) + pow(fBaseMass2, 2))
			- 2.*pow(fBaseMass1, 2)*pow(fBaseMass2, 2) )
			/ (4.*s), 0.5);
    }

    // Get the scattering amplitude
    const complex<double> f0(fF0Real, fF0Imag);
    complex<double> scatterAmp;
    scatterAmp = 1.0 / ( 1.0 / f0
		      + 0.5 * fD0 * pow(kstarPrime / HbarC(), 2)
		      - I * kstarPrime/HbarC());

    // Now calculate the correlation function value
    cf[iBin] = 0.5 * norm(scatterAmp)/pow(fRadius,2)
                   * (1. - fD0/(2. * sqrt(TMath::Pi()) * fRadius));

    cf[iBin] += 2. * scatterAmp.real()
                * GetLednickyF1(2. * kstar[iBin] * fRadius / HbarC())
                / (sqrt(TMath::Pi())*fRadius);

    //printf("(%.016f+%.016f) ", scatterAmp.real(), scatterAmp.imag());
    //cout << sqrt(pow(scatterAmp.real(), 2) + pow(scatterAmp.imag(), 2)) << ' ';
    // cout << scatterAmp << " " << cf[iBin] << " ";

    // cf[iBin]
    float f2 = GetLednickyF2(2. * kstar[iBin] * fRadius / HbarC());
    float y = scatterAmp.imag()
//              * GetLednickyF2(2. * kstar[iBin] * fRadius / HbarC())
              * f2
              / fRadius;
    cf[iBin] -= y;
    // printf("%.016f ", f2);

    if (fIsIdentical && false) {
      cf[iBin] *= 0.5;
      cf[iBin] -= 0.5 * exp(-4. * pow(kstar[iBin] * fRadius/HbarC(),2));
    }
    cf[iBin] += 1.;
    printf("(%.03f, %.08f)\n", kstar[iBin], cf[iBin]); 
//, scatterAmp.real(), scatterAmp.imag());
  }

  // Now make a TGraph of the correlation function
  TGraph * baseGraph = new TGraph(fNBins, kstar, cf);
  delete[] kstar;
  delete[] cf;
  return baseGraph;
}

TGraph *LednickyEqn::GetLednickyGraph()
{
  // Used by Fitter.  Get a TGraph of the Lednicky Eqn
  // (in the case of residual correlations, transform it)

  TGraph *lednickyGraph = GetBaseLednickyGraph();
  lednickyGraph = TransformLednickyGraph(lednickyGraph);
  lednickyGraph->SetTitle(fName);
  return lednickyGraph;
}

void LednickyEqn::SetParameters(const vector<Double_t> pars)
{
  // Set all the fit parameters
  // Normalization is taken care of by PairSystem class
  fRadius = pars[0];
  fF0Real = pars[1];
  fF0Imag = pars[2];
  fD0     = pars[3];
}

TGraph* LednickyEqn::TransformLednickyGraph(TGraph *base)
{
  // If the LednickyEqn object is a residual correlation,
  // return a graph that has been transformed into the
  // relative momentum space of the primary correlations.

  if(!fTransformMatrix) return base;
  TGraph *transformedGraph = (TGraph*) base->Clone("transformedGraph");
  assert(transformedGraph);

  const Int_t nBins = transformedGraph->GetN();
  const Int_t nBinsTransform = fTransformMatrix->GetNbinsX();
  assert(nBins == nBinsTransform);
  // DaughterBins are in the relative momentum space of the
  // primary correlations
  for(Int_t daughterBin = 0; daughterBin < nBins; daughterBin++)
  {
    Double_t weightSum = 0.;
    Double_t valueSum = 0.;
    // ParentBins are in the relative momentum space of the
    // residual correlation
    for(Int_t parentBin = 0; parentBin < nBins; parentBin++)
    {
      Double_t weight = fTransformMatrix->GetBinContent(daughterBin+1, parentBin+1);
      weightSum += weight;
      valueSum += weight * base->GetY()[parentBin];
    }
    if(weightSum < 0.99) transformedGraph->GetY()[daughterBin] = 0;
    else transformedGraph->GetY()[daughterBin] = valueSum/weightSum;
  }

  delete base;
  return transformedGraph;
}


Double_t LednickyEqn::GetLednickyF1(Double_t z)
{
  double result = (1./z)*Faddeeva::Dawson(z);
  return result;
}

Double_t LednickyEqn::GetLednickyF2(Double_t z)
{
  TF1 lednickyF2("lednickyf2","(1-exp(-x*x))/(x)");
  double  y = lednickyF2.Eval(z);
  // printf("%.08f -> %.08f :", z, y);
  return y;
  // return lednickyF2.Eval(z);

}
