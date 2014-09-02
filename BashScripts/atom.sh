#! /bin/bash


#tmpDir=`mktemp -d`
#cd ${tmpDir}

export TERM=xterm
export TERMINFO=/etc/terminfo

export BASE=/afs/cern.ch/user/l/lmeng
export MCDIR=/atlas/data3/userdata/lmeng/MCFiles

source $BASE/.bashrc
source $BASE/local/tools/setupAtom

echo "Running analysis"

addAnalysis ATLAS_razor_1
addInput $MCDIR/dm012j_500k_8tev_med1tev_dm100gev.hepmc HepMC
addOutput Atom
launch 

#atom -a ATLAS_razor_0 $BASE/work/MCFiles/events_500k.hepmc | tee Atom.log
atom -a ATLAS_razor_1 $BASE/work/MCFiles/bgzvv012j_100k_8tev_med644gev_dm100gev.hepmc | tee Atom.log

echo "Finished analysis"

if [ -f Atom.yml ]; then
	mkdir $BASE/work/test
	cp -f ATLAS_razor_1* $BASE/work/test/
	cp -f Atom.* $BASE/work/test/
fi


exit

