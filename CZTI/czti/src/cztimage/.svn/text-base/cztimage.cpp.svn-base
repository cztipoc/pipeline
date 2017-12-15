#include "cztimage.h"

using namespace std;

cztimage::cztimage() {
    strcpy(modulename, "cztimage_v");
    strcat(modulename, VERSION);
}

cztimage::~cztimage() {

}

/* reading following values using pil from user:
 * 1. dpi file
 * 2. maskfile64x64
 * 3. maskfile
 * 4. shadowfile
 * 5. aspectfile
 * 6. catalogfile
 * 7. extnum                //for catalog file
 * 8. RA_colname            //for catalog file
 * 9. DEC_colname           //for catalog file
 * 10. FLUX_colname         //for catalog file
 * 6. outImgFile
 * 7. quad_no
 * 8. threshold
 * 9. oversampling factor
 * 10. sourcelist file name
 * 11. source strength threshold
 * 12. number of background coeffecients
 * 13. debugmode
 * 14. history
 * 15. clobber
 */
int cztimage::read(int argc, char **argv) {
    int status = 0;
    if (PIL_OK != (status = PILInit(argc, argv))) {
        LOG(ERROR) << "***Error Initializing PIL***";
        return status;
    }
    if (PIL_OK != (status = PILGetFname("par_dpifile", dpifile))) {
        LOG(ERROR) << "***Error reading input dpi file:" << dpifile << "***";
        return status;
    }
    if (PIL_OK != (status = PILGetFname("par_maskfile64x64", maskfile64x64))) {
        LOG(ERROR) << "***Error reading mask file with 64x64 pixels:" << maskfile64x64 << "***";
        return status;
    }

    if (PIL_OK != (status = PILGetFname("par_maskfile8100", maskfile8100))) {
        LOG(ERROR) << "***Error reading mask file:" << maskfile8100 << "***";
        return status;
    }

    if (PIL_OK != (status = PILGetFname("par_shadowfile", shadowfile))) {
        LOG(ERROR) << "***Error reading shadow file:" << shadowfile << "***";
        return status;
    }
    if (PIL_OK != (status = PILGetFname("par_aspectfileQ0", aspectfileQ0))) {
        LOG(ERROR) << "***Error reading aspect file:" << aspectfileQ0 << "***";
        return status;
    }
    if (PIL_OK != (status = PILGetFname("par_aspectfileQ1", aspectfileQ1))) {
        LOG(ERROR) << "***Error reading aspect file:" << aspectfileQ1 << "***";
        return status;
    }
    if (PIL_OK != (status = PILGetFname("par_aspectfileQ2", aspectfileQ2))) {
        LOG(ERROR) << "***Error reading aspect file:" << aspectfileQ2 << "***";
        return status;
    }
    if (PIL_OK != (status = PILGetFname("par_aspectfileQ3", aspectfileQ3))) {
        LOG(ERROR) << "***Error reading aspect file:" << aspectfileQ3 << "***";
        return status;
    }
    if (PIL_OK != (status = PILGetFname("par_catalogfile", catalogfile))) {
        LOG(ERROR) << "***Error reading catalog file:" << catalogfile << "***";
        return status;
    }
    if (PIL_OK != (status = PILGetInt("par_extnum", &extnum))) {
        LOG(ERROR) << "***Error Reading extension number for catalog file:" << catalogfile << "***";
        return status;
    }

    if (PIL_OK != (status = PILGetString("par_RAcolname", RA_colname))) {
        LOG(ERROR) << "***Error reading RA column name:***";
        return status;
    }

    if (PIL_OK != (status = PILGetString("par_DECcolname", DEC_colname))) {
        LOG(ERROR) << "***Error reading DEC column name:***";
        return status;
    }

    if (PIL_OK != (status = PILGetString("par_FLUXcolname", FLUX_colname))) {
        LOG(ERROR) << "***Error reading FLUX column name:***";
        return status;
    }
    if (PIL_OK != (status = PILGetFname("par_outImgFile", outImgFile))) {
        LOG(ERROR) << "***Error reading output file:" << outImgFile << "***";
        return status;
    }


    do {
        if (PIL_OK != (status = PILGetString("par_quadsToProcess", quadsToProcess))) {
            LOG(ERROR) << "***Error reading quadrant:" << quadsToProcess << "***";
            return status;
        }
    } while (isQuadrantNumbersValid(quadsToProcess));

    do {
        if (PIL_OK != (status = PILGetReal4("par_resolutionLimit", &resolutionLimit))) {
            LOG(ERROR) << "***Error reading resolution_limit***";
            return status;
        }
    } while (isResolutionLimitValid(resolutionLimit));

    if (PIL_OK != (status = PILGetReal4("par_threshold", &threshold))) {
        LOG(ERROR) << "***Error reading threshold***";
        return status;
    }



    do {
        if (PIL_OK != (status = PILGetInt("par_oversamplingfactor", &oversamplingfactor))) {
            LOG(ERROR) << "***Error reading oversampling factor***";
            return status;
        }
    } while (isOversamplingfactorValid(oversamplingfactor));


    if (PIL_OK != (status = PILGetFname("par_sourcelist", sourcelist))) {
        LOG(ERROR) << "***Error reading sourcelist file name***";
        return status;
    }
    if (PIL_OK != (status = PILGetReal4("par_sourceStrengthThr", &sourceStrengthThr))) {
        LOG(ERROR) << "***Error reading threshold for source strengths***";
        return status;
    }

    do {
        if (PIL_OK != (status = PILGetInt("par_nBkgdCoef", &nBkgdCoef))) {
            LOG(ERROR) << "***Error reading number of background coefficients --Allowed range is 1-6***";
            return status;
        }
    } while (isNBkgdCoefValid(nBkgdCoef));



    if (PIL_OK != (status = PILGetBool("par_debugmode", &debugmode))) {
        LOG(ERROR) << "***Error reading debugmode parameter***";

        return status;
    }
    if (PIL_OK != (status = PILGetBool("par_history", &history))) {
        LOG(ERROR) << "***Error reading history parameter***";
        return status;
    }
    if (PIL_OK != (status = PILGetBool("par_clobber", &clobber))) {
        LOG(ERROR) << "***Error Reading clobber***";
        return status;
    }
    PILClose(status);
    return (EXIT_SUCCESS);
}

int cztimage::read(char *dpifile, char *maskfile64x64, char *maskfile8100, char *shadowfile,
        char *aspectfile, char* catalogfile, int extnum, char* RA_colname, char* DEC_colname,
        char* FLUX_colname, float resolution_limit, float threshold, int oversamplingfactor, char *sourcelist, float source_strength_thr,
        char *outImgFile, char* quad_no, int nBkgdCoef, int debugmode, int clobber, int history) {
    strcpy(this->dpifile, dpifile);
    strcpy(this->maskfile64x64, maskfile64x64);
    strcpy(this->maskfile8100, maskfile8100);
    strcpy(this->shadowfile, shadowfile);
    strcpy(this->aspectfileQ0, aspectfile);
    strcpy(this->catalogfile, catalogfile);
    this->extnum = extnum;
    strcpy(this->RA_colname, RA_colname);
    strcpy(this->DEC_colname, DEC_colname);
    strcpy(this->FLUX_colname, FLUX_colname);
    this->resolutionLimit = resolution_limit;
    this->threshold = threshold;
    strcpy(this->sourcelist, sourcelist);
    strcpy(this->outImgFile, outImgFile);
    strcpy(this->quadsToProcess, quad_no);
    this->oversamplingfactor = oversamplingfactor;
    this->sourceStrengthThr = source_strength_thr;
    this->nBkgdCoef = nBkgdCoef;
    this->debugmode = debugmode;
    this->clobber = clobber;
    this->history = history;
    return (EXIT_SUCCESS);
}

void cztimage::display() {
    LOG(INFO) << "----------------------------------------------------------------------------";
    LOG(INFO) << "                          CZTIMAGE PARAMETERS                            ";
    LOG(INFO) << "----------------------------------------------------------------------------";
    LOG(INFO) << "Input DPI file                             :" << dpifile;
    LOG(INFO) << "Input Mask file                            :" << maskfile64x64;
    LOG(INFO) << "Compressed mask file                       :" << maskfile8100;
    LOG(INFO) << "Shadow file                                :" << shadowfile;
    LOG(INFO) << "Input Aspect file                          :" << aspectfileQ0;
    LOG(INFO) << "Catalog file                               :" << catalogfile;
    LOG(INFO) << "Extension number for database in catalog   :" << extnum;
    LOG(INFO) << "RA column name                             :" << RA_colname;
    LOG(INFO) << "DEC column name                            :" << DEC_colname;
    LOG(INFO) << "FLUX column name                           :" << FLUX_colname;
    LOG(INFO) << "Resolution limit                           :" << resolutionLimit;
    LOG(INFO) << "Threshold                                  :" << threshold;
    LOG(INFO) << "Oversampling factor                        :" << oversamplingfactor;
    LOG(INFO) << "Output file                                :" << outImgFile;
    LOG(INFO) << "Quadrants to be processed                  :" << quadsToProcess;
    LOG(INFO) << "No of background coef                      :" << nBkgdCoef;
    LOG(INFO) << "Source list filename                       :" << sourcelist;
    LOG(INFO) << "Source strength threshold                  :" << sourceStrengthThr;
    LOG(INFO) << "Debug Mode                                 :" << debugmode;
    LOG(INFO) << "Clobber                                    :" << clobber;
    LOG(INFO) << "History                                    :" << history;
    LOG(INFO) << "----------------------------------------------------------------------------";
}

