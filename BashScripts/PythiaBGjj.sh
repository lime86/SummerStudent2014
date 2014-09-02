#! /bin/bash
RUNDIR=PythiaBGjj
#OUTDIR=/atlas/data3/userdata/lmeng/MCFiles

if [ -d $RUNDIR ]; then
  echo "Output directory already exists!"
  exit
fi

cp -r $HOME/PythiaShowering $RUNDIR
cp /afs/cern.ch/work/l/lmeng/MG5_aMC_v2_1_1/bgjj/Events/run_01/events.lhe.gz $RUNDIR/

cd $RUNDIR

gunzip events.lhe.gz

OUTFILE=bgjj_500k_8tev_med1tev_dm100gev_1.hepmc
export TERM=xterm
export TERMINFO=/etc/terminfo

source /afs/cern.ch/user/l/lmeng/local/tools/setupAtom

./main32.exe main32.cmnd $OUTFILE

if [ -f $OUTFILE ]; then
  #cp -f $OUTFILE /afs/cern.ch/work/l/lmeng/MCFiles/
  cp -f $OUTFILE $OUTDIR/
fi

cd ..

rm -rf $RUNDIR
