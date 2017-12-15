
#include"cztpha2energy.h"


cztpha2energy::cztpha2energy() {
    strcpy(modulename, "cztpha2energy_v");
    strcat(modulename, VERSION);
}

void cztpha2energy::display() {
    LOG(INFO) << "----------------------------------------------------------------------------";
    LOG(INFO) << "                           CZTPHA2ENERGY PARAMETERS                                      ";
    LOG(INFO) << "----------------------------------------------------------------------------";
    LOG(INFO)  << "Input Event File      : " << InputEvtFile; //input event file
    LOG(INFO)  << "Gain File             : " << GainFile;
    LOG(INFO)  << "Offset File           : " << OffsetFile;
    LOG(INFO)  << "Output Event File     : " << OutputEvtFile;
    LOG(INFO)  << "clobber               : " << clobber;
    LOG(INFO)  << "history               : " << history;
    LOG(INFO) << "----------------------------------------------------------------------------";

}

int cztpha2energy::read(int argc, char **argv) {
    int status = 0;
    if (PIL_OK != (status = PILInit(argc, argv))) {
        LOG(ERROR) << "Error Initializing PIL";
        return (EXIT_FAILURE);
    }
    if (PIL_OK != (status = PILGetFname("InputEvtFile", InputEvtFile))) {
        LOG(ERROR) << "Error Reading Input Event file";
        return (EXIT_FAILURE);
    }
    if (PIL_OK != (status = PILGetFname("GainFile", GainFile))) {
        LOG(ERROR) << "Error in reading Reading Gain Offset File";
        return (EXIT_FAILURE);
    }
    
    if (PIL_OK != (status = PILGetFname("OffsetFile", OffsetFile))) {
        LOG(ERROR) << "Error in reading Offset File";
    }
////////////    if (PIL_OK != (status = PILGetFname("rmfile", RMF))) {
////////////        LOG(ERROR) << "Error Reading Response Matrix File";
////////////        return (EXIT_FAILURE);
////////////    }
    if (PIL_OK != (status = PILGetFname("OutputEvtFile", OutputEvtFile))) {
        LOG(ERROR) << "Error Reading Output File";
        return (EXIT_FAILURE);
    }
    if (PIL_OK != (status = PILGetBool("clobber", &clobber))) {
        return (EXIT_FAILURE);
    }
    if (PIL_OK != (status = PILGetBool("history", &history))) {
        return (EXIT_FAILURE);
    }
    PILClose(status);
    return (EXIT_SUCCESS);
}

int cztpha2energy::read(char* InputEvtFile, char* GainFile, char* OutFile, char* outfile, int clobber, int history){
    strcpy(this->InputEvtFile, InputEvtFile);
    strcpy(this->GainFile, GainFile);
    strcpy(this->OffsetFile, OffsetFile);
    strcpy(this->OutputEvtFile,OutputEvtFile);
    this->clobber = clobber;
    this->history = history;
    return (EXIT_SUCCESS);
}