int cztimage::addWCS() {

    LOG(INFO) << "--Adding WCS--";

    int status = 0; //status variable
    fitsfile *faspect, *fout; //fits file pointer to hold aspect file
    float RA = 0.0, DEC = 0.0, TWIST = 0.0; //to read RA, DEC and TWIST from aspect file
    float crval1=0.0, crval2=0.0, crota1=0.0, crota2=0.0, cd1_1=0.0, cd1_2=0.0, cd2_1=0.0, cd2_2=0.0;
    float pixel_pitch_rad=0.0, pixel_pitch_deg=0.0; //Pixel pitch is the increment in angle going from one pixel to the next at the
                            //center of the field
    
    float crpix = 0, cdelt = 0; //WCS RA DEC coordinate system
    float crpix1d = 0, cdelt1d = 0, crval1d = 0, crpix2d = 0, cdelt2d = 0, crval2d = 0; //Detector coordinate system
    float latpole;

    //Reading pertinent values from Aspect file
    fits_open_file(&faspect, aspectfileQ0, READONLY, &status);
    if (status) {
        LOG(ERROR) << "***Error opening file " << aspectfileQ0;
        return (EXIT_FAILURE);
    }

    fits_movnam_hdu(faspect, BINARY_TBL, "ASPECT", NULL, &status);
    if (status) {
        LOG(ERROR) <<"Error in moving to ASPECT HDU in " << aspectfileQ0;
        fits_report_error(stderr, status);
        return (EXIT_FAILURE);
    }
    fits_read_key(faspect, TFLOAT, "RA", &RA, NULL, &status);
    if (status) {
        LOG(ERROR) << "Error in reading RA keyword from header of " << aspectfileQ0;
        fits_report_error(stderr, status);
        return (EXIT_FAILURE);
    }

    fits_read_key(faspect, TFLOAT, "DEC", &DEC, NULL, &status);
    if (status) {
        LOG(ERROR) << "Error in reading DEC keyword from header of " << aspectfileQ0;
        fits_report_error(stderr, status);
        return (EXIT_FAILURE);
    }

    fits_read_key(faspect, TFLOAT, "TWIST", &TWIST, NULL, &status);
    if (status) {
        LOG(ERROR) << "Error in reading TWIST keyword from header of following " << aspectfileQ0;
        fits_report_error(stderr, status);
        return (EXIT_FAILURE);
    }

    fits_close_file(faspect, &status);
    if (status) {
        fits_report_error(stderr, status);
        return (EXIT_FAILURE);
    }
    //Pertinent values read.
    cout<<"*****************************************HERE***************************"<; 
    cout<<"RA VALUE::::::::::"<<RA; 
    cout<<"DEC VALUE::::::::::"<<DEC; 
    cout<<"TWIST VALUE::::::::::"<<TWIST; 
    crval1=RA;
    crval2=DEC;
    crota1=-TWIST;
    crota2=-TWIST;
    pixel_pitch_rad=atan(AVG_PIXEL_WIDTH/(MASKHEIGHT*oversamplingfactor));
    pixel_pitch_deg = pixel_pitch_rad * 180/M_PI;
    cd1_1=cos(TWIST*M_PI/180)*pixel_pitch_deg;
    cd1_2=-sin(TWIST*M_PI/180)*pixel_pitch_deg;
    cd2_1=sin(TWIST*M_PI/180)*pixel_pitch_deg;
    cd2_2=cos(TWIST*M_PI/180)*pixel_pitch_deg;

    //adding WCS keywords to both synthetic and actual image files 
   fits_open_file(&fout, outImgFile, READWRITE, &status);
    if (status) {
        LOG(ERROR) << "***Error in opening file " << outImgFile << "***";
        return (EXIT_FAILURE);
    }
    fits_write_key(fout, TSTRING, "CTYPE1", (char *) "RA---TAN", "", &status);
    if (status) {
        fits_report_error(stderr, status);
        return (EXIT_FAILURE);
    }   
    fits_write_key(fout, TSTRING, "CTYPE2", (char *) "DEC--TAN", "", &status);
    if (status) {
        fits_report_error(stderr, status);
        return (EXIT_FAILURE);
    }
    fits_write_key(fout, TFLOAT, "CRVAL1", &crval1, "", &status);
    if (status) {
        fits_report_error(stderr, status);
        return (EXIT_FAILURE);
    } 

    fits_write_key(fout, TFLOAT, "CRVAL2", &crval2, "", &status);
    if (status) {
        fits_report_error(stderr, status);
        return (EXIT_FAILURE);
    }
    
    fits_write_key(fout, TFLOAT, "CROTA1", &crota1, "", &status);
    if (status) {
        fits_report_error(stderr, status);
        return (EXIT_FAILURE);
    }
    
    fits_write_key(fout, TFLOAT, "CROTA2", &crota2, "", &status);
    if (status) {
        fits_report_error(stderr, status);
        return (EXIT_FAILURE);
    }
    fits_write_key(fout, TFLOAT, "CD1_1", &cd1_1, "", &status);
    if (status) {
        fits_report_error(stderr, status);
        return (EXIT_FAILURE);
    }
    fits_write_key(fout, TFLOAT, "CD1_2", &cd1_2, "", &status);
    if (status) {
        fits_report_error(stderr, status);
        return (EXIT_FAILURE);
    }
    fits_write_key(fout, TFLOAT, "CD2_1", &cd2_1, "", &status);
    if (status) {
        fits_report_error(stderr, status);
        return (EXIT_FAILURE);
    }
    fits_write_key(fout, TFLOAT, "CD2_2", &cd2_2, "", &status);
    if (status) {
        fits_report_error(stderr, status);
        return (EXIT_FAILURE);
    }
    

    
    /*
    crpix = (XSIZE * oversamplingfactor / NUMQUAD) / 2;
    cdelt = atan((double) DETECTORWIDTH / (float) (MASKHEIGHT * NO_PIX_X_PER_DET * oversamplingfactor));
    cdelt = cdelt * 180 / M_PI;
    LOG(INFO) << "CDELT:" << cdelt;
    LOG(INFO) << "Val:" << (float) DETECTORWIDTH / (float) (MASKHEIGHT * NO_PIX_X_PER_DET * oversamplingfactor);

     

   
    fits_open_file(&fout, outImgFile, READWRITE, &status);
    if (status) {
        LOG(ERROR) << "***Error in opening file " << outImgFile << "***";
        return (EXIT_FAILURE);
    }

    fits_write_key(fout, TSTRING, "CTYPE1", (char *) "RA---TAN", "", &status);
    if (status) {
        fits_report_error(stderr, status);
        return (EXIT_FAILURE);
    }
    fits_write_key(fout, TFLOAT, "CRPIX1", &crpix, "", &status);
    if (status) {
        fits_report_error(stderr, status);
        return (EXIT_FAILURE);
    }
    fits_write_key(fout, TFLOAT, "CDELT1", &cdelt, "", &status);
    if (status) {
        fits_report_error(stderr, status);
        return (EXIT_FAILURE);
    }
    fits_write_key(fout, TFLOAT, "CRVAL1", &RA, "", &status);
    if (status) {
        fits_report_error(stderr, status);
        return (EXIT_FAILURE);
    }
    fits_write_key(fout, TSTRING, "CTYPE2", (char *) "DEC--TAN", "", &status);
    if (status) {
        fits_report_error(stderr, status);
        return (EXIT_FAILURE);
    }
    fits_write_key(fout, TFLOAT, "CRPIX2", &crpix, "", &status);
    if (status) {
        fits_report_error(stderr, status);
        return (EXIT_FAILURE);
    }
    fits_write_key(fout, TFLOAT, "CDELT2", &cdelt, "", &status);
    if (status) {
        fits_report_error(stderr, status);
        return (EXIT_FAILURE);
    }
    fits_write_key(fout, TFLOAT, "CRVAL2", &DEC, "", &status);
    if (status) {
        fits_report_error(stderr, status);
        return (EXIT_FAILURE);
    }
    fits_write_key(fout, TFLOAT, "CROTA2", &TWIST, "", &status);
    if (status) {
        fits_report_error(stderr, status);
        return (EXIT_FAILURE);
    }
    */
    
    
    /* Detector coordinates [We'll write them later once they are verified.]
    // Writing detector coordinates (SUBJECTED TO ERROR VERFIY IT LATER...)
    crpix1d = (XSIZE * oversamplingfactor / NUMQUAD) / 2;
    crpix2d = (XSIZE * oversamplingfactor / NUMQUAD) / 2;
    cdelt1d = 1;
    cdelt2d = 1;
    crval1d = 0;
    crval2d = 0;
    latpole =90;
    fits_write_key(fout, TSTRING, "CTYPE1d", (char *) "DETECTOR_X", "", &status);
    if (status) {
        fits_report_error(stderr, status);
        return (EXIT_FAILURE);
    }
    fits_write_key(fout, TFLOAT, "CRPIX1d", &crpix1d, "", &status);
    if (status) {
        fits_report_error(stderr, status);
        return (EXIT_FAILURE);
    }

    fits_write_key(fout, TFLOAT, "CDELT1d", &cdelt1d, "", &status);
    if (status) {
        fits_report_error(stderr, status);
        return (EXIT_FAILURE);
    }
    fits_write_key(fout, TFLOAT, "CRVAL1d", &crval1d, "", &status);
    if (status) {
        fits_report_error(stderr, status);
        return (EXIT_FAILURE);
    }
    fits_write_key(fout, TSTRING, "CTYPE1d", (char *) "DETECTOR_y", "", &status);
    if (status) {
        fits_report_error(stderr, status);
        return (EXIT_FAILURE);
    }
    fits_write_key(fout, TFLOAT, "CRPIX1d", &crpix2d, "", &status);
    if (status) {
        fits_report_error(stderr, status);
        return (EXIT_FAILURE);
    }
    fits_write_key(fout, TFLOAT, "CDELT1d", &cdelt2d, "", &status);
    if (status) {
        fits_report_error(stderr, status);
        return (EXIT_FAILURE);
    }
    fits_write_key(fout, TFLOAT, "CRVAL1d", &crval2d, "", &status);
    if (status) {
        fits_report_error(stderr, status);
        return (EXIT_FAILURE);
    }
    
    fits_write_key(fout, TFLOAT, "LATPOLE", &latpole, "", &status);
    if (status) {
        fits_report_error(stderr, status);
        return (EXIT_FAILURE);
    }
    // Detector coordinates written
    
    */

    fits_close_file(fout, &status);
    if (status) {
        fits_report_error(stderr, status);
        return (EXIT_FAILURE);
    }

    LOG(INFO) << "--WCS added--";

    return (EXIT_SUCCESS);
}

