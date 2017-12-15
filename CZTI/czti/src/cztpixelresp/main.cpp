/* 
 * @file main.cpp
 * @author Tanul Gupta
 * @date June 8, 2015, 2:48 PM
 * @brief This main file corresponds to cztpixelresp module
 * @details 
 */

#include <cstdlib>
#include <ctime>
#include "cztpixelresp.h"

using namespace std;

int main(int argc, char** argv) {
    google::InitGoogleLogging(argv[0]);
    google::SetStderrLogging(google::INFO);
    time_t st,et;
    st=time(NULL);
    
    checkPFILESenv();
    checkParFile("cztpixelresp");
    
    Cztpixelresp par;
    
    if(par.read(argc,argv)){
        LOG(ERROR)<<"***Error reading parameters***";
        return (EXIT_FAILURE);
    }
    par.display();
    LOG(INFO)<<"CZTPIXELRESP STARTED....................";
    int status=0;
    status=par.cztpixelrespProcess();
    if(status){
        LOG(ERROR)<<"***Error in cztpixelresp process***";
        exit(EXIT_FAILURE);
    }
    LOG(INFO)<<"CZTPIXELRESP COMPLETED SUCCESSFULLY";
    et=time(NULL);
    LOG(INFO)<<"Time Elapsed:"<<et-st<<" sec";
    return (EXIT_SUCCESS);
 }