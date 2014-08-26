// -*- C++ -*-
#include <iostream>

///ATOM
#include "Atom/AtomAnalysis.hh"
#include "Atom/Detectors/Atlas.hh"
#include "Atom/Detectors/Atlas2011.hh"
#include "Atom/Projections/Razor.hh"
#include "Atom/VariableCut.hh"
#include "Atom/EfficiencyHelper.hh"
#include "Atom/XSecHelper.hh"


///ROOT
#include "TH1.h"
#include "TH2.h"
#include "TPDF.h"
#include "TCanvas.h"
#include "TSystem.h"
#include "TROOT.h"

using namespace std;

namespace Atom {


    class ATLAS_razor_0 : public AtomAnalysis {
    public:

        /// @name Constructors etc.
        //@{

        /// Constructor
        ATLAS_razor_0()
            : AtomAnalysis("ATLAS_razor_0"),
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
            
            //IsoMuon muonBase(const RangeSelector& filter, double isoRadius, double isoPTCut, double isoCutConst) 
            //IsoMuon mu_pre = _atlas.muonBase(_atlas.combMuonRange(), 0.2, 0.0, 1.8);
            
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
            
            Razor razr(razorparticles, met);
            addProjection(razr, "Razor");
            
            /// @todo Book histograms here, e.g.:
            
			///ROOT histograms
			_h_met			= new TH1F("met", "Missing Energy", 200,0,200);
			_h_met->GetXaxis()->SetTitle("MET (GeV)");
			_h_met->GetYaxis()->SetTitle("Occurrence");
			
			_h_metTrg		= new TH1F("metTrg", "Missing Energy Trigger", 150,0,1500);
			_h_metTrg->GetXaxis()->SetTitle("MET (GeV)");
			_h_metTrg->GetYaxis()->SetTitle("Occurrence");

			_h_vetoedEvts	= new TH1F("vetoedEvts", "Vetoed Events", 2,0,2);
			_h_vetoedEvts->GetXaxis()->SetTitle("Vetoed Events");
			_h_vetoedEvts->GetYaxis()->SetTitle("Occurrence");
			
			_h_jetMult		= new TH1F("jetMult", "Jet Multiplicity", 10,0,10);
			_h_jetMult->GetXaxis()->SetTitle("Jet Multiplicity");
			_h_jetMult->GetYaxis()->SetTitle("Occurrence");
			
			_h_ht				= new TH1F("ht", "HT", 800,0,8000); //scalar sum of pt in all jets
			_h_ht->GetXaxis()->SetTitle("HT (GeV)");
			_h_ht->GetYaxis()->SetTitle("Occurrence");
			
			_h_leadingJetPt		= new TH1F("leadingJetPt", "Leading Jet PT", 400,0,4000);
			_h_leadingJetPt->GetXaxis()->SetTitle("Leading Jet PT (GeV)");
			_h_leadingJetPt->GetYaxis()->SetTitle("Occurrence");
			
			_h_subleadingJetPt	= new TH1F("subleadingJetPt", "Subleading Jet PT", 400,0,4000);
			_h_subleadingJetPt->GetXaxis()->SetTitle("Subleading Jet PT (GeV)");
			_h_subleadingJetPt->GetYaxis()->SetTitle("Occurrence");
			
			_h_otherJetPt		= new TH1F("otherJetPt", "Other Jet PT", 400,0,4000);
			_h_otherJetPt->GetXaxis()->SetTitle("Other Jet PT (GeV)");
			_h_otherJetPt->GetYaxis()->SetTitle("Occurrence");
	
			_h_razor			= new TH2F("razorPlot", "Razor Plot", 200, 0., 2000., 100, 0., 1);
			_h_razor->GetXaxis()->SetTitle("M_R (GeV)");
			_h_razor->GetYaxis()->SetTitle("R2");
			_h_razor->GetZaxis()->SetTitle("Occurrence");
			
			_canvas = new TCanvas("Canvas");
			
			check = 0;
			vetoCheck = 0;
			
			logfile.open ("ATLAS_razor_0.log", ios::trunc);

			cout << "Finished initialisation" << endl;

        }


