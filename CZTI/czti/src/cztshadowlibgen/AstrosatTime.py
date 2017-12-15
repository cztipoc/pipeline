#!/usr/bin/env python2.7
## Vedant Kumar, 2016 july 16
## Updated on 2016 july 22
"""
    AstrosatTime.py
    Aim: This is a time conversion routine for Astrosat CZTI Instrument, It can be used to convert Astrosat Seconds (since epoch, 2010.0 UTC) to ISO 8601 date,
    Calender Date, Year and DayNumber, Julian Day and Modified Julian Day formats and Vice Versa.
    Type Astrosat_Time.py -h to get help

"""

#------------------------------------------------------------------------

from astropy.io import fits
import numpy as np
import argparse,sys
from astropy.time import Time, TimeDelta
from datetime import datetime
#-----------------------------------------------------------------------

##### Defining default values and constants ####


Epoch_Date_UTC=Time('2010-01-01 00:00:00', scale='utc')
Epoch_Date_TT=Time('2010-01-01 00:01:06.184', scale='tt')
First_LeapSec=Time('2012-06-30 23:59:60', scale='utc')
Second_LeapSec=Time('2015-06-30 23:59:60', scale='utc')


"""
######### default values to check locally  #####
ISO_Date=Time('2016-06-15 22:22:22', scale='utc')
YDAY=Time('2016:167:22:22:22.000', scale='tt')
JD=Time(2457566.66746, format='jd', scale='utc')
MJD=Time(55362.00000000, format='mjd', scale='utc')
Astrosat_second=14256000.000
"""



def convertISODatetosec(ISO_Date,outflag):
    if ISO_Date.scale =='utc' and outflag=='utc':
            if ISO_Date>Epoch_Date_UTC:
                 if ISO_Date<First_LeapSec:
                        Astrosat_second=ISO_Date-Epoch_Date_UTC
                        Astrosat_second=Astrosat_second.sec
                 elif ISO_Date<Second_LeapSec:
                        Astrosat_second=ISO_Date-Epoch_Date_UTC
                        Astrosat_second=Astrosat_second.sec-1
                 else: 
                        Astrosat_second=ISO_Date-Epoch_Date_UTC
                        Astrosat_second=Astrosat_second.sec-2
            else:
                 Astrosat_second=0.0
            Astrosat_iso=ISO_Date.iso
            Astrosat_yday=ISO_Date.yday
            Astrosat_jd=ISO_Date.jd
            Astrosat_mjd=ISO_Date.mjd
    if ISO_Date.scale =='utc' and outflag=='tt':
            if ISO_Date>Epoch_Date_UTC:
                 if ISO_Date<First_LeapSec:
                        Astrosat_second=ISO_Date-Epoch_Date_UTC
                        Astrosat_second=Astrosat_second.sec
                 elif ISO_Date<Second_LeapSec:
                        Astrosat_second=ISO_Date-Epoch_Date_UTC
                        Astrosat_second=Astrosat_second.sec-1
                 else: 
                        Astrosat_second=ISO_Date-Epoch_Date_UTC
                        Astrosat_second=Astrosat_second.sec-2
            else:
                 Astrosat_second=0.0
            Astrosat_iso=ISO_Date.tt.iso
            Astrosat_yday=ISO_Date.tt.yday
            Astrosat_jd=ISO_Date.tt.jd
            Astrosat_mjd=ISO_Date.tt.mjd
    if ISO_Date.scale =='tt' and outflag=='tt':
           if ISO_Date>Epoch_Date_TT:
                 if ISO_Date<First_LeapSec:
                        Astrosat_second=ISO_Date-Epoch_Date_TT
                        Astrosat_second=Astrosat_second.sec
                 elif ISO_Date<Second_LeapSec:
                        Astrosat_second=ISO_Date-Epoch_Date_TT
                        Astrosat_second=Astrosat_second.sec-1
                 else: 
                        Astrosat_second=ISO_Date-Epoch_Date_TT
                        Astrosat_second=Astrosat_second.sec-2
           else:
                Astrosat_second=0.0
           Astrosat_iso=ISO_Date.tt.iso
           Astrosat_yday=ISO_Date.tt.yday
           Astrosat_jd=ISO_Date.tt.jd
           Astrosat_mjd=ISO_Date.tt.mjd
    if ISO_Date.scale =='tt' and outflag=='utc':
           if ISO_Date>Epoch_Date_TT:
                if ISO_Date<First_LeapSec:
                        Astrosat_second=ISO_Date-Epoch_Date_TT
                        Astrosat_second=Astrosat_second.sec
                elif ISO_Date<Second_LeapSec:
                        Astrosat_second=ISO_Date-Epoch_Date_TT
                        Astrosat_second=Astrosat_second.sec-1
                else: 
                        Astrosat_second=ISO_Date-Epoch_Date_TT
                        Astrosat_second=Astrosat_second.sec-2
           else:
                Astrosat_second=0.0
           Astrosat_iso=ISO_Date.utc.iso
           Astrosat_yday=ISO_Date.utc.yday
           Astrosat_jd=ISO_Date.utc.jd
           Astrosat_mjd=ISO_Date.utc.mjd
    return Astrosat_iso,Astrosat_yday,Astrosat_jd,Astrosat_mjd,Astrosat_second
 

