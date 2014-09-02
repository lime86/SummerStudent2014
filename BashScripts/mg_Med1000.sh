#! /bin/bash
export TERM=xterm
export TERMINFO=/etc/terminfo

export ATLAS_LOCAL_ROOT_BASE=/cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase
source /afs/cern.ch/user/l/lmeng/.bashrc
source ${ATLAS_LOCAL_ROOT_BASE}/user/atlasLocalSetup.sh
source ${ATLAS_LOCAL_ROOT_BASE}/packageSetups/atlasLocalROOTSetup.sh --rootVersion ${rootVersionVal} 5.99.06-x86_64-slc6-gcc48-opt

export WORK=/afs/cern.ch/work/l/lmeng
export SOURCEDIR=Med1000_DM001

NEVENTS=500000
QCUT=80
EBEAM=4000
MED=1000
WIDTH=`echo "$MED/3" | bc`

for DM in 1 10 50 100 200 300 400 500 600 700 800 900 1000 1100 1200 1300
  do
    
		export MCDIR=Med${MED}_DM${DM}

		cp -rf $WORK/MG5_aMC_v2_1_1 MG5

		sed -i 's/100 = nevents/'$NEVENTS' = nevents/g' MG5/$SOURCEDIR/Cards/run_card.dat
		sed -i 's/6500 = ebeam1/'$EBEAM' = ebeam1/g' MG5/$SOURCEDIR/Cards/run_card.dat
		sed -i 's/6500 = ebeam2/'$EBEAM' = ebeam2/g' MG5/$SOURCEDIR/Cards/run_card.dat
		sed -i 's/0 = ickkw/1 = ickkw/g' MG5/$SOURCEDIR/Cards/run_card.dat
		sed -i 's/0 = xqcut/'$QCUT' = xqcut/g' MG5/$SOURCEDIR/Cards/run_card.dat

		sed -i 's/8 1.000000e+04/8 '$MED' /g' MG5/$SOURCEDIR/Cards/param_card.dat
		sed -i 's/9 1.000000e+02/9 '$DM'/g' MG5/$SOURCEDIR/Cards/param_card.dat
		sed -i 's/9000010 100.000000/9000010 '$DM'/g' MG5/$SOURCEDIR/Cards/param_card.dat
		sed -i 's/101 10000.000000/101 '$MED'/g' MG5/$SOURCEDIR/Cards/param_card.dat
		sed -i 's/DECAY 101 1.000000e+00 # Wxi/DECAY 101 '$WIDTH' #Wxi/g' MG5/$SOURCEDIR/Cards/param_card.dat

		./MG5/$SOURCEDIR/bin/generate_events -f

		cp -rf MG5/$SOURCEDIR $WORK/MedBy3/$MGDIR

		rm -rf MG5
		
		###############Pythia 
		
		RUNDIR=temp

		if [ -d $RUNDIR ]; then
		  echo "Output directory already exists!"
		  exit
		fi

		cp -r $HOME/PythiaShowering $RUNDIR
		cp $WORK/MedBy3/$MGDIR/Events/run_01/events.lhe.gz $RUNDIR/

		cd $RUNDIR

		gunzip events.lhe.gz

		/bin/cat <<EOM > Atom.batch
		addAnalysis ATLAS_razor_1
		addInput file.hepmc HepMC
		addOutput Atom
		set SaveHistograms on
		launch
		EOM
		
		./main32.exe main32.cmnd file.hepmc
		atom-batch -b Atom.batch
		
		
	done

exit
