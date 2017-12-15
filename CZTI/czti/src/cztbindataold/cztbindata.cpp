
#include "cztbindata.h"

using namespace std;

cztbindata::cztbindata(){
    strcpy(modulename,"cztbindata_v");
    strcat(modulename,VERSION);
    strcpy(quadsToProcess,"");
    ra=0;
    dec=0;
    binsize=0;
    strcpy(gtifile,"");
    nbins=0;
    binmin=NULL;
    binmax=NULL;
    uflag=false;
    strcpy(gtiextname,"");
    strcpy(bintype,"");
    strcpy(this->pixelqualityfile,"\0");
}

cztbindata::~cztbindata(){
    if(binmin!=NULL)  delete[] binmin;
    if(binmax!=NULL)  delete[] binmax;
}

int cztbindata::read(int argc,char **argv){
    int status=0;
    if(PIL_OK!=(status=PILInit(argc,argv))){
        LOG(ERROR)<<"***Error Initializing PIL***";
        return status;
    }
    if(PIL_OK!=(status=PILGetFname("par_infile",infile))){
        LOG(ERROR)<<"***Error reading input file"<<infile<<"***";
        return status;
    }
    if(PIL_OK!=(status=PILGetFname("par_maskfile",maskfile))){
        LOG(ERROR)<<"***Error reading mask file"<<maskfile<<"***";
        return status;
    }
    if(PIL_OK!=(status=PILGetFname("par_aspectfile",aspectfile))){
        LOG(ERROR)<<"***Error reading aspect file:"<<aspectfile<<"***";
        return status;
    }
    
    if(PIL_OK!=(status=PILGetString("par_quadsToProcess", quadsToProcess))){
        LOG(ERROR)<<"***Error reading quadrant:"<<quadsToProcess<<"***";
        
        return status;
    }
    
    if (isQuadrantNumbersValid(quadsToProcess)){
            LOG(ERROR)<<"***Quadrant values can only be 0 or 1 or 2 or 3***";
            status = isQuadrantNumbersValid(quadsToProcess);
            return status;
    }
    
    if(PIL_OK!=(status=PILGetFname("par_pixelqualityfile",pixelqualityfile))){
        LOG(ERROR)<<"***Error reading pixel quality file***";
        return status;
    }
    if(strcmp(pixelqualityfile,"-")==0 || strcmp(pixelqualityfile," ")==0){
        strcpy(this->pixelqualityfile,"\0");
    }
    if(PIL_OK!=(status=PILGetFname("par_outfile",outfile))){
        LOG(ERROR)<<"***Error reading output file:"<<outfile<<"***";
        return status;
    }
    if(PIL_OK!=(status=PILGetString("par_timerange",timerange))){
        LOG(ERROR)<<"***Error reading time range***";
        return status;
    }
    if(PIL_OK!=(status=PILGetString("par_energyrange",energyrange))){
        LOG(ERROR)<<"***Error reading energy range***";
        return status;
    }
    if(PIL_OK!=(status=PILGetString("par_outtype",outputtype))){
        LOG(ERROR)<<"***Error reading output type***";
        return status;
    }
    if(strcasecmp(outputtype,"lc")==0){
        sflag=false;
        if(PIL_OK!=(status=PILGetString("par_bintype",bintype))){   //reading bin tyoe in case of lc
            LOG(ERROR)<<"***Error reading binning type***";
            return status;
        }
        if(strncasecmp(bintype,"u",1)==0){
            uflag=true;
            if(PIL_OK!=(status=PILGetReal4("par_binsize",&binsize))){
                LOG(ERROR)<<"Error reading binsize***";
                return status;
            }
        }
        else if(strncasecmp(bintype,"g",1)==0){
            if(PIL_OK!=(status=PILGetFname("par_gtifile",gtifile))){
                LOG(ERROR)<<"***Error reading gtifile***";
                return status;
            }
            if(PIL_OK!=(status=PILGetString("par_gtiextname",gtiextname))){
                LOG(ERROR)<<"***Error reading gti extname***";
                return status;
            }
        }
        else{
            LOG(ERROR)<<"***Invalid bin type value***";
            return (EXIT_FAILURE);
        }   
    }
    else if(strcasecmp(outputtype,"spec")==0){
        sflag=true;
        if(PIL_OK!=(status=PILGetReal4("par_binsize",&binsize))){  //reading binsize for energy
                LOG(ERROR)<<"Error reading binsize***";
                return status;
        }
    }
    else{
        LOG(ERROR)<<"***Invalid output type value***";
        return (EXIT_FAILURE);
    }
    
    if(PIL_OK!=(status=PILGetString("par_RA",RA_str))){
        LOG(ERROR)<<"***Error reading Right Ascension***";
        return status;
    }
    if(PIL_OK!=(status=PILGetString("par_DEC",DEC_str))){
        LOG(ERROR)<<"***Error reading Declination***";
        return status;
    }
    if(PIL_OK!=(status=PILGetString("par_tcol",tcol))){
        LOG(ERROR)<<"***Error reading time column name***";
        return status;
    }
    if(PIL_OK!=(status=PILGetString("par_ecol",ecol))){
        LOG(ERROR)<<"***Error reading energy column name***";
        return status;
    }
    if(PIL_OK!=(status=PILGetString("par_xcol",xcol))){
        LOG(ERROR)<<"***Error reading X column name***";
        return status;
    }
    if(PIL_OK!=(status=PILGetString("par_ycol",ycol))){
        LOG(ERROR)<<"***Error reading Y column name***";
        return status;
    }
    if(PIL_OK!=(status=PILGetBool("par_make_temp_files",&make_temp_files))){
        LOG(ERROR)<<"***Error reading delete_temp_files parameter***";
        return status;
    }
    if(PIL_OK!=(status=PILGetBool("par_history",&history))){
        LOG(ERROR)<<"***Error reading history parameter***";
        return status;
    }
    if(PIL_OK!=(status=PILGetBool("par_clobber",&clobber))){
        LOG(ERROR)<<"***Error Reading clobber***";
        return status;
    }
    PILClose(status);
    return (EXIT_SUCCESS);
}

