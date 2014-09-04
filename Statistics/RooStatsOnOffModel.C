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


void RooStatsOnOffModel()
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

   int nobs = 77;             // number of observed events
   double b = 83;             // number of background events
   double sigmab = 14;         // uncertainty in b (estimated as Gaussian)
   int lowerBackground = 0;   // lower range for background
   int upperBackground = 500; // same but upper
   int lowerSignal = 0;       // "guess" of lower range for signal (only used for plotting)
   int upperSignal = 500;      // same but upper

   RooWorkspace w("w",true);

   w.factory("sum:nexp(s[0,0,50],b[1,0,3])");
   // Poisson of (n | s+ b)
   w.factory("Poisson:pdf(nobs[0,150],nexp)");

   // use tau :  y = tau*b
   w.factory("prod:y(tau[10],b)");
   w.factory("prod:y0(tau,b0[83,0,150])");
   w.factory("Poisson:off_pdf(y0,y)");
   w.factory("PROD:model(pdf,off_pdf)");

   RooRealVar * obs = w.var("nobs");
   w.var("b0")->setVal(b);
   w.var("b0")->setConstant(true); // needed for being treated as global observables

   //set value of observed events
   obs->setVal(nobs);

   // make data set with the namber of observed events
   RooDataSet data("data","", *obs );
   data.add(*obs );
   w.import(data);

   w.Print();

//   w.factory(nexp.c_str());
//   w.factory(poisson.c_str());
//   w.factory(constraint.c_str());
// Poisson of (n | s+b)
//   w.factory("PROD:model(pdf,constraint)");


//   w.var("b0")->setVal(b);
//   w.var("b0")->setConstant(true); // needed for being treated as global observables
//   w.var("sigmab")->setVal(sigmab);

   ModelConfig mc("ModelConfig",&w);
   mc.SetPdf(*w.pdf("model"));
   mc.SetParametersOfInterest(*w.var("s"));
   mc.SetObservables(*w.var("nobs"));
   mc.SetNuisanceParameters(*w.var("b"));

   // these are needed for the hypothesis tests
   mc.SetSnapshot(*w.var("s"));
   mc.SetGlobalObservables(*w.var("b0"));

   mc.Print();
   // import model in the workspace
   w.import(mc);


//   ModelConfig mc("ModelConfig",&w);
//   mc.SetPdf(*w.pdf("model"));
//   mc.SetParametersOfInterest(*w.var("s"));
//   mc.SetObservables(*w.var("nobs"));
//   w.defineSet("nuisParams","b");
//   mc.SetNuisanceParameters(*w.set("nuisParams"));
//
//   // these are needed for the hypothesis tests
//   mc.SetSnapshot(*w.var("s"));
//   mc.SetGlobalObservables(*w.var("b0"));
//
//   mc.Print();
//   // import model in the workspace
//   w.import(mc);
//
//   // make data set with the namber of observed events
//   RooDataSet data("data","", *w.var("nobs"));
//   w.var("nobs")->setVal(nobs);
//   data.add(*w.var("nobs") );
//   // import data set in workspace and save it in a file
//   w.import(data);
//
//   w.Print();

   TString fileName = "CountingModel.root";

   // write workspace in the file (recreate file if already existing)
   w.writeToFile(fileName, true);

}
