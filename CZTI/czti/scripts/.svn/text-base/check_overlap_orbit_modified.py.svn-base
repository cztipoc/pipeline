#!/usr/bin/env python
#Shrikant Chaudhari 21 Oct 2016
#This script checks data gap/overlap between the orbits and datagap/overlap between the two obsID according to the provided argument
#Note: Please Do NOT delete commented lines.

from astropy.io import fits
import numpy as np
import os,subprocess,argparse,sys
import texttable as tt

#Flags
overlap_flag=0
missing_flag=0
draw=0

if len(sys.argv)==2:
	
	#print "\nprocessing for ORBITWISE overlap/gap"
	parser=argparse.ArgumentParser()
	parser.add_argument("observation", type=str,help='enter the level1 path for perticular observation')
	args=parser.parse_args()
	obs=args.observation
	#findCMD = "find"+ " "+obs+"/ "+ "-name *.fits|grep 'orbit'|grep -v 'ignore'|grep 'modeM0'|sort" 
	findCMD= "find -L"+ " "+obs+"/ "+ "-name *.mkf|grep 'orbit'|grep -v ignore|grep -v V|sort"
	#print findCMD
	out = subprocess.Popen(findCMD,shell=True,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE)
	(stdout,stderr)=out.communicate()
	fits_file=stdout.decode().split()
	tstarti=[]
	tstopi=[]
	orbits=[]
	temp_orbits=[]
	tab = tt.Texttable()
	tab.set_cols_width([70,20,10,10,15])
	tab.set_cols_align(['l','r','r','r','l'])

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
	#print orbits
	#print tstarti
	#print tstopi
	while j<(len(tstarti)-1):
		#print (tstarti[j+1]+100)
		#print tstopi[j]
		#if ((tstarti[j+1]+100)>tstopi[j]):
		tdiff=tstarti[j+1]-tstopi[j]
		if (tdiff>100):
			draw=1
			missing_flag=missing_flag+1
			if missing_flag==1:
				row=[obs,str(tdiff)+" sec MISSING ",str(orbits[j]).zfill(5),str(orbits[j+1]).zfill(5),' ']
			else:
				row=[' ',str(tdiff)+" sec MISSING ",str(orbits[j]).zfill(5),str(orbits[j+1]).zfill(5),' ']
			tab.add_row(row)
			#print "%s\t\t%s sec\t%s\t%s: " %(obs,tdiff,orbits[j],orbits[j+1])
			
		
		j=j+1

	k=0
	
	while k<(len(tstarti)):
		#print k
		l=0
		for l in range(0,k):
			#print l
			
			tdiff_start=tstarti[k]-tstarti[l]
			tdiff_stop=tstopi[k]-tstopi[l]
			#print str(orbits[k]).zfill(5),str(orbits[l]).zfill(5),tdiff_start,tdiff_stop
			#if (tstarti[k]<tstarti[l]):
			if (tdiff_start<-100):
				#print "ORBITS overlaping found...[Start]:",orbits[k],orbits[l],tstarti[k],tstarti[l],tdiff_start
				draw=1
				overlap_flag=overlap_flag+1
				#print "@@@@@@@@"+str(tdiff_start),missing_flag,overlap_flag
				
				if (missing_flag==0 and overlap_flag==1):
					#if(tdiff_start<-100):
					row=[obs,str(tdiff_start)+" sec OVERLAP @ start ",str(orbits[k]).zfill(5),str(orbits[l]).zfill(5),"move "+str(orbits[l]).zfill(5)+" to ignore"]
					#else:
					#row=[' ',str(tdiff_stop)+" sec OVERLAP ",str(orbits[k]).zfill(5),str(orbits[l]).zfill(5)]
					tab.add_row(row)


				if (missing_flag!=0 and overlap_flag>=1):
					#if(tdiff_start<-100):
					#print "#########"+str(tdiff_start),missing_flag,overlap_flag
					row=[' ',str(tdiff_start)+" sec OVERLAP @ start ",str(orbits[k]).zfill(5),str(orbits[l]).zfill(5),"move "+str(orbits[l]).zfill(5)+" to ignore"]
					#else:
					#row=[' ',str(tdiff_stop)+" sec OVERLAP ",str(orbits[k]).zfill(5),str(orbits[l]).zfill(5)]
					tab.add_row(row)




			if (tdiff_stop<-100):
				draw=1
				overlap_flag=overlap_flag+1
				if (missing_flag==0 and overlap_flag==1):
					#if(tdiff_start<-100):
					row=[obs,str(tdiff_stop)+" sec OVERLAP @ end ",str(orbits[k]).zfill(5),str(orbits[l]).zfill(5), "move "+str(orbits[l]).zfill(5)+" to ignore"]
					#else:
					#row=[' ',str(tdiff_stop)+" sec OVERLAP ",str(orbits[k]).zfill(5),str(orbits[l]).zfill(5)]
					tab.add_row(row)

					
				if (missing_flag!=0 and overlap_flag>1):
					#if(tdiff_start<-100):
					row=[' ',str(tdiff_stop)+" sec OVERLAP @ end ",str(orbits[k]).zfill(5),str(orbits[l]).zfill(5),"move "+str(orbits[l]).zfill(5)+" to ignore"]
					#else:
					#row=[' ',str(tdiff_stop)+" sec OVERLAP ",str(orbits[k]).zfill(5),str(orbits[l]).zfill(5)]
					tab.add_row(row)




		k=k+1

		

	#if flag==0:
	#	print obs
	
	
	
	

	if(missing_flag==0 and overlap_flag==0):
		print  "\n"+obs
	tab.set_deco(tab.HEADER)
	if draw==1:
		print "\n"
		s = tab.draw()
		print s
		

