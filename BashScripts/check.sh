#!/bin/bash
cd /afs/cern.ch/work/l/lmeng/MCFiles/
SLEEPTIME=20m

while :
	do
		echo "Checking for MCFiles..."
		if [ -f bgwlv012j_500k_8tev_med1tev_dm100gev.hepmc ]; then
			sleep $SLEEPTIME
			chmod 755 bgwlv012j_500k_8tev_med1tev_dm100gev.hepmc
			echo "Copying bgwlv012j_500k_8tev_med1tev_dm100gev.hepmc"
			mv bgwlv012j_500k_8tev_med1tev_dm100gev.hepmc /atlas/data3/userdata/lmeng/MCFiles/
		fi
		
		if [ -f bgzvv012j_500k_8tev_med1tev_dm100gev.hepmc ]; then
			sleep $SLEEPTIME
			chmod 755 bgzvv012j_500k_8tev_med1tev_dm100gev.hepmc
			echo "Copying bgzvv012j_500k_8tev_med1tev_dm100gev.hepmc"
			mv bgzvv012j_500k_8tev_med1tev_dm100gev.hepmc /atlas/data3/userdata/lmeng/MCFiles/
		fi
		
		if [ -f bgtt_500k_8tev_med1tev_dm100gev.hepmc ]; then
			sleep $SLEEPTIME
			chmod 755 bgtt_500k_8tev_med1tev_dm100gev.hepmc
			echo "Copying bgtt_500k_8tev_med1tev_dm100gev.hepmc"
			mv bgtt_500k_8tev_med1tev_dm100gev.hepmc /atlas/data3/userdata/lmeng/MCFiles/
		fi
		
		if [ -f bgqq_500k_8tev_med1tev_dm100gev.hepmc ]; then
			sleep $SLEEPTIME
			chmod 755 bgqq_500k_8tev_med1tev_dm100gev.hepmc
			echo "Copying bgqq_500k_8tev_med1tev_dm100gev.hepmc"
			mv bgqq_500k_8tev_med1tev_dm100gev.hepmc /atlas/data3/userdata/lmeng/MCFiles/
		fi
		
		if [ -f events_dm012j_500k_8tev_med1tev_dm10gev.hepmc ]; then
			sleep $SLEEPTIME
			chmod 755 events_dm012j_500k_8tev_med1tev_dm10gev.hepmc
			echo "Copying events_dm012j_500k_8tev_med1tev_dm10gev.hepmc"
			mv events_dm012j_500k_8tev_med1tev_dm10gev.hepmc /atlas/data3/userdata/lmeng/MCFiles/
		fi

		if [ -f events_dm012j_500k_8tev_med1tev_dm100gev.hepmc ]; then
			sleep $SLEEPTIME
			chmod 755 events_dm012j_500k_8tev_med1tev_dm100gev.hepmc
			echo "Copying events_dm012j_500k_8tev_med1tev_dm100gev.hepmc"
			mv events_dm012j_500k_8tev_med1tev_dm100gev.hepmc /atlas/data3/userdata/lmeng/MCFiles/
		fi

		sleep 120
	done


