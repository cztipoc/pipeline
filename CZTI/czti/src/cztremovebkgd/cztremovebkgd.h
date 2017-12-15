/*****************************************
 * File: cztremovebkgd.h
 * Author - Tanul Gupta
 * File created - June 20, 2013 ; 15:01 
 ****************************************/
#ifndef CZTREMOVEBKGD_H
#define CZTREMOVEBKGD_H

#include<pil.h>
#include "glog/logging.h"
#include<linalg.h>
#include<ap.h>
#include"utils.h"
#include"ExpMap.h"
#include "cztstring.h"
#include "cztmatrix.h"


#define NUMQUAD_ROW 2
#define NUMQUAD_COL 2

using namespace std;


class cztremovebkgd {
private:
    char modulename[NAMESIZE];
    char dpifile[PIL_LINESIZE];
    char shadowfile[PIL_LINESIZE];
    float threshold;
    char outfile[PIL_LINESIZE];
    char quad_no[25];
    char nobkgdfile[PIL_LINESIZE];
    int nBkgdCoef; //number of background coefficients
    int debugmode; //if yes/true, creates intermediate file 
    int clobber;
    int history;

    //data parameters
    float *dpi;
    
    int knownSources; //number of sources/peaks found, number of knownSources in the field of view
    int size; //size after oversampling
    int skyimgsize; //size for skyimage
    float **shadows, **bkgdshadows, **knownShadows;
    
    int getHistory(vector<string> &vhistory);
    int getDPI();
    
    /**
     * Function to subtract known source shadows from the DPI
     * @return known source subracted DPI
     */   

public:
    cztremovebkgd();
    ~cztremovebkgd();
    int read(int argc, char **argv);
    int read(char *dpifile, char *shadowfile, float threshold, char *outfile, char* quad_no, int nBkgdCoef, int debugmode = NO,
            int clobber = YES, int history = YES);
    void display();

    /**
     * Function to create image from DPI
     * @param par
     * @return 
     */
    int cztremovebkgdProcess();

    /**
     * Generates required number of background shadows maximum is six
     * Shadows should have size [nBkgdCoef][XSIZE*YSIZE]
     * Shadows are stored in the array 'bkgdshadows'
     * @return 
     */
    int getBkgdShadows();

    /**
     * Function to subtract known source shadows from the DPI
     * @return known source + background subracted DPI
     */
    int knownSourceSubtractedDPI();
    
    /**
     * Function to write dpi file
     * @return DPI file in fits format.
     */
    int writeDPI();
};

#endif	/* CZTREMOVEBKGD_H */
