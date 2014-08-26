#! /usr/bin/env python

from cls import *

# region = "8TeV SR1" etc
def getData(region, data):
  with open("bkgobs.txt") as f:
    for line in f.readlines():
      if region in line:
        words = line.split(' ')
        if data == "bkg":
          return float(words[2])
        if data == "errbkg1":
          return float(words[3])
        if data == "errbkg2":
          return float(words[4])
        if data == "errbkg3":
          return float(words[5])
        if data == "obs":
          return float(words[6])
        if data == "lim":
          return float(words[7])

# region = "8TeV SR1" etc
def get14TeVIntegral(metcut):
  integral = 0.
  with open("plot.csv") as f:
    for line in f.readlines():
      words = line.split(' ')
      if int(words[1]) >= metcut:
        integral += float(words[0])*(float(words[2]) - float(words[1]))
  return integral

def get14TeVData(metcut, systerrPercent, femtobarn = 20):
  nEv = get14TeVIntegral(metcut) * (femtobarn / 20.)
  staterr = sqrt(nEv)
  systerr = nEv * systerrPercent
  obs = nEv
  lim = calcCLs(nEv, [staterr, systerr], obs, 0.95)
  return bkg, staterr, systerr, 0, obs, lim

def getAllData(region):
  bkg = getData(region, "bkg")
  errbkg1 = getData(region, "errbkg1")
  errbkg2 = getData(region, "errbkg2")
  errbkg3 = getData(region, "errbkg3")
  obs = getData(region, "obs")
  lim = getData(region, "lim")
  return bkg, errbkg1, errbkg2, errbkg3, obs, lim

def compareLimits(region):
  bkg, errbkg1, errbkg2, errbkg3, obs, lim = getAllData(region)
  ownCLs = calcCLs(bkg,[errbkg1, errbkg2, errbkg3], obs, 0.95)
#  print ownCLs
#  print lim
  return 100 * (ownCLs - lim)/lim

def main():
  print calcCLs(5361,[53,268],5361,0.95)
  print calcCLs(714,[8,36],714,0.95)
  print calcCLs(137,[4,7],137,0.95)
#  print "7 TeV SR1 difference in limits: "
#  print compareLimits("7TeV SR1")
#  print "7 TeV SR2 difference in limits: "
#  print compareLimits("7TeV SR2")
#  print "7 TeV SR3 difference in limits: "
#  print compareLimits("7TeV SR3")
#  print "7 TeV SR4 difference in limits: "
#  print compareLimits("7TeV SR4")
#  print "8 TeV SR1 difference in limits: "
#  print compareLimits("8TeV SR1")
#  print "8 TeV SR2 difference in limits: "
#  print compareLimits("8TeV SR2")
#  print "8 TeV SR3 difference in limits: "
#  print compareLimits("8TeV SR3")
#  print "8 TeV SR4 difference in limits: "
#  print compareLimits("8TeV SR4")
#  print "CMS 8 TeV SR1 difference in limits: "
#  print compareLimits("9TeV SR1")
#  print "CMS 8 TeV SR2 difference in limits: "
#  print compareLimits("9TeV SR2")
#  print "CMS 8 TeV SR3 difference in limits: "
#  print compareLimits("9TeV SR3")
#  print "CMS 8 TeV SR4 difference in limits: "
#  print compareLimits("9TeV SR4")


if __name__ == "__main__":
  main()

