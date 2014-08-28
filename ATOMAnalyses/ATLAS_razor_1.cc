// -*- C++ -*-
#include <iostream>

///ATOM
#include "Atom/AtomAnalysis.hh"
#include "Atom/Detectors/Atlas.hh"
#include "Atom/Detectors/Atlas2011.hh"
//#include "Atom/Projections/Razor.hh"
#include "Atom/VariableCut.hh"
#include "Atom/EfficiencyHelper.hh"
#include "Atom/XSecHelper.hh"

///Rivet
#include "Rivet/Math/Vector3.hh"
#include "Rivet/Math/Vector4.hh"

///ROOT
#include "TH1.h"
#include "TH2.h"
#include "TPDF.h"
#include "TCanvas.h"
#include "TSystem.h"
#include "TDirectory.h"
#include "TROOT.h"

using namespace std;

namespace Atom {


    class ATLAS_razor_1 : public AtomAnalysis {
    public:

        /// @name Constructors etc.
        //@{

        /// Constructor
        ATLAS_razor_1()
            : AtomAnalysis("ATLAS_razor_1"),
              _atlas(getAtlas2011(_bEff, _bSmear)) {
            /// @todo Set whether your finalize method needs the generator cross section
            setNeedsCrossSection(true);
        }


        //@}


    public:

        /// @name Analysis methods
        //@{

        /// Book histograms and initialise projections before the run
        void initLocal() {
			gROOT->SetBatch();
            FinalState base = _atlas.finalState();
            
            IsoElectron ele_pre = _atlas.electronBase(_atlas.mediumEleRange(), 0.2, 0.15, 0.0);
            FastSimFinalState<IsoElectron> ele_smear = _atlas.mediumElectron(ele_pre);
            addProjection(ele_smear, "IsoElectrons");
            
            IsoMuon mu_pre = _atlas.muonBase(RangeSelector(RangeSelector::TRANSVERSE_MOMENTUM, 7.0, 14000.), 0.2, 0.0, 1.8);
            FastSimFinalState<IsoMuon> mu_smear = _atlas.combinedMuon(mu_pre);
            addProjection(mu_smear, "IsoMuons");
            
            JetFinalState jets_pre = _atlas.jetBase(base, 0.4, _atlas.topoJetRange() & RangeSelector(RangeSelector::TRANSVERSE_MOMENTUM, 30.0, 7000.0));
            FastSimFinalState<JetFinalState> jets = _atlas.topoJet(jets_pre);
            addProjection(jets, "topoJets");
            
            MergedFinalState lept(ele_smear, mu_smear);
            addProjection(lept, "IsoLeptons");

            NearIsoParticle jets_clean = _atlas.removeOverlap(jets, lept, _atlas.jetElectronSeparation());
            addProjection(jets_clean, "Jets");
            
            MergedFinalState razorparticles(jets_clean, lept); //allow a particle's uniqueness (default)
            addProjection(razorparticles, "RazorParticles");
            
            HTTools ht(razorparticles); //scalar sum of pt of all particles + missing Et
            addProjection(ht, "Ht");
            
            MissingMomentum met = _atlas.met(base);
            addProjection(met, "MissingET");
            
            //Razor razr(razorparticles, met);
            //addProjection(razr, "Razor");
            
            /// @todo Book histograms here, e.g.:
            
			///ROOT histograms
			_f_hist = new TFile("ATLAS_razor_1.root","RECREATE");
			
			_h_met			= new TH1F("met", "Missing Energy", 300,0,300);
			_h_met->GetXaxis()->SetTitle("MET (GeV)");
			_h_met->GetYaxis()->SetTitle("Occurrence");
			
			_h_vetoedEvts	= new TH1F("vetoedEvts", "Vetoed Events", 2,0,2);
			_h_vetoedEvts->GetXaxis()->SetTitle("Vetoed Events");
			_h_vetoedEvts->GetYaxis()->SetTitle("Occurrence");
			
			_h_jetMult		= new TH1F("jetMult", "Jet Multiplicity", 10,0,10);
			_h_jetMult->GetXaxis()->SetTitle("Jet Multiplicity");
			_h_jetMult->GetYaxis()->SetTitle("Occurrence");
			
			_h_ht				= new TH1F("ht", "HT", 250,0,500); //scalar sum of pt in all jets
			_h_ht->GetXaxis()->SetTitle("HT (GeV)");
			_h_ht->GetYaxis()->SetTitle("Occurrence");
			
			_h_leadingJetPt		= new TH1F("leadingJetPt", "Leading Jet PT", 500,0,1000);
			_h_leadingJetPt->GetXaxis()->SetTitle("Leading Jet PT (GeV)");
			_h_leadingJetPt->GetYaxis()->SetTitle("Occurrence");
			
			_h_subleadingJetPt	= new TH1F("subleadingJetPt", "Subleading Jet PT", 250,0,500);
			_h_subleadingJetPt->GetXaxis()->SetTitle("Subleading Jet PT (GeV)");
			_h_subleadingJetPt->GetYaxis()->SetTitle("Occurrence");
			
			_h_otherJetPt		= new TH1F("otherJetPt", "Other Jet PT", 250,0,500);
			_h_otherJetPt->GetXaxis()->SetTitle("Other Jet PT (GeV)");
			_h_otherJetPt->GetYaxis()->SetTitle("Occurrence");
	
			_h_razor			= new TH2F("razorPlot", "Razor Plot", 200, 0., 2000., 140, 0., 1.4);
			_h_razor->SetOption("colz");
			_h_razor->GetXaxis()->SetTitle("M_R (GeV)");
			_h_razor->GetYaxis()->SetTitle("R2");
			_h_razor->GetZaxis()->SetTitle("Occurrence");
			
			_objList = gDirectory->GetList();
			
			_canvas = new TCanvas("Canvas");
			
			check = 0;
			vetoCheck = 0;
			
			logfile.open ("ATLAS_razor_1.log", ios::trunc);

			cout << "Finished initialisation" << endl;

        }


