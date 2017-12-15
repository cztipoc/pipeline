#include "glog/logging.h"
#include<iostream>
using namespace std;
int main(){
    string log_file;
    log_file="shrikant.log";
    google::InitGoogleLogging(log_file.c_str());
    google::SetStderrLogging(google::INFO);
    LOG(INFO)<<"CZTSCIENCE2EVENT STARTED....................";
    LOG(ERROR)<<"CZTSCIENCE2EVENT ERRORED....................";
}
