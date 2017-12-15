/**
 The line where comment "*TBC" is there, the numerator has to be changed to A.x
 * if CZTI pointing is along y axis
 
 */
#include<iostream>
#include<fitsio.h>
#include<cmath>
#include"cztgaas.h"

using namespace std;

cztgaas::cztgaas(){
    strcpy(modulename,"cztgaas_v");
    strcat(modulename,VERSION);
}

int cztgaas::read(int argc,char **argv){
   int status=0;

    if(PIL_OK!=(status=PILInit(argc,argv))){
        LOG(ERROR)<<"***Error Initializing PIL***";
        return status;
    }

    if(PIL_OK!=(status=PILGetFname("evtfile",eventfile))){
        LOG(ERROR)<<"***Error Reading input event data file:"<<eventfile<<"***";
        return status;
    }

    if(PIL_OK!=(status=PILGetFname("teldef",teldeffile))){
        LOG(ERROR)<<"***Error Reading teldeffile file:"<<teldeffile<<"***";
        return status;
    }

    if(PIL_OK!=(status=PILGetFname("attfile",attitudefile))){
        LOG(ERROR)<<"***Error Reading attitude file name:"<<attitudefile<<"***";
        return status;
    }
    
    if(PIL_OK!=(status=PILGetFname("outfile",outfile))){
        LOG(ERROR)<<"***Error Reading output aspect array file name:"<<outfile<<"***";
        return status;
    }

    if(PIL_OK!=(status=PILGetBool("history",&history))){
        LOG(ERROR)<<"***Error Reading history parameter"<<history<<"***";
        return status;
    }

    if(PIL_OK!=(status=PILGetBool("clobber",&clobber))){
        LOG(ERROR)<<"***Error Reading clobber:"<<clobber<<"***";
        return status;
    }

    PILClose(status);
    return (EXIT_SUCCESS);
 }

int cztgaas::read(char *eventfile,char *teldef,char *att,char *outfile,
        int clobber,int history){
    
    strcpy(this->eventfile,eventfile);
    strcpy(this->teldeffile,teldef);
    strcpy(this->attitudefile,att);
    strcpy(this->outfile,outfile);
    this->clobber=clobber;
    this->history=history;
    
    return (EXIT_SUCCESS);
}

void cztgaas::display(){
    LOG(INFO)<<"-----------------------------------------------------------------";
    LOG(INFO)<<"                     CZTGAAS PARAMETERS                            ";
    LOG(INFO)<<"-----------------------------------------------------------------";
    LOG(INFO)<<"Taskname           : "<<modulename;
    LOG(INFO)<<"Event data file    : "<<eventfile;         //input science data file 
    LOG(INFO)<<"TELDEF file        : "<<teldeffile;
    LOG(INFO)<<"Attitude file      : "<<attitudefile;
    LOG(INFO)<<"Output aspect file : "<<outfile;       //output event file
    LOG(INFO)<<"Clobber            : "<<clobber;
    LOG(INFO)<<"History            : "<<history;
    LOG(INFO)<<"-----------------------------------------------------------------";

}