void cztbindata::clear(){
    strcpy(infile,"");
    strcpy(maskfile,"");
    strcpy(aspectfile,"");
    strcpy(pixelqualityfile,"");
    strcpy(outfile,"");
    strcpy(quadsToProcess, "");
    strcpy(timerange,"");
    strcpy(energyrange,"");
    strcpy(gtifile,"");
    strcpy(gtiextname,"");
    strcpy(bintype,"");
    binsize=0; 
    strcpy(outputtype,"");
    strcpy(RA_str,"");
    strcpy(DEC_str,"");
    strcpy(tcol,"");
    strcpy(ecol,"");
    strcpy(xcol,"");
    strcpy(ycol,"");
    binmin=NULL;
    binmax=NULL;
    nbins=0;
    
}

int cztbindata::read(char* infile, char* maskfile, char* aspectfile, char* quadsToProcess, char* outfile,
        char *pixelqualityfile,char* outtype, char* timerange, char* energyrange, 
        char* bintype, char* gtifile,char *gtiextname,float binsize, char* Ra, char* Dec, 
        char* tcol,char* ecol, char* xcol, char* ycol,int make_temp_files,int clobber, int history){
    strcpy(this->infile,infile);
    strcpy(this->maskfile,maskfile);
    strcpy(this->aspectfile,aspectfile);
    strcpy(this->quadsToProcess, quadsToProcess);
    if(pixelqualityfile!=NULL)
        strcpy(this->pixelqualityfile,pixelqualityfile);
    else
        strcpy(this->pixelqualityfile,"\0");
    strcpy(this->outfile,outfile);
    strcpy(this->outputtype,outtype);
    strcpy(this->timerange,timerange);
    strcpy(this->energyrange,energyrange);
    if(strcasecmp(outtype,"lc")==0){
        this->sflag=false;
        strcpy(this->bintype,bintype);
        if(strncasecmp(bintype,"u",1)==0){
            this->binsize=binsize;
            this->uflag=true;
        }
        else if(strncasecmp(bintype,"g",1)==0){
            strcpy(this->gtifile,gtifile);
            strcpy(this->gtiextname,gtiextname);
        }
        else{
            LOG(ERROR)<<"Invalid bintype value";
            return (EXIT_FAILURE);
        }
    }
    else if(strcasecmp(outtype,"spec")==0){
        this-> sflag=true;
        this->binsize=binsize;
    }
    else{
        LOG(ERROR)<<"***Invalid output type value***";
        return (EXIT_FAILURE);
    }
    strcpy(this->RA_str,Ra);
    strcpy(this->DEC_str,Dec);
    strcpy(this->tcol,tcol);
    strcpy(this->ecol,ecol);
    strcpy(this->xcol,xcol);
    strcpy(this->ycol,ycol);
    this->make_temp_files=make_temp_files;
    this->clobber=clobber;
    this->history=history;
    return (EXIT_SUCCESS);
}

void cztbindata::display(){
    LOG(INFO)<<"----------------------------------------------------------------------------";
    LOG(INFO)<<"                          CZTBINDATA PARAMETERS                            ";
    LOG(INFO)<<"----------------------------------------------------------------------------";
     LOG(INFO)<<"Input Event file                 :"<<infile;
     LOG(INFO)<<"Input Mask file                  :"<<maskfile;
     LOG(INFO)<<"Input Aspect file                :"<<aspectfile;
     LOG(INFO)<<"Quadrants to be processed        :"<<quadsToProcess;
     LOG(INFO)<<"Pixel Quality File               :"<<pixelqualityfile;
     LOG(INFO)<<"Output file                      :"<<outfile;
     LOG(INFO)<<"Output Type                      :"<<outputtype;
     LOG(INFO)<<"Time Range                       :"<<timerange;
     LOG(INFO)<<"Energy Range                     :"<<energyrange;
     LOG(INFO)<<"Bin Type                         :"<<bintype;
     LOG(INFO)<<"GTI file                         :"<<gtifile;
     LOG(INFO)<<"Bin Size                         :"<<binsize;
     LOG(INFO)<<"Right Ascension                  :"<<RA_str;
     LOG(INFO)<<"Declination                      :"<<DEC_str;
     LOG(INFO)<<"Time column                      :"<<tcol;
     LOG(INFO)<<"Energy Column                    :"<<ecol;
     LOG(INFO)<<"X column                         :"<<xcol;
     LOG(INFO)<<"Y column                         :"<<ycol;
    if(make_temp_files==YES)
         LOG(INFO)<<"Create Temporary files :  YES";
    else
         LOG(INFO)<<"Create Temporary files : NO";
    if(clobber==YES)
        LOG(INFO)<<"Clobber                     : YES";
    else
        LOG(INFO)<<"Clobber                     : NO";
    if(history==YES)
        LOG(INFO)<<"history                     : YES";
    else
        LOG(INFO)<<"history                     : NO";
   
    LOG(INFO)<<"----------------------------------------------------------------------------";
}

