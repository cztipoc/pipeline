/* 
 * File:   cztfilterbadpix.h
 * Author: preeti
 *
 * Created on September 27, 2012, 3:42 PM
 */

#ifndef CZTFILTERBADPIX_H
#define	CZTFILTERBADPIX_H
#include "glog/logging.h"

#include "utils.h"

class cztfilterbadpix{
private:
    //data members
    char modulename[NAMESIZE];
    char infile[PIL_LINESIZE];    //input event file/DPI
    char inputtype[15];              //event/dpi
    char qualitymapfile[PIL_LINESIZE];   //quality map file generated from cztfindbadpix
    char outfile[PIL_LINESIZE];  //output filename, will serve as prefix if more than one DPI is to be produced
                                 //will produce dpi if input is dpi or produce event file if input is event file
                                 //if infile and outfile are same or outfile is -, then infile will be updated       
    char outputquality[25];      //good/bad/noisy/dead
    char xcol[25];
    char ycol[25];
    int history;
    int clobber;
    
public:
    cztfilterbadpix();
    void display();
    int read(int argc,char **argv);
    int read(char *infile,char *inputtype,char *badpixfile,char *outfile,
        char *outputquality,char *xcol,char *ycol,int clobber=YES,int history=YES);
    
    /**
    * Function to filter bad pixels from event file or dpi file
    * output file only contains good/dead/noisy/bad pixels
    * @return 
    */
    int cztfilterbadpixProcess();
    
    /**
    * Function to generate history for the module, to be written to the output file
    * @param vhistory
    * @return 
    */
    int getHistory(vector<string> &vhistory);
};

struct Q_coordinatelist{
    vector<int> vqx,vqy;
};

int xyExist(int x,int y,vector<int> &vx,vector<int> &vy);

#endif	/* CZTFILTERBADPIX_H */

