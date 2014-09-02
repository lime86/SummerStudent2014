#! /bin/bash

export TERM=xterm
export TERMINFO=/etc/terminfo

BASE=/afs/cern.ch/user/l/lmeng
UNIGE=/atlas/data3/userdata/lmeng/MCFiles

source $BASE/.bashrc
source $BASE/local/tools/setupAtom

cd $BASE/public/ATOMAnalyses

###### Background ######

D1=bgzvv012j
F1=bgzvv012j_8tev_0.hepmc

D2=bgwlv012j
F2=bgwlv012j_8tev_0.hepmc

D3=bgtt
F3=bgtt_8tev_0.hepmc

D4=bgqq
F4=bgqq_8tev_0.hepmc

D5=bgjj
F5=bgjj_8tev_0.hepmc

D6=bgww
F6=bgww_8tev_0.hepmc


###### Signal ######

DM10=dm012j_10
FDM10=dm012j_8tev_med1000_dm10_0.hepmc

DM100=dm012j_100
FDM100=dm012j_8tev_med1000_dm100_0.hepmc

##################
if ! [ -d "$D1" ]; then
	if [ -f "$UNIGE/$F1" ]; then
		echo "Running analysis on bgzvv012j"
		mkdir $D1
		cd $D1
		atom -a ATLAS_razor_1 $UNIGE/$F1 | tee Atom.log
		cd ..
	fi
fi
##################
if ! [ -d "$D2" ]; then
	if [ -f "$UNIGE/$F2" ]; then
		echo "Running analysis on bgwlv012j"
		mkdir $D2
		cd $D2
		atom -a ATLAS_razor_1 $UNIGE/$F2 | tee Atom.log
		cd ..
	fi
fi
##################
if ! [ -d "$D3" ]; then
	if [ -f "$UNIGE/$F3" ]; then
		echo "Running analysis on bgtt"
		mkdir $D3
		cd $D3
		atom -a ATLAS_razor_1 $UNIGE/$F3 | tee Atom.log
		cd ..
	fi
fi
##################
if ! [ -d "$D4" ]; then
	if [ -f "$UNIGE/$F4" ]; then
		echo "Running analysis on bgqq"
		mkdir $D4
		cd $D4
		atom -a ATLAS_razor_1 $UNIGE/$F4 | tee Atom.log
		cd ..
	fi
fi
##################
if ! [ -d "$D5" ]; then
	if [ -f "$UNIGE/$F5" ]; then
		echo "Running analysis on bgjj"
		mkdir $D5
		cd $D5
		atom -a ATLAS_razor_1 $UNIGE/$F5 | tee Atom.log
		cd ..
	fi
fi
##################
if ! [ -d "$D6" ]; then
	if [ -f "$UNIGE/$F6" ]; then
		echo "Running analysis on bgww"
		mkdir $D6
		cd $D6
		atom -a ATLAS_razor_1 $UNIGE/$F6 | tee Atom.log
		cd ..
	fi
fi
##################
if ! [ -d "$DM10" ]; then
	if [ -f "$UNIGE/$FDM10" ]; then
		echo "Running analysis on dm012j_10"
		mkdir $DM10
		cd $DM10
		atom -a ATLAS_razor_1 $UNIGE/$FDM10 | tee Atom.log
		cd ..
	fi
fi
##################
if ! [ -d "$DM100" ]; then
	if [ -f "$UNIGE/$FDM100" ]; then
		echo "Running analysis on dm012j_100"
		mkdir $DM100
		cd $DM100
		atom -a ATLAS_razor_1 $UNIGE/$FDM100 | tee Atom.log
		cd ..
	fi
fi
##################

echo "Finished analyses."

exit