int cztbindata::cztbindataProcess(){
    fitsfile *fptrIN;
    int status=0; //status variable
    float thetaXradian, thetaYradian;
    float nX, nY, nZ;
    //Output spectrum/light curve file existence check
    if(FileExists(outfile)){
        if(clobber==YES)
                unlink(outfile);
        else{
            LOG(ERROR)<<""<<outfile<<" already exists";
            LOG(ERROR)<<"Use clobber=yes for overwriting the file";
            return (EXIT_FAILURE);
        }
    }
    
    //Generating CALDB file paths
    compMaskFilepath = caldb_full_path_generator(maskfile);
    if(compMaskFilepath==""){
        LOG(ERROR)<< "Not able to generate Mask File (compressed) path";
        return(EXIT_FAILURE);
    }
    //opening input event file

    fits_open_file(&fptrIN,infile,READONLY,&status);
    if(status){ fits_report_error(stderr,status); return (EXIT_FAILURE);  }
     /*************************************************************************/
    
    //reading RA and DEC from input event file (if user does not provide them as input) &
    //converting them into radians.
    
    if(strcmp(RA_str,"-")==0 || strcmp(DEC_str,"-")==0){
        status=readRaDec(fptrIN,&ra,&dec);
        if(status) {
            LOG(ERROR)<<"Error in reading RA and DEC from input event file";
            return (EXIT_FAILURE);
        }
    } 
    else{
        ra=atof(RA_str);
        dec=atof(DEC_str);
    }
    LOG(INFO)<<"RA:"<<ra<<"   DEC:"<<dec;
    ra=ra*M_PI/180;  dec=dec*M_PI/180;  //converting to radians
    
    /*******************************************************************************/
    
    //Calculating Camera coordinates using RA and DEC and quaternions from aspect file.
    //These are stored in thetaXradian and thetaYradian in degrees.


    if (to_thetaX_thetaY((string) aspectfile, ra, dec, thetaXradian, thetaYradian)) {
        LOG(ERROR) << "Error in converting from CZTI body vector to corresponding thetaX and thetaY.";
        return EXIT_FAILURE;
    }    

    LOG(INFO)<<"Theta X: "<<thetaXradian*180/M_PI<<" deg   "<<"Theta Y: "<<thetaYradian*180/M_PI << " deg";  //printing in degrees
    
    /**********************************************************************************/
    
    //computing shift of mask pattern and verifying that it is less than detector width which is 39 mm.
    //Shift is stored in dx and dy.
    double dx,dy;
    dx=(-1)*MASKHEIGHT*tan(thetaXradian);
    dy=(-1)*MASKHEIGHT*tan(thetaYradian);
    LOG(INFO)<<"Shift of Mask  --- dx:"<<dx<<"  dy:"<<dy;
    if(abs(dx)>DETECTORWIDTH || abs(dy)>DETECTORWIDTH){
       LOG(ERROR)<<"***Error-Shift is greater than detector width (39 mm)***";
       return (EXIT_FAILURE);
    }
   /***********************************************************************************/
    /*            */
    
    // initializing maskweight[128x128], mskwt_quad[4][64x64] and openf[128x128]
    // in this 'openf' stands for open fraction
    double *maskweight;
    float *openf;
    
    double **mskwt_quad;;
    mskwt_quad=new double *[NUMQUAD];
    if(mskwt_quad==NULL)  { LOG(ERROR)<<"Out of memory error"; return (EXIT_FAILURE); }
    for(int i=0;i<NUMQUAD;i++){
        mskwt_quad[i]=new double[NUMQUAD,PIXELS_PER_ROW_QUAD*PIXELS_PER_COL_QUAD];
        if(mskwt_quad[i]==NULL)   { LOG(ERROR)<<"Out of memory error"; return (EXIT_FAILURE); }
    }  

    maskweight=new double[YSIZE*XSIZE];
    openf=new float[YSIZE*XSIZE];
  
    if(maskweight==NULL || openf==NULL){
        LOG(ERROR)<<"***Out of Memory***";
        return (EXIT_FAILURE);
    }
   // arrays maskweight, mskwt_quad and openf initialized.
    /********************************************************************************/
    
    
    MaskWeighting mskwt((char*) compMaskFilepath.c_str(),thetaXradian,thetaYradian);
    if(mskwt.run()){
        LOG(ERROR)<<"***Error in running mask weighting***";
        return (EXIT_FAILURE);
    }
    
            LOG(INFO)<<"Computing weights...";
    if(mskwt.getMaskWeight(maskweight)){
        LOG(ERROR)<<"***Error in  mskwt.getMaskWeight()***";
        return (EXIT_FAILURE);
    }
    LOG(INFO) << "Mask weights for all detectors evaluated";
    
    if(mskwt.getMaskWeightQuadWise(mskwt_quad)){
        LOG(ERROR)<<"***Error in mskwt.getMaskWeightQuadWiss***";
    }
     LOG(INFO) << "Mask weights for quadrants independently evaluated.";
     
    LOG(INFO)<<"Computing OpenFraction...";
    if(mskwt.getExposureMap(openf)){
        LOG(ERROR)<<"***Error in  mskwt.getExposureMap()***";
        return (EXIT_FAILURE);
    } 
    
    //Assigning weight zero to all bad pixels
    if(strcmp(pixelqualityfile,"\0")!=0){
        LOG(INFO)<<"Using pixel quality file";
        filterWeight(pixelqualityfile,maskweight);
    }
    
    LOG(INFO)<<"Creating weighted event file";
    if(writeWeightedEvent(infile, mskwt_quad)){
        LOG(ERROR)<<"***Error in creating weighted event file***";
        return (EXIT_FAILURE);
    }
    
    char filename[FLEN_FILENAME];  //filename to be used for temporary files
    //----------------------------------------------------------------

    double WA=0,R=0,sumw=0,sumwf=0;         //WA - weighted area, R - Reliability Index 
    double *WA_quad;
    WA_quad = new double[4];
    for(int i=0; i<4; i++){
        WA_quad[i]=0.0;
    }
                                        //WA = summation of (weight x open fraction x area)
                                        //sumw - summation of weights,  
                                        //sumwf - summation of weight*openfraction
    int i,j=0;
    
    for(i=0;i<YSIZE*YSIZE;i++){
            WA=WA+(maskweight[i]*openf[i]*mskwt.area[i]);
            sumw=sumw+maskweight[i];
            sumwf=sumwf+(maskweight[i]*openf[i]);
        
    }

    R=1.0-(sumw/sumwf);
    
    LOG(INFO)<< "**********calculated total weighted area********";
    
    //initializing new variable area_quad to store area of each pixel in different quadrants.
    float **area_quad, **openf_quad;
    area_quad= new float*[4];
    openf_quad = new float*[4];
    for(int i=0; i<4; i++){
        area_quad[i]=new float[PIXELS_PER_ROW_QUAD*PIXELS_PER_COL_QUAD];
        openf_quad[i]= new float[PIXELS_PER_ROW_QUAD*PIXELS_PER_COL_QUAD];
    }
    for(int i=0; i<4; i++){
        for(int j=0; j<PIXELS_PER_ROW_QUAD*PIXELS_PER_COL_QUAD; j++){
            area_quad[i][j]=0.0;
            openf_quad[i][j]=0.0;
        }
    }
    
    mskwt.getExposureMapQuadWise(openf_quad);
    allToQuad(mskwt.area, area_quad);
    
    
    for(int i=0; i<4; i++){
        for(int j=0; j<PIXELS_PER_ROW_QUAD*PIXELS_PER_COL_QUAD;j++){
            WA_quad[i] = WA_quad[i] + mskwt_quad[i][j]*openf_quad[i][j]*area_quad[i][j]; 
        }
        LOG(INFO)<<"Weighted Area for quadrant "<< i << "is "<<WA_quad[i];;
    }
    
    LOG(INFO)<<"Weighted Area is "<<WA;
    LOG(INFO)<<"Reliability Index is "<<R;
    
        //writing weight, open fraction and area to a file
     if(make_temp_files==YES){ 
        sprintf(filename,"!%s.weight",infile);
        writeImgD(filename,  maskweight,YSIZE,XSIZE);
        
        sprintf(filename,"!%s.weightq0",infile);
        writeImgD(filename,  mskwt_quad[0],64,64);   
        
        
        sprintf(filename,"!%s.openf",infile);
        writeImg(filename,openf,YSIZE,XSIZE);
        
        sprintf(filename,"!%s.openfq0",infile);
        writeImg(filename, openf_quad[0],64,64);
        
        sprintf(filename,"!%s.area",infile);
        writeImg(filename,mskwt.area,YSIZE,XSIZE);
        mskwt.write("!Mask.fits");
    } 
    //---------------------------------------------------------
    //creating time bins/energy bins
    if(makeBins()){
        LOG(ERROR)<<"Error in creating bins";
        return (EXIT_FAILURE);
    }
    

    fitsfile *fout;                   //for output file
    fits_create_file(&fout,outfile,&status);
    if(status){  fits_report_error(stderr,status);  return (-1); }
    if(prepareOutputFile(fptrIN,fout,sflag,ecol)){
        LOG(ERROR)<<"Error in creating output file";
        return (EXIT_FAILURE);
    }
    //writing keywords for tstart,tstop,estart,estop
    DLOG(INFO) << "Writing keywords TSTART, TSTOP, EMIN, EMAX & ECOL to ouput file " << outfile;
    fits_movabs_hdu(fout,2,NULL,&status);
    if(status) { fits_report_error(stderr,status);  return (EXIT_FAILURE); }
    fits_write_key(fout,TDOUBLE,"TSTART",&tstart,"Start Time for data",&status);
    if(status) { fits_report_error(stderr,status);  return (EXIT_FAILURE); }
    fits_write_key(fout,TDOUBLE,"TSTOP",&tstop,"Stop time for data",&status);
    if(status) { fits_report_error(stderr,status);  return (EXIT_FAILURE); }
    fits_write_key(fout,TDOUBLE,"EMIN",&estart,"Min energy for data",&status);
    if(status) { fits_report_error(stderr,status);  return (EXIT_FAILURE); }
    fits_write_key(fout,TDOUBLE,"EMAX",&estop,"Max energy for data",&status);
    if(status) { fits_report_error(stderr,status);  return (EXIT_FAILURE); }
    fits_write_key(fout,TSTRING,"ECOL",ecol,"Energy column used for binning",&status);
    if(status) { fits_report_error(stderr,status);  return (EXIT_FAILURE); }
    
    //buffers for holding output
    double *xval=new double[nbins];    //to hold UT(for LC) or Energy/Channel (for SPEC)
    double *flux=new double[nbins];
    double *error=new double[nbins];
    for(long i=0;i<nbins;i++){
        xval[i]=0;
        flux[i]=0;
        error[i]=0;
    }

    
    /****************************************************************************************************
     Following is the code to process data quadrant wise using weighted event file.
     ****************************************************************************************************/
    //initializing array containing quadrants to be processed
    int* quadArray;
    int noccurrence;
    stringFinder(quadsToProcess, ",", 0, &noccurrence);
    int nelems_quad = noccurrence +1 ;
    quadArray = new int[nelems_quad];
    
    DLOG(INFO) << "Number of quadrants to be processed " << nelems_quad;
    if(csvStrToInt(quadsToProcess, ",", quadArray, &nelems_quad)){
      LOG(ERROR)<<"***Error in converting quadrant string array into integer array***";
    }
    
    fitsfile *wevt;
    char* wfile = new char[strlen(infile)+5];
    strcpy(wfile,infile);
    sprintf(wfile,"%s.wevt",infile);
    fits_open_file(&wevt,wfile,READONLY,&status);
    if(status){
      LOG(ERROR)<<"***Error in opening weighted event file : "<<wfile << "***";  
    }

    for (i = 0; i < nelems_quad; i++) {
        double *flux_temp = new double[nbins];
        double *error_temp = new double[nbins];
        for (j = 0; j < nbins; j++) {
            flux_temp[j] = 0;
            error_temp[j] = 0;
        }
        status = Bin_quad(wevt, sflag, tstart, tstop, estart, estop, binmin, binmax, nbins, binsize,ecol, tcol, xcol, ycol, "weight", WA_quad[quadArray[i]], quadArray[i], xval, flux_temp, error_temp);
        if (status) {
            LOG(ERROR) << "***Error in Bin_quad() while processing quadrant " << quadArray[i] << " ***";
            return (EXIT_FAILURE);
        }
        for (j = 0; j < nbins; j++) {
            flux[j] = flux[j] + flux_temp[j];
            error[j] = error[j] + error_temp[j];
        }
        delete[] flux_temp, error_temp;

        
    }

    fits_close_file(wevt, &status);
    if (status) {
        LOG(ERROR) << "Error in closing weighted event file";
        fits_report_error(stderr, status);
        return (EXIT_FAILURE);
    }
    
    /****************************************************************************************************
     
     ****************************************************************************************************/   

    
    //writing curve
    long num=nbins;
    //fits_movabs_hdu(fout,2,NULL,&status);
    //if(status) { fits_report_error(stderr,status);  return (EXIT_FAILURE); }
    fits_write_col(fout,TDOUBLE,1,1,1,num,xval,&status);
    if(status) { fits_report_error(stderr,status);  return (EXIT_FAILURE); }
    fits_write_col(fout,TDOUBLE,2,1,1,num,flux,&status);
    if(status) { fits_report_error(stderr,status);  return (EXIT_FAILURE); }
    fits_write_col(fout,TDOUBLE,3,1,1,num,error,&status);
    if(status) { fits_report_error(stderr,status);  return (EXIT_FAILURE); }
    

    fits_movabs_hdu(fout,3,NULL,&status);
    if(status) { fits_report_error(stderr,status);  return (EXIT_FAILURE); }
    fits_write_col(fout,TDOUBLE,1,1,1,nbins,binmin,&status);
    if(status) { fits_report_error(stderr,status);  return (EXIT_FAILURE); }
    fits_write_col(fout,TDOUBLE,2,1,1,nbins,binmax,&status);
    if(status) { fits_report_error(stderr,status);  return (EXIT_FAILURE); }
    
    fits_close_file(fout,&status);            //closing output file
    if(status) { fits_report_error(stderr,status);  return (EXIT_FAILURE); }
    fits_close_file(fptrIN,&status);          //closing input file
    if(status){ fits_report_error(stderr,status); return (EXIT_FAILURE);  }
    //updating keywords in output file
    updateKeywords(outfile,modulename);
    //writing history to output file
    if(history==YES){
         vector<string> vhistory;
         getHistory(vhistory);
         writeHistory(outfile,vhistory);           //writes history to each HDU of output file
     }
    
    /*for(int i=0;i<nbins;i++)  delete[] fulldpi[i];
    delete[] fulldpi; */
    delete[] flux,xval,error;
    
    delete[] maskweight,openf;
            
    return (EXIT_SUCCESS);
}

