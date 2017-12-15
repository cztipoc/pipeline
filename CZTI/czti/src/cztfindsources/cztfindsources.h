/*****************************************
 * File: cztfindsources.h
 * Author - Tanul Gupta
 * File created - July 5, 2013 ; 10:17 
 ****************************************/
#ifndef CZTFINDSOURCES_H
#define CZTFINDSOURCES_H

#include<pil.h>
#include "glog/logging.h"
#include<linalg.h>
#include<ap.h>
#include"utils.h"
#include"ExpMap.h"
#include "cztstring.h"
#include "cztmatrix.h"

#define PEAKLIMIT 20
#define NUMQUAD_ROW 2
#define NUMQUAD_COL 2


using namespace std;

class cztfindsources{
private:
    //Input parameters
    char modulename[NAMESIZE];
    char imagefile[PIL_LINESIZE];   //Input DPI file for image creation
    char dpifile[PIL_LINESIZE];
    char maskfile64x64[PIL_LINESIZE];
    char maskfile[PIL_LINESIZE];
    char shadowfile[PIL_LINESIZE];
    char aspectfile[PIL_LINESIZE];
    float threshold;
    char sourcelist[PIL_LINESIZE];
    char outfile[PIL_LINESIZE];
    char quad_no[25];
    char nobkgdfile[PIL_LINESIZE];
    int oversamplingfactor;
    float source_strength_thr;
    int nBkgdCoef;   //number of background coefficients
    int debugmode;             //if yes/true, creates intermediate file 
    int clobber;
    int history;
    
    //data parameters
    float *dpi,*mask,*rawskyimage,*oversampledmask,*oversampleddpi, *nobkgdskyimage, *skyimage;
    int nsources, knownSources;                 //number of sources/peaks found, number of knownSources in the field of view
    int size;            //size after oversampling
    int skyimgsize;    //size for skyimage
    vector<double> thetax,thetay,flux,ra,dec;
    float **shadows,**bkgdshadows, **knownShadows;
    fitsfile *fdpi,*fout,*fnobkgd;
         
     int addWCS();
     
     /**
    * Function to read the (128 x 128) mask array from mask file and 
    * store it in the single dimnsion array mask of size 128x128.
    * @return - Returns 0 on success
    */
    int getMaskArray();
    
     /**
    * Function to read dpi for each quadrant from dpi file and 
    * store it in the single dimnsion array dpi of size 128x128.
    * @return - Returns 0 on success
    */
    int getDPI();
     
      
   //Function to oversample mask and DPI array by oversamplingfactor 
    void oversample();
    
     
    int identifyPeaks();
    
    /**
    * Function to create an output file named 'outfile' and write the skyimage to it
    * It also copies keywords from dpifile to ouput file 
    * @param dpifile
    * @param outfile
    * @param rawimg
    * @param size
    * @param oversamplefactor
    * @return 
    */
    int writeSkyImage();
    
    /**
    * Generates required number of background shadows maximum is six
    * Shadows should have size [nBkgdCoef][XSIZE*YSIZE]
    * Shadows are stored in the array 'shadows'
    * @return 
    */
    int getBkgdShadows();
    
    /**
    * Function to fit DPI and find source strengths and background
    * @param dpi - Observed DPI
    * @param shadows - Shadows for peaks found in image
    * @param nshadow - Number of peaks / shadows
    * @param bkgdshadow - Background shadows
    * @param nbkgd - Number of background coefficients
    * @param nsources - number of sources found, Value will be returned in this
    * @return 
    */

    
    int linearfitDPI();
    
    /**
    * Function to generate history for the module, to be written to the output file
    * @param vhistory
    * @return 
    */
    int getHistory(vector<string> &vhistory);
    
    /**
     * Function to subtract known source shadows from the DPI
     * @return known source subracted DPI
     */    
    int knownSourceSubtractedDPI();
    
public:  
    cztfindsources();
    ~cztfindsources();
    int read(int argc,char **argv);
    int read(char *imagefile, char* dpifile, char *maskfile,char *aspectfile,float threshold, char *sourcelist,float source_strength_thr, int debugmode=NO, int clobber=YES, int history=YES);
    void display();
  
    /**
    * Function to  image from DPI
    * @param par
    * @return 
    */
    int cztfindsourcesProcess();
};



void doFft(float data[], unsigned long nn, int isign);

/**
 * Function that retains  the central (XSIZE/4)*(YSIZE/4) of image which is given in correlation matrix
 * factor gives the oversampling factor
 * The new size of image is returned in centralsize;
 * @param correlationMatrix
 * @param TOTALROWS
 * @param TOTALCOLS
 * @param factor
 * @param centralsize
 * @return 
 */
int keepCentral(float *correlationMatrix,int TOTALROWS,int TOTALCOLS,int factor,
        int *centralsize);
/**
 * Function to swap alternate wuadrants i.e. data of quadrant 0 with 3rd and data of quadrant 1st with second in any matrix. It divides a matrix of size n*n into 4 quadrants of 
 * size n/2 * n/2 and then swap them.
 * @param data
 * @param TOTALROWS
 * @param TOTALCOLS
 * @param factor
 */
void swapComplex(float *data,int TOTALROWS,int TOTALCOLS,int factor);


void generateCorrelationMatrix_2d(float *dph,float *mask,float *correlationMatrix,
        int TOTALROWS,int TOTALCOLS,int factor);

int applySigma(vector<float> &intensity,vector<int> &row,vector<int> &col,float threshold);


#endif 