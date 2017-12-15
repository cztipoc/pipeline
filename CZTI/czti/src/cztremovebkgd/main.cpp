/* 
 * File:   main.cpp
 * Author: Tanul
 *
 * Created on June 21, 2013, 11:00 AM
 */

#include <cstdlib>
#include <ctime>
#include "cztremovebkgd.h"

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
    checkParFile("cztremovebkgd");
    
    cztremovebkgd par;
    if(par.read(argc,argv)){
        LOG(ERROR)<<"***Error reading parameters***";
        return (EXIT_FAILURE);
    }
    par.display();
    LOG(INFO)<<"CZTREMOVEBKGD STARTED....................";
    int status=0;
    status=par.cztremovebkgdProcess();
    if(status){
        LOG(ERROR)<<"***Error in cztremovebkgd_process***";
        exit(EXIT_FAILURE);
    }
    LOG(INFO)<<"CZTREMOVEBKGD COMPLETED SUCCESSFULLY";
    et=time(NULL);
    LOG(INFO)<<"Time Elapsed:"<<et-st<<" seconds";
    cout<<endl;
    return (EXIT_SUCCESS);
 }

