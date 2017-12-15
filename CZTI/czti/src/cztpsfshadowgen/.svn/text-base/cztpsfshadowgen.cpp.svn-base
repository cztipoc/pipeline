
#include "cztpsfshadowgen.h"
#include "maskGeometry.h"

cztpsfshadowgen::cztpsfshadowgen() {
    strcpy(modulename, "cztpsfshadowgen_v");
    strcat(modulename, VERSION);
}

int cztpsfshadowgen::read(int argc, char **argv) {
    int status = 0;

    if (PIL_OK != (status = PILInit(argc, argv))) {
        LOG(ERROR) << "***Error Initializing PIL***";
        return status;
    }

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

    if (PIL_OK != (status = PILGetFname("catalog", catalogfile))) {
        LOG(ERROR) << "***Error Reading catalog file:" << catalogfile << "***";
        return status;
    }

    if (PIL_OK != (status = PILGetInt("extnumCatalog", &extnumCatalog))) {
        LOG(ERROR) << "***Error Reading extension number for catalog file:" << catalogfile << "***";
        return status;
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

    if (PIL_OK != (status = PILGetReal4("fovx", &fovx))) {
        LOG(ERROR) << "***Error Reading fovx: " << fovx << "***";
        return status;
    }
    
    if (PIL_OK != (status = PILGetReal4("fovy", &fovy))) {
        LOG(ERROR) << "***Error Reading fovy: " << fovy << "***";
        return status;
    }

    if (PIL_OK != (status = PILGetFname("outfile", outfile))) {
        LOG(ERROR) << "***Error reading output file***";
        return status;
    }

    if (PIL_OK != (status = PILGetFname("sourcelist", sourcelist))) {
        LOG(ERROR) << "***Error reading output sourcelist file name:" << sourcelist << "***";
        return status;
    }

    if (PIL_OK != (status = PILGetBool("history", &history))) {
        LOG(ERROR) << "***Error Reading history parameter***";
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

int cztpsfshadowgen::read(char *aspectfileQ0, char *aspectfileQ1, char *aspectfileQ2, char *aspectfileQ3,
            char *catalog,int extnumCatalog,char *compmaskfile, char *camerageomfile, 
            char* effareafile, char* sourcelist, char *outfile, float fovx, float fovy, int includeMask,
            int nBins, float ekev, float minEnergy, float maxEnergy, int maskOversampling,
            int clobber,int history) {
    strcpy(this->aspectfileQ0, aspectfileQ0);
    strcpy(this->aspectfileQ1, aspectfileQ1);
    strcpy(this->aspectfileQ2, aspectfileQ2);
    strcpy(this->aspectfileQ3, aspectfileQ3);
    strcpy(this->catalogfile, catalog);
    this->extnumCatalog = extnumCatalog;
    strcpy(this->compMaskfile, compmaskfile);
    strcpy(this->cameraGeomFile, camerageomfile);
    strcpy(this->effectiveAreafile, effareafile);
    strcpy(this->outfile, outfile);
    strcpy(this->sourcelist, sourcelist);
    this->fovx=fovx;
    this->fovy=fovy;
    this->includeMask=includeMask;
    this->nBins=nBins;
    this->ekev=ekev;
    this->minEnergy=minEnergy;
    this->maxEnergy=maxEnergy;
    this->maskOversampling=maskOversampling;
    this->clobber = clobber;
    this->history = history;
    return (EXIT_SUCCESS);
}

void cztpsfshadowgen::display() {
    LOG(INFO) << "----------------------------------------------------------------------------";
    LOG(INFO) << "                          CZTPSFSHADOWGEN PARAMETERS                            ";
    LOG(INFO) << "----------------------------------------------------------------------------";
    LOG(INFO) << "Input Aspect file Q0     : " << aspectfileQ0;
    LOG(INFO) << "Input Aspect file Q1     : " << aspectfileQ1;
    LOG(INFO) << "Input Aspect file Q2     : " << aspectfileQ2;
    LOG(INFO) << "Input Aspect file Q3     : " << aspectfileQ3;
    LOG(INFO) << "Input Catalog file       : " << catalogfile;
    LOG(INFO) << "Extension no in catalog  : " << extnumCatalog;
    LOG(INFO) << "Camera Geometry file     : " << cameraGeomFile;
    LOG(INFO) << "Input Mask file          : " << compMaskfile;
    LOG(INFO) << "Effective Area file      : " << effectiveAreafile;
    LOG(INFO) << "Include mask?            : " << includeMask;
    if (includeMask == 1) {
    LOG(INFO) << "Mask oversampling factor : " << maskOversampling;
    }
    LOG(INFO) << "Number of energy bins    : " << nBins;
    if (nBins > 1) {
    LOG(INFO) << "Energy start (keV)       : " << minEnergy;
    LOG(INFO) << "Energy end   (keV)       : " << maxEnergy;
    } else {
    LOG(INFO) << "Energy(keV)              : " << ekev;
    }
    LOG(INFO) << "Field of View x (deg)    : " << fovx;
    LOG(INFO) << "Field of View y (deg)    : " << fovy;
    LOG(INFO) << "Output shadow file       : " << outfile;
    LOG(INFO) << "Output source list file  : " << sourcelist;
    LOG(INFO) << "Clobber                  : " << clobber;
    LOG(INFO) << "History                  : " << history;
    LOG(INFO) << "----------------------------------------------------------------------------";
}


int cztpsfshadowgen::cztpsfshadowgenProcess() {
    ShadowFileHandler sfh; //read and write shadow file
    AspectFileHandler aspect; //read and process aspect files to convert from RA to DEC
    StarCatalog starcat; //to read catalog file
    Catalog catalog; //store catalog file data
    vector <string> aspectFilenames;
    float nX = 0.0, nY = 0.0, nZ = 0.0, nXt = 0.0, nYt = 0.0, nZt = 0.0; //pointing and twist vectors
    float RA = 0.0, DEC = 0.0, TWIST = 0.0; //RA,DEC (pointing direction) & TWIST
    TanMaskGeometry tanmask; //to read compressed mask file
    vector <vector <unsigned char> > fullfinermask; //to store 18200x18200 uncompressed mask
    EffArea effarea;
    vector <vector <float> > fullNormalizedEffarea;
    string cameraGeometryFilePath = ""; //complete path to camera geometry file
    Shadow shadow;
    ShadowTable shadowtab;
    string catalogFilePath="";
    string compMaskFilePath="";
    string effareaFilePath = ""; //complete path to effective area file
    vector<string> outfiles; //name of output files of this module.
    vector<string> shadowExtnames; //shadow extension names in output shadow file
    vector<string> keys; //key names
    vector<float> valuesF; //Float values
    int status=0;
    int nsources=0; //number of sources found in CZTI FOV
    int isource=0; //index over number of sources found in CZTI field of view
    
    //deleting output files if clobber is yes
    outfiles.push_back(outfile);
    outfiles.push_back(sourcelist);
    if(deleteFile(outfiles,clobber)){
        LOG(ERROR) << "Error in deleting files.";
        return EXIT_FAILURE;
    }
    //output files deleted
    
    //GETTING RA,DEC, TWIST FOR POINTING DIRECTION
    //Reading multiple aspect files to calculate average RA, DEC & TWIST values
    //for all 4 quadrants
    aspectFilenames.push_back(aspectfileQ0);
    aspectFilenames.push_back(aspectfileQ1);
    aspectFilenames.push_back(aspectfileQ2);
    aspectFilenames.push_back(aspectfileQ3);

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
    //RA, DEC & TWIST VALUES STORED
    
    //FINDING SOURCES IN CZTI FIELD OF VIEW
    catalogFilePath = caldb_full_path_generator((string) catalogfile);
    if(starcat.read_catalog_file(catalogFilePath, extnumCatalog)){
        LOG(ERROR) << "Error in reading Catalog file.";
        return EXIT_FAILURE;
    }
    if(starcat.find_sources_in_FOV(catalog, RA, DEC, TWIST, fovx*TORAD, fovy*TORAD)){
        LOG(ERROR) << "Error in finding source in CZTI Field of View.";
        return EXIT_FAILURE;
    }
    nsources = catalog.get_nsources();
    LOG(INFO) << "Total number of sources found in CZTI FOV (" << fovx << "deg," << fovy <<"deg) : " << nsources;
    //FOUND SOURCES PRESENT IN CZTI FIELD OF VIEW

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
    if (nBins == 1) {
        if (effarea.calculate_full_normalized_effarea(effareaFilePath, ekev, ekev)) {
            LOG(ERROR) << "Error in evaluation of full normalized effective area " <<
                    "for energy " << ekev;
            return EXIT_FAILURE;
        }
    } else if (nBins > 1) {
        if (effarea.calculate_full_normalized_effarea(effareaFilePath, minEnergy, maxEnergy)) {
            LOG(ERROR) << "Error in evaluation of full normalized effective area " <<
                    "for energy " << minEnergy << " to " << maxEnergy << ".";
            return EXIT_FAILURE;
        }
    }
    fullNormalizedEffarea = effarea.get_fullnormalized_effarea();
    //resetting effective area to save memory
    effarea.reset();
    //EFFECTIVE AREA FILE READING END

    //Creating Shadow file to store nsources
    //SHADOW COMPUTATION FOR NSOURCES
    cameraGeometryFilePath = caldb_full_path_generator(cameraGeomFile);
    //setting shadow file parameters
    
    for(isource=0; isource<nsources; isource++){
        shadowExtnames.push_back(catalog.sourceNameBAT[isource]);
    }
    
    //creating empty shadow file
    if (sfh.create_shadow_file(outfile, SHADOWTEMPLATE, shadowExtnames)) {
        LOG(ERROR) << "Error in creating empty shadow file " << outfile;
        return EXIT_FAILURE;
    }
    for(isource=0; isource<nsources; isource++) {
        shadow.set_camerageomfile(cameraGeometryFilePath);
        if (nBins > 1) {
            shadow.set_energyrange(nBins, minEnergy, maxEnergy);
        } else if (nBins == 1) {
            shadow.set_energyvalue(nBins, ekev);
        }
        if (includeMask == 1) {
            shadow.set_maskParameter(includeMask, maskOversampling);
        } else {
            shadow.set_maskParameter(includeMask);
        }
        shadow.set_sourceFlux(catalog.Flux[isource]);
        shadow.set_thetaXYr(catalog.thetaxr[isource], catalog.thetayr[isource]);
        
        LOG(INFO) << "Calculating shadow for source " << catalog.sourceNameBAT[isource];
        //display shadow parameters
        shadow.display();
        //checking whether shadow parameters are in valid range
        if (shadow.check_shadow_par()) {
            LOG(ERROR) << "Shadow generation parameters not in valid range.";
            return (EXIT_FAILURE);
        }
        //compute shadow
        if (shadow.compute_shadow(fullNormalizedEffarea, fullfinermask)) {
            LOG(ERROR) << "Error in computing shadow.";
            return EXIT_FAILURE;
        }
        //getting shadow table and storing it in local variable
        shadowtab = shadow.get_shadow_table();
        //resetting shadow object to save memory
        shadow.reset_shadowtab();
        //SHADOW COMPUTATION END
        
        sfh.set_shadowtab(shadowtab);
        //writing shadow file for a particular shadow
        if (sfh.write_shadow_file(outfile, shadowExtnames[isource])) {
            LOG(ERROR) << "Error in writing SHADOW extension in output shadow file " << outfile;
            return EXIT_FAILURE;
        }

        //Updating keys
        string keysArray[] = {"RA", "DEC", "THETAX", "THETAY", "FLUX", "ESTART", "ESTOP", "NBINS"};
        float valuesFArray[] = {catalog.RA[isource] , catalog.DEC[isource],
                    shadowtab.thetaXr, shadowtab.thetaYr, shadowtab.sourceFlux, shadowtab.energyMin, shadowtab.energyMax,
                    (float) shadowtab.nBins};
        keys.assign(keysArray, keysArray + 8);
        valuesF.assign(valuesFArray, valuesFArray + 8);
        updateKeys(outfile, catalog.sourceNameBAT[isource], keys, valuesF, TFLOAT);  

        //keys updated
    }
    
    if (history == YES) {
        vector<string> vhistory;
        getHistory(vhistory);
        writeHistory(outfile, vhistory);
    }
    //UPDATING KEYWORDS
    updateKeywords(outfile, modulename);
    return EXIT_SUCCESS;
}

/**
 * Function to generate history for the module, to be written to the output file
 * @param par
 * @param vhistory
 * @return 
 */
int cztpsfshadowgen::getHistory(vector<string> &vhistory) {
	//char *user = getlogin();
    char *user = getenv("USER");
	string str = "Module run by " + (string) user;
    vhistory.push_back(str);
    vhistory.push_back("Parameter List START for " + (string) modulename);
    vhistory.push_back("P01 aspectfileQ0=" + (string) aspectfileQ0);
    vhistory.push_back("P02 aspectfileQ1=" + (string) aspectfileQ1);
    vhistory.push_back("P03 aspectfileQ2=" + (string) aspectfileQ2);
    vhistory.push_back("P04 aspectfileQ3=" + (string) aspectfileQ3);
    vhistory.push_back("P05 catalog=" + (string) catalogfile);
    vhistory.push_back("P06 extnumCatalog=" + itoa(extnumCatalog));
    vhistory.push_back("P07 cameraGeomFile=" + (string) cameraGeomFile);
    vhistory.push_back("P08 compMaskfile=" + (string) compMaskfile);
    vhistory.push_back("P09 effAreafile=" + (string) effectiveAreafile);
    vhistory.push_back("P10 nBins=" + itoa(nBins));
    vhistory.push_back("P11 minEnergy=" + itoa(minEnergy));
    vhistory.push_back("P12 maxEnergy=" + itoa(maxEnergy));
    vhistory.push_back("P13 include mask?=" + itoa(includeMask));
    vhistory.push_back("P14 mask oversampling factor=" + itoa(maskOversampling));
    vhistory.push_back("P15 fovx=" + itoa(fovx));
    vhistory.push_back("P16 fovy=" + itoa(fovy));
    vhistory.push_back("P17 outfile=" + (string) outfile);
    vhistory.push_back("P18 sourcelist=" + (string) sourcelist);

    if (clobber == YES)
        vhistory.push_back("P19 clobber=yes");
    else
        vhistory.push_back("P19 clobber=no");
    if (history == YES)
        vhistory.push_back("P20 history=yes");
    else
        vhistory.push_back("P20 history=no");
    if(debug==YES)
        vhistory.push_back("P21 debug=yes");
    else
        vhistory.push_back("P22 debug=no");
    vhistory.push_back("Parameter List END");
    return (EXIT_SUCCESS);
}