int cztpha2energy::cztpha2energyProcess() {
    if(FileExists(OutputEvtFile)){
        if(clobber==YES){
                if(unlink(OutputEvtFile)!=0){
                    //cerr<<"Error in deleting "<<OutputEvtFile; return -1;
                }
        }
        else{
            LOG(INFO)<<""<<OutputEvtFile<<" already exists";
            LOG(INFO)<<"Use clobber=yes for overwriting the file";
            return (EXIT_FAILURE);
        }
    }
    
    fitsfile *fptr,*fptr_in;               //fptr for output file
    int status = 0, flag = 0, i = 0;

    //Opening event file
    //fits_open_file(&fptr_in, InputEvtFile, READWRITE, &status);   //commented by mayuri
    fits_open_file(&fptr_in, InputEvtFile, READONLY, &status);
    
    if(status){
        fits_report_error(stderr,status);
        LOG(ERROR)<<"***Error in opening file "<<InputEvtFile<<"***";
        return(EXIT_FAILURE);
    } 
   
    int hdutype=0, colnum, numofhdu, curhdu;
    //long nrows[4];        //row numbers for four quadrant events

    fits_get_num_hdus(fptr_in, &numofhdu, &status);
    if(status){
        fits_report_error(stderr,status);
        LOG(ERROR)<<"***Error in getting number of HDUs in event file***";
        return(EXIT_FAILURE);
    }
    if(numofhdu!=NUM_HDU_EVENTFILE){
        LOG(INFO)<<"***Found "<<numofhdu<<" HDUs in event file ....Expected is "<<NUM_HDU_EVENTFILE<<"***";
        return (EXIT_FAILURE);
    }
    for(int i=2;i<=NUM_HDU_EVENTFILE;i++){
        fits_movabs_hdu(fptr_in,i,&hdutype,&status);
        if(status) { fits_report_error(stderr,status); return (EXIT_FAILURE); }
        if(hdutype!=BINARY_TBL){
            LOG(ERROR)<<"***HDU "<<i<<" in file "<<InputEvtFile<<" is not binary table***";
            return (EXIT_FAILURE);
        }
    } 
            
    
    //reading gain offset file
    float gain[YSIZE][XSIZE], offset[YSIZE][XSIZE], QE[YSIZE][XSIZE];
    flag = readGainOffsetFile(GainOffsetFile, gain, "GAIN", offset, "OFFSET",
            QE, "QE", XSIZE, YSIZE);
    if (flag) {
        LOG(ERROR)<<"***Error in reading gain and offset from gain offset file***";
        return (EXIT_FAILURE);
    }

    //reading ebounds file
    float Ebounds[NUM_CHANNEL][3];
    flag = readEbounds(RMF, NUM_CHANNEL, Ebounds, "EBOUNDS");
    if (flag) {
        LOG(ERROR)<<"***Error in reading energy bounds from response matrix file***";
        return (EXIT_FAILURE);
    }

    string comment = "HDU updated by module: " + (string) modulename + "  GainOffsetfile:" +
            (string) GainOffsetFile + " RMF file:" + (string) RMF;

    fits_movabs_hdu(fptr_in,1,NULL,&status);
    if(status) {fits_report_error(stderr,status); return (EXIT_FAILURE); }
    
    fits_create_file(&fptr,OutputEvtFile,&status);
    if(status){
        fits_report_error(stderr,status);
        LOG(ERROR)<<"***Error creating file "<<OutputEvtFile<<"***";
        return(EXIT_FAILURE);
    }
    
    fits_copy_file(fptr_in, fptr, 1, 1, 1, &status);
    if(status){
        fits_report_error(stderr,status);
        LOG(ERROR)<<"***Error in copying file***";
        return(EXIT_FAILURE);
    }
    
    fits_close_file(fptr_in,&status);
    if(status){
        fits_report_error(stderr,status);
        return(EXIT_FAILURE);
    } 
   
    char *extname[] = {"Q0", "Q1", "Q2", "Q3"};   //Extension names for quadrant event data
    char value[FLEN_VALUE];
    
    unsigned short *pha,*pi;           //to store pha value for one quadrant
    unsigned char *detx,*dety;   //to store pixel locations within one quadrant
    float *energy;
    long nrows;
    int ncols;
    int firstrow=1, firstelem=1;
    
    char *ttype[]={"PI","Energy"};
    char *tform[]={"I","E"};

    bool dataflag=false;
    
    for(i=0;i<NUMQUAD;i++){                        //for four quadrants       
        LOG(INFO)<<"Processing Quadrant "<<i+1<<" data.....";
        
        fits_write_comment(fptr,comment.c_str(),&status);
        if(status)  { fits_report_error(stderr,status);  return (EXIT_FAILURE); }
        fits_movnam_hdu(fptr, BINARY_TBL, extname[i], 0, &status);
        
        if(status)  { fits_report_error(stderr,status);  return (EXIT_FAILURE); }
 
        //checking whether quadrant data at correct HDU number or not
        fits_get_hdu_num(fptr, &curhdu);
        if(curhdu!=(i+2)){               //Quadrant 0 data must be at HDU 2, Q1 at HDU 3, Q2 at HDU 4 and Q4 at HDU 5
            LOG(ERROR)<<"cztpha2energy_process():***Data not found at correct quadrant number***";
            return (EXIT_FAILURE);
        }
        fits_get_num_cols(fptr,&ncols,&status);
        if(status)  { fits_report_error(stderr,status);  return (EXIT_FAILURE); }
                
        //checking whether PI/Energy column already exists
        fits_get_colnum(fptr, CASEINSEN, "ENERGY", &colnum, &status);
        if (status == 0) {
            LOG(ERROR) << "***Energy column already exists!!!***";
            return (EXIT_FAILURE);
        } 
        else if(status==COL_NOT_FOUND){
            status=0;
            fits_insert_col(fptr,ncols+1,ttype[1],tform[1],&status);
            if(status){ LOG(ERROR)<<"***Error in inserting column ENERGY***";
            return (EXIT_FAILURE); }
        }
        else{
            LOG(ERROR)<<"***Error in finding column number for ENERGY***";
            return (EXIT_FAILURE);
        }

        fits_get_colnum(fptr, CASEINSEN, "PI", &colnum, &status);
        if (status == 0) {
            LOG(INFO) << "***PI column already exist!!!*** ";
            return (EXIT_FAILURE);
        } 
        else if(status==COL_NOT_FOUND) {
            status = 0; //cout<<"PI column does not exist";
            fits_insert_col(fptr,ncols+2,ttype[0],tform[0],&status);
            if(status){ LOG(ERROR)<<"****Error in inserting column PI***";
            return (EXIT_FAILURE); }     
        }
        else{
            LOG(ERROR)<<"***Error in finding column number for PI***";
            return (EXIT_FAILURE);
        }
        
        //get number of rows for pha
        fits_get_num_rows(fptr,&nrows,&status);
        if(status)  { fits_report_error(stderr,status);  return (EXIT_FAILURE); }
        
        LOG(INFO)<<"Number of rows in quadrant "<<i+1<<" is "<<nrows;
        
        if(nrows==0)  continue;
        else{
            dataflag=dataflag|true;
            pha=new unsigned short[nrows];    
            if(pha==NULL) { LOG(ERROR)<<"cztpha2energy_process():PHA -Out of Memory Error"; return (EXIT_FAILURE);}
            detx=new unsigned char[nrows];    
            if(detx==NULL) { LOG(ERROR)<<"cztpha2energy_process():DETX-Out of Memory Error"; return (EXIT_FAILURE);}
            dety=new unsigned char[nrows];  
            if(dety==NULL) { LOG(ERROR)<<"cztpha2energy_process():DETY-Out of Memory Error"; return (EXIT_FAILURE);}
            pi=new unsigned short[nrows];
            if(pi==NULL) { LOG(ERROR)<<"cztpha2energy_process():PI-Out of Memory Error"; return (EXIT_FAILURE);}
            energy=new float[nrows];
            if(energy==NULL) { LOG(ERROR)<<"cztpha2energy_process():ENERGY-Out of Memory Error"; return (EXIT_FAILURE);}

            //getting column number for pha
            fits_get_colnum(fptr, CASEINSEN, "PHA", &colnum, &status);
            if(status)  { fits_report_error(stderr,status);  return (EXIT_FAILURE); }
           
            fits_read_col(fptr,TUSHORT,colnum,firstrow,firstelem,nrows,NULL,pha,NULL,&status);
            if(status)  { fits_report_error(stderr,status);  return (EXIT_FAILURE); }
            
            //getting column number for detx
            fits_get_colnum(fptr, CASEINSEN, "DETX", &colnum, &status);
            if(status)  { fits_report_error(stderr,status);  return (EXIT_FAILURE); }
            
            fits_read_col(fptr,TUSHORT,colnum,firstrow,firstelem,nrows,NULL,detx,NULL,&status);
            if(status)  { fits_report_error(stderr,status);  return (EXIT_FAILURE); }
           
            //getting column number for dety
            fits_get_colnum(fptr, CASEINSEN, "DETY", &colnum, &status);
            if(status)  { fits_report_error(stderr,status);  return (EXIT_FAILURE); }
            
            fits_read_col(fptr,TUSHORT,colnum,firstrow,firstelem,nrows,NULL,dety,NULL,&status);
            if(status)  { fits_report_error(stderr,status);  return (EXIT_FAILURE); }
            
            if(pha2energy(i,pha,detx,dety,nrows,gain,offset,Ebounds,pi,energy)){
                LOG(ERROR)<<"***Error in pha2energy conversion***";
                return (EXIT_FAILURE);
            }

            //getting column number for PI
            fits_get_colnum(fptr, CASEINSEN, "PI", &colnum, &status);
            if(status)  { fits_report_error(stderr,status);  return (EXIT_FAILURE); }
           
            //writing PI Column to file
            fits_write_col(fptr,TUSHORT,ncols+2,firstrow,firstelem,nrows,pi,&status);
            if(status)  { fits_report_error(stderr,status);  return (EXIT_FAILURE); }
           
            //getting column number for Energy
            fits_get_colnum(fptr, CASEINSEN, "Energy", &colnum, &status);
            if(status)  { fits_report_error(stderr,status);  return (EXIT_FAILURE); }
            
            //writing Energy Column to file
            fits_write_col(fptr,TFLOAT,ncols+1,firstrow,firstelem,nrows,energy,&status);
            if(status)  { fits_report_error(stderr,status);  return (EXIT_FAILURE); }
           
            //delete pha,pi,energy,detx,dety;
            }
    }
    if(dataflag==false){
        LOG(ERROR)<<"***No Event data found in input file***";
        //unlink(OutputEvtFile);
        fits_delete_file(fptr,&status);
        if(status)  { fits_report_error(stderr,status);  return (EXIT_FAILURE); }
        return (EXIT_FAILURE);
    }
         
    //updating keywords and writing history if required
    for(i=1;i<=numofhdu;i++){
        fits_movabs_hdu(fptr,i,NULL,&status);  if(status)  { fits_report_error(stderr,status);  return (EXIT_FAILURE); }
        fits_write_date(fptr,&status);   if(status)  { fits_report_error(stderr,status);  return (EXIT_FAILURE); }
        fits_write_chksum(fptr,&status);   if(status)  { fits_report_error(stderr,status);  return (EXIT_FAILURE); }
        fits_update_key(fptr,TSTRING,"CREATOR",modulename,"Module that created this file",&status);
        if(status)  { fits_report_error(stderr,status);  return (EXIT_FAILURE); }
        if(history==YES){
            vector<string> vhistory;
            getHistory(vhistory);
            for(int j=0;j<vhistory.size();j++){
                fits_write_history(fptr,vhistory[j].c_str(),&status);
                 if(status)  { fits_report_error(stderr,status);  return (EXIT_FAILURE); }
            }
        }
    }
    
    fits_close_file(fptr,&status);
    if(status)  { fits_report_error(stderr,status);  return (EXIT_FAILURE); }
    return (EXIT_SUCCESS);

}

