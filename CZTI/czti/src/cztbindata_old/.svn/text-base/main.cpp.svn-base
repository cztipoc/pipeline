/* 
 * File:   main.cpp
 * Author: preeti
 *
 * Created on October 15, 2012, 6:11 PM
 */

#include <cstdlib>
#include <ctime>
#include "cztbindata.h"


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
    checkParFile("cztbindata");
    
    cztbindata par;
    if(par.read(argc,argv)){
        LOG(ERROR)<<"***Error reading parameters***";
        return (EXIT_FAILURE);
    }
    par.display();
    LOG(INFO)<<"CZTBINDATA STARTED....................";
    int status=0;
    status=par.cztbindataProcess();
    if(status){
        LOG(ERROR)<<"***Error in cztbindata_process***";
        exit(EXIT_FAILURE);
    }
    LOG(INFO)<<"CZTBINDATA COMPLETED SUCCESSFULLY";
    et=time(NULL);
    LOG(INFO)<<"Time Elapsed:"<<et-st<<" seconds";
    return (EXIT_SUCCESS);
 }

