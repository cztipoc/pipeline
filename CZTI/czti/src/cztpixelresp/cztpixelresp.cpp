#include "cztpixelresp.h"


Cztpixelresp::Cztpixelresp() {
    strcpy(modulename, "cztpixelresp_v");
    strcpy(modulename, VERSION);
}

int Cztpixelresp::read(int argc, char** argv){
    int status=0; //status variable
    
    if (PIL_OK != (status = PILInit(argc, argv))) {
        LOG(ERROR) << "*** Error initializing PIL ***";
        return status;
    }
    if (PIL_OK != (status = PILGetFname("par_eboundsFile", eboundsFile))) {
        LOG(ERROR) << "***Error Reading ebounds file:" << eboundsFile << "***";
        return status;
    }

    if (PIL_OK != (status = PILGetFname("par_respParFile", respParFile))) {
        LOG(ERROR) << "***Error Reading Response file:" << respParFile << "***";
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

int Cztpixelresp::read(char *eboundsFile, char* respParFile, int clobber, int history){
    strcpy(this->eboundsFile, eboundsFile);
    strcpy(this->respParFile, respParFile);
    this->clobber = clobber;
    this->history = history;
    
    return (EXIT_SUCCESS);
}

void Cztpixelresp::display(){
    LOG(INFO) << "----------------------------------------------------------------------";
    LOG(INFO) << "               CZTPIXELRESP PARAMETERS                                ";
    LOG(INFO) << "----------------------------------------------------------------------";
    LOG(INFO) << "Input Ebounds File        : " << eboundsFile;
    LOG(INFO) << "Output Response File      : " << respParFile;
    LOG(INFO) << "Clobber                   : " << clobber;
    LOG(INFO) << "History                   : " << history;
    LOG(INFO) << "----------------------------------------------------------------------------";        
}

int Cztpixelresp::cztPixelRespProcess(){
    int i,j=0;
    int status=0;
    ResponsePar responseParObj;
    Ebounds eboundsObj;
    
}