int pha2energy(int quadrantno,unsigned short *pha,unsigned char *detx,unsigned char *dety,

        long nrows,float gain[YSIZE][XSIZE],float offset[YSIZE][XSIZE], float ebounds[NUM_CHANNEL][3],
        unsigned short *pi,float *energy) {
    //cout << "Converting--";
    
    int xadd,yadd;    
    
    switch(quadrantno){
        case 0: yadd=0; xadd=0;         //Q0 starts at (0,0)
            break;
        case 1: yadd=0;  xadd=XSIZE/2;  //Q1 starts at (0,64)
            break;
        case 2: yadd=YSIZE/2;  xadd=XSIZE/2;
            break;
        case 3: yadd=YSIZE/2;  xadd=0;
            break;
        default: LOG(ERROR)<<"pha2energy(): Invalid quadrant number";
                return (EXIT_FAILURE);
    }
    
    float g,o;   //for gain and offset
    int x,y;
    int channel;
    for(long i=0;i<nrows;i++){
        x=detx[i]+xadd;   y=dety[i]+yadd;
        //cout<<setw(14)<<i<<setw(10)<<"  x:"<<x<<setw(10)<<"y:"<<y;
        g=gain[y][x];   //finding gain to be used for this pha
        o=offset[y][x];  //finding offset to be used for this pha
        pi[i]=g*(pha[i]-o);
        channel=pi[i];
        if(channel>(NUM_CHANNEL-1))  channel=NUM_CHANNEL-1;
        energy[i]=ebounds[channel][2];
    }
    LOG(INFO)<<"Conversion of PHA to PI/ENERGY completed!!!";
    return (EXIT_SUCCESS);
}

