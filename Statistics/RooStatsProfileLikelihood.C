using namespace RooStats;

void RooStatsProfileLikelihood(
   const char* infile =  "CountingModel.root",
   const char* workspaceName = "w",
   const char* modelConfigName = "ModelConfig",
   const char* dataName = "data" )
{
  /////////////////////////////////////////////////////////////
  // First part is just to access the workspace file
  ////////////////////////////////////////////////////////////

  // open input file
  TFile *file = TFile::Open(infile);
  if (!file) return;

  // get the workspace out of the file
  RooWorkspace* w = (RooWorkspace*) file->Get(workspaceName);


  // get the modelConfig out of the file
  RooStats::ModelConfig* mc = (RooStats::ModelConfig*) w->obj(modelConfigName);

  // get the modelConfig out of the file
  RooAbsData* data = w->data(dataName);


  ProfileLikelihoodCalculator pl(*data,*mc);
  pl.SetConfidenceLevel(0.95); // 95% interval
  LikelihoodInterval* interval = pl.GetInterval();

   // find the iterval on the first Parameter of Interest
  RooRealVar* firstPOI = (RooRealVar*) mc->GetParametersOfInterest()->first();

  double lowerLimit = interval->LowerLimit(*firstPOI);
  double upperLimit = interval->UpperLimit(*firstPOI);


  cout << "\n95% interval on " <<firstPOI->GetName()<<" is : ["<<
    lowerLimit << ", "<<
    upperLimit <<"] "<<endl;


  LikelihoodIntervalPlot * plot = new LikelihoodIntervalPlot(interval);
  plot->SetMaximum(5);
  //plot->SetNPoints(50);  // do not use too many points, it could become very slow for some models
  plot->Draw("");  // use option TF1 if too slow (plot.Draw("tf1")

}
