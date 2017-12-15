
#include"cztgtigen.h"

using namespace std;

cztgtigen::cztgtigen(){
    
    strcpy(modulename,"cztgtigen_v");
    strcat(modulename,VERSION);
    strcpy(thresholdfile,"");
    strcpy(mkffile,"");
    strcpy(l2gtifile,"");
    strcpy(customgti,"");
    strcpy(gtiextname,"");
}

int cztgtigen::read(int argc, char** argv){
    int status=0;
    
    if(PIL_OK!=(status=PILInit(argc,argv))){
        LOG(ERROR)<<"***Error Initializing PIL***";
        return status;
    }
    
    LOG(INFO)<<"--------------------------------------";
    LOG(INFO)<<"Description for run mode parameter for CZTGTIGEN";
    LOG(INFO)<<"--------------------------------------";
    LOG(INFO)<<"It can have 4 values- copy/mkf/custom/custom-refined";
    LOG(INFO)<<"1) 'mkf'          - In mkf mode, it creates a gti based on the valid ranges given in ";
    LOG(INFO)<<"                    threshold file for each mkf parameter";
    LOG(INFO)<<"2) 'custom'       - In custom mode, it creates gti based on intervals given by user";
    LOG(INFO)<<"3) 'mkf-refined'  - In this mode it refines the gti produced in mkf mode with level2 gti";
    LOG(INFO)<<"4) 'custom-refined'- In this mode it refines the gti produced in custom mode with level2 gti";
    LOG(INFO)<<"5) 'copy' - copies input file to output file";
    LOG(INFO);
    if(PIL_OK!=(status=PILGetString("runmode",runmode))){
        LOG(ERROR)<<"***Error Reading runmode parameter***";
        return status;
    }
      
    if(strncasecmp(runmode,"mkf-refined",11)==0)  runmodeflag=MKF_REFINED;
    else if(strncasecmp(runmode,"custom-refined",14)==0)  runmodeflag=CUSTOM_REFINED;
    else if(strncasecmp(runmode,"mkf",3)==0)  runmodeflag=MKF;
    else if(strncasecmp(runmode,"custom",6)==0)  runmodeflag=CUSTOM;
    else if(strncasecmp(runmode,"copy",4)==0)    runmodeflag=COPY;
    
    else{
        LOG(ERROR)<<"***Wrong mode enteredUse mkf/custom/mkf-refined/custom-refined/copy***";
        return (EXIT_FAILURE);
    }
    if(runmodeflag==MKF || runmodeflag==MKF_REFINED){
        if(PIL_OK!=(status=PILGetFname("mkffile",mkffile))){
            LOG(ERROR)<<"***Error reading mkffile file:"<<mkffile<<"***";
            return status;
        }
        if(PIL_OK!=(status=PILGetFname("thresholdfile",thresholdfile))){
            LOG(ERROR)<<"***Error reading threshold file:"<<thresholdfile<<"***";
            return status;
        }
    } else if (runmodeflag == CUSTOM || runmodeflag == CUSTOM_REFINED) {
        do {
            if (PIL_OK != (status = PILGetString("customgti", customgti))) {
                LOG(ERROR) << "***Error reading custom intervals***";
                return status;
            }
        } while (isBinsValid(customgti));
    }  
    if(runmodeflag==MKF_REFINED || runmodeflag==CUSTOM_REFINED || runmodeflag==COPY){
        if(PIL_OK!=(status=PILGetFname("l2gtifile",l2gtifile))){
            LOG(ERROR)<<"***Error reading gti file:"<<l2gtifile<<"***";
            return status;
        }
    }
     
    if(PIL_OK!=(status=PILGetFname("outfile",outfile))){
        LOG(ERROR)<<"***Error reading outfile:"<<outfile<<"***";
        return status;
    }
    if(PIL_OK!=(status=PILGetString("gtiextname",gtiextname))){
            LOG(ERROR)<<"***Error reading gti extension name***";
            return status;
    }
    if(PIL_OK!=(status=PILGetBool("history",&history))){
        LOG(ERROR)<<"***Error reading history parameter***";
        return status;
    }
    
    if(PIL_OK!=(status=PILGetBool("clobber",&clobber))){
        LOG(ERROR)<<"***Error reading clobber parameter***";
        return status;
    }

    PILClose(status);
    return (EXIT_SUCCESS);
}

