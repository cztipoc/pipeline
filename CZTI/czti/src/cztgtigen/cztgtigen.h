/* 
 * File:   cztgtigen.h
 * Author: preeti
 *
 * Created on September 11, 2012, 10:16 AM
 */

#ifndef CZTGTIGEN_H
#define	CZTGTIGEN_H

#include<pil.h>
#include "utils.h"
#include "glog/logging.h"
#include "validations.h"
#include "level1handler.h"
#include "caldbHandler.h"
#include "level2validation.h"
#include "macrodef.h"
#include "mkfRegeneration.h"
#include "Mvector.h"
#include <fstream>


#define MKF 1
#define CUSTOM 2
#define MKF_REFINED 3
#define CUSTOM_REFINED 4 
#define COPY 5

using namespace std;

class cztgtigen{
private:
    char modulename[NAMESIZE];
    char mkffile[PIL_LINESIZE];
    char thresholdfile[PIL_LINESIZE];  //ASCII file containing valid ranges for each parameter 
    char customgti[PIL_LINESIZE];    //custom good time intervals given in format t1-t2,t3-t4,....
    char l2gtifile[PIL_LINESIZE];    //existing gti file
    char runmode[25];   //must be lbt/custom/custom-refined/lbt-refined
    int runmodeflag;       //will be LBT/CUSTOM/LBT_REFINED/CUSTOM_REFINED
    char outfile[PIL_LINESIZE];
    char gtiextname[NAMESIZE];  //extension name for GTI
    int clobber;
    int history;
public:
    cztgtigen();
    void display();
    int read(int argc,char **argv);
    int read(char *runmode,char *mkffile,char *thresholdfile,char *customgti,
        char *gtifile,char *gtiextname,char *outfile,int clobber=NO,int history=NO);
    
    int cztgtigenProcess();
    void getHistory(vector<string> &vhistory);
    
};


class MKF_Threshold{
 public:
    string parameter;
    string range;
    float LV,UV;              //lower value and upper value; 
    void display();
    
};
int readMKF_Threshold(char *file,vector<MKF_Threshold> &lbt_thr);

struct GTI{
    double tstart,tstop;
};
//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------

int createGTIfile(char *file,char *extname);


int writeGTI(char *infile,int extnum,vector<GTI> &gti);

int readGTI(char *infile,int extnum,vector<GTI> &vectorGTI);

int refineGTI(vector<GTI> &gti2refine,vector<GTI> &ingti);

#endif	/* CZTGTIGEN_H */

