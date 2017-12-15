
#include "cztfindbadpix.h"

using namespace std; 

cztfindbadpix::cztfindbadpix(){
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

int cztfindbadpix::read(int argc,char **argv){
    int status=0;
   
    if(PIL_OK!=(status=PILInit(argc,argv))){
        LOG(ERROR)<<"***Error Initializing PIL***";
        return status;
    }
    
    if(PIL_OK!=(status=PILGetFname("infile",infile))){
        LOG(ERROR)<<"***Error Reading input file:"<<infile<<"***";
        return status;
    }
    
    if(PIL_OK!=(status=PILGetString("inputtype",inputtype))){
        LOG(ERROR)<<"***Error Reading inputtype:"<<inputtype<<"***";
        
        return status;
    }
    
    if(strncasecmp(inputtype,"Eve",3)==0){
        if(PIL_OK!=(status=PILGetString("xcol",xcol))){
            LOG(ERROR)<<"***Error Reading X column name:"<<xcol<<"***";
            return status;
        }
        if(PIL_OK!=(status=PILGetString("ycol",ycol))){
            LOG(ERROR)<<"***Error Reading Y column name:"<<ycol<<"***";
            return status;
        }
    }
    
    if(PIL_OK!=(status=PILGetFname("CALDBbadpixelfile",CALDBbadpixelfile))){
        LOG(ERROR)<<"***Error Reading badpixelfile:"<<CALDBbadpixelfile<<"***";
        return status;
    }
    
    if(PIL_OK!=(status=PILGetFname("outfile",outfile))){
        LOG(ERROR)<<"***Error Reading outfile:"<<outfile<<"***";
        return status;
    }
    
    if(PIL_OK!=(status=PILGetBool("useAlgo",&useAlgo))){
        LOG(ERROR)<<"***Error Reading useAlgo parameter***";
        return status;
    }
    if(useAlgo==YES){
         if(PIL_OK!=(status=PILGetFname("aspectfile",aspectfile))){
            LOG(ERROR)<<"***Error Reading aspectfile:"<<aspectfile<<"***";
            return status;
         }
         if(PIL_OK!=(status=PILGetFname("maskfile8100",maskfile8100))){
            LOG(ERROR)<<"***Error Reading Mask file:"<<maskfile8100<<"***";
            return status;
         }
         if(PIL_OK!=(status=PILGetString("ra",ra))){
            LOG(ERROR)<<"***Error Reading RA***";
            return status;
         }
         if(PIL_OK!=(status=PILGetString("dec",dec))){
            LOG(ERROR)<<"***Error Reading DEC***";
            return status;
         }
         if(PIL_OK!=(status=PILGetReal4("threshold",&threshold))){
            LOG(ERROR)<<"***Error Reading threshold***";
            return status;
         }    
    }
    
    if(PIL_OK!=(status=PILGetBool("history",&history))){
        LOG(ERROR)<<"***Error Reading history parameter***";
        return status;
    }
    
    if(PIL_OK!=(status=PILGetBool("clobber",&clobber))){
        LOG(ERROR)<<"***Error Reading clobber***";
        return status;
    }

    PILClose(status);
    return (EXIT_SUCCESS);
}

int cztfindbadpix::read(char *infile,char *inputtype,char *CALDBbadpixfile,
        char *outfile,int useAlgo,float threshold,char *xcol,char *ycol,
        char *aspectfile,char *maskfile,char *ra,char *dec,int clobber,int history){
    strcpy(this->infile,infile);
    strcpy(this->inputtype,inputtype);
    strcpy(this->CALDBbadpixelfile,CALDBbadpixfile);
    strcpy(this->outfile,outfile);
    this->useAlgo=useAlgo;
    if(xcol!=NULL)
        strcpy(this->xcol,xcol);
    if(ycol!=NULL)
        strcpy(this->ycol,ycol);
    if(aspectfile!=NULL)
        strcpy(this->aspectfile,aspectfile);
    if(maskfile8100!=NULL)
        strcpy(this->maskfile8100,maskfile);
    if(ra!=NULL)
        strcpy(this->ra,ra);
    if(dec!=NULL)
        strcpy(this->dec,dec);
    this->threshold=threshold;
    this->clobber=clobber;
    this->history=history;
    return (EXIT_SUCCESS);
}

void cztfindbadpix::display(){
    LOG(INFO)<<"----------------------------------------------------------------------------";
    LOG(INFO)<<"                          CZTFINDBADPIX PARAMETERS                            ";
    LOG(INFO)<<"----------------------------------------------------------------------------";
    LOG(INFO)<<"Input event file       : "<<infile;
    LOG(INFO)<<"Input Type             : "<<inputtype;
    LOG(INFO)<<"CALDB Bad Pixel File   : "<<CALDBbadpixelfile;
    LOG(INFO)<<"useAlgo                : "<<useAlgo;
    LOG(INFO)<<"Threshold              : "<<threshold;
    LOG(INFO)<<"Output Bad Pixel file  : "<<outfile; 
    LOG(INFO)<<"X column name          : "<<xcol;
    LOG(INFO)<<"Y column name          : "<<ycol;
    LOG(INFO)<<"Aspect file            : "<<aspectfile;
    LOG(INFO)<<"Mask file              : "<<maskfile8100;
    LOG(INFO)<<"Right Ascension        : "<<ra;
    LOG(INFO)<<"Declination            : "<<dec;
    LOG(INFO)<<"Clobber                : "<<clobber;
    LOG(INFO)<<"History                : "<<history;
    LOG(INFO)<<"----------------------------------------------------------------------------";
}


/**
 * Function to detect bad pixels from event file or dpi file
 * output file contains a bad pixel map with quality as
 * 0 - GOOD
 * 1 - DEAD
 * 2 - NOISY
 * 3 - BAD
 * @param par
 * @return 
 */
int cztfindbadpix::cztfindbadpixProcess(){
     int status=0;
     string errorMsg="";
     fitsfile *fin; //fits pointer to input event/dpi file
     Badpix badpix;


    // Checking whether output badpixel file exists or not
    // If yes then deletes it (for clobber=yes)
    // Otherwise raises an error
    if (FileExists(outfile)) {
        if (clobber == YES) {
            if (unlink(outfile) != 0) {
                LOG(ERROR) << "Error in deleting Output Event File: " << outfile;
            }
        } else {
            LOG(INFO) << outfile << " already exists.";
            LOG(INFO) << "Use clobber=yes for overwriting the file.";
            return (EXIT_FAILURE);
        }
    }
    // Output badpixel file existence check finished.

    fits_open_file(&fin, infile, READONLY, &status);
    if (status) {
        LOG(ERROR) << "Error in opening input file";
        fits_report_error(stderr, status);
        return status;
    }
    if (createBadPixFile(fin, outfile, XSIZE, YSIZE)) {
        LOG(ERROR) << "***Error creating empty bad pixel file***";
        return (EXIT_FAILURE);
    }

    // reading quality map from bad pix file and storing it in a 2D array of size 128x128 representing all the quadrants;
    unsigned char qualitymap[XSIZE * YSIZE];
    if (badpix.read_badpix_file(CALDBbadpixelfile, "Q0")) {
        LOG(INFO) << "unable to read gains file.";
        exit(EXIT_FAILURE);
    }
    badpixMapQuad = badpix.create_badPixMap(status);
    
    
    if (getBadPix4mCALDB(CALDBbadpixelfile, qualitymap)) {
        LOG(ERROR) << "***Error in getBadPix4mCALDB()***";
        return (EXIT_FAILURE);
    }
     
 
     
     if(useAlgo==YES){
         long dpi[XSIZE*YSIZE];
         if(getFullDPI(fin,inputtype,xcol,ycol,dpi)){
             return (EXIT_FAILURE);
         }
         //writeImg("!DPIInput",dpi,XSIZE,YSIZE);   //Just to check
         double ra_double=0,dec_double=0;
         if(strcmp(ra,"-")==0 || strcmp(dec,"-")==0){
             if(getRaDec(aspectfile,1,&ra_double,&dec_double)){
                 LOG(ERROR)<<"***Error in getRaDec()***";
                 return (EXIT_FAILURE);
             }
         }
         else{
            ra_double=atof(ra);
            dec_double=atof(dec);
         }
         
         //converting ra and dec to radians
         ra_double=ra_double*M_PI/180;
         dec_double=dec_double*M_PI/180;
         //cout<<"RA:"<<ra_double<<"\tDEC:"<<dec_double;
         detectDeadNoisyPix(dpi,ra_double,dec_double,aspectfile,maskfile8100, threshold,qualitymap);      
     }
     
     //writing quality map to output file
    if(writeQmap(outfile,qualitymap)){
        LOG(ERROR)<<"***Error in writing quality map to output file***";
        return (EXIT_FAILURE);
    }

     
     fits_close_file(fin,&status);
     if(status){
        fits_report_error(stderr,status);
        return status;
     }
     //updating keywords in output file
     updateKeywords(outfile,modulename);
    //writing history to output file
     if(history==YES){
         vector<string> vhistory;
         getHistory(vhistory);
         writeHistory(outfile,vhistory);
     }
     
    return (EXIT_SUCCESS);
}

//function to create bad pixel file 
int createBadPixFile(fitsfile *fin,char *outfile,int xsize,int ysize){
    int status=0;
    fitsfile *fbad;
    fits_create_file(&fbad,outfile,&status);
    if(status){
        LOG(ERROR)<<"Error creating output bad pixel file";
        fits_report_error(stderr,status);
        return status;
    }
    int bitpix=BYTE_IMG;
    long naxes[2];
    naxes[0]=xsize;
    naxes[1]=ysize;
    int naxis=2;
    fits_create_img(fbad,bitpix,naxis,naxes,&status);
    if(status){
        LOG(ERROR)<<"Error creating image for bad pixel quality map";
        fits_report_error(stderr,status);
        return status;
    }
    
    //moving to hdu 1 in input file
    fits_movabs_hdu(fin,1,NULL,&status);
    if(status){
        LOG(ERROR)<<"Error moving to first hdu in input file";
        fits_report_error(stderr,status);
        return status;
    }
    
    //Keywords in bad pixel file from input file
    if(copyUserKeyWords(fin,fbad)){
        LOG(ERROR)<<"***Error copying keywords from input file***";
        return (EXIT_FAILURE);
    }
    char extname[]="QUALITYMAP";
    fits_write_key(fbad,TSTRING,"EXTNAME",extname,NULL,&status);
    int goodpixval=GOODPIX,noisypixval=NOISYPIX,deadpixval=DEADPIX,badpixval=BADPIX;
        
    fits_write_key(fbad,TINT,GOODPIX_VAL_KEYWORD,&goodpixval,"Good pixel value",&status);
    fits_write_key(fbad,TINT,DEADPIX_VAL_KEYWORD,&deadpixval,"Dead pixel value",&status);
    fits_write_key(fbad,TINT,NOISYPIX_VAL_KEYWORD,&noisypixval,"Noisy pixel value",&status);
    fits_write_key(fbad,TINT,BADPIX_VAL_KEYWORD,&badpixval,"Bad pixel value",&status);
    fits_close_file(fbad,&status);
    if(status){
        LOG(ERROR)<<"Error closing bad pixel quality map file";
        fits_report_error(stderr,status);
        return status;
    }
    return (EXIT_SUCCESS);
}

int getBadPix4mCALDB(char *caldbfile,unsigned char *qmap){
    int status=0;
    fitsfile *fbadpix_caldb;
    fits_open_file(&fbadpix_caldb,caldbfile,READONLY,&status);
    if(status){
        LOG(ERROR)<<"Error opening CALDB bad pixel file :"<<caldbfile;
        fits_report_error(stderr,status);
        return status;
    }
    fits_movnam_hdu(fbadpix_caldb,IMAGE_HDU,(char *)BADPIX_EXT_CALDB,0,&status);
    if(status){
        LOG(ERROR)<<"Error moving to badpix extension in CALDB bad pixel file";
        fits_report_error(stderr,status);
        return status;
    }
    int maxdim=4;
    long naxes[4];
    int bitpix,naxis;
    fits_get_img_param(fbadpix_caldb,maxdim,&bitpix,&naxis,naxes,&status);
    if(status){
        LOG(ERROR)<<"Error moving to badpix extension in CALDB bad pixel file";
        fits_report_error(stderr,status);
        return status;
    }
    if(naxis!=2){
        LOG(ERROR)<<"***Found "<<naxis<<" axes in caldb bad pixel mapMust have 2 axes***";
        return (EXIT_FAILURE);
    }
    if(naxes[0]!=XSIZE || naxes[1]!=YSIZE){
        LOG(ERROR)<<"***Found "<<naxes[0]<<" x "<<naxes[1]<<" image in CALDBExpected size is "<<XSIZE<<" x "<<YSIZE<<"***";
        return (EXIT_FAILURE);
    }
    long fpixel[2];
    fpixel[0]=fpixel[1]=1;
    fits_read_pix(fbadpix_caldb,TBYTE,fpixel,XSIZE*YSIZE,NULL,qmap,NULL,&status);
    if(status){
        LOG(ERROR)<<"Error reading caldb file";
        fits_report_error(stderr,status);
        return status;
    }
    int badpixl=0;
    fits_read_key(fbadpix_caldb,TINT,(char *)BADPIX_VAL_KEYWORD,&badpixl,NULL,&status);
    if(status){
        LOG(ERROR)<<"Error reading "<<BADPIX_VAL_KEYWORD<<" keyword in caldb file";
        fits_report_error(stderr,status);
        return status;
    }
  
    for(int i=0;i<XSIZE*YSIZE;i++){
        if(qmap[i]==badpixl)
            qmap[i]=BADPIX;
        else
            qmap[i]=GOODPIX;
    }
    return (EXIT_SUCCESS);
}

 int writeQmap(char *outfile,unsigned char *qualitymap){
     
     long ngoodpix=0,ndeadpix=0,nnoisypix=0,nbadpix=0;
     
     for(int i=0;i<XSIZE*YSIZE;i++){
         switch(qualitymap[i]){
             case GOODPIX: ngoodpix++;
                break;
             case DEADPIX: ndeadpix++;  nbadpix++;
                break;
             case NOISYPIX: nnoisypix++; nbadpix++;
                break;
             case BADPIX: nbadpix++;
         }
     }
     LOG(INFO)<<"GOOD PIXELS-------:"<<ngoodpix;
     LOG(INFO)<<"DEAD PIXELS-------:"<<ndeadpix;
     LOG(INFO)<<"NOISY PIXELS------:"<<nnoisypix;
     LOG(INFO)<<"BAD PIXELS--------:"<<nbadpix;
     fitsfile *fqmap;
     int status=0;
     fits_open_file(&fqmap,outfile,READWRITE,&status);
     if(status){
         LOG(ERROR)<<"***Error opening output quality map file***";
         fits_report_error(stderr,status);
         return (EXIT_FAILURE);
     }
     long fpixel[2];
     fpixel[0]=fpixel[1]=1;
     fits_write_pix(fqmap,TBYTE,fpixel,XSIZE*YSIZE,qualitymap,&status);
     if(status){
         LOG(ERROR)<<"***Error writing pixels in output quality map file***";
         fits_report_error(stderr,status);
         return (EXIT_FAILURE);
     }
     fits_write_key(fqmap,TLONG,"NGOOD",&ngoodpix,NULL,&status);
     fits_write_key(fqmap,TLONG,"NDEAD",&ndeadpix,NULL,&status);
     fits_write_key(fqmap,TLONG,"NNOISY",&nnoisypix,NULL,&status);
     fits_write_key(fqmap,TLONG,"NBAD",&nbadpix,NULL,&status);
     fits_close_file(fqmap,&status);
     if(status){
         LOG(ERROR)<<"***Error closing output quality map file***";
         fits_report_error(stderr,status);
         return (EXIT_FAILURE);
     }
     return (EXIT_SUCCESS);
 }
 
 int detectDeadNoisyPix(long *dpi_pixels,double ra,double dec,char *aspectfile,char *maskfile,
        float threshold,unsigned char *qmap){
        
         
        float emap[XSIZE*YSIZE],deviation[XSIZE*YSIZE];
        float dpi[XSIZE*YSIZE];
        for(int i=0;i<XSIZE*YSIZE;i++)  dpi[i]=(float)dpi_pixels[i];
        double theta_x,theta_y;

        if(getCC(aspectfile,ra,dec,&theta_x,&theta_y)){
            LOG(ERROR)<<"Error in getCC()";
            LOG(ERROR)<<"***Error in detectDeadNoisyPix()***";
            return (EXIT_FAILURE);
        }
        
        //edited by Tanul Gupta
        MaskWeighting mskwt(maskfile,theta_x,theta_y);
        if(mskwt.run()){
                LOG(ERROR)<<"****Error in mask Weighting****";
                return (EXIT_FAILURE);
        }
        mskwt.getExposureMap(emap);
        
        //edited by Tanul Gupta
        //writeImg("!ExposureMap",emap,XSIZE,YSIZE);       //JUST TO CHECK;

        double coef[2];
        LOG(INFO)<<"Fitting DPI counts and Exposure Map counts......";
        int flag=fitPolynomial<float>(emap,dpi,XSIZE*YSIZE,coef,1);    //coef[0]=B,coef[1]=S
        if(flag) { LOG(ERROR)<<"***Error in fitting***"; return (EXIT_FAILURE); }
        LOG(INFO)<<"RMSE in fitting DPI counts and Exposure Map counts:"<<RMSE(emap,dpi,XSIZE*YSIZE,coef,1);
        LOG(INFO)<<"Mean Source Count....:"<<coef[1];
        LOG(INFO)<<"Background...........:"<<coef[0];
        double cmodel;
        for(int i=0;i<XSIZE*YSIZE;i++){
            cmodel=coef[1]*emap[i]+coef[0];
            deviation[i]=(float)dpi[i]-cmodel;
        }
        vector<float> set_d1,set_d2;
        vector<int> set_p1,set_p2;   //for pixel locations
        for(int i=0;i<XSIZE*YSIZE;i++){
            if(emap[i]>SETBOUNDRY){
                set_d1.push_back(deviation[i]);
                set_p1.push_back(i);
            }
            else{
                set_d2.push_back(deviation[i]);
                set_p2.push_back(i);
            }
        }
        //for first set
        double rms_pix=0;
        rms_pix=getrms<float>(set_d1.data(),set_d1.size());
        double T=threshold*rms_pix;
        LOG(INFO)<<"Number of pixels in set 1:"<<set_d1.size();
        LOG(INFO)<<"RMS :"<<rms_pix<<"\t"<<T;
        for(int i=0;i<set_d1.size();i++){
            if(set_d1[i]<(-1*T)) qmap[set_p1[i]]=DEADPIX;
            else if(set_d1[i]>T)      qmap[set_p1[i]]=NOISYPIX;
        }
        
        //for second set
        rms_pix=getrms<float>(set_d2.data(),set_d2.size());
        T=threshold*rms_pix;
        LOG(INFO)<<"Number of pixels in set 2:"<<set_d2.size();
        LOG(INFO)<<"RMS :"<<rms_pix<<"\t"<<T;
        for(int i=0;i<set_d2.size();i++){
            if(set_d2[i]>T)        qmap[set_p2[i]]=NOISYPIX;
            else if(set_d2[i]<(-1*T))   qmap[set_p2[i]]=DEADPIX;
        }
        return (EXIT_SUCCESS);
 }
 

/**
 * Function to generate history for the module, to be written to the output file
 * @param par
 * @param vhistory
 * @return 
 */
int cztfindbadpix::getHistory(vector<string> &vhistory){
    //char *user=getlogin();
    char *user = getenv("USER");
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
    vhistory.push_back("Parameter List END");
    return (EXIT_SUCCESS);
}