int cztgtigen::read(char *runmode,char *mkffile,char *thresholdfile,
        char *customgti,char *gtifile,char *gtiextname,char *outfile,int clobber,int history){
    strcpy(this->runmode,runmode);
    if(strncasecmp(runmode,"mkf",3)==0)  runmodeflag=MKF;
    else if(strncasecmp(runmode,"custom",6)==0)  runmodeflag=CUSTOM;
    else if(strncasecmp(runmode,"mkf-refined",11)==0)  runmodeflag=MKF_REFINED;
    else if(strncasecmp(runmode,"custom-refined",14)==0)  runmodeflag=CUSTOM_REFINED;
    else if(strncasecmp(runmode,"copy",4)==0)   runmodeflag=COPY;
    else{
        LOG(ERROR)<<"***Wrong mode enteredUse mkf/custom/mkf-refined/custom-refined/copy***";
        return (EXIT_FAILURE);
    }
    if(mkffile!=NULL)
        strcpy(this->mkffile,mkffile);
    if(thresholdfile!=NULL)
        strcpy(this->thresholdfile,thresholdfile);
    if(gtifile!=NULL)
        strcpy(this->l2gtifile,gtifile);
    if(customgti!=NULL)
        strcpy(this->customgti,customgti);
    strcpy(this->outfile,outfile);
    strcpy(this->gtiextname,gtiextname);
    this->clobber=clobber;
    this->history=history;
    return (EXIT_SUCCESS);
}

void cztgtigen::display(){
    LOG(INFO)<<"----------------------------------------------------------------------------";
    LOG(INFO)<<"                           CZTGTIGEN PARAMETERS                               ";
    LOG(INFO)<<"----------------------------------------------------------------------------";
    LOG(INFO)<<"Modulename         : "<<modulename;
    LOG(INFO)<<"Run Mode           : "<<runmode;
    LOG(INFO)<<"MKF file           : "<<mkffile;     
    LOG(INFO)<<"MKF Threshold file : "<<thresholdfile;
    LOG(INFO)<<"Level 2 GTI file   : "<<l2gtifile;
    LOG(INFO)<<"GTI extension name : "<<gtiextname;
    LOG(INFO)<<"Custom Intervals   : "<<customgti;
    LOG(INFO)<<"Output gti file    : "<<outfile;       
    LOG(INFO)<<"Clobber            : "<<clobber;
    LOG(INFO)<<"History            : "<<history;
    LOG(INFO)<<"----------------------------------------------------------------------------";
}