        /// Perform the per-event analysis
        bool analyzeLocal(const Event& event, const double weight) {
			unsigned int jetMult = 0;
            
            // get jets
            const Particles jets = applyProjection<NearIsoParticle>(event, "Jets").particlesByPt(); //jets_clean
            const Particles eles = applyProjection< FastSimFinalState<IsoElectron> >(event, "IsoElectrons").particlesByPt();
            const Particles mus = applyProjection< FastSimFinalState<IsoMuon> >(event, "IsoMuons").particlesByPt();
            const Particles razr = applyProjection<MergedFinalState>(event, "RazorParticles").particlesByPt(); //jets_clean and leptons
            const MissingMomentum pmet = applyProjection<MissingMomentum>(event, "MissingET");
            const FourMomentum met = pmet.missingEt();
 
            // get Ht & co.
            const HTTools Htools = applyProjection<HTTools>(event, "Ht");
            //const Razor razor = applyProjection<Razor>(event, "Razor");
            
			/// Megajets
			FourMomentum megaJet1(0., 0., 0., 0.);
			FourMomentum megaJet2(0., 0., 0., 0.);

			/// Razor Variables
			double mr = 0.;
			double mrt = 0.;
			double r = 0.;
            
            
            // fill histograms
            jetMult = razr.size();
            _h_met->Fill(met.mod(), weight);
            _h_jetMult->Fill(jetMult, weight);
            _h_ht->Fill(Htools.HT(),weight);
            
            ///jet multiplicity cut
			if (jetMult < 2 ) {
				++vetoCheck;
				_h_vetoedEvts->Fill(1.,weight);
				vetoEvent;
			}
			
			_h_leadingJetPt->Fill(razr[0].momentum().pT(), weight);
			_h_subleadingJetPt->Fill(razr[1].momentum().pT(), weight);
			
			for (unsigned int i = 2; i < jetMult; ++i) {
				_h_otherJetPt->Fill(razr[i].momentum().pT(), weight);
			}
			
			
			//if ( !(jets.at(0).pt() >= 60 && jets.at(1).pt() >= 60) ) {
			//if ( !(jets[0].momentum().pT() >= 60 && jets[1].momentum().pT() >= 60) ) {
			//		++vetoCheck;
			//		_h_vetoedEvts->Fill(1.,1.);
			//		vetoEvent;
			//}
			
			if ( jetMult == 2 ) {
				megaJet1 = razr[0].momentum();
				megaJet2 = razr[1].momentum();
			}
			else {
				//hemisphere(razr, megaJet1, megaJet2);
				hemisphere(razr, megaJet1, megaJet2);
			}
			
			//get the pt vector from a pseudojet
			Rivet::Vector3 pt_j1 = getVecPt(megaJet1);
			Rivet::Vector3 pt_j2 = getVecPt(megaJet2);
			
			//vector multiplication: Vector3 class
			mr		= sqrt( pow(( megaJet1.E() + megaJet2.E() ), 2) - pow(( megaJet1.pz() + megaJet2.pz() ), 2) );
			mrt		= sqrt( ( met.mod() * ( megaJet1.pt() + megaJet2.pt() ) - getP3(met).dot(pt_j1 + pt_j2) ) / 2.);
			r		= mrt/mr;

   
            //----------CUTS--------------------
            ///missing ET cut
            //if ( met.mod() < 40. ) {
			//	_h_vetoedEvts->Fill(1.,1.);
			//	cout << "Event vetoed: MET trigger"<<endl;
			//	vetoEvent;
			//}
						
			///jet kinematics cut
			//if ( !(jets[0].momentum().pT() >= 60 && jets[1].momentum().pT() >= 60) ) {
			//	++vetoCheck;
			//	_h_vetoedEvts->Fill(1.,1.);
			//	vetoEvent;
			//}
							

            _effh.PassEvent("Efficiency of the Baseline Selection");			
			
			_h_vetoedEvts->Fill(0., weight);			
			_h_razor->Fill(mr, r*r, weight);
			
			++check;
			//logfile << "Evaluated events: "<< check << ". Vetoed events: "<< vetoCheck << "|| MET = "<< met.mod() << "| Leading Jet pt = " << jets[0].momentum().pT() << "| R = " << rval <<endl;
			
            return true;
        }

