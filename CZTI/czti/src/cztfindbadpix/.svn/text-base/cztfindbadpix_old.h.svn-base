/* 
 * File:   cztfindbadpix.h
 * Author: preeti
 *
 * Created on September 27, 2012, 4:49 PM
 */

#ifndef CZTFINDBADPIX_H
#define	CZTFINDBADPIX_H

#include <pil.h>
#include "utils.h"
#include "ExpMap.h"
#include "Fitting.h"
#include "glog/logging.h"
#include "level2validation.h"
#include <vector>
#include <string>
#include "caldbHandler.h"

#define SETBOUNDRY 0.5

class cztfindbadpix{
private:
    //data members
    char modulename[NAMESIZE];
    char infile[PIL_LINESIZE];    //input event file/DPI
    char inputtype[15];              //event/dpi
    char CALDBbadpixelfile[PIL_LINESIZE];   //bad pixel file generated from cztfindbadpix
    char outfile[PIL_LINESIZE];      //output bad pixel file  
    int useAlgo;                 //whether to use user algorithm to detect dead and noisy pixels
    float threshold;
    char xcol[25],ycol[25];
    char aspectfile[PIL_LINESIZE];
    char maskfile8100[PIL_LINESIZE];     //subsampled mask file with 8100x8100 pixels 
    char ra[25],dec[25];     // - for default
    int history;
    int clobber;
public:
    cztfindbadpix();
    void display();
    int read(int argc,char **argv);
    int read(char *infile,char *inputtype,char *CALDBbadpixfile,char *outfile,
        int useAlgo,float threshold,char *xcol,char *ycol,char *aspectfile,
        char *maskfile,char *ra,char *dec,int clobber=YES,int history=YES);
    
    /**
    * Function to detect bad pixels from event file or dpi file
    * output file contains a bad pixel map with quality as
    * 0 - GOOD
    * 1 - DEAD
    * 2 - NOISY
    * 3 - BAD
    * @return 
    */
    int cztfindbadpixProcess();
    
    /**
    * Function to generate history for the module, to be written to the output file
    * @param vhistory
    * @return 
    */
    int getHistory(vector<string> &vhistory);

};



/**
 * FUnction to create a bad pixel quality map file
 * It create the output file with an image of xsize x ysize
 * It also copies keywords from primary header of input file
 * @param fin
 * @param outfile
 * @param xsize
 * @param ysize
 * @return 
 */
int createBadPixFile(fitsfile *fin,char *outfile,int xsize,int ysize);

/**
 * Create bad pixel map from caldb, CALDB contains an image of badpixels.
 * It returns bad pixel map with bad pixel quality as 3 (BADPIX)
 * @param caldbfile
 * @param qmap
 * @param xsize
 * @param ysize
 * @return 
 */
int getBadPix4mCALDB(char *caldbfile,unsigned char *qmap);

/**
 * Function to detect dead and noiy pixels using a user algorithm and update 
 * quality map (qmap) from dpi
 * @param dpi
 * @param ra
 * @param dec
 * @param aspectfile
 * @param maskfile
 * @param threshold
 * @param qmap
 * @return 
 */
int detectDeadNoisyPix(long *dpi,double ra,double dec,char *aspectfile,char *maskfile,
        float threshold,unsigned char *qmap);

/**
 * Function to write quality map to output FITS file
 * @param outfile
 * @param qualitymap
 * @return 
 */
int writeQmap(char *outfile,unsigned char *qualitymap);


#endif	/* CZTFINDBADPIX_H */