        /// Perform the per-event analysis
        bool analyzeLocal(const Event& event, const double weight) {
			unsigned int jetMult = 0;
			double rval = 0.0;
            double mrval = 0.0;
            
            // get jets
            const Particles jets = applyProjection<NearIsoParticle>(event, "Jets").particlesByPt();
            const Particles eles = applyProjection< FastSimFinalState<IsoElectron> >(event, "IsoElectrons").particlesByPt();
            const Particles mus = applyProjection< FastSimFinalState<IsoMuon> >(event, "IsoMuons").particlesByPt();
            const Particles razr = applyProjection<MergedFinalState>(event, "RazorParticles").particlesByPt();
            const MissingMomentum pmet = applyProjection<MissingMomentum>(event, "MissingET");
            const FourMomentum met = pmet.missingEt();
 
            // get Ht & co.
            const HTTools Htools = applyProjection<HTTools>(event, "Ht");
            const Razor razor = applyProjection<Razor>(event, "Razor");
            
            // fill histograms
            jetMult = jets.size();
            //_h_met->Fill(met.mod(), 1.);
            _h_met->Fill(met.mod(), weight);
            _h_jetMult->Fill(jetMult, 1.);
            
            ///jet multiplicity cut
			if (jets.size() >= 2 ) {
				//++vetoCheck;
				//_h_vetoedEvts->Fill(1.,1.);
				//vetoEvent;
				
				_h_leadingJetPt->Fill(jets[0].momentum().pT(), 1.);
				_h_subleadingJetPt->Fill(jets[1].momentum().pT(), 1.);
				
				for (unsigned int i = 2; i < jetMult; ++i) {
					_h_otherJetPt->Fill(jets[i].momentum().pT(), 1.);
				}
			}
            

            _h_ht->Fill(Htools.HT(),1.);
 
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
							
            /// @todo Check object reco in ATLAS
            
            FourMomentum j1 = razor.getHemisphere(0);
            FourMomentum j2 = razor.getHemisphere(1);
            
			rval = razor.getR();
			mrval = razor.getMR();
                        
            //      std::cout << "Jets: " << jets.size() << std::endl;
            //      for(int i=0; i<jets.size(); i++) std::cout << "j" << i << ": " << jets[i].momentum() << ", pT="<<jets[i].momentum().pT() <<", phi=" << jets[i].momentum().phi() <<", eta=" << jets[i].momentum().eta() << std::endl;
            //      std::cout << "met: " << met << ", pT="<<met.pT() <<", phi=" << (met.phi()+PI) << std::endl;
            //      std::cout << "Hemi: " << j1 << ", " << j2 << ", br=" << razor.getBetaR() << ", dphi=" << deltaPhi(j1.phi(),j2.phi()) << ", R=" << razor.getR() << ", mR=" << razor.getMR()<< std::endl;
            //      std::cout << "----\n";

			/*
            if (!_vcut.ApplyCut(fabs(razor.getBetaR()), CUT_LT, 0.99, "betaR")) {
                vetoEvent;
            }


            if (!_vcut.ApplyCut(deltaPhi(j1.phi(), j2.phi()), CUT_LT, 2.8, "DeltaPhi_JJ")) {
                vetoEvent;
            }
			*/

            
            _effh.PassEvent("Efficiency of the Baseline Selection");

			//_fill_hist1D(_h_razorMr, mrval, weight);
			//_fill_hist1D(_h_razorR2, rval*rval, weight);
			
			
			_h_vetoedEvts->Fill(0., 1.);			
			_h_razor->Fill(mrval, rval*rval, 1.);
			
			++check;
			//logfile << "Evaluated events: "<< check << ". Vetoed events: "<< vetoCheck << "|| MET = "<< met.mod() << "| Leading Jet pt = " << jets[0].momentum().pT() << "| R = " << rval <<endl;
			
            return true;
        }


        /// Normalise histograms etc., after the run
        void finalizeLocal() {
			_canvas->cd(0);
			_canvas->Print("ATLAS_razor_0.pdf[", "pdf");
	
			_h_met->Draw();
			_canvas->Print("ATLAS_razor_0.pdf", "pdf");
			_canvas->Print("ATLAS_razor_0_met.C");	

			_h_vetoedEvts->Draw();
			_canvas->Print("ATLAS_razor_0.pdf", "pdf");
			_canvas->Print("ATLAS_razor_0_vetoedEvts.C");
			
			_h_jetMult->Draw();
			_canvas->Print("ATLAS_razor_0.pdf", "pdf");
			_canvas->Print("ATLAS_razor_0_jetMult.C");
			
			_h_leadingJetPt->Draw();
			_canvas->Print("ATLAS_razor_0.pdf", "pdf");
			_canvas->Print("ATLAS_razor_0_leadingJetPt.C");
			
			_h_subleadingJetPt->Draw();
			_canvas->Print("ATLAS_razor_0.pdf", "pdf");
			_canvas->Print("ATLAS_razor_0_subleadingJetPt.C");
			
			_h_otherJetPt->Draw();
			_canvas->Print("ATLAS_razor_0.pdf", "pdf");
			_canvas->Print("ATLAS_razor_0_otherJetPt.C");
			
			_h_ht->Draw();
			_canvas->Print("ATLAS_razor_0.pdf", "pdf");
			_canvas->Print("ATLAS_razor_0_ht.C");
			
			_h_razor->Draw("colz");
			_canvas->Print("ATLAS_razor_0.pdf", "pdf");
			_canvas->Print("ATLAS_razor_0_razor.C");
			
			_canvas->Print("ATLAS_razor_0.pdf]", "pdf");
			//_canvas->Print();

            _luminosity = std::make_pair(35.0 / picobarn, 0.11);
            double norm = crossSection() * _luminosity.first / sumOfWeights();

			logfile.close();
			
            cout << "Finalising..."<<endl;
        }

        //@}


    private:

        ATLASProjections _atlas;

    private:

        /// @name Histograms
		
		//@{


		//Histo1DPtr _h_metTrg;
		//Histo1DPtr _h_jetMult;
		//Histo1DPtr _h_jetMultCut;
		//Histo1DPtr _h_razorMr;
		//Histo1DPtr _h_razorR2;
		

		//ROOT histograms
		TH1F * _h_met;
		TH1F * _h_metTrg;
		TH1F * _h_vetoedEvts;
		
		TH1F * _h_jetMult;
		TH1F * _h_leadingJetPt;
		TH1F * _h_subleadingJetPt;
		TH1F * _h_otherJetPt;
		TH1F * _h_ht;
		
		TH2F * _h_razor;
		
		TCanvas * _canvas;
		
		//Other variables
		unsigned int check;
		unsigned int vetoCheck;
		
		ofstream logfile;

		//@}


    };



    // This global object acts as a hook for the plugin system
    AtomPlugin(ATLAS_razor_0)


}