int cztbindata::makeBins(){
    int status=0;
    int hdutype=0;
    long nrows=0;
    int tcol_n;
    fitsfile *fptr;   //for event file
    fits_open_file(&fptr,infile,READONLY,&status);
    if(status){ fits_report_error(stderr,status); return (EXIT_FAILURE);  }
    //code for getting time range for data
    if(strcmp(timerange,"-")==0){
        vector<double> time1,time2;
        
        //loop to find start time and stop time of data
        for(int i=EVENTDATA_HDU_BEGIN;i<=EVENTDATA_HDU_END;i++){
            fits_movabs_hdu(fptr,i,&hdutype,&status);
            if(status){ fits_report_error(stderr,status); return (EXIT_FAILURE); }
            if(hdutype!=BINARY_TBL){
                LOG(ERROR)<<"***Wrong HDU type found in event data file***";
                LOG(ERROR)<<"***Expected Binary table for events***";
                return (EXIT_FAILURE);
            }
            fits_get_num_rows(fptr,&nrows,&status);
            if(status){ fits_report_error(stderr,status); return (EXIT_FAILURE);  }
            if(nrows>0){
                double t1,t2;
                fits_get_colnum(fptr,CASEINSEN,tcol,&tcol_n,&status);
                if(status){ fits_report_error(stderr,status); return (EXIT_FAILURE);  }
                fits_read_col(fptr,TDOUBLE,tcol_n,1,1,1,NULL,&t1,NULL,&status);   //reading first row time in time column
                if(status){ fits_report_error(stderr,status); return (EXIT_FAILURE);  } 
                fits_read_col(fptr,TDOUBLE,tcol_n,nrows,1,1,NULL,&t2,NULL,&status);  //reading last row time in time column
                if(status){ fits_report_error(stderr,status); return (EXIT_FAILURE);  }
                time1.push_back(t1);   time2.push_back(t2);
            }
         }
        min<double>(time1.data(),time1.size(), &tstart);
        max<double>(time2.data(),time2.size(),&tstop);
    }
    else{
        getbins(timerange,&tstart,&tstop,1);   
    }
    
    fits_close_file(fptr,&status);     //closing input event file
    if(status){ fits_report_error(stderr,status); return (EXIT_FAILURE);  }   
    //code for getting energy range for data
    if(strcmp(energyrange,"-")==0){
        if(strcasecmp(ecol,"PHA")==0 || strcasecmp(ecol,"PI")==0){
            estart=MINCHANNEL;  estop=MAXCHANNEL;
        }
        else if(strcasecmp(ecol,"ENERGY")==0){
            estart=EMIN;  estop=EMAX;
        }
        else{
            LOG(ERROR)<<"Invalid ecol value..Must be PHA/PI/ENERGY";
            return (EXIT_FAILURE);
        }
    }
    else{
        getbins(energyrange,&estart,&estop,1);    
    }

    double range;
    if (sflag) { //for spectrum
        range = estop - estart;
        if ((long) (range * 1000000) % (long) (binsize * 1000000) == 0) {

            nbins = (int) (range / binsize);
            
        } 
        else {
            nbins = (range / binsize) + 1;
            
        }
        binmin = new double[nbins];
        binmax = new double[nbins];
        for (int i = 0; i < nbins; i++) {
            binmin[i] = estart + i*binsize; //making energy bins
            binmax[i] = binmin[i] + binsize;
        }
    } else { //for light curve
        if (uflag) { //in case of uniform binning
            range = tstop - tstart;
            if ((long) (range * 1000000) % (long) (binsize * 1000000) == 0) {
                nbins = range / binsize;
                
            } else {
                nbins = (range / binsize) + 1;
                //cout << "/////////////////////////////////////////////second nbins is :" << nbins;
            }
            binmin = new double[nbins];
            binmax=new double[nbins];
            for(int i=0;i<nbins;i++){
                binmin[i]=tstart+i*binsize;        //making energy bins
                binmax[i]=binmin[i]+binsize;
            }
        }
        else{                                //in case of gti binning
            fits_open_file(&fptr,gtifile,READONLY,&status);
            if(status){ fits_report_error(stderr,status); return (EXIT_FAILURE);  }
            fits_movnam_hdu(fptr,BINARY_TBL,gtiextname,0,&status);
            if(status){ fits_report_error(stderr,status); return (EXIT_FAILURE);  }
            fits_get_num_rows(fptr,&nrows,&status);
            if(status){ fits_report_error(stderr,status); return (EXIT_FAILURE);  }
            if(nrows<=0){
                LOG(ERROR)<<"GTI not found in "<<gtifile;
                return (EXIT_FAILURE);
            }
            double *tempstart,*tempstop;
            tempstart=new double[nrows];
            tempstop=new double[nrows];
            fits_read_col(fptr,TDOUBLE,1,1,1,nrows,NULL,tempstart,NULL,&status);  //reading 1st column in GTI extension START/TSTART
            if(status){ fits_report_error(stderr,status); return (EXIT_FAILURE);  }
            fits_read_col(fptr,TDOUBLE,2,1,1,nrows,NULL,tempstop,NULL,&status);  //reading 2st column in GTI extension  STOP/TSTOP
            if(status){ fits_report_error(stderr,status); return (EXIT_FAILURE);  }
            fits_close_file(fptr,&status);
            if(status){ fits_report_error(stderr,status); return (EXIT_FAILURE);  }
            //checking whether gti bins exist within timerange
            if(tstart>tempstop[nrows-1 || tstop<tempstart[0]]){
                LOG(ERROR)<<"GTI bins are not within timerange";
                return (EXIT_FAILURE);
            }
            //to find which bins lie in teime range
            int i=0;
            while(tstart>tempstart[i] && i<nrows) i++;
            if(i>0)         i--;
            int j=nrows-1;
            while(tstop<tempstop[j] && j>=0)   j--;
            if(j<(nrows-1)) j++;
            nbins=j-i+1;
            binmin=new double[nbins];
            binmax=new double[nbins];
            for(int k=0;k<nbins;k++,i++){
                binmin[k]=tempstart[i];
                binmax[k]=tempstop[i];
            }
            delete[] tempstart;
            delete[] tempstop;
        }
    }
    return (EXIT_SUCCESS);
}

