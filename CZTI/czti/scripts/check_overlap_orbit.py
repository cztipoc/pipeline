#!/usr/bin/env python
#This script checks the missing time and overlapping orbits also it checks for the missing time between two observations

from astropy.io import fits
import numpy as np
import os,subprocess,argparse
import sys
if len(sys.argv)==2:
	#print "\nprocessing for ORBITWISE overlap/gap"
	parser=argparse.ArgumentParser()
	parser.add_argument("observation", type=str,help='enter the level1 path for perticular observation')
	args=parser.parse_args()
	obs=args.observation
	#findCMD = "find"+ " "+obs+"/ "+ "-name *.fits|grep 'orbit'|grep -v 'ignore'|grep 'modeM0'|sort" 
	findCMD= "find"+ " "+obs+"/ "+ "-name *.mkf|grep 'orbit'|grep -v ignore|sort"
	#print findCMD
	out = subprocess.Popen(findCMD,shell=True,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE)
	(stdout,stderr)=out.communicate()
	fits_file=stdout.decode().split()
	tstarti=[]
	tstopi=[]
	orbits=[]
	temp_orbits=[]
	#print "\n"+obs+"\n"
	for i in range (len(fits_file)):
		#print fits_file[i]
		temp_orbits.append(fits_file[i].split('/'))
		orbits.append(temp_orbits[i][7])
		hdu=fits.open(fits_file[i])
		#print hdu[0].header
		tstarti.append(hdu[0].header["TSTARTI"])
		tstopi.append(hdu[0].header["TSTOPI"])
		hdu.close()
	#print"\n"


	#print orbits
	#print"\n"

	#print tstarti
	#print"\n"
	#print tstopi
	#print "\n"
	j=0

	while j<(len(tstarti)-1):
		#print (tstarti[j+1]+100)
		#print tstopi[j]
		#if ((tstarti[j+1]+100)>tstopi[j]):
		tdiff=tstarti[j+1]-tstopi[j]
		if (tdiff>100):
			print obs
			print "missing Times found before the orbit: "+orbits[j+1],tstopi[j],tstarti[j+1],tdiff


		j=j+1

	k=1
	while k<(len(tstarti)):
		#print k
		l=0
		for l in range(0,k):
			#print l
			tdiff_start=tstarti[k]-tstarti[l]
			#if (tstarti[k]<tstarti[l]):
			if (tdiff_start<0):
				#print "ORBITS overlaping found...[Start]:",orbits[k],orbits[l],tstarti[k],tstarti[l],tdiff_start
				startoverlap=l
	
		
			tdiff_stop=tstopi[k]-tstopi[l]
			#if (tstopi[k]<tstopi[l]):
			if (tdiff_stop<0):
				stopoverlap=l
				#print "ORBITS overlaping found...[Stop]:",orbits[k],orbits[l],tstopi[k],tstopi[l],tdiff_stop
	#	print orbits[k] +" is overlapping orbits "+orbits[startoverlap]+ " and "+ orbits[stopoverlap]
	#		l=l+1
	#print "\n"
		k=k+1



if len(sys.argv)==3:
	print "\nProcessing for observarion time gap"
	ob1=sys.argv[1]
	ob2=sys.argv[2]
#finding last orbit fits file for observation 1
	#findCMD = "find"+ " "+ob1+"/czti/orbit/ "+ "-name *.fits|grep -v 'ignore'|grep 'modeM0'|sort|tail -1"
	findCMD= "find"+ " "+ob1+"/ "+ "-name *.mkf|grep 'orbit'|grep -v ignore|sort"
	out = subprocess.Popen(findCMD,shell=True,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE)
	(stdout,stderr)=out.communicate()
	fits_file_ob1=stdout.decode().split()
#finding first orbit fits file for observation 2
	#findCMD = "find"+ " "+ob2+"/czti/orbit/ "+ "-name *.fits|grep -v 'ignore'|grep 'modeM0'|sort|head -1"
	findCMD= "find"+ " "+ob2+"/ "+ "-name *.mkf|grep 'orbit'|grep -v ignore|sort"
	out = subprocess.Popen(findCMD,shell=True,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE)
	(stdout,stderr)=out.communicate()
	fits_file_ob2=stdout.decode().split()
	#print fits_file_ob1[0]
	#print fits_file_ob2[0]
	if not fits_file_ob1:
		print "MKF file NOT found: ",ob1


	if not fits_file_ob2:
		print "MKF file NOT found: ",ob2,"\n"
		sys.exit(0)

		
	hdu1=fits.open(fits_file_ob1[len(fits_file_ob1)-1])
	hdu2=fits.open(fits_file_ob2[0])
	tstop_ob1=hdu1[0].header["TSTOPI"]
	tstart_ob2=hdu2[0].header["TSTARTI"]
	if (tstart_ob2-tstop_ob1)>100:
		#getting start and end orbit
		temp_ob1=fits_file_ob1[len(fits_file_ob1)-1].split("/")
		stop_orb1=temp_ob1[len(temp_ob1)-2]
		temp_ob1=fits_file_ob1[0].split("/")
		start_orb1=temp_ob1[len(temp_ob1)-2]
		print ob1+"\t"+"start orbit:",start_orb1," stop orbit:",stop_orb1

		temp_ob2=fits_file_ob2[len(fits_file_ob2)-1].split("/")
		stop_orb2=temp_ob2[len(temp_ob2)-2]
		temp_ob2=fits_file_ob2[0].split("/")
		start_orb2=temp_ob2[len(temp_ob2)-2]
		print ob2+"\t"+"start orbit:",start_orb2," stop orbit:",stop_orb2
		#print "stop time of 1st observation:  ",tstop_ob1,""
		#print "Start time of 2nd observation: ",tstart_ob2," 
		print "TIMEMISSING between observation: ",tstart_ob2-tstop_ob1,"\n"


		
#print "\n"
