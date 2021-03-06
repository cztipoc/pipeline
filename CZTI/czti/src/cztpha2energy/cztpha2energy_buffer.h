/* 
 * @file cztpha2energyv2.h
 * @author Tanul Gupta
 * @date Created on May 8, 2015, 9:28 AM
 * @brief cztpha2energyv2.h
 * @details This class is designed to convert the PHA value to PI and Energy value
 * for each event in event file.
 */

#ifndef CZTPHA2ENERGYV2_H
#define	CZTPHA2ENERGYV2_H

//Include section
#include <pil.h>
#include <cstdlib>
#include <utils.h>
#include "glog/logging.h"
#include "level2validation.h"
#include <vector>
#include <string>
#include "caldbHandler.h"
#include <math.h>
using namespace std;

class Cztpha2energy{
private:
    char modulename[NAMESIZE];
    char inputEvtFile[PIL_LINESIZE];
    //char eboundsFile[PIL_LINESIZE];
    //char goFile[PIL_LINESIZE];
    char outputEvtFile[PIL_LINESIZE];
    char tempExtFile[PIL_LINESIZE];
    int clobber;
    int history;
    int buffer;
    string eboundsFile;
    string goFile;
    
public:
    Cztpha2energy();
    void display();
    int read(int argc, char **argv);
    int read(char* inputEvtFile, char* outputEvtFile, char* tempExtFile,
           int clobber, int history, int buffer);
    
    int cztpha2energy_process();
    
    /**
     * This function stores history of the operations applied on the input file in the
     * header of the output file
     * @param vhistory: vector to hold history strings
     * @return 0:successful else fail
     */
    int get_history(vector<string> &vhistory);
    
};

#endif	/* CZTPHA2ENERGYV2_H */


