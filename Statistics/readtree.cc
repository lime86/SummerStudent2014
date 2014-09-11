//C(++) headers
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
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

string getPath(string input) {
	int pos = input.find_last_of("/");
	string path = input.substr(0,pos+1);
	return path;
}

int readtree(char * input) {
	gROOT->Reset();
	
	string path = getPath(string(input));
	
	TFile * f = TFile::Open(input);
	if (f == 0) {
		cout<<"Cannot open "<<input<<endl;
		return 0;
	}
	TH2F * hist = (TH2F *)f->Get("razorPlot");
	
	int nXBins = hist->GetNbinsX();
	int nYBins = hist->GetNbinsY();
	
	//slide: MR > 100 GeV, R² > 0.1
	//paper: MR > 250 GeV, R² > 0.81
	int SR[4][2] = {	{10, 10},
						{15, 30},
						{20, 75},
						{25, 81}
					};
	
	ofstream outfile ("cuts.txt", ios::app);
	fstream signal(path+"Atom.signal", ios::in | ios::out | ios::app);
	double norm = 0.0;
	if (signal.good()) {
		string line;
		string tmp;
		getline(signal, line);
		unsigned int pos = 0;
		pos = line.find_last_of(' ');
		tmp = line.substr(pos+1);
		//cout << tmp << endl;
		norm = atof(tmp.c_str());
	}
	
	for ( int i = 0; i < 4; ++i) {
		double error = 0.0;
		double integral = 0.0;
		integral = hist->TH2::IntegralAndError(SR[i][0], nXBins, SR[i][1], nYBins, error, "");
		outfile << input << " " << "SR" << i+1 << " " << integral << " " << error << endl;
		signal << "Total weight of SR" << i+1 << " events normalised to luminosity: " << norm * integral << endl;
		//cout << input << " " << "SR" << i+1 << " " << integral << " " << error << endl;
	}

	outfile.close();
	signal.close();
	gApplication->Terminate();
	return 42;
}


