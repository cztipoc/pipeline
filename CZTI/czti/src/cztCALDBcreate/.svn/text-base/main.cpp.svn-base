
/* 
 * @file main.cpp
 * @author Tanul Gupta
 * @date Created on June 25, 09:58 AM
 * @brief Creates CALDB files
 * @details This module creates CALDB file 
 */

#include <cstdlib>
#include "cztCALDBcreate.h"

using namespace std;

int main(int argc, char** argv) {
    google::InitGoogleLogging(argv[0]);
    google::SetStderrLogging(google::INFO);
    int status=0;
    
    checkPFILESenv();
    checkParFile("cztCALDBcreate");
    
    CztCALDBcreate par;
    par.read(argc,argv);
    par.display();
    LOG(INFO)<<"cztCALDBcreate started....................";
    status=par.cztCALDBcreate_process();
    if(status){
        LOG(ERROR)<<"***Error in cztCALDBcreate process***";
        exit(EXIT_FAILURE);
    }
    LOG(INFO)<<"cztCALDBcreate completed successfully.";
    return (EXIT_SUCCESS);
 }
