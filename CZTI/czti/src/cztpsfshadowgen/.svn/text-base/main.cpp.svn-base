/* 
 * File:   main.cpp
 * Author: preeti, Tanul
 *
 * Created on October 15, 2012, 4:29 PM
 * Modified on November 10, 2014
 */

#include <cstdlib>
#include <ctime>
#include "cztpsfshadowgen.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
        char const* log_file=std::getenv("logfile_env");
    if ( log_file == NULL){
		log_file="cztpsfshadowgen.log";
    }    
    google::InitGoogleLogging(log_file);
google::SetStderrLogging(google::INFO);
    		

    time_t st,et;
    st=time(NULL);
    
    checkPFILESenv();
    checkParFile("cztpsfshadowgen");
    
    cztpsfshadowgen par;
    
    if(par.read(argc,argv)){
        LOG(ERROR)<<"***Error reading parameters***";
        return (EXIT_FAILURE);
    }
    par.display();
    LOG(INFO)<<"CZTPSFSHADOWGEN STARTED....................";
    int status=0;
    status=par.cztpsfshadowgenProcess();
    if(status){
        LOG(ERROR)<<"***Error in cztpsfshadowgen process***";
        exit(EXIT_FAILURE);
    }
    LOG(INFO)<<"CZTPSFSHADOWGEN COMPLETED SUCCESSFULLY";
    et=time(NULL);
    LOG(INFO)<<"Time Elapsed:"<<et-st<<" sec";
    return (EXIT_SUCCESS);
 }

