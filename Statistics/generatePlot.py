#! /usr/bin/env python

import sys
import os
import os.path
from math import *
from cls import *
from stats import *
from Invdisttree import Invdisttree
import numpy as np
from scipy import interpolate
import matplotlib.pyplot as plt
import matplotlib.colors as mc

def plotGrid(xgrid, ygrid, zs, dm, med, folder, sqrts, name = None):
  print "plotGrid\n"
  
  font = {#'family' : 'cursive',
          #'weight' : 'bold',
          'size'   : 16}
  plt.rc('font', **font)
  levels = [0, 2, 4, 6, 8, 10, 4*pi, 100]
  plt.contourf(xgrid,ygrid,zs, levels=levels, colors=('#9900ff', '#0000ff', '#0099ff', '#00ff99', '#00ff00', '#99ff00', '#ff9900', '#ff0000')) #reshape Z too!
  plt.xlabel("$M_\chi$ [GeV]")
  plt.ylabel("$M_\\xi$ [GeV]")
  plt.figtext(0.6,0.8, "$\Gamma_\\xi = M_\\xi / 8 \pi$")
  cbar = plt.colorbar()
  ticklabels = ["0", "2", "4", "6", "8", "10", "4 $\pi$"]
  cbar.set_ticklabels(ticklabels)
  cbar.ax.set_ylabel("95% C.L. upper limit on $\sqrt{g_q g_\chi}$")
  coords_filled = list()
  coords_notfilled = list()
  for i in dm:
    for j in med:
      foldername = folder+"/Med"+str(j)+"_DM"+str(i)
      if os.path.isfile(foldername+"/Atom.signal"):
        coords_filled.append([i,j])
      else:
        coords_notfilled.append([i,j])
  plt.plot(*zip(*coords_filled), marker='.', color='black', ls='')
  plt.plot(*zip(*coords_notfilled), marker='.', color='white', ls='')
  if name is None:
    plt.savefig(str(sqrts)+".png", format="png", dpi=300 )
  else:
    plt.savefig(name+".png", format="png", dpi=300 )


def getMassPoints(folder):
  print "getMassPoints\n"
  paths = [os.path.join(folder,o) for o in os.listdir(folder) if os.path.isdir(os.path.join(folder,o))]
  runs = []

  for tmp in paths:
    tmp = tmp.replace(folder+"/", '')
    runs.append(tmp)

  med_list = []
  dm_list = []

  for runmasses in runs:
    split = runmasses.split('_')
    for masspoint in split:
      if "Med" in masspoint:
        masspoint = masspoint.replace('Med', '')
        med_list.append(int(masspoint))
      if "DM" in masspoint:
        masspoint = masspoint.replace('DM', '')
        dm_list.append(int(masspoint))

  med = list(set(med_list))
  dm = list(set(dm_list))

  return med, dm

def getSRs(filename):
  with open(filename) as f:
    for line in f.readlines():
      if "Total weight of SR1 events normalised to luminosity:" in line:
        words = line.split(' ')
        sr1 = float(words[-1])
      if "Total weight of SR2 events normalised to luminosity:" in line:
        words = line.split(' ')
        sr2 = float(words[-1])
      if "Total weight of SR3 events normalised to luminosity:" in line:
        words = line.split(' ')
        sr3 = float(words[-1])
      if "Total weight of SR4 events normalised to luminosity:" in line:
        words = line.split(' ')
        sr4 = float(words[-1])
  return sr1, sr2, sr3, sr4

def nearestPointInterpolator(med, dm, folder, sqrts):
  print "nearestPointInterpolator\n"
  sr1_lim = getData(str(sqrts)+"TeV SR1", "lim")
  sr2_lim = getData(str(sqrts)+"TeV SR2", "lim")
  sr3_lim = getData(str(sqrts)+"TeV SR3", "lim")
  sr4_lim = getData(str(sqrts)+"TeV SR4", "lim")
  
  print sr1_lim, ' ', sr2_lim, ' ', sr3_lim, ' ', sr4_lim
  
  points = list()
  values = list()

  for j in med:
    thisrow = list()
    for i in dm:
      foldername = folder+"/Med"+str(j)+"_DM"+str(i)
      if not os.path.isfile(foldername+"/Atom.signal"):
        continue
      sr1, sr2, sr3, sr4 = getSRs(foldername+"/Atom.signal")
      print "getSRs for med", j, " and dm", i, " \n"
      # To avoid division by 0
      sr1 += 0.00000001
      sr2 += 0.00000001
      sr3 += 0.00000001
      sr4 += 0.00000001
      mu = min(sr1_lim/sr1, sr2_lim/sr2, sr3_lim/sr3, sr4_lim/sr4)
      limit = sqrt(sqrt(mu))
      points.append([i, j])
      values.append(limit)

  return Invdisttree( np.array(points), np.array(values) )

def linearInterpolator(med, dm, folder, sqrts, fillInEmpty = None):
	print "linearInterpolator\n"
	sr1_lim = getData(str(sqrts)+"TeV SR1", "lim")
	sr2_lim = getData(str(sqrts)+"TeV SR2", "lim")
	sr3_lim = getData(str(sqrts)+"TeV SR3", "lim")
	sr4_lim = getData(str(sqrts)+"TeV SR4", "lim")
	limits = list()

	for j in med:
		thisrow = list()
		for i in dm:
			foldername = folder+"/Med"+str(j)+"_DM"+str(i)
			if not os.path.isfile(foldername+"/Atom.signal"):
				if fillInEmpty is not None:
					thisrow.append(float(fillInEmpty(np.array([i, j]), nnear = 4, p=1)))
				else:
					thisrow.append(15)
				continue
				
			sr1, sr2, sr3, sr4 = getSRs(foldername+"/Atom.signal")
			print "another getSRs med",j, " dm", i, " in linearInterpolator\n"
			# To avoid division by 0
			sr1 += 0.00000001
			sr2 += 0.00000001
			sr3 += 0.00000001
			sr4 += 0.00000001
			mu = min(sr1_lim/sr1, sr2_lim/sr2, sr3_lim/sr3, sr4_lim/sr4)
			limit = sqrt(sqrt(mu))
			thisrow.append(limit)
			
		limits.append(thisrow)

	return interpolate.interp2d(dm, med, limits, kind='linear')

def main():

	if sys.argv[1][-1] == '/':
		folder = sys.argv[1][:-1]
	else:
		folder = sys.argv[1]

	if "8TeV" in folder:
		sqrts = 8
	elif "7TeV" in folder:
		sqrts = 7
	elif "14TeV" in folder:
		sqrts = 14

	med, dm = getMassPoints(folder)

	invdisttree = nearestPointInterpolator(med, dm, folder, sqrts)

	interp = linearInterpolator(med, dm, folder, sqrts)

	xs = range(10,1310,10)
	ys = range(10,6010,10)
	xgrid = list()
	ygrid = list()
	for y in ys:
		xgrid.append(xs)
	tmp = 0
	for x in ys:
		tmplist = [ys[tmp]] * len(xs)
		ygrid.append(tmplist)
		tmp += 1

	zs = list()
	for y in ys:
		tmp = list()
		for x in xs:
			tmp.append(float(interp(x, y)))
			#tmp.append(float(invdisttree(np.array([x, y]), nnear = 4, p=1)))
		zs.append(tmp)

	plotGrid(xgrid, ygrid, zs, dm, med, folder, sqrts)

if __name__ == "__main__":
  main()
