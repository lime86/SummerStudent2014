################################################################
## In principle all you have to setup is defined in this file ##
################################################################
from configManager import configMgr
from ROOT import kBlack,kWhite,kGray,kRed,kPink,kMagenta,kViolet,kBlue,kAzure,kCyan,kTeal,kGreen,kSpring,kYellow,kOrange
from configWriter import TopLevelXML,Measurement,ChannelXML,Sample
from systematic import Systematic
from math import sqrt

# Setup for ATLAS plotting
from ROOT import gROOT
gROOT.LoadMacro("./macros/AtlasStyle.C")
import ROOT
ROOT.SetAtlasStyle()

##########################

# Set observed and expected number of events in counting experiment
ndata     = 123228
nbkg     = 126332.699982
nsig     = 1.
nbkgErr     = 779.511698107
nsigErr     = 0.

# Set systematics for bkg and signal (1 +- relative uncertainties)
ucb = Systematic("ucb", configMgr.weights, 1+0.0220418365592 , 1.-0.0220418365592 , "user", "userOverallSys")

##########################

# Setting the parameters of the hypothesis test
#configMgr.blindSR=True
#configMgr.blindCR=True
configMgr.doExclusion=True # True=exclusion, False=discovery
#configMgr.nTOYs=5000
configMgr.calculatorType=2 # 2=asymptotic calculator, 0=frequentist calculator
configMgr.testStatType=3   # 3=one-sided profile likelihood test statistic (LHC default)
configMgr.nPoints=50       # number of values scanned of signal-strength for upper-limit determination of signal strength.

##########################

# Give the analysis a name
configMgr.analysisName = "MI_SR2_95CL_incl"
configMgr.outputFileName = "results/%s_Output.root"%configMgr.analysisName

# Define cuts
configMgr.cutsDict["UserRegion"] = "1."

#Define weights
configMgr.weights = "1."

#Define samples
bkgSample = Sample("Bkg",kGreen-9)
bkgSample.setStatConfig(True)
bkgSample.setNormByTheory(False)     #this has to be true for samples with normalisation taken from MC, it means include lumi error (set false if data driven)
bkgSample.buildHisto([nbkg],"UserRegion","cuts")
bkgSample.buildStatErrors([nbkgErr],"UserRegion","cuts")
bkgSample.addSystematic(ucb)

sigSample = Sample("Sig",kPink)
sigSample.setNormFactor("mu_Sig",1.0,0.,24645.6)
sigSample.setStatConfig(True)
sigSample.setNormByTheory(False)    #this has to be false since xsec is scaled by mu
sigSample.buildHisto([nsig],"UserRegion","cuts")
sigSample.buildStatErrors([nsigErr],"UserRegion","cuts")

dataSample = Sample("Data",kBlack)
dataSample.setData()
dataSample.buildHisto([ndata],"UserRegion","cuts")

# Define top-level
ana = configMgr.addFitConfig("SPlusB")
ana.addSamples([bkgSample,sigSample,dataSample])
ana.setSignalSample(sigSample)

# Define measurement
meas = ana.addMeasurement(name="NormalMeasurement",lumi=1.0,lumiErr=0.028)
meas.addPOI("mu_Sig")
#meas.addParamSetting("Lumi",True,1)   #you only need this if you want to turn off lumi uncertainty copmpletely

# Add the channel
chan = ana.addChannel("cuts",["UserRegion"],1,0.,1.)
ana.setSignalChannels([chan])

# These lines are needed for the user analysis to run
# Make sure file is re-made when executing HistFactory
if configMgr.executeHistFactory:
    if os.path.isfile("data/%s.root"%configMgr.analysisName):
        os.remove("data/%s.root"%configMgr.analysisName)

