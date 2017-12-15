/* 
 * @file  cztexpmap.h
 * @author Tanul Gupta
 * @date Created on June 26, 2015, 2:53 PM
 * @brief This class takes user input and creates exposure map based on thetax, thetay & energy of photon
 * @details This class takes user input and creates exposure map based on thetax, thetay & energy of photon
 * @version 1.0
 */

#ifndef CZTEXPMAP_H
#define CZTEXPMAP_H

#include "glog/logging.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <fitsio.h>
#include <pil.h>
#include "detectorGeometry.h"
#include "caldbHandler.h"
#include "utils.h"
#include "level2validation.h"
#include "validations.h"
#include "maskGeometry.h"
#include "Exposure.h"


using namespace std;

class TransMask;

class Cztexpmap{
private:
    char modulename[NAMESIZE];
    char cameraGeomFile[PIL_LINESIZE];
    char compressedMaskFile[PIL_LINESIZE];
    float ekev;
    float thetax;
    float thetay;
    float minEnergy;
    float maxEnergy;
    int nBins;
    char outExposureFile[PIL_LINESIZE];
    int includeMask;
    int maskOversampling;
    ExposureTable expTab;
    int history;
    int clobber;
    int debug;
public:
    Cztexpmap();
    void display();
    int read(int argc, char** argv);
    int read(char *cameraGeomFile, char* compressedMaskFile, char* outExposureFile, 
            float thetax, float thetay, int includeMask, int maskOversampling, int nBins, float ekev,
            float minEnergy=10.0, float maxEnergy=100.0, 
            int clobber=YES,int history=YES, int debug=NO);
    /**
     * To generate exposure map file based on user input.
     * @return 
     */
    int cztexpmap_process();

    /**
     * Function to generate history for the module to be written to the output file
     * @param vhistory
     * @return 
     */
    int get_history(vector<string> &vhistory);
};

#endif /* CZTEXPMAP_H */