#print convertISODatetosec(ISO_Date,'utc')
#print convertISODatetosec(ISO_Date,'tt')
  



def convertYDAYtosec(YDAY,outflag):
    if YDAY.scale =='utc' and outflag=='utc':
        if YDAY>Epoch_Date_UTC:
                 if YDAY<First_LeapSec:
                        Epoch_Date_yday=Time(Epoch_Date_UTC,format='yday')
                        Astrosat_second=(YDAY-Epoch_Date_yday).sec
                 elif YDAY<Second_LeapSec:
                        Epoch_Date_yday=Time(Epoch_Date,format='yday')
                        Astrosat_second=(YDAY-Epoch_Date_yday).sec-1

                 else: 
                      Epoch_Date_yday=Time(Epoch_Date_UTC,format='yday')
                      Astrosat_second=(YDAY-Epoch_Date_yday).sec-2                
        else:
             Astrosat_second=0.0
        Astrosat_yday=YDAY.yday
        Astrosat_iso=YDAY.iso
        Astrosat_jd=YDAY.jd
        Astrosat_mjd=YDAY.mjd
    if YDAY.scale =='utc' and outflag=='tt':
        if YDAY>Epoch_Date_UTC:
                 if YDAY<First_LeapSec:
                        Epoch_Date_yday=Time(Epoch_Date_UTC,format='yday')
                        Astrosat_second=(YDAY-Epoch_Date_yday).sec
                 elif YDAY<Second_LeapSec:
                        Epoch_Date_yday=Time(Epoch_Date,format='yday')
                        Astrosat_second=(YDAY-Epoch_Date_yday).sec-1

                 else: 
                      Epoch_Date_yday=Time(Epoch_Date_UTC,format='yday')
                      Astrosat_second=(YDAY-Epoch_Date_yday).sec-2                
        else:
             Astrosat_second=0.0
        Astrosat_yday=YDAY.tt.yday
        Astrosat_iso=YDAY.tt.iso
        Astrosat_jd=YDAY.tt.jd
        Astrosat_mjd=YDAY.tt.mjd
    if YDAY.scale =='tt' and outflag=='tt':
        if YDAY>Epoch_Date_TT:
                 if YDAY<First_LeapSec:
                        Epoch_Date_yday=Time(Epoch_Date_TT,format='yday')
                        Astrosat_second=(YDAY-Epoch_Date_yday).sec
                 elif YDAY<Second_LeapSec:
                        Epoch_Date_yday=Time(Epoch_Date_TT,format='yday')
                        Astrosat_second=(YDAY-Epoch_Date_yday).sec-1

                 else: 
                      Epoch_Date_yday=Time(Epoch_Date_TT,format='yday')
                      Astrosat_second=(YDAY-Epoch_Date_yday).sec-2                
        else:
             Astrosat_second=0.0
        Astrosat_yday=YDAY.tt.yday
        Astrosat_iso=YDAY.tt.iso
        Astrosat_jd=YDAY.tt.jd
        Astrosat_mjd=YDAY.tt.mjd
    if YDAY.scale =='tt' and outflag=='utc':
        if YDAY>Epoch_Date_TT:
                 if YDAY<First_LeapSec:
                        Epoch_Date_yday=Time(Epoch_Date_TT,format='yday')
                        Astrosat_second=(YDAY-Epoch_Date_yday).sec
                 elif YDAY<Second_LeapSec:
                        Epoch_Date_yday=Time(Epoch_Date_TT,format='yday')
                        Astrosat_second=(YDAY-Epoch_Date_yday).sec-1

                 else: 
                      Epoch_Date_yday=Time(Epoch_Date_TT,format='yday')
                      Astrosat_second=(YDAY-Epoch_Date_yday).sec-2                
        else:
             Astrosat_second=0.0
        Astrosat_yday=YDAY.utc.yday
        Astrosat_iso=YDAY.utc.iso
        Astrosat_jd=YDAY.utc.jd
        Astrosat_mjd=YDAY.utc.mjd
    return Astrosat_iso,Astrosat_yday,Astrosat_jd,Astrosat_mjd,Astrosat_second


