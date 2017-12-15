#include "cztCALDBcreate.h"

CztCALDBcreate::CztCALDBcreate() {
    strcpy(modulename, "cztCALDBcreate_v");
    strcat(modulename, VERSION);
}

int CztCALDBcreate::read(int argc, char** argv) {
    int status = 0;

    if (PIL_OK != (status = PILInit(argc, argv))) {
        LOG(ERROR) << "***Error Initializing PIL***";
        return status;
    }

    if (PIL_OK != (status = PILGetString("par_CALDBfile", CALDBfile))) {
        LOG(ERROR) << "***Error Reading CALDB file type:" << CALDBfile << "***";
        return status;
    }
    if (PIL_OK != (status = PILGetBool("par_history", &history))) {
        LOG(ERROR) << "***Error Reading history parameter***";
        return status;
    }

    if (PIL_OK != (status = PILGetBool("par_clobber", &clobber))) {
        LOG(ERROR) << "***Error Reading clobber***";
        return status;
    }

    PILClose(status);
    return (EXIT_SUCCESS);    
}

int CztCALDBcreate::read(char* CALDBfile, int clobber, int history){
    strcpy(this->CALDBfile, CALDBfile);
    this->clobber=clobber;
    this->history=history;
    
    return (EXIT_SUCCESS);
}

void CztCALDBcreate::display() {
    LOG(INFO) << "----------------------------------------------------------------------------";
    LOG(INFO) << "                          CZTCALDBCREATE PARAMETERS                            ";
    LOG(INFO) << "----------------------------------------------------------------------------";
    LOG(INFO) << "Output CALDB File Type                     :" << CALDBfile;
    LOG(INFO) << "Clobber                                    :" << clobber;
    LOG(INFO) << "History                                    :" << history;
    LOG(INFO) << "----------------------------------------------------------------------------";
}

int CztCALDBcreate::cztCALDBcreate_process(){
    int status=0; //status variable
    int i,j=0;    //counter variables
    string outFilename="";
    string content="";
    string creationTime="";
    CztiCameraGeometry cztcam;

    //Construction output file CALDB file name 
    //getting current time
    time_t now = time(0);
    char utcTime[100];
    tm* gmtm = gmtime(&now);
    sprintf(utcTime, "%d%02d%02d", gmtm->tm_year+1900, gmtm->tm_mon+1, gmtm->tm_mday);
    creationTime = string(utcTime);
    
    //transforming content type into lower case
    content = string(CALDBfile);
    transform(content.begin(), content.end(), content.begin(), ::tolower);

    outFilename= caldb_filename_generator(content, CALDBVERSION, creationTime);
    LOG(INFO) << "Output CALDB file name: " << outFilename;
    //output CALDB file name generated

    if (FileExists((char*) outFilename.c_str())) {
        if (clobber == YES) {
            if (unlink((char*) outFilename.c_str()) != 0) {
                LOG(ERROR) << "Error in deleting Output DPI File: " << outFilename;
            }
        } else {
            LOG(INFO) << outFilename << " already exists.";
            LOG(INFO) << "Use clobber=yes for overwriting the file.";
            return (EXIT_FAILURE);
        }
    }
    
    if(content=="camerageo"){
        
        DLOG(INFO) << "Creating camera geometry CALDB file.";
        if(cztcam.define_camera_geometry()){
            LOG(ERROR)<< "ERROR in calculating CZTI camera surface definition.";
            return (EXIT_FAILURE);
        }
        if(cztcam.write_camera_geometry_file(outFilename, "cztiGeometryTemplate")){
            LOG(ERROR)<< "ERROR in creating file: " << outFilename;
            return (EXIT_FAILURE);
        }
        

        if (history == YES) {
            vector<string> vhistory;
            get_history(vhistory);
            writeHistory((char*)outFilename.c_str(), vhistory);
        }
        
        //Updating CALDB keywords and writing checksum
        if(update_CALDB_keywords((char*) outFilename.c_str(), modulename)){
            LOG(ERROR)<< "Error in updating Keywords in CALDB file: " << outFilename;
            return EXIT_FAILURE;
        }
    }
    
    return EXIT_SUCCESS;
}

int CztCALDBcreate::get_history(vector<string>& vhistory){
    //char* user=getlogin();
    char *user = getenv("USER");
	string str="Module run by " + (string)user;
    vhistory.push_back("COMMENT -------------------------------------");
    vhistory.push_back("COMMENT    HISTORY");
    vhistory.push_back("COMMENT -------------------------------------");    
    vhistory.push_back(str);
    vhistory.push_back("Parameter List start for " + (string)modulename);
    vhistory.push_back("P1 CALDBfiletype = " + (string) CALDBfile);
    if (clobber == YES)
        vhistory.push_back("P2 clobber=yes");
    else
        vhistory.push_back("P2 clobber=no");
    if (history == YES)
        vhistory.push_back("P3 history=yes");
    else
        vhistory.push_back("P3 history=no");
    vhistory.push_back("Parameter List END");
    return (EXIT_SUCCESS);    
}
