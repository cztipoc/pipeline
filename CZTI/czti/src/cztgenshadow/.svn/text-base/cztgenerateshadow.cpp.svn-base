
#include"cztgenerateshadow.h"

cztgenerateshadow::cztgenerateshadow() {
    strcpy(modulename, "cztgenerateshadow_v");
    strcat(modulename, VERSION);
}

int cztgenerateshadow::read(int argc, char **argv) {
    int status = 0;

    if (PIL_OK != (status = PILInit(argc, argv))) {
        LOG(ERROR) << "***Error in Initializing PIL***";
        return status;
    }

    if (PIL_OK != (status = PILGetString("coordtype", coordtype))) {
        LOG(ERROR) << "***Error reading input type***";
        LOG(ERROR) << "***Type can be camera/celestial***";
        return status;
    }

    if (PIL_OK != (status = PILGetReal("theta1", &theta1))) {
        LOG(ERROR) << "***Error reading theta1***";
        return status;
    }

    if (PIL_OK != (status = PILGetReal("theta2", &theta2))) {
        LOG(ERROR) << "***Error reading theta2***";
        return status;
    }

    if (strncasecmp(coordtype, "celestial", 6) == 0) {
        if (PIL_OK != (status = PILGetFname("aspectfileQ0", aspectfileQ0))) {
            LOG(ERROR) << "***Error reading input aspect file for Q0***";
            return status;
        }
        if (PIL_OK != (status = PILGetFname("aspectfileQ1", aspectfileQ1))) {
            LOG(ERROR) << "***Error reading input aspect file for Q1***";
            return status;
        }
        if (PIL_OK != (status = PILGetFname("aspectfileQ2", aspectfileQ2))) {
            LOG(ERROR) << "***Error reading input aspect file for Q2***";
            return status;
        }
        if (PIL_OK != (status = PILGetFname("aspectfileQ3", aspectfileQ3))) {
            LOG(ERROR) << "***Error reading input aspect file for Q3***";
            return status;
        }
    }

    if (PIL_OK != (status = PILGetFname("cameraGeomFile", cameraGeomFile))) {
        LOG(ERROR) << "***Error Reading camera geometry CALDB filename:" << cameraGeomFile << "***";
        return status;
    }

    if (PIL_OK != (status = PILGetFname("compMaskfile", compMaskfile))) {
        LOG(ERROR) << "***Error reading input compressed maskfile file***";
        return status;
    }
    
    if (PIL_OK != (status = PILGetFname("effectiveAreafile", effectiveAreafile))) {
        LOG(ERROR) << "***Error reading input effective area file***";
        return status;
    }

    do {
        if (PIL_OK != (status = PILGetInt("nbins", &nBins))) {
            LOG(ERROR) << "***Error Reading nBins: " << nBins << "***";
            return status;
        }
    } while (checkRange(1, 20, nBins));


    if (nBins == 1) {
        do {
            if (PIL_OK != (status = PILGetReal4("ekev", &ekev))) {
                LOG(ERROR) << "***Error Reading ekev: " << ekev << "***";
                return status;
            }
        } while (checkRange(10.0f, 100.0f, ekev));
    } else {
        do {
            if (PIL_OK != (status = PILGetReal4("minenergy", &minEnergy))) {
                LOG(ERROR) << "***Error Reading minEnergy: " << minEnergy << "***";
                return status;
            }
        } while (checkRange(10.0f, 100.0f, minEnergy));

        do {
            if (PIL_OK != (status = PILGetReal4("maxenergy", &maxEnergy))) {
                LOG(ERROR) << "***Error Reading maxEnergy: " << maxEnergy << "***";
                return status;
            }
        } while (checkRange(10.0f, 100.0f, maxEnergy));
    }

    if (PIL_OK != (status = PILGetBool("includeMask", &includeMask))) {
        LOG(ERROR) << "***Error Reading includeMask parameter***";
        return status;
    }

    if (includeMask == 1) {
        do {
            if (PIL_OK != (status = PILGetInt("maskoversampling", &maskOversampling))) {
                LOG(ERROR) << "***Error Reading maskOversampling factor: " << maskOversampling << "***";
                return status;
            }
        } while (checkRange(1, 20, maskOversampling));
    }
    
    if (PIL_OK != (status = PILGetReal4("sourceflux", &sourceflux))) {
        LOG(ERROR) << "***Error reading sourceflux (in counts/area)***";
        return status;
    }

    if (PIL_OK != (status = PILGetFname("outfile", outfile))) {
        LOG(ERROR) << "***Error reading output file***";
        return status;
    }
    if (PIL_OK != (status = PILGetBool("history", &history))) {
        LOG(ERROR) << "***Error reading history parameter***";
        return status;
    }
    if (PIL_OK != (status = PILGetBool("clobber", &clobber))) {
        LOG(ERROR) << "***Error Reading clobber***";
        return status;
    }
    if (PIL_OK != (status = PILGetBool("debug", &debug))) {
        LOG(ERROR) << "***Error Reading debug***";
        return status;
    }
    PILClose(status);
    return (EXIT_SUCCESS);
}

