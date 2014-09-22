#! /bin/bash
export TERM=xterm
export TERMINFO=/etc/terminfo
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/lib:/usr/lib64
SRCDIR=/afs/cern.ch/work/l/lmeng/TCAD/Tutorial/2D
RUNDIR=TCAD

source $HOME/.bashrc
source $HOME/local/tools/setupTCAD.sh

mkdir $RUNDIR
cd $RUNDIR

sprocess $SRCDIR/sprocess1_fps.cmd

if [ -f *.* ]; then
	cp -f * $SRCDIR/
fi

cd ..

rm -rf $RUNDIR

exit
