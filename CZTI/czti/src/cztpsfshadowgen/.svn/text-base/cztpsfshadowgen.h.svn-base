/* 
 * File:   cztpsfshadowgen.h
 * Author: preeti, Tanul Gupta
 *
 * Created on October 15, 2012, 4:33 PM
 * Modified on November 10, 2014
 */

#ifndef CZTPSFSHADOWGEN_H
#define	CZTPSFSHADOWGEN_H

#include <iomanip>
#include "utils.h"
#include "glog/logging.h"
#include "cztstring.h"
#include "coordinateTransformation.h"
#include "caldbHandler.h"
#include "level2validation.h"
#include "Exposure.h"
#include "validations.h"
#include "maskGeometry.h"
#include "detectorGeometry.h"

class cztpsfshadowgen{

private:
    char modulename[NAMESIZE];
    char aspectfileQ0[PIL_LINESIZE];
    char aspectfileQ1[PIL_LINESIZE];
    char aspectfileQ2[PIL_LINESIZE];
    char aspectfileQ3[PIL_LINESIZE];
    char catalogfile[PIL_LINESIZE];
    int extnumCatalog;   //in catalog
    char RA_colname[25];
    char DEC_colname[25];
    char FLUX_colname[25];
    char outfile[PIL_LINESIZE];
    char sourcelist[PIL_LINESIZE];    //ASCII output file containing list of sources
    char cameraGeomFile[PIL_LINESIZE];
    char compMaskfile[PIL_LINESIZE];
    char effectiveAreafile[PIL_LINESIZE];
    float ekev;
    float minEnergy;
    float maxEnergy;
    float fovx; //field of view x in degrees
    float fovy; //field of view y in degrees
    int nBins;
    int includeMask;
    int maskOversampling;
    int history;
    int clobber;
    int debug;
public:
    cztpsfshadowgen();
    int read(int argc,char **argv);
    int read(char *aspectfileQ0, char *aspectfileQ1, char *aspectfileQ2, char *aspectfileQ3,
            char *catalog,int extnumCatalog,char *compmaskfile, char *camerageomfile, 
            char* effareafile, char* sourcelist, char *outfile, float fovx, float fovy, int includeMask,
            int nBins, float ekev=10.0, float minEnergy=10.0, float maxEnergy=100.0, int maskOversampling=10,
            int clobber=YES,int history=YES);
    void display();

    /**
    * Function to create shadow for known sources in FOV of CZT detector
    * @param par
    * @return 
    */
    int cztpsfshadowgenProcess();

    /**
    * Function to generate history for the module, to be written to the output file
    * @param par
    * @param vhistory
    * @return 
    */
    int getHistory(vector<string> &vhistory);
    
};

#endif	/* CZTPSFSHADOWGEN_H */