int cztimage::cztimageProcess() {

    LOG(INFO) << "-- Processing of image started --";

    /* Variable declaration*/
    int i = 0, j=0; //counter variables
    int status = 0; //status variable


    /* Variable declaration completed*/

    /******************Creating Image from DPI********************************/
    //deleting output image file if it already exisits.
    if (FileExists(outImgFile)) {
        if (clobber == YES) {
            unlink(outImgFile);
        } else {
            LOG(ERROR) << outImgFile << "  already exists";
            LOG(ERROR) << "Use clobber=yes for overwriting the file";
            return (EXIT_FAILURE);
        }
    }
    
    //getting complete path for CALDB files.
    maskfile64x64path = caldb_full_path_generator(maskfile64x64);
    if(maskfile64x64path==""){
        LOG(ERROR)<< "Not able to generate Mask File (64x64) path";
        return(EXIT_FAILURE);
    }
    catalogFilePath = caldb_full_path_generator(catalogfile);
    if (catalogFilePath == "") {
        LOG(ERROR) << "Not able to generate Catalog File path.";
        return (EXIT_FAILURE);
    }   
    maskfile8100path = caldb_full_path_generator(maskfile8100);
    if(maskfile8100path==""){
        LOG(ERROR)<< "Not able to generate Mask File (8175x8175) path";
        return(EXIT_FAILURE);
    }    
    //Complete path generated

    mask = new float[YSIZE * XSIZE];
    dpi = new float[YSIZE * XSIZE];

    //initializing mask array to contain zeros
    for (i = 0; i <= YSIZE * XSIZE; i++) {
        mask[i] = 0;
        dpi[i] = 0;
    }

    if (getMaskArray()) {
        LOG(ERROR) << "Error in reading mask";
        return (EXIT_FAILURE);
    }
    if (getDPI()) {
        LOG(ERROR) << "Error in reading DPI";
        return (EXIT_FAILURE);
    }

    skyimgsize = XSIZE * oversamplingfactor / 4; //size of the sky image created
    size = XSIZE*oversamplingfactor;
    rawskyimage = new float[size * size];

    if (rawskyimage == NULL) {
        LOG(ERROR) << "Out of memory error";
        return (EXIT_FAILURE);
    }

    for (i = 0; i < size * size; i++) {
        rawskyimage[i] = 0;
    }
    
    
    oversample(); //to oversample dpi and mask

    generateCorrelationMatrix_2d(oversampleddpi, oversampledmask, rawskyimage, YSIZE, XSIZE, oversamplingfactor);
    keepCentral(rawskyimage, YSIZE, XSIZE, oversamplingfactor, &skyimgsize); //retains 1/4th area from centre
    //raw sky image created and stored in rawskyimage array               

    fits_create_file(&fout, outImgFile, &status);
    if (status) {
        LOG(ERROR) << "***Error in output image file creation***";
        fits_report_error(stderr, status);
        return (EXIT_FAILURE);
    }


    writeSkyImage(); //writing image data to output files

    fits_close_file(fout, &status);
    
    /**************Image from DPI created********************************/


    if (addWCS()) {
        LOG(ERROR) << "***Error in adding WCS to output files***";
        return (EXIT_FAILURE);
    }

    //updating keywords in output file
    if (updateKeywords(outImgFile, modulename)) {
        LOG(ERROR) << "***Error in updating keywords***";
        return (EXIT_FAILURE);
    }

    //findKnownSources(catalogfile, aspectfileQ0, extnum, RA_colname, DEC_colname, FLUX_colname, RA_ks, DEC_ks, FLUX_ks, thetax_ks, thetay_ks); //RA, DEC, FLUX, thetax and thetay of known sources.

    if(identifyPeaks()){
        return(EXIT_FAILURE);
    }
    LOG(INFO)<< ">>>>>>>>>>>> IDENIFIED PEAKS <<<<<<<<<<<<<<";
    displayPeaks();
    LOG(INFO)<< ">>>>>>>> KNOWN SOURCES <<<<<<<<<<";
    displayKnownSources();
    
    int size_knownSources = RA_ks.size();
    int size_identifiedPeaks = ra.size();

    LOG(INFO) << "Matching peaks in known and identified sources.";
    for(i=0; i<size_knownSources; i++){
        if (size_identifiedPeaks==0){
            ra.push_back(RA_ks[i]); dec.push_back(DEC_ks[i]); flux.push_back(FLUX_ks[i]); thetax.push_back(thetax_ks[i]); thetay.push_back(thetay_ks[i]);
        }
        for(j=0; j< size_identifiedPeaks; j++){
            if ( (thetax_ks[i]> thetax[j]+angleConverter(resolutionLimit, "arcmin", "radians")) || (thetax_ks[i]<thetax[j]-angleConverter(resolutionLimit, "arcmin", "radians")) ){
                ra.push_back(RA_ks[i]); dec.push_back(DEC_ks[i]); flux.push_back(FLUX_ks[i]); thetax.push_back(thetax_ks[i]); thetay.push_back(thetay_ks[i]);
            }
            else if((thetay_ks[i]> thetay[j]+angleConverter(resolutionLimit, "arcmin", "radians")) || (thetay_ks[i]<thetay[j]-angleConverter(resolutionLimit, "arcmin", "radians"))){
                ra.push_back(RA_ks[i]); dec.push_back(DEC_ks[i]); flux.push_back(FLUX_ks[i]); thetax.push_back(thetax_ks[i]); thetay.push_back(thetay_ks[i]);
            }
        }
    }
    

    LOG(INFO) << "Criterion for matching is: Resolution limit of " << resolutionLimit << " arcminutes i.e. " << angleConverter(resolutionLimit, "arcmin", "radians") << " radians";
    displayPeaks();

    /***SVD FITTING STARTED ***/
    nsources = ra.size();
    LOG(INFO) << "NSOURCES FOUND: "<< nsources; //debug
    LOG(INFO) << "-- SVD fitting started--";

    //  >START generating shadows for known sources
    shadows = allocateMemory<float>(nsources, XSIZE * YSIZE); //buffer to store shadows of all the peaks and identified sources in the field of view.
    if (shadows == NULL) {
        LOG(ERROR) << "Out of memory error";
        return (EXIT_FAILURE);
    }


    for (i = 0; i < nsources; i++) {
        LOG(INFO) << "Generating shadow for source in FOV with following camera coordinates: thetax-" << thetax[i] << "  thetay-" << thetay[i];
        MaskWeighting mskwt((char*)maskfile8100path.c_str(), thetax[i], thetay[i]);
        if (mskwt.run()) {
            LOG(ERROR) << "****Error in mask Weighting****";
            return (EXIT_FAILURE);
        }
        mskwt.getShadow(shadows[i], XSIZE * YSIZE);
    }

    if (debugmode == YES) {
        for (int i = 0; i < nsources; i++) {
            char name[512];
            sprintf(name, "shadow_%d", i);
            ofstream fb(name, ios::binary);
            fb.write((char *) shadows[i], sizeof (float)*XSIZE * YSIZE);
            fb.close();
        }
    }

    //  >END shadows for known and identified sources generated and stored in variable shadows[i][XSIZE*YSIZE]

    //  >START generating background shadows based on coefficied provided by user.

    bkgdshadows = allocateMemory<float>(nBkgdCoef, XSIZE * YSIZE);
    getBkgdShadows();

    if (debugmode == YES) {
        unsigned char* UC_bkgd;
        UC_bkgd = new unsigned char[XSIZE*YSIZE];
        char name[512], jpegName[512];
        for (int i = 0; i < nBkgdCoef; i++) {
            
            sprintf(name, "bkgd_%d", i);
            ofstream fb(name, ios::binary);
            fb.write((char *) bkgdshadows[i], sizeof (float)*XSIZE * YSIZE);
            fb.close();
            
            sprintf(jpegName, "bkgd_%d.jpeg", i);
            scaleDownToUC<float>(bkgdshadows[i], UC_bkgd, YSIZE, XSIZE);
            makeJpeg(UC_bkgd, XSIZE, YSIZE, 1, JCS_GRAYSCALE, 100, jpegName);
        }
        delete[] UC_bkgd;
        
        
        
        
    }
    //  >END background shadows generated and stored in bkgdshadows[i][XSIZE*YSIZE]

       LOG(INFO)<<"Fitting sources......";
   int prev_peaks=nsources,prev_nbkgd=nBkgdCoef;
    while(1){
        if ((prev_peaks==0 && prev_nbkgd==0)) {
            break;
        }
        if(linearfitDPI()){
                LOG(ERROR)<<"***Error in linearfitDPI***";
                return (EXIT_FAILURE);
        }
        
        if((prev_peaks==nsources && prev_nbkgd==nBkgdCoef))            break;
        prev_peaks=nsources;  prev_nbkgd=nBkgdCoef;
        LOG(INFO)<<"prev_peaks:"<<prev_peaks<<"     prev_nbkgd:"<<prev_nbkgd;
    }
    LOG(INFO)<<"Number of sources:  "<<nsources;
    LOG(INFO)<<"Number of background:"<<nBkgdCoef;
    LOG(INFO)<<"Fitting Complete";
   
 //adding sources to sourcelist
    ofstream fout(sourcelist);
    if(!fout){
        LOG(ERROR)<<"***Error in opening file "<<sourcelist<<"***";
        return (EXIT_FAILURE);
    }
    
    //finding RA and DEC for sources found
    double ra_rad,dec_rad;              //for radians
    ra.clear();  dec.clear();
    fout<<setw(15)<<"RA"<<setw(15)<<"DEC"<<setw(15)<<"FLUX";
    for(int i=0;i<thetax.size();i++){
        getRaDec(aspectfileQ0,thetax[i],thetay[i],&ra_rad,&dec_rad);
        ra.push_back(ra_rad);
        dec.push_back(dec_rad);
        fout<<endl<<setw(15)<<ra[i]<<setw(15)<<dec[i]<<setw(15)<<flux[i]<<setw(15)<<thetax[i]<<setw(15)<<thetay[i];
        cout<<endl<<setw(15)<<ra[i]<<setw(15)<<dec[i]<<setw(15)<<flux[i]<<setw(15)<<thetax[i]<<setw(15)<<thetay[i];
    }
    fout.close(); 
    
    writeSyntheticImage();
    
    /***SVD FITTING COMPLETED***/
    //delete[] mask,dpi,oversampled_dpi,oversampled_mask;     
    //writing history to output file
    if (history == YES) {
        vector<string> vhistory;
        getHistory(vhistory);
        writeHistory(outImgFile, vhistory); //writes history to each HDU of output file


    }

    delete[] mask, dpi, oversampleddpi, oversampledmask, rawskyimage, nobkgdskyimage;
    return (EXIT_SUCCESS);
}

//Assumption - The Mask Image file contains dpi in 4 quadrants ie. extension no 2,3,4,5
//Each quadrant mask is of size 64 x 64

int cztimage::getMaskArray() {
    fitsfile *fptr;
    int status = 0;
    int npixQuad = XSIZE * YSIZE / NUMQUAD;
    int hduid=0;
    float *quadmask = new float[npixQuad];

    /*code to process quadrants*/
    int* quadArray;
    int noccurrence;
    stringFinder(quadsToProcess, ",", 0, &noccurrence);
    int nelems_quad = noccurrence + 1;
    quadArray = new int[nelems_quad];

    if (csvStrToInt(quadsToProcess, ",", quadArray, &nelems_quad)) {
        LOG(ERROR) << "***Error in converting quadrant string array into integer array***";
    }
    /*code to process quadrants*/






    fits_open_file(&fptr, (char*) maskfile64x64path.c_str(), READONLY, &status);
    if (status != 0) {
        LOG(ERROR) << "***Error opening mask file***";
        fits_report_error(stderr, status);
        return (EXIT_FAILURE);
        ;
    }

    int hdutype = 0, qid, r, c, index = 0;
    long fpixel[2];
    fpixel[0] = 1;
    fpixel[1] = 1;
    for (int i = 0; i < NUMQUAD; i++) {
        fits_movabs_hdu(fptr, i+2, &hdutype, &status);
        if (status != 0) {
            LOG(ERROR) << "***Error moving HDU***";
            fits_report_error(stderr, status);
            return (EXIT_FAILURE);
            ;
        }
        fits_read_key(fptr, TINT, "QUADID", &qid, NULL, &status);
        if (status) {
            fits_report_error(stderr, status);
            return (EXIT_FAILURE);
        }
        if (hdutype != IMAGE_HDU) {
            LOG(ERROR) << "Image HDU not found in mask file at HDU no " << i + 2;
            return (EXIT_FAILURE);
        }
        if (hdutype == IMAGE_HDU && qid == i) {
            fits_read_pix(fptr, TFLOAT, fpixel, npixQuad, NULL, quadmask, NULL, &status);
            if (status != 0) {
                LOG(ERROR) << "***Error reading mask pixels***";
                fits_report_error(stderr, status);
                return (EXIT_FAILURE);
                ;
            }
            r = (qid / NUMQUAD_ROW)*(YSIZE / NUMQUAD_COL);
            c = (qid % NUMQUAD_ROW)*(XSIZE / NUMQUAD_ROW);
            index = 0;
            for (int u = r; u < r + (YSIZE / NUMQUAD_COL); u++)
                for (int v = c; v < c + (XSIZE / NUMQUAD_ROW); v++){
                    mask[u * XSIZE + v] = quadmask[index++];
//                    LOG(INFO) << "MASK VALUE::" << mask[u*XSIZE+v];
                }
                    
        }
    }
    fits_close_file(fptr, &status);
    if (status != 0) {
        LOG(ERROR) << "***Error closing maskfile " << maskfile64x64path << " ***";
        fits_report_error(stderr, status);
        return (EXIT_FAILURE);
        ;
    }
    return (EXIT_SUCCESS);
}

//Assumption - The DPI Image file contains dpi in 4 quadrants ie. extension no 2,3,4,5
//Each quadrant DPI is 64 x 64

