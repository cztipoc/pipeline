/* 
 * File:   cztdpigen.h
 * Author: preeti
 *
 * Created on September 18, 2012, 6:05 PM
 */

#ifndef CZTDPIGEN_H
#define	CZTDPIGEN_H

#include "utils.h"
#include "glog/logging.h"
#include "cztstring.h"
#include "validations.h"

class cztdpigen{
private:
    //data members
    char modulename[NAMESIZE];
    char infile[PIL_LINESIZE];    //input event file
    char outfile[PIL_LINESIZE];   //output filename, will serve as prefix if more than one DPI is to be produced
    char qefile[PIL_LINESIZE];    //Quantum efficiency file
    char ebins[PIL_LINESIZE];
    char timerange[PIL_LINESIZE];
    char ecol[25];
    char tcol[25];
    char xcol[25];
    char ycol[25];
    char quad_no[25]; //new addition to incorporate quadrant wise functionality
    int ntbins;
    int nebins;  //number of energy bins, decides number of output files to be produced
    int ndpi;
    double *emin,*emax;
    double *tstart,*tstop;
    int history;
    int clobber;
public:
    /**************************************************************************************
     * cztdpigen() - initializes some of the values like modulename, quad_no, ntbins etc.
     * display() - 
    **************************************************************************************/
    
    cztdpigen();
    void display();
    int read(int argc,char **argv);
    int read(char *infile,char *qefile,char *outfile, char *quad_no, char *ebins,char *timerange,
        char *ecol,char *tcol,char *xcol,char *ycol,int clobber=YES,
        int history=YES);
    
    /**
    * Function to generate DPI from event file
    * @param par
    * @return 
    */
    int cztdpigenProcess();   
    
    /**
    * Function to generate history for the module to be written to the output file
    * @param vhistory
    * @return 
    */
    int getHistory(vector<string> &vhistory);
    
};

struct DPI{
    long *pixels;
    double emin,emax,tstart,tstop;
    int quadid;
    int dpisize;
    char ecol[25];
    DPI(long *pixels,double emin,double emax,double tstart,double tstop,   //for full dpi quadid=-1
        int quadid,char *ecol,int dpisize){
        this->emin=emin;       this->emax=emax;
        this->tstart=tstart;   this->tstop=tstop;
        this->dpisize=dpisize;
        this->pixels=new long[dpisize];
        strcpy(this->ecol,ecol);
        memmove(this->pixels,pixels,sizeof(long)*dpisize);
    }
    ~DPI(){
        delete[] pixels;
    }
};

/**
 * Function to get the names of output files based on number of DPIs to be generated
 * The outputfiles are stored in vector outputfiles
 * Returns 0 on success
 * @param ndpi
 * @param outfile
 * @param outputfiles
 * @return 
 */
int getNoutputfiles(int ndpi,char *outfile,vector<string> &outputfiles);

/**
 * Function to create empty fits files for DPIs. All files have one primary extension, 4 extensions for
 * 4 quadrant DPIs and one for full DPI (in total 6 extensions).
 * This function also copies keywords from input file to output file 
 * @param fin
 * @param outputfiles
 * @return 
 */
int createOutputFiles4DPI(fitsfile *fin,vector<string> &outputfiles);

/**
 * Function to write DPI image to specific HDU in file. The structure contains all information
 * for the DPI to be written
 * @param fptr
 * @param hdu
 * @param dpi
 * @return 
 */
int writeDPI(fitsfile *fptr,int hdu,DPI &dpi);

/**
 * Function to get tstart and tstop values from timerange string
 * Reads time from input event data file if timerange is '-'
 * else reads it from event file and provide the maximum tstart and minimum tstop i.e. tstart and tstop which is
 * which is common to all quadrants.
 * @param fptr
 * @param timerange
 * @param tstart
 * @param tstop
 * @return 
 */
int getTime(fitsfile *fptr,char *timerange,int ntbins,double *tstart,double *tstop,char *tcol);

/**
 * Function to get emin and emax values for each ebin entered
 * @param ebins
 * @param ecol
 * @param nebins
 * @param emin
 * @param emax
 * @return 
 */
int getEnergy(char *ebins,char *ecol,int nebins,double *emin,double *emax);

#endif	/* CZTDPIGEN_H */

