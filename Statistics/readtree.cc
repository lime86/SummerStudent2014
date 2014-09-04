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
#include <TApplication.h>

using namespace std;

int readtree(char * input) {
	gROOT->Reset();
	
	TFile * f = TFile::Open(input);
	if (f == 0) {
		cout<<"Cannot open "<<input<<endl;
		return 0;
	}
	TH2F * hist = (TH2F *)f->Get("razorPlot");
	
	int nXBins = hist->GetNbinsX();
	int nYBins = hist->GetNbinsY();
	
	cout << "Number of X bins: " << nXBins << " | Number of Y bins: " << nYBins <<endl;
	//slide: MR > 100 GeV, R² > 0.1
	//paper: MR > 250 GeV, R² > 0.81
	int SR[4][2] = {	{10, 10},
						{15, 30},
						{20, 55},
						{25, 81}
					};

	//cout<<"Number of X bins: "<<nXBins<<" | Number of Y bins: "<<nYBins<<endl;
	
	ofstream outfile ("cuts.txt", ios::app);
	
	for ( int i = 0; i < 4; ++i) {
		double error = 0.0;
		double integral = 0.0;
		integral = hist->TH2::IntegralAndError(SR[i][0], nXBins, SR[i][1], nYBins, error, "");
		//outfile << input << " " << "SR" << i << " " << integral << " " << error << endl;
		cout << input << " " << "SR" << i << " " << integral << " " << error << endl;
	}

	outfile.close();
	gApplication->Terminate();
	return 42;
}
