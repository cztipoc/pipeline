/* 
 * @file  cztmkfgen.h
 * @author Tanul Gupta
 * @date Created on October 16, 2015, 2:51 PM
 * @brief Regenerates mkf file for level-2 analysis
 * @details
 * @version 2.0
 */
/*
 *Ajay Vibhute, Dec 18 2015
 * Modified code to Read HK paramters, cztcounter, vetocounter from hdr file
 * */
#ifndef CZTMKFGEN_H
#define CZTMKFGEN_H

#include <iostream>
#include "glog/logging.h"
#include "utils.h"
#include "level1handler.h"
#include "macrodef.h"
#include "Mvector.h"
#include "mkfRegeneration.h"

using namespace std;

class cztmkfgen{
private:
    char modulename[NAMESIZE];
    char outmkffilename[PIL_LINESIZE];
    char inattfilename[PIL_LINESIZE];
    char inlbtfilename[PIL_LINESIZE];
    char inorbfilename[PIL_LINESIZE];
    char inhdrfilename[PIL_LINESIZE];
    int clobber;
    int history;
public:
    cztmkfgen();
    void display();
    int read(int argc, char** argv);
    int read(char* inattfilename, char* inlbtfilename, char* inorbfilename, char*inhdrfilename, char* outmkffilename, int clobber=NO,
            int history=NO);
    int cztmkfgenProcess();
    void getHistory(vector<string> &vhistory);
};

#endif /*CZTMKFGEN_H*/