#print convertYDAYtosec(YDAY,'tt')




def convertJDtosec(JD,outflag):
    if JD.scale =='utc' and outflag=='utc':
        if JD>Epoch_Date_UTC:
                 if JD<First_LeapSec:
                        Epoch_Date_jd=Time(Epoch_Date_UTC,format='jd')
                        Astrosat_second=(JD-Epoch_Date_jd).sec
                 elif JD<Second_LeapSec:
                        Epoch_Date_jd=Time(Epoch_Date_UTC,format='jd')
                        Astrosat_second=(JD-Epoch_Date_jd).sec-1

                 else: 
                        Epoch_Date_jd=Time(Epoch_Date_UTC,format='jd')
                        Astrosat_second=(JD-Epoch_Date_jd).sec-2
        else:
              Astrosat_second=0.0
        Astrosat_jd=JD.jd
        Astrosat_iso=JD.iso
        Astrosat_yday=JD.yday
        Astrosat_mjd=JD.mjd
    if JD.scale =='utc' and outflag=='tt':
        if JD>Epoch_Date_UTC:
                 if JD<First_LeapSec:
                        Epoch_Date_jd=Time(Epoch_Date_UTC,format='jd')
                        Astrosat_second=(JD-Epoch_Date_jd).sec
                 elif JD<Second_LeapSec:
                        Epoch_Date_jd=Time(Epoch_Date_UTC,format='jd')
                        Astrosat_second=(JD-Epoch_Date_jd).sec-1
                 else: 
                        Epoch_Date_jd=Time(Epoch_Date_UTC,format='jd')
                        Astrosat_second=(JD-Epoch_Date_jd).sec-2
        else:
              Astrosat_second=0.0
        Astrosat_jd=JD.tt.jd
        Astrosat_iso=JD.tt.iso
        Astrosat_yday=JD.tt.yday
        Astrosat_mjd=JD.tt.mjd
    if JD.scale =='tt' and outflag=='tt':
        if JD>Epoch_Date_TT:
                 if JD<First_LeapSec:
                        Epoch_Date_jd=Time(Epoch_Date_TT,format='jd')
                        Astrosat_second=(JD-Epoch_Date_jd).sec
                 elif JD<Second_LeapSec:
                        Epoch_Date_jd=Time(Epoch_Date_TT,format='jd')
                        Astrosat_second=(JD-Epoch_Date_jd).sec-1

                 else: 
                        Epoch_Date_jd=Time(Epoch_Date_TT,format='jd')
                        Astrosat_second=(JD-Epoch_Date_jd).sec-2
        else:
              Astrosat_second=0.0
        Astrosat_jd=JD.tt.jd
        Astrosat_iso=JD.tt.iso
        Astrosat_yday=JD.tt.yday
        Astrosat_mjd=JD.tt.mjd
    if JD.scale =='tt' and outflag=='utc':
        print 'Hello World'
        if JD>Epoch_Date_TT: 
                 if JD<First_LeapSec:
                        Epoch_Date_jd=Time(Epoch_Date_TT,format='jd')
                        Astrosat_second=(JD-Epoch_Date_jd).sec
                 elif JD<Second_LeapSec:
                        Epoch_Date_jd=Time(Epoch_Date_TT,format='jd')
                        Astrosat_second=(JD-Epoch_Date_jd).sec-1

                 else: 
                        Epoch_Date_jd=Time(Epoch_Date_TT,format='jd')
                        Astrosat_second=(JD-Epoch_Date_jd).sec-2
        else:
              Astrosat_second=0.0
        Astrosat_jd=JD.utc.jd
        Astrosat_iso=JD.utc.iso
        Astrosat_yday=JD.utc.yday
        Astrosat_mjd=JD.utc.mjd
    return Astrosat_iso,Astrosat_yday,Astrosat_jd,Astrosat_mjd,Astrosat_second


