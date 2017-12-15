/* 
 * File:   cztgenerateshadow.h
 * Author: Tanul Gupta, Preeti Tahlani
 * Created on August 23, 2015, 9:28 AM
 */

#ifndef CZTGENSHADOW_H
#define	CZTGENSHADOW_H

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
#include "badpixCALDB.h"
#include "cztstring.h"

class cztgenerateshadow {
private:
    char modulename[NAMESIZE];
    char coordtype[NAMESIZE]; //camera/celestial
    double theta1, theta2; //RA,DEC or thetax,thetay //sholud be in degrees
    char cameraGeomFile[PIL_LINESIZE];
    char compMaskfile[PIL_LINESIZE];
    char effectiveAreafile[PIL_LINESIZE];
    char aspectfileQ0[PIL_LINESIZE];
    char aspectfileQ1[PIL_LINESIZE];
    char aspectfileQ2[PIL_LINESIZE];
    char aspectfileQ3[PIL_LINESIZE];
    float ekev;
    float minEnergy;
    float maxEnergy;
    int nBins;
    int includeMask;
    int maskOversampling;
    float sourceflux; //in counts/area
    char outfile[PIL_LINESIZE]; //output shadow file
    int clobber;
    int history;
    int debug;

    

public:
    cztgenerateshadow();

    int read(int argc, char **argv);
    int read(char *coordtype, double theta1, double theta2, char *aspectfileQ0,char *aspectfileQ1,
            char *aspectfileQ2,char *aspectfileQ3, char *compMaskfile, char *cameraGeomfile,
            char *effectiveAreafile, float ekev, float minEnergy, float maxEnergy, int nBins,
            int includeMask, int maskOversampling,float sourceflux, char *outfile, 
            int clobber = YES, int history = YES, int debug=NO);
    void display();

    int cztgenerateshadowProcess();
    /**
     * Function to generate history for the module, to be written to the output file
     * @param par
     * @param vhistory
     * @return 
     */
    int get_history(vector<string> &vhistory);
};



#endif	/* CZTGENSHADOW_H */

