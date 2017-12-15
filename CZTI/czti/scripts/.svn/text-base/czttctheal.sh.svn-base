#!/bin/bash
#This script will execute czttctheal and correct orbitwise tct file according to decoded laxpctime from science file

curfile=$1
if [ -z "$curfile" ]; then
    echo "No arguments provided"
    exit 1
fi

if [ ! -z "$curfile" ]; then
	>/tmp/czttctheal_input.txt
	tct_file=$(find $curfile/ -name *.tct|grep 'orbit')>>/tmp/czttctheal_input.txt
	nfiles=$(find $curfile/ -name *.fits|grep 'orbit'|grep -e 'M0' -e 'M9' -e 'SS'|wc -l)>>/tmp/czttctheal_input.txt
	M0fits_file=$(find $curfile/ -name *.fits|grep 'orbit'|grep 'M0')>>/tmp/czttctheal_input.txt
	SSfits_file=$(find $curfile/ -name *.fits|grep 'orbit'|grep 'SS')>>/tmp/czttctheal_input.txt
	M9fits_file=$(find $curfile/ -name *.fits|grep 'orbit'|grep 'M9')>>/tmp/czttctheal_input.txt
	echo $tct_file>>/tmp/czttctheal_input.txt
	echo $nfiles>>/tmp/czttctheal_input.txt
	echo $M0fits_file>>/tmp/czttctheal_input.txt
	echo $M9fits_file>>/tmp/czttctheal_input.txt
	echo $SSfits_file>>/tmp/czttctheal_input.txt
	echo "y">>/tmp/czttctheal_input.txt
	cp "$tct_file" "$tct_file""0"
	echo "czttctheal.sh: $USER $(date "+%Y-%m-%d %H:%M:%S"):: Copied $tct_file "$tct_file"'0'"
	echo "czttctheal.sh: $USER $(date "+%Y-%m-%d %H:%M:%S"):: HEALING tct: $tct_file"
	czttctheal</tmp/czttctheal_input.txt
	p_status=$?	
		if [ $p_status -ne 0 ];then
			echo "czttctheal.sh: $USER $(date "+%Y-%m-%d %H:%M:%S"):: Error in czttctheal execution`perror $p_status` "
			exit -1
		else
			echo "czttctheal.sh: $USER $(date "+%Y-%m-%d %H:%M:%S"):: czttctheal execution completed"
		fi
	rm /tmp/czttctheal_input.txt
	echo "czttctheal.sh: $USER $(date "+%Y-%m-%d %H:%M:%S"):: SCRIPT EXECUTION COMPLETED"

fi

