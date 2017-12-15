#include "cztimage.h"

using namespace std;

cztimage::cztimage() {
    strcpy(modulename, "cztimage_v");
    strcat(modulename, VERSION);
}

cztimage::~cztimage() {
    delete[] mask, dpi, oversampleddpi, oversampledmask, rawskyimage, nobkgdskyimage;
}

int cztimage::read(int argc, char **argv) {
    int status = 0;
    if (PIL_OK != (status = PILInit(argc, argv))) {
        LOG(ERROR) << "***Error Initializing PIL***";
        return status;
    }
    if (PIL_OK != (status = PILGetFname("dpifile", dpifile))) {
        LOG(ERROR) << "***Error reading input dpi file:" << dpifile << "***";
        return status;
    }
    if (PIL_OK != (status = PILGetFname("maskfile64x64", maskfile64x64))) {
        LOG(ERROR) << "***Error reading mask file with 64x64 pixels:" << maskfile64x64 << "***";
        return status;
    }

    if (PIL_OK != (status = PILGetFname("maskfile", maskfile))) {
        LOG(ERROR) << "***Error reading mask file:" << maskfile << "***";
        return status;
    }

    if (PIL_OK != (status = PILGetFname("shadowfile", shadowfile))) {
        LOG(ERROR) << "***Error reading shadow file:" << shadowfile << "***";
        return status;
    }
    if (PIL_OK != (status = PILGetFname("aspectfile", aspectfile))) {
        LOG(ERROR) << "***Error reading aspect file:" << aspectfile << "***";
        return status;
    }
    if (PIL_OK != (status = PILGetFname("outfile", outfile))) {
        LOG(ERROR) << "***Error reading output file:" << outfile << "***";
        return status;
    }


    if (PIL_OK != (status = PILGetString("quad_no", quad_no))) {
        LOG(ERROR) << "***Error reading quadrant:" << quad_no << "***";

        return status;
    }

    if (quad_check(quad_no)) {
        LOG(ERROR) << "***Quadrant values can only be 0 or 1 or 2 or 3***";
        status = quad_check(quad_no);
        return status;
    }



    if (PIL_OK != (status = PILGetReal4("threshold", &threshold))) {
        LOG(ERROR) << "***Error reading threshold***";
        return status;
    }
    if (PIL_OK != (status = PILGetInt("oversamplingfactor", &oversamplingfactor))) {
        LOG(ERROR) << "***Error reading oversampling factor***";
        return status;
    }
    if (PIL_OK != (status = PILGetFname("sourcelist", sourcelist))) {
        LOG(ERROR) << "***Error reading sourcelist file name***";
        return status;
    }
    if (PIL_OK != (status = PILGetReal4("source_strength_thr", &source_strength_thr))) {
        LOG(ERROR) << "***Error reading threshold for source strengths***";
        return status;
    }
    if (PIL_OK != (status = PILGetInt("nBkgdCoef", &nBkgdCoef))) {
        LOG(ERROR) << "***Error reading number of background coefficients --Allowed range is 1-6***";
        return status;
        if (nBkgdCoef > 6 || nBkgdCoef < 1) {
            LOG(ERROR) << "***Error reading number of background coefficients --Allowed range is 1-6***";
            return status;
        }
    }
    if (PIL_OK != (status = PILGetBool("debugmode", &debugmode))) {
        LOG(ERROR) << "***Error reading debugmode parameter***";
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
    PILClose(status);
    return (EXIT_SUCCESS);
}

int cztimage::read(char *dpifile, char *maskfile64x64, char *maskfile, char *shadowfile,
        char *aspectfile, float threshold, int oversamplingfactor, char *sourcelist, float source_strength_thr,
        char *outfile, char* quad_no, int nBkgdCoef, int debugmode, int clobber, int history) {
    strcpy(this->dpifile, dpifile);
    strcpy(this->maskfile64x64, maskfile64x64);
    strcpy(this->maskfile, maskfile);
    strcpy(this->shadowfile, shadowfile);
    strcpy(this->aspectfile, aspectfile);
    this->threshold = threshold;
    strcpy(this->sourcelist, sourcelist);
    strcpy(this->outfile, outfile);
    strcpy(this->quad_no, quad_no);
    this->oversamplingfactor = oversamplingfactor;
    this->source_strength_thr = source_strength_thr;
    this->nBkgdCoef = nBkgdCoef;
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
    LOG(INFO) << "Shadow file                                :" << shadowfile;
    LOG(INFO) << "Input Aspect file                          :" << aspectfile;
    LOG(INFO) << "Threshold                                  :" << threshold;
    LOG(INFO) << "Oversampling factor                        :" << oversamplingfactor;
    LOG(INFO) << "Output file                                :" << outfile;
    LOG(INFO) << "Quadrants to be processed                  :" << quad_no;
    LOG(INFO) << "No of background coef                      :" << nBkgdCoef;
    LOG(INFO) << "Clobber                                    :" << clobber;
    LOG(INFO) << "History                                    :" << history;
    LOG(INFO) << "----------------------------------------------------------------------------";
}

int cztimage::addWCS() {
    int status = 0;
    fitsfile *fptr;
    fits_open_file(&fptr, aspectfile, READONLY, &status);
    if (status) {
        LOG(ERROR) << "***Error opening file " << aspectfile;
        return (EXIT_FAILURE);
    }
    double RA, DEC, TWIST;

    fits_read_key(fptr, TDOUBLE, "RA", &RA, NULL, &status);
    if (status) {
        fits_report_error(stderr, status);
        return (EXIT_FAILURE);
    }

    fits_read_key(fptr, TDOUBLE, "DEC", &DEC, NULL, &status);
    if (status) {
        fits_report_error(stderr, status);
        return (EXIT_FAILURE);
    }

    fits_read_key(fptr, TDOUBLE, "TWIST", &TWIST, NULL, &status);
    if (status) {
        fits_report_error(stderr, status);
        return (EXIT_FAILURE);
    }

    fits_close_file(fptr, &status);
    if (status) {
        fits_report_error(stderr, status);
        return (EXIT_FAILURE);
    }

    float crpix = (XSIZE * oversamplingfactor / NUMQUAD) / 2;
    float cdelt = atan((double) DETECTORWIDTH / (float) (MASKHEIGHT * NO_PIX_X_PER_DET * oversamplingfactor));
    cdelt = cdelt * 180 / M_PI;
    LOG(INFO) << "CDELT:" << cdelt;
    LOG(INFO) << "Val:" << (float) DETECTORWIDTH / (float) (MASKHEIGHT * NO_PIX_X_PER_DET * oversamplingfactor);
    //adding WCS keywords to both files
    //for full image

    fits_open_file(&fptr, outfile, READWRITE, &status);
    if (status) {
        LOG(ERROR) << "***Error in opening file " << outfile << "***";
        return (EXIT_FAILURE);
    }

    fits_write_key(fptr, TSTRING, "CTYPE1", (char *) "RA---TAN", "", &status);
    fits_write_key(fptr, TFLOAT, "CRPIX1", &crpix, "", &status);
    fits_write_key(fptr, TFLOAT, "CDELT1", &cdelt, "", &status);
    fits_write_key(fptr, TFLOAT, "CRVAL1", &RA, "", &status);

    fits_write_key(fptr, TSTRING, "CTYPE2", (char *) "DEC--TAN", "", &status);
    fits_write_key(fptr, TFLOAT, "CRPIX2", &crpix, "", &status);
    fits_write_key(fptr, TFLOAT, "CDELT2", &cdelt, "", &status);
    fits_write_key(fptr, TFLOAT, "CRVAL2", &DEC, "", &status);

    fits_write_key(fptr, TFLOAT, "CROTA2", &TWIST, "", &status);

    fits_close_file(fptr, &status);
    if (status) {
        fits_report_error(stderr, status);
        return (EXIT_FAILURE);
    }

    //for background subtracted image
    fits_open_file(&fptr, nobkgdfile, READWRITE, &status);
    if (status) {
        LOG(ERROR) << "***Error in opening file " << outfile << "***";
        return (EXIT_FAILURE);
    }

    fits_write_key(fptr, TSTRING, "CTYPE1", (char *) "RA---TAN", "", &status);
    fits_write_key(fptr, TFLOAT, "CRPIX1", &crpix, "", &status);
    fits_write_key(fptr, TFLOAT, "CDELT1", &cdelt, "", &status);
    fits_write_key(fptr, TFLOAT, "CRVAL1", &RA, "", &status);

    fits_write_key(fptr, TSTRING, "CTYPE2", (char *) "DEC--TAN", "", &status);
    fits_write_key(fptr, TFLOAT, "CRPIX2", &crpix, "", &status);
    fits_write_key(fptr, TFLOAT, "CDELT2", &cdelt, "", &status);
    fits_write_key(fptr, TFLOAT, "CRVAL2", &DEC, "", &status);

    fits_write_key(fptr, TFLOAT, "CROTA2", &TWIST, "", &status);

    fits_close_file(fptr, &status);
    if (status) {
        fits_report_error(stderr, status);
        return (EXIT_FAILURE);
    }
    return (EXIT_SUCCESS);
}

int cztimage::cztimageProcess() {

    strcpy(nobkgdfile, outfile);
    strcat(nobkgdfile, ".nobkgd");

    if (FileExists(outfile) || FileExists(nobkgdfile)) {
        if (clobber == YES) {
            unlink(outfile);
            unlink(nobkgdfile);
        } else {
            LOG(ERROR) << "" << outfile << " /  " << nobkgdfile << "  already exists";
            LOG(ERROR) << "Use clobber=yes for overwriting the file";
            return (EXIT_FAILURE);
        }
    }

    mask = new float[YSIZE * XSIZE];

    dpi = new float[YSIZE * XSIZE];
    if (dpi == NULL || mask == NULL) {
        LOG(ERROR) << "Out of memory error";
        return (EXIT_FAILURE);
    }

    /*intializing mask array and dpi array to contain zeros*/
    for (int i = 0; i <= 16384; i++) {
        mask[i] = 0;
        dpi[i] = 0;
    }

    float intpart = 0;
    float power = (float) log(oversamplingfactor) / (float) (log(2));
    float fraction = modf(power, &intpart);
    if (fraction > 0) {
        LOG(ERROR) << "***Over sampling must be provided in powers of 2***";
        return (EXIT_FAILURE);
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
    nobkgdskyimage = new float[size * size];
    if (rawskyimage == NULL || nobkgdskyimage == NULL) {
        LOG(ERROR) << "Out of memory error";
        return (EXIT_FAILURE);
    }
    oversample();
    generateCorrelationMatrix_2d(oversampleddpi, oversampledmask, rawskyimage, YSIZE, XSIZE, oversamplingfactor);
    keepCentral(rawskyimage, YSIZE, XSIZE, oversamplingfactor, &skyimgsize); //retains 1/4th area from centre
    //raw sky image created and stored in rawskyimage array 


    // this part of the code deals with identifying contribution from known sources and background and then 
    // subtracting it from the DPI.
    fitsfile *fptr;
    int status = 0;
    fits_open_file(&fptr, shadowfile, READONLY, &status);
    if (status) {
        fits_report_error(stderr, status);
        return (EXIT_FAILURE);
    }
    int nhdu = 0;
    fits_get_num_hdus(fptr, &nhdu, &status);
    if (status) {
        fits_report_error(stderr, status);
        return (EXIT_FAILURE);
    }
    if (nhdu < 2) {
        LOG(INFO) << "No known shadow found in file";
        return (EXIT_SUCCESS);
    }

    // reading known shadows from shadow file generated using cztpsfshadowgen
    knownSources = nhdu - 1;
    LOG(INFO) << "Number of known sources found in field of view: " << knownSources;
    knownShadows = allocateMemory<float>(knownSources, XSIZE * YSIZE);
    float *shadow; //variable to store individual shadows
    shadow = new float[XSIZE * YSIZE];

    for (int i = 0; i < XSIZE * YSIZE; i++) {
        shadow[i] = 0;
    }

    long fpixel[2];
    fpixel[0] = fpixel[1] = 1;
    for (int i = 2; i <= nhdu; i++) {

        fits_movrel_hdu(fptr, 1, NULL, &status);
        if (status) {
            fits_report_error(stderr, status);
            return (EXIT_FAILURE);
        }
        fits_read_pix(fptr, TFLOAT, fpixel, XSIZE*YSIZE, NULL, shadow, NULL, &status);
        if (status) {
            fits_report_error(stderr, status);
            return (EXIT_FAILURE);
        }
        for (int j = 0; j < XSIZE * YSIZE; j++) {
            knownShadows[i - 2][j] = shadow[j];
            //cout << knownShadows[i - 2][j] << "----";
        }

    }


    delete[] shadow;
    fits_close_file(fptr, &status);
    if (status) {
        fits_report_error(stderr, status);
        return (EXIT_FAILURE);
    }

    LOG(INFO) << "Known Sources shadows read and stored in array knownShadows";
    // shadows read and file closed

    bkgdshadows = allocateMemory<float>(nBkgdCoef, XSIZE * YSIZE);
    getBkgdShadows();

    knownSourceSubtractedDPI();
    // background and known sources have been removed from the DPI, the only thing left is to generate image 
    // and identify unknown sources and then calculate their flux and position.


    oversample(); //to oversample dpi and mask


    generateCorrelationMatrix_2d(oversampleddpi, oversampledmask, nobkgdskyimage, YSIZE, XSIZE, oversamplingfactor);
    keepCentral(nobkgdskyimage, YSIZE, XSIZE, oversamplingfactor, &skyimgsize); //retains 1/4th area from centre
    //raw sky image created and stored in rawskyimage array                                                                                                                                                                                                                                                                                                                                                                                                                                                                     writeSkyImage(par.dpifile,par.outfile,rawimg,newsize,par.oversamplingfactor);

    identifyPeaks(); //this is the function that return nsources

    //computing shadows
    shadows = allocateMemory<float>(nsources, XSIZE * YSIZE); //buffer to store shadows for peaks obtained 
    //float shadow[YSIZE][XSIZE];    
    //generating shadows from peaks


    for (int i = 0; i < nsources; i++) {
        LOG(INFO) << thetax[i] << " " << thetay[i];
        //cout<<thetax[i]<<" "<<thetay[i];

        MaskWeighting mskwt(maskfile, thetax[i], thetay[i]);
        if (mskwt.run()) {
            LOG(ERROR) << "****Error in mask Weighting****";
            return (EXIT_FAILURE);
        }
        mskwt.getShadow(shadow, XSIZE * YSIZE);

        for (int j = 0; j < XSIZE * YSIZE; j++)
            shadows[i][j] = shadow[j];
    }
    if (debugmode == YES) {
        for (int i = 0; i < nsources; i++) {
            char name[512];
            sprintf(name, "shadow_%d", i);
            ofstream fb(name, ios::binary);
            fb.write((char *) shadows[i], sizeof (float) *XSIZE * YSIZE);
            fb.close();
        }
    }

    /*---------------------END OF JUST TO CHECK------------------*/


    LOG(INFO) << "Fitting sources......";
    int prev_peaks = nsources, prev_nbkgd = nBkgdCoef;
    while (1) {
        if (linearfitDPI()) {
            LOG(ERROR) << "***Error in linearfitDPI***";
            return (EXIT_FAILURE);
        }

        //cout<<"prev_peaks:"<<prev_peaks<<"     prev_nbkgd:"<<prev_nbkgd;
        if (prev_peaks == nsources && prev_nbkgd == nBkgdCoef) break;
        prev_peaks = nsources;
        prev_nbkgd = nBkgdCoef;
        LOG(INFO) << "prev_peaks:" << prev_peaks << "     prev_nbkgd:" << prev_nbkgd;
    }
    LOG(INFO) << "Number of sources:  " << nsources;
    LOG(INFO) << "Fitting Complete";

    fits_open_file(&fdpi, dpifile, READONLY, &status);
    if (status) {
        fits_report_error(stderr, status);
        LOG(ERROR) << "***Error opening input dpi file " << dpifile << "***";
        return (EXIT_FAILURE);
    }
    //creating output files 
    fits_create_file(&fout, outfile, &status);
    if (status) {
        LOG(ERROR) << "***Error in output image file creation***";
        fits_report_error(stderr, status);
        return (EXIT_FAILURE);
    }

    fits_create_file(&fnobkgd, nobkgdfile, &status);
    if (status) {
        LOG(ERROR) << "Error in no background output image file creation";
        fits_report_error(stderr, status);
        return (EXIT_FAILURE);
    }

    writeSkyImage(); //writing image data to output files

    fits_close_file(fout, &status);
    fits_close_file(fnobkgd, &status);

    //adding sources to sourcelist
    ofstream fout(sourcelist);
    if (!fout) {
        LOG(ERROR) << "***Error in opening file " << sourcelist << "***";
        return (EXIT_FAILURE);
    }

    //finding RA and DEC for sources found
    double ra_rad, dec_rad; //for radians
    ra.clear();
    dec.clear();
    fout << setw(15) << "RA" << setw(15) << "DEC" << setw(15) << "FLUX";
    for (int i = 0; i < thetax.size(); i++) {
        getRaDec(aspectfile, thetax[i], thetay[i], &ra_rad, &dec_rad);
        ra.push_back(ra_rad * 180 / M_PI);
        dec.push_back(dec_rad * 180 / M_PI);
        fout << endl << setw(15) << ra[i] << setw(15) << dec[i] << setw(15) << flux[i] << setw(15) << thetax[i] << setw(15) << thetay[i];
        ;
    }
    fout.close();

    if (addWCS()) {
        LOG(ERROR) << "***Error in adding WCS to output files***";
        return (EXIT_FAILURE);
    }

    //updating keywords in output file
    if (updateKeywords(outfile, modulename)) {
        LOG(ERROR) << "***Error in updating keywords***";
        return (EXIT_FAILURE);
    }
    if (updateKeywords(nobkgdfile, modulename)) {
        LOG(ERROR) << "***Error in updating keywords***";
        return (EXIT_FAILURE);
    }
    //delete[] mask,dpi,oversampled_dpi,oversampled_mask;     
    //writing history to output file
    if (history == YES) {
        vector<string> vhistory;
        getHistory(vhistory);
        writeHistory(outfile, vhistory); //writes history to each HDU of output file
        writeHistory(nobkgdfile, vhistory);

    }
    return (EXIT_SUCCESS);

}

/******************************************************************************************************
 The functions described below are related to matrix arithmetic.
 *****************************************************************************************************/
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


/*****************************************************************************************************
 Matrix arithmetic functions over.
 ******************************************************************************************************/

/*******************************************************************************************************
 FFT related functions
 *******************************************************************************************************/
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

/******************************************************************************************************
 FFT related functions over
 *******************************************************************************************************/
//Assumption - The Mask Image file contains dpi in 4 quadrants ie. extension no 2,3,4,5
//Each quadrant mask is of size 64 x 64

int cztimage::getMaskArray() {
    fitsfile *fptr;
    int status = 0, quadid = 0;
    int npixQuad = XSIZE * YSIZE / NUMQUAD;
    float *quadmask = new float[npixQuad];

    /*code to process quadrants*/
    int* quadArray;
    int noccurrence;
    stringFinder(quad_no, ",", 0, &noccurrence);
    int nelems_quad = noccurrence + 1;
    quadArray = new int[nelems_quad];

    if (csvStrToInt(quad_no, ",", quadArray, &nelems_quad)) {
        LOG(ERROR) << "***Error in converting quadrant string array into integer array***";
    }
    /*code to process quadrants*/

    fits_open_file(&fptr, maskfile64x64, READONLY, &status);
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

    for (int i = 0; i < nelems_quad; i++) {

        quadid = quadArray[i];
        LOG(INFO) << "getting mask for quadrant " << quadid;
        char extname[FLEN_VALUE];
        quadToHDU(quadid, extname);

        fits_movnam_hdu(fptr, IMAGE_HDU, extname, 0, &status);
        if (status) {
            LOG(ERROR) << "*** Error in moving to specified HDU number: " << quadid << "***";
            fits_report_error(stderr, status);
            return (EXIT_FAILURE);
        }

        fits_read_key(fptr, TINT, "QUADID", &qid, NULL, &status);
        if (status) {
            LOG(ERROR) << "*** Error in reading quadrant id from FITS file ***";
            fits_report_error(stderr, status);
            return (EXIT_FAILURE);
        }

        fits_read_pix(fptr, TFLOAT, fpixel, npixQuad, NULL, quadmask, NULL, &status);
        if (status) {
            LOG(ERROR) << "***Error reading mask pixels***";
            fits_report_error(stderr, status);
            return (EXIT_FAILURE);
            ;
        }
        r = (qid / NUMQUAD_ROW)*(YSIZE / NUMQUAD_COL);
        c = (qid % NUMQUAD_ROW)*(XSIZE / NUMQUAD_ROW);
        index = 0;
        for (int u = r; u < r + (YSIZE / NUMQUAD_COL); u++)
            for (int v = c; v < c + (XSIZE / NUMQUAD_ROW); v++)
                mask[u * XSIZE + v] = quadmask[index++];


    }

    fits_close_file(fptr, &status);
    if (status) {
        LOG(ERROR) << "***Error closing maskfile " << maskfile64x64 << " ***";
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
    float *quaddpi = new float[npixQuad];
    
    //code to initialize dpi array and fill it with zeros
    for(int i=0; i<XSIZE*YSIZE; i++){
        dpi[i]=0;
    }
    
    /*code to process quadrants*/
    int* quadArray;
    int noccurrence;
    stringFinder(quad_no, ",", 0, &noccurrence);
    int nelems_quad = noccurrence + 1;
    int quadid=0;
    quadArray = new int[nelems_quad];

    if (csvStrToInt(quad_no, ",", quadArray, &nelems_quad)) {
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
    for (int i = 0; i < nelems_quad; i++) {
        quadid = quadArray[i];
        LOG(INFO) << "getting dpi for quadrant " << quadid;
        char extname[FLEN_VALUE];
        DPIquadToHDU(quadid, extname);

        fits_movnam_hdu(fptr, IMAGE_HDU, extname, 0, &status);
         if (status) {
            LOG(ERROR) << "*** Error in moving to specified HDU number: " << quadid << "***";
            fits_report_error(stderr, status);
            return (EXIT_FAILURE);
        }      
        
        fits_read_key(fptr, TINT, "QUADID", &qid, NULL, &status);
        if (hdutype != IMAGE_HDU) {
            LOG(ERROR) << "Image HDU not found in DPI file at HDU no " << i + 2;
            return (EXIT_FAILURE);
        }
        if (qid == quadid) {
            fits_read_pix(fptr, TFLOAT, fpixel, npixQuad, NULL, quaddpi, NULL, &status);
            if (status != 0) {
                LOG(ERROR) << "***Error reading DPI pixels***";
                fits_report_error(stderr, status);
                return (EXIT_FAILURE);
                ;
            }
            r = (qid / NUMQUAD_ROW)*(YSIZE / NUMQUAD_COL);
            c = (qid % NUMQUAD_ROW)*(XSIZE / NUMQUAD_ROW);
            index = 0;
            for (int u = r; u < r + (YSIZE / NUMQUAD_COL); u++)
                for (int v = c; v < c + (XSIZE / NUMQUAD_ROW); v++)
                    dpi[u * XSIZE + v] = quaddpi[index++];
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
    LOG(INFO) << "Oversampling initiated";
    size = XSIZE* oversamplingfactor; //oversampled rows/columns
    oversampleddpi = new float[size * size];
    oversampledmask = new float[size * size];
    int x, y;
    LOG(INFO) << "New Size: " << size << " x " << size;
    for (int i = 0; i < YSIZE; i++)
        for (int j = 0; j < XSIZE; j++)
            for (int k = i * oversamplingfactor; k < i * oversamplingfactor + oversamplingfactor; k++)
                for (int l = j * oversamplingfactor; l < j * oversamplingfactor + oversamplingfactor; l++) {
                    oversampleddpi[k * size + l] = (float) dpi[i * XSIZE + j] / (float) (oversamplingfactor * oversamplingfactor);
                    oversampledmask[k * size + l] = (float) mask[i * XSIZE + j];
                }
    LOG(INFO) << "Oversampling completed";
}

int cztimage::identifyPeaks() {
    vector<int> row, col;
    vector<float> intensity;
    ;
    int r, c;
    int rowcen = skyimgsize / 2;
    int colcen = skyimgsize / 2;
    double val = (double) (DETECTORWIDTH / (double) (16.0 * oversamplingfactor * MASKHEIGHT));
    //this value is used in computation of thetax & thetay from x.y or row,col
    double theta;
    double thr = getrms<float>(rawskyimage, skyimgsize * skyimgsize) * threshold;
    flux.clear();
    thetax.clear();
    thetay.clear();
    //selecting peaks from the image
    for (int i = 0; i < skyimgsize * skyimgsize; i++) {
        r = (i / skyimgsize);
        c = (i % skyimgsize);
        if (rawskyimage[i] > thr) {
            intensity.push_back(rawskyimage[i]);
            row.push_back(r);
            col.push_back(c);
        }
        //cout<<rawimg[i]<<" "<<r<<" "<<c;    
    }
    LOG(INFO) << "Size :" << intensity.size();
    LOG(INFO) << "First cut peaks detected-";
    LOG(INFO);

    if (row.size() == 0) {
        LOG(ERROR) << "***No Peaks detected in image***";
        LOG(ERROR) << "***Run with lower threshold***";
        return (EXIT_FAILURE);
    }

    LOG(INFO) << "Refining Peaks.......";
    //refining peaks
    float max = 0;
    int winsize;
    if (oversamplingfactor < 8)
        winsize = 9;
    else
        winsize = 3 * oversamplingfactor;
    LOG(INFO) << "Window size:" << winsize;
    if (winsize % 2 == 0) winsize = winsize - 1; //if winsize is even, make it odd
    int start_r, end_r, start_c, end_c;
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
        //cout<<"Peak:"<<row[i]<<"  "<<col[i];
        for (int k = start_r; k <= end_r; k++) {
            for (int l = start_c; l <= end_c; l++) {
                if (rawskyimage[k * skyimgsize + l] > max) {
                    max = rawskyimage[k * skyimgsize + l];
                    row[i] = k;
                    col[i] = l;
                }
            }
        }
        //cout<<max<<" "<<row[i]<<" "<<col[i];
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
            //cout<<i<<"  Flag false";
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
        LOG(INFO) << r << "  " << c << "  " << flux[i];
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

void generateCorrelationMatrix_2d(float *dph, float *mask, float *correlationMatrix,
        int TOTALROWS, int TOTALCOLS, int factor) {
    int n = TOTALROWS * TOTALCOLS * factor*factor;
    int fftN = 2 * n;
    int i = 0, j = 0;
    float *dph_complex, *mask_complex, *correlation_complex;
    dph_complex = (float*) malloc(sizeof (float) *(fftN + 1));
    mask_complex = (float*) malloc(sizeof (float) *(fftN + 1));
    correlation_complex = (float*) malloc(sizeof (float) *(fftN + 1));
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

    doFft(dph_complex, n, 1);
    doFft(mask_complex, n, 1);
    for (i = 1; i <= fftN; i += 2) {
        correlation_complex[i] = (mask_complex[i] * dph_complex[i])+(mask_complex[i + 1] * dph_complex[i + 1]); // to be altered and checked
        correlation_complex[i + 1] = (mask_complex[i + 1] * dph_complex[i])-(mask_complex[i] * dph_complex[i + 1]); // to be altered and checked

        correlation_complex[i] /= n;
        correlation_complex[i + 1] /= n;
    }
    doFft(correlation_complex, n, -1);
    for (i = 1, j = 0; i <= fftN; i += 2, j++) {
        correlationMatrix[j] = sqrt((correlation_complex[i] * correlation_complex[i])+(correlation_complex[i + 1] * correlation_complex[i + 1]));
    }
    swapComplex(correlationMatrix, TOTALROWS, TOTALCOLS, factor);
    free(dph_complex);
    free(mask_complex);
    return;
}

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

int cztimage::knownSourceSubtractedDPI() {
    LOG(INFO) << "Creating matrix A";
    int cols_A = knownSources + nBkgdCoef;
    LOG(INFO) << "Number of known sources in the field of view are: " << knownSources;
    LOG(INFO) << "Number of background coefficients: " << nBkgdCoef;
    int rows_A = XSIZE*YSIZE;
    float *A = new float[cols_A * rows_A];
    if (A == NULL) {
        LOG(ERROR) << "Out of Memory Error";
        return (EXIT_FAILURE);
    }
    LOG(INFO) << "Copying shadows in Matrix A";

    for (int j = 0; j < knownSources; j++)
        for (int i = 0; i < rows_A; i++) {
            if ((i * cols_A + j)>(rows_A * cols_A)) {
                LOG(ERROR) << "Out of Index Error @ " << j << "," << i;
                LOG(ERROR) << "Size of array is " << rows_A * cols_A << " and index is " << (i * rows_A + j);
                return (EXIT_FAILURE);
            }
            //cout<<"i:"<<i<<" j:"<<j;

            //LOG(INFO) << knownShadows[j][i];
            A[i * cols_A + j] = knownShadows[j][i];

        }
    //setting background    
    LOG(INFO) << "Copying backgrounds...";
    for (int j = knownSources; j < cols_A; j++)
        for (int i = 0; i < rows_A; i++) {
            if ((i * cols_A + j)>(rows_A * cols_A)) {
                LOG(ERROR) << "Out of Index Error@ " << j << "," << i;
                LOG(ERROR) << "Size of array is " << rows_A * cols_A << " and index is " << (i * rows_A + j);
                return (EXIT_FAILURE);
            }
            //cout<<"i:"<<i<<" j:"<<j;
            A[i * cols_A + j] = bkgdshadows[j - knownSources][i];
        }
    LOG(INFO) << "Matrix A created";

    //matrix B is same as DPI
    //creating coefficient matrix
    int cols_B = 1; //columns in B will be 1
    int rows_B = rows_A;

    float *Coef = new float[cols_A];
    if (Coef == NULL) {
        LOG(ERROR) << "Out of Memory Error";
        return (EXIT_FAILURE);
    }
    LOG(INFO) << "Calling function multiply_AinvB.... ";
    if (multiply_AinvB(A, dpi, rows_A, cols_A, cols_B, Coef)) {
        LOG(ERROR) << "***Error in multiply_AinvB function***";
        return (EXIT_FAILURE);
    }

    // Subtracting background from dpi
    for (int i = 1; i < nBkgdCoef; i++)
        for (int j = 0; j < XSIZE * YSIZE; j++)
            if (Coef[cols_A - nBkgdCoef - 1 + i] >= threshold)
                bkgdshadows[0][j] = bkgdshadows[0][j] + Coef[cols_A - nBkgdCoef - 1 + i] * bkgdshadows[i][j]; //this will give total component of background

    for (int i = 0; i < XSIZE * YSIZE; i++)
        dpi[i] = dpi[i] - bkgdshadows[0][i];


    //Subtracting known source shadow from DPI
    for (int i = 1; i <= knownSources; i++) {
        LOG(INFO) << "known Source Coefficinent " << i << ": " << Coef[i - 1];

        for (int j = 0; j < XSIZE * YSIZE; j++) {

            if (Coef[i - 1] >= threshold)
                knownShadows[0][j] = knownShadows[0][j] + Coef[i - 1] * knownShadows[i][j];
        }//this will give total component of background
    }
    for (int i = 0; i < XSIZE * YSIZE; i++)
        dpi[i] = dpi[i] - knownShadows[0][i];

}

/*****************************************************************************************************
 DESCRIPTION OF FUNCTION linearfitDPI
 * 
 * it first make matrix A where it stores all background and shadows.
 * then it declares a coefficient matrix having same number of rows as total number of sources and 
 * backgrounds.
 * then matrix B is created which stores the entire DPI of image to be processed.
 * coefficient matrix is the derived using AinvB.
 * coefficients are the compared with threshold till the time there are no unknown sources whose coefficient 
 * is less than the threshold supplied by the user.
 *****************************************************************************************************/
int cztimage::linearfitDPI() {
    //creating matrix A
    //cout<<"1.  Num of Sources - "<<nsources<<"    Backgrounds-"<<nBkgdCoef;
    //cout<<"";
    LOG(INFO) << "Creating matrix A.........";
    int cols_A = nsources;
    int rows_A = XSIZE*YSIZE;
    float *A = new float[cols_A * rows_A];
    if (A == NULL) {
        LOG(ERROR) << "Out of Memory Error";
        return (EXIT_FAILURE);
    }
    //setting shadows
    LOG(INFO) << "Copying shadows...";
    for (int j = 0; j < nsources; j++)
        for (int i = 0; i < rows_A; i++) {
            if ((i * cols_A + j)>(rows_A * cols_A)) {
                LOG(ERROR) << "Out of Index Error @ " << j << "," << i;
                LOG(ERROR) << "Size of array is " << rows_A * cols_A << " and index is " << (i * rows_A + j);
                return (EXIT_FAILURE);
            }
            //cout<<"i:"<<i<<" j:"<<j;
            A[i * cols_A + j] = shadows[j][i];
        }

    LOG(INFO) << "Matrix A created";
    if (debugmode == 1) {
        ofstream fout("matrix.txt"); //Just to check the matrices formed
        //writing A in file matrix.txt 
        for (int i = 0; i < rows_A; i++) {
            fout;
            for (int j = 0; j < cols_A; j++) {
                fout << setw(12) << A[i * cols_A + j];
            }
        }
        fout.close();
    }
    //matrix B is same as DPI
    //creating coefficient matrix
    int cols_B = 1; //columns in B will be 1
    int rows_B = rows_A;

    float *Coef = new float[cols_A];
    if (Coef == NULL) {
        LOG(ERROR) << "Out of Memory Error";
        return (EXIT_FAILURE);
    }
    LOG(INFO) << "Calling function multiply_AinvB.... ";
    if (multiply_AinvB(A, dpi, rows_A, cols_A, cols_B, Coef)) {
        LOG(ERROR) << "***Error in multiply_AinvB function***";
        return (EXIT_FAILURE);
    }

    int temp_nshadow = 0, temp_nbkgd = 0;
    float **temp_shadows = allocateMemory<float>(nsources, XSIZE * YSIZE);
    vector<double> temp_tx, temp_ty, temp_flux;
    temp_tx = thetax;
    temp_ty = thetay;
    temp_flux = flux;
    thetax.clear();
    thetay.clear();
    flux.clear();
    //copying shadows and background shadows to temporary arrays 
    for (int i = 0; i < nsources; i++) {
        for (int j = 0; j < XSIZE * YSIZE; j++) {
            temp_shadows[i][j] = shadows[i][j];
        }
    }

    int index = 0;

    LOG(INFO) << "The source strengths are-";
    for (int i = 0; i < nsources; i++) {
        LOG(INFO) << "Source Strength: " << i << "  : " << Coef[i];
        if (Coef[i] > source_strength_thr) {
            temp_nshadow++;
            thetax.push_back(temp_tx[i]);
            thetay.push_back(temp_ty[i]);
            flux.push_back((temp_flux[i]));
            for (int j = 0; j < XSIZE * YSIZE; j++)
                shadows[index][j] = temp_shadows[i][j];
            index++;
        }
    }


    freeMemory(temp_shadows, nsources);


    nsources = temp_nshadow;

    delete[] A, Coef;
    temp_ty.clear();
    temp_tx.clear();
    temp_flux.clear();
    //cout<<"2.  Num of Sources - "<<nsources<<"    Backgrounds-"<<nBkgdCoef;
    //cout<<"linearfitDPI complete";
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
    fits_write_pix(fout, TFLOAT, fpixel, skyimgsize*skyimgsize, rawskyimage, &status);
    if (status) {
        fits_report_error(stderr, status);
        return (EXIT_FAILURE);
    }

    if (copyUserKeyWords(fdpi, fout)) {
        LOG(ERROR) << "***Error in copying keywords from dpi file to output raw image***";
        return (EXIT_FAILURE);
    }
    fits_write_key(fout, TINT, "OVERSMPF", &oversamplingfactor, "Oversampling factor", &status);
    if (status) {
        fits_report_error(stderr, status);
        return (EXIT_FAILURE);
    }

    //writing background subtracted image
    fits_create_img(fnobkgd, bitpix, naxis, naxes, &status); //raw image
    if (status) {
        fits_report_error(stderr, status);
        return (EXIT_FAILURE);
    }
    fits_write_pix(fnobkgd, TFLOAT, fpixel, skyimgsize*skyimgsize, nobkgdskyimage, &status);
    if (status) {
        fits_report_error(stderr, status);
        return (EXIT_FAILURE);
    }

    if (copyUserKeyWords(fdpi, fnobkgd)) {
        LOG(ERROR) << "***Error in copying keywords from dpi file to output raw image***";
        return (EXIT_FAILURE);
    }
    fits_write_key(fnobkgd, TINT, "OVERSMPF", &oversamplingfactor, "Oversampling factor", &status);
    if (status) {
        fits_report_error(stderr, status);
        return (EXIT_FAILURE);
    }

    return (EXIT_SUCCESS);
}

int cztimage::getHistory(vector<string> &vhistory) {
    char *user = getlogin();
    string str = "Module run by " + (string) user;
    vhistory.push_back(str);
    vhistory.push_back("Parameter List START for " + (string) modulename);
    vhistory.push_back("P1 dpifile=" + (string) dpifile);
    vhistory.push_back("P2 maskfile=" + (string) maskfile64x64);
    vhistory.push_back("P3 shadowfile=" + (string) shadowfile);
    vhistory.push_back("P4 aspectfile=" + (string) aspectfile);
    vhistory.push_back("P5 outfile=" + (string) outfile);
    char temp[25];
    sprintf(temp, "%f", threshold);
    vhistory.push_back("P6 threshold=" + (string) temp);
    sprintf(temp, "%d", oversamplingfactor);
    vhistory.push_back("P7 oversamplingfactor=" + (string) temp);
    vhistory.push_back("P8 sourcelist=" + (string) sourcelist);
    if (clobber == YES)
        vhistory.push_back("P9 clobber=yes");
    else
        vhistory.push_back("P9 clobber=no");
    if (history == YES)
        vhistory.push_back("P10 history=yes");
    else
        vhistory.push_back("P10 history=no");
    vhistory.push_back("Parameter List END");
    return (EXIT_SUCCESS);
}

int cztimage::getBkgdShadows() {
    int u, v;
    int j;
    if (nBkgdCoef < 1 || nBkgdCoef > 6) return (EXIT_FAILURE);
    switch (nBkgdCoef) {
        case 6: for (int i = 0; i < YSIZE; i++) { //varying with xy2 direction
                for (j = 0; j < XSIZE; j++) {
                    bkgdshadows[5][i * XSIZE + j] = (float) (i * j * j) / (float) ((XSIZE - 1)*(YSIZE - 1)*(YSIZE - 1));
                }
            }
        case 5: for (int i = 0; i < YSIZE; i++) { //varying with x2y direction
                for (j = 0; j < XSIZE; j++) {
                    bkgdshadows[4][i * XSIZE + j] = (float) (i * i * j) / (float) ((XSIZE - 1)*(XSIZE - 1)*(YSIZE - 1));
                }
            }
        case 4: for (int i = 0; i < YSIZE; i++) { //varying with xy direction
                for (j = 0; j < XSIZE; j++) {
                    bkgdshadows[3][i * XSIZE + j] = (float) (i * j) / (float) ((YSIZE - 1)*(XSIZE - 1));
                }
            }
        case 3: for (int i = 0; i < YSIZE; i++) { //varying with y direction
                for (j = 0; j < XSIZE; j++) {
                    bkgdshadows[2][i * XSIZE + j] = (float) j / (float) (YSIZE - 1);
                }
            }
        case 2: for (int i = 0; i < YSIZE; i++) { //varying with x direction
                for (j = 0; j < XSIZE; j++) {
                    bkgdshadows[1][i * XSIZE + j] = (float) i / (float) (XSIZE - 1);
                }
            }
        case 1: for (int i = 0; i < YSIZE * XSIZE; i++)
                bkgdshadows[0][i] = 1;
    }
    return (EXIT_SUCCESS);
}
