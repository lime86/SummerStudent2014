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

BG=(	bgzvv012j bgzvv012j_8tev_0.hepmc \
		bgwlv012j bgwlv012j_8tev_0.hepmc \
		bgtt bgtt_8tev_0.hepmc \
		bgqq bgqq_8tev_0.hepmc \
		bgjj bgjj_8tev_0.hepmc \
		bgww bgww_8tev_0.hepmc)

DM=(	Med1000_DM10 dm012j_8tev_med1000_dm10_0.hepmc \
		Med1000_DM100 dm012j_8tev_med1000_dm100_0.hepmc)

for (( i=0; i<${#BG[@]}; i++ ));
	do
		if ! [ -d "${BG[i]}" ]; then
			if [ -f "$UNIGE/${BG[i+1]}" ]; then
				echo "Running analysis on ${BG[i]}"
				mkdir ${BG[i]}
				cd ${BG[i]}
				
/bin/cat <<EOM > Atom.batch
addAnalysis ATLAS_razor_1
addInput $UNIGE/${BG[1]} HepMC
addOutput Atom
set SaveHistograms on
set IgnoreBeams on
launch
EOM
				atom-batch -b Atom.batch | tee Atom.log
				
				#atom -a ATLAS_razor_1 $UNIGE/${BG[1]} | tee Atom.log
				cd ..
			fi
		fi
	done

	
for i in 10 100
	do
		if ! [ -d "dm012j_${i}" ]; then
			echo "No directory dm012j_${i}"
			if [ -f "$UNIGE/dm${i}_8tev.hepmc" ]; then
				echo "Running analysis on dm012j_${i}"
				mkdir dm012j_${i}
				cd dm012j_${i}
				
/bin/cat <<EOM > Atom.batch
addAnalysis ATLAS_razor_1
addInput $UNIGE/dm${i}_8tev.hepmc HepMC
addOutput Atom
set SaveHistograms on
set IgnoreBeams on
launch
EOM

				atom-batch -b Atom.batch | tee Atom.log
				#atom -a ATLAS_razor_1 $UNIGE/dm012j_8tev_med1000_dm${i}_0.hepmc | tee Atom.log
				cd ..
			fi
		fi
	done
		
echo "Finished analyses."

exit