#print convertJDtosec(JD,'tt')


def convertMJDtosec(MJD,outflag):
    if MJD.scale =='utc' and outflag=='utc':
        if MJD>Epoch_Date_UTC:
                 if MJD<First_LeapSec:
                        Epoch_Date_mjd=Time(Epoch_Date_UTC,format='mjd')
                        Astrosat_second=(MJD-Epoch_Date_mjd).sec
                 elif MJD<Second_LeapSec:
                        Epoch_Date_mjd=Time(Epoch_Date_UTC,format='mjd')
                        Astrosat_second=(MJD-Epoch_Date_mjd).sec-1

                 else: 
                        Epoch_Date_mjd=Time(Epoch_Date_UTC,format='mjd')
                        Astrosat_second=(MJD-Epoch_Date_mjd).sec-2
        else:
              Astrosat_second=0.0
        Astrosat_jd=MJD.jd
        Astrosat_iso=MJD.iso
        Astrosat_yday=MJD.yday
        Astrosat_mjd=MJD.mjd
    if MJD.scale =='utc' and outflag=='tt':
        if MJD>Epoch_Date_UTC:
                 if MJD<First_LeapSec:
                        Epoch_Date_mjd=Time(Epoch_Date_UTC,format='mjd')
                        Astrosat_second=(MJD-Epoch_Date_mjd).sec
                 elif MJD<Second_LeapSec:
                        Epoch_Date_mjd=Time(Epoch_Date_UTC,format='mjd')
                        Astrosat_second=(MJD-Epoch_Date_mjd).sec-1

                 else: 
                        Epoch_Date_mjd=Time(Epoch_Date_UTC,format='mjd')
                        Astrosat_second=(MJD-Epoch_Date_mjd).sec-2
        else:
              Astrosat_second=0.0
        Astrosat_jd=MJD.tt.jd
        Astrosat_iso=MJD.tt.iso
        Astrosat_yday=MJD.tt.yday
        Astrosat_mjd=MJD.tt.mjd
    if MJD.scale =='tt' and outflag=='tt':
        if MJD>Epoch_Date_TT:
                 if MJD<First_LeapSec:
                        Epoch_Date_mjd=Time(Epoch_Date_TT,format='mjd')
                        Astrosat_second=(MJD-Epoch_Date_mjd).sec
                 elif MJD<Second_LeapSec:
                        Epoch_Date_mjd=Time(Epoch_Date_TT,format='mjd')
                        Astrosat_second=(MJD-Epoch_Date_mjd).sec-1

                 else: 
                        Epoch_Date_mjd=Time(Epoch_Date_TT,format='mjd')
                        Astrosat_second=(MJD-Epoch_Date_mjd).sec-2
        else:
              Astrosat_second=0.0
        Astrosat_jd=MJD.tt.jd
        Astrosat_iso=MJD.tt.iso
        Astrosat_yday=MJD.tt.yday
        Astrosat_mjd=MJD.tt.mjd
    if MJD.scale =='tt' and outflag=='utc':
        if MJD>Epoch_Date_TT: 
                 if MJD<First_LeapSec:
                        Epoch_Date_mjd=Time(Epoch_Date_TT,format='mjd')
                        Astrosat_second=(MJD-Epoch_Date_mjd).sec
                 elif MJD<Second_LeapSec:
                        Epoch_Date_jd=Time(Epoch_Date_TT,format='mjd')
                        Astrosat_second=(MJD-Epoch_Date_mjd).sec-1

                 else: 
                        Epoch_Date_mjd=Time(Epoch_Date_TT,format='mjd')
                        Astrosat_second=(MJD-Epoch_Date_mjd).sec-2
        else:
              Astrosat_second=0.0
        Astrosat_jd=MJD.utc.jd
        Astrosat_iso=MJD.utc.iso
        Astrosat_yday=MJD.utc.yday
        Astrosat_mjd=MJD.utc.mjd
    return Astrosat_iso,Astrosat_yday,Astrosat_jd,Astrosat_mjd,Astrosat_second


