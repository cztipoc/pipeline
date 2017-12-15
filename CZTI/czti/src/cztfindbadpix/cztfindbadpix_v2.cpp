#include "cztfindbadpix_v2.h"

using namespace std;

Cztfindbadpix::Cztfindbadpix(){
    strcpy(modulename,"cztfindbadpix_v");
    strcat(modulename,VERSION);
    strcpy(this->xcol,"");
    strcpy(this->ycol,"");
    strcpy(this->aspectfile,"");
    strcpy(this->maskfile8100,"");
    strcpy(this->ra,"");
    strcpy(this->dec,"");
    this->threshold=0;
}

int Cztfindbadpix::read(int argc, char **argv) {
    int status = 0;

    if (PIL_OK != (status = PILInit(argc, argv))) {
        LOG(ERROR) << "***Error Initializing PIL***";
        return status;
    }

    if (PIL_OK != (status = PILGetFname("par_infile", infile))) {
        LOG(ERROR) << "***Error Reading input file:" << infile << "***";
        return status;
    }

    if (PIL_OK != (status = PILGetString("par_inputtype", inputtype))) {
        LOG(ERROR) << "***Error Reading inputtype:" << inputtype << "***";

        return status;
    }

    if (strncasecmp(inputtype, "Eve", 3) == 0) {
        if (PIL_OK != (status = PILGetString("par_xcol", xcol))) {
            LOG(ERROR) << "***Error Reading X column name:" << xcol << "***";
            return status;
        }
        if (PIL_OK != (status = PILGetString("par_ycol", ycol))) {
            LOG(ERROR) << "***Error Reading Y column name:" << ycol << "***";
            return status;
        }
    }

    if (PIL_OK != (status = PILGetFname("par_CALDBbadpixelfile", CALDBbadpixelfile))) {
        LOG(ERROR) << "***Error Reading badpixelfile:" << CALDBbadpixelfile << "***";
        return status;
    }

    if (PIL_OK != (status = PILGetFname("par_outfile", outfile))) {
        LOG(ERROR) << "***Error Reading outfile:" << outfile << "***";
        return status;
    }

    if (PIL_OK != (status = PILGetBool("par_useAlgo", &useAlgo))) {
        LOG(ERROR) << "***Error Reading useAlgo parameter***";
        return status;
    }
    if (useAlgo == YES) {
        if (PIL_OK != (status = PILGetFname("par_aspectfile", aspectfile))) {
            LOG(ERROR) << "***Error Reading aspectfile:" << aspectfile << "***";
            return status;
        }
        if (PIL_OK != (status = PILGetFname("par_maskfile8100", maskfile8100))) {
            LOG(ERROR) << "***Error Reading Mask file:" << maskfile8100 << "***";
            return status;
        }
        if (PIL_OK != (status = PILGetString("par_ra", ra))) {
            LOG(ERROR) << "***Error Reading RA***";
            return status;
        }
        if (PIL_OK != (status = PILGetString("par_dec", dec))) {
            LOG(ERROR) << "***Error Reading DEC***";
            return status;
        }
        if (PIL_OK != (status = PILGetReal4("par_threshold", &threshold))) {
            LOG(ERROR) << "***Error Reading threshold***";
            return status;
        }
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

int Cztfindbadpix::read(char *infile, char *inputtype, char *CALDBbadpixfile,
        char *outfile, int useAlgo, float threshold, char *xcol, char *ycol,
        char *aspectfile, char *maskfile, char *ra, char *dec, int clobber, int history) {
    strcpy(this->infile, infile);
    strcpy(this->inputtype, inputtype);
    strcpy(this->CALDBbadpixelfile, CALDBbadpixfile);
    strcpy(this->outfile, outfile);
    this->useAlgo = useAlgo;
    if (xcol != NULL)
        strcpy(this->xcol, xcol);
    if (ycol != NULL)
        strcpy(this->ycol, ycol);
    if (aspectfile != NULL)
        strcpy(this->aspectfile, aspectfile);
    if (maskfile8100 != NULL)
        strcpy(this->maskfile8100, maskfile);
    if (ra != NULL)
        strcpy(this->ra, ra);
    if (dec != NULL)
        strcpy(this->dec, dec);
    this->threshold = threshold;
    this->clobber = clobber;
    this->history = history;
    
    return (EXIT_SUCCESS);
}

void Cztfindbadpix::display() {
    LOG(INFO) << "----------------------------------------------------------------------------";
    LOG(INFO) << "                          CZTFINDBADPIX PARAMETERS                            ";
    LOG(INFO) << "----------------------------------------------------------------------------";
    LOG(INFO) << "Input event file       : " << infile;
    LOG(INFO) << "Input Type             : " << inputtype;
    LOG(INFO) << "CALDB Bad Pixel File   : " << CALDBbadpixelfile;
    LOG(INFO) << "useAlgo                : " << useAlgo;
    LOG(INFO) << "Threshold              : " << threshold;
    LOG(INFO) << "Output Bad Pixel file  : " << outfile;
    LOG(INFO) << "X column name          : " << xcol;
    LOG(INFO) << "Y column name          : " << ycol;
    LOG(INFO) << "Aspect file            : " << aspectfile;
    LOG(INFO) << "Mask file              : " << maskfile8100;
    LOG(INFO) << "Right Ascension        : " << ra;
    LOG(INFO) << "Declination            : " << dec;
    LOG(INFO) << "Clobber                : " << clobber;
    LOG(INFO) << "History                : " << history;
    LOG(INFO) << "----------------------------------------------------------------------------";
}

int Cztfindbadpix::cztfindbadpix_process(){
    int status=0;
    int i,j,k=0; //counter variables
    string errorMsg="";
    fitsfile *fin; //fits pointer to event/ dpi file
    string badPixCALDBFileName; //path to badpix CALDB file
    vector<vector <unsigned char> > fullBadpixMap_2D;
    unsigned char* fullBadpixMap;

    // Checking whether output badpixel file exists or not
    // If yes then deletes it (for clobber=yes)
    // Otherwise raises an error
    if (FileExists(outfile)) {
        if (clobber == YES) {
            if (unlink(outfile) != 0) {
                LOG(ERROR) << "Error in deleting Output Badpixel File: " << outfile;
            }
        } else {
            LOG(INFO) << outfile << " already exists.";
            LOG(INFO) << "Use clobber=yes for overwriting the file.";
            return (EXIT_FAILURE);
        }
    }
    // Output badpixel file existence check finished.    
    
    // opening input event/dpi file
    fits_open_file(&fin, infile, READONLY, &status);
    if (status) {
        LOG(ERROR) << "Error in opening input file";
        fits_report_error(stderr, status);
        return status;
    }
    // input event/dpi file opened.
    
    if (create_badpix_file(fin, outfile, XSIZE, YSIZE)) {
        LOG(ERROR) << "***Error creating empty bad pixel file***";
        return (EXIT_FAILURE);
    }
    
    //reading Quality map from badpix file and storing it in a 2D vector of size
    //128x128 representing all the quadrants.
    
    badPixCALDBFileName = caldb_full_path_generator(CALDBbadpixelfile);
    if (badPixCALDBFileName == "") {
        LOG(ERROR) << "Not able to generate CALDB badpix file path.";
        return (EXIT_FAILURE);
    }
    fullBadpixMap_2D = create_full_badPixMap(badPixCALDBFileName, status);
    if(status){
        LOG(ERROR) << "Error in generating 2-Dimensional fullBadpixMap from CALDB badpix file " << badPixCALDBFileName;
        return (EXIT_FAILURE);
    }
    
    // Creating fullBadpixMap 1D Array
    fullBadpixMap = new unsigned char [XSIZE*YSIZE];
    
    for(i=0; i<XSIZE; i++){
        for(j=0; j<YSIZE; j++){
            fullBadpixMap[i*128+j]=fullBadpixMap_2D[i][j];
            DLOG(INFO) << (int)fullBadpixMap[i*128+j];
        }
    }
    // fullBadpixMap 1D array created
    
    // Detecting dead and noisy pixels if useAlgo==yes

    if (useAlgo == YES) {
        long dpi[XSIZE * YSIZE];
        if (getFullDPI2(fin, inputtype, xcol, ycol, dpi)) {
            return (EXIT_FAILURE);
        }
        //writeImg("!DPIInput",dpi,XSIZE,YSIZE);   //Just to check
        double ra_double = 0, dec_double = 0;
        if (strcmp(ra, "-") == 0 || strcmp(dec, "-") == 0) {
            if (getRaDec(aspectfile, 1, &ra_double, &dec_double)) {
                LOG(ERROR) << "***Error in getRaDec()***";
                return (EXIT_FAILURE);
            }
        } else {
            ra_double = atof(ra);
            dec_double = atof(dec);
        }

        //converting ra and dec to radians
        ra_double = ra_double * M_PI / 180;
        dec_double = dec_double * M_PI / 180;
        
        detectDeadNoisyPix(dpi, ra_double, dec_double, aspectfile, maskfile8100, threshold, fullBadpixMap);
    }
    
    // Dead and noisy pixels detected.
    
    
    

    //writing quality map to output file
    if (write_qmap(outfile, fullBadpixMap)) {
        LOG(ERROR) << "***Error in writing quality map to output file***";
        return (EXIT_FAILURE);
    }

    fits_close_file(fin, &status);
    if (status) {
        fits_report_error(stderr, status);
        return EXIT_FAILURE;
    }
    
    //Updating keywords in output file
    updateKeywords(outfile, modulename);
    
    //Writing history to output file
    if(history==YES){
        vector<string> vhistory;
        get_history(vhistory);
        writeHistory(outfile, vhistory);
    }
    //History written in output badpixel file
    
    return status;
}

// writing quality map file
int write_qmap(char *outfile, unsigned char *qualitymap) {

    long ngoodpix = 0, ndeadpix = 0, nnoisypix = 0, nbadpix = 0;

    for (int i = 0; i < XSIZE * YSIZE; i++) {
        switch (qualitymap[i]) {
            case GOODPIX: ngoodpix++;
                break;
            case DEADPIX: ndeadpix++;
                nbadpix++;
                break;
            case NOISYPIX: nnoisypix++;
                nbadpix++;
                break;
            case BADPIX: nbadpix++;
        }
    }
    LOG(INFO) << "GOOD PIXELS-------:" << ngoodpix;
    LOG(INFO) << "DEAD PIXELS-------:" << ndeadpix;
    LOG(INFO) << "NOISY PIXELS------:" << nnoisypix;
    LOG(INFO) << "BAD PIXELS--------:" << nbadpix;
    fitsfile *fqmap;
    int status = 0;
    fits_open_file(&fqmap, outfile, READWRITE, &status);
    if (status) {
        LOG(ERROR) << "***Error opening output quality map file***";
        fits_report_error(stderr, status);
        return (EXIT_FAILURE);
    }
    long fpixel[2];
    fpixel[0] = fpixel[1] = 1;
    fits_write_pix(fqmap, TBYTE, fpixel, XSIZE*YSIZE, qualitymap, &status);
    if (status) {
        LOG(ERROR) << "***Error writing pixels in output quality map file***";
        fits_report_error(stderr, status);
        return (EXIT_FAILURE);
    }
    fits_write_key(fqmap, TLONG, "NGOOD", &ngoodpix, NULL, &status);
    fits_write_key(fqmap, TLONG, "NDEAD", &ndeadpix, NULL, &status);
    fits_write_key(fqmap, TLONG, "NNOISY", &nnoisypix, NULL, &status);
    fits_write_key(fqmap, TLONG, "NBAD", &nbadpix, NULL, &status);
    fits_close_file(fqmap, &status);
    if (status) {
        LOG(ERROR) << "***Error closing output quality map file***";
        fits_report_error(stderr, status);
        return (EXIT_FAILURE);
    }
    return (EXIT_SUCCESS);
}

//function to create bad pixel file 
int create_badpix_file(fitsfile *fin, char *outfile, int xsize, int ysize) {
    int status = 0;
    fitsfile *fbad;
    fits_create_file(&fbad, outfile, &status);
    if (status) {
        LOG(ERROR) << "Error creating output bad pixel file";
        fits_report_error(stderr, status);
        return status;
    }
    int bitpix = BYTE_IMG;
    long naxes[2];
    naxes[0] = xsize;
    naxes[1] = ysize;
    int naxis = 2;
    fits_create_img(fbad, bitpix, naxis, naxes, &status);
    if (status) {
        LOG(ERROR) << "Error creating image for bad pixel quality map";
        fits_report_error(stderr, status);
        return status;
    }

    //moving to hdu 1 in input file
    fits_movabs_hdu(fin, 1, NULL, &status);
    if (status) {
        LOG(ERROR) << "Error moving to first hdu in input file";
        fits_report_error(stderr, status);
        return status;
    }

    //Keywords in bad pixel file from input file
    if (copyUserKeyWords(fin, fbad)) {
        LOG(ERROR) << "***Error copying keywords from input file***";
        return (EXIT_FAILURE);
    }
    char extname[] = "QUALITYMAP";
    fits_write_key(fbad, TSTRING, "EXTNAME", extname, NULL, &status);
    int goodpixval = GOODPIX, noisypixval = NOISYPIX, deadpixval = DEADPIX, badpixval = BADPIX;

    fits_write_key(fbad, TINT, GOODPIX_VAL_KEYWORD, &goodpixval, "Good pixel value", &status);
    fits_write_key(fbad, TINT, DEADPIX_VAL_KEYWORD, &deadpixval, "Dead pixel value", &status);
    fits_write_key(fbad, TINT, NOISYPIX_VAL_KEYWORD, &noisypixval, "Noisy pixel value", &status);
    fits_write_key(fbad, TINT, BADPIX_VAL_KEYWORD, &badpixval, "Bad pixel value", &status);
    fits_close_file(fbad, &status);
    if (status) {
        LOG(ERROR) << "Error closing bad pixel quality map file";
        fits_report_error(stderr, status);
        return status;
    }
    return (EXIT_SUCCESS);
}

int Cztfindbadpix::get_history(vector<string> &vhistory){
    //char *user=getlogin();
    char *user = getenv("USER");
	char temp[MAX_KEYWORD_SIZE];
    string str="Module run by "+(string)user;
    vhistory.push_back(str);
    vhistory.push_back("Parameter List START for "+(string)modulename);
    vhistory.push_back("P1 infile="+(string)infile);
    vhistory.push_back("P2 inputtype="+(string)inputtype);
    vhistory.push_back("P3 CALDBbadpixelfile="+(string)CALDBbadpixelfile);
    vhistory.push_back("P4 outfile="+(string)outfile);

    if(clobber==YES) 
        vhistory.push_back("P5 clobber=yes");
    else
        vhistory.push_back("P5 clobber=no");
    if(history==YES)
        vhistory.push_back("P6 history=yes");
    else
        vhistory.push_back("P6 history=no");

    if (useAlgo == YES) {
        vhistory.push_back("P7 useAlgo=yes");
        sprintf(temp, "%d", threshold);
        vhistory.push_back("P8 threshold="+(string)temp);
    }
    else {
        vhistory.push_back("P7 useAlgo=no");
    }
    vhistory.push_back("Parameter List END");
    return (EXIT_SUCCESS);
}

int detectDeadNoisyPix(long *dpi_pixels, double ra, double dec, char *aspectfile, char *maskfile,
        float threshold, unsigned char *qmap) {


    float emap[XSIZE * YSIZE], deviation[XSIZE * YSIZE];
    float dpi[XSIZE * YSIZE];
    for (int i = 0; i < XSIZE * YSIZE; i++) dpi[i] = (float) dpi_pixels[i];
    double theta_x, theta_y;

    if (to_thetaX_thetaY((string) aspectfile, ra, dec, theta_x, theta_y)){
        LOG(ERROR) << "Error in getting camera coordinates()";
        LOG(ERROR) << "***Error in detectDeadNoisyPix()***";
        return (EXIT_FAILURE);
    }
    
    theta_x=0;
    theta_y=0;
    //edited by Tanul Gupta
    MaskWeighting mskwt(maskfile, theta_x, theta_y);
    if (mskwt.run()) {
        LOG(ERROR) << "****Error in mask Weighting****";
        return (EXIT_FAILURE);
    }
    mskwt.getExposureMap(emap);

    //edited by Tanul Gupta
    //writeImg("!ExposureMap",emap,XSIZE,YSIZE);       //JUST TO CHECK;

    double coef[2];
    LOG(INFO) << "Fitting DPI counts and Exposure Map counts......";
    int flag = fitPolynomial<float>(emap, dpi, XSIZE*YSIZE, coef, 1); //coef[0]=B,coef[1]=S
    if (flag) {
        LOG(ERROR) << "***Error in fitting***";
        return (EXIT_FAILURE);
    }
    LOG(INFO) << "RMSE in fitting DPI counts and Exposure Map counts:" << RMSE(emap, dpi, XSIZE*YSIZE, coef, 1);
    LOG(INFO) << "Mean Source Count....:" << coef[1];
    LOG(INFO) << "Background...........:" << coef[0];
    double cmodel;
    for (int i = 0; i < XSIZE * YSIZE; i++) {
        cmodel = coef[1] * emap[i] + coef[0];
        deviation[i] = (float) dpi[i] - cmodel;
    }
    vector<float> set_d1, set_d2;
    vector<int> set_p1, set_p2; //for pixel locations
    for (int i = 0; i < XSIZE * YSIZE; i++) {
        if (emap[i] > SETBOUNDRY) {
            set_d1.push_back(deviation[i]);
            set_p1.push_back(i);
        } else {
            set_d2.push_back(deviation[i]);
            set_p2.push_back(i);
        }
    }
    //for first set
    double rms_pix = 0;
    rms_pix = getrms<float>(set_d1.data(), set_d1.size());
    double T = threshold*rms_pix;
    LOG(INFO) << "Number of pixels in set 1:" << set_d1.size();
    LOG(INFO) << "RMS :" << rms_pix << "\t" << T;
    for (int i = 0; i < set_d1.size(); i++) {
        if (set_d1[i]<(-1 * T)) qmap[set_p1[i]] = DEADPIX;
        else if (set_d1[i] > T) qmap[set_p1[i]] = NOISYPIX;
    }

    //for second set
    rms_pix = getrms<float>(set_d2.data(), set_d2.size());
    T = threshold*rms_pix;
    LOG(INFO) << "Number of pixels in set 2:" << set_d2.size();
    LOG(INFO) << "RMS :" << rms_pix << "\t" << T;
    for (int i = 0; i < set_d2.size(); i++) {
        if (set_d2[i] > T) qmap[set_p2[i]] = NOISYPIX;
        else if (set_d2[i]<(-1 * T)) qmap[set_p2[i]] = DEADPIX;
    }
    return (EXIT_SUCCESS);
}

int getFullDPI2(fitsfile *fin, char *inputtype, char *xcol, char *ycol, long *dpi) {
    int status = 0;
    if (strncasecmp(inputtype, "Eve", 3) == 0) { //for event file input
        //make dpi from event file
        long qdpi[NUMQUAD][XPIX_QUAD * YPIX_QUAD];
        for (int i = EVENTDATA_HDU_BEGIN, j = 0; j < NUMQUAD; i++, j++) {
            LOG(INFO) << "HDU number:" << i;
            //            if (getDPI(fin, i, xcol, ycol, qdpi[j], XPIX_QUAD, YPIX_QUAD)) {
            //                LOG(ERROR) << "Error in dpi generation for quadrant " << j;
            //                return (EXIT_FAILURE);
            //            }

        }
        //char name[25];
        //sprintf(name,"!DPI_3");
        //writeImg(name,qdpi[3],XPIX_QUAD,YPIX_QUAD);

        if (makeFullImage(qdpi[0], qdpi[1], qdpi[2], qdpi[3], dpi)) {
            LOG(ERROR) << "Error in dpi generation from event data";
            return (EXIT_FAILURE);
        }
        //writeImg("!FULLDPI",dpi,XSIZE,YSIZE);
        LOG(INFO) << "Created FULLDPI";
    } else {
        fits_movnam_hdu(fin, IMAGE_HDU, "FULLDPI", 0, &status);
        if (status) {
            fits_report_error(stderr, status);
            return (EXIT_FAILURE);
        }
        long fpixel[2];
        fpixel[0] = fpixel[1] = 1;
        fits_read_pix(fin, TLONG, fpixel, XSIZE*YSIZE, NULL, dpi, NULL, &status);
        if (status) {
            fits_report_error(stderr, status);
            return (EXIT_FAILURE);
        }
    }
    return (EXIT_SUCCESS);
}

int getDPI2(fitsfile *fptr, char* extname, double emin, double emax, double tstart, double tstop,
        char *ecol, char *tcol, char *xcol, char *ycol, long *dpi_pixels) //size of dpi_pixels is XPIX_QUAD x YPIX_QUAD
{
    LOG(INFO) << "-- Generating DPI for HDU " << extname << "(present in Event File) --";
    int status = 0;
    char filename[FLEN_FILENAME];
    int ecol_n, tcol_n, xcol_n, ycol_n;
    long i = 0, j = 0, index = 0; //counter variables
    long nrows = 0, firstrow = 1, firstelem = 1;
    long totcount = 0;

    fits_file_name(fptr, filename, &status);
    LOG(INFO) << "Generating DPI for  " << filename;
    if (status) {
        LOG(ERROR) << "Error getting filename";
        fits_report_error(stderr, status);
        return (EXIT_FAILURE);
    }
    fits_movnam_hdu(fptr, BINARY_TBL, extname, 0, &status);

    if (status) {
        LOG(ERROR) << "Error in moving to HDU " << extname << " in " << filename;
        fits_report_error(stderr, status);
        return (EXIT_FAILURE);
    }

    fits_get_colnum(fptr, CASEINSEN, ecol, &ecol_n, &status);
    if (status) {
        LOG(ERROR) << "Error getting column number for " << ecol << " in file " << filename;
        fits_report_error(stderr, status);
        return (EXIT_FAILURE);
    }
    fits_get_colnum(fptr, CASEINSEN, tcol, &tcol_n, &status);
    if (status) {
        LOG(ERROR) << "Error getting column number for " << tcol << " in file " << filename;
        fits_report_error(stderr, status);
        return (EXIT_FAILURE);
    }
    fits_get_colnum(fptr, CASEINSEN, xcol, &xcol_n, &status);
    if (status) {
        LOG(ERROR) << "Error getting column number for " << xcol << " in file " << filename;
        fits_report_error(stderr, status);
        return (EXIT_FAILURE);
    }
    fits_get_colnum(fptr, CASEINSEN, ycol, &ycol_n, &status);
    if (status) {
        LOG(ERROR) << "Error getting column number for " << ycol << " in file " << filename;
        fits_report_error(stderr, status);
        return (EXIT_FAILURE);
    }

    //get number of rows in event file
    fits_get_num_rows(fptr, &nrows, &status);
    if (status) {
        fits_report_error(stderr, status);
        return (EXIT_FAILURE);
    }

    LOG(INFO) << "Number of rows in HDU " << extname << " : " << nrows;


    initArray<long>(dpi_pixels, XPIX_QUAD*YPIX_QUAD, 0); //declared and initialize array dpi_pixels[XPIX_QUAD*YPIX_QUAD] with  value=0
    double *time = new double[nrows];
    double *energy = new double[nrows];
    unsigned char *detx = new unsigned char[nrows];
    unsigned char *dety = new unsigned char[nrows];

    initArray<double>(time, nrows, 0);
    initArray<double>(energy, nrows, 0);
    initArray<unsigned char>(detx, nrows, 0);
    initArray<unsigned char>(dety, nrows, 0);

    LOG(INFO) << "Initialized....";
    for (i = 0; i < XPIX_QUAD * YPIX_QUAD; i++) {
        totcount = totcount + dpi_pixels[i];
    }

    if (nrows > 0) {

        fits_read_col(fptr, TDOUBLE, ecol_n, firstrow, firstelem, nrows, NULL, energy, NULL, &status);
        if (status) {
            fits_report_error(stderr, status);
            return (EXIT_FAILURE);
        }
        //reading time column
        fits_read_col(fptr, TDOUBLE, tcol_n, firstrow, firstelem, nrows, NULL, time, NULL, &status);
        if (status) {
            fits_report_error(stderr, status);
            return (EXIT_FAILURE);
        }
        //reading x column
        fits_read_col(fptr, TBYTE, xcol_n, firstrow, firstelem, nrows, NULL, detx, NULL, &status);
        if (status) {
            fits_report_error(stderr, status);
            return (EXIT_FAILURE);
        }
        //reading y column
        fits_read_col(fptr, TBYTE, ycol_n, firstrow, firstelem, nrows, NULL, dety, NULL, &status);
        if (status) {
            fits_report_error(stderr, status);
            return (EXIT_FAILURE);
        }

        for (i = 0; i < nrows; i++) {
            index = dety[i] * YPIX_QUAD + detx[i];
            if (index > (XPIX_QUAD * YPIX_QUAD)) {
                LOG(INFO) << "dety:" << dety[i] << "  detx:" << detx[i] << " Index:" << index;
                LOG(ERROR) << "Index Out of bounds";
                return (EXIT_FAILURE);
            }

            if (emin == 0 && emax == 0 && tstart == 0 && tstop == 0) { //for full dpi
                dpi_pixels[index]++;
            }
            if ((emin == 0 && emax == 0) && time[i] >= tstart && time[i] < tstop) {
                dpi_pixels[index]++;
            }
            if (energy[i] >= emin && energy[i] < emax && time[i] >= tstart && time[i] < tstop) {
                dpi_pixels[index]++;
            }
            if (energy[i] >= emin && energy[i] < emax && tstart == 0 && tstop == 0) {
                dpi_pixels[index]++;
            }

        }
        long totcount = 0;
        for (int i = 0; i < XPIX_QUAD * YPIX_QUAD; i++)
            totcount = totcount + dpi_pixels[i];
    } else {
        return (EXIT_SUCCESS);
    }


    LOG(INFO) << "-- DPI generated for HDU " << extname << "(present in Event File) --";

    delete[] time, energy, detx, dety, dpi_pixels;

    return (EXIT_SUCCESS);

}
