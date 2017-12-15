/* 
 * File:   main.cpp
 * Author: preeti
 * Module: cztpipeline
 * Wrapper main function for CZT pipeline.This will run with default parameters provided 
 * Created on July 21, 2012, 3:27 PM
 */

#include <cstdlib>
#include <iostream>
#include <cztpipeline.h>

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
    google::InitGoogleLogging(argv[0]);
    google::SetStderrLogging(google::INFO);
    time_t st,et;
    st=time(NULL);
    checkPFILESenv();
    checkParFile("cztpipeline");
    int errflag=0;
    cztpipeline par;	
    errflag=par.read(argc,argv);
    if(errflag){
        LOG(ERROR)<<"***Error in reading parameters***";
        exit (EXIT_FAILURE);
    }
    par.display();
    LOG(INFO) << "-----------------Running \"CZTI DATA PROCESSING PIPELINE\"-------------------";
    if (par.cztpipelineProcess()) {
        LOG(ERROR) << endl << "***Error in CZTPIPELINE***";
        et = time(NULL);
        LOG(INFO) << "Time Elapsed---" << et - st << " seconds" << endl;
        return (EXIT_FAILURE);

    } 
    else
        LOG(INFO) << "----------\"CZT DATA PROCESSING PIPELINE\" Completed Successfully -----------";
    et = time(NULL);
    LOG(INFO) << "Time Elapsed---" << et - st << " seconds" << endl;
    return (EXIT_SUCCESS);
}
    
