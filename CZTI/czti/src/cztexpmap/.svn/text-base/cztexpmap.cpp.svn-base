#include "cztexpmap.h"

Cztexpmap::Cztexpmap(){
    strcpy(modulename, "cztexpmap_v");
    strcat(modulename, VERSION);
}

int Cztexpmap::read(int argc, char** argv){
    int status=0;

    if (PIL_OK != (status = PILInit(argc, argv))) {
        LOG(ERROR) << "***Error Initializing PIL***";
        return status;
    }

    if (PIL_OK != (status = PILGetFname("par_cameraGeomFile", cameraGeomFile))) {
        LOG(ERROR) << "***Error Reading camera geometry CALDB filename:" << cameraGeomFile << "***";
        return status;
    }
    
    if (PIL_OK != (status = PILGetFname("par_compMaskFile", compressedMaskFile))) {
        LOG(ERROR) << "***Error Reading compressed mask CALDB filename:" << compressedMaskFile << "***";
        return status;
    }

    if (PIL_OK != (status = PILGetFname("par_expfile", outExposureFile))) {
        LOG(ERROR) << "***Error Reading output exposure filename:" << outExposureFile << "***";
        return status;
    }
    
    do{
    if (PIL_OK != (status = PILGetReal4("par_thetax", &thetax))) {
        LOG(ERROR) << "***Error Reading thetax: " << thetax << "***";
        return status;
    }
    } while (checkRange(0.0f,90.0f, thetax));
    
    do{
    if (PIL_OK != (status = PILGetReal4("par_thetay", &thetay))) {
        LOG(ERROR) << "***Error Reading thetay: " << thetay << "***";
        return status;
    }
    } while (checkRange(0.0f,90.0f, thetay));
    
    do{
    if (PIL_OK != (status = PILGetInt("par_nbins", &nBins))) {
        LOG(ERROR) << "***Error Reading nBins: " << nBins << "***";
        return status;
    }
    } while (checkRange(1, 20, nBins));
    
    
    if(nBins==1) {
        do {
            if (PIL_OK != (status = PILGetReal4("par_ekev", &ekev))) {
                LOG(ERROR) << "***Error Reading ekev: " << ekev << "***";
                return status;
            }
        } while (checkRange(0.0f, 1000.0f, ekev));       
    }
    else {
        do {
            if (PIL_OK != (status = PILGetReal4("par_minenergy", &minEnergy))) {
                LOG(ERROR) << "***Error Reading minEnergy: " << minEnergy << "***";
                return status;
            }
        } while (checkRange(10.0f, 100.0f, minEnergy));

        do {
            if (PIL_OK != (status = PILGetReal4("par_maxenergy", &maxEnergy))) {
                LOG(ERROR) << "***Error Reading maxEnergy: " << maxEnergy << "***";
                return status;
            }
        } while (checkRange(10.0f, 100.0f, maxEnergy));
    }
    
    if (PIL_OK != (status = PILGetBool("par_includeMask", &includeMask))) {
        LOG(ERROR) << "***Error Reading includeMask parameter***";
        return status;
    }

    if(includeMask==1){
    do {
        if (PIL_OK != (status = PILGetInt("par_maskoversampling", &maskOversampling))) {
            LOG(ERROR) << "***Error Reading maskOversampling factor: " << maskOversampling << "***";
            return status;
        }
    } while (checkRange(1, 20, maskOversampling));
    }
    
    if (PIL_OK != (status = PILGetBool("par_history", &history))) {
        LOG(ERROR) << "***Error Reading history parameter***";
        return status;
    }

    if (PIL_OK != (status = PILGetBool("par_clobber", &clobber))) {
        LOG(ERROR) << "***Error Reading clobber***";
        return status;
    }
    if (PIL_OK != (status = PILGetBool("par_debug", &debug))) {
        LOG(ERROR) << "***Error Reading debug***";
        return status;
    }

    PILClose(status);
    return (EXIT_SUCCESS);
   
}


int Cztexpmap::read(char* cameraGeomFile, char* compressedMaskFile, char* outExposureFile, 
        float thetax, float thetay, int includeMask, int maskOversampling, int nBins, 
        float ekev, float minEnergy, float maxEnergy, int clobber, 
        int history, int debug)
{
    strcpy(this->cameraGeomFile, cameraGeomFile);
    strcpy(this->outExposureFile, outExposureFile);
    strcpy(this->compressedMaskFile, compressedMaskFile);
    this->thetax = thetax;
    this->thetay = thetay;
    this->includeMask = includeMask;
    this->maskOversampling=maskOversampling;
    this->nBins = nBins;
    this->ekev = ekev;
    this->minEnergy=minEnergy;
    this->maxEnergy=maxEnergy;
    this->clobber = clobber;
    this->history = history;
    return (EXIT_SUCCESS);
}

