// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/VetoedFinalState.hh"
#include "Rivet/Projections/FastJets.hh"
#include "Rivet/Projections/MissingMomentum.hh"

#define ETAMIN -4.5
#define ETAMAX 4.5
#define MINPT 0.5
#define R 0.5

#define JET_ETA 3.
#define JET_PT 40.

/// @todo Include more projections as required, e.g. ChargedFinalState, FastJets, ZFinder...

namespace Rivet {


  class Razor : public Analysis {
  public:

    /// Constructor
    Razor() : Analysis("Razor")
    {    }


  public:

    /// @name Analysis methods
    //@{

    /// Book histograms and initialise projections before the run
	void init() {
		FinalState fs(ETAMIN, ETAMAX, MINPT*GeV); //FinalState (double mineta=-MAXDOUBLE, double maxeta=MAXDOUBLE, double minpt=0.0 *GeV)
		addProjection(fs, "FS");  

		VetoedFinalState vfs(fs); //veto for MET
		//VetoedFinalState vfsJets(fs); //veto for Jets

		/// Veto particles which shouldn't go into E_T^{miss} calculation
		/// or jets (won't show up in calorimeter = muons + neutrinos + neutralinos + chis)
		/// VetoedFinalState & 	addVetoPairDetail (const long id, const double ptmin, const double ptmax)

		//pt range in GeV???
		//vfs.addVetoPairDetail(11, 0., 5.); /// electrons, veto for 0 < pt < 5GeV criteria from RazorLikelihoodHowTo
		//vfs.addVetoPairDetail(13, 5., 10000.); /// muon, veto for 5GeV < pt (< 10GeV) criteria from RazorLikelihoodHowTo
		
		vfs.addVetoPairId(13); //muons
		
		// neutrinos
		vfs.addVetoPairId(12);
		vfs.addVetoPairId(14);
		vfs.addVetoPairId(16);

		// neutralinos
		vfs.addVetoPairId(1000022);
		vfs.addVetoPairId(1000023);
		vfs.addVetoPairId(1000025);
		vfs.addVetoPairId(1000035);

		vfs.addVetoPairId(9000010); //chi

		addProjection(vfs, "VETOEDFS");

		MissingMomentum etmiss(vfs);
		addProjection(etmiss, "ETMISS");

		addProjection(FastJets(vfs, FastJets::ANTIKT, R), "Jets"); //??
		//      addProjection(FastJets(cfs, FastJets::ANTIKT, R), "ChargedJets");

		_hist1d["MET_TRG"]		= bookHisto1D("trigger_MET", 50, 0., 1000., "Missing E_T (trigger level)", "E_{T}^{miss}", "Relative Occurrence");
		_hist1d["JET_MULT"]		= bookHisto1D("jet_mult", 50, 0., 50., "Jet Multiplicity", "Jet Multiplicity", "Relative Occurrence");
		_hist1d["JET_MULT_CUT"]	= bookHisto1D("jet_mult_cut", 50, 0., 50., "Jet Multiplicity after eta cut", "Jet Multiplicity", "Relative Occurrence");
		_hist1d["RAZOR_MR"]		= bookHisto1D("razor_mr", 14, 0., 1400., "Razor Variable M_R", "M_R", "Relative Occurrence");
		_hist1d["RAZOR_R2"]		= bookHisto1D("razor_r2", 14, 0., 1.4, "Razor Variable R^2", "R^2", "Relative Occurrence");
		//_hist2d["RAZOR"]		= bookHisto2D("razor", 14, 0., 1400., 14, 0., 1.4, "Razor Plot", "M_R (GeV)", "R^2", "");
		_profile1d["RAZOR"]		= bookProfile1D("razor", 14, 0., 1.4, "Razor Plot", "M_R (GeV)", "R^2");

    }


