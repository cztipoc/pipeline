/* 
 * File:   main.cpp
 * Author: Tanul, Preeti
 * Created on October 23, 2012, 9:27 AM
 * Modified on November 7, 2014
 */

#include <cstdlib>
#include <ctime>
#include "cztgenerateshadow.h"
#include"glog/logging.h"

using namespace std;

int main(int argc, char** argv) {
        char const* log_file=std::getenv("logfile_env");
    if ( log_file == NULL){
		log_file="cztgenshadow.log";
    }    
    google::InitGoogleLogging(log_file);
google::SetStderrLogging(google::INFO);
    		

    time_t st,et;
    st=time(NULL);
    

    cztgenerateshadow par1;
    checkPFILESenv();
    checkParFile("cztgenshadow");
    

    if(par1.read(argc,argv)){
        LOG(ERROR)<<"***Error reading parameters***";
        return (EXIT_FAILURE);
    }
    par1.display();
    LOG(INFO)<<"CZTGENSHADOW STARTED....................";
    int status=0;
    status=par1.cztgenerateshadowProcess();
    if(status){
        LOG(ERROR)<<"***Error in cztgenshadow process***";
        exit(EXIT_FAILURE);
    }
    LOG(INFO)<<"CZTGENSHADOW COMPLETED SUCCESSFULLY";
    et=time(NULL);
    LOG(INFO)<<"Time Elapsed:"<<et-st<<" sec";
    return (EXIT_SUCCESS);
}

    
    
