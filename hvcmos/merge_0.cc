//C(++) headers
#include <cstdlib>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include <signal.h>

//root headers
#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TSystem.h> 
#include <TMath.h>
#include <TDirectory.h>

#define MAXHIT 1000

//first y value

using namespace std;

struct _hits {
	int NHits;
	int PixX[MAXHIT];
	int PixY[MAXHIT];
	int Value[MAXHIT];
	int Timing[MAXHIT];
	int HitInCluster[MAXHIT];
	double PosX[MAXHIT];
	double PosY[MAXHIT];
	double PosZ[MAXHIT];
};

int merge_0(char * input, char * output) {
	gROOT->Reset();
		
	_hits unmerged;
	_hits merged;
	

	TFile * f = TFile::Open(input);
	if (f == 0) cout<<"Cannot open "<<input<<endl;
	TDirectory * d = (TDirectory *)f->Get("Plane6");
	TTree * t = (TTree *)d->Get("Hits");
	
	TFile * fnew = new TFile(output, "RECREATE");
	if (fnew != 0) cout<<"Merging..."<<endl;
	TDirectory * dnew = fnew->mkdir("Plane6");
	dnew->cd();
	TTree * m_pltree = new TTree("Hits", "Hits");
	
	t->SetBranchAddress("NHits", &unmerged.NHits);
	t->SetBranchAddress("PixX", unmerged.PixX);
	t->SetBranchAddress("PixY", unmerged.PixY);
	t->SetBranchAddress("Value", unmerged.Value);
	t->SetBranchAddress("Timing", unmerged.Timing);
	t->SetBranchAddress("HitInCluster", unmerged.HitInCluster);
	t->SetBranchAddress("PosX", unmerged.PosX);
	t->SetBranchAddress("PosY", unmerged.PosY);
	t->SetBranchAddress("PosZ", unmerged.PosZ);
	
	m_pltree->Branch("NHits", &unmerged.NHits, "NHits/I");
	m_pltree->Branch("PixX", unmerged.PixX, "HitPixX[NHits]/I");
	m_pltree->Branch("PixY", merged.PixY, "HitPixY[NHits]/I");
	m_pltree->Branch("Value", unmerged.Value, "HitValue[NHits]/I");
	m_pltree->Branch("Timing", unmerged.Timing, "HitTiming[NHits]/I");
	m_pltree->Branch("HitInCluster", unmerged.HitInCluster, "HitInCluster[NHits]/I");
	m_pltree->Branch("PosX", unmerged.PosX, "HitPosX[NHits]/D");
	m_pltree->Branch("PosY", unmerged.PosY, "HitPosY[NHits]/D");
	m_pltree->Branch("PosZ", unmerged.PosZ, "HitPosZ[NHits]/D");
	

	int nentries = t->GetEntries();
	//cout<<"Number of entries: "<<nentries<<endl;
	
	for (int i = 0; i < nentries; ++i) {
		t->GetEntry(i);
		
		merged.NHits = unmerged.NHits;

		for (int j = 0; j < unmerged.NHits; ++j) {
			merged.PixX[j] = unmerged.PixX[j];
			
			if ( unmerged.NHits > 0 ) {
				merged.PixY[j] = TMath::FloorNint((unmerged.PixY[j]+1.0)/2.0);	//161-191 --> 81-96
			}

			

		}
		m_pltree->Fill();
	}
	
	fnew->Write();
	fnew->Print();
	cout<<"Merging done."<<endl;
	return 42;
}
