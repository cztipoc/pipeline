
#include "cztbindata.h"

using namespace std;

cztbindata::cztbindata(){
    strcpy(modulename,"cztbindata_v");
    strcat(modulename,VERSION);
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
    if(PIL_OK!=(status=PILGetFname("infile",infile))){
        LOG(ERROR)<<"***Error reading input file"<<infile<<"***";
        return status;
    }
    if(PIL_OK!=(status=PILGetFname("maskfile",maskfile))){
        LOG(ERROR)<<"***Error reading mask file"<<maskfile<<"***";
        return status;
    }
    if(PIL_OK!=(status=PILGetFname("aspectfile",aspectfile))){
        LOG(ERROR)<<"***Error reading aspect file:"<<aspectfile<<"***";
        return status;
    }
    if(PIL_OK!=(status=PILGetFname("pixelqualityfile",pixelqualityfile))){
        LOG(ERROR)<<"***Error reading pixel quality file***";
        return status;
    }
    if(strcmp(pixelqualityfile,"-")==0 || strcmp(pixelqualityfile," ")==0){
        strcpy(this->pixelqualityfile,"\0");
    }
    if(PIL_OK!=(status=PILGetFname("outfile",outfile))){
        LOG(ERROR)<<"***Error reading output file:"<<outfile<<"***";
        return status;
    }
    if(PIL_OK!=(status=PILGetString("timerange",timerange))){
        LOG(ERROR)<<"***Error reading time range***";
        return status;
    }
    if(PIL_OK!=(status=PILGetString("energyrange",energyrange))){
        LOG(ERROR)<<"***Error reading energy range***";
        return status;
    }
    if(PIL_OK!=(status=PILGetString("outtype",outputtype))){
        LOG(ERROR)<<"***Error reading output type***";
        return status;
    }
    if(strcasecmp(outputtype,"lc")==0){
        sflag=false;
        if(PIL_OK!=(status=PILGetString("bintype",bintype))){   //reading bin tyoe in case of lc
            LOG(ERROR)<<"***Error reading binning type***";
            return status;
        }
        if(strncasecmp(bintype,"u",1)==0){
            uflag=true;
            if(PIL_OK!=(status=PILGetReal4("binsize",&binsize))){
                LOG(ERROR)<<"Error reading binsize***";
                return status;
            }
        }
        else if(strncasecmp(bintype,"g",1)==0){
            if(PIL_OK!=(status=PILGetFname("gtifile",gtifile))){
                LOG(ERROR)<<"***Error reading gtifile***";
                return status;
            }
            if(PIL_OK!=(status=PILGetString("gtiextname",gtiextname))){
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
        if(PIL_OK!=(status=PILGetReal4("binsize",&binsize))){  //reading binsize for energy
                LOG(ERROR)<<"Error reading binsize***";
                return status;
        }
    }
    else{
        LOG(ERROR)<<"***Invalid output type value***";
        return (EXIT_FAILURE);
    }
    if(PIL_OK!=(status=PILGetString("RA",RA_str))){
        LOG(ERROR)<<"***Error reading Right Ascension***";
        return status;
    }
    if(PIL_OK!=(status=PILGetString("DEC",DEC_str))){
        LOG(ERROR)<<"***Error reading Declination***";
        return status;
    }
    if(PIL_OK!=(status=PILGetString("tcol",tcol))){
        LOG(ERROR)<<"***Error reading time column name***";
        return status;
    }
    if(PIL_OK!=(status=PILGetString("ecol",ecol))){
        LOG(ERROR)<<"***Error reading energy column name***";
        return status;
    }
    if(PIL_OK!=(status=PILGetString("xcol",xcol))){
        LOG(ERROR)<<"***Error reading X column name***";
        return status;
    }
    if(PIL_OK!=(status=PILGetString("ycol",ycol))){
        LOG(ERROR)<<"***Error reading Y column name***";
        return status;
    }
    if(PIL_OK!=(status=PILGetBool("history",&history))){
        LOG(ERROR)<<"***Error reading history parameter***";
        return status;
    }
    if(PIL_OK!=(status=PILGetBool("clobber",&clobber))){
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

int cztbindata::read(char* infile, char* maskfile, char* aspectfile, char* outfile,
        char *pixelqualityfile,char* outtype, char* timerange, char* energyrange, 
        char* bintype, char* gtifile,char *gtiextname,float binsize, char* Ra, char* Dec, 
        char* tcol,char* ecol, char* xcol, char* ycol,int clobber, int history){
    strcpy(this->infile,infile);
    strcpy(this->maskfile,maskfile);
    strcpy(this->aspectfile,aspectfile);
    if(pixelqualityfile!=NULL)
        strcpy(this->pixelqualityfile,pixelqualityfile);
    else
        strcpy(this->pixelqualityfile,"\0");
    strcpy(this->outfile,outfile);
    strcpy(this->outputtype,outtype);
    strcpy(this->timerange,timerange);
    strcpy(this->energyrange,energyrange);
    if(strcasecmp(outtype,"lc")==0){
        sflag=false;
        strcpy(this->bintype,bintype);
        if(strncasecmp(bintype,"u",1)==0){
            this->binsize=binsize;
            uflag=true;
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
        sflag=true;
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
    this->clobber=clobber;
    this->history=history;
    return (EXIT_SUCCESS);
}

void cztbindata::display(){
    LOG(INFO)<<"----------------------------------------------------------------------------";
    LOG(INFO)<<"                          CZTBINDATA PARAMETERS                            ";
    LOG(INFO)<<"----------------------------------------------------------------------------";
    LOG(INFO)<<"Input Event file       : "<<infile;
    LOG(INFO)<<"Input Mask file        : "<<maskfile;
    LOG(INFO)<<"Input Aspect file      : "<<aspectfile;
    LOG(INFO)<<"Pixel Quality File     : "<<pixelqualityfile;
    LOG(INFO)<<"Output file            : "<<outfile;
    LOG(INFO)<<"Output Type            : "<<outputtype;
    LOG(INFO)<<"Time Range             : "<<timerange;
    LOG(INFO)<<"Energy Range           : "<<energyrange;
    LOG(INFO)<<"Bin Type               : "<<bintype;
    LOG(INFO)<<"GTI file               : "<<gtifile;
    LOG(INFO)<<"Bin Size               : "<<binsize;
    LOG(INFO)<<"Right Ascension        : "<<RA_str; 
    LOG(INFO)<<"Declination            : "<<DEC_str;
    LOG(INFO)<<"Time column            : "<<tcol;
    LOG(INFO)<<"Energy Column          : "<<ecol;
    LOG(INFO)<<"X column               : "<<xcol;
    LOG(INFO)<<"Y column               : "<<ycol;
    LOG(INFO)<<"Clobber                : "<<clobber;
    LOG(INFO)<<"History                : "<<history;
    LOG(INFO)<<"----------------------------------------------------------------------------";
}

int cztbindata::cztbindataProcess(){
    //cout.setf(ios::showpoint);
    //cout.setf(ios::fixed,ios::floatfield);
    //cout.precision(6);
    if(FileExists(outfile)){
        if(clobber==YES)
                unlink(outfile);
        else{
            LOG(ERROR)<<""<<outfile<<" already exists";
            LOG(ERROR)<<"Use clobber=yes for overwriting the file";
            return (EXIT_FAILURE);
        }
    }
    //opening input event file
    fitsfile *fptr;
    int status=0;
    fits_open_file(&fptr,infile,READONLY,&status);
    if(status){ fits_report_error(stderr,status); return (EXIT_FAILURE);  }
     
    if(strcmp(RA_str,"-")==0 || strcmp(DEC_str,"-")==0){
        status=readRaDec(fptr,&ra,&dec);
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
    double theta_x,theta_y;
    if(getCC(aspectfile,ra,dec,&theta_x,&theta_y)){
        LOG(ERROR)<<"***Error in getting camera coordinates***";
        return (EXIT_FAILURE);
    }
    LOG(INFO)<<"Theta X: "<<theta_x*180/M_PI<<"   "<<"Theta Y: "<<theta_y*180/M_PI;  //printing in degrees
    //computing shift of mask pattern
    double dx,dy;
    dx=(-1)*MASKHEIGHT*tan(theta_x);
    dy=(-1)*MASKHEIGHT*tan(theta_y);
    LOG(INFO)<<"Shift of Mask  --- dx:"<<dx<<"  dy:"<<dy;
    if(abs(dx)>DETECTORWIDTH || abs(dy)>DETECTORWIDTH){
       LOG(ERROR)<<"***Error-Shift is greater than detector width (39 mm)***";
       return (EXIT_FAILURE);
    }
    
//    float maskweight[YSIZE][XSIZE],openf[YSIZE][XSIZE],area[YSIZE][XSIZE];
//    //array initialization
//    for(int i=0;i<XSIZE;i++){
//        for(int j=0;j<YSIZE;j++){
//            maskweight[i][j]=0;
//            openf[i][j]=0;
//            area[i][j]=0;
//        }
//    }
    float **maskweight,**openf,**area;
    //array initialization
    maskweight=new float*[YSIZE];
    openf=new float*[YSIZE];
    area=new float*[YSIZE];
    if(maskweight==NULL || openf==NULL || area==NULL){
        LOG(ERROR)<<"***Out of Memory***";
        return (EXIT_FAILURE);
    }
    for(int i=0;i<YSIZE;i++){
            maskweight[i]=new float[XSIZE];
            openf[i]=new float [XSIZE];
            area[i]=new float[XSIZE];
            if(maskweight[i]==NULL || openf[i]==NULL || area[i]==NULL){
                LOG(ERROR)<<"***Out of Memory***";
                return (EXIT_FAILURE);
            }
    }
    
    LOG(INFO)<<"Computing weights...";
    if(getWeight(maskweight,openf,maskfile,dx,dy)){
        LOG(ERROR)<<"***Error in getweight***";
        return (EXIT_FAILURE);
    }
    double ta=0;                            //total area
    LOG(INFO)<<"Computing area...";
    getArea(area,&ta);
    //Assigning weight zero to all bad pixels
    if(strcmp(pixelqualityfile,"\0")!=0){
        LOG(INFO)<<"Using pixel quality file";
        filterWeight(pixelqualityfile,maskweight);
    }
    //----------------------------------------------------------------
    //writing weight, open fraction and area to a file
    char filename[FLEN_FILENAME];  //filename to be used to write the weights,open fraction and area
//    sprintf(filename,"!%s.weight",infile);
//    writeArray(filename,maskweight,YSIZE,XSIZE);
//    sprintf(filename,"!%s.openf",infile);
//    writeArray(filename,openf,YSIZE,XSIZE);
//    sprintf(filename,"!%s.area",infile);
//    writeArray(filename,area,YSIZE,XSIZE);
    //---------------------------------------------------------
    double WA=0,R=0,sumw=0,sumwf=0;         //WA - weighted area, R - Reliability Index 
                                        //WA = summation of (weight x open fraction x area)
                                        //sumw - summation of weights,  
                                        //sumwf - summation of weight*openfraction
    int i,j;
    
    for(i=0;i<YSIZE;i++){
        for(j=0;j<XSIZE;j++){
            WA=WA+(maskweight[i][j]*openf[i][j]*area[i][j]);
            sumw=sumw+maskweight[i][j];
            sumwf=sumwf+(maskweight[i][j]*openf[i][j]);
        }
    }

    R=1.0-(sumw/sumwf);
    //cout<<"Weighted Area is "<<WA;
    //cout<<"Reliability Index is "<<R;
    
    //creating time bins/energy bins
    if(makeBins()){
        LOG(ERROR)<<"Error in creating bins";
        return (EXIT_FAILURE);
    }
    fitsfile *fout;                   //for output file
    fits_create_file(&fout,outfile,&status);
    if(status){  fits_report_error(stderr,status);  return (-1); }
    if(prepareOutputFile(fptr,fout,sflag,ecol)){
        LOG(ERROR)<<"Error in creating output file";
        return (EXIT_FAILURE);
    }
    //writing keywords for tstart,tstop,estart,estop
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
    long *qdpi[NUMQUAD],**fulldpi;
    for(int i=0;i<NUMQUAD;i++){
        qdpi[i]=new long[XPIX_QUAD*YPIX_QUAD];
        if(qdpi[i]==NULL)  { LOG(ERROR)<<"Out of memory error"; return (EXIT_FAILURE); }
    }
    fulldpi=new long *[nbins];
    if(fulldpi==NULL)  { LOG(ERROR)<<"Out of memory error"; return (EXIT_FAILURE); }
    for(int i=0;i<nbins;i++){
        fulldpi[i]=new long[XSIZE*YSIZE];
        if(fulldpi[i]==NULL)   { LOG(ERROR)<<"Out of memory error"; return (EXIT_FAILURE); }
    }
    
    //binning
    status=doBinning(fptr,sflag,tstart,tstop,estart,estop,binmin,
            binmax,nbins,ecol,tcol,xcol,ycol,fulldpi);
    if(status) { LOG(ERROR)<<"Error in binning";  return (EXIT_FAILURE); }
    //cout<<"Binning Complete";
    double sumwc=0.0,sumw2c=0.0,ut,channel;
    long totcount=0;
    for(int i=0;i<nbins;i++){
        sumwc=0;
        sumw2c=0;
        for(int j=0;j<XSIZE*YSIZE;j++){
            sumwc=sumwc+(maskweight[j/YSIZE][j%YSIZE]*fulldpi[i][j]);
            //cout<<fulldpi[i][j]<<"\t"<<maskweight[j/YSIZE][j%YSIZE];
            sumw2c=sumw2c+(maskweight[j/YSIZE][j%YSIZE]*maskweight[j/YSIZE][j%YSIZE]*fulldpi[i][j]);
        }
        //cout<<"Weighted sum SUM:"<<sumwc;
        flux[i]=sumwc/WA;
        //flux[i]=i*2;
        //cout<<sumwc/WA<<"   "<<sumwc<<"   "<<WA;
        error[i]=sumw2c/WA;
        xval[i]=binmin[i]+binsize/2;
    }    
    
//    for(int i=0;i<nbins;i++){
//        cout<<flux[i]<<"   "<<xval[i];
//    }
    
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
    
    
//    if(writeCurve(fout,xval,flux,error,nbins)){            //to write curve to output file
//        cerr<<"Error in writing curve to output file";
//        return (EXIT_FAILURE);
//    }
    //writing bins to output file
    fits_movabs_hdu(fout,3,NULL,&status);
    if(status) { fits_report_error(stderr,status);  return (EXIT_FAILURE); }
    fits_write_col(fout,TDOUBLE,1,1,1,nbins,binmin,&status);
    if(status) { fits_report_error(stderr,status);  return (EXIT_FAILURE); }
    fits_write_col(fout,TDOUBLE,2,1,1,nbins,binmax,&status);
    if(status) { fits_report_error(stderr,status);  return (EXIT_FAILURE); }

    
    fits_close_file(fout,&status);            //closing output file
    if(status) { fits_report_error(stderr,status);  return (EXIT_FAILURE); }
    fits_close_file(fptr,&status);          //closing input file
    if(status){ fits_report_error(stderr,status); return (EXIT_FAILURE);  }
    //updating keywords in output file
    updateKeywords(outfile,modulename);
    //writing history to output file
    if(history==YES){
         vector<string> vhistory;
         getHistory(vhistory);
         writeHistory(outfile,vhistory);           //writes history to each HDU of output file
     }
    
    for(int i=0;i<NUMQUAD;i++)  delete[] qdpi[i];
    for(int i=0;i<nbins;i++)  delete[] fulldpi[i];
    delete[] fulldpi;
    delete[] flux,xval,error;
    
    for(int i=0;i<YSIZE;i++){
        delete[] maskweight[i];
        delete[] openf[i];
        delete[] area[i];
    }
    delete[] maskweight,openf,area;
            
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
        min<double>(time1.data(),time1.size(),&tstart);
        max<double>(time2.data(),time2.size(),&tstop);
    }
    else{
        getbins(timerange,&tstart,&tstop,1);   
    }
    //cout<<"Time Range of output : "<<tstart<<"-"<<tstop;
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
    //cout<<"Energy Range of output : "<<estart<<"-"<<estop;
    //making bins......Time bins (Light curve) / Energy Bins (Spectrum)
    double range;
    if(sflag){                           //for spectrum
        range=estop-estart;
        if((long)(range*1000000)%(long)(binsize*1000000)==0)     nbins=(int)(range/binsize);
        else nbins=(range/binsize)+1;
        binmin=new double[nbins];
        binmax=new double[nbins];
        for(int i=0;i<nbins;i++){
            binmin[i]=estart+i*binsize;        //making energy bins
            binmax[i]=binmin[i]+binsize;
        }
    }
    else{                                  //for light curve
        if(uflag){                     //in case of uniform binning
            range=tstop-tstart;
            if((long)(range*1000000)%(long)(binsize*1000000)==0)     nbins=range/binsize;
            else nbins=(range/binsize)+1;
            binmin=new double[nbins];
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
    return (EXIT_SUCCESS);
}

int readRaDec(fitsfile *fptr,double *ra,double *dec){
    int status=0;
    fits_read_key(fptr,TDOUBLE,"RA",ra,NULL,&status);
    if(status){
        fits_report_error(stderr,status);
        return (EXIT_FAILURE);
    }
    fits_read_key(fptr,TDOUBLE,"DEC",dec,NULL,&status);
    if(status){
        fits_report_error(stderr,status);
        return (EXIT_FAILURE);
    }
    return (EXIT_SUCCESS);
}

int filterWeight(char *pixelqualityfile,float **weight){
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
        col=i%YSIZE;     row=i/YSIZE;
        if(pixelquality[i]!=GOODPIX){
            weight[row][col]=0;
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

int doBinning(fitsfile *fevt,bool sflag,double tstart,double tstop,double estart,
        double estop,double *binmin,double *binmax,int nbins,char *ecol,char *tcol,
        char *xcol,char *ycol,long **fulldpis){                //size of fulldpis will be nbins x (XSIZE*YSIZE)
    //cout<<"Energy "<<emin<<"-"<<emax;
    //cout<<"\tTime "<<tstart<<"-"<<tstop;
    //cout<<"Bins";
    //for(int i=0;i<nbins;i++)
        //cout<<""<<binmin[i]<<"-"<<binmax[i];
    //cout;
    //cout<<"Starting to generate DPI";
    int status=0;
    char filename[FLEN_FILENAME];
    fits_file_name(fevt,filename,&status);
    if(status){
        LOG(ERROR)<<"Error getting filename";
        fits_report_error(stderr,status);
        return (EXIT_FAILURE);
    }
    int ecol_n,tcol_n,xcol_n,ycol_n;
    long nrows=0,firstrow=1,firstelem=1;
    long totcount=0;
    for(int i=0,hdunum=EVENTDATA_HDU_BEGIN;i<NUMQUAD;i++,hdunum++){
        fits_movabs_hdu(fevt,hdunum,NULL,&status);
        if(status){
            LOG(ERROR)<<"Error in moving to HDU "<<hdunum<<" in file "<<filename;
            fits_report_error(stderr,status);
            return (EXIT_FAILURE);
        }
        fits_get_colnum(fevt,CASEINSEN,ecol,&ecol_n,&status);
        if(status){
            LOG(ERROR)<<"Error getting column number for ecol in file "<<filename;
            fits_report_error(stderr,status);
            return (EXIT_FAILURE);
        }
        fits_get_colnum(fevt,CASEINSEN,tcol,&tcol_n,&status);
        if(status){
            fits_report_error(stderr,status);
            return (EXIT_FAILURE);
        }
        fits_get_colnum(fevt,CASEINSEN,xcol,&xcol_n,&status);
        if(status){
            fits_report_error(stderr,status);
            return (EXIT_FAILURE);
        }
        fits_get_colnum(fevt,CASEINSEN,ycol,&ycol_n,&status);
        if(status){
            fits_report_error(stderr,status);
            return (EXIT_FAILURE);
        }
        //get number of rows in event file
        fits_get_num_rows(fevt,&nrows,&status);
        if(status){
            fits_report_error(stderr,status);
            return (EXIT_FAILURE);
        }
        //cout<<"Number of Rows Found:"<<nrows;
        //cout<<"Total count:"<<totcount;
        //reading x,y,time and energy column data
        if(nrows>0){      
            double *time,*energy;
            unsigned char *detx,*dety;
            time=(double *)malloc(nrows*sizeof(double));
            energy=(double *)malloc(nrows*sizeof(double));
            detx=(unsigned char *)malloc(nrows*sizeof(char));
            dety=(unsigned char *)malloc(nrows*sizeof(char));
            if(time==NULL || energy==NULL || detx==NULL || dety==NULL){
                LOG(ERROR)<<"***Out of Memory Error-doBinning()***";
                return (EXIT_FAILURE);
            }
            //cout<<"Starting to read";   
            //reading energy column
            fits_read_col(fevt,TDOUBLE,ecol_n,firstrow,firstelem,nrows,NULL,energy,NULL,&status); 
            if(status){
                fits_report_error(stderr,status);
                return (EXIT_FAILURE);
            }
            //reading time column
            fits_read_col(fevt,TDOUBLE,tcol_n,firstrow,firstelem,nrows,NULL,time,NULL,&status); 
            if(status){
                fits_report_error(stderr,status);
                return (EXIT_FAILURE);
            }
            //reading x column
            fits_read_col(fevt,TBYTE,xcol_n,firstrow,firstelem,nrows,NULL,detx,NULL,&status); 
            if(status){
                fits_report_error(stderr,status);
                return (EXIT_FAILURE);
            }
            //reading y column
            fits_read_col(fevt,TBYTE,ycol_n,firstrow,firstelem,nrows,NULL,dety,NULL,&status); 
            if(status){
                fits_report_error(stderr,status);
                return (EXIT_FAILURE);
            }
            //making detx and dety wrt to full array of 128 x 128
            for(long k=0;k<nrows;k++){
                switch(i){
                    case 1: detx[k]=detx[k]+XPIX_QUAD;
                        break;
                    case 2: detx[k]=detx[k]+XPIX_QUAD;
                            dety[k]=dety[k]+YPIX_QUAD;    
                        break;
                    case 3: dety[k]=dety[k]+YPIX_QUAD;
                        break;
                }
             }
            
            //cout<<"Reading column data complete";   
            //Binning to make DPI
            long index=0;
            //cout<<"Starting binning";
            for(long k=0;k<nrows;k++){
                //x=detx[i]; y=dety[i];
                //cout<<""<<energy[k]<<" "<<time[k]<<"  "<<(int)detx[k]<<"  "<<(int)dety[k];
                index=dety[k]*YSIZE+detx[k];
                if(index>(XSIZE*YSIZE)) {
                    LOG(ERROR)<<"dety:"<<dety[k]<<"  detx:"<<detx[k]<<" Index:"<<index;
                    LOG(ERROR)<<"Index Out of bounds";
                    return (EXIT_FAILURE);
                }
                //cout<<"k:"<<k<<"  "<<energy[k];
                int binindex=0;
                if(sflag){
                    if(energy[k]<binmin[0] || energy[k]>=binmax[nbins-1])  continue;
                    while(energy[k]>binmax[binindex])
                         binindex++;
                    //if(binindex>nbins-1) { cout<<""<<binindex<<" : Bin Index Out of Bounds";  continue; }
                    if(time[k]>=tstart && time[k]<tstop)
                            fulldpis[binindex][index]++;
                    
                }
                else{
                    if(time[k]<binmin[0] || time[k]>=binmax[nbins-1])  continue;
                    while(time[k]>binmax[binindex])
                        binindex++;  
                    //if(binindex>nbins-1) { cout<<"Bin Index Out of Bounds";  continue; }
                    if(energy[k]>=estart && energy[k]<estop && time[k]>=binmin[binindex] && time[k]<binmax[binindex])  //checking again due to gti binning
                        fulldpis[binindex][index]++;
                    
                    
                }
                
                //cout<<"Bin Index:"<<binindex;                            
            }
            free(time);
            free(energy);
            free(detx);
            free(dety);
        }
    }
        //cout<<"DPI generated for "<<emin<<"-"<<emax<<" and "<<tstart<<"-"<<tstop;
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
