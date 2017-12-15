/* 
 * File:   cztimage.h
 * Author: preeti, Tanul
 *
 * Created on October 16, 2012, 12:15 PM
 * Modified on November 5, 2014
 */

#ifndef CZTIMAGE_H
#define	CZTIMAGE_H

#include<pil.h>
#include "linalg.h"
#include<ap.h>
#include"utils.h"
#include"ExpMap.h"
#include "validations.h"
#include "glog/logging.h"
#include "cztstring.h"
#include "jpeg_handling.h"
#include "caldbHandler.h"
#include "coordinateTransformation.h"




#define SWAP(a,b) tempr=(a);(a)=(b);(b)=tempr
#define FFT 0
#define IFFT 1

#define NUMQUAD_ROW 2
#define NUMQUAD_COL 2

#define PEAKLIMIT 20

#define DPI 1
#define MASK 0

using namespace std;
using namespace alglib;

class cztimage{
private:
    //Input parameters
    char modulename[NAMESIZE];
    char dpifile[PIL_LINESIZE];   //Input DPI file for image creation
    char maskfile64x64[PIL_LINESIZE];
    char maskfile8100[PIL_LINESIZE];
    char shadowfile[PIL_LINESIZE];
    char aspectfileQ0[PIL_LINESIZE];
    char aspectfileQ1[PIL_LINESIZE];
    char aspectfileQ2[PIL_LINESIZE];
    char aspectfileQ3[PIL_LINESIZE];
    char catalogfile[PIL_LINESIZE];
    int extnum;
    char RA_colname[25];
    char DEC_colname[25];
    char FLUX_colname[25];
    float resolutionLimit;    
    float threshold;
    char sourcelist[PIL_LINESIZE];
    char outImgFile[PIL_LINESIZE];
    char quadsToProcess[25];
    char nobkgdfile[PIL_LINESIZE];
    int oversamplingfactor;
    float sourceStrengthThr;
    int nBkgdCoef;   //number of background coefficients
    int debugmode;             //if yes/true, creates intermediate file 
    int clobber;
    int history;
    
    //full caldb file paths
    string maskfile64x64path;
    string catalogFilePath;
    string maskfile8100path;
    
    //data parameters
    float *dpi,*mask,*rawskyimage,*oversampledmask,*oversampleddpi, *nobkgdskyimage;;
    int nsources, knownSources;                 //number of sources/peaks found, number of knownSources in the field of view
    int size;            //size after oversampling
    int skyimgsize;    //size for skyimage
    vector<double> thetax,thetay,flux,ra,dec; //ra,dec,thetax,thetay: radians; flux: image value at that point.
    vector<double> RA_ks, DEC_ks, FLUX_ks, thetax_ks, thetay_ks; //RA, DEC, flux and camera coordinates thetax & thetay for known sources (ks)
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
    
    /**
     * Identify peaks in the image and stores following information about them.
     * Peak is identified based on a window size which is 9 if oversampling factor 
     * is <8. If it is greather than 8, then window size is taken as 3*oversampling
     * factor.
     * After identfying peaks, the function provide following vectors containing the
     * eponymous values.
     * @param RA - in radians
     * @param DEC - in radians
     * @param FLUX - DN value of cross correlated image.
     * @param thetax - in radians
     * @param thetay - in radians
     */
    
    int identifyPeaks();
    
    /**
     * Function to display RA, DEC, flux, thetax and thetay of identified peaks
     */
    int displayPeaks();
    
    /**
     * Function to display RA, DEC, flux, thetax and thetay of known sources
     * 
     */
    int displayKnownSources();
    
    
    /**
    * Function to create an output file named 'outImgFile' and write the skyimage to it
    * It also copies keywords from dpifile to ouput file 
    * @param dpifile
    * @param outImgFile
    * @param rawimg
    * @param size
    * @param oversamplefactor
    * @return 
    */
    
    int writeSkyImage();
    
    
    /**
     * Function to write synthetic image, a jpeg image of the identified peaks 
     * (after SVD fitting) in the field of view.
     * @param: thetax 
     * @param: thetay
     * @param: flux
     * @param: skyimgsize 
     */
    int writeSyntheticImage();
    
    
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
    cztimage();
    ~cztimage();
    int read(int argc,char **argv);
    int read(char *dpifile,char *maskfile64x64,char *maskfile8100,char *shadowfile,
    char *aspectfile, char* catalogfile, int extnum, char* RA_colname, char* DEC_colname,
        char* FLUX_colname, float resolution_limit, float threshold, int oversamplingfactor,
        char *sourcelist,float source_strength_thr,char *outImgFile, char* quad_no, int nBkgdCoef,int debugmode=NO,
        int clobber=YES, int history=YES);
    void display();
  
    /**
    * Function to create image from DPI
    * @param par
    * @return 
    */
    int cztimageProcess();
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

/**
 * Function to remove contribution of known sources from DPI
 * @param knownSourcesShadowfile
 * @param dpi
 * @param rows
 * @param XSIZE
 * @return 
 */
//int removeKnownSources(char *knownSourcesShadowfile,float *dpi,int rows,int XSIZE);

//svd related
/**
 * To multiply two matrices A and B with sizes mxn and nxp to give output matrix C with size mxp
 * @param A
 * @param B
 * @param C
 * @param m
 * @param n
 * @param p
 * @return 
 */
int matrixproduct(real_2d_array &A,real_2d_array &B,real_2d_array &C,
        ae_int_t m,ae_int_t n,ae_int_t p);

/**
 * Multiplies inverse of A with B to get the result in X array  
 * @param Aarr
 * @param Barr
 * @param m
 * @param n
 * @param p
 * @param X
 * @return 
 */
int multiply_AinvB(float *Aarr,float *Barr,int m,int n,int p,float *X);


#endif	/* CZTIMAGE_H */

