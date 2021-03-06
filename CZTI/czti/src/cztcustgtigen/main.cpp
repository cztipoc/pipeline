/* 
 * File:   main.cpp
 * Author: Tanul, preeti
 *
 * Created on September 11, 2012, 10:18 AM
 * Modified on December 15, 2014
 */

#include <cstdlib>
#include <ctime>
#include "cztgtigenv2.h"
#include "glog/logging.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
    google::InitGoogleLogging(argv[0]);
    google::SetStderrLogging(google::INFO);
    time_t st, et;
    st = time(NULL);
    checkPFILESenv();
    checkParFile("cztgtigen");
    
    cztgtigen par;
    if(par.read(argc,argv)){
        LOG(ERROR)<<"***Error reading parameters***";
        return (EXIT_FAILURE);
    }
    
    par.display();
    LOG(INFO)<<"CZTGTIGEN STARTED....................";
    int status=0;
    status=par.cztgtigenProcess();
    if(status){
        LOG(ERROR)<<"***Error in cztgtigen_process***";
        exit(EXIT_FAILURE);
    }
   LOG(INFO)<<"CZTGTIGEN COMPLETED SUCCESSFULLY";
   et=time(NULL);
   LOG(INFO)<<"Time Elapsed:"<< et-st << " seconds";
   return (EXIT_SUCCESS);
 }

