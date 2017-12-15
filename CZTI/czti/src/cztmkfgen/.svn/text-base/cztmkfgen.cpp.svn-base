#include "cztmkfgen.h"

cztmkfgen::cztmkfgen() {
    strcpy(modulename, "cztmkfgen_v");
    strcat(modulename, VERSION);
    strcpy(inattfilename,"");
    strcpy(inlbtfilename,"");
    strcpy(inorbfilename, "");
    strcpy(inhdrfilename, "");
    strcpy(outmkffilename, "");
}

int cztmkfgen::read(int argc, char** argv){
    int status=0;
    if (PIL_OK != (status = PILInit(argc, argv))) {
        LOG(ERROR) << "***Error Initializing PIL***";
        return EXIT_FAILURE;
    }
    if (PIL_OK != (status = PILGetFname("inattfilename", inattfilename))) {
        LOG(ERROR) << "***Error reading Attitude file:" << inattfilename << "***";
        return status;
    }
    if (PIL_OK != (status = PILGetFname("inlbtfilename", inlbtfilename))) {
        LOG(ERROR) << "***Error reading LBT file:" << inlbtfilename << "***";
        return status;
    }
    if (PIL_OK != (status = PILGetFname("inorbfilename", inorbfilename))) {
        LOG(ERROR) << "***Error reading Orbit file:" << inorbfilename << "***";
        return status;
    }
    if (PIL_OK != (status = PILGetFname("inhdrfilename", inhdrfilename))) {
        LOG(ERROR) << "***Error reading Header file:" << inhdrfilename << "***";
        return status;
    }
    if (PIL_OK != (status = PILGetFname("outmkffilename", outmkffilename))) {
        LOG(ERROR) << "***Error reading MKF file:" << outmkffilename << "***";
        return status;
    }
    if (PIL_OK != (status = PILGetBool("history", &history))) {
        LOG(ERROR) << "***Error reading history parameter***";
        return status;
    }

    if (PIL_OK != (status = PILGetBool("clobber", &clobber))) {
        LOG(ERROR) << "***Error reading clobber parameter***";
        return status;
    }

    PILClose(status);
    return (EXIT_SUCCESS);
}

int cztmkfgen::read(char* inattfilename, char* inlbtfilename, char* inorbfilename, char*inhdrfilename,  char* outmkffilename, int clobber, int history){
    strcpy(this->inattfilename, inattfilename);
    strcpy(this->inlbtfilename, inlbtfilename);
    strcpy(this->inorbfilename, inorbfilename);
    strcpy(this->inhdrfilename, inhdrfilename);
    strcpy(this->outmkffilename, outmkffilename);
    this->history=history;
    this->clobber=clobber;
    return EXIT_SUCCESS;
}

void cztmkfgen::display() {
    LOG(INFO) << "----------------------------------------------------------------------------";
    LOG(INFO) << "                           CZTMKFGEN PARAMETERS                               ";
    LOG(INFO) << "----------------------------------------------------------------------------";
    LOG(INFO) << "Modulename         : " << modulename;
    LOG(INFO) << "Orbit file         : " << inorbfilename;
    LOG(INFO) << "LBT file           : " << inlbtfilename;
    LOG(INFO) << "Attitude file      : " << inattfilename;
    LOG(INFO) << "Output mkf file    : " << outmkffilename;
    LOG(INFO) << "Clobber            : " << clobber;
    LOG(INFO) << "History            : " << history;
    LOG(INFO) << "----------------------------------------------------------------------------";
}

int cztmkfgen::cztmkfgenProcess(){
    int status=0;
    Mkf mkf;
    Attitude att;
    Orbit orb;
    LBT lbt;
    
    if (FileExists(outmkffilename)) {
        if (clobber == YES) {
            unlink(outmkffilename);
        } else {
            LOG(ERROR) << outmkffilename << "  already exists";
            LOG(ERROR) << "Use clobber=yes for overwriting the file";
            return (EXIT_FAILURE);
        }
    }
    
    LOG(INFO) << "Reading attitude, orbit & lbt file to recreate mkf.";
    try{
        att.read_attitude_file((string) inattfilename);
        orb.read_orbit_file((string) inorbfilename);
        lbt.read_lbt_file((string) inlbtfilename);
        att.display_header_keywords();
        orb.display_header_keywords();
        lbt.display_header_keywords();
        mkf.regenerate_mkf_values((string) inattfilename, (string) inlbtfilename,
                (string) inorbfilename,(string)inhdrfilename);
        mkf.write_regenerated_mkffile((string) outmkffilename, CORMKFTEMPATE);
	mkf.write_header(outmkffilename,inattfilename);
    } catch(ErrorHandler errHandler){
        logError(errHandler);
        if(errHandler.severity==errERROR){
            return EXIT_FAILURE;
        }
    }

    updateKeywords(outmkffilename, modulename);
    if (history == YES) {
        vector<string> vhistory;
        getHistory(vhistory);
        if (writeHistory(outmkffilename, vhistory)) {
            LOG(ERROR) << "Error writing history";
            return (EXIT_FAILURE);
        }
    }
    return (EXIT_SUCCESS);
}

void cztmkfgen::getHistory(vector<string>& vhistory){
    //char *user=getlogin();
    char *user = getenv("USER");
	string str = "Module run by " + (string) user;
    vhistory.push_back(str);
    vhistory.push_back("Parameter List START for " + (string) modulename);
    vhistory.push_back("P1 inattfilename=" + (string) inattfilename);
    vhistory.push_back("P2 inlbtfilename=" + (string) inlbtfilename);
    vhistory.push_back("P3 inorbfilename=" + (string) inorbfilename);
    vhistory.push_back("P4 outmkffilename=" + (string) outmkffilename);
    if (clobber == YES)
        vhistory.push_back("P5 clobber=yes");
    else
        vhistory.push_back("P5 clobber=no");
    if (history == YES)
        vhistory.push_back("P6 history=yes");
    else
        vhistory.push_back("P6 history=no");
    vhistory.push_back("Parameter List END");
}

