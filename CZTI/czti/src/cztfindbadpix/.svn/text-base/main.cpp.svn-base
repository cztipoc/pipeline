/* 
 * File:   main.cpp
 * Author: preeti
 *
 * Created on September 27, 2012, 4:49 PM
 */

#include <cstdlib>
#include "cztfindbadpix_v2.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
    google::InitGoogleLogging(argv[0]);
    google::SetStderrLogging(google::INFO);
    checkPFILESenv();
    checkParFile("cztfindbadpix");
    Cztfindbadpix par;
    if(par.read(argc,argv)){
        LOG(ERROR)<<"***Error reading parameters***";
        return (EXIT_FAILURE);
    }
    par.display();
    LOG(INFO)<<"CZTFINDBADPIX STARTED....................";
    int status=0;
    status=par.cztfindbadpix_process();
    if(status){
        LOG(ERROR)<<"***Error in cztfindbadpix process***";
        exit(EXIT_FAILURE);
    }
    LOG(INFO)<<"CZTFINDBADPIX COMPLETED SUCCESSFULLY";
    return (EXIT_SUCCESS);
 }

