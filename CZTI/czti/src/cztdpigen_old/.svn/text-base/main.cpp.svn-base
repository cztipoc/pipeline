/* 
 * File:   newmain.cpp
 * Author: preeti
 *
 * Created on September 18, 2012, 6:05 PM
 */

#include <cstdlib>
#include "cztdpigen.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
    google::InitGoogleLogging(argv[0]);
    google::SetStderrLogging(google::INFO);
    
    checkPFILESenv();
    checkParFile("cztdpigen");
    
    cztdpigen par;
    par.read(argc,argv);
    par.display();
    LOG(INFO)<<"CZTDPIGEN STARTED....................";
    int status=0;
    status=par.cztdpigenProcess();
    if(status){
        LOG(ERROR)<<"***Error in cztdpigen process***";
        exit(EXIT_FAILURE);
    }
    LOG(INFO)<<"CZTDPIGEN COMPLETED SUCCESSFULLY";
    return (EXIT_SUCCESS);
 }

