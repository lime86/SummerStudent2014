#! /usr/bin/env python

from math import *
from scipy.stats import poisson
import sys
try:
    import numpy as np
except Exception:
    print ''' WARNING:
    Could not load numpy. CLs value cannot be calculated!
    '''


def cls(Nsig, errNsig, Nbkg, errNbkg, Nobs):
    distListSig = []
    distList = []
    nb = 0
    nsb = 0
    ntot = 100000
    sigList = np.random.poisson(Nsig,ntot)
    #sigList = np.random.normal(Nsig,errNsig,ntot)
    errList = []
    #lumiError = np.random.normal(1, 0.036, ntot)
    lumiError = np.ones(ntot)
    for err in errNbkg:
        errList.append(np.random.normal(0,0.000001+err,ntot))

    for i in range(0,ntot):
        totErr = 0
        for j in range(0, len(errNbkg)):
            totErr += errList[j][i]
        distListSig.append(np.random.poisson(max(0.000001,sigList[i]+(Nbkg+totErr)*lumiError[i])))
        distList.append(np.random.poisson(max(0.000001,Nbkg+totErr))*lumiError[i])
        if distList[i]<Nobs:
            nb=nb+1
        if distListSig[i]<Nobs:
            nsb=nsb+1
    #print "nsb, nb", nsb, nb, Nsig, errNsig, Nbkg, errNbkg, Nobs
    if nsb>nb:
        cls=1
    else:
        cls=float(nsb)/float(nb)
    return cls

def altCLs(Nbkg, Nobs, CL):
    Nsig=0
    while (True):
        clb = poisson.cdf(Nobs, Nbkg)
        clbs = poisson.cdf(Nobs, Nbkg+Nsig)
        cls = clbs/clb
        if (cls < (1 - CL)):
            break
        Nsig += 1
    return Nsig

def calcCLs(Nbkg, errNbkg, Nobs, confLvl):
    alpha = 1
    sig = Nbkg / 2
    step = Nbkg/10 + 1
    while ( abs(confLvl - 1 + alpha) > 0.001 ):
        sig, alpha = calcStep(Nbkg, errNbkg, Nobs, confLvl, sig, step)
        step /= -2.
    return sig

def calcStep(Nbkg, errNbkg, Nobs, confLvl, start, stepsize):
    sig = start
    sign = 1
    sign = copysign(sign, stepsize)
    if(sign < 0):
        alpha = 0
    else:
        alpha = 1
    while(sign*alpha > sign*(1 - confLvl)):
        if sig < 0:
            sig = 0
        alpha = cls(sig, 0, Nbkg, errNbkg, Nobs)
        sig += stepsize
    return sig, alpha