int cztgtigen::cztgtigenProcess(){
    if(clobber==YES){
        unlink(outfile);
    }
    else{
        LOG(ERROR)<<""<<outfile<<"  already exists";
        LOG(ERROR)<<" Use the option 'clobber=yes' to overwrite the file";
        return (EXIT_FAILURE);
    }
    
    vector<GTI> gti; 
    vector<GTI> l2gti;
    gti.clear();
    l2gti.clear();
    fitsfile *fgti,*fout;
    int status=0;
    
    if(runmodeflag==COPY){
        fits_open_file(&fgti,l2gtifile,READONLY,&status); 
        if(status!=0) { 
            fits_report_error(stderr,status);
            LOG(ERROR)<<"***Error opening file "<<l2gtifile<<"***";
            return status;}
        fits_create_file(&fout,outfile,&status);   
        if(status!=0) { fits_report_error(stderr,status);  
                LOG(ERROR)<<"***Error creating output file "<<outfile<<"***";
                return status;}
        fits_copy_file(fgti,fout,1,1,1,&status); 
        if(status!=0) { fits_report_error(stderr,status);  return status;}
        fits_close_file(fgti,&status);  
        if(status!=0) { fits_report_error(stderr,status);  return status;}
        fits_close_file(fout,&status);  
        if(status!=0) { fits_report_error(stderr,status);  return status;}
    }
    else if(runmodeflag==MKF || runmodeflag==MKF_REFINED){
        GTI objgti;
        gti.clear();
        Mkf mkf;
        vector <double> tstart, tstop;
        tstart.clear(); tstop.clear();
        if(mkf.read_mkf_file((string) mkffile)){
            LOG(ERROR) << "Error in reading mkf file: " << mkffile;
            return EXIT_FAILURE;
        }
        if(mkf.get_filtered_gti(thresholdfile, tstart, tstop)){
            LOG(ERROR) << "Error in generating mkf filtered gti";
            return EXIT_FAILURE;
        }
        for(int itime=0; itime<tstart.size(); itime++){
            objgti.tstart= tstart[itime];
            objgti.tstop= tstop[itime];
            gti.push_back(objgti);
        }
        LOG(INFO) << "Completed GTI creation based on mkf";
        
    }
    else if(runmodeflag==CUSTOM || runmodeflag==CUSTOM_REFINED){
        int ntbins=0;          //number of time bins
        ntbins=get_nbins(customgti);
        double *start,*stop;
        start=new double[ntbins];
        stop=new double[ntbins];
        if(start==NULL || stop==NULL){
            LOG(ERROR)<<"***Out of memory error***";
            return (EXIT_FAILURE);
        }
        getbins(customgti,start,stop,ntbins);
        GTI objgti;
        gti.clear();
        for(int i=0;i<ntbins;i++){
            if(start[i]>stop[i]) { LOG(ERROR)<<"***Input GTI is Invalid***";  return (EXIT_FAILURE); }
            if(i<ntbins-1){
                if(stop[i]>start[i+1]) 
                { LOG(ERROR)<<"***Overlapping intervals in input found***";  return (EXIT_FAILURE);  }
            }
            objgti.tstart=start[i];  objgti.tstop=stop[i];
            gti.push_back(objgti);
        }
        delete[] start,stop;
        LOG(INFO)<<"Completed GTI creation based on custom intervals";
    }
    if(runmodeflag==MKF_REFINED || runmodeflag==CUSTOM_REFINED){
        LOG(INFO)<<"Refining based on input level2 GTI....";
        int numhdu=0;
        fits_open_file(&fgti,l2gtifile,READONLY,&status);  if(status!=0) { fits_report_error(stderr,status);  return status;}
        fits_get_num_hdus(fgti,&numhdu,&status);               if(status!=0) { fits_report_error(stderr,status);  return status;}
        if(numhdu<2){ 
            LOG(ERROR)<<"***Expected at least 2 extensions in input level2 GTI file***";
            return (EXIT_FAILURE);
        }
        int hdutype;
        char extname[FLEN_VALUE];
        fits_movnam_hdu(fgti,BINARY_TBL,gtiextname,0,&status);
        if(status!=0) { fits_report_error(stderr,status);  return status;}
        int hdunum;
        fits_get_hdu_num(fgti,&hdunum);
        if(readGTI(l2gtifile,hdunum,l2gti)){
            LOG(ERROR)<<"***Error in reading level2 gti file***";
            return (EXIT_FAILURE);
        }
        fits_close_file(fgti,&status);
        if(status!=0) { fits_report_error(stderr,status);  return status;}
        if(refineGTI(gti,l2gti)){
            LOG(ERROR)<<"***Error in refining GTI with level2 gti***";
            return (EXIT_FAILURE);
        } 
        LOG(INFO)<<"Completed GTI refinement based on input GTI";
      }
    if(runmodeflag!=COPY){
        if(createGTIfile(outfile,gtiextname)){
            LOG(ERROR)<<"***Error in creating GTI file***";
            return (EXIT_FAILURE);
        } 
        if(writeGTI(outfile,2,gti)){
            LOG(ERROR)<<"***Error in writing GTI***";
            return (EXIT_FAILURE);
        }
    }
      updateKeywords(outfile,modulename);
      if(history==YES){
          vector<string> vhistory;
          getHistory(vhistory);
          if(writeHistory(outfile,vhistory)){
              LOG(ERROR)<<"Error writing history";
              return (EXIT_FAILURE);
          }
      }
    return (EXIT_SUCCESS);
}


int createGTIfile(char *file,char *gtiextname){
    fitsfile *fptr;
    int status=0;
    fits_create_file(&fptr,file,&status);
    if(status)  { LOG(ERROR)<<"***Error in creation of GTI file***";  fits_report_error(stderr,status); return (EXIT_FAILURE);  }
    char *ttype[]={"TSTART","TSTOP"};
    char *tform[]={"D","D"};
    char *tunit[]={"s","s"};
    int tfields=2,qid;
    fits_create_tbl(fptr,BINARY_TBL,0,tfields,ttype,tform,tunit,gtiextname,&status);
    if(status)  { LOG(ERROR)<<"***Error in table creation for GTI file***";  fits_report_error(stderr,status); return (EXIT_FAILURE);  }
    fits_close_file(fptr,&status);
    if(status)  { LOG(ERROR)<<"***Error in creation of GTI file***";  fits_report_error(stderr,status); return (EXIT_FAILURE);  }
    return (EXIT_SUCCESS);
}


int writeGTI(char *infile,int extnum,vector<GTI> &gti){
    fitsfile *fptr;
    int status=0;
    fits_open_file(&fptr,infile,READWRITE,&status); if(status)  return status;
    int hdutype=0;
    fits_movabs_hdu(fptr,extnum,&hdutype,&status);  if(status)  return status;
    if(hdutype!=BINARY_TBL){
        LOG(ERROR)<<"Expected Binary Table HDU for writing GTI at HDU no "<<extnum;
        return (EXIT_FAILURE);
    }
    long nrows;
    fits_get_num_rows(fptr,&nrows,&status);
     if(status)  return status;
    if(nrows!=0){
        fits_delete_rows(fptr,1,nrows,&status);  if(status)  return status;
    }
    for(long i=1;i<=gti.size();i++){
        fits_write_col(fptr,TDOUBLE,1,i,1,1,&gti[i-1].tstart,&status); 
        if(status!=0)  return status;
        fits_write_col(fptr,TDOUBLE,2,i,1,1,&gti[i-1].tstop,&status); 
        if(status!=0)  return status; 
    }
    fits_close_file(fptr,&status);
    if(status)  return status;
    return (EXIT_SUCCESS);
}