#print convertJDtosec(MJD,'utc')


def convertAStoAllUTC(Astrosat_second,outflag):
    if outflag=='utc':
          if Astrosat_second>0:
               Astrosat_second=TimeDelta(Astrosat_second, format='sec')
          else:
               Astrosat_second=TimeDelta(0.0, format='sec')
    Astrosat_iso=(Epoch_Date_UTC+Astrosat_second).iso
    Astrosat_yday=(Epoch_Date_UTC+Astrosat_second).yday
    Astrosat_jd=(Epoch_Date_UTC+Astrosat_second).jd
    Astrosat_mjd=(Epoch_Date_UTC+Astrosat_second).mjd
    return Astrosat_iso,Astrosat_yday,Astrosat_jd,Astrosat_mjd,Astrosat_second


def convertAStoAllTT(Astrosat_second,outflag):
    if outflag=='tt':
         if Astrosat_second>0:
               Astrosat_second=TimeDelta(Astrosat_second, format='sec')
         else:
               Astrosat_second=TimeDelta(0.0, format='sec')
    
    Astrosat_iso=(Epoch_Date_UTC+Astrosat_second).tt.iso
    Astrosat_yday=(Epoch_Date_UTC+Astrosat_second).tt.yday
    Astrosat_jd=(Epoch_Date_UTC+Astrosat_second).tt.jd
    Astrosat_mjd=(Epoch_Date_UTC+Astrosat_second).tt.mjd
    return Astrosat_iso,Astrosat_yday,Astrosat_jd,Astrosat_mjd,Astrosat_second
    

#print convertAStoAllTT(1234567.0,'tt')


parser = argparse.ArgumentParser()
parser.add_argument("inflag", help= "Enter ASTROSAT seconds since 2010.0 UTC (decimal) to covert to other formats ", type=str)
parser.add_argument("outflag", help= "Enter ASTROSAT seconds since 2010.0 UTC (decimal) to covert to other formats ", type=str)
parser.add_argument("-isdit",   "--Astrosat_isout",  help= "Enter Astrosat Time in ISO 8601 date (yyyy-MM-dd hh:mm:ss) to covert to other formatsss ", type=str)
group = parser.add_mutually_exclusive_group()
group.add_argument("-ati",     "--Astrosat_second", help= "Enter ASTROSAT seconds since 2010.0 UTC (decimal) to covert to other formats ", type=float)
group.add_argument("-isdid",    "--Astrosat_isod",   help= "Enter Astrosat Time in ISO 8601 date (yyyy-MM-dd hh:mm:ss) to covert to other formats ",type=str)
group.add_argument("-ydni",    "--Astrosat_yday",   help= "Enter Astrosat Time in Year and day number (yyyy:ddd:hh:mm:ss) to covert to other formats ", type=str)
group.add_argument("-jdi",     "--Astrosat_jd",     help= "Enter Astrosat Time in Julian Day (ddddddd.ddd...) to covert to other formats ",  type=float)
group.add_argument("-mjdi",    "--Astrosat_mjd",    help= "Astrosat Time in Modified Julian Day (ddddd.ddd...) to covert to other formats  ", type=float)

args = parser.parse_args()
Astrosat_iso=str(args.Astrosat_isod)+str(" ")+str(args.Astrosat_isout)




################################################ Execution of time fields for Astrosat Seconds  ###############################################

if args.Astrosat_second and args.outflag=='UTC':
       print convertAStoAllUTC(args.Astrosat_second,'utc')[0], convertAStoAllUTC(args.Astrosat_second,'utc')[1], convertAStoAllUTC(args.Astrosat_second,'utc')[2], convertAStoAllUTC(args.Astrosat_second,'utc')[3], convertAStoAllUTC(args.Astrosat_second,'utc')[4]