int cztimage::getDPI() {
    fitsfile *fptr;
    int status = 0;
    int npixQuad = XSIZE * YSIZE / NUMQUAD;
    long *quaddpi = new long[npixQuad];
    int hduid=0;
    /*code to process quadrants*/
    int* quadArray;
    int noccurrence;
    stringFinder(quadsToProcess, ",", 0, &noccurrence);
    int nelems_quad = noccurrence + 1;
    quadArray = new int[nelems_quad];

    if (csvStrToInt(quadsToProcess, ",", quadArray, &nelems_quad)) {
        LOG(ERROR) << "***Error in converting quadrant string array into integer array***";
    }
    /*code to process quadrants*/
    
    fits_open_file(&fptr, dpifile, READONLY, &status);
    if (status != 0) {
        LOG(ERROR) << "***Error opening DPI file***";
        fits_report_error(stderr, status);
        return (EXIT_FAILURE);
        ;
    }

    int hdutype = 0, qid, r, c, index = 0;
    long fpixel[2];
    fpixel[0] = 1;
    fpixel[1] = 1;
    for (int i = 0; i < NUMQUAD; i++) {;
        fits_movabs_hdu(fptr, i+2, &hdutype, &status);
        if (status != 0) {
            LOG(ERROR) << "***Error moving HDU***";
            fits_report_error(stderr, status);
            return (EXIT_FAILURE);
            ;
        }
        fits_read_key(fptr, TINT, "QUADID", &qid, NULL, &status);
        if (hdutype != IMAGE_HDU) {
            LOG(ERROR) << "Image HDU not found in DPI file at HDU no " << i + 2;
            return (EXIT_FAILURE);
        }
        if (qid == i) {
            fits_read_pix(fptr, TLONG, fpixel, npixQuad, NULL, quaddpi, NULL, &status);
            if (status != 0) {
                LOG(ERROR) << "***Error reading DPI pixels***";
                fits_report_error(stderr, status);
                return (EXIT_FAILURE);
            }
            r = (qid / NUMQUAD_ROW)*(YSIZE / NUMQUAD_COL);
            c = (qid % NUMQUAD_ROW)*(XSIZE / NUMQUAD_ROW);
            index = 0;
            for (int u = r; u < r + (YSIZE / NUMQUAD_COL); u++)
                for (int v = c; v < c + (XSIZE / NUMQUAD_ROW); v++){
                    dpi[u * XSIZE + v] = (float) quaddpi[index++];
//                    LOG(INFO) << "DPI VALUE::" << dpi[u * XSIZE + v];
                }
            
        }
    }
    fits_close_file(fptr, &status);
    if (status) {
        LOG(ERROR) << "***Error closing dpifile " << dpifile << " ***";
        fits_report_error(stderr, status);
        return (EXIT_FAILURE);
        ;
    }
    return (EXIT_SUCCESS);
}

void cztimage::oversample() {
    LOG(INFO) << "---Oversampling of DPI and mask started---";
    size = XSIZE* oversamplingfactor; //oversampled rows/columns
    oversampleddpi = new float[size * size];
    oversampledmask = new float[size * size];
    int x, y;
    LOG(INFO) << "Oversampled size: " << size << " x " << size;
    for (int i = 0; i < YSIZE; i++)
        for (int j = 0; j < XSIZE; j++)
            for (int k = i * oversamplingfactor; k < i * oversamplingfactor + oversamplingfactor; k++)
                for (int l = j * oversamplingfactor; l < j * oversamplingfactor + oversamplingfactor; l++) {
                    oversampleddpi[k * size + l] = (float) dpi[i * XSIZE + j] / (float) (oversamplingfactor * oversamplingfactor);
                    oversampledmask[k * size + l] = (float) mask[i * XSIZE + j];
                }
    LOG(INFO) << "---Oversampling of DPI and Mask completed---";
}

void doFft(float data[], unsigned long nn, int isign) {
    unsigned long n, mmax, m, j, istep, i;
    double wtemp, wr, wpr, wpi, wi, theta;
    float tempr, tempi;

    n = nn << 1;
    j = 1;
    for (i = 1; i < n; i += 2) {
        if (j > i) {
            SWAP(data[j], data[i]);
            SWAP(data[j + 1], data[i + 1]);
        }
        m = nn;
        while (m >= 2 && j > m) {
            j -= m;
            m >>= 1;
        }
        j += m;
    }
    mmax = 2;
    while (n > mmax) {
        istep = mmax << 1;
        theta = isign * (2 * M_PI / mmax);
        wtemp = sin(0.5 * theta);
        wpr = -2.0 * wtemp*wtemp;
        wpi = sin(theta);
        wr = 1.0;
        wi = 0.0;
        for (m = 1; m < mmax; m += 2) {
            for (i = m; i <= n; i += istep) {
                j = i + mmax;
                tempr = wr * data[j] - wi * data[j + 1];
                tempi = wr * data[j + 1] + wi * data[j];
                data[j] = data[i] - tempr;
                data[j + 1] = data[i + 1] - tempi;
                data[i] += tempr;
                data[i + 1] += tempi;
            }
            wr = (wtemp = wr) * wpr - wi * wpi + wr;
            wi = wi * wpr + wtemp * wpi + wi;
        }
        mmax = istep;
    }
}

//void generateCorrelationMatrix_2d(float *dph, float *mask, float *correlationMatrix,int TOTALROWS, int TOTALCOLS, int factor)
void generateCorrelationMatrix_2d(float *dph,float *mask,float *correlationMatrix,int TOTALROWS,int TOTALCOLS,int sampleCount )
{


	int n=TOTALCOLS*TOTALROWS*sampleCount*sampleCount,fftN=0,k=0,ii=0,jj=0,i=0,j=0;
	int n_=TOTALROWS*sampleCount;
	float *dph_complex,*phase_complex,*correlation_complex,f,*mask_complex,*tmp_complex;
	FILE*fp;
	float xshift=2*M_PI*sampleCount*2,fy=0,fx=0,yshift=2*M_PI*sampleCount*0;

	fftN=n*2;

	dph_complex=(float*)malloc(sizeof(float)*(fftN+1));
	phase_complex=(float*)malloc(sizeof(float)*(fftN+1));
	correlation_complex=(float*)malloc(sizeof(float)*(fftN+1));
	mask_complex=(float*)malloc(sizeof(float)*(fftN+1));
	tmp_complex=(float*)malloc(sizeof(float)*(fftN+1));


	for(i=1,j=0;i<=fftN;i+=2,j++)
	{
		dph_complex[i]=dph[j];
		dph_complex[i+1]=0;
		mask_complex[i]=mask[j];
		mask_complex[i+1]=0;
		correlation_complex[i]=0;
		correlation_complex[i+1]=0;
	}

	phase_complex[1]=1;
	phase_complex[2]=0;	
	k=3;
	for(i=1;i<n/2;i++)
	{
		ii=i/n_;
		jj=i%n_;
		fx=(ii/(n_*1.0))*xshift;
		fy=(jj/(n_*1.0))*yshift;
		phase_complex[k]=(cos(fx)*cos(fy)) - (sin(fx)*sin(fy)) ;
		phase_complex[k+1]=(sin(fx)*cos(fy)) + (cos(fx)*sin(fy));
		k+=2;

	}
	for(i=-n/2;i<0;i++)
	{	
		ii=i/n_;
		jj=i%n_;
		fx=(ii/(n_*1.0))*xshift;
		fy=(jj/(n_*1.0))*yshift;
		phase_complex[k]=(cos(fx)*cos(fy)) - (sin(fx)*sin(fy)) ;
		phase_complex[k+1]=(sin(fx)*cos(fy)) + (cos(fx)*sin(fy));
		k+=2;

	}
	doFft(dph_complex,n,1);
	doFft(mask_complex,n,1);
	for(i=1;i<=fftN;i+=2)
	{
		tmp_complex[i]=(mask_complex[i]*dph_complex[i])+(mask_complex[i+1]*dph_complex[i+1]);
		tmp_complex[i+1]=(mask_complex[i+1]*dph_complex[i])-(mask_complex[i]*dph_complex[i+1]);
		correlation_complex[i]=(tmp_complex[i]*phase_complex[i])-(tmp_complex[i+1]*phase_complex[i+1]);
		correlation_complex[i+1]=(tmp_complex[i+1]*phase_complex[i])+(tmp_complex[i]*phase_complex[i+1]);
		correlation_complex[i]/=n;
		correlation_complex[i+1]/=n;
	}
	doFft(correlation_complex,n,-1);
	for(i=1,j=0;i<=fftN;i+=2,j++)
	{

		correlationMatrix[j]=sqrt((correlation_complex[i]*correlation_complex[i])+(correlation_complex[i+1]*correlation_complex[i+1]));
	}
	swapComplex(correlationMatrix);
	free(dph_complex);
    free(mask_complex);
    free(tmp_complex);
}


/*
void generateCorrelationMatrix_2d(float *dph, float *mask, float *correlationMatrix,int TOTALROWS, int TOTALCOLS, int factor)
	   	{
    LOG(INFO) << "--Starting correlation matrix generation--";

    int n = TOTALROWS * TOTALCOLS * factor*factor;
    int fftN = 2 * n;
    int i = 0, j = 0;
    float *dph_complex, *mask_complex, *correlation_complex;
    dph_complex = (float*) malloc(sizeof (float)*(fftN + 1));
    mask_complex = (float*) malloc(sizeof (float)*(fftN + 1));
    correlation_complex = (float*) malloc(sizeof (float)*(fftN + 1));
    for (i = 1, j = 0; i <= fftN; i++) {
        if (i % 2 == 0) {
            dph_complex[i] = dph[j];
            mask_complex[i] = mask[j];
            j++;
        } else {
            dph_complex[i] = 0;
            mask_complex[i] = 0;
        }
    }

    LOG(INFO) << "Taking fft of dph_complex";
    doFft(dph_complex, n, 1);
    LOG(INFO) << "Taking fft of mask_complex";
    doFft(mask_complex, n, 1);

    for (i = 1; i <= fftN; i += 2) {
        correlation_complex[i] = (mask_complex[i] * dph_complex[i])+(mask_complex[i + 1] * dph_complex[i + 1]); // to be altered and checked
        correlation_complex[i + 1] = (mask_complex[i + 1] * dph_complex[i])-(mask_complex[i] * dph_complex[i + 1]); // to be altered and checked

        correlation_complex[i] /= n;
        correlation_complex[i + 1] /= n;
    }

    LOG(INFO) << "Taking fft of correlation_complex";
    doFft(correlation_complex, n, -1);
    for (i = 1, j = 0; i <= fftN; i += 2, j++) {
        correlationMatrix[j] = sqrt((correlation_complex[i] * correlation_complex[i])+(correlation_complex[i + 1] * correlation_complex[i + 1]));
    }
    swapComplex(correlationMatrix, TOTALROWS, TOTALCOLS, factor);
    free(dph_complex);
    free(mask_complex);

    LOG(INFO) << "--Correlation matrix generated--";
    return;
}
*/



void swapComplex(float *data, int TOTALROWS, int TOTALCOLS, int factor) {
    float temp;
    int i = 0, ii = 0, j = 0, jj = 0, n = (TOTALROWS * factor) / 2;
    int cols = TOTALCOLS*factor;
    for (i = 0, ii = n; i < n; i++, ii++) {
        for (j = 0, jj = n; j < n; j++, jj++) {
            temp = data[i * TOTALCOLS * factor + j];
            data[i * TOTALCOLS * factor + j] = data[ii * TOTALCOLS * factor + jj];
            data[ii * TOTALCOLS * factor + jj] = temp;

        }
    }
    for (i = n, ii = 0; i < n + n; i++, ii++) {
        for (j = 0, jj = n; j < n; j++, jj++) {
            temp = data[i * TOTALCOLS * factor + j];
            data[i * TOTALCOLS * factor + j] = data[ii * TOTALCOLS * factor + jj];
            data[ii * TOTALCOLS * factor + jj] = temp;
        }
    }
}

int keepCentral(float *correlationMatrix, int TOTALROWS, int TOTALCOLS, int factor, int *centralsize) {
    float*temp;
    int i = 0, j = 0, ii = 0, jj = 0;
    int totalElements = TOTALROWS*factor;
    int centralCount = totalElements / 4;
    int startIndex = (totalElements / 2)-(centralCount / 2);
    int endIndex = startIndex + centralCount;
    temp = new float[TOTALROWS * TOTALCOLS * factor * factor];
    if (temp == NULL) {
        printf("\tError(%s:%d):Error while allocating memory", __FILE__, __LINE__);
        return (EXIT_FAILURE);
    }
    for (i = 0; i < TOTALCOLS * TOTALROWS * factor * factor; i++) {
        temp[i] = correlationMatrix[i];
        correlationMatrix[i] = 0;
    }
    for (i = startIndex, ii = 0; i < endIndex; i++, ii++)
        for (j = startIndex, jj = 0; j < endIndex; j++, jj++)
            correlationMatrix[ii * centralCount + jj] = temp[i * TOTALCOLS * factor + j];

    *centralsize = centralCount;
    delete[] temp;
    return (EXIT_SUCCESS);
}