	void analyze(const Event& event) {

		const double weight = event.weight();

		// Get missing E_T vector and veto if magnitude is
		// less than 40 GeV (trigger condition in analysis)
		const Vector3& missingET = - getVecPt(applyProjection<MissingMomentum>(event, "ETMISS").missingEt());
		if ( missingET.mod() < 40. ) vetoEvent;
		_hist1d["MET_TRG"]->fill(missingET.mod(), weight);
		
		/// CUT 1 -- MISSING ET > 40 GeV


		const FastJets& fastjets = applyProjection<FastJets>(event, "Jets");

		//What are pseudojets and why pseudojets? maybe leave out?
		PseudoJets jets40 = fastjets.pseudoJetsByPt(JET_PT*GeV); /// Consider all jets with pt > 40 GeV from RazorLikelihoodHowTo
		unsigned int jetMult = jets40.size();
		_hist1d["JET_MULT"]->fill(jetMult, weight);

		/* //can't erase?
		for (int i = jetMult; i > 0; --i) { //go through all jets40 backwards to remove wrong eta jets
		  if ( abs(jets40.at(i).eta()) >= JET_ETA) jets40.erase(i);
		}
		*/
		
		unsigned int jetMultCut = jets40.size();
		_hist1d["JET_MULT_CUT"]->fill(jetMultCut, weight);
		
		/// Megajets
		fastjet::PseudoJet megaJet1(0., 0., 0., 0.);
		fastjet::PseudoJet megaJet2(0., 0., 0., 0.);

		/// Razor Variables
		double mr = 0.;
		double mrt = 0.;
		double r = 0.;

		if ( jets40.size() < 2 ) vetoEvent; /// at least 2 jets
		else { /// leading and subleading jets > 60 GeV
			if ( jets40.at(0).perp() > 60 && jets40.at(1).perp() > 60 ) {
				if ( jets40.size() == 2 ) {
					megaJet1 = jets40.at(0);
					megaJet2 = jets40.at(1);
				}
				else {
					hemisphere(jets40, megaJet1, megaJet2);
				}
				
				//get the pt vector from a pseudojet
				Vector3 pt_j1 = getVecPt(megaJet1);
				Vector3 pt_j2 = getVecPt(megaJet2);
				
				//vector multiplication: Vector3 class
				mr		= sqrt( pow(( megaJet1.E() + megaJet2.E() ), 2) - pow(( megaJet1.pz() + megaJet2.pz() ), 2) );
				mrt		= sqrt( ( missingET.mod() * ( megaJet1.perp() + megaJet2.perp() ) - missingET.dot(add(pt_j1, pt_j2)) ) / 2.);
				r		= mrt/mr;
				
				_hist1d["RAZOR_MR"]->fill(mr, weight);
				_hist1d["RAZOR_R2"]->fill(r*r, weight);
				//_hist2d["RAZOR"]->fill(mr, r*r, weight);
				//_profile1d["RAZOR"]->fill(mr, r*r, 1.0);
			}
			else vetoEvent;
		}
	}

    /// Normalise histograms etc., after the run
	void finalize() {

	  /// @todo Normalise, scale and otherwise manipulate histograms here

	  // scale(_h_YYYY, crossSection()/sumOfWeights()); // norm to cross section
	  // normalize(_h_YYYY); // normalize to unity

	}

    //@}


  private:

    // Data members like post-cuts event weight counters go here


  private:

    /// @name Histograms
    std::map<std::string, Histo1DPtr> _hist1d;
    //std::map<std::string, Histo2DPtr> _hist2d;
    std::map<std::string, Profile1DPtr> _profile1d;

	Vector3 getVecPt(fastjet::PseudoJet jet) {
		Vector3 vecPt(jet.px(), jet.py(), 0.);
		return vecPt;
	}
	
	Vector3 getVecPt(FourMomentum jet) {
		Vector3 vecPt(jet.px(), jet.py(), 0.);
		return vecPt;
	}
	
	Vector3 getVector3(fastjet::PseudoJet pseudoJet) {
		Vector3 vec3(pseudoJet.px(), pseudoJet.py(), pseudoJet.pz());		
		return vec3;
	}
	
	//returns the index of the pseudojet with the max invariant mass (mass2() == mod2()???)
	unsigned int getIndex (PseudoJets pseudoJets) {
		double maxInvMass = 0.;
		unsigned int maxIndex = 0;
		for (unsigned int i = 1; i < pseudoJets.size(); ++i) { //index 0 is already assigned to megaJet1
			if ( pseudoJets.at(i).m() > maxInvMass ) {
				maxInvMass = pseudoJets.at(i).m();
				maxIndex = i;
			}
		}
		return maxIndex;
	}
	

	/// criterium for assigning jets to hemisphere
	double getStuff(fastjet::PseudoJet jet, fastjet::PseudoJet megaJet) {
		double result = 0.;
		double theta = getVector3(megaJet).angle( getVector3(jet) );
		result = ( megaJet.E() - getVector3(megaJet).mod() * cos(theta) ) * megaJet.E() / pow(megaJet.E() + jet.E(), 2);
		return result;
	}
	
	void hemisphere(PseudoJets pseudoJets, fastjet::PseudoJet& megaJet1, fastjet::PseudoJet& megaJet2) {
		unsigned int maxIndex = getIndex(pseudoJets);
		
		/// assign seeds
		
		/// assign the jet with highest pt to megaJet1
		/// assign of the remaining jets the one with max invariant mass to megaJet2
		//add(megaJet1, pseudoJets.at(0));
		//add(megaJet2, pseudoJets.at(maxIndex));
		megaJet1 += pseudoJets.at(0);
		megaJet2 += pseudoJets.at(maxIndex);

		/// assign the remaining jets into the 2 hemispheres
		for (unsigned int i = 1; i < pseudoJets.size(); ++i) { //index 0 is already assigned to megaJet1
			if ( i != maxIndex) {
				double d1 = getStuff(pseudoJets.at(i), megaJet1);
				double d2 = getStuff(pseudoJets.at(i), megaJet2);
					
				if ( d1 < d2 )  megaJet1 += pseudoJets.at(i);
				else megaJet2 += pseudoJets.at(i);
			}
		}	
	}

		//@{
		Profile1DPtr _h_XXXX;
		Histo1DPtr _h_YYYY;
		//Histo2DPtr _h_ZZZZ;
		//@}

	};

	// The hook for the plugin system
	DECLARE_RIVET_PLUGIN(Razor);
}