int cztgaas::cztgaasProcess(){
    if(FileExists(outfile)){
        if(clobber==YES){
                unlink(outfile);
        }
        else{
            LOG(ERROR)<<""<<outfile<<" already exists";
            LOG(ERROR)<<"Use clobber=yes for overwriting the file";
            return (EXIT_FAILURE);
        }
    }
   
    Q q_cztalign;
    if(getCZTIq(teldeffile,&q_cztalign.q1,&q_cztalign.q2,&q_cztalign.q3,
            &q_cztalign.q4)){
        LOG(ERROR)<<"***Error in reading CZTI alignment quaternion from teldef file***";
        return (EXIT_FAILURE);
    }
    q_cztalign.normalize();
    q_cztalign.getAxisAngle(); //calculates x,y,z and theta values.
    
    LOG(INFO)<<"CZTI alignment quaternion";
    q_cztalign.display(); // displays q1, q2, q3, q4 and theta and axis vector.
   
    Axis cztaxis(q_cztalign.x,q_cztalign.y,q_cztalign.z),cztinertial,cztrot;  
    Axis czt_det_normal(0,1,0);
    Axis tunitvector(1,0,0),tinertial;
    
    double start,stop;
    
    vector<Att> att; //Att is a structure containing q1,q2,q3,q4,t and a function to display these.
    if(getATTq(eventfile,attitudefile,att,1)!=0){
        LOG(ERROR)<<"***Error reading attitude data***";
        return (EXIT_FAILURE);
    }
    long numatt;
    numatt=att.size();
    if(numatt<=0){
        LOG(ERROR)<<"***Number of required attitude samples are zero***";
        return (EXIT_FAILURE);
    }
    double *qc_array=new double[numatt*4];  //each quaternion will have four elements
                                            //storing in array to write in o/p fits file in one go
    double *axisA=new double[numatt*3];     //each axis has three elements
    double *axisT=new double[numatt*3];     
    
//  for(int i=0;i<att.size();i++){
//      att[i].display();
//  }
    long index_qc=0,indexA=0,indexT=0;
    LOG(INFO)<<"Number of attitude points:"<<att.size();
    double sum_xa=0,sum_ya=0,sum_za=0,sum_xt=0,sum_yt=0,sum_zt=0;
    for(int i=0;i<att.size();i++){
        Q q_att;
        q_att.q1=att[i].q1;  q_att.q2=att[i].q2;   q_att.q3=att[i].q3;   q_att.q4=att[i].q4;
        q_att.normalize();
        rotate(cztaxis,q_att,cztrot);
        Q q_new,qc;
        q_new.theta=q_cztalign.theta;    q_new.x=cztrot.x; 
        q_new.y=cztrot.y;                q_new.z=cztrot.z;
        q_new.getQuat();
        q_new.normalize();
        quaternion_product(q_new,q_att,qc);   //computing qc
        qc_array[index_qc++]=qc.q1;  qc_array[index_qc++]=qc.q2;  
        qc_array[index_qc++]=qc.q3;  qc_array[index_qc++]=qc.q4;
        rotate(czt_det_normal,qc,cztinertial);
        sum_xa = sum_xa + cztinertial.x;
        sum_ya = sum_ya + cztinertial.y;
        sum_za = sum_za + cztinertial.z;
        axisA[indexA++]=cztinertial.x;  axisA[indexA++]=cztinertial.y;   axisA[indexA++]=cztinertial.z;
        rotate(tunitvector,qc,tinertial);
        sum_xt = sum_xt + tinertial.x;
        sum_yt = sum_yt + tinertial.y;
        sum_zt = sum_zt + tinertial.z;
         axisT[indexT++]=tinertial.x;  axisT[indexT++]=tinertial.y;   axisT[indexT++]=tinertial.z;
    }
    double denominator,xa,ya,za,xt,yt,zt;
    xa=sum_xa/att.size();  ya=sum_ya/att.size();   za=sum_za/att.size();
    xt=sum_xt/att.size();  yt=sum_yt/att.size();   zt=sum_zt/att.size();
    
    Axis A(xa,ya,za),T(xt,yt,zt);       //A is the inertial vector towards the source
                                        //T is the inertial vector for x axis of the CZTI instrument                                         
    A.normalize();     T.normalize();
    //A.display(); 
    //T.display();
    double cos_ra=(A.y/sqrt(A.x*A.x+A.y*A.y));         //to be changed as per pointing axis of CZTI  -TBC*
    //double cos_ra=(A.x/sqrt(A.x*A.x+A.y*A.y));       //if pointing axis is X axis of CZTI
    double RA,DEC;
    RA=acos(cos_ra);
    DEC=asin(A.z);
    if(A.y<0) RA=2*M_PI-RA;
    LOG(INFO)<<"RA:"<<RA<<" radians    DEC:"<<DEC<<" radians";
    RA=RA*180/M_PI;   DEC=DEC*180/M_PI;
    LOG(INFO)<<"RA:"<<RA<<" deg       DEC:"<<DEC<<" deg";
    //cout;
    
    double twist;
    Axis ref(cos_ra,(A.z),0.0);
    twist=acos((ref.x*T.x+ref.y*T.y+ref.z*T.z)/(T.getMod()*ref.getMod()));
    twist=twist*180/M_PI;
    if(T.z>0) twist=360-twist;
    //cout<<"Twist:"<<twist<<" deg";
    if(twist==NAN) twist=0;
     //finding average QC values
    double qc1=0,qc2=0,qc3=0,qc4=0;
    for(long i=0;i<numatt*4;i=i+4){
        qc1=qc1+qc_array[i];
        qc2=qc2+qc_array[i+1];
        qc3=qc3+qc_array[i+2];
        qc4=qc4+qc_array[i+3];
    }
    
    qc1=qc1/numatt;  qc2=qc2/numatt;  qc3=qc3/numatt;  qc4=qc4/numatt;
    //cout<<"QC";
    //cout<<"\t"<<qc1<<" "<<qc2<<" "<<qc3<<" "<<qc4;
    //creating average aspect file
    fitsfile *fout,*fevt;
    int status=0;
    fits_open_file(&fevt,eventfile,READWRITE,&status);
    if(status){
        fits_report_error(stderr,status);
        LOG(ERROR)<<"***Error in opening file "<<eventfile<<"***";
        return (EXIT_FAILURE);
    }
    
    fits_create_file(&fout,outfile,&status); 
    if(status){
        fits_report_error(stderr,status);
        LOG(ERROR)<<"***Error in creating file "<<outfile<<"***";
        return (EXIT_FAILURE);
    }
    
    int bitpix=8;
    int naxis=0;
    long *naxes=NULL; 
    fits_create_img(fout,bitpix,naxis,naxes,&status); 
    if(status){
        fits_report_error(stderr,status);
        return (EXIT_FAILURE);
    }
    
    if(copyUserKeyWords(fevt,fout))
        return (EXIT_FAILURE);
    
    fits_write_key(fout,TDOUBLE,"RA",&RA,"Average Aspect",&status);  
    if(status) { fits_report_error(stderr,status);  return (EXIT_FAILURE); }
    fits_write_key(fout,TDOUBLE,"DEC",&DEC,"Average Aspect",&status); 
    if(status) { fits_report_error(stderr,status);  return (EXIT_FAILURE); }
    fits_write_key(fout,TDOUBLE,"TWIST",&twist,"Average Aspect",&status); 
    if(status) { fits_report_error(stderr,status);  return (EXIT_FAILURE); }
    fits_write_key(fout,TDOUBLE,"QC1",&qc1,"Average Aspect",&status); 
    if(status) { fits_report_error(stderr,status);  return (EXIT_FAILURE); }
    fits_write_key(fout,TDOUBLE,"QC2",&qc2,"Average Aspect",&status); 
    if(status) { fits_report_error(stderr,status);  return (EXIT_FAILURE); }
    fits_write_key(fout,TDOUBLE,"QC3",&qc3,"Average Aspect",&status);
    if(status) { fits_report_error(stderr,status);  return (EXIT_FAILURE); }
    fits_write_key(fout,TDOUBLE,"QC4",&qc4,"Average Aspect",&status); 
    if(status) { fits_report_error(stderr,status);  return (EXIT_FAILURE); }
    fits_write_key(fout,TSTRING,"CREATOR",modulename,NULL,&status); 
    if(status) { fits_report_error(stderr,status);  return (EXIT_FAILURE); }
    fits_write_key(fout,TSTRING,"ORIGIN",(char *)ORIGIN,NULL,&status);
    if(status) { fits_report_error(stderr,status);  return (EXIT_FAILURE); }
    fits_write_date(fout,&status); 
    if(status) { fits_report_error(stderr,status);  return (EXIT_FAILURE); }
        
    string comment="Inputs: Event file: "+(string)eventfile+" Attitude file: "+
            (string)attitudefile+" TELDEF file: "+(string)teldeffile;
    
    fits_write_comment(fout,comment.c_str(),&status);
    if(status) { fits_report_error(stderr,status);  return (EXIT_FAILURE); }
    
    fits_write_chksum(fout,&status);
    
    //Creating next HDU for aspect values
    int tfields=4;
    
    char *ttype[]={"Time","Qc","CZTI_Normal","CZTI_X"};
    char *tform[]={"D","4D","3D","3D"};
    char *tunit[]={"s","","",""};
    char *extname="Aspect";
    fits_create_tbl(fout,BINARY_TBL,numatt,tfields,ttype,tform,tunit,extname,&status); 
    if(status) { fits_report_error(stderr,status);  return (EXIT_FAILURE); }
    
    long firstrow=1;
    long firstelem=1;
    for(long i=0;i<att.size();i++){
        fits_write_col(fout,TDOUBLE,1,i+1,firstelem,1,&att[i].t,&status); 
        if(status!=0)  { fits_report_error(stderr,status);  return status; } 
    }

    fits_write_col(fout,TDOUBLE,2,firstrow,firstelem,numatt*4,qc_array,&status); 
    if(status) { fits_report_error(stderr,status);  return (EXIT_FAILURE); }
    fits_write_col(fout,TDOUBLE,3,firstrow,firstelem,numatt*3,axisA,&status);  
    if(status) { fits_report_error(stderr,status);  return (EXIT_FAILURE); }
    fits_write_col(fout,TDOUBLE,4,firstrow,firstelem,numatt*3,axisT,&status); 
    if(status) { fits_report_error(stderr,status);  return (EXIT_FAILURE); }
    
    fits_write_key(fout,TDOUBLE,"RA",&RA,"Average Aspect",&status);  
    if(status) { fits_report_error(stderr,status);  return (EXIT_FAILURE); }
    fits_write_key(fout,TDOUBLE,"DEC",&DEC,"Average Aspect",&status); 
    if(status) { fits_report_error(stderr,status);  return (EXIT_FAILURE); }
    fits_write_key(fout,TDOUBLE,"TWIST",&twist,"Average Aspect",&status); 
    if(status) { fits_report_error(stderr,status);  return (EXIT_FAILURE); }
    fits_write_key(fout,TSTRING,"CREATOR",modulename,NULL,&status); 
    if(status) { fits_report_error(stderr,status);  return (EXIT_FAILURE); }
    fits_write_key(fout,TSTRING,"ORIGIN",(char *)ORIGIN,NULL,&status);
    if(status) { fits_report_error(stderr,status);  return (EXIT_FAILURE); }
    fits_write_date(fout,&status); 
    if(status) { fits_report_error(stderr,status);  return (EXIT_FAILURE); }
    fits_write_chksum(fout,&status);
    LOG(INFO)<<"Updating event file....";
    //updating in event file
    fits_update_key(fevt,TDOUBLE,"RA",&RA,"Average Aspect",&status);  
    if(status) { fits_report_error(stderr,status);  return (EXIT_FAILURE); }
    fits_update_key(fevt,TDOUBLE,"DEC",&DEC,"Average Aspect",&status); 
    if(status) { fits_report_error(stderr,status);  return (EXIT_FAILURE); }
    fits_update_key(fevt,TDOUBLE,"TWIST",&twist,"Average Aspect",&status); 
    if(status) { fits_report_error(stderr,status);  return (EXIT_FAILURE); }
    
    //writng history to output file
    
     if(history==YES){
         LOG(INFO)<<"Writing History ....";
         vector<string> vhistory;
         getHistory(vhistory);
         for(int i=1;i<=2;i++){
             fits_movabs_hdu(fout,i,NULL,&status);
             if(status) { fits_report_error(stderr,status);  return (EXIT_FAILURE); }
              for(int j=0;j<vhistory.size();j++){
                fits_write_history(fout,vhistory[j].c_str(),&status);
                if(status) { fits_report_error(stderr,status);  return (EXIT_FAILURE); }
            }
        }
      }
    
    delete[] qc_array,axisA,axisT;
    fits_close_file(fout,&status);  
    if(status) { fits_report_error(stderr,status);  return (EXIT_FAILURE); }
    fits_close_file(fevt,&status);  
    if(status) { fits_report_error(stderr,status);  return (EXIT_FAILURE); }
    LOG(INFO);
    return (EXIT_SUCCESS);          
}

