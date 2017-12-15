/* 
 * File:   main.cpp
 * Author: preeti
 *
 * Created on September 27, 2012, 3:42 PM
 */

#include <cstdlib>
#include "cztfilterbadpix.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
    google::InitGoogleLogging(argv[0]);
    google::SetStderrLogging(google::INFO);
    
    checkPFILESenv();
    checkParFile("cztfilterbadpix");
    
    cztfilterbadpix par;
    if(par.read(argc,argv)){
       LOG(ERROR)<<"***Error reading input parameters***";
       return (EXIT_FAILURE);        
    }
    par.display();
    LOG(INFO)<<"CZTFILTERBADPIX STARTED....................";
    int status=0;
    status=par.cztfilterbadpixProcess();
    if(status){
        LOG(ERROR)<<"***Error in cztfilterbadpix process***";
        exit(EXIT_FAILURE);
    }
    LOG(INFO)<<"CZTFILTERBADPIX COMPLETED SUCCESSFULLY";
    return (EXIT_SUCCESS);
 }