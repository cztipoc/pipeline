/* 
 * File:   main.cpp
 * Author: Tanul Gupta
 * Created on October 16, 2015
 */

/*
 * Ajay Vibhute, Dec 17, 2015
 * Added RollRot column in output mkf file
 * 
 */
#include <cstdlib>
#include <ctime>
#include "cztmkfgen.h"
#include "glog/logging.h"

using namespace std;

int main(int argc, char** argv) {
    
        char const* log_file=std::getenv("logfile_env");
    if ( log_file == NULL){
		log_file="cztmkfgen.log";
    }    
    google::InitGoogleLogging(log_file);
google::SetStderrLogging(google::INFO);
    		

    time_t st, et;
    st = time(NULL);
    checkPFILESenv();
    checkParFile("cztmkfgen");
    
    cztmkfgen par;
    if(par.read(argc,argv)){
        LOG(ERROR)<<"***Error reading parameters***";
        return (EXIT_FAILURE);
    }
    
    par.display();
    LOG(INFO)<<"CZTMKFGEN STARTED....................";
    int status=0;
    status=par.cztmkfgenProcess();
    if(status){
        LOG(ERROR)<<"***Error in cztmkfgenProcess***";
        exit(EXIT_FAILURE);
    }
   LOG(INFO)<<"CZTMKFGEN COMPLETED SUCCESSFULLY";
   et=time(NULL);
   LOG(INFO)<<"Time Elapsed:"<< et-st << " seconds";
   return (EXIT_SUCCESS);
   
 }