int cztimage::getHistory(vector<string> &vhistory) {

    LOG(INFO) << "--Writing History--";
    //char *user = getlogin();
    char *user = getenv("USER");
	string str = "Module run by " + (string) user;
    vhistory.push_back(str);
    vhistory.push_back("Parameter List START for " + (string) modulename);
    vhistory.push_back("P1 dpifile=" + (string) dpifile);
    vhistory.push_back("P2 maskfile=" + (string) maskfile64x64path);
    vhistory.push_back("P3 compressed_maskfile=" + string(maskfile8100path));
    vhistory.push_back("P4 shadowfile=" + (string) shadowfile);
    vhistory.push_back("P5 aspectfileQ0=" + (string) aspectfileQ0);
    vhistory.push_back("P6 outImgFile=" + (string) outImgFile);
    vhistory.push_back("P7 quad_no=" + (string) quadsToProcess);
    char temp[25];
    sprintf(temp, "%f", threshold);
    vhistory.push_back("P8 threshold=" + (string) temp);
    sprintf(temp, "%d", oversamplingfactor);
    vhistory.push_back("P9 oversamplingfactor=" + (string) temp);
    vhistory.push_back("P10 sourcelist=" + (string) sourcelist);
    sprintf(temp, "%d", sourceStrengthThr);
    vhistory.push_back("P11 source strength threshold=" + (string) temp);
    sprintf(temp, "%d", nBkgdCoef);
    vhistory.push_back("P12 nBkgdCoef=" + (string) temp);
    if (clobber == YES)
        vhistory.push_back("P13 clobber=yes");
    else
        vhistory.push_back("P13 clobber=no");
    if (history == YES)
        vhistory.push_back("P14 history=yes");
    else
        vhistory.push_back("P14 history=no");
    vhistory.push_back("Parameter List END");

    LOG(INFO) << "--History written successfully--";
    return (EXIT_SUCCESS);
}

int matrixproduct(real_2d_array &A, real_2d_array &B, real_2d_array &C,
        ae_int_t m, ae_int_t n, ae_int_t p) {

    if (m == 0 || n == 0 || p == 0) {
        LOG(ERROR) << "Empty Array Found";
        return (EXIT_FAILURE);
    }
    ae_int_t i, j, k;
    int index = 0;
    double val, c;
    double *outarray;
    outarray = new double[m * p];
    if (outarray == NULL) {
        LOG(ERROR) << "Out of memory error";
        return (EXIT_FAILURE);
    }
    for (i = 0; i < m; i++) {
        for (j = 0; j < p; j++) {
            val = 0;
            for (k = 0; k < n; k++) {
                val = A(i, k) * B(k, j);
                //cout<<A(i,k)<<" * "<<B(k,j)<<" = "<<val;
                c = c + val;
            }
            outarray[index++] = c;
            if (index > (m * p)) {
                LOG(ERROR) << "***Index out of bounds error***";
                LOG(ERROR) << "Index-" << index << "  Max is- " << (m * p);
                return (EXIT_FAILURE);
            }
            c = 0;
        }
    }
    index = 0;
    for (i = 0; i < m; i++)
        for (j = 0; j < p; j++) {
            C(i, j) = outarray[index++];
            if (index > (m * p)) {
                LOG(ERROR) << "***Index out of bounds error***";
                LOG(ERROR) << "Index-" << index << "  Max is- " << (m * p);
                return (EXIT_FAILURE);
            }
        }
    delete[] outarray;
    return (EXIT_SUCCESS);
}

//size of matrix A is m x n and size of matrix B is m x p
//C will be the returned matrix of size n x p

int multiply_AinvB(float *Aarr, float *Barr, int M, int N, int P, float *C) {
    //it will use the concept
    // X=InverseA x B
    // X=Inverse(TransposeA x A) x TransposeA x B
    LOG(INFO) << "Inside multiply_AinvB function";
    LOG(INFO) << "M:" << M << "  N:" << N << "  P:" << P;
    real_2d_array A, invAtxA, At, B; //At is transpose matrix of A
    ae_int_t m = M, n = N, p = P;
    A.setlength(m, n);
    for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++)
            A(i, j) = Aarr[i * n + j];
    //setting values for B matrix
    B.setlength(m, p);
    for (int i = 0; i < m; i++)
        for (int j = 0; j < p; j++)
            B(i, j) = Barr[i * p + j];
    LOG(INFO) << "Content set for A and B ";
    At.setlength(n, m);
    rmatrixtranspose((long) m, (long) n, A, 0, 0, At, 0, 0); //taking transpose of A
    LOG(INFO) << "Transpose of A completed";
    real_2d_array AtxA; //for storing At x A

    AtxA.setlength(n, n);
    matrixproduct(At, A, AtxA, n, m, n);

    //taking inverse of AtxA using SVD
    ae_int_t uneeded = 2, vtneeded = 2, additionalmemory = 2;

    real_1d_array w;
    real_2d_array u, vt;

    w.setlength(n);
    u.setlength(n, n);
    vt.setlength(n, n);

    ae_int_t info;
    matinvreport rep;

    bool retval;
    retval = rmatrixsvd(AtxA, n, n, uneeded, vtneeded, additionalmemory, w, u, vt);
    if (retval != true) {
        LOG(ERROR) << "Error in function rmatrixsvd";
        return (EXIT_FAILURE);
    }

    real_2d_array eigenmat;
    eigenmat.setlength(w.length(), w.length());
    for (int i = 0; i < eigenmat.rows(); i++) {
        for (int j = 0; j < eigenmat.cols(); j++) {
            if (i == j) eigenmat(i, j) = w(i);
            else eigenmat(i, j) = 0;
        }
    }

    real_2d_array temp;
    temp.setlength(n, n);
    invAtxA.setlength(n, n);

    //finding inverse of A
    real_2d_array vtt, ut;
    vtt.setlength(vt.cols(), vt.rows());
    ut.setlength(u.cols(), u.rows());

    rmatrixtranspose(vt.rows(), vt.cols(), vt, 0, 0, vtt, 0, 0); //taking transpose of vt
    //rmatrixinverse(vt,info,rep);
    rmatrixinverse(eigenmat, info, rep);
    rmatrixtranspose(u.rows(), u.cols(), u, 0, 0, ut, 0, 0); //taking transpose of u
    //rmatrixinverse(u,info,rep);
    //vt x eigenmatrix = temp
    if (matrixproduct(vtt, eigenmat, temp, vt.rows(), eigenmat.rows(), eigenmat.cols())) {
        LOG(ERROR) << "***Error in matrix product of vt and eigenmat***";
        return (EXIT_FAILURE);
    }
    //temp x u = inverse of A
    if (matrixproduct(temp, ut, invAtxA, temp.rows(), u.rows(), u.cols())) {
        LOG(ERROR) << "***Error in matrix product of temp and u***";
        return (EXIT_FAILURE);
    }
    real_2d_array invA;
    invA.setlength(n, m);
    if (matrixproduct(invAtxA, At, invA, invAtxA.rows(), At.rows(), At.cols())) {
        LOG(ERROR) << "***Error in matrix product of invAtxA and At***";
        return (EXIT_FAILURE);
    }
    LOG(INFO) << "Final multiplication of Ainverse and B.........";
    //multiplying this with B
    real_2d_array invAB;
    invAB.setlength(n, p);
    if (matrixproduct(invA, B, invAB, n, m, p)) {
        LOG(ERROR) << "***Error in matrix product of invA and B***";
        return (EXIT_FAILURE);
    }
    for (int i = 0; i < n; i++)
        for (int j = 0; j < p; j++)
            C[i * p + j] = invAB(i, j);
    LOG(INFO) << "Mulitplication of Ainverse and B completed";
    return (EXIT_SUCCESS);
}

int applySigma(vector<float> &intensity, vector<int> &row, vector<int> &col, float threshold) {
    LOG(INFO) << "Input Data Points:" << intensity.size();
    if (intensity.size() <= 0) {
        LOG(ERROR) << "No data points found for applying sigma----------------applySigma()";
        return (EXIT_FAILURE);
    }

    vector<float> flux;
    vector<int> r, c;
    flux = intensity;
    r = row;
    c = col;
    //clearing the input values so that they can be updated
    intensity.clear();
    row.clear();
    col.clear();
    double mean = getmean<float>(flux.data(), flux.size());
    double rms = getrms<float>(flux.data(), flux.size());
    LOG(INFO) << "Mean:" << mean << "     RMS:" << rms;
    LOG(INFO) << "Mean + thr x rms : " << mean + threshold*rms;
    LOG(INFO) << "Mean - thr x rms : " << mean - threshold*rms;
    for (long i = 0; i < r.size(); i++) {
        if (flux[i]>(mean - threshold * rms) && flux[i]<(mean + threshold * rms)) {
            intensity.push_back(flux[i]);
            row.push_back(r[i]);
            col.push_back(c[i]);
        }
    }
    LOG(INFO) << "Output Data Points:" << intensity.size();
    return (EXIT_SUCCESS);
}

//function to write raw image and background subtracted image to output files

int cztimage::writeSkyImage() {

    LOG(INFO) << "--Writing sky image--";
    int i=0; //counter variables
    int status = 0;
    //writing raw sky image to output file  
    int bitpix = FLOAT_IMG;
    int naxis = 2;
    long naxes[2];
    naxes[0] = skyimgsize;
    naxes[1] = skyimgsize;
    long fpixel[2];
    fpixel[0] = fpixel[1] = 1;
    fits_create_img(fout, bitpix, naxis, naxes, &status); //raw image
    if (status) {
        fits_report_error(stderr, status);
        return (EXIT_FAILURE);
    }
    LOG(INFO) << "Primary image structure created";
    fits_write_pix(fout, TFLOAT, fpixel, skyimgsize*skyimgsize, rawskyimage, &status);
    if (status) {
        fits_report_error(stderr, status);
        return (EXIT_FAILURE);
    }
    LOG(INFO) << "Image data writing completed";


    fits_write_key(fout, TINT, "OVERSMPF", &oversamplingfactor, "Oversampling factor", &status);
    if (status) {
        fits_report_error(stderr, status);
        return (EXIT_FAILURE);
    }

    //writing jpeg image
    if (debugmode == 1) {
        LOG(INFO) << "Writing synthetic image";
        unsigned char* UC_outimage;
        char jpegName[512];
        UC_outimage = new unsigned char[skyimgsize * skyimgsize];
        if (UC_outimage == NULL) {
            LOG(ERROR) << "Out of memory error";
            return (EXIT_FAILURE);
        }
        for (i = 0; i < skyimgsize * skyimgsize; i++) {
            UC_outimage[i] = 0;
        }
        sprintf(jpegName, "%s.jpeg", outImgFile);
        scaleDownToUC<float>(rawskyimage, UC_outimage, skyimgsize, skyimgsize);
        makeJpeg(UC_outimage, skyimgsize, skyimgsize, 1, JCS_GRAYSCALE, 100, jpegName);
        LOG(INFO) << "Synthetic image written as " << outImgFile << ".jpg";
    }
    //END jpeg image written
    
    LOG(INFO) << "--Writing of sky image completed--";
   
    return (EXIT_SUCCESS);
}

