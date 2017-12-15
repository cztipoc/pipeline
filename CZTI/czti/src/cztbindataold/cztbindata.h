/* 
 * File:   cztbindata.h
 * Author: preeti
 *
 * Created on October 15, 2012, 6:11 PM
 */

#ifndef CZTBINDATA_H
#define	CZTBINDATA_H

#include<pil.h>
#include<pthread.h>
#include<iomanip>
#include <string>
#include"utils.h"
#include"ExpMap.h"
#include "glog/logging.h"
#include "cztstring.h"
#include "validations.h"
#include "coordinateTransformation.h"

#define SPEC_EXTNAME "SPECTRUM"
#define LC_EXTNAME "LIGHTCURVE"


using namespace std;


class cztbindata{
private:
    char modulename[NAMESIZE];
    char infile[PIL_LINESIZE];   //input event file
    char maskfile[PIL_LINESIZE];
    char aspectfile[PIL_LINESIZE];
    char pixelqualityfile[PIL_LINESIZE];
    char outfile[PIL_LINESIZE];
    char timerange[PIL_LINESIZE];
    char energyrange[PIL_LINESIZE];
    char bintype[25];   //could be gti/uniform
    char gtifile[PIL_LINESIZE];  //required for gti binning
    char gtiextname[FLEN_VALUE];  //GTI extension name
    float binsize;                 //required for uniform binning 
    char outputtype[25];          //lc/spec
    char RA_str[25];           //if '-' then read Ra from event file
    char DEC_str[25];          //if '-' then read Dec from event file
    char tcol[25];
    char ecol[25];
    char xcol[25];
    char ycol[25];
    char quadsToProcess[25];
    double *binmin,*binmax;   //will hold bin start and bin stop for time/energy
    int nbins;                //will hold number of bins
    bool sflag;   //true for spectrum output type and false for temporal output.
    bool uflag;    //true for uniform binning
    double tstart,tstop,estart,estop;
    float ra,dec;
    int clobber;
    int history;
    int make_temp_files;
    
    //to store caldb file paths
    string compMaskFilepath;
    
    /**
    * Function to create energy bins/time bins based on input values
    * Sets the values of nbins, binmin and binmax in cztbindata_param object for lc/spec generation
    * @return 
    */
    int makeBins();
    
    
public:
    cztbindata();
    ~cztbindata();
    int read(int argc,char **argv);
    int read(char *infile,char *maskfile,char *aspectfile, char *quadsToProcess, char *pixelqualityfile,
        char *outfile,char *outtype,char *timerange,char *energyrange,char *bintype,
        char *gtifile,char *gtiextname,float binsize,char *Ra,char *Dec,char *tcol,char *ecol,
        char *xcol,char *ycol,int make_temp_files,int clobber=YES,int history=YES);
    void display();
    void clear();
    
    /**
    * Function to produce light curve or spectrum
    * @param par
    * @return 
    */
    int cztbindataProcess();

    /**
    * Function to generate history for the module, to be written to the output file
    * @param vhistory
    * @return 
    */
    int getHistory(vector<string> &vhistory);
    
};



/**
 * Function to prepare empty output file based on output type
 * It creates table for lc/spec data to be written
 * Copies primary header keywords from input to output file.
 * sflag gives the type of output file to be created.
 * ecol gives the name of ecol name
 * @param fevt
 * @param fout
 * @param sflag
 * @param ecol
 * @return 
 */
int prepareOutputFile(fitsfile *fevt,fitsfile *fout,bool sflag,char *ecol);

/**
 * Function to write LC/SPEC to output file already created. Writes data in second
 * extension of file
 * @param fout
 * @param xval
 * @param flux
 * @param error
 * @param num
 * @return 
 */
int writeCurve(fitsfile *fout,double *xval,double *flux,double *error,long num);

/**
 * Read Ra and Dec value from header of input file
 * @param fptr
 * @param ra
 * @param dec
 * @return 
 */
int readRaDec(fitsfile *fptr,float *ra, float *dec);

/**
 * Function to make weight of all bad pixels as zero
 * @param pixelqualityfile
 * @param weight
 * @return 
 */
/*********************************************************************************
 * function to assign 0 weight to all the badpixels as found in the file generated by cztfilterbadpix.
 * **********************************************************************************/
int filterWeight(char *pixelqualityfile,double *weight);

/**
 * Function to generate all DPIs required for light curve or spectrum
 * @param fevt
 * @param sflag
 * @param tstart
 * @param tstop
 * @param estart
 * @param estop
 * @param binmin
 * @param binmax
 * @param nbins
 * @param ecol
 * @param tcol
 * @param xcol
 * @param ycol
 * @param fulldpis
 * @return 
 */
int doBinning(fitsfile *fevt,bool sflag,double tstart,double tstop,double estart,
        double estop,double *binmin,double *binmax,int nbins,char *ecol,char *tcol,
        char *xcol,char *ycol,long **fulldpis);

 int Bin_quad(fitsfile *wevt, bool sflag, double tstart, double tstop, double estart, double estop, double *binmin, double *binmax, int nbins, float binsize,char* ecol, char* tcol, char* xcol,
        char* ycol, char* wcol,  double WA_quad, int quadrant_number, double* xval, double *flux, double *error);
 
 
int writeAllDPI(char *file,long **fulldpi,long num,long size=XSIZE*YSIZE);
#endif	/* CZTBINDATA_H */

/****************************************************
 * function to write event file with weights appended to each event 
 * based upon which detector pixel event had taken place.
 ****************************************************/
int writeWeightedEvent(char *file, double **mskwt_quad);

void printerror( int status, char* error);