if args.Astrosat_second and args.outflag=='TT':
       print convertAStoAllTT(args.Astrosat_second,'tt')[0], convertAStoAllTT(args.Astrosat_second,'tt')[1], convertAStoAllTT(args.Astrosat_second,'tt')[2], convertAStoAllTT(args.Astrosat_second,'tt')[3], convertAStoAllTT(args.Astrosat_second,'tt')[4]

if  args.Astrosat_second==0 and args.outflag=='UTC':
           print '2010-01-01 00:00:00.000', '2010:001:00:00:00.000' ,2455197.50000000, 55197.000 ,0.0
if  args.Astrosat_second==0 and args.outflag=='TT':
           print '2010-01-01 00:01:06.184', '2010:001:00:01:06.184' ,2455197.50076602, 55197.00076602,0.0


################################################ Execution of time fields for ISO  8601 date   ###############################################

if args.Astrosat_isod and args.Astrosat_isout and args.inflag=='UTC' and args.outflag=='UTC':
       Current_iso=Time(Astrosat_iso, scale='utc')
       print  convertISODatetosec(Current_iso,'utc')[0],  convertISODatetosec(Current_iso,'utc')[1], convertISODatetosec(Current_iso,'utc')[2], convertISODatetosec(Current_iso,'utc')[3],  convertISODatetosec(Current_iso,'utc')[4]

if args.Astrosat_isod and args.Astrosat_isout and args.inflag=='UTC' and args.outflag=='TT':
       Current_iso=Time(Astrosat_iso, scale='utc')
       print convertISODatetosec(Current_iso,'tt')[0] ,convertISODatetosec(Current_iso,'tt')[1],convertISODatetosec(Current_iso,'tt')[2],convertISODatetosec(Current_iso,'tt')[3], convertISODatetosec(Current_iso,'tt')[4]

if args.Astrosat_isod and args.Astrosat_isout and args.inflag=='TT' and args.outflag=='TT':
       Current_iso=Time(Astrosat_iso, scale='tt')
       print  convertISODatetosec(Current_iso,'tt')[0],  convertISODatetosec(Current_iso,'tt')[1], convertISODatetosec(Current_iso,'tt')[2], convertISODatetosec(Current_iso,'tt')[3],  convertISODatetosec(Current_iso,'tt')[4]

if  args.Astrosat_isod and args.Astrosat_isout and args.inflag=='TT' and args.outflag=='UTC':
    Current_iso=Time(Astrosat_iso, scale='tt')
    print  convertISODatetosec(Current_iso,'utc')[0],  convertISODatetosec(Current_iso,'utc')[1], convertISODatetosec(Current_iso,'utc')[2], convertISODatetosec(Current_iso,'utc')[3],  convertISODatetosec(Current_iso,'utc')[4]



################################################ Execution of time fields for  Year and day number   ###############################################

if args.Astrosat_yday and args.inflag=='UTC' and args.outflag=='UTC':
    Current_yday=Time(args.Astrosat_yday, scale='utc')
    print convertYDAYtosec(Current_yday,'utc')[0],  convertYDAYtosec(Current_yday,'utc')[1], convertYDAYtosec(Current_yday,'utc')[2],  convertYDAYtosec(Current_yday,'utc')[3],  convertYDAYtosec(Current_yday,'utc')[4]

if  args.Astrosat_yday and args.inflag=='UTC' and args.outflag=='TT':
    Current_yday=Time(args.Astrosat_yday, scale='utc')
    print convertYDAYtosec(Current_yday,'tt')[0],  convertYDAYtosec(Current_yday,'tt')[1],  convertYDAYtosec(Current_yday,'tt')[2],  convertYDAYtosec(Current_yday,'tt')[3],  convertYDAYtosec(Current_yday,'tt')[4]

if  args.Astrosat_yday and args.inflag=='TT' and args.outflag=='TT':
    Current_yday=Time(args.Astrosat_yday, scale='tt')
    print convertYDAYtosec(Current_yday,'tt')[0],  convertYDAYtosec(Current_yday,'tt')[1],  convertYDAYtosec(Current_yday,'tt')[2],  convertYDAYtosec(Current_yday,'tt')[3],  convertYDAYtosec(Current_yday,'tt')[4]

