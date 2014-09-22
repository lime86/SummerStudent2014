#! /bin/bash
export TERM=xterm
export TERMINFO=/etc/terminfo

export ATLAS_LOCAL_ROOT_BASE=/cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase
source /afs/cern.ch/user/l/lmeng/.bashrc
#source ${ATLAS_LOCAL_ROOT_BASE}/user/atlasLocalSetup.sh
#source ${ATLAS_LOCAL_ROOT_BASE}/packageSetups/atlasLocalROOTSetup.sh --rootVersion ${rootVersionVal} 5.99.06-x86_64-slc6-gcc48-opt
source /afs/cern.ch/user/l/lmeng//local/tools/setupAtom

export WORK=/afs/cern.ch/work/l/lmeng
export SOURCEDIR=bgjj
OUTDIR=/afs/cern.ch/work/l/lmeng/

NEVENTS=5000000
QCUT=80
EBEAM=4000
MED=1000
DM=100
WIDTH=`echo "$MED/3" | bc`
	
MGDIR=$SOURCEDIR

echo "Copying MG5"

cp -rf $WORK/MG5_aMC_v2_1_1 MG5

echo "Editing cards"

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

echo "Generating events"

./MG5/$SOURCEDIR/bin/generate_events -f

#cp -rf MG5/$SOURCEDIR $WORK/MedBy3/$MGDIR

###############Pythia 

RUNDIR=temp

if [ -d $RUNDIR ]; then
  echo "Output directory already exists!"
  exit
fi

cp -r $HOME/PythiaShowering $RUNDIR
cp MG5/$SOURCEDIR/Events/run_01/events.lhe.gz $RUNDIR/

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

if ! [ -d */ $WORK/MedBy3/$MGDIR/ ]; then
	mkdir $WORK/MedBy3/$MGDIR/
fi

cp ATLAS* $WORK/MedBy3/$MGDIR/
cp Atom* $WORK/MedBy3/$MGDIR/

cd ..

rm -rf MG5
rm -rf $RUNDIR

exit