        /// Normalise histograms etc., after the run
        void finalizeLocal() {
			_canvas->cd(0);
			_canvas->Print("ATLAS_razor_1.pdf[", "pdf");
			
			TList* objlist = gDirectory->GetList();
			for( int io=0; io < objlist->GetEntries(); io++){
				objlist->At(io)->Write();
				objlist->At(io)->Draw();
				_canvas->Print("ATLAS_razor_1.pdf", "pdf");	
			}
			

			/*
			_h_met->Draw();
			_canvas->Print("ATLAS_razor_1.pdf", "pdf");
			_canvas->Print("ATLAS_razor_1_met.C");	

			_h_vetoedEvts->Draw();
			_canvas->Print("ATLAS_razor_1.pdf", "pdf");
			_canvas->Print("ATLAS_razor_1_vetoedEvts.C");
			
			_h_jetMult->Draw();
			_canvas->Print("ATLAS_razor_1.pdf", "pdf");
			_canvas->Print("ATLAS_razor_1_jetMult.C");
			
			_h_leadingJetPt->Draw();
			_canvas->Print("ATLAS_razor_1.pdf", "pdf");
			_canvas->Print("ATLAS_razor_1_leadingJetPt.C");
			
			_h_subleadingJetPt->Draw();
			_canvas->Print("ATLAS_razor_1.pdf", "pdf");
			_canvas->Print("ATLAS_razor_1_subleadingJetPt.C");
			
			_h_otherJetPt->Draw();
			_canvas->Print("ATLAS_razor_1.pdf", "pdf");
			_canvas->Print("ATLAS_razor_1_otherJetPt.C");
			
			_h_ht->Draw();
			_canvas->Print("ATLAS_razor_1.pdf", "pdf");
			_canvas->Print("ATLAS_razor_1_ht.C");
			
			_h_razor->Draw("colz");
			_canvas->Print("ATLAS_razor_1.pdf", "pdf");
			_canvas->Print("ATLAS_razor_1_razor.C");
			*/
			
			_canvas->Print("ATLAS_razor_1.pdf]", "pdf");

            _luminosity = std::make_pair(35.0 / picobarn, 0.11);
            double norm = crossSection() * _luminosity.first / sumOfWeights();

			logfile.close();
			_f_hist->Write();
            cout << "Finalising..."<<endl;
        }

