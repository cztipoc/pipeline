/* 
 * File:   main.cpp
 * Author: Tanul, preeti
 *
 * Created on October 16, 2012, 12:13 PM
 * Modified on October 20, 2014
 */

#include <cstdlib>
#include <ctime>
#include "cztimage_v2.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
        char const* log_file=std::getenv("logfile_env");
    if ( log_file == NULL){
		log_file="cztimage.log";
    }    
    google::InitGoogleLogging(log_file);
google::SetStderrLogging(google::INFO);
    		
    
    time_t st,et;
    st=time(NULL);
    
    checkPFILESenv();
    checkParFile("cztimage");
    
    Cztimage par;
    if(par.read(argc,argv)){
        LOG(ERROR)<<"***Error reading parameters***";
        return (EXIT_FAILURE);
    }
    par.display();
    LOG(INFO)<<"CZTIMAGE STARTED....................";
    int status=0;
    status=par.cztimageProcess();
    if(status){
        LOG(ERROR)<<"***Error in cztimage_process***";
        exit(EXIT_FAILURE);
    }
    LOG(INFO)<<"CZTIMAGE COMPLETED SUCCESSFULLY";
    et=time(NULL);
    LOG(INFO)<<"Time Elapsed:"<<et-st<<" seconds";
    cout<<endl;
    return (EXIT_SUCCESS);
 }