int cztgenerateshadow::read(char *coordtype, double theta1, double theta2, char *aspectfileQ0,char *aspectfileQ1,
            char *aspectfileQ2,char *aspectfileQ3, char *compMaskfile, char *cameraGeomfile,
            char *effectiveAreafile, float ekev, float minEnergy, float maxEnergy, int nBins,
            int includeMask, int maskOversampling,float sourceflux, char *outfile, 
            int clobber, int history, int debug){
    strcpy(this->coordtype, coordtype);
    if (aspectfileQ0 != NULL) {
        strcpy(this->aspectfileQ0, aspectfileQ0);
    }
    if (aspectfileQ1 != NULL) {
        strcpy(this->aspectfileQ1, aspectfileQ1);
    }
    if (aspectfileQ2 != NULL) {
        strcpy(this->aspectfileQ2, aspectfileQ2);
    }
    if (aspectfileQ3 != NULL) {
        strcpy(this->aspectfileQ3, aspectfileQ3);
    }
    strcpy(this->compMaskfile, compMaskfile);
    strcpy(this->effectiveAreafile, effectiveAreafile);
    this->theta1 = theta1;
    this->theta2 = theta2;
    this->ekev=ekev;
    this->minEnergy=minEnergy;
    this->maxEnergy=maxEnergy;
    this->nBins=nBins;
    this->includeMask=includeMask;
    this->maskOversampling=maskOversampling;            
    this->sourceflux = sourceflux;
    strcpy(this->outfile, outfile);
    this->clobber = clobber;
    this->history = history;
    this->debug = debug;
    return (EXIT_SUCCESS);
}

void cztgenerateshadow::display() {
    LOG(INFO) << "----------------------------------------------------------------------------";
    LOG(INFO) << "                          CZTGENSHADOW PARAMETERS                           ";
    LOG(INFO) << "----------------------------------------------------------------------------";
    LOG(INFO) << "Coordinate type          : " << coordtype;
    LOG(INFO) << "Theta 1 (degrees)        : " << theta1;
    LOG(INFO) << "Theta 2 (degrees)        : " << theta2;
    LOG(INFO) << "Camera Geometry file     : " << cameraGeomFile;
    LOG(INFO) << "Input Aspect file Q0     : " << aspectfileQ0;
    LOG(INFO) << "Input Aspect file Q1     : " << aspectfileQ1;
    LOG(INFO) << "Input Aspect file Q2     : " << aspectfileQ2;
    LOG(INFO) << "Input Aspect file Q3     : " << aspectfileQ3;
    LOG(INFO) << "Input Mask file          : " << compMaskfile;
    LOG(INFO) << "Effective Area file      : " << effectiveAreafile;
    LOG(INFO) << "Source Flux              : " << sourceflux;
    LOG(INFO) << "Include mask?            : " << includeMask;
    if(includeMask==1){
    LOG(INFO) << "Mask oversampling factor : " << maskOversampling;
    }
    LOG(INFO) << "Number of energy bins    : " << nBins;
    if(nBins>1){
    LOG(INFO) << "Energy start (keV)       : " << minEnergy;
    LOG(INFO) << "Energy end   (keV)       : " << maxEnergy;
    } else {
    LOG(INFO) << "Energy(keV)              : " << ekev;
    }
    LOG(INFO) << "Output shadow file       : " << outfile;
    LOG(INFO) << "Clobber                  : " << clobber;
    LOG(INFO) << "History                  : " << history;
    LOG(INFO) << "Debug                    : " << debug;
    LOG(INFO) << "----------------------------------------------------------------------------";
}