int readGainOffsetFile(char *file, float gain[][XSIZE], char *gain_name,
        float offset[][XSIZE], char *offset_name, float QE[][XSIZE],
        char *QE_name, int x, int y) {
    fitsfile *fptr;
    int status = 0;
    fits_open_file(&fptr, file, READONLY, &status);
    if(status){
        fits_report_error(stderr,status);
        LOG(ERROR)<<"***Error in opening file "<<file<<"***";
        return(EXIT_FAILURE);
    } 

    long naxes[2];
    int bitpix;

    //Reading Quantum Efficiency map
    fits_movnam_hdu(fptr, IMAGE_HDU, QE_name, 0, &status);
    if(status)  { 
        LOG(ERROR)<<"***Image HDU expected in gain offset file***";  
        fits_report_error(stderr,status);  return (EXIT_FAILURE); }

    fits_get_img_param(fptr, 2, &bitpix, NULL, naxes, &status);
    if(status)  { fits_report_error(stderr,status);  return (EXIT_FAILURE); }

    if (naxes[0] != XSIZE || naxes[1] != YSIZE) {
        LOG(ERROR) << "Array Size does not match size of detector pixels";
        return (EXIT_FAILURE);
    }

    long fpixel[2];
    fpixel[0] = 1; fpixel[1] = 1;
    long nelements = (int)XSIZE*(int)YSIZE;
    fits_read_pix(fptr, TFLOAT, fpixel, nelements, NULL, QE, NULL, &status);
    if(status)  { fits_report_error(stderr,status);  return (EXIT_FAILURE); }

    //Reading Offset map
    fits_movnam_hdu(fptr, IMAGE_HDU, offset_name, 0, &status);
    if(status)  { fits_report_error(stderr,status);  return (EXIT_FAILURE); }

    fits_get_img_size(fptr, 2, naxes, &status);
    if(status)  { fits_report_error(stderr,status);  return (EXIT_FAILURE); }

    if (naxes[0] != XSIZE || naxes[1] != YSIZE) {
        LOG(ERROR) << "Array Size does not match size of detector pixels";
        return (EXIT_FAILURE);
    }

    fits_read_pix(fptr, TFLOAT, fpixel, nelements, NULL, offset, NULL, &status);
    if(status)  { fits_report_error(stderr,status);  return (EXIT_FAILURE); }

    //Reading Gain Map
    fits_movnam_hdu(fptr, IMAGE_HDU, gain_name, 0, &status);
    if(status)  { fits_report_error(stderr,status);  return (EXIT_FAILURE); }

    fits_get_img_size(fptr, 2, naxes, &status);
    if(status)  { fits_report_error(stderr,status);  return (EXIT_FAILURE); }

    if(naxes[0] != XSIZE || naxes[1] != YSIZE) {
        LOG(ERROR) << "Array Size does not match size of detector pixels";
        return (EXIT_FAILURE);
    }

    fits_read_pix(fptr, TFLOAT, fpixel, nelements, NULL, gain, NULL, &status);
    if(status)  { fits_report_error(stderr,status);  return (EXIT_FAILURE); }

    fits_close_file(fptr, &status);
    LOG(INFO) << "Gain Offset reading complete!!!";
    return (EXIT_SUCCESS);
}

