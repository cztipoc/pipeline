
/* 
 * @file cztfindbadpix_v2.h
 * @author Tanul Gupta
 * @date Created on May 18, 2015, 9:28 AM
 * @brief 
 * @details This module finds the dead and noisy pixels using the event data counts
 */

#ifndef CZTFINDBADPIX_V2_H
#define	CZTFINDBADPIX_V2_H

#include "pil.h"
#include "glog/logging.h"
#include <vector>
#include <string>
#include "caldbHandler.h"
#include "badpixCALDB.h"
#include "level2validation.h"
#include "ExpMap.h"
#include "Fitting.h"
#include "coordinateTransformation.h"


#define SETBOUNDRY 0.5

class Cztfindbadpix {
private:
    //data members
    char modulename[NAMESIZE];
    char infile[PIL_LINESIZE]; //input event file/DPI
    char inputtype[15]; //event/dpi
    char CALDBbadpixelfile[PIL_LINESIZE]; //bad pixel file from CALDB
    char outfile[PIL_LINESIZE]; //output bad pixel file  
    int useAlgo; //whether to use user algorithm to detect dead and noisy pixels
    float threshold;
    char xcol[25], ycol[25];
    char aspectfile[PIL_LINESIZE];
    char maskfile8100[PIL_LINESIZE]; //subsampled mask file with 8100x8100 pixels 
    char ra[25], dec[25]; // - for default
    int history;
    int clobber;
public:
    Cztfindbadpix();
    void display();
    int read(int argc, char **argv);
    int read(char *infile, char *inputtype, char *CALDBbadpixfile, char *outfile,
            int useAlgo, float threshold, char *xcol, char *ycol, char *aspectfile,
            char *maskfile, char *ra, char *dec, int clobber = YES, int history = YES);

    /**
     * Function to detect bad pixels from event file or dpi file
     * output file contains a bad pixel map with quality as
     * 0 - GOOD
     * 1 - DEAD
     * 2 - NOISY
     * 3 - BAD
     * @return 
     */
    int cztfindbadpix_process();

    /**
     * Function to generate history for the module, to be written to the output file
     * @param vhistory
     * @return 
     */
    int get_history(vector<string> &vhistory);

};
/**
 * Function to create a bad pixel quality map file. Creates an output badpixel
 * image file of size 128x128.
 * @param fin: input event/dpi file
 * @param outfile: output badpixel file
 * @param xsize: width of badpixel image
 * @param ysize: height of badpixel image.
 * @return 
 */
int create_badpix_file(fitsfile *fin, char *outfile, int xsize, int ysize);


/**
 * Function to write quality map file
 * @param outfile
 * @param qualitymap
 * @return 
 */
int write_qmap(char *outfile, unsigned char *qualitymap);


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

int getDPI2(fitsfile *fptr, char* extname, double emin, double emax, double tstart, double tstop,
        char *ecol, char *tcol, char *xcol, char *ycol, long *dpi_pixels);

int getFullDPI2(fitsfile *fptr, char *type, char *xcol, char *ycol, long *dpi);

#endif	/* CZTFINDBADPIX_V2_H */