int cztgenerateshadow::cztgenerateshadowProcess() {
    ShadowFileHandler sfh; //read and write shadow file
    AspectFileHandler aspect; //to read and process aspect files
    ExposureTable expTable;
    TanMaskGeometry tanmask; //to read compressed mask file
    vector <vector <unsigned char> > fullfinermask; //to store 18200x18200 uncompressed mask
    EffArea effarea;
    vector <vector <float> > fullNormalizedEffarea;
    Shadow shadow;
    ShadowTable shadowtab;
    vector <string> aspectFilenames;
    float nX = 0.0, nY = 0.0, nZ = 0.0, nXt = 0.0, nYt = 0.0, nZt = 0.0; //pointing and twist vectors
    float RA = 0.0, DEC = 0.0, TWIST = 0.0; //RA,DEC (pointing direction) & TWIST
    double thetax, thetay; //camera coordinates in degrees
    double txrad, tyrad; //thetax and thetay in radians
    double theta=0.0; //camera coordinate in degree
    double trad=0.0;  // theta in radians
    double dx, dy;
    string compMaskFilePath=""; //complete path to compressed mask file
    string cameraGeometryFilePath=""; //complete path to camera geometry file
    string effareaFilePath=""; //complete path to effective area file
    vector<string> shadowExtnames; //shadow extension names in output shadow file
    vector<string> debugFilenames;
    string tempDebugfilename;
    vector<string> keys; //key names
    vector<float> valuesF; //Float values
    vector<int> valuesI; //Integer values
    
    int status = 0; //status variable

    if (FileExists(outfile)) {
        if (clobber == YES) {
            unlink(outfile);
        } else {
            LOG(ERROR) << "" << outfile << " already exists";
            LOG(ERROR) << "Use clobber=yes for overwriting the file";
            return (EXIT_FAILURE);
        }
    }

    if (strncasecmp(coordtype, "camera", 6) == 0) {
        thetax = theta1;
        thetay = theta2;
        txrad = theta1 * TORAD;
        tyrad = theta2 * TORAD;
    }        //converting RA, DEC to camera coordinates
    else if (strncasecmp(coordtype, "celestial", 9) == 0) {

        //storing aspect filenames in a single vector
        aspectFilenames.push_back(aspectfileQ0);
        aspectFilenames.push_back(aspectfileQ1);
        aspectFilenames.push_back(aspectfileQ2);
        aspectFilenames.push_back(aspectfileQ3);

        //Reading multiple aspect files to calculate average RA, DEC & TWIST values for
        //all 4 quadrants.
        if (aspect.read_multiple_aspect_files(aspectFilenames)) {
            LOG(ERROR) << "Error in reading multiple aspect files simultaneously.";
            return EXIT_FAILURE;
        }

        if (aspect.get_avg_pointing_and_twist_vector(nX, nY, nZ, nXt, nYt, nZt, status)) {
            LOG(ERROR) << "Error in calculating average pointing and twist vector.";
            return EXIT_FAILURE;
        }

        if (calculate_RA_DEC_Twist(nX, nY, nZ, nXt, nYt, nZt, RA, DEC, TWIST, status)) {
            LOG(ERROR) << "Error in calculating RA, DEC & TWIST values.";
            return EXIT_FAILURE;
        }
        
        if (to_thetaX_thetaY((double) RA, (double) DEC, (double) TWIST, theta1*TORAD, theta2*TORAD, 
                txrad, tyrad)) {
            LOG(ERROR) << "Error in converting celestial coordinates RA,DEC: " <<
                    theta1 << ", " << theta2 << " to camera coordinates.";
            return EXIT_FAILURE;
        }
        thetax = txrad * TODEG;
        thetay = tyrad * TODEG;
    } else {
        LOG(ERROR) << "Coordinates entered by user are neither celestial nor camera.";
        return EXIT_FAILURE;
    }
    //Direction of source photon calculated in radians (in camera coordinates)
    
    //Calculating theta
    trad = atan(tan(txrad)*tan(txrad) + tan(tyrad)*tan(tyrad));
    theta = trad*TORAD;
    
    //COMPRESSED MASK READING
    compMaskFilePath = caldb_full_path_generator(compMaskfile);
    //Extracting compressed mask file
    LOG(INFO) << "Reading compressed mask file to create 18200x18200 full uncompressed mask.";
    if (tanmask.read_compressed_mask(compMaskFilePath)) {
        return EXIT_FAILURE;
    }
    if (tanmask.get_full_finer_mask()) {
        LOG(ERROR) << "Error in generating full finer mask from individual quadrant masks.";
        return EXIT_FAILURE;
    }
    //storing full finer mask
    fullfinermask = tanmask.get_fullUnMask();    
    //clearing tanmask stored variables to save memory
    tanmask.reset_full_uncomressed_mask();
    //COMPRESSED MASK READING END

    //EFFECTIVE AREA FILE READING
    effareaFilePath = caldb_full_path_generator(effectiveAreafile);
    if(nBins==1){
        if(effarea.calculate_full_normalized_effarea(effareaFilePath, ekev, ekev)){
            LOG(ERROR) << "Error in evaluation of full normalized effective area " <<
                    "for energy " << ekev;
            return EXIT_FAILURE;
        }
    }
    else if(nBins>1){
        if(effarea.calculate_full_normalized_effarea(effareaFilePath, minEnergy, maxEnergy)){
            LOG(ERROR) << "Error in evaluation of full normalized effective area " <<
                    "for energy " << minEnergy << " to " << maxEnergy << ".";
            return EXIT_FAILURE;
        }
    }
    fullNormalizedEffarea = effarea.get_fullnormalized_effarea();
    //resetting effective area to save memory

    effarea.reset();
    //EFFECTIVE AREA FILE READING END
    
    //SHADOW COMPUTATION
    cameraGeometryFilePath = caldb_full_path_generator(cameraGeomFile);
    //setting shadow file parameters
    shadow.set_camerageomfile(cameraGeometryFilePath);
    if(nBins>1){
        shadow.set_energyrange(nBins, minEnergy, maxEnergy);
    } else if(nBins==1){
        shadow.set_energyvalue(nBins, ekev);
    }
    if(includeMask==1){
        shadow.set_maskParameter(includeMask, maskOversampling);
    } else
    {  
        shadow.set_maskParameter(includeMask);
    }
    shadow.set_sourceFlux(sourceflux);
    shadow.set_thetaXYr(txrad, tyrad);
    //display shadow parameters
    shadow.display();
    //checking whether shadow parameters are in valid range
    if(shadow.check_shadow_par()){
        LOG(ERROR) << "Shadow generation parameters not in valid range.";
        return (EXIT_FAILURE);
    }
    //compute shadow
    if(shadow.compute_shadow(fullNormalizedEffarea, fullfinermask)){
        LOG(ERROR) << "Error in computing shadow.";
        return EXIT_FAILURE;
    }
    //getting shadow table and storing it in local variable
    shadowtab = shadow.get_shadow_table();
    //resetting shadow object to save memory
    shadow.reset_shadowtab();
    //SHADOW COMPUTATION END
    
    //DEBUG OUTPUT
    if(debug==YES){
        tempDebugfilename = (string)outfile + "_debug_shadow.img";
        debugFilenames.push_back(tempDebugfilename);
        if(deleteFile(debugFilenames, clobber)){
            LOG(ERROR) << "Error in deleting debug files.";
            return EXIT_FAILURE;
        }
        //writing debug file
        if(write_img(shadowtab.shadow2D, tempDebugfilename, FLOAT_IMG, TFLOAT)){
            LOG(ERROR) << "Error in writing debug shadow image.";
            return EXIT_FAILURE;
        }
    }
    
    
    sfh.set_shadowtab(shadowtab);
    //Writing shadow file
    if(sfh.create_shadow_file(outfile, SHADOWTEMPLATE, shadowExtnames)){
        LOG(ERROR) << "Error in creating empty shadow file " << outfile;
        return EXIT_FAILURE;
    }
    if(sfh.write_shadow_file(outfile, "SHADOW")){
        LOG(ERROR) << "Error in writing SHADOW extension in output shadow file " << outfile;
        return EXIT_FAILURE;
    }
    
    string keysArray[]={"THETAX","THETAY","FLUX","ESTART","ESTOP","NBINS"};
    float valuesFArray[]={shadowtab.thetaXr, shadowtab.thetaYr, shadowtab.sourceFlux, shadowtab.energyMin, shadowtab.energyMax,
                            (float)shadowtab.nBins};
    keys.assign(keysArray,keysArray+6);
    valuesF.assign(valuesFArray,valuesFArray+6);
    updateKeys(outfile, "SHADOW", keys, valuesF, TFLOAT);

    if (history == YES) {
        vector<string> vhistory;
        get_history(vhistory);
        writeHistory(outfile, vhistory);
    }
    //UPDATING KEYWORDS
    updateKeywords(outfile, modulename);

    return (EXIT_SUCCESS);
}

