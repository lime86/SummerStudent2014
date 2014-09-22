#! /usr/bin/env python

from cls import *
from datetime import datetime
	
def getBkg(region):
	bkg = 0.
	errtmp = 0.
	with open("cuts.txt") as f:
		for line in f.readlines():
			if region in line:
				words = line.split(' ')
				if words[0].startswith("bg"):
					#print words[0]+' '+words[1]
					bkg += float(words[2])
					errtmp += pow(float(words[3]),2)
		#print bkg, ' ', sqrt(errtmp)
		return bkg, sqrt(errtmp)
						
def getDM(region, mass):
	with open("cuts.txt") as f:
		for line in f.readlines():
			if region in line:
				words = line.split(' ')
				if words[0] == "Med1000_DM"+str(mass)+"/ATLAS_razor_1.root":
					#print float(words[2]), ' ', float(words[3])
					#return float(words[2]), float(words[3])
					return float(words[2])

def main():
	outfile = open("bkgobs.txt", "w")
	outfile.write("Dataset SR Nbkg errNbkg1 errNbkg2 errNbkg3 Obs 95%obs\n")
	Nbkg = 0.
	errNbkg = 0.
	Nobs = 0.
	cls = 0.
	#dmMass = [0.01, 0.1, 1, 10, 50, 100, 200, 300, 400, 500, 600, 700, 800, 900, 1000, 1100, 1200, 1300]
	dmMass = [1, 10, 50, 100, 200, 300, 400, 500]
	for mass in dmMass:
		for i in range(1,5):
			Nbkg, errNbkg = getBkg("SR"+str(i))
			Nobs = getDM("SR"+str(i), mass) + Nbkg
			#print "SR"+str(i), Nbkg, errNbkg, Nobs
			print datetime.now()
			cls = calcCLs(Nbkg, [errNbkg, 0, 0], Nobs, 0.95)
			print datetime.now(), "SR"+str(i), Nbkg, errNbkg, Nobs, cls
			string = "8TeV_"+str(mass)+"GeV SR"+str(i)+" "+str(Nbkg)+" "+str(errNbkg)+" 0 0 "+str(Nobs)+" "+str(cls)+"\n"
			outfile.write(string)
	
	outfile.close()

if __name__ == "__main__":
  main()