int cztimage::identifyPeaks() {
    
    LOG(INFO)<<"-- Peaks Identification started --";
    vector<int> row, col;
    vector<float> intensity;
    int r, c;
    int rowcen = skyimgsize / 2;
    int colcen = skyimgsize / 2;
    double val = (double) (DETECTORWIDTH / (double) (16.0 * oversamplingfactor * MASKHEIGHT));    //this value is used in computation of thetax & thetay from x.y or row,col
    double theta = 0;
    double RA_tmp=0, DEC_tmp=0; //temporary variables used to store RA and DEC in the middle of processing.
    double thr = getrms<float>(rawskyimage, skyimgsize * skyimgsize) * threshold;
    float max = 0;
    int winsize = 0;
    int start_r=0, end_r=0, start_c=0, end_c=0;
    
    
    LOG(INFO) << "Threshold used while identifying peaks:" << thr;
    //selecting peaks from the image
    for (int i = 0; i < skyimgsize * skyimgsize; i++) {
        r = (i / skyimgsize);
        c = (i % skyimgsize);
        if (rawskyimage[i] > thr) {
            intensity.push_back(rawskyimage[i]);
            row.push_back(r);
            col.push_back(c);
        }
    }
    LOG(INFO) << "No. of peaks detected in first iteration :" << intensity.size();

    if (row.size() == 0) {
        LOG(ERROR) << "***No Peaks detected in image***";
        LOG(ERROR) << "***Run with lower threshold***";
        return (EXIT_FAILURE);
    }

    LOG(INFO) << "Refining Peaks.......";
    if (oversamplingfactor < 8)
        winsize = 9;
    else
        winsize = 3 * oversamplingfactor;
    if (winsize % 2 == 0) winsize = winsize - 1; //if winsize is even, make it odd
    LOG(INFO) << "Window size:" << winsize;
    
    for (int i = 0; i < row.size(); i++) {
        start_r = row[i] - winsize / 2;
        end_r = row[i] + winsize / 2;
        start_c = col[i] - winsize / 2;
        end_c = col[i] + winsize / 2;
        if (start_r < 0) start_r = 0;
        if (end_r > skyimgsize) end_r = skyimgsize;
        if (start_c < 0) start_c = 0;
        if (end_c > skyimgsize) end_c = skyimgsize;
        max = 0;
        for (int k = start_r; k <= end_r; k++) {
            for (int l = start_c; l <= end_c; l++) {
                if (rawskyimage[k * skyimgsize + l] > max) {
                    max = rawskyimage[k * skyimgsize + l];
                    row[i] = k;
                    col[i] = l;
                }
            }
        }
    }
    //finding final peaks , removing duplicate peaks
    vector<int> frow, fcol;
    frow.push_back(row[0]);
    fcol.push_back(col[0]);
    bool flag = false;
    for (int i = 0; i < row.size(); i++) {
        for (int j = 0; j < frow.size(); j++) {
            if (frow[j] == row[i] && fcol[j] == col[i]) {
                flag = true;
                break;
            }
        }
        if (flag == false) {
            frow.push_back(row[i]);
            fcol.push_back(col[i]);
        } else
            flag = false;
    }
    row.clear();
    col.clear();

    LOG(INFO) << "Final Peaks - ";
    for (int i = 0; i < frow.size(); i++) {
        r = frow[i];
        c = fcol[i];
        flux.push_back(rawskyimage[r * skyimgsize + c]);
        thetax.push_back(atan(val * (double) (c - colcen)));
        thetay.push_back(atan(val * (double) (r - rowcen)));
        to_RA_DEC((string) aspectfileQ0, thetax[i], thetay[i], RA_tmp, DEC_tmp);
    //    getRaDec(aspectfileQ0,thetax[i],thetay[i],&RA_tmp, &DEC_tmp);
        ra.push_back(RA_tmp);
        dec.push_back(DEC_tmp);
    }

    nsources = flux.size();

    LOG(INFO) << "Number of peaks found:" << flux.size();

    if (nsources > PEAKLIMIT) {
        LOG(ERROR) << "***Cannot process this many number of peaks*** ***Run with larger threshold***";
        LOG(ERROR) << "Peak Limit is set to " << PEAKLIMIT << " at present";
        return (EXIT_FAILURE);
    }
    frow.clear();
    fcol.clear();
    intensity.clear();
    return (EXIT_SUCCESS);
}