void Cztexpmap::display() {
    LOG(INFO) << "----------------------------------------------------------------------------";
    LOG(INFO) << "                          CZTEXPMAP PARAMETERS                            ";
    LOG(INFO) << "----------------------------------------------------------------------------";
    LOG(INFO) << "Camera Geometry File                       :" << cameraGeomFile;
    LOG(INFO) << "Compressed Mask File                       :" << compressedMaskFile;
    LOG(INFO) << "Output Exposure File                       :" << outExposureFile;
    LOG(INFO) << "thetax                                     :" << thetax;
    LOG(INFO) << "thetay                                     :" << thetay;
    LOG(INFO) << "Include Mask Transmission                  :" << includeMask;
    if(includeMask==1){
    LOG(INFO) << "Mask oversampling factor                   :" << maskOversampling;
    }
    LOG(INFO) << "Number of Energy bins                      :" << nBins;
    if(nBins==1){
    LOG(INFO) << "Energy (in keV)                            :" << ekev;
    } else {
    LOG(INFO) << "Minimum Energy (in keV)                    :" << minEnergy;    
    LOG(INFO) << "Maximum Energy (in keV)                    :" << maxEnergy;    
    }
    LOG(INFO) << "Clobber                                    :" << clobber;
    LOG(INFO) << "History                                    :" << history;
    LOG(INFO) << "Debug                                      :" << debug;
    LOG(INFO) << "----------------------------------------------------------------------------";
}

int Cztexpmap::cztexpmap_process(){
    int status=0;
    ExposuremapFileHandler expfile;
    string compMaskFilePath="";
    ExposureTable expTable;
    TanMaskGeometry msk; //to get full uncompressed mask
    vector <vector <unsigned char> > fullfinermask;
    vector <vector <float> > fulloftrans; //full mask open fraction with transmission
    string outDebugfile;

    //deleting output exposure file if it already exists.
    if (FileExists(outExposureFile)) {
        if (clobber == YES) {
            unlink(outExposureFile);
        } else {
            LOG(ERROR) << outExposureFile << "  already exists";
            LOG(ERROR) << "Use clobber=yes for overwriting the file";
            return (EXIT_FAILURE);
        }
    }
    
    compMaskFilePath = caldb_full_path_generator(compressedMaskFile);

    LOG(INFO) << "Reading compressed mask file to create 18200x18200 full uncompressed mask.";
    //Getting full finer mask
    if (msk.read_compressed_mask(compMaskFilePath)) {
        LOG(ERROR) << "Error in reading CALDB finer mask file " << compMaskFilePath;
        return EXIT_FAILURE;
    }
    if (msk.get_full_finer_mask()) {
        LOG(ERROR) << "Error in generating full finer mask from individual quadrant masks.";
        return EXIT_FAILURE;
    }

    //storing full finer mask
    fullfinermask = msk.get_fullUnMask();
    
    LOG(INFO) << "Mask oversampling: " << maskOversampling;
    if(nBins>1){
        if(calculate_average_pixexposure(thetax, thetay, cameraGeomFile, fullfinermask,
            expTable, includeMask, maskOversampling, nBins,minEnergy, maxEnergy)){
            LOG(ERROR) << "Error in evaluating exposure map.";
            return EXIT_FAILURE;
        }
    }
    else if(nBins==1){
        if(calculate_average_pixexposure(thetax, thetay, cameraGeomFile, fullfinermask,
            expTable, includeMask, maskOversampling, nBins,ekev)){
            LOG(ERROR) << "Error in evaluating exposure map.";
            return EXIT_FAILURE;
        }
    }

    //Writing exposure map file
    if(expfile.set_exposuretable(expTable)){
        LOG(ERROR)<< "Error in assigning exposure table evaluated to corresponding variable";
        return EXIT_FAILURE;
    }
    
    if(expfile.write_exposure_map_file(outExposureFile, "expmapTemplate")){
        LOG(ERROR)<< "Error in writing Exposure map file: "<< outExposureFile;
        return EXIT_FAILURE;
    }
    
    //Vizualization of exposure
    if(debug==YES){
        expTable.generate_full_exposure_image();
        outDebugfile = (string) outExposureFile +"_debug"+".img";
        if (FileExists((char *) outDebugfile.c_str())) {
            unlink((char*)outDebugfile.c_str());
        }
        write_img(expTable.fullExposure, outDebugfile, FLOAT_IMG, TFLOAT);
    }


    //Writing History
    if (history == YES) {
        vector<string> vhistory;
        get_history(vhistory);
        writeHistory(outExposureFile, vhistory); //writes history to each HDU of output filE
    }    

    //UPDATING KEYWORDS
    updateKeywords(outExposureFile, modulename);    
    
    return status;
}

int Cztexpmap::get_history(vector<string>& vhistory){
    char temp[25];
    //char *user = getlogin();
	char *user = getenv("USER");
    string str = "Module run by " + (string) user;
    vhistory.push_back(str);
    vhistory.push_back("Parameter List START for " + (string) modulename);
    vhistory.push_back("P1 camera geometry file =" + (string) cameraGeomFile);
    vhistory.push_back("P2 output exposure file =" + (string) outExposureFile);
    
    sprintf(temp, "%f", thetax);
    vhistory.push_back("P3 thetax (in degrees) =" + (string) temp);
    sprintf(temp, "%f", thetay);
    vhistory.push_back("P4 thetay (in degrees) =" + (string) temp);
    sprintf(temp, "%f", ekev);
    vhistory.push_back("P5 Energy (in keV) =" + (string) temp);
    if (clobber == YES)
        vhistory.push_back("P6 clobber=yes");
    else
        vhistory.push_back("P6 clobber=no");
    if (history == YES)
        vhistory.push_back("P7 history=yes");
    else
        vhistory.push_back("P7 history=no");
    vhistory.push_back("Parameter List END");

    return (EXIT_SUCCESS);    
}