int readGTI(char *infile,int extnum,vector<GTI> &vectorGTI){
    fitsfile *fptr;
    int status=0;
    fits_open_file(&fptr,infile,READONLY,&status); if(status) { fits_report_error(stderr,status);  return status;}
    fits_movabs_hdu(fptr,extnum,NULL,&status);  if(status) { fits_report_error(stderr,status);  return status;}
    long nrows;
    GTI gti;
    fits_get_num_rows(fptr,&nrows,&status); if(status) { fits_report_error(stderr,status);  return status;}
    for(long i=1;i<=nrows;i++){
        fits_read_col(fptr,TDOUBLE,1,i,1,1,NULL,&gti.tstart,NULL,&status);
       if(status) { fits_report_error(stderr,status);  return status;}
        fits_read_col(fptr,TDOUBLE,2,i,1,1,NULL,&gti.tstop,NULL,&status);
       if(status) { fits_report_error(stderr,status);  return status;}
        vectorGTI.push_back(gti);
    }
    fits_close_file(fptr,&status);
    if(status)  { fits_report_error(stderr,status);  return status;  }
    return (EXIT_SUCCESS);
}

int refineGTI(vector<GTI> &gti2refine,vector<GTI> &ingti){
    int s1=gti2refine.size();
    int s2=ingti.size();
    if(s1<=0){
        LOG(ERROR)<<"Number of intervals are zero/less than zero in GTI";
        return (EXIT_FAILURE);
    }
    if(s2<=0){
        LOG(INFO)<<"Number of intervals in level2 GTI is zero";
        return (EXIT_SUCCESS);
    }
    int i,j;
   
    vector<GTI> outgti;
    
    GTI objgti;
    
    for(i=0;i<s1;i++){
        for(j=0;j<s2;j++){
            if(gti2refine[i].tstart>=ingti[j].tstart && gti2refine[i].tstop<=ingti[j].tstop)
                objgti.tstart=gti2refine[i].tstart;  
            //cout<<gti2refine[i].tstart<<" "<<ingti[i].tstart;
            if(gti2refine[i].tstart>ingti[j].tstart)
                objgti.tstart=gti2refine[i].tstart;
            else
                objgti.tstart=ingti[j].tstart;
            //cout<<gti2refine[i].tstop<<" "<<ingti[i].tstop;
            if(gti2refine[i].tstop<ingti[j].tstop)
                objgti.tstop=gti2refine[i].tstop;
            else
                objgti.tstop=ingti[j].tstop;
            outgti.push_back(objgti);
            //cout<<objgti.tstart<<" - "<<objgti.tstop;
            
        }
    }
    
    gti2refine.clear();
    vector<GTI>::iterator iter=outgti.begin();
    while(iter!=outgti.end()){
        if((*iter).tstart<=(*iter).tstop){
            gti2refine.push_back((*iter));
            LOG(INFO)<<(*iter).tstart<<"-"<<(*iter).tstop;
        }
        iter++;    
    }
   return (EXIT_SUCCESS);
}  

void cztgtigen::getHistory(vector<string> &vhistory){
    //char *user=getlogin();
    char *user = getenv("USER");
	string str="Module run by "+(string)user;
    vhistory.push_back(str);
    vhistory.push_back("Parameter List START for "+(string)modulename);
    vhistory.push_back("P1 runmode="+(string)runmode);
    vhistory.push_back("P2 mkffile="+(string)mkffile);
    vhistory.push_back("P3 lbt_thresholdfile="+(string)thresholdfile);
    vhistory.push_back("P4 customgti="+(string)customgti);
    vhistory.push_back("P5 l2gtifile="+(string)l2gtifile);
    vhistory.push_back("P6 gtiextname="+(string)gtiextname);
    vhistory.push_back("P7 outfile="+(string)outfile);
    if(clobber==YES) 
        vhistory.push_back("P8 clobber=yes");
    else
        vhistory.push_back("P8 clobber=no");
    if(history==YES)
        vhistory.push_back("P9 history=yes");
    else
        vhistory.push_back("P9 history=no");
    vhistory.push_back("Parameter List END");
}