int cztimage::writeSyntheticImage(){
    LOG(INFO) << "--Writing synthetic image--";
    int i=0,j=0,k=0; //counter variables;
    int rowcen = skyimgsize / 2;
    int colcen = skyimgsize / 2;
    double val = (double) (DETECTORWIDTH / (double) (16.0 * oversamplingfactor * MASKHEIGHT));    //this value is used in computation of thetax & thetay from x.y or row,col and vice versa
    float exponent=0;
    float sigma = 1/sqrt(2*M_PI);
    int r=0,c=0; //row and column temporary variables to store row and column number of SVD fitted identified peaks.
    int size = thetax.size();
    unsigned char* UC_synimage;
    char jpegName[512];
    float **synimage=allocateMemory<float>(skyimgsize,skyimgsize);
    float *f_synimage;

    f_synimage = new float[skyimgsize* skyimgsize];
    UC_synimage = new unsigned char[skyimgsize*skyimgsize];
  
    if (UC_synimage == NULL || f_synimage==NULL) {
        LOG(ERROR) << "Out of memory error";
        return (EXIT_FAILURE);
    }
    
    for(i=0;i<skyimgsize;i++){
        for(j=0;j<skyimgsize;j++){
            synimage[i][j]=0;
            f_synimage[i*skyimgsize+j]=0;
            UC_synimage[i*skyimgsize+j]=0;
        }
    }
  
    for(i=0;i<size;i++){
        c = (int)((tan(thetax[i])/val) + colcen);
        r = (int)((tan(thetay[i])/val) + colcen);
        for(j=0;j<skyimgsize;j++){
            for(k=0;k<skyimgsize;k++){
generateCorrelationMatrix_2d(oversampleddpi, oversampledmask, rawskyimage, YSIZE, XSIZE, oversamplingfactor);
    keepCentral(rawskyimage, YSIZE, XSIZE, oversamplingfactor, &skyimgsize); //retains 1/4th area from centre
    //raw sky image created and stored in rawskyimage array               

    fits_create_file(&fout, outImgFile, &status);
    if (status) {
        LOG(ERROR) << "***Error in output image file creation***";
        fits_report_error(stderr, status);
        return (EXIT_FAILURE);
    }


    writeSkyImage(); //writing image data to output files

    fits_close_file(fout, &status);
    
    /**************Image from DPI created********************************/


    if (addWCS()) {
        LOG(ERROR) << "***Error in adding WCS to output files***";
        return (EXIT_FAILURE);
    }

    //updating keywords in output file
    if (updateKeywords(outImgFile, modulename)) {
        LOG(ERROR) << "***Error in updating keywords***";
        return (EXIT_FAILURE);
    }

    //findKnownSources(catalogfile, aspectfileQ0, extnum, RA_colname, DEC_colname, FLUX_colname, RA_ks, DEC_ks, FLUX_ks, thetax_ks, thetay_ks); //RA, DEC, FLUX, thetax and thetay of known sources.

    if(identifyPeaks()){
        return(EXIT_FAILURE);
    }
    LOG(INFO)<< ">>>>>>>>>>>> IDENIFIED PEAKS <<<<<<<<<<<<<<";
    displayPeaks();
    LOG(INFO)<< ">>>>>>>> KNOWN SOURCES <<<<<<<<<<";
    displayKnownSources();
    
    int size_knownSources = RA_ks.size();
    int size_identifiedPeaks = ra.size();

    LOG(INFO) << "Matching peaks in known and identified sources.";
    for(i=0; i<size_knownSources; i++){
        if (size_identifiedPeaks==0){
            ra.push_back(RA_ks[i]); dec.push_back(DEC_ks[i]); flux.push_back(FLUX_ks[i]); thetax.push_back(thetax_ks[i]); thetay.push_back(thetay_ks[i]);
        }
        for(j=0; j< size_identifiedPeaks; j++){
            if ( (thetax_ks[i]> thetax[j]+angleConverter(resolutionLimit, "arcmin", "radians")) || (thetax_ks[i]<thetax[j]-angleConverter(resolutionLimit, "arcmin", "radians")) ){
                ra.push_back(RA_ks[i]); dec.push_back(DEC_ks[i]); flux.push_back(FLUX_ks[i]); thetax.push_back(thetax_ks[i]); thetay.push_back(thetay_ks[i]);
            }
            else if((thetay_ks[i]> thetay[j]+angleConverter(resolutionLimit, "arcmin", "radians")) || (thetay_ks[i]<thetay[j]-angleConverter(resolutionLimit, "arcmin", "radians"))){
                ra.push_back(RA_ks[i]); dec.push_back(DEC_ks[i]); flux.push_back(FLUX_ks[i]); thetax.push_back(thetax_ks[i]); thetay.push_back(thetay_ks[i]);
            }
        }
    }
    

    LOG(INFO) << "Criterion for matching is: Resolution limit of " << resolutionLimit << " arcminutes i.e. " << angleConverter(resolutionLimit, "arcmin", "radians") << " radians";
    displayPeaks();

    /***SVD FITTING STARTED ***/
    nsources = ra.size();
    LOG(INFO) << "NSOURCES FOUND: "<< nsources; //debug
    LOG(INFO) << "-- SVD fitting started--";

    //  >START generating shadows for known sources
    shadows = allocateMemory<float>(nsources, XSIZE * YSIZE); //buffer to store shadows of all the peaks and identified sources in the field of view.
    if (shadows == NULL) {
        LOG(ERROR) << "Out of memory error";
        return (EXIT_FAILURE);
    }


    for (i = 0; i < nsources; i++) {
        LOG(INFO) << "Generating shadow for source in FOV with following camera coordinates: thetax-" << thetax[i] << "  thetay-" << thetay[i];
        MaskWeighting mskwt((char*)maskfile8100path.c_str(), thetax[i], thetay[i]);
        if (mskwt.run()) {
            LOG(ERROR) << "****Error in mask Weighting****";
            return (EXIT_FAILURE);
        }
        mskwt.getShadow(shadows[i], XSIZE * YSIZE);
    }

    if (debugmode == YES) {
        for (int i = 0; i < nsources; i++) {
            char name[512];
            sprintf(name, "shadow_%d", i);
            ofstream fb(name, ios::binary);
            fb.write((char *) shadows[i], sizeof (float)*XSIZE * YSIZE);
            fb.close();
        }
    }

    //  >END shadows for known and identified sources generated and stored in variable shadows[i][XSIZE*YSIZE]

    //  >START generating background shadows based on coefficied provided by user.

    bkgdshadows = allocateMemory<float>(nBkgdCoef, XSIZE * YSIZE);
    getBkgdShadows();

    if (debugmode == YES) {
        unsigned char* UC_bkgd;
        UC_bkgd = new unsigned char[XSIZE*YSIZE];
        char name[512], jpegName[512];
        for (int i = 0; i < nBkgdCoef; i++) {
            
            sprintf(name, "bkgd_%d", i);
            ofstream fb(name, ios::binary);
            fb.write((char *) bkgdshadows[i], sizeof (float)*XSIZE * YSIZE);
            fb.close();
            
            sprintf(jpegName, "bkgd_%d.jpeg", i);
            scaleDownToUC<float>(bkgdshadows[i], UC_bkgd, YSIZE, XSIZE);
            makeJpeg(UC_bkgd, XSIZE, YSIZE, 1, JCS_GRAYSCALE, 100, jpegName);
        }
        delete[] UC_bkgd;
        
        
        
        
    }
    //  >END background shadows generated and stored in bkgdshadows[i][XSIZE*YSIZE]

       LOG(INFO)<<"Fitting sources......";
   int prev_peaks=nsources,prev_nbkgd=nBkgdCoef;
    while(1){
        if ((prev_peaks==0 && prev_nbkgd==0)) {
            break;
        }
        if(linearfitDPI()){
                LOG(ERROR)<<"***Error in linearfitDPI***";
                return (EXIT_FAILURE);
        }
        
        if((prev_peaks==nsources && prev_nbkgd==nBkgdCoef))            break;
        prev_peaks=nsources;  prev_nbkgd=nBkgdCoef;
        LOG(INFO)<<"prev_peaks:"<<prev_peaks<<"     prev_nbkgd:"<<prev_nbkgd;
    }
    LOG(INFO)<<"Number of sources:  "<<nsources;
    LOG(INFO)<<"Number of background:"<<nBkgdCoef;
    LOG(INFO)<<"Fitting Complete";
   
 //adding sources to sourcelist
    ofstream fout(sourcelist);
    if(!fout){
        LOG(ERROR)<<"***Error in opening file "<<sourcelist<<"***";
        return (EXIT_FAILURE);
    }
    
    //finding RA and DEC for sources found
    double ra_rad,dec_rad;              //for radians
    ra.clear();  dec.clear();
    fout<<setw(15)<<"RA"<<setw(15)<<"DEC"<<setw(15)<<"FLUX";
    for(int i=0;i<thetax.size();i++){
        getRaDec(aspectfileQ0,thetax[i],thetay[i],&ra_rad,&dec_rad);
        ra.push_back(ra_rad);
        dec.push_back(dec_rad);
        fout<<endl<<setw(15)<<ra[i]<<setw(15)<<dec[i]<<setw(15)<<flux[i]<<setw(15)<<thetax[i]<<setw(15)<<thetay[i];
        cout<<endl<<setw(15)<<ra[i]<<setw(15)<<dec[i]<<setw(15)<<flux[i]<<setw(15)<<thetax[i]<<setw(15)<<thetay[i];
    }
    fout.close(); 
    
    writeSyntheticImage();
    
    /***SVD FITTING COMPLETED***/
    //delete[] mask,dpi,oversampled_dpi,oversampled_mask;     
    //writing history to output file
    if (history == YES) {
        vector<string> vhistory;
        getHistory(vhistory);
        writeHistory(outImgFile, vhistory); //writes history to each HDU of output file


    }

    delete[] mask, dpi, oversampleddpi, oversampledmask, rawskyimage, nobkgdskyimage;
    return (EXIT_SUCCESS);
}

//Assumption - The Mask Image file contains dpi in 4 quadrants ie. extension no 2,3,4,5
//Each quadrant mask is of size 64 x 64

int cztimage::getMaskArray() {
    fitsfile *fptr;
    int status = 0;
    int npixQuad = XSIZE * YSIZE / NUMQUAD;
    int hduid=0;
    float *quadmask = new float[npixQuad];

    /*code to process quadrants*/
    int* quadArray;
    int noccurrence;
    stringFinder(quadsToProcess, ",", 0, &noccurrence);
    int nelems_quad = noccurrence + 1;
    quadArray = new int[nelems_quad];

    if (csvStrToInt(quadsToProcess, ",", quadArray, &nelems_quad)) {
        LOG(ERROR) << "***Error in converting quadrant string array into integer array***";
    }
    /*code to process quadrants*/






    fits_open_file(&fptr, (char*) maskfile64x64path.c_str(), READONLY, &status);
    if (status != 0) {
        LOG(ERROR) << "***Error opening mask file***";
        fits_report_error(stderr, status);
        return (EXIT_FAILURE);
        ;
    }

    int hdutype = 0, qid, r, c, index = 0;
    long fpixel[2];
    fpixel[0] = 1;
    fpixel[1] = 1;
    for (int i = 0; i < NUMQUAD; i++) {
        fits_movabs_hdu(fptr, i+2, &hdutype, &status);
        if (status != 0) {
            LOG(ERROR) << "***Error moving HDU***";
            fits_report_error(stderr, status);
            return (EXIT_FAILURE);
            ;
        }
        fits_read_key(fptr, TINT, "QUADID", &qid, NULL, &status);
        if (status) {
            fits_report_error(stderr, status);
            return (EXIT_FAILURE);
        }
        if (hdutype != IMAGE_HDU) {
            LOG(ERROR) << "Image HDU not found in mask file at HDU no " << i + 2;
            return (EXIT_FAILURE);
        }
        if (hdutype == IMAGE_HDU && qid == i) {
            fits_read_pix(fptr, TFLOAT, fpixel, npixQuad, NULL, quadmask, NULL, &status);
            if (status != 0) {
                LOG(ERROR) << "***Error reading mask pixels***";
                fits_report_error(stderr, status);
                return (EXIT_FAILURE);
                ;
            }
            r = (qid / NUMQUAD_ROW)*(YSIZE / NUMQUAD_COL);
            c = (qid % NUMQUAD_ROW)*(XSIZE / NUMQUAD_ROW);
            index = 0;
            for (int u = r; u < r + (YSIZE / NUMQUAD_COL); u++)
                for (int v = c; v < c + (XSIZE / NUMQUAD_ROW); v++){
                    mask[u * XSIZE + v] = quadmask[index++];
//                    LOG(INFO) << "MASK VALUE::" << mask[u*XSIZE+v];
                }
                    
        }
    }
    fits_close_file(fptr, &status);
    if (status != 0) {
        LOG(ERROR) << "***Error closing maskfile " << maskfile64x64path << " ***";
        fits_report_error(stderr, status);
        return (EXIT_FAILURE);
        ;
    }
    return (EXIT_SUCCESS);
}

//Assumption - The DPI Image file contains dpi in 4 quadrants ie. extension no 2,3,4,5
//Each quadrant DPI is 64 x 64

int cztimage::getDPI() {
    fitsfile *fptr;
    int status = 0;
    int npixQuad = XSIZE * YSIZE / NUMQUAD;
    long *quaddpi = new long[npixQuad];
    int hduid=0;
    /*code to process quadrants*/
    int* quadArray;
    int noccurrence;
    stringFinder(quadsToProcess, ",", 0, &noccurrence);
    int nelems_quad = noccurrence + 1;
    quadArray = new int[nelems_quad];

    if (csvStrToInt(quadsToProcess, ",", quadArray, &nelems_quad)) {
        LOG(ERROR) << "***Error in converting quadrant string array into integer array***";
    }
    /*code to process quadrants*/
    
    fits_open_file(&fptr, dpifile, READONLY, &status);
    if (status != 0) {
        LOG(ERROR) << "***Error opening DPI file***";
        fits_report_error(stderr, status);
        return (EXIT_FAILURE);
        ;
    }

    int hdutype = 0, qid, r, c, index = 0;
    long fpixel[2];
    fpixel[0] = 1;
    fpixel[1] = 1;
    for (int i = 0; i < NUMQUAD; i++) {;
        fits_movabs_hdu(fptr, i+2, &hdutype, &status);
        if (status != 0) {
            LOG(ERROR) << "***Error moving HDU***";
            fits_report_error(stderr, status);
            return (EXIT_FAILURE);
            ;
        }
        fits_read_key(fptr, TINT, "QUADID", &qid, NULL, &status);
        if (hdutype != IMAGE_HDU) {
            LOG(ERROR) << "Image HDU not found in DPI file at HDU no " << i + 2;
            return (EXIT_FAILURE);
        }
        if (qid == i) {
            fits_read_pix(fptr, TLONG, fpixel, npixQuad, NULL, quaddpi, NULL, &status);
            if (status != 0) {
                LOG(ERROR) << "***Error reading DPI pixels***";
                fits_report_error(stderr, status);
                return (EXIT_FAILURE);
            }
            r = (qid / NUMQUAD_ROW)*(YSIZE / NUMQUAD_COL);
            c = (qid % NUMQUAD_ROW)*(XSIZE / NUMQUAD_ROW);
            index = 0;
            for (int u = r; u < r + (YSIZE / NUMQUAD_COL); u++)
                for (int v = c; v < c + (XSIZE / NUMQUAD_ROW); v++){
                    dpi[u * XSIZE + v] = (float) quaddpi[index++];
//                    LOG(INFO) << "DPI VALUE::" << dpi[u * XSIZE + v];
                }
            
        }
    }
    fits_close_file(fptr, &status);
    if (status) {
        LOG(ERROR) << "***Error closing dpifile " << dpifile << " ***";
        fits_report_error(stderr, status);
        return (EXIT_FAILURE);
        ;
    }
    return (EXIT_SUCCESS);
}

void cztimage::oversample() {
    LOG(INFO) << "---Oversampling of DPI and mask started---";
    size = XSIZE* oversamplingfactor; //oversampled rows/columns
    oversampleddpi = new float[size * size];
    oversampledmask = new float[size * size];
    int x, y;
    LOG(INFO) << "Oversampled size: " << size << " x " << size;
    for (int i = 0; i < YSIZE; i++)
        for (int j = 0; j < XSIZE; j++)
            for (int k = i * oversamplingfactor; k < i * oversamplingfactor + oversamplingfactor; k++)
                for (int l = j * oversamplingfactor; l < j * oversamplingfactor + oversamplingfactor; l++) {
                    oversampleddpi[k * size + l] = (float) dpi[i * XSIZE + j] / (float) (oversamplingfactor * oversamplingfactor);
                    oversampledmask[k * size + l] = (float) mask[i * XSIZE + j];
                }
    LOG(INFO) << "---Oversampling of DPI and Mask completed---";
}

void doFft(float data[], unsigned long nn, int isign) {
    unsigned long n, mmax, m, j, istep, i;
    double wtemp, wr, wpr, wpi, wi, theta;
    float tempr, tempi;

    n = nn << 1;
    j = 1;
    for (i = 1; i < n; i += 2) {
        if (j > i) {
            SWAP(data[j], data[i]);
            SWAP(data[j + 1], data[i + 1]);
        }
        m = nn;
        while (m >= 2 && j > m) {
            j -= m;
            m >>= 1;
        }
        j += m;
    }
    mmax = 2;
    while (n > mmax) {
        istep = mmax << 1;
        theta = isign * (2 * M_PI / mmax);
        wtemp = sin(0.5 * theta);
        wpr = -2.0 * wtemp*wtemp;
        wpi = sin(theta);
        wr = 1.0;
        wi = 0.0;
        for (m = 1; m < mmax; m += 2) {
            for (i = m; i <= n; i += istep) {
                j = i + mmax;
                tempr = wr * data[j] - wi * data[j + 1];
                tempi = wr * data[j + 1] + wi * data[j];
                data[j] = data[i] - tempr;
                data[j + 1] = data[i + 1] - tempi;
                data[i] += tempr;
                data[i + 1] += tempi;
            }
            wr = (wtemp = wr) * wpr - wi * wpi + wr;
            wi = wi * wpr + wtemp * wpi + wi;
        }
        mmax = istep;
    }
}

//void generateCorrelationMatrix_2d(float *dph, float *mask, float *correlationMatrix,int TOTALROWS, int TOTALCOLS, int factor)
void generateCorrelationMatrix_2d(float *dph,float *mask,float *correlationMatrix,int TOTALROWS,int TOTALCOLS,int sampleCount )
{


	int n=TOTALCOLS*TOTALROWS*sampleCount*sampleCount,fftN=0,k=0,ii=0,jj=0,i=0,j=0;
	int n_=TOTALROWS*sampleCount;
	float *dph_complex,*phase_complex,*correlation_complex,f,*mask_complex,*tmp_complex;
	FILE*fp;
	float xshift=2*M_PI*sampleCount*7.53125,fy=0,fx=0,yshift=2*M_PI*sampleCount*0;

	fftN=n*2;

	dph_complex=(float*)malloc(sizeof(float)*(fftN+1));
	phase_complex=(float*)malloc(sizeof(float)*(fftN+1));
	correlation_complex=(float*)malloc(sizeof(float)*(fftN+1));
	mask_complex=(float*)malloc(sizeof(float)*(fftN+1));
	tmp_complex=(float*)malloc(sizeof(float)*(fftN+1));


	for(i=1,j=0;i<=fftN;i+=2,j++)
	{
		dph_complex[i]=dph[j];
		dph_complex[i+1]=0;
		mask_complex[i]=mask[j];
		mask_complex[i+1]=0;
		correlation_complex[i]=0;
		correlation_complex[i+1]=0;
	}

	phase_complex[1]=1;
	phase_complex[2]=0;	
	k=3;
	for(i=1;i<n/2;i++)
	{
		ii=i/n_;
		jj=i%n_;
		fx=(ii/(n_*1.0))*xshift;
		fy=(jj/(n_*1.0))*yshift;
		phase_complex[k]=(cos(fx)*cos(fy)) - (sin(fx)*sin(fy)) ;
		phase_complex[k+1]=(sin(fx)*cos(fy)) + (cos(fx)*sin(fy));
		k+=2;

	}
	for(i=-n/2;i<0;i++)
	{	
		ii=i/n_;
		jj=i%n_;
		fx=(ii/(n_*1.0))*xshift;
		fy=(jj/(n_*1.0))*yshift;
		phase_complex[k]=(cos(fx)*cos(fy)) - (sin(fx)*sin(fy)) ;
		phase_complex[k+1]=(sin(fx)*cos(fy)) + (cos(fx)*sin(fy));
		k+=2;

	}
	doFft(dph_complex,n,1);
	doFft(mask_complex,n,1);
	for(i=1;i<=fftN;i+=2)
	{
		tmp_complex[i]=(mask_complex[i]*dph_complex[i])+(mask_complex[i+1]*dph_complex[i+1]);
		tmp_complex[i+1]=(mask_complex[i+1]*dph_complex[i])-(mask_complex[i]*dph_complex[i+1]);
		correlation_complex[i]=(tmp_complex[i]*phase_complex[i])-(tmp_complex[i+1]*phase_complex[i+1]);
		correlation_complex[i+1]=(tmp_complex[i+1]*phase_complex[i])+(tmp_complex[i]*phase_complex[i+1]);
		correlation_complex[i]/=n;
		correlation_complex[i+1]/=n;
	}
	doFft(correlation_complex,n,-1);
	for(i=1,j=0;i<=fftN;i+=2,j++)
	{

		correlationMatrix[j]=sqrt((correlation_complex[i]*correlation_complex[i])+(correlation_complex[i+1]*correlation_complex[i+1]));
	}
	swapComplex(correlationMatrix);
	free(dph_complex);
    free(mask_complex);
    free(tmp_complex);
}


/*
void g
                exponent = -1*(pow((j-c),2)+pow((k-r),2))/(2*pow(sigma,2));
                synimage[j][k] = flux[i]*pow(M_El,exponent);
            }
        }
    }

    for(i=0; i<skyimgsize;i++){
        for(j=0;j<skyimgsize;j++){
            f_synimage[i*skyimgsize+j] = synimage[i][j];
        }
    }

    //writing jpeg image
    sprintf(jpegName, "%s_synthetic.jpeg", outImgFile);
    scaleDownToUC<float>(f_synimage, UC_synimage, skyimgsize, skyimgsize);
    makeJpeg(UC_synimage, skyimgsize, skyimgsize, 1, JCS_GRAYSCALE, 100, jpegName);
    LOG(INFO) << "Synthetic image written as " << jpegName;

    
    delete[] UC_synimage, f_synimage;
    freeMemory(synimage,skyimgsize);
    LOG(INFO) << "--Writing of synthetic image completed--";
}
int cztimage::getBkgdShadows(){ 
    LOG(INFO) << "--Background shadows are being computed--";
    int i=0, j=0;
    if(nBkgdCoef<1 || nBkgdCoef>6)  return (EXIT_FAILURE);
    switch(nBkgdCoef){
        case 6:  for( i=0;i<YSIZE;i++){       //varying with xy2 direction
                        for(j=0;j<XSIZE;j++){
                                bkgdshadows[5][i*XSIZE+j]=(float)(i*j*j)/(float)((XSIZE-1)*(YSIZE-1)*(YSIZE-1));
                        }
                 }
        case 5: for( i=0;i<YSIZE;i++){       //varying with x2y direction
                        for(j=0;j<XSIZE;j++){
                                bkgdshadows[4][i*XSIZE+j]=(float)(i*i*j)/(float)((XSIZE-1)*(XSIZE-1)*(YSIZE-1));
                        }
                 }
        case 4:  for( i=0;i<YSIZE;i++){       //varying with xy direction
                        for(j=0;j<XSIZE;j++){
                                bkgdshadows[3][i*XSIZE+j]=(float)(i*j)/(float)((YSIZE-1)*(XSIZE-1));
                        }
                 }
        case 3:  for( i=0;i<YSIZE;i++){       //varying with y direction
                        for(j=0;j<XSIZE;j++){
                               bkgdshadows[2][i*XSIZE+j]=(float)j/(float)(YSIZE-1);
                        }
                 }
        case 2:  for( i=0;i<YSIZE;i++){       //varying with x direction
                        for(j=0;j<XSIZE;j++){
                                bkgdshadows[1][i*XSIZE+j]=(float)i/(float)(XSIZE-1);
                        }
                 }
        case 1:  for( i=0;i<YSIZE*XSIZE;i++)
                        bkgdshadows[0][i]=1;
    }
    return (EXIT_SUCCESS);
    LOG(INFO) << "--Background shadows computed successfully--";
}

int cztimage::linearfitDPI(){
    LOG(INFO) << "--------------------------------------";
    LOG(INFO) << "-- Linear fitting of DPI initiated --";
    LOG(INFO)<<"Creating matrix A.........";
    nsources = ra.size();
    LOG(INFO) << "nsources evaluated: "<< nsources; //debug
    int cols_A= nsources + nBkgdCoef;
    LOG(INFO) << "columns in A:"<< cols_A;
    int rows_A = XSIZE*YSIZE;
    float *A=new float[cols_A*rows_A];
    if(A==NULL){
        LOG(ERROR)<<"Out of Memory Error";
        return (EXIT_FAILURE);
    }
    //setting shadows
    LOG(INFO)<<"Copying shadows...";
    for(int j=0;j<nsources;j++)
        for(int i=0;i<rows_A;i++){
            if((i*cols_A+j)>(rows_A*cols_A)){
                LOG(ERROR)<<"Out of Index Error @ "<<j<<","<<i;
                LOG(ERROR)<<"Size of array is "<<rows_A*cols_A<<" and index is "<<(i*rows_A+j);
                return (EXIT_FAILURE);
            }
            A[i*cols_A+j]=shadows[j][i];
        }
    //setting background    
    LOG(INFO)<<"Copying backgrounds...";
    for(int j=nsources;j<cols_A;j++)
        for(int i=0;i<rows_A;i++){
            if((i*cols_A+j)>(rows_A*cols_A)){
                LOG(ERROR)<<"Out of Index Error@ "<<j<<","<<i;
                LOG(ERROR)<<"Size of array is "<<rows_A*cols_A<<" and index is "<<(i*rows_A+j);
                return (EXIT_FAILURE);
            }
            //cout<<"i:"<<i<<" j:"<<j;
            A[i*cols_A+j]=bkgdshadows[j-nsources][i];
        }
    LOG(INFO)<<"Matrix A created";
    if(debugmode==1){
        ofstream fout("matrix.txt");  //Just to check the matrices formed
        //writing A in file matrix.txt 
        for(int i=0;i<rows_A;i++){
            fout;
            for(int j=0;j<cols_A;j++){
                fout<<setw(12)<<A[i*cols_A+j];
            }
            
        }
        fout.close();
    }
    //matrix B is same as DPI
    //creating coefficient matrix
    int cols_B=1;           //columns in B will be 1
    int rows_B=rows_A;
    
    float *Coef=new float[cols_A];
    if(Coef==NULL){
        LOG(ERROR)<<"Out of Memory Error";
        return (EXIT_FAILURE);
    }
    LOG(INFO)<<"Calling function multiply_AinvB.... ";
    if(multiply_AinvB(A,dpi,rows_A,cols_A,cols_B,Coef)){
        LOG(ERROR)<<"***Error in multiply_AinvB function***";
        return (EXIT_FAILURE);
    }     
   
    int temp_nshadow=0,temp_nbkgd=0;
    float **temp_shadows=allocateMemory<float>(nsources,XSIZE*YSIZE);   
    float **temp_bkgdshadow=allocateMemory<float>(nBkgdCoef,XSIZE*YSIZE);
    vector<double> temp_ra, temp_dec, temp_tx,temp_ty,temp_flux;
    temp_ra=ra; temp_dec=dec; temp_tx=thetax; temp_ty=thetay; temp_flux=flux;
    ra.clear(); dec.clear(); thetax.clear();  thetay.clear(); flux.clear();
    //copying shadows and background shadows to temporary arrays 
    for(int i=0;i<nsources;i++){
        for(int j=0;j<XSIZE*YSIZE;j++){
            temp_shadows[i][j]=shadows[i][j];
        }
    }
    
    for(int i=0;i<nBkgdCoef;i++){
        for(int j=0;j<XSIZE*YSIZE;j++){
            temp_bkgdshadow[i][j]=bkgdshadows[i][j];
        }
    }
    int index=0;
    
    LOG(INFO)<<"The source strengths are-";
    for(int i=0;i<nsources;i++){
        LOG(INFO)<<"Source Strength "<<i<<"  : "<<Coef[i];
        if(Coef[i]>sourceStrengthThr){
            temp_nshadow++;
            ra.push_back(temp_ra[i]);
            dec.push_back(temp_dec[i]);
            thetax.push_back(temp_tx[i]);
            thetay.push_back(temp_ty[i]);
            flux.push_back((temp_flux[i]));
            for(int j=0;j<XSIZE*YSIZE;j++)
                shadows[index][j]=temp_shadows[i][j];
             index++;
        }
    }     
    LOG(INFO)<<"The background strengths are-";
    index=0;
    for(int i=nsources;i<nsources+nBkgdCoef;i++){
        LOG(INFO)<< "Background Strength "<< i << " :" << Coef[i];
        if(Coef[i]>sourceStrengthThr){
            temp_nbkgd++;
            for(int j=0;j<XSIZE*YSIZE;j++)
                bkgdshadows[index][j]=temp_bkgdshadow[i-nsources][j];
             index++;
        }
    }
    
    freeMemory(temp_shadows,nsources);
    freeMemory(temp_bkgdshadow,nBkgdCoef);
    
    nsources=temp_nshadow;
    nBkgdCoef=temp_nbkgd;
    delete[] A,Coef;
    temp_ra.clear(); temp_dec.clear(); temp_ty.clear(); temp_tx.clear(); temp_flux.clear();

    LOG(INFO)<<"-- Linear fitting of DPI completed --";
    
    return (EXIT_SUCCESS);
}

int cztimage::displayPeaks(){
    int n = ra.size();
    int i=0; //counter variable
    
    LOG(INFO)<< "RA" << "  |  " << "DEC" << "  |  " << "thetax" << "  |  " << "thetay" << "  |  " << "flux";
    for (i=0; i< n; i++){
        LOG(INFO)<< ra[i] << "  |  " << dec[i] << "  |  " << thetax[i] << "  |  " << thetay[i] << "  |  " << flux[i];
    }
    return EXIT_SUCCESS;
}

int cztimage::displayKnownSources(){
    int n = RA_ks.size();
    int i=0; //counter variable
    
    LOG(INFO)<< "RA" << "  |  " << "DEC" << "  |  " << "thetax" << "  |  " << "thetay" << "  |  " << "flux";
    for (i=0; i< n; i++){
        LOG(INFO)<< RA_ks[i] << "  |  " << DEC_ks[i] << "  |  " << thetax_ks[i] << "  |  " << thetay_ks[i] << "  |  " << FLUX_ks[i];
    }
    return EXIT_SUCCESS;
}
