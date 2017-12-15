/* 
 * File:   main.cpp
 * Author: tanul
 *
 * Created on July 5, 2012, 10:19 AM
 */

#include <cstdlib>
#include <ctime>
#include "cztfindsources.h"


using namespace std;

int main(int argc, char** argv) {
    google::InitGoogleLogging(argv[0]);
    google::SetStderrLogging(google::INFO);
    time_t st,et;
    st=time(NULL);
    
    checkPFILESenv();
    checkParFile("cztfindsources");
    
    cztfindsources par;
    if(par.read(argc,argv)){
        LOG(ERROR)<<"***Error reading parameters***";
        return (EXIT_FAILURE);
    }
    par.display();
    LOG(INFO)<<"CZTFINDSOURCES STARTED....................";
    int status=0;
    status=par.cztfindsourcesProcess();
    if(status){
        LOG(ERROR)<<"***Error in cztfindsources_process***";
        exit(EXIT_FAILURE);
    }
    LOG(INFO)<<"CZTFINDSOURCES COMPLETED SUCCESSFULLY";
    et=time(NULL);
    LOG(INFO)<<"Time Elapsed:"<<et-st<<" seconds";
    cout<<endl;
    return (EXIT_SUCCESS);
 }