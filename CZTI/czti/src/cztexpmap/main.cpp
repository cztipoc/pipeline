/* 
 * @file  caldbHandler.h
 * @author Tanul Gupta
 * @date Created on July, 01, 4:05 PM
 * @version 0.2.0
 */

#include "cztexpmap.h"

using namespace std;

int main(int argc, char** argv) {
        char const* log_file=std::getenv("logfile_env");
    if ( log_file == NULL){
		log_file="cztexpmap.log";
    }    
    google::InitGoogleLogging(log_file);
google::SetStderrLogging(google::INFO);
    		

   
    time_t st,et;
    st=time(NULL);
    checkPFILESenv();
    checkParFile("cztexpmap");
    Cztexpmap par;
    par.read(argc,argv);
    par.display();
    LOG(INFO)<<"CZTEXPMAP STARTED....................";
    int status=0;
    status=par.cztexpmap_process();
    if(status){
        LOG(ERROR)<<"***Error in cztexpmap process***";
    }else
        LOG(INFO)<<"CZTEXPMAP COMPLETED SUCCESSFULLY";
    et=time(NULL);
    LOG(INFO)<<"Time Elapsed---"<<et-st<<" seconds"<<endl;
    return 0;
}