void Att::display(){
    LOG(INFO)<< "time: "<< t<<" q1: "<<q1<<" q2: "<<q2<<" q3: "<<q3<<" q4: "<<q4;
}

int getCZTIq(char *teldef,double *q1,double *q2, double *q3, double *q4){
    //Code for reading teldef file 
    //value of quaternions will be returned in q1,q2,q3,q4   Note:q1 will be scalar
    fitsfile *fptr;
    int status=0;
    fits_open_file(&fptr,teldef,READONLY,&status);
    
    bool flag=true;
    fits_read_key(fptr,TDOUBLE,"Q1",&(*q1),NULL,&status);
    if(status)  flag=flag & false;
    fits_read_key(fptr,TDOUBLE,"Q2",&(*q2),NULL,&status);
    if(status)  flag=flag & false;
    fits_read_key(fptr,TDOUBLE,"Q3",&(*q3),NULL,&status);
    if(status)  flag=flag & false;
    fits_read_key(fptr,TDOUBLE,"Q4",&(*q4),NULL,&status);
    if(status)  flag=flag & false;
        
    double A,B,C;
    
    if(flag==false){
        LOG(INFO)<<"Computing czt Qs from Euler angles";
        fits_read_key(fptr,TDOUBLE,"EULERA",&A,NULL,&status);
        if(status) { fits_report_error(stderr,status);  return (EXIT_FAILURE); }
        fits_read_key(fptr,TDOUBLE,"EULERB",&B,NULL,&status);
        if(status) { fits_report_error(stderr,status);  return (EXIT_FAILURE); }
        fits_read_key(fptr,TDOUBLE,"EULERC",&C,NULL,&status);
        if(status) { fits_report_error(stderr,status);  return (EXIT_FAILURE); }
        
        *q1=cos(A/2)*cos(B/2)*cos(C/2)+sin(A/2)*sin(B/2)*sin(C/2);
        *q2=sin(A/2)*cos(B/2)*cos(C/2)-cos(A/2)*sin(B/2)*sin(C/2);
        *q3=cos(A/2)*sin(B/2)*cos(C/2)+sin(A/2)*cos(B/2)*sin(C/2);
        *q4=cos(A/2)*cos(B/2)*sin(C/2)-sin(A/2)*sin(B/2)*cos(C/2);
      
      }
    
     //cout<<"CZTI alignment quaternions "<<*q1<<" "<<*q2<<" "<<*q3<<" "<<*q4;   
   // *q1=1;   *q2=0;    *q3=0;   *q4=0;
    
     fits_close_file(fptr,&status);
     if(status) { fits_report_error(stderr,status);  return (EXIT_FAILURE); }
     return (EXIT_SUCCESS);
}