int prepareOutputFile(fitsfile *fevt,fitsfile *fout,bool sflag,char *ecol){
    int status=0;
    int bitpix=BYTE_IMG;
    int naxis=0;
    long naxes[2];
    naxes[0]=naxes[1]=0;
    ecol = "Energy";
    fits_create_img(fout,bitpix,naxis,naxes,&status);
    if(status){ fits_report_error(stderr,status); return (EXIT_FAILURE);  }
    //for copying keywords from event file
    fits_movabs_hdu(fevt,1,NULL,&status);        //moving to first HDU
    if(status){ fits_report_error(stderr,status); return (EXIT_FAILURE);  }
    copyUserKeyWords(fevt,fout);
    if(sflag){                         //for spectrum
        if(strcasecmp(ecol,"Energy")==0){
            char *ttype[]={"Energy","Flux","Error"};
            char *tform[]={"D","D","D"};
            char *tunit[]={"KeV","cnts/area","cnts/area"};
            fits_create_tbl(fout,BINARY_TBL,0,3,ttype,tform,tunit,"SPECTRUM",&status);
            if(status){  fits_report_error(stderr,status);  return (-1); }
        }
        else if(strcasecmp(ecol,"PI")==0 || strcasecmp(ecol,"PHA")==0){
            char *ttype[]={"Channel","Flux","Error"};
            char *tform[]={"D","D","D"};
            char *tunit[]={"","cnts/area","cnts/area"};
            fits_create_tbl(fout,BINARY_TBL,0,3,ttype,tform,tunit,SPEC_EXTNAME,&status);
            if(status){  fits_report_error(stderr,status);  return (-1); }
        }
        else{
            LOG(ERROR)<<"Invalid ecol type..Must be PHA/PI/Energy"; return (EXIT_FAILURE);
        }
        //for EBOUNDS extension
        char *ttype[]={"EMIN","EMAX"};
        char *tform[]={"D","D"};
        char *tunit[]={"",""};
        fits_create_tbl(fout,BINARY_TBL,0,2,ttype,tform,tunit,"EBINS",&status);
        if(status){  fits_report_error(stderr,status);  return (-1); }
        fits_write_key(fout,TSTRING,"ECOL",ecol,"Energy column used for binning",&status);
        if(status) { fits_report_error(stderr,status);  return (EXIT_FAILURE); }
    }
    else{                //for light curve
        {
            char *ttype[]={"UT","Flux","Error"};
            char *tform[]={"D","D","D"};
            char *tunit[]={"sec","cnts/area","cnts/area"};
            fits_create_tbl(fout,BINARY_TBL,0,3,ttype,tform,tunit,LC_EXTNAME,&status);
            if(status){  fits_report_error(stderr,status);  return (-1); }
        }
        {
            char *ttype[]={"TSTART","TSTOP"};
            char *tform[]={"D","D"};
            char *tunit[]={"",""};
            fits_create_tbl(fout,BINARY_TBL,0,2,ttype,tform,tunit,"TIMEBINS",&status);
            if(status){  fits_report_error(stderr,status);  return (-1); }
        }
    }
        LOG(INFO) << "WE ARE HERE::::::::::";
    return (EXIT_SUCCESS);
}