        //@}


    private:

        ATLASProjections _atlas;

    private:

        /// @name Histograms
		
		//@{
		//ROOT histograms
		TFile * _f_hist;
		TH1F * _h_met;
		TH1F * _h_vetoedEvts;
		
		TH1F * _h_jetMult;
		TH1F * _h_leadingJetPt;
		TH1F * _h_subleadingJetPt;
		TH1F * _h_otherJetPt;
		TH1F * _h_ht;
		
		TH2F * _h_razor;
		
		TList * _objList;
		
		TCanvas * _canvas;
		
		//Other variables
		unsigned int check;
		unsigned int vetoCheck;
		
		ofstream logfile;
		
		
		
		//functions
		
		Rivet::Vector3 getVecPt(FourMomentum jet) {
			Rivet::Vector3 vecPt(jet.px(), jet.py(), 0.);
			return vecPt;
		}
		
		Rivet::Vector3 getP3(FourMomentum jet) {
			Rivet::Vector3 vec3(jet.px(), jet.py(), jet.pz());		
			return vec3;
		}
		
		//returns the index of the pseudojet with the max invariant mass (mass2() == mod2()???)
		unsigned int getIndex (const Particles jets) {
			double maxInvMass = 0.;
			unsigned int maxIndex = 0;
			for (unsigned int i = 1; i < jets.size(); ++i) { //index 0 is already assigned to megaJet1
				if ( jets[i].mass() > maxInvMass ) {
					maxInvMass = jets[i].mass();
					maxIndex = i;
				}
			}
			return maxIndex;
		}
		

		/// criterium for assigning jets to hemisphere
		double getStuff(FourMomentum jet, FourMomentum megaJet) {
			double result = 0.;
			double theta = getP3(megaJet).angle( getP3(jet) );
			result = ( megaJet.E() - getP3(megaJet).mod() * cos(theta) ) * megaJet.E() / pow(megaJet.E() + jet.E(), 2);
			return result;
		}
		
		void hemisphere(Particles jets, FourMomentum& megaJet1, FourMomentum& megaJet2) {
			unsigned int maxIndex = getIndex(jets);
			
			/// assign seeds
			
			/// assign the jet with highest pt to megaJet1
			/// assign of the remaining jets the one with max invariant mass to megaJet2

			megaJet1 += jets[0].momentum();
			megaJet2 += jets[maxIndex].momentum();

			/// assign the remaining jets into the 2 hemispheres
			for (unsigned int i = 1; i < jets.size(); ++i) { //index 0 is already assigned to megaJet1
				if ( i != maxIndex) {
					double d1 = getStuff(jets[i].momentum(), megaJet1);
					double d2 = getStuff(jets[i].momentum(), megaJet2);
						
					if ( d1 < d2 )  megaJet1 += jets[i].momentum();
					else megaJet2 += jets[i].momentum();
				}
			}	
		}

		//@}


    };



    // This global object acts as a hook for the plugin system
    AtomPlugin(ATLAS_razor_1)


}
