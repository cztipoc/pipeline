

#include"cztdpigen.h"

using namespace std;

cztdpigen::cztdpigen(){
    strcpy(modulename,"cztdpigen_v");
    strcat(modulename,VERSION);
    strcpy(quad_no,"");
    nebins=0;    //will be set in cztdpigen_process
    emin=NULL;  //will be set in cztdpigen_process
    emax=NULL;  //will be set in cztdpigen_process
    ntbins=0;
    ndpi=0;
    tstart=NULL;
    tstop=NULL;
}

int cztdpigen::read(int argc,char **argv){
    int status=0;
   
    if(PIL_OK!=(status=PILInit(argc,argv))){
        LOG(ERROR)<<"***Error Initializing PIL***";
        return status;
    }
    
    if(PIL_OK!=(status=PILGetFname("infile",infile))){
        LOG(ERROR)<<"***Error Reading input file:"<<infile<<"***";
        return status;
    }
    
    if(PIL_OK!=(status=PILGetFname("qefile",qefile))){
        LOG(ERROR)<<"***Error Reading QE file:"<<qefile<<"***";
        return status;
    }
    
    if (PIL_OK != (status = PILGetFname("outfile", outfile))) {
        LOG(ERROR) << "***Error Reading outfile:" << outfile << "***";
        return status;
    }

    do {
        if (PIL_OK != (status = PILGetString("quad_no", quad_no))) {
            LOG(ERROR) << "***Error reading quadrant:" << quad_no << "***";
            return status;
        }
    } while (isQuadrantNumbersValid(quad_no));


    do {

        if (PIL_OK != (status = PILGetString("ebins", ebins))) {
            LOG(ERROR) << "***Error Reading ebins:" << ebins << "***";
            return status;
        }
    } while (isBinsValid(ebins));

    do {
        if (PIL_OK != (status = PILGetString("timerange", timerange))) {
            LOG(ERROR) << "***Error Reading timerange***";
            return status;
        }
    } while (isBinsValid(timerange));
    
    if(PIL_OK!=(status=PILGetString("tcol",tcol))){
        LOG(ERROR)<<"***Error reading time column***";
        return status;
    } 
     
    if(PIL_OK!=(status=PILGetString("ecol",ecol))){
        LOG(ERROR)<<"***Error reading energy column***";
        return status;
    } 
    
    if(PIL_OK!=(status=PILGetString("xcol",xcol))){
        LOG(ERROR)<<"***Error reading X column***";
        return status;
    } 
    
    if(PIL_OK!=(status=PILGetString("ycol",ycol))){
        LOG(ERROR)<<"***Error reading Y column***";
        return status;
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

int cztdpigen::read(char* infile,char *qefile, char* outfile, char *quad_no, char* ebins, char *timerange,
        char* ecol, char* tcol, char* xcol, char* ycol, int clobber, int history){
    strcpy(this->infile,infile);
    strcpy(this->qefile,qefile);
    strcpy(this->outfile,outfile);
    strcpy(this->quad_no, quad_no);
    strcpy(this->ebins,ebins);
    strcpy(this->timerange,timerange);
    strcpy(this->ecol,ecol);
    strcpy(this->tcol,tcol);
    strcpy(this->xcol,xcol);
    strcpy(this->ycol,ycol);
    this->clobber=clobber;
    this->history=history;
    return (EXIT_SUCCESS);
}

void cztdpigen::display(){
    LOG(INFO)<<"----------------------------------------------------------------------------";
    LOG(INFO)<<"                          CZTDPIGEN PARAMETERS                            ";
    LOG(INFO)<<"----------------------------------------------------------------------------";
     LOG(INFO)<<"Input event file                           :"<<infile;
     LOG(INFO)<<"output file                                :"<<outfile;
     LOG(INFO)<<"QE file name                               :"<<qefile;
     LOG(INFO)<<"Quadrants to be processed                  :"<<quad_no;
     LOG(INFO)<<"Time Range                                 :"<<timerange;
     LOG(INFO)<<"Energy Bins                                :"<<ebins;
     LOG(INFO)<<"Energy column name                         :"<<ecol;
     LOG(INFO)<<"Time column name                           :"<<tcol;
     LOG(INFO)<<"X column name                              :"<<xcol;
     LOG(INFO)<<"Y column name                              :"<<ycol;
     LOG(INFO)<<"Clobber                                    :"<<clobber;
     LOG(INFO)<<"History                                    :"<<history;
    LOG(INFO)<<"----------------------------------------------------------------------------";
}
    
int cztdpigen::cztdpigenProcess(){
       
    int status=0; //status variable
    int i=0,j=0,k=0; //counter variables
    fitsfile *fptr; //pointer to input event file
    fitsfile *fdpi; //pointer to output dpi file
    
    long nrows=0;
    int quadid=0; 
    long** dpi;
    long* fulldpi;
    int outputfileindex=0; 
    vector<string> history_str;
    char extname[FLEN_VALUE];
    int extname_n=0;
    
    
    
    fits_open_file(&fptr,infile,READONLY,&status);
    if(status){
        fits_report_error(stderr,status);
        LOG(ERROR)<<"Error in opening file:"<<infile;
        return (EXIT_FAILURE);
    }
    if(!isEventFileValid(fptr)){             //checking validity of event file
        LOG(ERROR)<<"***Input file "<<infile<<" is not in proper format***";
        return (EXIT_FAILURE);
    }
    //-----------finding number of DPIs to be created----------
    nebins=get_nbins(ebins); //calculates the numbers as given by user in energy bins
    ntbins=get_nbins(timerange);
    ndpi=nebins*ntbins;
    if(ndpi<=0) { 
        LOG(ERROR)<<"Number of DPI to be generated is zero/less than zero";
        return (EXIT_FAILURE);
    }
    
    //---------getting time and energy ranges--------------
    tstart=new double[ntbins];
    tstop=new double[ntbins];
    emin=new double[nebins];
    emax=new double[nebins];
    if(tstart==NULL || tstop==NULL || emin==NULL || emax==NULL){
         LOG(ERROR)<<"Out of memory error - getTime()"; 
         return (EXIT_FAILURE); 
    }
    if(getTime(fptr,timerange,ntbins,tstart,tstop,tcol)){
        LOG(ERROR)<<"***Error in preparing time bins***";
        return (EXIT_FAILURE);
    }
    if(getEnergy(ebins,ecol,nebins,emin,emax)){
        LOG(ERROR)<<"***Error in preparing energy bins***";
        return (EXIT_FAILURE);
    }
    //printing time and energy ranges
    LOG(INFO)<<"Time Bins-";
    for(i=0;i<ntbins;i++)
        LOG(INFO)<<"\t"<<tstart[i]<<"-"<<tstop[i];
    LOG(INFO)<<"Energy Bins-";
    for(i=0;i<nebins;i++)
        LOG(INFO)<<"\t"<<emin[i]<<"-"<<emax[i];  
    
    //-----------generating output filename---------
    vector<string> outputfiles;
    status=getNoutputfiles(ndpi,outfile,outputfiles);
    if(status!=0) { 
        LOG(ERROR)<<"Error in getting output filenames for dpi";
        return (EXIT_FAILURE);
    }
   
    //----------printing output file to be generated
    LOG(INFO)<<"-----Output Files to be generated----";
    for(i=0;i<outputfiles.size();i++){
        LOG(INFO)<<i+1<<". "<<outputfiles[i].c_str();
    }
    LOG(INFO);
    //----------------------------------------------
    //------------checking whether output files exist-------
    for(i=0;i<outputfiles.size();i++){
        if(FileExists((char *)outputfiles[i].c_str())){
            if(clobber==YES){
                    unlink((char *)outputfiles[i].c_str());
            }
            else{
                LOG(ERROR)<<""<<outfile<<" already exists";
                LOG(ERROR)<<"Use clobber=yes for overwriting the file";
                return (EXIT_FAILURE);
            }
        }
    }
    
    /***************************************************************************************************
    * following is the code to generate quadrant integer array from string supplied by user 
    ***************************************************************************************************/
    
    int* quadArray;
    int noccurrence;
    stringFinder(quad_no, ",", 0, &noccurrence);
    int nelems_quad = noccurrence +1 ;
    quadArray = new int[nelems_quad];
    
    if(csvStrToInt(quad_no, ",", quadArray, &nelems_quad)){
      LOG(ERROR)<<"***Error in converting quadrant string array into integer array***";
    }
    
    //----------------------------------------------
     //creating empty output DPI files
  status=createOutputFiles4DPI(fptr,outputfiles);
    if(status){
        LOG(ERROR)<<"***Error in creating output DPI files***";
        return (EXIT_FAILURE);
    }
    
    LOG(INFO)<<"output files for dpi created.";
    //------------------------------------------------------   
    
    //reading qefile
    float qearray[XSIZE*YSIZE];
    if(readQE(qefile,qearray)){
        LOG(ERROR)<<"***Error reading Quantum Efficiency ***";
        return (EXIT_FAILURE);
    }
    
    LOG(INFO)<< "Quantum efficiency file has been read.";
    
    //to generate DPI from event file

    fulldpi = new long[XSIZE*YSIZE];
    dpi = allocateMemory<long>(NUMQUAD, XPIX_QUAD*YPIX_QUAD); 
    if (dpi == NULL || fulldpi==NULL) {
        LOG(ERROR) << "Out of memory error";
        return (EXIT_FAILURE);
    }

    for(i=0; i<NUMQUAD; i++){
        for(j=0; j< XPIX_QUAD*YPIX_QUAD; j++){
            dpi[i][j]=0;
        }
    }

    for(i=0; i<XSIZE*YSIZE; i++){
        fulldpi[i]=0;
    }
    //---------generating DPIs and writing to files------------
    // status=0;

    for(i=0;i<ntbins;i++){
        for(j=0;j<nebins;j++){
            //opening output fits files
            fits_open_file(&fdpi,(char *)outputfiles[outputfileindex].c_str(),READWRITE,&status);
            if(status){
                fits_report_error(stderr,status);
                LOG(ERROR)<<"***Error opening file "<<outputfiles[outputfileindex]<<"***";
                return (EXIT_FAILURE);
            }
            LOG(INFO)<<"Writing DPI to file "<<outputfiles[outputfileindex];
            for(k=0;k<nelems_quad;k++){  
                LOG(INFO)<<"HDU "<<k;
                quadid=quadArray[k];
                extname_n=quadid+2; //HDU number of respective quadrant in event file
                quadToHDU(quadid, extname);

                status = getDPI(fptr, extname, emin[j], emax[j], tstart[i], tstop[i], ecol, tcol,
                        xcol, ycol, dpi[quadid]);

                 if(status){
                       LOG(ERROR)<<"Error in dpi generation for "<<emin[j]<<"-"<<
                             emax[j]<<" and "<<tstart[i]<<"-"<<tstop[i];
                     continue;
                 }
                LOG(INFO)<< "DPI "<< k+1 << " generated but not stored.";
                 status=0;
                 DPI objdpi(dpi[quadid],emin[j],emax[j],tstart[i],tstop[i],
                         quadid,ecol,XPIX_QUAD*YPIX_QUAD);
                 
               /*  LOG(INFO) << "---------------------------------------------------";
                for (int i = 0; i < objdpi.dpisize; i++) {

                    if (objdpi.pixels[i] > 0)
                        cout << objdpi.pixels[i] << "---";


                }
                LOG(INFO) << "---------------------------------------------------";       */          
                 //cout<<"Writing DPI for HDU "<<k;
                 if(writeDPI(fdpi,quadid+2,objdpi)){
                     LOG(ERROR)<<"Error in writing DPI";
                     return (EXIT_FAILURE);
                 }
                 
             }
            
            makeFullImage(dpi[0],dpi[1],dpi[2],dpi[3],fulldpi);
            DPI objdpi(fulldpi,emin[j],emax[j],tstart[i],tstop[i],
                         quadid,ecol,XSIZE*YSIZE);
            //cout<<"Writing DPI for HDU "<<k;
            if(writeDPI(fdpi,6,objdpi)){
                LOG(ERROR)<<"Error in writing DPI";
                return (EXIT_FAILURE);
            }
            status=0;
            fits_close_file(fdpi,&status);
            if(status){
                LOG(ERROR)<<"***Error closing file "<<outputfiles[outputfileindex]<<"***";
                return (EXIT_FAILURE);
            }
            outputfileindex++;
        }
    }
    delete[] tstart,tstop,emin,emax,quadArray;
    fits_close_file(fptr,&status);
    if(status){
        LOG(ERROR)<<"***Error closing input file***";
        return (EXIT_FAILURE);
    }
    
    //updating keywords and writing history
    char filename[FLEN_FILENAME];
    for(int i=0;i<outputfiles.size();i++){
        strcpy(filename,outputfiles[i].c_str());
        updateKeywords(filename,modulename);
        if(history==YES){
            vector<string> vhistory;
            getHistory(vhistory);
            writeHistory(filename,vhistory);
        }
    }
    
    return (EXIT_SUCCESS);
}
   
    
int getNoutputfiles(int ndpi,char *outfile,vector<string> &outputfiles){
    LOG(INFO)<<"NDPI:"<<ndpi;
    outputfiles.clear();    
    int status=0;
    char filename[FLEN_FILENAME];
    if(ndpi<=0) { return (EXIT_FAILURE); }
    else if(ndpi==1){
        outputfiles.push_back((string(outfile)));
    }
    else {
        for(int i=0;i<ndpi;i++){
            sprintf(filename,"%s_%d",outfile,i);
            outputfiles.push_back((string)filename);
        }
    }
    
    return (EXIT_SUCCESS);
}

//to create empty output file for DPI
int createOutputFiles4DPI(fitsfile *fin,vector<string> &outputfiles){
    int status=0;
    fits_movabs_hdu(fin,1,NULL,&status); //moving to first HDU for copying keywords
    if(status){
        fits_report_error(stderr,status);
        return (EXIT_FAILURE);
    }
    fitsfile *fptr;
    int bitpix, naxis;
    long naxes[2];
    for(int i=0;i<outputfiles.size();i++){
        fits_create_file(&fptr,(char *)outputfiles[i].c_str(),&status);   //creating new file
        if(status){
            fits_report_error(stderr,status);
            return (EXIT_FAILURE);
        }
        //for primary header
        bitpix=BYTE_IMG;
        naxis=0;
        naxes[0]=0;     naxes[1]=0;
        fits_create_img(fptr,bitpix,naxis,naxes,&status);  //empty primary HDU
        if(status){
            fits_report_error(stderr,status);
            return (EXIT_FAILURE);
        }
        
        if(copyUserKeyWords(fin,fptr)){                   //copying keywords from input file
            LOG(ERROR)<<"***Error in copying keywords***";
        }
        char extname[FLEN_VALUE];
        for(int j=0,k=EVENTDATA_HDU_BEGIN;j<NUMQUAD;j++,k++){                  //creating image for each quadrant
            bitpix=LONG_IMG;
            naxis=2;
            naxes[0]=XPIX_QUAD;   naxes[1]=YPIX_QUAD;
            fits_create_img(fptr,bitpix,naxis,naxes,&status);
            if(status){
                fits_report_error(stderr,status);
                return (EXIT_FAILURE);
            }
            sprintf(extname,"Q%d_DPI",j);
            fits_update_key(fptr,TSTRING,"EXTNAME",extname,NULL,&status);
            if(status){
                LOG(ERROR)<<"***Error updating EXTNAME***";
                fits_report_error(stderr,status);
                return (EXIT_FAILURE);
            }
           
            fits_movabs_hdu(fin,k,NULL,&status);
            if(status){
                fits_report_error(stderr,status);
                return (EXIT_FAILURE);
            }
            if(copyUserKeyWords(fin,fptr)){                   //copying keywords from input file
                LOG(ERROR)<<"***Error in copying keywords***";
                return (EXIT_FAILURE);
            }
        }
        //creating hdu for full dpi
        naxes[0]=XSIZE;   naxes[1]=YSIZE;
        fits_create_img(fptr,bitpix,naxis,naxes,&status);
        if(status){
            fits_report_error(stderr,status);
            return (EXIT_FAILURE);
        }
        sprintf(extname,"FULLDPI");
        fits_update_key(fptr,TSTRING,"EXTNAME",extname,NULL,&status);
        if(status){
            LOG(ERROR)<<"***Error updating EXTNAME***";
            fits_report_error(stderr,status);
            return (EXIT_FAILURE);
        }
        if(copyUserKeyWords(fin,fptr)){                   //copying keywords from input file
            LOG(ERROR)<<"***Error in copying keywords***";
            return (EXIT_FAILURE);
        }
        fits_delete_key(fptr,"QUADID",&status);
        if(status){
            fits_report_error(stderr,status);
            return (EXIT_FAILURE);
        }
        
        fits_close_file(fptr,&status);
        if(status){
            fits_report_error(stderr,status);
            return (EXIT_FAILURE);
        }
    }
    return (EXIT_SUCCESS);
}

int writeDPI(fitsfile *fptr,int hdu,DPI &dpi){
    int status=0;
    long totcounts=0;
    long fpixel[2];
    LOG(INFO)<<"DPI to be written in HDU NUMBER "<< hdu;
    
    fits_movabs_hdu(fptr,hdu,NULL,&status);
    if(status){
        fits_report_error(stderr,status);
        return (EXIT_FAILURE);
    }
    LOG(INFO)<< "moved to HDU "<< hdu;

    for(int i=0;i<dpi.dpisize;i++)
            totcounts=totcounts+dpi.pixels[i];
    
     //---adding required keywords---
    fits_update_key(fptr,TDOUBLE,"EMIN",&dpi.emin,NULL,&status);
    fits_update_key(fptr,TDOUBLE,"EMAX",&dpi.emax,NULL,&status);
    fits_update_key(fptr,TDOUBLE,"TSTART",&dpi.tstart,NULL,&status);
    fits_update_key(fptr,TDOUBLE,"TSTOP",&dpi.tstop,NULL,&status);
    //cout<<""<<dpi.tstop;
    fits_update_key(fptr,TSTRING,"ECOL",dpi.ecol,NULL,&status);
    fits_write_key(fptr,TLONG,"TOTCOUNT",&totcounts,NULL,&status);
    //----writing data---

    fpixel[0]=1;
    fpixel[1]=1;
    fits_write_pix(fptr,TLONG,fpixel,dpi.dpisize,dpi.pixels,&status);
    LOG(INFO) << " writing dpi with dpisize :" << dpi.dpisize;
    
    if(status){
        fits_report_error(stderr,status);
        return (EXIT_FAILURE);
    }
    return (EXIT_SUCCESS);
}

int getTime(fitsfile *fptr,char *timerange,int ntbins,double *tstart,double *tstop,char *tcol){
    int status=0;
    LOG(INFO)<<"Time Range:"<<timerange;
    if(timerange==NULL){
        LOG(ERROR)<<"Time Range value not correct - getEventTime";
        return (EXIT_FAILURE);
    }
    else if(strcmp(timerange,"-")==0){   //get time from file
        vector<double> vtstart;   //stores tstart for four quadrants which contains data
        vector<double> vtstop;    //stores tstop for four quadrants which contains data
        long nrows;
        for(int i=EVENTDATA_HDU_BEGIN;i<=EVENTDATA_HDU_END;i++){
            fits_movabs_hdu(fptr,i,NULL,&status);
            if(status){
                LOG(ERROR)<<"Error in moving to HDU "<<i<<" in input event file";
                fits_report_error(stderr,status);
                return (EXIT_FAILURE);
            }
            fits_get_num_rows(fptr,&nrows,&status);
            if(status){
                LOG(ERROR)<<"Error getting number of rows in event file";
                fits_report_error(stderr,status);
                return (EXIT_FAILURE);
            }
            if(nrows<=0) continue;
            else{               //if event data is there
                int colnum=0;
                fits_get_colnum(fptr,CASEINSEN,(char *)tcol,&colnum,&status);
                if(status && status!=COL_NOT_UNIQUE){
                    LOG(ERROR)<<"***Error in finding time column - getTime()***";
                    fits_report_error(stderr,status);
                    return (EXIT_FAILURE);
                }
                long firstrow=1,firstelem=1;
                double *time=new double[nrows];
                double starttime,stoptime;
                if(time==NULL){ 
                    LOG(ERROR)<<"Out of memory error - getTime()"; 
                    return (EXIT_FAILURE); 
                }
                fits_read_col(fptr,TDOUBLE,colnum,firstrow,firstelem,nrows,
                        NULL,time,NULL,&status);
                if(status){
                    LOG(ERROR)<<"Error in reading column "<<colnum<<" in event file";
                    fits_report_error(stderr,status);
                    return (EXIT_FAILURE);
                }
                min<double>(time,nrows,&starttime);
                max<double>(time,nrows,&stoptime);
                LOG(INFO)<<"Start Time :"<<starttime;
                LOG(INFO)<<"Stop Time: "<<stoptime;
                vtstart.push_back(starttime);
                vtstop.push_back(stoptime);
                delete[] time;
               }
        }
        if(vtstart.size()<=0)  { LOG(ERROR)<<"Data not found in event file"; return (EXIT_FAILURE); }
        else if(vtstart.size()==1) tstart[0]=vtstart[0]; 
        else min(vtstart.data(),vtstart.size(),&tstart[0]);
        
        if(vtstop.size()<=0)   { LOG(ERROR)<<"Data not found in event file"; return (EXIT_FAILURE); }
        else if(vtstop.size()==1) tstop[0]=vtstop[0]; 
        else max(vtstop.data(),vtstop.size(),&tstop[0]);
    }
    else{
         getbins(timerange,tstart,tstop,ntbins);
     }
     return (EXIT_SUCCESS);
}

int getEnergy(char *ebins,char *ecol,int nebins,double *emin,double *emax){
    fits_uppercase(ecol);
    if(strcmp(ebins,"-")==0){
        if(strcmp(ecol,"PHA")==0 || strcmp(ecol,"PI")==0){
            emin[0]=MINCHANNEL; emax[0]=MAXCHANNEL;
        }
        else if(strcmp(ecol,"ENERGY")==0){
             emin[0]=EMIN; emax[0]=EMAX;
        }
        else{
            LOG(ERROR)<<"***Invalid ecol value***  Valid values are PHA/PI/ENERGY";
            return (EXIT_FAILURE);
        }
    }
    else{                    //if some ranges are provided
        getbins(ebins,emin,emax,nebins);
    }     
    return (EXIT_SUCCESS);
}

int cztdpigen::getHistory(vector<string> &vhistory){
    //char *user=getlogin();
    char *user = getenv("USER");
	string str="Module run by "+(string)user;
    vhistory.push_back(str);
    vhistory.push_back("Parameter List START for "+(string)modulename);
    vhistory.push_back("P1 infile="+(string)infile);
    vhistory.push_back("P2 qefile="+(string)qefile);
    vhistory.push_back("P3 outfile="+(string)outfile);
    vhistory.push_back("P4 quad_no="+(string)quad_no);
    vhistory.push_back("P5 ebins="+(string)ebins);
    vhistory.push_back("P6 timerange="+(string)timerange);
    vhistory.push_back("P7 ecol="+(string)ecol);
    vhistory.push_back("P8 tcol="+(string)tcol);
    vhistory.push_back("P9 xcol="+(string)xcol);
    vhistory.push_back("P10 ycol="+(string)ycol);
    if(clobber==YES) 
        vhistory.push_back("P11 clobber=yes");
    else
        vhistory.push_back("P11 clobber=no");
    if(history==YES)
        vhistory.push_back("P12 history=yes");
    else
        vhistory.push_back("P12 history=no");
    vhistory.push_back("Parameter List END");
    return (EXIT_SUCCESS);
}

