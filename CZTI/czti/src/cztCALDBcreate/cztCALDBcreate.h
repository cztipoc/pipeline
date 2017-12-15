/* 
 * @file cztCALDBcreate.h
 * @author Tanul Gupta
 * @date Created on June 25, 2015, 10:00 AM
 * @brief This file creates CALDB files based on user input
 * @details This file creates CALDB files based on user input
 */

#ifndef CZTCALDBCREATE_H
#define	CZTCALDBCREATE_H

#include <pil.h>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include "glog/logging.h"
#include "utils.h"
#include "fitsio.h"
#include "validations.h"
#include "caldbHandler.h"


using namespace std;


class CztCALDBcreate {
private:
    char modulename[NAMESIZE];
    char CALDBfile[NAMESIZE];
    int clobber;
    int history;
    
public:
    CztCALDBcreate();
    int read(int argc, char** argv);
    int read(char* CALDBfile, int clobber=NO, int history=NO);
    void display();
    int cztCALDBcreate_process();
    int get_history(vector<string> &vhistory);    
};
#endif	/* CZTCALDBCREATE_H */