int readRaDec(fitsfile *fptr,float *ra,float *dec){
    int status=0;
    fits_read_key(fptr,TFLOAT,"RA",ra,NULL,&status);
    if(status){
        fits_report_error(stderr,status);
        return (EXIT_FAILURE);
    }
    fits_read_key(fptr,TFLOAT,"DEC",dec,NULL,&status);
    if(status){
        fits_report_error(stderr,status);
        return (EXIT_FAILURE);
    }
    return (EXIT_SUCCESS);
}

int filterWeight(char *pixelqualityfile,double *weight){
    int status=0;
    fitsfile *fptr;
    fits_open_file(&fptr,pixelqualityfile,READONLY,&status);
    if(status) { fits_report_error(stderr,status);  return (EXIT_FAILURE); }
    unsigned char pixelquality[YSIZE*XSIZE];
    long fpixel[2];
    fpixel[0]=1;  fpixel[1]=1;
    fits_read_pix(fptr,TBYTE,fpixel,XSIZE*YSIZE,NULL,pixelquality,NULL,&status);
    if(status) { fits_report_error(stderr,status);  return (EXIT_FAILURE); }
    int row,col;
    for(int i=0;i<XSIZE*YSIZE;i++){
        if(pixelquality[i]!=GOODPIX){
            weight[i]=0;
        }
    }
    fits_close_file(fptr,&status);
    if(status) { fits_report_error(stderr,status);  return (EXIT_FAILURE); }
    return (EXIT_SUCCESS);
}

