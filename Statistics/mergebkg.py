#! /usr/bin/env python

from cls import *

# region = "8TeV SR1" etc
def getData(region, data):
	with open("cuts.txt") as f:
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
		else:
			return 0
			
def getBkg(region):
	bkg = 0
	errtmp = 0
		with open("cuts.txt") as f:
			for line in f.readlines():
				if region in line:
					words = line.split(' ')
					if words[0].startswith("bg"):
						bkg += float(words[2])
						errtmp += pow(float(words[3]),2)
			print bkg, ' ', sqrt(errtmp)
			return bkg, sqrt(errtmp)
						
def getDM(region, mass):
	with open("cuts.txt") as f:
		for line in f.readlines():
			if region in line:
				words = line.split(' ')
				for words[0] == "dm012j_"+str(mass)+"/ATLAS_razor_1.root":
					print floor(float((words[2])), ' ', floor(float(words[3]))
					return floor(float((words[2])), floor(float(words[3]))			
					
outfile = open("bkgobs.txt", "w")

outfile.write(mass, " SR1 ", Nbkg, ' ', errNbkg, " 0 0 ", Nobs, ' ', cls, '\n')

outfile.close()

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
	print "8 TeV SR1 difference in limits: "
	print compareLimits("8TeV SR1")
	print "8 TeV SR2 difference in limits: "
	print compareLimits("8TeV SR2")
	print "8 TeV SR3 difference in limits: "
	print compareLimits("8TeV SR3")
	print "8 TeV SR4 difference in limits: "
	print compareLimits("8TeV SR4")
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