int getATTq(char *evtfile, char *attfile,vector<Att> &att,float interval){
    Att attitude;
    //temporarily for now
    attitude.q1=1;                 //in attitude file, last quaternion is scalar
    attitude.q2=0;
    attitude.q3=0;
    attitude.q4=0;    //must be removed in actual scenario
    att.push_back(attitude);
    LOG(INFO)<<"Attitude quaternion is taken as 1,0,0,0";
    return (EXIT_SUCCESS);
    //-------------------------------
    
    
    int status=0;
    fitsfile *fptr;
    
    //reading event file header for values of TSTART and TSTOP
    double tstart,tstop;
    double start[6],stop[6]; //to store tstart and tstop from all extensions of event file
    fits_open_file(&fptr,evtfile,READONLY,&status);
    if(status!=0){
        LOG(ERROR)<<"***Error opening file:"<<evtfile;
        return (EXIT_FAILURE);
    }
    //reading tstart and tstop from each extension
    for(int i=2;i<=5;i++){
        
        fits_movabs_hdu(fptr,i,NULL,&status);
        if(status) { fits_report_error(stderr,status);  
            LOG(ERROR)<<"***Error moving to HDU "<<i<<" in event file***";
            return (EXIT_FAILURE); }
        
        fits_read_key(fptr,TDOUBLE,"TSTART",&(start[i-2]),NULL,&status);
        if(status!=0){
            start[i-2]=9999999999999999; status=0;
        }
        fits_read_key(fptr,TDOUBLE,"TSTOP",&(stop[i-2]),NULL,&status);
        if(status!=0){
            stop[i-2]=0; status=0;
        }
    }
    fits_close_file(fptr,&status);
    if(status) { fits_report_error(stderr,status);  return (EXIT_FAILURE); }
    min<double>(start,6,&tstart);   //taking smallest tstart value of all extensions
    max<double>(stop,6,&tstop);     //taking largest tstop value of all extensions
    
    LOG(INFO)<<"Event start time:"<<tstart<<"Event stop time:"<<tstop;
   
    if(tstart>tstop) {
        LOG(ERROR)<<"***Could not get start and end time from event file***";
        return (EXIT_FAILURE);
    }
    
    //event file reading complete
    
    //Reading attitude file for attitude Qs during TSTART to TSTOP
    fits_open_file(&fptr,attfile,READONLY,&status);
    if(status) { fits_report_error(stderr,status);  return (EXIT_FAILURE); }
    
    char *extname="ATTITUDE";
    fits_movnam_hdu(fptr,BINARY_TBL,extname,0,&status);
    if(status) { 
        LOG(ERROR)<<"***Error moving to HDU ATTITUDE***";
        fits_report_error(stderr,status);  
        return (EXIT_FAILURE); 
    }
    double q[4];
    long nrows=0;
    
    fits_get_num_rows(fptr,&nrows,&status);  
    if(status) { fits_report_error(stderr,status);  return (EXIT_FAILURE); }
        
    int colnum;
    fits_get_colnum(fptr,CASEINSEN,"Q_SAT",&colnum,&status);
    if(status) { fits_report_error(stderr,status);  return (EXIT_FAILURE); }
   
    double time=0,nexttime=0;
    int i=1;
    
    char str[5];
    //reading attitude data at required intervals
    do{
        fits_read_col(fptr,TDOUBLE,colnum,i,1,4,NULL,q,NULL,&status);
        if(status) { fits_report_error(stderr,status);  return (EXIT_FAILURE); }
        fits_read_col(fptr,TDOUBLE,1,i,1,1,NULL,&time,NULL,&status);  //Time is assumed to be in column 1 of attitude file
        if(status){
            fits_report_error(stderr,status);
            LOG(ERROR)<<"***Error reading row number "<<i<<" in attitude file***";
            return (EXIT_FAILURE);
        }
        //cout<<"time:"<<time<<"";
        attitude.t=time;
        attitude.q1=q[3];                 //in attitude file, last quaternion is scalar
        attitude.q2=q[0];
        attitude.q3=q[1];
        attitude.q4=q[2];
        
        att.push_back(attitude);
        
   
        nexttime=time;
        while (((nexttime-time) < interval) && i<=nrows){
            i++; if(i>nrows) break;
            fits_read_col(fptr,TDOUBLE,1,i,1,1,NULL,&nexttime,NULL,&status);
            //cout<<"Row number:"<<i;
            //status=0;
            if(status){
                fits_report_error(stderr,status);
                LOG(ERROR)<<"***Error reading time column ***";
                return (EXIT_FAILURE);
            }
        }
        //cout<<""<<i;
      }while(i<=nrows);
      LOG(INFO)<<"Attitude Size:"<<att.size();
    if(tstart<att[0].t && tstop<att[0].t || tstart>att[att.size()-1].t && tstop>att[att.size()-1].t ){
        LOG(ERROR)<<"***Event file duration and attitude file duration are different***";
        return (EXIT_FAILURE);
    }  
    else if(tstart<att[0].t && tstop>att[att.size()-1].t) {}
    else {
        vector<Att>::iterator curr=att.end()-1;
        if(tstop<(curr->t)){
                while((curr->t)>tstop){
                    att.erase(curr);
                    curr=att.end()-1;
                }
        }
        
        curr=att.begin();
        if(tstart>att[0].t){
                while(tstart>(curr->t)){
                    att.erase(curr);
                    curr=att.begin();
                }
        }
    }
    fits_close_file(fptr,&status);
    if(status) { fits_report_error(stderr,status);  return (EXIT_FAILURE); }
    return (EXIT_SUCCESS);
}

int cztgaas::getHistory(vector<string> &vhistory){
    //char *user=getlogin();
	char *user = getenv("USER");
    vhistory.push_back("Module run by "+(string)user);
    vhistory.push_back("Parameter List START for "+(string)modulename);
    vhistory.push_back("P1 eventfile="+(string)eventfile);
    vhistory.push_back("P2 teldeffile="+(string)teldeffile);
    vhistory.push_back("P3 attitudefile="+(string)attitudefile);
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
