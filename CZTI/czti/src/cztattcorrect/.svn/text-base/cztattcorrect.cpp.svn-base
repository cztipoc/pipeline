#include<stdio.h>
#include<cstdlib>
#include<fitsio.h>
#include<string.h>
#include<pil.h>
#include<iostream>
#include<math.h>
#include "glog/logging.h"

using namespace std;

double getlagvalue(long n,double time_arr[],double value_arr[],double sectime);
long getNumrows(char *infile, int hdunum);
double searchgetlag(long nrows,long UTtimearr_length,double TIME[],double col2[],double UTtime_arr[],double outvaluecol2[]);
int searchgetlag_int(long nrows,long UTtimearr_length,double *TIME,int *col2,double UTtime_arr[],int *outvaluecol15);
int getlagvalue_int(long n, double time_arr[],int value_arr[],double sectime);
unsigned int searchgetlag_uint(long nrows,long UTtimearr_length,double *TIME,unsigned int *col2,double UTtime_arr[],unsigned int *outvaluecol15);
unsigned int getlagvalue_uint(long n, double time_arr[],unsigned int value_arr[],double sectime);

int main()
{
        char const* log_file=std::getenv("logfile_env");
    if ( log_file == NULL){
		log_file="cztattcorrect.log";
    }    
    google::InitGoogleLogging(log_file);
google::SetStderrLogging(google::INFO);
    		
	
	double *TIME=NULL,**cl2=NULL,*cl3=NULL,*cl4=NULL,*cl5=NULL,**outvaluecol2=NULL,*outvaluecol3=NULL,*outvaluecol4=NULL,*outvaluecol5=NULL;
	unsigned int *outvaluecol6=NULL;
	unsigned int *cl6=NULL;	
	char infile[PIL_LINESIZE];
	char outfile[PIL_LINESIZE];
	char History[1024];
	cout<<"\nEnter the input att file: ";
	cin>>infile;
	cout<<"\nEnter the output file name: ";
	cin>>outfile;
	cout<<"\nHistory[y/n]: ";
	cin>>History;

	long nrows=0;
	int status=0;
	long i,m,z;
	long a;
	long row;
	//double first_time;
	double last_time;
	long UTtimearr_length;
	float timedel=1.0;
	float deltat=1.0;
	double tstop;
	long tstopi;
	float tstopf;
	double *tempcol1;
	double *tempcol2;
	double *tempcol3;	
	double *tempcol4;

	double *outtempcol1;
	double *outtempcol2;
	double *outtempcol3;	
	double *outtempcol4;

	fitsfile *fptr;
	fitsfile *fout;
	//opening input file
	cout<<"\nOpening input file...";
	fits_open_file(&fptr,infile,READONLY,&status);
	if(status) { LOG(ERROR)<<"Error in opening input file: "<<infile;fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_movabs_hdu(fptr,2,NULL, &status);
	if(status) { LOG(ERROR)<<"Error in moving to HDU ";fits_report_error(stderr,status); return(EXIT_FAILURE);}

	//("input file hdu move status: %d\n ",status);
	nrows=getNumrows(infile,2);
	LOG(INFO)<<"No of Rows: "<<nrows;    
	TIME=(double*)malloc(sizeof(double)*nrows);
	cl2=(double**)malloc(sizeof(double*)*nrows);
	cl3=(double*)malloc(sizeof(double)*nrows);
	cl4=(double*)malloc(sizeof(double)*nrows);
	cl5=(double*)malloc(sizeof(double)*nrows);
	cl6=(unsigned int*)malloc(sizeof(unsigned int)*nrows);

	tempcol1=(double*)malloc(sizeof(double)*nrows);
	tempcol2=(double*)malloc(sizeof(double)*nrows);
	tempcol3=(double*)malloc(sizeof(double)*nrows);
	tempcol4=(double*)malloc(sizeof(double)*nrows);

	outtempcol1=(double*)malloc(sizeof(double)*nrows);
	outtempcol2=(double*)malloc(sizeof(double)*nrows);
	outtempcol3=(double*)malloc(sizeof(double)*nrows);
	outtempcol4=(double*)malloc(sizeof(double)*nrows);

	for(z=0;z<nrows;z++){
	cl2[z]=(double*)malloc(sizeof(double)*4);
	}
	

	outvaluecol3=(double*)malloc(sizeof(double)*nrows);
	outvaluecol4=(double*)malloc(sizeof(double)*nrows);
	outvaluecol5=(double*)malloc(sizeof(double)*nrows);
	outvaluecol6=(unsigned int*)malloc(sizeof(unsigned int)*nrows);


	// defining array after nrow variable
	double *UTtime_arr;
	UTtime_arr=(double*)malloc(sizeof(double)*nrows);
	for(i=0;i<nrows;i++){
		UTtime_arr[i]=0;
	}

	
	//column data stored in following arrays

	LOG(INFO)<<"Reading input file....";	

	fits_read_col(fptr,TDOUBLE,1,1,1,nrows,NULL,TIME,NULL,&status);
	if(status) { LOG(ERROR)<<"Error in reading Time column for file "<<infile;fits_report_error(stderr,status); return(EXIT_FAILURE);}


	for(z=0;z<nrows;z++){
	fits_read_col(fptr,TDOUBLE,2,z+1,1,4,NULL,cl2[z],NULL,&status);
	}
	if(status) { LOG(ERROR)<<"Error in reading 2nd column "<<infile;fits_report_error(stderr,status); return(EXIT_FAILURE);}


/*	for(z=0;z<nrows;z++){
	fits_read_col(fptr,TINT,6,z+1,1,1,NULL,cl6[z],NULL,&status);
	}
	if(status) { ("\nError(%s:%d):Error in reading column6 array ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
*/
	
	fits_read_col(fptr,TDOUBLE,3,1,1,nrows,NULL,cl3,NULL,&status);
	if(status) { LOG(ERROR)<<"Error in reading col3";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_read_col(fptr,TDOUBLE,4,1,1,nrows,NULL,cl4,NULL,&status);
	if(status) { LOG(ERROR)<<"Error in reading col4" ;fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_read_col(fptr,TDOUBLE,5,1,1,nrows,NULL,cl5,NULL,&status);
	if(status) {LOG(ERROR)<<"Error in reading col5 ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
//	fits_read_col(fptr,TUINT,6,1,1,nrows,NULL,cl6,NULL,&status);
//	if(status) { ("\nError(%s:%d):Error in reading col6 ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_read_col_bit_uint(fptr,6,1,nrows,1,8,cl6,&status);
	if(status) { LOG(ERROR)<<"Error in reading col6 ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	for(z=0;z<nrows;z++){
		tempcol1[z]=cl2[z][0];
		tempcol2[z]=cl2[z][1];
		tempcol3[z]=cl2[z][2];
		tempcol4[z]=cl2[z][3];
		//("\ntempcol: %lf",tempcol[z]);
	}

		
	long first_time;
	last_time=TIME[nrows-1];
	//("\nstart time is: %lf\n",first_time);

	//getting UTtime_arr array having timedel=1sec from start time+1 of orb
	first_time=(long)floor(TIME[0])+1;
	UTtime_arr[0]=first_time;  
	i=0;
	while(UTtime_arr[i]<last_time){
		i++;
		UTtime_arr[i]=first_time+i;
	}
	if(UTtime_arr[i]>last_time){
		i--;
	}
	//("\n first element in UTtime_arr:%lf",UTtime_arr[0]);
	UTtimearr_length=i+1;
	

	
	//testing
	//long size;
	//size=sizeof(UTtime_arr)/sizeof(UTtime_arr[0]);
	//("\nlength of UTtime_arr array is : %ld",size);
	
	LOG(INFO)<<"Searching Times and interpolating......";
	searchgetlag(nrows,UTtimearr_length,TIME,tempcol1,UTtime_arr,outtempcol1);
	searchgetlag(nrows,UTtimearr_length,TIME,tempcol2,UTtime_arr,outtempcol2);
	searchgetlag(nrows,UTtimearr_length,TIME,tempcol3,UTtime_arr,outtempcol3);
	searchgetlag(nrows,UTtimearr_length,TIME,tempcol4,UTtime_arr,outtempcol4);
	searchgetlag(nrows,UTtimearr_length,TIME,cl3,UTtime_arr,outvaluecol3);
	searchgetlag(nrows,UTtimearr_length,TIME,cl4,UTtime_arr,outvaluecol4);
	searchgetlag(nrows,UTtimearr_length,TIME,cl5,UTtime_arr,outvaluecol5);
	searchgetlag_uint(nrows,UTtimearr_length,TIME,cl6,UTtime_arr,outvaluecol6);
	for(z=0;z<nrows;z++){
		cl2[z][0]=tempcol1[z];
		cl2[z][1]=tempcol2[z];
		cl2[z][2]=tempcol3[z];
		cl2[z][3]=tempcol4[z];
		//("\ntempcol: %lf",tempcol[z]);
	}

	
	//creating new fits file
	LOG(INFO)<<"Creating output file.....";
	fits_create_file(&fout,outfile,&status);
	if(status) { LOG(ERROR)<<"Error in creating output file "<<outfile;fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_movabs_hdu(fptr,1,NULL, &status);
	if(status) { LOG(ERROR)<<"Error in moving to HDU ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_copy_hdu(fptr, fout,NULL,&status);
	if(status) { LOG(ERROR)<<"Error in coping to HDU ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_update_key(fout,TFLOAT,"TIMEDEL",&timedel,NULL,&status);
	if(status) { LOG(ERROR)<<"Error in updating TIMEDEL key ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_update_key(fout,TFLOAT,"DELTAT",&deltat,NULL,&status);
	if(status) { LOG(ERROR)<<"Error in updating DELTAT key ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_read_key(fptr,TLONG,"TSTOPI",&tstopi,NULL,&status);
	if(status) { LOG(ERROR)<<"Error in updating TSTOPI key ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_read_key(fptr,TFLOAT,"TSTOPF",&tstopf,NULL,&status);
	if(status) { LOG(ERROR)<<"Error in updating TSTOPF key ";fits_report_error(stderr,status); return(EXIT_FAILURE);}

	tstop=tstopi+tstopf;
	//cout<<"\ntstop: "<<tstop;
	fits_update_key(fptr,TDOUBLE,"TSTOP",&tstop,"observation stop time",&status);
	if(status) {  LOG(ERROR)<<"Error in updating TSTOP key ";fits_report_error(stderr,status); return(EXIT_FAILURE);}


	//fits_movabs_hdu(fout,2,NULL, &status);
	cout<<"\nCreating BINTABLE HDU.....";
	char *ttype[6]={"TIME","Q_SAT","Roll_RA","Roll_DEC","Roll_ROT","ATD_STATUS"};
	char *tform[6]={"D","4D","D","D","D","8X"};
	char *tunit[6]={"s"," ","deg","deg","deg"," "};
	//char *ext[10]="ORBIT";
	const char *extname="ATTITUDE";
	//extname=ext;
	
	fits_create_tbl(fout, BINARY_TBL, 0, 6, ttype, tform, tunit,extname,&status);
	if(status) { LOG(ERROR)<<"Error in creating table ";fits_report_error(stderr,status); return(EXIT_FAILURE);}

	//writing interpolated values to columns

	fits_write_col(fout,TDOUBLE,1,1,1,UTtimearr_length,UTtime_arr,&status);
	if(status) { LOG(ERROR)<<"Error in writing col1 ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	for(z=0;z<UTtimearr_length;z++){
		fits_write_col(fout,TDOUBLE,2,z+1,1,4,cl2[z],&status);
	}
	if(status) { LOG(ERROR)<<"Error in writing col2 array ";fits_report_error(stderr,status); return(EXIT_FAILURE);}	
	fits_write_col(fout,TDOUBLE,3,1,1,UTtimearr_length,outvaluecol3,&status);
	if(status) {LOG(ERROR)<<"Error in writing col3 ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_write_col(fout,TDOUBLE,4,1,1,UTtimearr_length,outvaluecol4,&status);
	if(status) {LOG(ERROR)<<"Error in writing col4 ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_write_col(fout,TDOUBLE,5,1,1,UTtimearr_length,outvaluecol5,&status);
	if(status) {LOG(ERROR)<<"Error in writing col5 ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_write_col(fout,TBYTE,6,1,1,UTtimearr_length,outvaluecol6,&status);
	if(status) {LOG(ERROR)<<"Error in writing col6 ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_close_file(fout,&status);
	if(status) {LOG(ERROR)<<"Error in closing output file: "<<outfile;fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_open_file(&fout,outfile,READWRITE,&status);
	if(status) {LOG(ERROR)<<"Error in opening file";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_movabs_hdu(fout,2,NULL, &status);
	if(status) {LOG(ERROR)<<"Error in moving to HDU ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_write_key(fout,TFLOAT,"TIMEDEL",&timedel,"sampling interval in seconds",&status);
	if(status) {LOG(ERROR)<<"Error in updating  TIMEDEL key";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_write_key(fout,TFLOAT,"DELTAT",&deltat,"sampling interval in seconds",&status);
	if(status) {LOG(ERROR)<<"Error in updating DELTAT key ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	
	char str[80];
	char comment[80];
	float fkeyval;
	cout<<"\nModifing primary header and extension........";
	//fptr pointing primary header and fout pointing towards second extension of fout
	fits_read_card(fptr,"INSTRUME",str,&status);
	fits_write_record(fout,str,&status);
	fits_read_card(fptr,"DATE-OBS",str,&status);
	fits_write_record(fout,str,&status);
	fits_read_card(fptr,"TIME-OBS",str,&status);
	fits_write_record(fout,str,&status);
	fits_read_card(fptr,"DATE-END",str,&status);
	fits_write_record(fout,str,&status);
	fits_read_card(fptr,"TIME-END",str,&status);
	fits_write_record(fout,str,&status);
	fits_read_card(fptr,"TIMESYS",str,&status);
	fits_write_record(fout,str,&status);
	fits_read_card(fptr,"MJDREFI",str,&status);
	fits_write_record(fout,str,&status);
	fits_read_card(fptr,"MJDREFF",str,&status);
	fits_write_record(fout,str,&status);
	fits_read_card(fptr,"TIMEZERI",str,&status);
	fits_write_record(fout,str,&status);
	fits_read_card(fptr,"TIMEZERF",str,&status);
	fits_write_record(fout,str,&status);
	fits_read_card(fptr,"TIMEUNIT",str,&status);
	fits_write_record(fout,str,&status);
	fits_read_card(fptr,"TSTART",str,&status);
	fits_write_record(fout,str,&status);
	fits_read_card(fptr,"TSTARTI",str,&status);
	fits_write_record(fout,str,&status);
	fits_read_card(fptr,"TSTARTF",str,&status);
	fits_write_record(fout,str,&status);
	//fits_read_card(fptr,"TSTOP",str,&status);
	//fits_write_record(fout,str,&status);
	fits_read_card(fptr,"TSTOPI",str,&status);
	fits_write_record(fout,str,&status);
	fits_read_card(fptr,"TSTOPF",str,&status);
	fits_write_record(fout,str,&status);
	fits_read_card(fptr,"OBJECT",str,&status);
	fits_write_record(fout,str,&status);
	
	fits_read_key(fptr,TFLOAT,"RA_PNT",&fkeyval,comment,&status);
	fits_write_key(fout,TFLOAT,"RA",&fkeyval,comment,&status);
	fits_read_key(fptr,TFLOAT,"DEC_PNT",&fkeyval,comment,&status);
	fits_write_key(fout,TFLOAT,"DEC",&fkeyval,comment,&status);

	fits_read_card(fptr,"EQUINOX",str,&status);
	fits_write_record(fout,str,&status);
	fits_read_card(fptr,"RADECSYS",str,&status);
	fits_write_record(fout,str,&status);
	
	fits_read_card(fptr,"OBSERVER",str,&status);
	fits_write_record(fout,str,&status);
	fits_read_card(fptr,"OBS_ID",str,&status);
	fits_write_record(fout,str,&status);

	//fits_read_card(fptr,"HDUVERS",str,&status);
	//fits_write_record(fout,str,&status);
	//fits_read_card(fptr,"HDUCLAS1",str,&status);
	//fits_write_record(fout,str,&status);
	//fits_read_card(fptr,"LONGSTRN",str,&status);
	//fits_write_record(fout,str,&status);
	


	double zero=0.0;
	long end_time=(long)UTtime_arr[UTtimearr_length-1];
	fits_update_key(fout,TDOUBLE,"TSTART",&UTtime_arr[0],"Start time",&status);
	if(status) {LOG(ERROR)<<"Error in updating TSTART key ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_update_key(fout,TLONG,"TSTARTI",&first_time,"integer portion of start time",&status);
	if(status) {LOG(ERROR)<<"Error in updating TSTARTI key ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_update_key(fout,TDOUBLE,"TSTARTF",&zero,"fractional observation start time",&status);
	if(status) {LOG(ERROR)<<"Error in updating TSTARTF key ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_update_key(fout,TDOUBLE,"TSTOP",&UTtime_arr[UTtimearr_length-1],"Stop time",&status);
	if(status) {LOG(ERROR)<<"Error in updating TSTOP key ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_update_key(fout,TLONG,"TSTOPI",&end_time,"integer observation stop time",&status);
	if(status) {LOG(ERROR)<<"Error in updating TSTOPI key ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_update_key(fout,TDOUBLE,"TSTOPF",&zero,"fractional observation stop time",&status);
	if(status) {LOG(ERROR)<<"Error in updating TSTOPF key ";fits_report_error(stderr,status); return(EXIT_FAILURE);}


	fits_write_chksum(fout,&status);
	if(status) {LOG(ERROR)<<"Error in writing checksum ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_movabs_hdu(fout,1,NULL,&status);
	if(status) {LOG(ERROR)<<"Error in moving to HDU ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_update_key(fout,TDOUBLE,"TSTART",&UTtime_arr[0],"Start time",&status);
	if(status) {LOG(ERROR)<<"Error in updating TSTART  key ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_update_key(fout,TLONG,"TSTARTI",&first_time,"integer portion of start time",&status);
	if(status) {LOG(ERROR)<<"Error in updating TSTARTI key ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_update_key(fout,TDOUBLE,"TSTARTF",&zero,"fractional observation start time",&status);
	if(status) {LOG(ERROR)<<"Error in updating TSTARTF key ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_update_key(fout,TDOUBLE,"TSTOP",&UTtime_arr[UTtimearr_length-1],"Stop time",&status);
	if(status) {LOG(ERROR)<<"Error in updating TSTOP key ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_update_key(fout,TLONG,"TSTOPI",&end_time,"integer observation stop time",&status);
	if(status) {LOG(ERROR)<<"Error in updating TSTOPI key ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_update_key(fout,TDOUBLE,"TSTOPF",&zero,"fractional observation stop time",&status);
	if(status) {LOG(ERROR)<<"Error in updating TSTOPF key ";fits_report_error(stderr,status); return(EXIT_FAILURE);}

	fits_write_chksum(fout,&status);
	if(status) {LOG(ERROR)<<"Error in writing checksum ";fits_report_error(stderr,status); return(EXIT_FAILURE);}

	if((*History=='y') && status==0){
		//cout<<"\n####writing history";
		fits_movabs_hdu(fout,1,NULL,&status);
		if(status) {LOG(ERROR)<<"Error in moving to Extension ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
		char history[1024]="executed cztattcorrect(att interpolation)";
		fits_write_history(fout, history,&status);
		if(status) {LOG(ERROR)<<"Error in writting history to primary HDU ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
}


	//closing all fits file
	fits_close_file(fout,&status);
	if(status) {LOG(ERROR)<<"Error in closing file  "<<outfile;fits_report_error(stderr,status); return(EXIT_FAILURE);}
	//fits_close_file(fptr,&status);
	//if(status) {LOG(ERROR)<<"Error in closing file  "<<infile;fits_report_error(stderr,status); return(EXIT_FAILURE);}	

	delete[] TIME;
	delete[] cl2;
	delete[] cl3;
	delete[] cl4;
	delete[] cl5;
	delete[] cl6;
	delete[] tempcol1;
	delete[] tempcol2;
	delete[] tempcol3;
	delete[] tempcol4;
	delete[] outtempcol1;
	delete[] outtempcol2;
	delete[] outtempcol3;
	delete[] outtempcol4;
	cout<<"\n#######INTERPOLATION COMPLETED......";
}
	
double searchgetlag(long nrows,long UTtimearr_length,double *TIME,double *col2,double UTtime_arr[],double *outvaluecol15){
       	long j;
	long index,index_new,i;
	double nearer[4];
	double nearer_value[4];
	
	for(j=0;j<UTtimearr_length;j++){
	if(j==0){
		index=0;
	}
	
	index_new=index;
	while(TIME[index_new]<=UTtime_arr[j]){
		index_new++;
	}
	index=index_new;
	//("########found element at: %ld",index);
	
	nearer[0]=TIME[index-2];
	nearer[1]=TIME[index-1];
	nearer[2]=TIME[index];
	nearer[3]=TIME[index+1];
	//for(i=0;i<4;i++){
	//	("\n####%lf",nearer[i]);
	//}
	

	if(TIME[index]-TIME[index-1]>100 || TIME[index+1]-TIME[index]>100){
		outvaluecol15[j]=col2[index];
		//cout<<"\nGAp FOUND..###";
	}		

	else{
	nearer_value[0]=col2[index-2];
	nearer_value[1]=col2[index-1];
	nearer_value[2]=col2[index];
	nearer_value[3]=col2[index+1];

	//for(i=0;i<4;i++){
	//	("\nindex: %d",i);
	//	("\nvalue#%lf",nearer_value[i]);
	//}
				
	//getiing lagren value array 
	//("\nUTTIME value to be interpolated is: %lf\n",UTtime_arr[j]);
	
	outvaluecol15[j]=getlagvalue(4,nearer,nearer_value,UTtime_arr[j]);
	//("\n\n The respective outvaluecol2 of the variable value_arr is: %lf",outvaluecol2[j-1]);
	}
	}
}




double getlagvalue(long n, double time_arr[],double value_arr[],double sectime)
{	
	long b,length;
	
	//length=(sizeof(sectime)/sizeof(double));
	//length=2;
	//for(b=0;b<length;b++){
	int i,j;
	double s,t,k;
	s=1;
	t=1;
	k=0;	
        for(i=0; i<n; i++)
        {
            s=1;
            t=1;
            for(j=0; j<n; j++)
            {
                if(j!=i)
                {
                    s=s*(sectime-time_arr[j]);
                    t=t*(time_arr[i]-time_arr[j]);
                }
            }
            k=k+((s/t)*value_arr[i]);
        }
        //("\n\n The respective value of the variable value_arr is: %lf",k);
	return k;
}

int searchgetlag_int(long nrows,long UTtimearr_length,double *TIME,int *col2,double UTtime_arr[],int *outvaluecol15){
       	long j;
	long index,index_new,i;
	double nearer[4];
	int nearer_value[4];
	
	for(j=0;j<UTtimearr_length;j++){
	if(j==0){
		index=0;
	}
	
	index_new=index;
	while(TIME[index_new]<=UTtime_arr[j]){
		index_new++;
	}
	index=index_new;
	//("########found element at: %ld",index);
	
	nearer[0]=TIME[index-2];
	nearer[1]=TIME[index-1];
	nearer[2]=TIME[index];
	nearer[3]=TIME[index+1];
	/*for(i=0;i<4;i++){
		("\n####%lf",nearer[i]);
	}
	*/
	nearer_value[0]=col2[index-2];
	nearer_value[1]=col2[index-1];
	nearer_value[2]=col2[index];
	nearer_value[3]=col2[index+1];

/*	for(i=0;i<UTtimearr_length;i++){
		("\n####%lf",nearer[i]);
		("\nindex: %d",i);
		("\nvalue#%lf",nearer_value[i]);
	}
				
*/	//getting lagren value array 
	//("\nUTTIME value to be interpolated is: %lf\n",UTtime_arr[j]);
	
	outvaluecol15[j]=getlagvalue_int(4,nearer,nearer_value,UTtime_arr[j]);
	//("\n\n The respective outvaluecol2 of the variable value_arr is: %lf",outvaluecol2[j-1]);

	}
}


int getlagvalue_int(long n, double time_arr[],int value_arr[],double sectime)
{	
	long b,length;
	
	//length=(sizeof(sectime)/sizeof(double));
	//length=2;
	//for(b=0;b<length;b++){
	int i,j;
	double s,t,k;
	s=1;
	t=1;
	k=0;	
        for(i=0; i<n; i++)
        {
            s=1;
            t=1;
            for(j=0; j<n; j++)
            {
                if(j!=i)
                {
                    s=s*(sectime-time_arr[j]);
                    t=t*(time_arr[i]-time_arr[j]);
                }
            }
            k=k+((s/t)*value_arr[i]);
        }
        //("\n\n The respective value of the variable value_arr is: %lf",k);
	return k;
}

unsigned int searchgetlag_uint(long nrows,long UTtimearr_length,double *TIME,unsigned int *col2,double UTtime_arr[],unsigned int *outvaluecol15){
    long j;
	long index,index_new,i;
	double nearer[4];
	unsigned int nearer_value[4];
	
	for(j=0;j<UTtimearr_length;j++){
	if(j==0){
		index=0;
	}
	
	index_new=index;
	while(TIME[index_new]<=UTtime_arr[j]){
		index_new++;
	}
	index=index_new;
	//("########found element at: %ld",index);
	
	nearer[0]=TIME[index-2];
	nearer[1]=TIME[index-1];
	nearer[2]=TIME[index];
	nearer[3]=TIME[index+1];
	/*for(i=0;i<4;i++){
		("\n####%lf",nearer[i]);
	}
	*/
	nearer_value[0]=col2[index-2];
	nearer_value[1]=col2[index-1];
	nearer_value[2]=col2[index];
	nearer_value[3]=col2[index+1];

	/*for(i=0;i<4;i++){
		("\nindex: %d",i);
		("\nvalue#%lf",nearer_value[i]);
	}
	*/			
	//getiing lagren value array 
	//("\nUTTIME value to be interpolated is: %lf\n",UTtime_arr[j]);
	
	outvaluecol15[j]=getlagvalue_uint(4,nearer,nearer_value,UTtime_arr[j]);
	//("\n\n The respective outvaluecol2 of the variable value_arr is: %lf",outvaluecol2[j-1]);

	}
}


unsigned int getlagvalue_uint(long n, double time_arr[],unsigned int value_arr[],double sectime)
{	
	long b,length;
	
	//length=(sizeof(sectime)/sizeof(double));
	//length=2;
	//for(b=0;b<length;b++){
	int i,j;
	double s,t,k;
	s=1;
	t=1;
	k=0;	
        for(i=0; i<n; i++)
        {
            s=1;
            t=1;
            for(j=0; j<n; j++)
            {
                if(j!=i)
                {
                    s=s*(sectime-time_arr[j]);
                    t=t*(time_arr[i]-time_arr[j]);
                }
            }
            k=k+((s/t)*value_arr[i]);
        }
        //("\n\n The respective value of the variable value_arr is: %lf",k);
	return k;
}


long getNumrows(char *infile, int hdunum)
{
	long numrows;
	int status=0;
	fitsfile *fptr;
	fits_open_file(&fptr,infile,READONLY, &status);
	//move hdu
	fits_movabs_hdu(fptr,hdunum,NULL,&status);
	fits_get_num_rows(fptr,&numrows,&status);
	fits_close_file(fptr,&status);

	return numrows;
}