int writeCurve(fitsfile *fout,double *xval,double *flux,double *error,long num){
    int status=0;
    fits_movabs_hdu(fout,2,NULL,&status);
    if(status) { fits_report_error(stderr,status);  return (EXIT_FAILURE); }
    fits_write_col(fout,TDOUBLE,1,1,1,num,xval,&status);
    if(status) { fits_report_error(stderr,status);  return (EXIT_FAILURE); }
    fits_write_col(fout,TDOUBLE,2,1,1,num,flux,&status);
    if(status) { fits_report_error(stderr,status);  return (EXIT_FAILURE); }
    fits_write_col(fout,TDOUBLE,3,1,1,num,error,&status);
    if(status) { fits_report_error(stderr,status);  return (EXIT_FAILURE); }
    return (EXIT_SUCCESS);
}

 
int Bin_quad(fitsfile *wevt, bool sflag, double tstart, double tstop, double estart, double estop, double *binmin, double *binmax, int nbins, float binsize, char* ecol, char* tcol, char* xcol,
        char* ycol, char* wcol,  double WA_quad, int quadrant_number, double* xval, double *flux, double *error){
     if(sflag){
        LOG(INFO)<< "Analyzing data from quadrant " << quadrant_number << " to produce energy spectrum";
     }
     else {
         LOG(INFO)<< "Analyzing data from quadrant " << quadrant_number << " to produce light curve";
     }
     char filename[FLEN_FILENAME];
     int status=0;
     
     fits_file_name(wevt, filename, &status);
     if(status){
         LOG(ERROR)<<"Error getting filename";
         fits_report_error(stderr,status);
         return (EXIT_FAILURE);
     }
     LOG(INFO)<<"Weighted event file :"<< filename;
     int ecol_n,tcol_n,xcol_n, ycol_n, wcol_n;
     char* HDUname= new char[3];
     long nrows=0, firstrow=1, firstelem=1, totcount=0;
     

    quadToHDU(quadrant_number, HDUname);
    LOG(INFO) << "Moving to HDU no " << quadrant_number << " whose extension name is :" << HDUname;
    fits_movnam_hdu(wevt, BINARY_TBL, HDUname, 0, &status);
    if (status) {
        LOG(ERROR) << "***Error in moving to extension name :" << HDUname << " ***";
        fits_report_error(stderr, status);
        return (EXIT_FAILURE);
    }

    fits_get_colnum(wevt, CASEINSEN, ecol, &ecol_n, &status);
    LOG(INFO) << "Energy column number : " << ecol_n;
    if (status) {
        LOG(ERROR) << "*** Error getting column number for ecol in file " << filename << " ***";
        fits_report_error(stderr, status);
        return (EXIT_FAILURE);
    }
    fits_get_colnum(wevt, CASEINSEN, tcol, &tcol_n, &status);
    if (status) {
        LOG(ERROR) << "*** Error getting column number for tcol in file " << filename << " ***";
        fits_report_error(stderr, status);
        return (EXIT_FAILURE);
    }
    fits_get_colnum(wevt, CASEINSEN, xcol, &xcol_n, &status);
    if (status) {
        LOG(ERROR) << "*** Error getting column number for xcol in file " << filename << " ***";
        fits_report_error(stderr, status);
        return (EXIT_FAILURE);
    }
    fits_get_colnum(wevt, CASEINSEN, ycol, &ycol_n, &status);
    if (status) {
        LOG(ERROR) << "*** Error getting column number for ycol in file " << filename << " ***";
        fits_report_error(stderr, status);
        return (EXIT_FAILURE);
    }

    fits_get_colnum(wevt, CASEINSEN, wcol, &wcol_n, &status);
    LOG(INFO) << "Weight column number : " << wcol_n;
    if (status) {
        LOG(ERROR) << "***Error getting column number for wcol in file " << filename << " ***";
        fits_report_error(stderr, status);
        return (EXIT_FAILURE);
    }

    //get number of rows in event file
    fits_get_num_rows(wevt, &nrows, &status);
    if (status) {
        LOG(ERROR) << "*** Error getting number of rowsl in file " << filename << " ***";
        fits_report_error(stderr, status);
        return (EXIT_FAILURE);
    }

    if (nrows > 0) {
        double *energy, *weight_quad;
        double *time;
        unsigned char *detx, *dety;
        time = new double[nrows];
        energy = new double[nrows];
        detx = new unsigned char[nrows];
        dety = new unsigned char[nrows];
        weight_quad = new double[nrows];
        LOG(INFO)<<"time, energy, detx, dety and weight_quad created.";
        if (time == NULL || energy == NULL || detx == NULL || dety == NULL || weight_quad == NULL) {
            LOG(ERROR) << "***Out of Memory Error-timeBinning()***";
            return (EXIT_FAILURE);
        }
        fits_read_col(wevt, TDOUBLE, ecol_n, firstrow, firstelem, nrows, NULL, energy, NULL, &status);
        if (status) {
            LOG(ERROR) << "***Error in reading energy column***";
            fits_report_error(stderr, status);
            return (EXIT_FAILURE);
        }

        fits_read_col(wevt, TDOUBLE, tcol_n, firstrow, firstelem, nrows, NULL, time, NULL, &status);
        if (status) {
            LOG(ERROR) << "***Error in reading time column***";
            fits_report_error(stderr, status);
            return (EXIT_FAILURE);
        }
        //reading x column
        fits_read_col(wevt, TBYTE, xcol_n, firstrow, firstelem, nrows, NULL, detx, NULL, &status);
        if (status) {
            LOG(ERROR) << "***Error in reading detx column***";
            fits_report_error(stderr, status);
            return (EXIT_FAILURE);
        }
        //reading y column
        fits_read_col(wevt, TBYTE, ycol_n, firstrow, firstelem, nrows, NULL, dety, NULL, &status);
        if (status) {
            LOG(ERROR) << "***Error in reading dety column***";
            fits_report_error(stderr, status);
            return (EXIT_FAILURE);
        }

        fits_read_col(wevt, TDOUBLE, wcol_n, firstrow, firstelem, nrows, NULL, weight_quad, NULL, &status);
        if (status) {
            LOG(ERROR) << "***Error in reading weight column***";
            fits_report_error(stderr, status);
            return (EXIT_FAILURE);
        }

        double *sumwc, *sumw2c;
        sumwc = new double[nbins];
        sumw2c = new double[nbins];

        LOG(INFO)<<"sumwc and sumw2c array created with size.";
        
        for (int bin = 0; bin < nbins; bin++) {
            sumwc[bin] = 0.0;
            sumw2c[bin] = 0.0;
            flux[bin]=0.0;
            error[bin]=0.0;
        }
        
        LOG(INFO) << "NUMBER OF BINS : " << nbins;
        LOG(INFO)<< "ESTART :" << estart;
        LOG(INFO)<< "ESTOP : "<< estop;
        LOG(INFO)<< "TSTART  :"<< tstart;
        LOG(INFO)<<"TSTOP : "<< tstop;
        int binindex = 0;
        //double binsize_energy = (estop - estart) / (double) nbins;
        //double binsize_time = (tstop - tstart) / (double) nbins;
        double binsize_energy = (double)binsize;
        double binsize_time = (double) binsize;
        if (sflag) {
            LOG(INFO) << "Energy bin size calculated : " << binsize_energy;
            for (int i = 0; i < nrows; i++) {
                if (time[i] >= tstart && time[i] <= tstop) {
                    if (energy[i] >= estart && energy[i] < estop) {
                        binindex = (int) ((energy[i] - estart) / binsize_energy);
                        sumwc[binindex] += weight_quad[i];
                        sumw2c[binindex] += weight_quad[i] * weight_quad[i];
                    }
                   // if ((int)energy[i]==1023) 
                        //LOG(INFO) << binindex << "        " << sumwc[binindex] << "                  " << sumw2c[binindex] << "           " << i ;
                }
            }


        } else {
            LOG(INFO) << "Time bin size calculated : " << binsize_time;
            for (int i = 0; i < nrows; i++) {
                //LOG(INFO)<<"eNERGY "<< energy[i] << "     time: "<< time[i];
                if (energy[i] >= estart && energy[i] <= estop) {
                    if (time[i] >= tstart && time[i] <= tstop) {
                        binindex = (int) ((time[i] - tstart) / binsize_time);
                        sumwc[binindex] += weight_quad[i];
                        sumw2c[binindex] += weight_quad[i] * weight_quad[i];
                    }
                    //LOG(INFO) << binindex << "        " << sumwc[binindex] << "                  " << sumw2c[binindex] << "           " << i;
                }
            }
        }
        LOG(INFO)<<"Returning flux, error and parameter values for quadrant :" << quadrant_number;;
        for (int bin = 0; bin < nbins; bin++) {
            flux[bin] = sumwc[bin] / WA_quad;
            error[bin] = sumw2c[bin] / WA_quad;
            xval[bin] = (binmin[bin] + binmax[bin]) / 2;
        }


    }

    return (EXIT_SUCCESS);
}

