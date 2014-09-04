#! /bin/bash

M1=bla
M2=blubb
BG=(DIR FILE)
BASE=/afs/cern.ch/user/l/lmeng
for i in $( ls $BASE/public/ATOMAnalyses); do
            echo "$i"
           #root -l "readtree.cc+(\"$i/ATLAS_razor_1.root\")"

            
        done
	
