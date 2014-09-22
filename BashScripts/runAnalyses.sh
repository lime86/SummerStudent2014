#! /bin/bash

export TERM=xterm
export TERMINFO=/etc/terminfo

BASE=/afs/cern.ch/user/l/lmeng
UNIGE=/atlas/data3/userdata/lmeng/MCFiles

OUTDIR=$BASE/public/ATOMAnalyses

source $BASE/.bashrc
source $BASE/local/tools/setupAtom

#cd $BASE/TestChamber/ATOM8TeV

if ! [ -d "$OUTDIR" ]; then
	mkdir $OUTDIR
fi

cd $OUTDIR

BG=(bgzvv012j bgwlv012j	bgtt bgqq bgjj bgww)

for (( i=0; i<${#BG[@]}; i++ )); do
	if ! [ -d "${BG[i]}" ]; then
		mkdir ${BG[i]}
		cd ${BG[i]}
		echo "Running analysis on ${BG[i]}"
		
		echo "addAnalysis ATLAS_razor_1" >> Atom.batch
		
		for j in $( ls $UNIGE | grep ${BG[i]} ); do
			echo "addInput $UNIGE/$j HepMC" >> Atom.batch
		done
		
		echo "addOutput Atom" >> Atom.batch
		echo "set SaveHistograms on" >> Atom.batch
		echo "set IgnoreBeams on" >> Atom.batch
		echo "launch" >> Atom.batch
		
		atom-batch -b Atom.batch | tee Atom.log
		
		cd ..
	fi
done

DM=(	Med1000_DM10 dm012j_8tev_med1000_dm10_0.hepmc \
		Med1000_DM100 dm012j_8tev_med1000_dm100_0.hepmc)



for i in 10 100
	do
	DMDIR=Med1000_DM${i}
		if ! [ -d ${DMDIR} ]; then
			echo "No directory $DMDIR"
			if [ -f "$UNIGE/dm${i}_8tev.hepmc" ]; then
				echo "Running analysis on $DMDIR"
				mkdir Med1000_DM${i}
				cd Med1000_DM${i}
				
/bin/cat <<EOM > Atom.batch
addAnalysis ATLAS_razor_1
addInput $UNIGE/dm${i}_8tev.hepmc HepMC
addOutput Atom
set SaveHistograms on
set IgnoreBeams on
launch
EOM

				atom-batch -b Atom.batch | tee Atom.log
				#atom -a ATLAS_razor_1 $UNIGE/dm012j_8tev_med1000_dm${i}_0.hepmc HepMC | tee Atom.log
				cd ..
			fi
		fi
	done
		
echo "Finished analyses."

exit