int cztbindata::getHistory(vector<string> &vhistory){
    //char *user=getlogin();
    char *user = getenv("USER");
	string str="Module run by "+(string)user;
    vhistory.push_back(str);
    vhistory.push_back("Parameter List START for "+(string)modulename);
    vhistory.push_back("P1 infile="+(string)infile);
    vhistory.push_back("P2 maskfile="+(string)maskfile);
    vhistory.push_back("P3 aspectfile="+(string)aspectfile);
    vhistory.push_back("P4 outfile="+(string)outfile);
    vhistory.push_back("P5 outputtype="+(string)outputtype);
    vhistory.push_back("P6 timerange="+(string)timerange);
    vhistory.push_back("P7 energyrange="+(string)energyrange);
    vhistory.push_back("P8 bintype="+(string)bintype);
    vhistory.push_back("P9 gtifile="+(string)gtifile);
    char temp[25];
    sprintf(temp,"%d",binsize);
    vhistory.push_back("P10 binsize="+(string)temp);
    vhistory.push_back("P11 tcol="+(string)tcol);
    vhistory.push_back("P12 ecol="+(string)ecol);
    vhistory.push_back("P13 xcol"+(string)xcol);
    vhistory.push_back("P14 ycol="+(string)ycol);
    if(clobber==YES) 
        vhistory.push_back("P15 clobber=yes");
    else
        vhistory.push_back("P15 clobber=no");
    if(history==YES)
        vhistory.push_back("P16 history=yes");
    else
        vhistory.push_back("P16 history=no");
    vhistory.push_back("Parameter List END");
    return (EXIT_SUCCESS);
}

int writeAllDPI(char *file,long **fulldpi,long num,long size){
    fitsfile *fptr;
    int status=0;
    long naxis=2;
    long naxes[2]; naxes[0]=XSIZE; naxes[1]=YSIZE;
    int bitpix=LONG_IMG;
    long fpixel[2]={1,1};
    fits_create_file(&fptr,file,&status);
    if(status){
        LOG(ERROR)<<"Error in creating file "<<file;
        exit(1);
    }
    if(num>20)  num=20;
    for(long i=0;i<num;i++){
        fits_create_img(fptr,bitpix,naxis,naxes,&status);
         if(status){
            LOG(ERROR)<<"Error creating image";
            exit(1);
        }
        fits_write_pix(fptr,TLONG,fpixel,XSIZE*YSIZE,fulldpi[i],&status);
        if(status){
            LOG(ERROR)<<"Error in writing pixels for DPI number "<<i+1;
            exit(1);
        }
    }
    fits_close_file(fptr,&status);
    return 0;
}

int writeWeightedEvent(char *file, double **mskwt_quad){
    
    fitsfile *evtptr, *wevtptr;
    char* wfile= new char [strlen(file)+5];
    int *detx, *dety;
    double *weight;
    int ncols=0;
    int wt_col=0;
    char error[1000];
    strcpy(wfile,file);
    
    sprintf(wfile,"!%s.wevt",file);
     int status=0,i, hdunum, detx_n, dety_n, wt_n;
     long int nrows;
     fits_open_file(&evtptr,file,READONLY,&status);
     fits_report_error(stderr,status);
     printerror(status,"***Error in opening event file***");

     fits_create_file(&wevtptr, wfile, &status);
     printerror(status,"***Error in creating weighted event file***");
     
     fits_copy_file(evtptr, wevtptr,1,1,1,&status);
     printerror(status,"***Error in copying weighted event file to event file***");
     
     fits_close_file(evtptr, &status);
     printerror(status,"***Error in closing event file***");
     
     for(i=0,hdunum=2; i<4;i++,hdunum++){
        fits_movabs_hdu(wevtptr, hdunum, NULL,&status);
        printerror(status,"***Error in moving to specified HDU***");

        fits_get_num_cols(wevtptr, &ncols, &status);
        printerror(status, "***Error in getting total number of columnst***");

        wt_col=ncols+1;
        fits_insert_col(wevtptr, wt_col,"weight", "D", &status);
        printerror(status, "***Error in inserting a new column named weight***");
        
        fits_get_num_rows(wevtptr,&nrows,&status);
        detx=new int[nrows];
        dety= new int[nrows];
        weight= new double[nrows];
        fits_get_colnum(wevtptr,CASEINSEN,"detx",&detx_n,&status);
        fits_get_colnum(wevtptr,CASEINSEN,"dety",&dety_n,&status);
        fits_read_col(wevtptr,TINT,detx_n,1,1,nrows,NULL,detx,NULL,&status); 
        fits_read_col(wevtptr,TINT,dety_n,1,1,nrows,NULL,dety,NULL,&status);
        
        cout << detx[i] << "---------" << dety[i] << endl;
        
        for(int j=0;j<nrows;j++){
            
            

            weight[j]=mskwt_quad[i][(dety[j]*PIXELS_PER_ROW_QUAD+detx[j])];
            //LOG(INFO) << weight[j] << "detY-'" << dety[j] << "---detx--" <<detx[j];
        }


        fits_write_col(wevtptr, TDOUBLE,wt_col,1,1, nrows, weight, &status);
        delete[] dety,detx,weight;
                  
        }

     fits_close_file(wevtptr, &status);
             
    
     
    return 0;
}

void  printerror( int status, char* error="ERROR")
{
    if (status)
    {
        LOG(ERROR)<<error;
         //fits_report_error(stderr,status);
        exit(status);   
    }
    return;
}