#for Data gaps between the Observations
if len(sys.argv)==3:
	tab = tt.Texttable()
	tab.set_cols_width([80,80,20])
	tab.set_cols_align(['l','l','r'])
	draw=0
	#print "\nProcessing for observarion time gap"
	ob1=sys.argv[1]
	ob2=sys.argv[2]
	#finding last orbit fits file for observation 1
	#findCMD = "find"+ " "+ob1+"/czti/orbit/ "+ "-name *.fits|grep -v 'ignore'|grep 'modeM0'|sort|tail -1"
	findCMD= "find -L "+ " "+ob1+"/ "+ "-name *.mkf|grep 'orbit'|grep -v ignore|grep -v V|sort"
	out = subprocess.Popen(findCMD,shell=True,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE)
	(stdout,stderr)=out.communicate()
	fits_file_ob1=stdout.decode().split()
	#finding first orbit fits file for observation 2
	#findCMD = "find"+ " "+ob2+"/czti/orbit/ "+ "-name *.fits|grep -v 'ignore'|grep 'modeM0'|sort|head -1"
	findCMD= "find -L "+ " "+ob2+"/ "+ "-name *.mkf|grep 'orbit'|grep -v ignore|grep -v V|sort"
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
		draw=1
		temp_ob1=fits_file_ob1[len(fits_file_ob1)-1].split("/")
		stop_orb1=temp_ob1[len(temp_ob1)-2]
		temp_ob1=fits_file_ob1[0].split("/")
		start_orb1=temp_ob1[len(temp_ob1)-2]
		#print ob1+"\t"+"start orbit:",start_orb1," stop orbit:",stop_orb1

		temp_ob2=fits_file_ob2[len(fits_file_ob2)-1].split("/")
		stop_orb2=temp_ob2[len(temp_ob2)-2]
		temp_ob2=fits_file_ob2[0].split("/")
		start_orb2=temp_ob2[len(temp_ob2)-2]
		#print ob2+"\t"+"start orbit:",start_orb2," stop orbit:",stop_orb2
		#print "stop time of 1st observation:  ",tstop_ob1,""
		#print "Start time of 2nd observation: ",tstart_ob2," 
		#print "TIMEMISSING between observation: ",tstart_ob2-tstop_ob1,"\n"
		row=[ob1,ob2,str(tstart_ob2-tstop_ob1)+" sec GAP "]
		tab.add_row(row)
	
	
	tab.set_deco(tab.HEADER)
	if draw==1:
		s = tab.draw()
		print s
		print "\n"

if len(sys.argv)==1:
	print "Please provide correct arguments. For more healp type 'script -h' option"
	sys.exit(-1)


