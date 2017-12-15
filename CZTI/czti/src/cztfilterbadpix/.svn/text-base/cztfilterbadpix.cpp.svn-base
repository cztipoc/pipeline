
#include "cztfilterbadpix.h"
#include <iomanip>

cztfilterbadpix::cztfilterbadpix(){
    strcpy(modulename,"cztfilterbadpix_v");
    strcat(modulename,VERSION);
    strcpy(xcol,"");
    strcpy(ycol,"");
}

int cztfilterbadpix::read(int argc,char **argv){
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
    
    if(strncasecmp(inputtype,"event",3)==0){
         if(PIL_OK!=(status=PILGetString("xcol",xcol))){
            LOG(ERROR)<<"***Error Reading X column name:"<<xcol<<"***";
            return status;
        }
        if(PIL_OK!=(status=PILGetString("ycol",ycol))){
            LOG(ERROR)<<"***Error Reading Y column name:"<<ycol<<"***";
            return status;
        }
    }
    
    if(PIL_OK!=(status=PILGetFname("qualitymapfile",qualitymapfile))){
        LOG(ERROR)<<"***Error Reading qualitymapfile:"<<qualitymapfile<<"***";
        return status;
    }
    
    if(PIL_OK!=(status=PILGetFname("outfile",outfile))){
        LOG(ERROR)<<"***Error Reading outfile:"<<outfile<<"***";
        return status;
    }
    
    if(PIL_OK!=(status=PILGetString("outputquality",outputquality))){
        LOG(ERROR)<<"***Error Reading outputquality:"<<outputquality<<"***";
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

int cztfilterbadpix::read(char *infile,char *inputtype,char *qualitymapfile,char *outfile,
        char *outputquality,char *xcol,char *ycol,int clobber,int history){
    strcpy(this->infile,infile);
    strcpy(this->inputtype,inputtype);
    strcpy(this->qualitymapfile,qualitymapfile);
    strcpy(this->outfile,outfile);
    strcpy(this->outputquality,outputquality);
    if(xcol!=NULL)  strcpy(this->xcol,xcol);
    if(ycol!=NULL)  strcpy(this->ycol,ycol);
    this->clobber=clobber;
    this->history=history;
    return (EXIT_SUCCESS);
}

void cztfilterbadpix::display(){
    LOG(INFO)<<"----------------------------------------------------------------------------";
    LOG(INFO)<<"                          CZTFILTERBADPIX PARAMETERS                            ";
    LOG(INFO)<<"----------------------------------------------------------------------------";
    LOG(INFO)<<"Input event file   : "<<infile;
    LOG(INFO)<<"Quality Map File   : "<<qualitymapfile;
    LOG(INFO)<<"Input Type         : "<<inputtype;
    LOG(INFO)<<"output file        : "<<outfile;         
    LOG(INFO)<<"Output Quality     : "<<outputquality;
    LOG(INFO)<<"X column           : "<<xcol;
    LOG(INFO)<<"Y column           : "<<ycol;
    LOG(INFO)<<"Clobber            : "<<clobber;
    LOG(INFO)<<"History            : "<<history;
    LOG(INFO)<<"----------------------------------------------------------------------------";
}


/**
 * Function to filter bad pixels from event file or dpi file
 * output file only contains good/dead/noisy/bad pixels
 * @param par
 * @return 
 */
int cztfilterbadpix::cztfilterbadpixProcess(){
     int status=0;
     char errstr[1024];
     if(strcmp(outfile,infile)==0){  }
     else if(strcmp(outfile," ")==0 || strcmp(outfile,"-")==0){
         strcpy(outfile,infile);
     }
     else {                           //infile and outfile are different
        if(FileExists(outfile)){
            if(clobber==YES){
                    if(unlink(outfile)!=0){
                        //cerr<<"Error in deleting "<<outfile; 
                        //return (EXIT_FAILURE);
                    }
            }
            else{
                LOG(ERROR)<<""<<outfile<<" already exists";
                LOG(ERROR)<<"Use clobber=yes for overwriting the file";
                return (EXIT_FAILURE);
            }
        }
         //copying input to output file
         fitsfile *fin,*fout;
         //fits_open_file(&fin,infile,READWRITE,&status);  //commented by mayuri
         fits_open_file(&fin,infile,READONLY,&status);
         
         if(status) { 
             fits_report_error(stderr,status);
             LOG(ERROR)<<"***Error opening file "<<infile<<"***";
             return (EXIT_FAILURE);
         }
         fits_create_file(&fout,outfile,&status);
         if(status) { 
             fits_report_error(stderr,status);
             LOG(ERROR)<<"***Error creating file "<<outfile<<"***";
             return (EXIT_FAILURE);
         }
         fits_copy_file(fin,fout,1,1,1,&status);
         if(status) { 
             fits_report_error(stderr,status);
             LOG(ERROR)<<"***Error in copying file***";
             return (EXIT_FAILURE);
         }
         fits_close_file(fout,&status); if(status) { fits_report_error(stderr,status); return (EXIT_FAILURE); }
         fits_close_file(fin,&status);  if(status) { fits_report_error(stderr,status); return (EXIT_FAILURE); }
     }
     
     //creating list of required pixels wrt quadrant
     Q_coordinatelist qcoord[NUMQUAD];          //this qx,qy is with respect to quadrant not in full array of 128 x 128
                                                //contains list to be deleted                                                        
     vector<int> vx,vy;                              //this x,y is wrt to full 128x128 array
     int x,y,q,qx,qy;
     //Opening and Reading Quality map file
     fitsfile *fqmap;
     fits_open_file(&fqmap,qualitymapfile,READONLY,&status);
     if(status) { fits_report_error(stderr,status);  return (EXIT_FAILURE); }
     long npixels=0;
     unsigned char qmap[XSIZE*YSIZE];
     long fpixel[2];
     fpixel[0]=fpixel[1]=1;
     fits_read_pix(fqmap,TBYTE,fpixel,XSIZE*YSIZE,NULL,qmap,NULL,&status);
     if(status) { fits_report_error(stderr,status);  return (EXIT_FAILURE); }
     
     if(strncasecmp(outputquality,"good",3)==0){        //create list of pixels other than good
         for(int i=0;i<XSIZE*YSIZE;i++){
                x=i%XSIZE;   y=i/XSIZE;
                status=getqxqy(x,y,&qx,&qy,&q);
                if(status){
                    LOG(ERROR)<<"Error in getting qx and qy"; return (EXIT_FAILURE);
                }
                if(qmap[i]!=GOODPIX){
                    vx.push_back(x); vy.push_back(y);
                    qcoord[q].vqx.push_back(qx);
                    qcoord[q].vqy.push_back(qy);
                    npixels++;
                }
         }
     }
     else if(strncasecmp(outputquality,"dead",3)==0){
         for(int i=0;i<XSIZE*YSIZE;i++){
                x=i%XSIZE;   y=i/XSIZE;
                status=getqxqy(x,y,&qx,&qy,&q);
                if(status){
                    LOG(ERROR)<<"Error in getting qx and qy"; return (EXIT_FAILURE);
                }
                if(qmap[i]!=DEADPIX){
                    vx.push_back(x); vy.push_back(y);
                    qcoord[q].vqx.push_back(qx);
                    qcoord[q].vqy.push_back(qy);
                    npixels++;
                }
         }
     }
     else if(strncasecmp(outputquality,"noisy",3)==0){
         for(int i=0;i<XSIZE*YSIZE;i++){
                x=i%XSIZE;   y=i/XSIZE;
                status=getqxqy(x,y,&qx,&qy,&q);
                if(status){
                    LOG(ERROR)<<"Error in getting qx and qy"; return (EXIT_FAILURE);
                }
                if(qmap[i]!=NOISYPIX){
                    vx.push_back(x); vy.push_back(y);
                    qcoord[q].vqx.push_back(qx);
                    qcoord[q].vqy.push_back(qy);
                    npixels++;
                }
         }
     }
     else if(strncasecmp(outputquality,"bad",3)==0){                    //for bad pixels, will contain both dead and noisy pixels
         for(int i=0;i<XSIZE*YSIZE;i++){
                x=i%XSIZE;   y=i/XSIZE;
                status=getqxqy(x,y,&qx,&qy,&q);
                if(status){
                    LOG(ERROR)<<"Error in getting qx and qy"; return (EXIT_FAILURE);
                }
                if(qmap[i]==GOODPIX){                        //good pixels have to be removed
                    vx.push_back(x); vy.push_back(y);
                    qcoord[q].vqx.push_back(qx);
                    qcoord[q].vqy.push_back(qy);
                    npixels++;
                }
         }
     }
     else{
         LOG(ERROR)<<"Invalid qualityUse good/bad/dead/noisy";
         return (EXIT_FAILURE);
     }
     
     LOG(INFO)<<"Number of pixels accepted:"<<(XSIZE*YSIZE)-npixels;
     fits_close_file(fqmap,&status);
     if(status) { fits_report_error(stderr,status);  return (EXIT_FAILURE); }
     LOG(INFO)<<"Reading quality map file completed";
     fitsfile *fout;
     fits_open_file(&fout,outfile,READWRITE,&status);
     if(status){
         LOG(ERROR)<<"***Error in opening file***";
         fits_report_error(stderr,status);
         return status;
     }
     int hdutype=0;
     if(strncasecmp(inputtype,"event",3)==0){
         LOG(INFO)<<"Filtering EVENTS......";
         vector<long> rowlist;   //row list to delete
         int xcol_n,ycol_n;
         for(int i=EVENTDATA_HDU_BEGIN,j=0;j<NUMQUAD;j++,i++){
             LOG(INFO)<<"Filtering events for quadrant "<<j;
             fits_movabs_hdu(fout,i,&hdutype,&status);
             if(status) { fits_report_error(stderr,status);  return (EXIT_FAILURE); }
             if(hdutype!=BINARY_TBL) {
                 LOG(ERROR)<<"Data Must be binary table in file";
                 return (EXIT_FAILURE);
             }
             fits_get_colnum(fout,CASEINSEN,xcol,&xcol_n,&status);
            if(status) { fits_report_error(stderr,status);  return (EXIT_FAILURE); }
             fits_get_colnum(fout,CASEINSEN,ycol,&ycol_n,&status);
             if(status) { fits_report_error(stderr,status);  return (EXIT_FAILURE); }
             long nrows=0,firstrow=1,firstelem=1;
             fits_get_num_rows(fout,&nrows,&status);
             if(status) { fits_report_error(stderr,status);  return (EXIT_FAILURE); }
             if(nrows>0){
                int *detx,*dety;
                detx=new int[nrows];
                dety=new int[nrows];
                if(detx==NULL || dety==NULL){
                    LOG(ERROR)<<"***Out of Memory Error***";
                    return (EXIT_FAILURE);
                }
                //reading x column
                fits_read_col(fout,TINT,xcol_n,firstrow,firstelem,nrows,NULL,detx,NULL,&status); 
                if(status) { fits_report_error(stderr,status);  return (EXIT_FAILURE); }
                //reading y column
                fits_read_col(fout,TINT,ycol_n,firstrow,firstelem,nrows,NULL,dety,NULL,&status); 
                if(status) { fits_report_error(stderr,status);  return (EXIT_FAILURE); }
                rowlist.clear();
                for(long k=0;k<nrows;k++){
                    if(xyExist(detx[k],dety[k],qcoord[j].vqx,qcoord[j].vqy)==0)
                       rowlist.push_back(k+1); 
                }                                                                 
                fits_delete_rowlist(fout,rowlist.data(),rowlist.size(),&status);
                if(status) { fits_report_error(stderr,status);  return (EXIT_FAILURE); }
                delete[] detx,dety;
             }
         }        
      }
      else if(strncasecmp(inputtype,"dpi",3)==0){
          LOG(INFO)<<"Filtering DPI.....";
          long totcount=0;
          long qdpi[XPIX_QUAD*YPIX_QUAD];
          long fulldpi[XSIZE*YSIZE];
          for(int i=2,j=0;j<NUMQUAD;i++,j++){
             fits_movabs_hdu(fout,i,&hdutype,&status);
             if(status) { fits_report_error(stderr,status);  return (EXIT_FAILURE); }
             if(hdutype!=IMAGE_HDU) {
                 LOG(ERROR)<<"Image HDU not found in input file";
                 return (EXIT_FAILURE);
             }
             fits_read_key(fout,TLONG,"DPICOUNT",&totcount,NULL,&status);
             if(status){
                 LOG(ERROR)<<"Error reading TOTCOUNT keyword";
                 fits_report_error(stderr,status);
                 return (EXIT_FAILURE);
             }
             fits_read_pix(fout,TLONG,fpixel,XPIX_QUAD*YPIX_QUAD,NULL,qdpi,NULL,&status);
             if(status) { fits_report_error(stderr,status);  return (EXIT_FAILURE); }
             for(int k=0;k<qcoord[j].vqx.size();k++){
                 qx=qcoord[j].vqx[k];   qy=qcoord[j].vqy[k];
                 totcount=totcount-qdpi[qy*XPIX_QUAD+qx];
                 //cout<<"qx "<<qx<<"  qy "<<qy;
                 qdpi[qy*XPIX_QUAD+qx]=0;
             }
             fpixel[0]=fpixel[1]=1;
             fits_write_pix(fout,TLONG,fpixel,XPIX_QUAD*YPIX_QUAD,qdpi,&status);
             if(status) { fits_report_error(stderr,status);  return (EXIT_FAILURE); }
             fits_update_key(fout,TLONG,"DPICOUNT",&totcount,NULL,&status);
             if(status){
                LOG(ERROR)<<"Error updating DPICOUNT keyword";
                fits_report_error(stderr,status);
                return (EXIT_FAILURE);
             }
         }
         //for full dpi
         fits_movabs_hdu(fout,6,NULL,&status);
         if(status) { fits_report_error(stderr,status);  return (EXIT_FAILURE); }
         fpixel[0]=fpixel[1]=1;
         totcount=0;
//         fits_read_key(fout,TLONG,"TOTCOUNT",&totcount,NULL,&status);
//         if(status){
//            LOG(ERROR)<<"Error reading TOTCOUNT keyword";
//            fits_report_error(stderr,status);
//            return (EXIT_FAILURE);
//         }
         fits_read_pix(fout,TLONG,fpixel,XSIZE*YSIZE,NULL,fulldpi,NULL,&status);
         if(status) { fits_report_error(stderr,status);  return (EXIT_FAILURE); }
         for(int k=0;k<vx.size();k++){
            x=vx[k];   y=vy[k];
            totcount=totcount-fulldpi[y*XSIZE+x];
            fulldpi[y*XSIZE+x]=0;
         }
//         fits_update_key(fout,TLONG,"TOTCOUNT",&totcount,NULL,&status);
//         if(status){
//                LOG(ERROR)<<"Error updating TOTCOUNT keyword";
//                fits_report_error(stderr,status);
//                return (EXIT_FAILURE);
//         }
         fpixel[0]=fpixel[1]=1;
         fits_write_pix(fout,TLONG,fpixel,XSIZE*YSIZE,fulldpi,&status);
         if(status) { fits_report_error(stderr,status);  return (EXIT_FAILURE); }
     }
     else{
         LOG(ERROR)<<"***Invalid Input Type---Must be 'dpi'/'event'***";
         return (EXIT_FAILURE);
     }
     fits_close_file(fout,&status);
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

//returns 0 when (x,y) exists in vector list
int xyExist(int x,int y,vector<int> &vx,vector<int> &vy){
    int flag=1;               //x,y is not there
    for(int i=0;i<vx.size();i++){
        if(x==vx[i] && y==vy[i]){
            flag=0;
            break;
        }
    }
    return flag;
}

/**
 * Function to generate history for the module, to be written to the output file
 * @param par
 * @param vhistory
 * @return 
 */
int cztfilterbadpix::getHistory(vector<string> &vhistory){
    //char *user=getlogin();
	char *user = getenv("USER");
    string str="Module run by "+(string)user;
    vhistory.push_back(str);
    vhistory.push_back("Parameter List START for "+(string)modulename);
    vhistory.push_back("P1 infile="+(string)infile);
    vhistory.push_back("P2 inputtype="+(string)inputtype);
    vhistory.push_back("P3 qualitymapfile="+(string)qualitymapfile);
    vhistory.push_back("P4 outfile="+(string)outfile);
    vhistory.push_back("P5 outputquality="+(string)outputquality);
    if(clobber==YES) 
        vhistory.push_back("P6 clobber=yes");
    else
        vhistory.push_back("P6 clobber=no");
    if(history==YES)
        vhistory.push_back("P7 history=yes");
    else
        vhistory.push_back("P7 history=no");
    vhistory.push_back("Parameter List END");
    return (EXIT_SUCCESS);
}