if args.Astrosat_yday and args.inflag=='TT' and args.outflag=='UTC':
    Current_yday=Time(args.Astrosat_yday, scale='tt')
    print convertYDAYtosec(Current_yday,'utc')[0],  convertYDAYtosec(Current_yday,'utc')[1], convertYDAYtosec(Current_yday,'utc')[2],  convertYDAYtosec(Current_yday,'utc')[3],  convertYDAYtosec(Current_yday,'utc')[4]



################################################ Execution of time fields for Julian Day  ###############################################
if args.Astrosat_jd and args.inflag=='UTC' and args.outflag=='UTC':
    Current_jd=Time(args.Astrosat_jd, format='jd', scale='utc')
    print convertJDtosec(Current_jd,'utc')[0],  convertJDtosec(Current_jd,'utc')[1],  convertJDtosec(Current_jd,'utc')[2], convertJDtosec(Current_jd,'utc')[3], convertJDtosec(Current_jd,'utc')[4]

if  args.Astrosat_jd and args.inflag=='UTC' and args.outflag=='TT':
    Current_jd=Time(args.Astrosat_jd, format='jd', scale='utc')
    print convertJDtosec(Current_jd,'tt')[0],  convertJDtosec(Current_jd,'tt')[1],  convertJDtosec(Current_jd,'tt')[2], convertJDtosec(Current_jd,'tt')[3], convertJDtosec(Current_jd,'tt')[4]

if  args.Astrosat_jd and args.inflag=='TT' and args.outflag=='TT':
    Current_jd=Time(args.Astrosat_jd, format='jd', scale='tt')
    print convertJDtosec(Current_jd,'tt')[0],  convertJDtosec(Current_jd,'tt')[1],  convertJDtosec(Current_jd,'tt')[2], convertJDtosec(Current_jd,'tt')[3], convertJDtosec(Current_jd,'tt')[4]

if args.Astrosat_jd and args.inflag=='TT' and args.outflag=='UTC':
    Current_jd=Time(args.Astrosat_jd, format='jd', scale='tt')
    print convertJDtosec(Current_jd,'utc')[0],  convertJDtosec(Current_jd,'utc')[1],  convertJDtosec(Current_jd,'utc')[2], convertJDtosec(Current_jd,'utc')[3], convertJDtosec(Current_jd,'utc')[4]



################################################ Execution of time fields for Modified Julian Day  #######################################
if args.Astrosat_mjd and args.inflag=='UTC' and args.outflag=='UTC':
       Current_mjd=Time(args.Astrosat_mjd,format='mjd', scale='utc')
       print convertMJDtosec(Current_mjd,'utc')[0],convertMJDtosec(Current_mjd,'utc')[1],convertMJDtosec(Current_mjd,'utc')[2],convertMJDtosec(Current_mjd,'utc')[3],convertMJDtosec(Current_mjd,'utc')[4]

if args.Astrosat_mjd and args.inflag=='UTC' and args.outflag=='TT':
       Current_mjd=Time(args.Astrosat_mjd,format='mjd', scale='utc')
       print convertMJDtosec(Current_mjd,'tt')[0],convertMJDtosec(Current_mjd,'tt')[1],convertMJDtosec(Current_mjd,'tt')[2],convertMJDtosec(Current_mjd,'tt')[3],convertMJDtosec(Current_mjd,'tt')[4]

if args.Astrosat_mjd and args.inflag=='TT' and args.outflag=='TT':
       Current_mjd=Time(args.Astrosat_mjd,format='mjd', scale='tt')
       print convertMJDtosec(Current_mjd,'tt')[0],convertMJDtosec(Current_mjd,'tt')[1],convertMJDtosec(Current_mjd,'tt')[2],convertMJDtosec(Current_mjd,'tt')[3],convertMJDtosec(Current_mjd,'tt')[4]

if args.Astrosat_mjd and args.inflag=='TT' and args.outflag=='UTC':
       Current_mjd=Time(args.Astrosat_mjd,format='mjd', scale='tt')
       print convertMJDtosec(Current_mjd,'utc')[0],convertMJDtosec(Current_mjd,'utc')[1],convertMJDtosec(Current_mjd,'utc')[2],convertMJDtosec(Current_mjd,'utc')[3],convertMJDtosec(Current_mjd,'utc')[4]


