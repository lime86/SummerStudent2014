#! /bin/bash
export TERM=xterm
export TERMINFO=/etc/terminfo

BASE=/afs/cern.ch/user/l/lmeng
OUTPUT=/afs/cern.ch/work/l/lmeng/MCFiles
DIR=Med1000_DM001

NEVENTS=500000
QCUT=80
EBEAM=4000
MED=1000
DM=0.01
WIDTH=`echo "$MED/3" | bc`

export ATLAS_LOCAL_ROOT_BASE=/cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase
source /afs/cern.ch/user/l/lmeng/.bashrc
source ${ATLAS_LOCAL_ROOT_BASE}/user/atlasLocalSetup.sh
source ${ATLAS_LOCAL_ROOT_BASE}/packageSetups/atlasLocalROOTSetup.sh --rootVersion ${rootVersionVal} 5.99.06-x86_64-slc6-gcc48-opt

cp -rf $BASE/MG5_aMC_v2_1_2 MG5

sed -i 's/100 = nevents/'$NEVENTS' = nevents/g' MG5/$DIR/Cards/run_card.dat
sed -i 's/6500 = ebeam1/'$EBEAM' = ebeam1/g' MG5/$DIR/Cards/run_card.dat
sed -i 's/6500 = ebeam2/'$EBEAM' = ebeam2/g' MG5/$DIR/Cards/run_card.dat
sed -i 's/0 = ickkw/1 = ickkw/g' MG5/$DIR/Cards/run_card.dat
sed -i 's/0 = xqcut/'$QCUT' = xqcut/g' MG5/$DIR/Cards/run_card.dat

sed -i 's/8 1.000000e+04/8 '$MED' /g' MG5/$DIR/Cards/param_card.dat
sed -i 's/9 1.000000e+02/9 '$DM'/g' MG5/$DIR/Cards/param_card.dat
sed -i 's/9000010 100.000000/9000010 '$DM'/g' MG5/$DIR/Cards/param_card.dat
sed -i 's/101 10000.000000/101 '$MED'/g' MG5/$DIR/Cards/param_card.dat
sed -i 's/DECAY 101 1.000000e+00 # Wxi/DECAY 101 '$WIDTH' #Wxi/g' MG5/$DIR/Cards/param_card.dat

./MG5/$DIR/bin/generate_events -f

cp -rf MG5/$DIR $BASE/MG5_aMC_v2_1_2/

exit
