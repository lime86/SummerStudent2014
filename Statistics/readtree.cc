//C(++) headers
#include <cstdlib>
#include <stdio.h>
#include <iostream>
#include <fstream>

//root headers
#include <TROOT.h>
#include <TFile.h>
#include <TH2.h>
#include <TH1.h>
#include <TSystem.h> 

#define _MR_CUT 25 //paper: MR > 250 GeV
#define _R_CUT 81 //paper: R² > 0.81

//signal or background by commandline arguments

using namespace std;

int readtree(char * input) {
	gROOT->Reset();
	
	TFile * f = TFile::Open(input);
	if (f == 0) cout<<"Cannot open "<<input<<endl;
	TH2F * hist = (TH2F *)f->Get("razorPlot");
	//cout<<"TEST"<<endl;
	
	int nXBins = hist->GetNbinsX();
	int nYBins = hist->GetNbinsY();
	
	//cout<<"Number of X bins: "<<nXBins<<" | Number of Y bins: "<<nYBins<<endl;
	
	double error = 0.0;
	double integral = hist->TH2::IntegralAndError(_MR_CUT, nXBins, _R_CUT, nYBins, error, "");
	
	ofstream outfile ("numbers.txt", ios::app);
	outfile << input << integral << " " << error << endl;
	//cout << "SR1 " << integral << " " << error << endl;
	
	
	outfile.close();

	return 42;
}
