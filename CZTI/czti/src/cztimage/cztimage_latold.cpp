
#include "cztimage.h"

using namespace std;

cztimage::cztimage() {
    strcpy(modulename, "cztimage_v");
    strcat(modulename, VERSION);
}

cztimage::~cztimage() {
    delete[] mask, dpi, oversampleddpi, oversampledmask, rawskyimage;
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


    if (PIL_OK != (status = PILGetInt("oversamplingfactor", &oversamplingfactor))) {
        LOG(ERROR) << "***Error reading oversampling factor***";
        return status;
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

int cztimage::read(char *dpifile, char *maskfile64x64, char *maskfile,
        char *aspectfile, int oversamplingfactor,
        char *outfile, char* quad_no, int debugmode, int clobber, int history) {
    strcpy(this->dpifile, dpifile);
    strcpy(this->maskfile64x64, maskfile64x64);
    strcpy(this->maskfile, maskfile);

    strcpy(this->aspectfile, aspectfile);


    strcpy(this->outfile, outfile);
    strcpy(this->quad_no, quad_no);
    this->oversamplingfactor = oversamplingfactor;

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
    LOG(INFO) << "Input Aspect file                          :" << aspectfile;
    LOG(INFO) << "Oversampling factor                        :" << oversamplingfactor;
    LOG(INFO) << "Output file                                :" << outfile;
    LOG(INFO) << "Quadrants to be processed                  :" << quad_no;
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

    float crpix = (XSIZE * oversamplingfactor / NUMQUAD) / 2; //for oversampling factor of 2, it comes out to be 32.
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

    return (EXIT_SUCCESS);
}

int cztimage::cztimageProcess() {

    int status=0;
    if (FileExists(outfile)) {
        if (clobber == YES) {
            unlink(outfile);
        } else {
            LOG(ERROR) << "" << outfile << "  already exists";
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
    if (rawskyimage == NULL) {
        LOG(ERROR) << "Out of memory error";
        return (EXIT_FAILURE);
    }
    oversample();
    generateCorrelationMatrix_2d(oversampleddpi, oversampledmask, rawskyimage, YSIZE, XSIZE, oversamplingfactor);
    keepCentral(rawskyimage, YSIZE, XSIZE, oversamplingfactor, &skyimgsize); //retains 1/4th area from centre
    //raw sky image created and stored in rawskyimage array 


    fits_create_file(&fout, outfile, &status);
    if (status) {
        LOG(ERROR) << "***Error in output image file creation***";
        fits_report_error(stderr, status);
        return (EXIT_FAILURE);
    }


    writeSkyImage(); //writing image data to output files

    fits_close_file(fout, &status);


    if (addWCS()) {
        LOG(ERROR) << "***Error in adding WCS to output files***";
        return (EXIT_FAILURE);
    }

    //updating keywords in output file
    if (updateKeywords(outfile, modulename)) {
        LOG(ERROR) << "***Error in updating keywords***";
        return (EXIT_FAILURE);
    }

    //delete[] mask,dpi,oversampled_dpi,oversampled_mask;     
    //writing history to output file
    if (history == YES) {
        vector<string> vhistory;
        getHistory(vhistory);
        writeHistory(outfile, vhistory); //writes history to each HDU of output file


    }
    return (EXIT_SUCCESS);

}


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


    fits_write_key(fout, TINT, "OVERSMPF", &oversamplingfactor, "Oversampling factor", &status);
    if (status) {
        fits_report_error(stderr, status);
        return (EXIT_FAILURE);
    }


    return (EXIT_SUCCESS);
}

int cztimage::getHistory(vector<string> &vhistory) {
    //char *user = getlogin();
    char *user = getenv("USER");
	string str = "Module run by " + (string) user;
    vhistory.push_back(str);
    vhistory.push_back("Parameter List START for " + (string) modulename);
    vhistory.push_back("P1 dpifile=" + (string) dpifile);
    vhistory.push_back("P2 maskfile=" + (string) maskfile64x64);

    vhistory.push_back("P4 aspectfile=" + (string) aspectfile);
    vhistory.push_back("P5 outfile=" + (string) outfile);
    char temp[25];

    vhistory.push_back("P6 threshold=" + (string) temp);
    sprintf(temp, "%d", oversamplingfactor);
    vhistory.push_back("P7 oversamplingfactor=" + (string) temp);

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