int readEbounds(char *file, int num_chan, float Ebounds[][3], char *ebounds_name) {
    fitsfile *fptr;
    int status = 0;
    fits_open_file(&fptr, file, READONLY, &status);
    if(status)  { fits_report_error(stderr,status);  
        LOG(ERROR)<<"***Error opening file "<<file<<"***";
        return (EXIT_FAILURE); }

    fits_movnam_hdu(fptr, BINARY_TBL, ebounds_name, 0, &status);
    if(status)  { fits_report_error(stderr,status);  return (EXIT_FAILURE); }

    long nrows;
    fits_get_num_rows(fptr, &nrows, &status);
    if(status)  { fits_report_error(stderr,status);  return (EXIT_FAILURE); }

    if (nrows != num_chan) {
        LOG(ERROR) << "***Number of channels does not match number of rows in Ebounds extension***";
        return (EXIT_FAILURE);
    }

    for (int i = 0; i < num_chan; i++) {
        fits_read_col(fptr, TFLOAT, 2, i + 1, 1, 1, NULL, &Ebounds[i][0], NULL, &status);
        if(status)  { fits_report_error(stderr,status);  return (EXIT_FAILURE); }
        fits_read_col(fptr, TFLOAT, 2, i + 1, 1, 1, NULL, &Ebounds[i][1], NULL, &status);
        if(status)  { fits_report_error(stderr,status);  return (EXIT_FAILURE); }
        Ebounds[i][2] = (Ebounds[i][0] + Ebounds[i][1]) / 2;   //finding mid energy
    }
    LOG(INFO) << "Ebounds reading complete!!!";
    return (EXIT_SUCCESS);
}

int cztpha2energy::getHistory(vector<string> &vhistory){
    //char *user=getlogin();
    char *user = getenv("USER");
	string str="Module run by "+(string)user;
    vhistory.push_back(str);
    vhistory.push_back("Parameter List START for "+(string)modulename);
    vhistory.push_back("P1 infile="+(string)InputEvtFile);
//    vhistory.push_back("P2 GOfile="+(string)GainOffsetFile);
//    vhistory.push_back("P3 rmfile="+(string)RMF);
//    vhistory.push_back("P4 outfile="+(string)OutputEvtFile);
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
