#include "cztfindsources.h"

using namespace std;

cztfindsources::cztfindsources() {
    strcpy(modulename, "cztfindsources_v");
    strcat(modulename, VERSION);
}

cztfindsources::~cztfindsources() {
    delete[] dpi;
}

int cztfindsources::read(int argc, char **argv) {
    int status = 0;
    if (PIL_OK != (status = PILInit(argc, argv))) {
        LOG(ERROR) << "***Error Initializing PIL***";
        return status;
    }
    if (PIL_OK != (status = PILGetFname("imagefile", imagefile))) {
        LOG(ERROR) << "***Error reading input image file:" << imagefile << "***";
        return status;
    }
    
     if (PIL_OK != (status = PILGetFname("dpifile", dpifile))) {
        LOG(ERROR) << "***Error reading input image file:" << dpifile << "***";
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





    if (PIL_OK != (status = PILGetReal4("threshold", &threshold))) {
        LOG(ERROR) << "***Error reading threshold***";
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

int cztfindsources::read(char *imagefile, char *dpifile, char *maskfile, char *aspectfile, float threshold, char *sourcelist, float source_strength_thr, int debugmode, int clobber, int history) {
    strcpy(this->imagefile, imagefile);
    strcpy(this->dpifile, dpifile );
    strcpy(this->maskfile, maskfile);
    strcpy(this->aspectfile, aspectfile);   
    this->threshold = threshold;
    strcpy(this->sourcelist, sourcelist);
    this->source_strength_thr = source_strength_thr;

    this->clobber = clobber;
    this->history = history;
    return (EXIT_SUCCESS);
}

void cztfindsources::display() {
    LOG(INFO) << "----------------------------------------------------------------------------";
    LOG(INFO) << "                          CZTFINDSOURCES  PARAMETERS                            ";
    LOG(INFO) << "----------------------------------------------------------------------------";
    LOG(INFO) << "Input image file                             :" << imagefile;
    LOG(INFO) << "Threshold                                  :" << threshold;
;
    LOG(INFO) << "Clobber                                    :" << clobber;
    LOG(INFO) << "History                                    :" << history;
    LOG(INFO) << "----------------------------------------------------------------------------";
}

int cztfindsources::cztfindsourcesProcess() {

    int status = 0, NAXIS1=0, NAXIS2=0;
    fitsfile *fimg;
    
    if (FileExists(sourcelist)) {
        if (clobber == YES) {
            unlink(sourcelist);

        } else {
            LOG(ERROR) << "" << sourcelist << "  already exists";
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

    /*reading image file to detect peaks from it*/

    long fpixel[2], naxes[2];
    fpixel[0] = 1;
    fpixel[1] = 1;
    fits_open_file(&fimg, imagefile, READONLY, &status);
    if (status != 0) {
        LOG(ERROR) << "***Error opening IMAGE file***";
        fits_report_error(stderr, status);
        return (EXIT_FAILURE);
    }
    
    fits_get_img_size(fimg, 2, naxes, &status);
    if(status){
        LOG(ERROR) << "***Unable to read image size***'";
    }
    NAXIS1 = naxes[0];
    NAXIS2 = naxes[1];
    
    LOG(INFO)<< "------NAXIS 1-------"<< NAXIS1;
     LOG(INFO)<< "------NAXIS 2-------"<< NAXIS2;
    fits_movabs_hdu(fimg,1,NULL,&status);
    fits_read_key(fimg, TINT, "OVERSMPF", &oversamplingfactor, NULL, &status);
    LOG(INFO)<< "oversampling factor is: " << oversamplingfactor;

    
    if(status) { fits_report_error(stderr,status);  return (EXIT_FAILURE); }
    
    int npixQuad = NAXIS1*NAXIS2;
    skyimage = new float[npixQuad];

    fits_read_pix(fimg, TFLOAT, fpixel, npixQuad, NULL, skyimage, NULL, &status);
    if (status) {
        LOG(ERROR) << "***Error reading image pixels***";
        fits_report_error(stderr, status);
        return (EXIT_FAILURE);

    }

    skyimgsize = NAXIS1;

    if(identifyPeaks()) {
        LOG(ERROR) << "***Error in identifying peaks***";
        return(EXIT_FAILURE);
    }
    LOG(INFO)<< "Number of sources identified: " << nsources;
    //computing shadows
    shadows=allocateMemory<float>(nsources,XSIZE*YSIZE); //buffer to store shadows for peaks obtained 
    //float shadow[YSIZE][XSIZE];    
    //generating shadows from peaks

     
     float *shadow;
     shadow=new float[XSIZE*YSIZE];
     
    for(int i=0;i<nsources;i++){
        LOG(INFO)<<thetax[i]<<" "<<thetay[i];
        //cout<<thetax[i]<<" "<<thetay[i];
        
       MaskWeighting mskwt(maskfile,thetax[i],thetay[i]);
        if(mskwt.run()){
           LOG(ERROR)<<"****Error in mask Weighting****";
           return (EXIT_FAILURE);
        }
       LOG(INFO)<< "generating shadow for the identified source";
         mskwt.getShadow(shadow,XSIZE*YSIZE);
     
        for(int j=0;j<XSIZE*YSIZE;j++)
            shadows[i][j]=shadow[j];
         }
        if(debugmode==YES){
           for(int i=0;i<nsources;i++){
                char name[512];
                sprintf(name,"shadow_%d",i);
                ofstream fb(name,ios::binary);
                fb.write((char *)shadows[i],sizeof(float)*XSIZE*YSIZE);
                fb.close();
            }
        }
    
    /*---------------------END OF JUST TO CHECK------------------*/
     LOG(INFO)<<"Fitting sources......";
     int prev_peaks=nsources;
    while(1){
        if(linearfitDPI()){
                LOG(ERROR)<<"***Error in linearfitDPI***";
                return (EXIT_FAILURE);
        }
        
        //cout<<"prev_peaks:"<<prev_peaks<<"     prev_nbkgd:"<<prev_nbkgd;
        if(prev_peaks==nsources )            break;
        prev_peaks=nsources;  
        LOG(INFO)<<"prev_peaks:"<<prev_peaks;
        sleep(5);
    }
    LOG(INFO)<<"Number of sources:  "<<nsources;

    LOG(INFO)<<"Fitting Complete";
    //---------producing background subtracted image-----
    //finding total background in bkgdshaows[0]
    
         //adding sources to sourcelist
    ofstream fout(sourcelist);
    if(!fout){
        LOG(ERROR)<<"***Error in opening file "<<sourcelist<<"***";
        return (EXIT_FAILURE);
    }
    
    //finding RA and DEC for sources found
    double ra_rad,dec_rad;              //for radians
    ra.clear();  dec.clear();
    fout<<setw(15)<<"RA (degrees)"<<setw(15)<<"DEC (degrees)"<<setw(15)<<"FLUX"<< setw(15)<< "thetax (radians)" << setw(15) << "thetay (radians)'";
    for(int i=0;i<thetax.size();i++){
        getRaDec(aspectfile,thetax[i],thetay[i],&ra_rad,&dec_rad);
        ra.push_back(ra_rad*180/M_PI);
        dec.push_back(dec_rad*180/M_PI);
        fout<<endl<<setw(15)<<ra[i]<<setw(15)<<dec[i]<<setw(15)<<flux[i]<<setw(15)<<thetax[i]<<setw(15)<<thetay[i];;
    }
    fout.close(); 

    return (EXIT_SUCCESS);
}

int cztfindsources::identifyPeaks() {
    vector<int> row, col;
    vector<float> intensity;
    ;
    int r, c;
    int rowcen = skyimgsize / 2;
    int colcen = skyimgsize / 2;
    double val = (double) (DETECTORWIDTH / (double) (16.0 * oversamplingfactor * MASKHEIGHT));

    //this value is used in computation of thetax & thetay from x.y or row,col
    double theta;
    double thr = getrms<float>(skyimage, skyimgsize * skyimgsize) * threshold;
    LOG(INFO)<<"Threshold calculated is: " << thr;
    LOG(INFO)<< "SKYimgsize is:" << skyimgsize;
    LOG(INFO)<<"skyimage [2]: "<< skyimage[2];
    LOG(INFO)<< "Oversampling factor is: " << oversamplingfactor;
    //selecting peaks from the image
    for (int i = 0; i < skyimgsize * skyimgsize; i++) {
        r = (i / skyimgsize);
        c = (i % skyimgsize);
        if (skyimage[i] > thr) {
            intensity.push_back(skyimage[i]);
            row.push_back(r);
            col.push_back(c);
        }
        //cout<<rawimg[i]<<" "<<r<<" "<<c;    
    }
    //LOG(INFO) << "Size :" << intensity.size();
    LOG(INFO) << "Number of first cut peaks detected - "<<intensity.size();;
    //LOG(INFO);

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
                if (skyimage[k * skyimgsize + l] > max) {
                    max = skyimage[k * skyimgsize + l];
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
        flux.push_back(skyimage[r * skyimgsize + c]);
        thetax.push_back(atan(val * (double) (c - colcen)));
        thetay.push_back(atan(val * (double) (r - rowcen)));
        // LOG(INFO) << r << "  " << c << "  " << flux[i];
    }

    nsources = flux.size();

    LOG(INFO) << "Number of peaks found:" << flux.size();

    if (nsources > PEAKLIMIT) {
        LOG(ERROR) << "***Cannot process this many number of peaks***\n ***Run with larger threshold***";
        LOG(ERROR) << "Peak Limit is set to " << PEAKLIMIT << " at present";
        return (EXIT_FAILURE);
    }
    frow.clear();
    fcol.clear();
    intensity.clear();
    return (EXIT_SUCCESS);
}

int cztfindsources::linearfitDPI(){

    LOG(INFO)<<"Creating matrix A.........";
    int cols_A=nsources;
    int rows_A=XSIZE*YSIZE;
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
            //cout<<"i:"<<i<<" j:"<<j;
            A[i*cols_A+j]=shadows[j][i];
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
    LOG(INFO)<<"Calling function sourcelist_AinvB.... ";
    if(multiply_AinvB(A,dpi,rows_A,cols_A,cols_B,Coef)){
        LOG(ERROR)<<"***Error in multiply_AinvB function***";
        return (EXIT_FAILURE);
    }     
   
    int temp_nshadow=0;
    float **temp_shadows=allocateMemory<float>(nsources,XSIZE*YSIZE);   

    vector<double> temp_tx,temp_ty,temp_flux;
    temp_tx=thetax; temp_ty=thetay; temp_flux=flux;
    thetax.clear();  thetay.clear(); flux.clear();
    //copying shadows and background shadows to temporary arrays 
    for(int i=0;i<nsources;i++){
        for(int j=0;j<XSIZE*YSIZE;j++){
            temp_shadows[i][j]=shadows[i][j];
        }
    }
    

    int index=0;
    
    LOG(INFO)<<"The source strengths are-";
    for(int i=0;i<nsources;i++){
        LOG(INFO)<<"Source Strength: "<<i<<"  : "<<Coef[i];
        if(Coef[i]>source_strength_thr){
            temp_nshadow++;
            thetax.push_back(temp_tx[i]);
            thetay.push_back(temp_ty[i]);
            flux.push_back((temp_flux[i]));
            for(int j=0;j<XSIZE*YSIZE;j++)
                shadows[index][j]=temp_shadows[i][j];
             index++;
        }
    }     

    freeMemory(temp_shadows,nsources);
    
    nsources=temp_nshadow;

    delete[] A,Coef;
    temp_ty.clear(); temp_tx.clear(); temp_flux.clear();
    //cout<<"2.  Num of Sources - "<<nsources<<"    Backgrounds-"<<nBkgdCoef;
    //cout<<"linearfitDPI complete";
    return (EXIT_SUCCESS);
}

int cztfindsources::getDPI() {
    fitsfile *fptr;
    int status = 0;
    int npixQuad = XSIZE * YSIZE / NUMQUAD;
    float *quaddpi = new float[npixQuad];

    //code to initialize dpi array and fill it with zeros
    for (int i = 0; i < XSIZE * YSIZE; i++) {
        dpi[i] = 0;
    }

//    /*code to process quadrants*/
//    int* quadArray;
//    int noccurrence;
//    stringFinder(quad_no, ",", 0, &noccurrence);
//    int nelems_quad = noccurrence + 1;
//    int quadid = 0;
//    quadArray = new int[nelems_quad];
//
//    if (csvStrToInt(quad_no, ",", quadArray, &nelems_quad)) {
//        LOG(ERROR) << "***Error in converting quadrant string array into integer array***";
//    }
//    /*code to process quadrants*/
int* quadArray;
int nelems_quad=4;
quadArray = new int[nelems_quad];
for(int i=0; i<4; i++)
{
    quadArray[i] = i;
}
        
int quadid;



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
    for (int i = 0; i < 4; i++) {
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
