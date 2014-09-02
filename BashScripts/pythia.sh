#! /bin/bash

if [ -d PythiaShowering ]; then
  echo "Output directory already exists!"
  exit
fi

#export OUTFILE=$1
export OUTFILE=events_dm2j_100k_8tev_med644gev_dm100gev_stdpdf.hepmc
export TERM=xterm
export TERMINFO=/etc/terminfo

source /afs/cern.ch/user/l/lmeng/local/tools/setupAtom

cp -r $HOME/PythiaShowering .
cp -r $HOME/MG5_aMC_v2_1_1/dm_2j/Events/run_01/events.lhe.gz PythiaShowering/

cd PythiaShowering

gunzip PythiaShowering/events.lhe.gz

./main32.exe main32.cmnd $OUTFILE

if [ -f $OUTFILE ]; then
  cp -f $OUTFILE /afs/cern.ch/work/l/lmeng/MCFiles/
fi

exit