int cztgenerateshadow::get_history(vector<string> &vhistory) {
    //char *user = getlogin();
    char *user = getenv("USER");
	string str = "Module run by " + (string) user;
    vhistory.push_back(str);
    vhistory.push_back("Parameter List START for " + (string) modulename);
    vhistory.push_back("P1 inputtype=" + (string) coordtype);
    char temp[25];
    sprintf(temp, "%f", theta1);
    vhistory.push_back("P2 theta1=" + (string) temp);
    sprintf(temp, "%d", theta2);
    vhistory.push_back("P3 theta2=" + (string) temp);
    vhistory.push_back("P4 cameraGeomFile=" + (string) cameraGeomFile);
    vhistory.push_back("P5 compMaskfile=" + (string) compMaskfile);
    vhistory.push_back("P6 effAreafile=" + (string) effectiveAreafile);
    vhistory.push_back("P7 nBins=" + itoa(nBins));
    vhistory.push_back("P8 minEnergy=" + itoa(minEnergy));
    vhistory.push_back("P9 maxEnergy=" + itoa(maxEnergy));
    vhistory.push_back("P10 include mask?=" + itoa(includeMask));
    vhistory.push_back("P11 mask oversampling factor=" + itoa(maskOversampling));
    vhistory.push_back("P12 sourceflux=" + itoa(sourceflux));
    vhistory.push_back("P13 outfile=" + (string) outfile);
    if(coordtype=="celestial"){
        vhistory.push_back("P14 aspectfileQ0="+(string)aspectfileQ0);
        vhistory.push_back("P15 aspectfileQ1="+(string)aspectfileQ1);
        vhistory.push_back("P16 aspectfileQ2="+(string)aspectfileQ2);
        vhistory.push_back("P17 aspectfileQ3="+(string)aspectfileQ3);
    }
    if (clobber == YES)
        vhistory.push_back("P18 clobber=yes");
    else
        vhistory.push_back("P19 clobber=no");
    if (history == YES)
        vhistory.push_back("P20 history=yes");
    else
        vhistory.push_back("P21 history=no");
    vhistory.push_back("Parameter List END");
    return (EXIT_SUCCESS);
}

