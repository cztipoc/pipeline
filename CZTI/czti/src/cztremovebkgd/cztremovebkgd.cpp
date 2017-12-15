#include "cztremovebkgd.h"

using namespace std;

cztremovebkgd::cztremovebkgd() {
    strcpy(modulename, "cztremovebkgd_v");
    strcat(modulename, VERSION);
}

cztremovebkgd::~cztremovebkgd() {
    LOG(INFO)<<"Calling destructor cztremovebkgd";
////    if(dpi!=NULL) delete[] dpi; 
////    else LOG(INFO)<< "**dpi is NULL**";
////    LOG(INFO)<<"Cztremovebkgd destructor completed";
}

int cztremovebkgd::read(int argc, char **argv) {
    int status = 0;
    if (PIL_OK != (status = PILInit(argc, argv))) {
        LOG(ERROR) << "***Error Initializing PIL***";
        return status;
    }
    if (PIL_OK != (status = PILGetFname("dpifile", dpifile))) {
        LOG(ERROR) << "***Error reading input dpi file:" << dpifile << "***";
        return status;
    }

    if (PIL_OK != (status = PILGetFname("shadowfile", shadowfile))) {
        LOG(ERROR) << "***Error reading shadow file:" << shadowfile << "***";
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

    if (PIL_OK != (status = PILGetInt("nBkgdCoef", &nBkgdCoef))) {
        LOG(ERROR) << "***Error reading number of background coefficients --Allowed range is 1-6***";
        return status;
    }
    if (nBkgdCoef > 6 || nBkgdCoef < 1) {
        LOG(ERROR) << "***Error reading number of background coefficients --Allowed range is 1-6***";
        return EXIT_FAILURE;
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

int cztremovebkgd::read(char *dpifile, char *shadowfile, float threshold,
        char *outfile, char* quad_no, int nBkgdCoef, int debugmode, int clobber, int history) {
    strcpy(this->dpifile, dpifile);
    strcpy(this->shadowfile, shadowfile);
    this->threshold = threshold;
    ;
    strcpy(this->outfile, outfile);
    strcpy(this->quad_no, quad_no);
    this->nBkgdCoef = nBkgdCoef;
    this->clobber = clobber;
    this->history = history;
    return (EXIT_SUCCESS);
}

void cztremovebkgd::display() {
    LOG(INFO) << "----------------------------------------------------------------------------";
    LOG(INFO) << "                          CZTREMOVEBACKGROUND  PARAMETERS                            ";
    LOG(INFO) << "----------------------------------------------------------------------------";
    LOG(INFO) << "Input DPI file                             :" << dpifile;
    LOG(INFO) << "Shadow file                                :" << shadowfile;
    LOG(INFO) << "Threshold                                  :" << threshold;
    LOG(INFO) << "Output file                                :" << outfile;
    LOG(INFO) << "Quadrants to be processed                  :" << quad_no;
    LOG(INFO) << "No of background coef                      :" << nBkgdCoef;
    LOG(INFO) << "Clobber                                    :" << clobber;
    LOG(INFO) << "History                                    :" << history;
    LOG(INFO) << "----------------------------------------------------------------------------";
}

int cztremovebkgd::cztremovebkgdProcess() {


    if (FileExists(outfile)) {
        if (clobber == YES) {
            unlink(outfile);

        } else {
            LOG(ERROR) << "" << outfile << "  already exists";
            LOG(ERROR) << "Use clobber=yes for overwriting the file";
            return (EXIT_FAILURE);
        }
    }


    dpi = new float[YSIZE * XSIZE];
    if (dpi == NULL) {
        LOG(ERROR) << "Out of memory error";
        return (EXIT_FAILURE);
    }


    if (getDPI()) {
        LOG(ERROR) << "Error in reading DPI";
        return (EXIT_FAILURE);
    }

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
    //fitsfile *fdpi;
    writeDPI();
    delete[] dpi;
    return(EXIT_SUCCESS);
}

int cztremovebkgd::getBkgdShadows() {
    int u, v;
    int j;
    if (nBkgdCoef < 1 || nBkgdCoef > 6) {
        LOG(ERROR) << "*** Number of background coefficients cannot be greater than 6 ***";
        return (EXIT_FAILURE);
    }
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

int cztremovebkgd::knownSourceSubtractedDPI() {
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
            LOG(INFO)<< "-----------------------------------------------------------------";
    for (int i = 0; i < XSIZE * YSIZE; i++) {
        dpi[i] = dpi[i] - knownShadows[0][i];

        //LOG(INFO)<< dpi[i];
    }
                    LOG(INFO)<< "-----------------------------------------------------------------";
    LOG(INFO) << "background subtracted dpi array created and values are already stored in it.";
    return (EXIT_SUCCESS);
}

int cztremovebkgd::writeDPI() {
    LOG(INFO) << "Writing DPI file.";
    fitsfile *fptr, *fdpi;
    float fulldpi[XSIZE*YSIZE];
    int status = 0;
    fits_open_file(&fptr, dpifile, READONLY, &status);
    LOG(INFO) << "original dpi file opened";
    fits_create_file(&fdpi, outfile, &status);
    fits_copy_file(fptr, fdpi, 1, 1, 1, &status);
    fits_close_file(fptr, &status);
    LOG(INFO) << "original dpi file copied into new dpi file";
    float **quad_dpi;
    quad_dpi = new float*[4];
    for(int i = 0; i < 4; i++) {
        quad_dpi[i] = new float[XPIX_QUAD  * YPIX_QUAD ];
    }
    for(int i=0; i<4; i++){
        for(int j=0; j<XPIX_QUAD *YPIX_QUAD ; j++){
            quad_dpi[i][j]=0.0;
        }
    }
    breakDPIarray(dpi, quad_dpi);
    
    /*code to process quadrants*/
    int* quadArray;
    bool quadFlagArray[NUMQUAD];
    int noccurrence;
    stringFinder(quad_no, ",", 0, &noccurrence);
    int nelems_quad = noccurrence + 1;
    int quadid = 0;
    quadArray = new int[nelems_quad];


    if (csvStrToInt(quad_no, ",", quadArray, &nelems_quad)) {
        LOG(ERROR) << "***Error in converting quadrant string array into integer array***";
    }
   for(int i=0;i<NUMQUAD;i++){
    quadFlagArray[i]=false;;
    
} 
    
for(int i=0;i<nelems_quad;i++){
    quadFlagArray[quadArray[i]]=true;
    
}
    
////    for (int i = 0; i < 4; i++) {
////        for(int j=0; j<nelems_quad; j++){
////            if(i==quadArray[j]){
////                quadFlagArray[i]=1;
////            }
////            else quadFlagArray[i]=0;
////        }
////    }
    /*code to process quadrants*/
   
    for (int i = 0; i < 4; i++) {
        if(quadFlagArray[i]==false){
                for(int j=0; j<XPIX_QUAD *YPIX_QUAD ; j++){
                quad_dpi[i][j]=0.0;
                }
        }
    }

    
    makeFullImage( quad_dpi[0], quad_dpi[1],  quad_dpi[2],  quad_dpi[3], fulldpi);

    
    char HDUname[10];
    long fpixel[2];
    fpixel[0] = 1;
    fpixel[1] = 1;
    for (int quad_no = 0; quad_no < 4; quad_no++) {
        DPIquadToHDU(quad_no, HDUname);
        
        LOG(INFO) << "Moving to quadrant no " << quad_no << " whose extension name is :" << HDUname;
        fits_movnam_hdu(fdpi, IMAGE_HDU, HDUname, 0, &status);
        fits_write_pix(fdpi, TFLOAT, fpixel, XPIX_QUAD *YPIX_QUAD ,  quad_dpi[quad_no], &status);
    }
    strcpy(HDUname, "FULLDPI");
    fits_movnam_hdu(fdpi, IMAGE_HDU, HDUname, 0, &status);
    fits_write_pix(fdpi, TFLOAT, fpixel, XSIZE*YSIZE,  fulldpi, &status);

    fits_close_file(fdpi, &status);

    return (EXIT_SUCCESS);
}

//Assumption - The DPI Image file contains dpi in 4 quadrants ie. extension no 2,3,4,5
//Each quadrant DPI is 64 x 64

int cztremovebkgd::getDPI() {
    fitsfile *fptr;
    int status = 0;
    int npixQuad = XSIZE * YSIZE / NUMQUAD;
    float *quaddpi = new float[npixQuad];

    //code to initialize dpi array and fill it with zeros
    for (int i = 0; i < XSIZE * YSIZE; i++) {
        dpi[i] = 0;
    }

    /*code to process quadrants*/
    int* quadArray;
    int noccurrence;
    stringFinder(quad_no, ",", 0, &noccurrence);
    int nelems_quad = noccurrence + 1;
    int quadid = 0;
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
        DPIquadToHDU(quadid, extname); //converts integer quad id into extension name for the respective quadrant in DPI. for ex. 0 = 'Q0_DPI'

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
                
            }
            r = (qid / NUMQUAD_ROW)*(YSIZE / NUMQUAD_COL); //NUMQUAD_ROW =2 AND NUMQUAD_COL=2
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

int cztremovebkgd::getHistory(vector<string> &vhistory) {
    //char *user = getlogin();
    char *user = getenv("USER");
	string str = "Module run by " + (string) user;
    vhistory.push_back(str);
    vhistory.push_back("Parameter List START for " + (string) modulename);
    vhistory.push_back("P1 infile=" + (string) dpifile);
    vhistory.push_back("P2 outfile=" + (string) outfile);

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

