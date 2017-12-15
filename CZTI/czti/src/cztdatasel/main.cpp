/* 
 * File:   main.cpp
 * Author: preeti
 *
 * Created on September 12, 2012, 2:42 PM
 */

#include <cstdlib>

#include"cztdatasel.h"

using namespace std;

/*
 * Wrapper function for CZTDATSEL MODULE
 */
int main(int argc, char** argv) {
        char const* log_file=std::getenv("logfile_env");
    if ( log_file == NULL){
		log_file="cztdatasel.log";
    }    
    google::InitGoogleLogging(log_file);
    google::SetStderrLogging(google::INFO);
    		

    checkPFILESenv();
    checkParFile("cztdatasel");
    
    cztdatasel par;
    par.read(argc,argv);
    par.display();
    LOG(INFO)<<"CZTDATASEL STARTED....................";
    int status=0;
    status=par.cztdataselProcess();
    if(status){
        LOG(ERROR)<<"***Error in cztdatasel_process()***";
        exit(EXIT_FAILURE);
    }
    LOG(INFO)<<"CZTDATASEL COMPLETED SUCCESSFULLY";
    return (EXIT_SUCCESS);
}

