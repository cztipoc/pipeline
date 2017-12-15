/* 
 * File:   cztgaas.h
 * Author: Tanul Gupta, Preeti Tahlani
 *
 * Created on September 4, 2012, 3:50 PM
 * Modified on March 1, 2015
 */


#ifndef CZTGAAS_H
#define	CZTGAAS_H

#include<pil.h>
#include<iostream>
#include<cstring>
#include<vector>
#include<cmath>

#include"utils.h"
#include "glog/logging.h"
#include "cztstring.h"

using namespace std;

class cztgaas{
private:
    char modulename[NAMESIZE];
    char eventfile[PIL_LINESIZE];
    char teldeffile[PIL_LINESIZE];
    char attitudefile[PIL_LINESIZE];
    char outfile[PIL_LINESIZE];
    int clobber;
    int history;
public:    
    cztgaas();
    int read(int argc,char **argv);
    int read(char *eventfile,char *teldeffile,char *attitudefile,char *outfile,
        int clobber=NO,int history=NO);
    void display();
    
    int cztgaasProcess();
    
    /**
    * Creates history for the module to be written to the output files
    * @param par : Object of cztgaas_param class
    * @param vhistory : vector ho hold history strings
    * @return 
    */
    int getHistory(vector<string> &vhistory);

};



struct Att{
    double t,q1,q2,q3,q4;
    void display();
};

/**
 * Function to read CZTI alignment Qs from Teldef file. Qs are either provided directly 
 * in teldef file otherwise EULER angles will be given 
 * @param teldef
 * @param q1
 * @param q2
 * @param q3
 * @param q4
 * @return 
 */
int getCZTIq(char *teldef,double *q1,double *q2, double *q3, double *q4);

/**
 * This function reads attitude quaternions corresponding to events from the 
 * attitude file at required intervals 
 * @param attfile : Path of attitude file
 * @param att : vector that will contain the attitude
 * @param startrow : Row from where attitude has to be read
 * @param endrow : Row upto which attitude has to be read
 * @param interval : Time interval in seconds for required attitude
 * @return 
 */

int getATTq(char *evtfile, char *attfile,vector<Att> &att,float interval);    //interval in 'seconds'



#endif	/* CZTGAAS_H */


