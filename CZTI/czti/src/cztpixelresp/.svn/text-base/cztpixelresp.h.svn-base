
/* 
 * @file cztpixelresp.h
 * @author Tanul Gupta
 * @date June 8, 2015, 3:37 PM
 * @brief Header file to read, create and perform operations on pixel response file.
 * @details 
 */

#ifndef CZTPIXELRESP_H
#define	CZTPIXELRESP_H

#include <iostream>
#include "glog/logging.h"
#include "pil.h"

#include "caldbHandler.h"
#include "utils.h"

class Cztpixelresp{
private:
    char modulename[NAMESIZE];
    char eboundsFile[PIL_LINESIZE];
    char respParFile[PIL_LINESIZE];
    int clobber;
    int history;
    
public:
    Cztpixelresp();
    int read(int argc, char** argv);
    int read(char *);
    void display();
    
    /**
     * Function to create pixel response file.
     * @return 
     */
    int cztPixelRespProcess();
    
    /**
    * Function to generate history for the module, to be written to the output file
    * @param vhistory
    * @return 
    */    
    int getHistory(vector <string> &vhistory);
    
};




#endif	/* CZTPIXELRESP_H */

