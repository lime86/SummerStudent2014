#! /bin/bash

export TERM=xterm
export TERMINFO=/etc/terminfo

BASE=/afs/cern.ch/user/l/lmeng
WORK=/afs/cern.ch/work/l/lmeng
UNIGE=/atlas/data3/userdata/lmeng/MCFiles
OUTFILE=cuts.txt

export ATLAS_LOCAL_ROOT_BASE=/cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase
source /afs/cern.ch/user/l/lmeng/.bashrc
#source ${ATLAS_LOCAL_ROOT_BASE}/user/atlasLocalSetup.sh
#source ${ATLAS_LOCAL_ROOT_BASE}/packageSetups/atlasLocalROOTSetup.sh --rootVersion ${rootVersionVal} 5.99.06-x86_64-slc6-gcc48-opt
source $BASE/local/tools/setupAtom

#cd $BASE/TestChamber/Statistics
#cd $BASE/public/ATOMAnalyses
cd $HOME/work/MedBy3

for i in $( ls -d */);
	do
		if [ -f ${i}ATLAS_razor_1.root ]; then
			echo "$i"
			root -l -b "$BASE/TestChamber/Statistics/readtree.cc+(\"${i}ATLAS_razor_1.root\")"
		fi
	done

mv $OUTFILE $BASE/TestChamber/Statistics/
echo "Finished."

exit

