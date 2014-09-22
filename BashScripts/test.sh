#! /bin/bash

M1=bla
M2=blubb
BG=(DIR FILE)
BASE=/afs/cern.ch/user/l/lmeng
MCF=/atlas/data3/userdata/lmeng/MCFiles

BASE=/afs/cern.ch/user/l/lmeng
UNIGE=/atlas/data3/userdata/lmeng/MCFiles

OUTDIR=$BASE/public/test

cd $OUTDIR

BG=(bgzvv012j bgwlv012j	bgtt bgqq bgjj bgww)

for (( i=0; i<${#BG[@]}; i++ )); do
	if ! [ -d "${BG[i]}" ]; then
		echo "Running analysis on ${BG[i]}"
		mkdir ${BG[i]}
		cd ${BG[i]}
		echo "addAnalysis ATLAS_razor_1" >> Atom.batch
		
		for j in $( ls $MCF | grep ${BG[i]} ); do
			echo "addInput $UNIGE/$j HepMC"
		done
		
		echo "addOutput Atom"
		echo "set SaveHistograms on"
		echo "set IgnoreBeams on"
		echo "launch"

		cd ..
	fi
done

echo "Finished analyses."

exit

