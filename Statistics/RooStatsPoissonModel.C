#include <sstream>

// This should be a reasonably safe way to cast to string
// (Don't want to introduce boost dependency for lexical_cast)
template<class N>
std::string to_str (N number){
    std::ostringstream buff;
    buff<<number;
    return buff.str();
}

using namespace RooFit;
using namespace RooStats;


void RooStatsPoissonModel()
//void RooStatsPoissonModel(int b, double sigmab, int nobs, int lowerBackground, int upperBackground, int lowerSignal, int upperSignal)
{

//   int nobs = 124703;             // number of observed events
//   double b = 124000;             // number of background events
//   double sigmab = 4000;         // uncertainty in b (estimated as Gaussian)
//   int lowerBackground = 50000;   // lower range for background
//   int upperBackground = 200000; // same but upper
//   int lowerSignal = 0;       // "guess" of lower range for signal (only used for plotting)
//   int upperSignal = 11000;      // same but upper

//   int nobs = 8631;             // number of observed events
//   double b = 8800;             // number of background events
//   double sigmab = 400;         // uncertainty in b (estimated as Gaussian)
//   int lowerBackground = 4000;   // lower range for background
//   int upperBackground = 14000; // same but upper
//   int lowerSignal = 0;       // "guess" of lower range for signal (only used for plotting)
//   int upperSignal = 2000;      // same but upper

//   int nobs = 785;             // number of observed events
//   double b = 750;             // number of background events
//   double sigmab = 60;         // uncertainty in b (estimated as Gaussian)
//   int lowerBackground = 400;   // lower range for background
//   int upperBackground = 1200; // same but upper
//   int lowerSignal = 0;       // "guess" of lower range for signal (only used for plotting)
//   int upperSignal = 500;      // same but upper

//   int nobs = 77;             // number of observed events
//   double b = 83;             // number of background events
//   double sigmab = 14;         // uncertainty in b (estimated as Gaussian)
//   int lowerBackground = 0;   // lower range for background
//   int upperBackground = 500; // same but upper
//   int lowerSignal = 0;       // "guess" of lower range for signal (only used for plotting)
//   int upperSignal = 500;      // same but upper

//   int nobs = 350932;             // number of observed events
//   double b = 344400;             // number of background events
//   double sigmab = 12822;         // uncertainty in b (estimated as Gaussian)
//   int lowerBackground = 300000;   // lower range for background
//   int upperBackground = 400000; // same but upper
//   int lowerSignal = 0;       // "guess" of lower range for signal (only used for plotting)
//   int upperSignal = 50000;      // same but upper

//   int nobs = 25515;             // number of observed events
//   double b = 25600;             // number of background events
//   double sigmab = 1057;         // uncertainty in b (estimated as Gaussian)
//   int lowerBackground = 15000;   // lower range for background
//   int upperBackground = 35000; // same but upper
//   int lowerSignal = 0;       // "guess" of lower range for signal (only used for plotting)
//   int upperSignal = 5000;      // same but upper
//
//   int nobs = 2353;             // number of observed events
//   double b = 2180;             // number of background events
//   double sigmab = 171;         // uncertainty in b (estimated as Gaussian)
//   int lowerBackground = 500;   // lower range for background
//   int upperBackground = 5000; // same but upper
//   int lowerSignal = 0;       // "guess" of lower range for signal (only used for plotting)
//   int upperSignal = 1000;      // same but upper
//
   int nobs = 268;             // number of observed events
   double b = 380;             // number of background events
   double sigmab = 73;         // uncertainty in b (estimated as Gaussian)
   int lowerBackground = 0;   // lower range for background
   int upperBackground = 600; // same but upper
   int lowerSignal = 0;       // "guess" of lower range for signal (only used for plotting)
   int upperSignal = 500;      // same but upper



   RooWorkspace w("w");

// make Poisson model * Gaussian constraint

   std::string nexp = "sum:nexp(s["+to_str(0)+","+to_str(lowerSignal)+","+to_str(upperSignal)+"],b["+to_str(b)+","+to_str(lowerBackground)+","+to_str(upperBackground)+"])";
   std::string poisson = "Poisson:pdf(nobs["+to_str(lowerBackground)+","+to_str(upperBackground)+"],nexp)";
   std::string constraint = "Gaussian:constraint(b0["+to_str(lowerBackground)+","+to_str(upperBackground)+"],b,sigmab[1])";;

   w.factory(nexp.c_str());
   w.factory(poisson.c_str());
   w.factory(constraint.c_str());
// Poisson of (n | s+b)
   w.factory("PROD:model(pdf,constraint)");


   w.var("b0")->setVal(b);
   w.var("b0")->setConstant(true); // needed for being treated as global observables
   w.var("sigmab")->setVal(sigmab);


   ModelConfig mc("ModelConfig",&w);
   mc.SetPdf(*w.pdf("model"));
   mc.SetParametersOfInterest(*w.var("s"));
   mc.SetObservables(*w.var("nobs"));
   w.defineSet("nuisParams","b");
   mc.SetNuisanceParameters(*w.set("nuisParams"));

   // these are needed for the hypothesis tests
   mc.SetSnapshot(*w.var("s"));
   mc.SetGlobalObservables(*w.var("b0"));

   mc.Print();
   // import model in the workspace
   w.import(mc);

   // make data set with the namber of observed events
   RooDataSet data("data","", *w.var("nobs"));
   w.var("nobs")->setVal(nobs);
   data.add(*w.var("nobs") );
   // import data set in workspace and save it in a file
   w.import(data);

   w.Print();

   TString fileName = "CountingModel.root";

   // write workspace in the file (recreate file if already existing)
   w.writeToFile(fileName, true);

}
