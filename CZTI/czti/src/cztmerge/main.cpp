/* main.cpp cztmergescience


*/

#include <ctime>
#include "cztmergescience.h"

using namespace std;

int main(int argc, char** argv) 
{
        char const* log_file=std::getenv("logfile_env");
    if ( log_file == NULL){
		log_file="cztmergescience.log";
    }    
    google::InitGoogleLogging(log_file);
google::SetStderrLogging(google::INFO);
    		
    time_t st,et;

    checkPFILESenv();
    checkParFile("cztmergescience");

    cztmergescience par;

    if(par.read(argc,argv)){
        LOG(ERROR)<<"***Error reading parameters***";
        return (EXIT_FAILURE);
    }

    LOG(INFO)<<"CZTMERGESCIENCE STARTED....................";
    st=time(NULL);
			
    int status=0;
    status=par.cztmergescienceProcess();

    if(status){
        LOG(ERROR)<<"***Error in cztmergescience process***";
        exit(EXIT_FAILURE);
    }

    LOG(INFO)<<"CZTMERGESCIENCE COMPLETED SUCCESSFULLY";
    et=time(NULL);
    LOG(INFO)<<"Time Elapsed:"<<et-st<<" seconds";
    return (EXIT_SUCCESS);

}
