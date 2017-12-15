/* 
 * File:   cztpha2energy.h
 * Author: preeti
 *
 * Created on August 28, 2012, 5:58 PM
 */

#ifndef CZTPHA2ENERGY_H
#define	CZTPHA2ENERGY_H


#include<pil.h>
#include<iostream>
#include<cstdlib>
#include<cstring>
#include<string>
#include<utils.h>
#include<iomanip>
#include "glog/logging.h"
#include"cztstring.h"
using namespace std;

class cztpha2energy{
private: 
    char modulename[100];
    char InputEvtFile[PIL_LINESIZE];
    char GainFile[PIL_LINESIZE];
    char OffsetFile[PIL_LINESIZE];
//    char RMF[PIL_LINESIZE];                   //Response matrix file
    char OutputEvtFile[PIL_LINESIZE];
    int clobber;
    int history;
    
 public:  
    cztpha2energy();
    void display();
    int read(int argc, char **argv);
    int read(char *InputEvtFile, char *GainFile, char *OutFile, char *outfile,int clobber, int history);
    
    int cztpha2energyProcess();
    /**
    * Creates history for the module o be written to the output file
    * @param par : Object of cztpha2energy_param class
    * @param vhistory : vector ho hold history strings
    * @return 
    */
    int getHistory(vector<string> &vhistory);
};



/**
 * Function to read gain map, offset map and quantum efficiency(QE) map from czti
 * into corresponding buffers  
 * @param file          : Name of gain offset QE file
 * @param gain          : 2D Buffer to hold gain values
 * @param gain_name     : Extension name of gain map
 * @param offset        : 2D buffer to hold offset values
 * @param offset_name   : Name of extension containing offset values
 * @param QE            : 2D buffer to hold quantum efficiency values 
 * @param QE_name       : Name of extension containing QE map
 * @param x             : Size of x axis/number of columns in 2D buffer
 * @param y             : Size of y axis/number of rows in 2D buffer
 */
int readGainOffsetFile(char *file,float gain[][XSIZE],char *gain_name,
                float offset[][XSIZE],char *offset_name,float QE[][XSIZE],
                char *QE_name,int x,int y);


/**
 * Function to read EBOUNDS extension from Response Matrix File
 * @param file     : Name of response matrix file
 * @param num_chan : Number of energy channels 
 * @param Ebounds  : Array having 'num_chan' rows and 3 columns to store E_MIN, E_MAX and Mid _Energy Value
 * @param ebounds_name :name of ebounds extension 
 */
int readEbounds(char *file,int num_chan,float Ebounds[][3],char *ebounds_name);

/**
 * Function to convert pha to pi and energy for one quadrant using gain, offset and ebounds
 * @param quadrantno  : Quadrant number whose data is passed
 * @param pha : pha values for corresponding quadrant
 * @param detx : x location within the quadrant
 * @param dety : y location within quadrant
 * @param nrows : number of values of pha
 * @param gain : Array for gain for all quadrants (size YSIZExXSIZE)
 * @param offset :  Array for offset for all quadrants (size YSIZExXSIZE)
 * @param ebounds :  Array for energy channels for all quadrants (size num_of_channels x 3)
 * @param pi : output pi for pha
 * @param energy : output energy for pha
 * @return 
 * 
 * Note: for Q0, gain/offset indexes start at (0,0)
 *       for Q1, gain/offset indexes start at (0,XSIZE/2)
 *       for Q2, gain/offset indexes start at (YSIZE/2,XSIZE/2)
 *       for Q3, gain/offset indexes start at (YSIZE/2,0)
 */
int pha2energy(int quadrantno,unsigned short *pha,unsigned char *detx,unsigned char *dety,
        long nrows,float gain[YSIZE][XSIZE],float offset[YSIZE][XSIZE], float ebounds[NUM_CHANNEL][3],
        unsigned short *pi,float *energy);



#endif	/* CZTPHA2ENERGY_H */

