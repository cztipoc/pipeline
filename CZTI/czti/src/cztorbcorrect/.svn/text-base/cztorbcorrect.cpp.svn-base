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
double searchgetlag(long UTtimearr_length,double TIME[],float col2[],double UTtime_arr[],double outvaluecol2[]);

int main()
{	
        char const* log_file=std::getenv("logfile_env");
    if ( log_file == NULL){
		log_file="cztorbcorrect.log";
    }    
    google::InitGoogleLogging(log_file);
google::SetStderrLogging(google::INFO);
    		

	char infile[PIL_LINESIZE];
	char outfile[PIL_LINESIZE];
	char History[1024]="y";

	cout<<"\nEnter the input orb file: ";
	cin>>infile;
	cout<<"\nEnter the output file name: ";
	cin>>outfile;
	cout<<"\nHistory[y/n]: ";
	cin>>History;

	long nrows;
	int status=0;
	long i,m,k;
	long a;
	long row;
	//double first_time;
	double last_time;
	long UTtimearr_length;
	float timedel=1.0;
	float deltat=1.0;

	fitsfile *fptr;
	fitsfile *fout;
	double *TIME;
	float *cl2;
	float *cl3;
	float *cl4;
	float *cl5;
	float *cl6;
	float *cl7;
	float *cl8;
	float *cl9;
	float *cl10;
	float *cl11;
	float *cl12;
	float *cl13;
	float *cl14;
	float *cl15;
	float *cl16;
	double *outvaluecol2;
	double *outvaluecol3;
	double *outvaluecol4;
	double *outvaluecol5;
	double *outvaluecol6;
	double *outvaluecol7;
	double *outvaluecol8;
	double *outvaluecol9;
	double *outvaluecol10;
	double *outvaluecol11;
	double *outvaluecol12;
	double *outvaluecol13;
	double *outvaluecol14;
	double *outvaluecol15;
	double *outvaluecol16;
	double *UTtime_arr;
	//opening input file
	cout<<"\n Opening input file";
	fits_open_file(&fptr,infile,READONLY,&status);
        if(status) {LOG(ERROR)<<"Error in opening input file "<<infile;fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_movabs_hdu(fptr,2,NULL, &status);
        if(status) {LOG(ERROR)<<"Error in moving 2nd extension of file "<<infile;fits_report_error(stderr,status); return(EXIT_FAILURE);}        
	nrows=getNumrows(infile,2);
	LOG(INFO)<<"NO of rows: "<<nrows;
	// allocating dynamic memory
	TIME=(double*)malloc(sizeof(double)*nrows);
	UTtime_arr=(double*)calloc(nrows,sizeof(double));
	outvaluecol2=(double*)malloc(sizeof(double)*nrows);
	outvaluecol3=(double*)malloc(sizeof(double)*nrows);
	outvaluecol4=(double*)malloc(sizeof(double)*nrows);
	outvaluecol5=(double*)malloc(sizeof(double)*nrows);
	outvaluecol6=(double*)malloc(sizeof(double)*nrows);
	outvaluecol7=(double*)malloc(sizeof(double)*nrows);
	outvaluecol8=(double*)malloc(sizeof(double)*nrows);
	outvaluecol9=(double*)malloc(sizeof(double)*nrows);
	outvaluecol10=(double*)malloc(sizeof(double)*nrows);
	outvaluecol11=(double*)malloc(sizeof(double)*nrows);
	outvaluecol12=(double*)malloc(sizeof(double)*nrows);
	outvaluecol13=(double*)malloc(sizeof(double)*nrows);
	outvaluecol14=(double*)malloc(sizeof(double)*nrows);
	outvaluecol15=(double*)malloc(sizeof(double)*nrows);
	outvaluecol16=(double*)malloc(sizeof(double)*nrows);

	cl2=(float*)malloc(sizeof(float)*nrows);
	cl3=(float*)malloc(sizeof(float)*nrows);
	cl4=(float*)malloc(sizeof(float)*nrows);
	cl5=(float*)malloc(sizeof(float)*nrows);
	cl6=(float*)malloc(sizeof(float)*nrows);
	cl7=(float*)malloc(sizeof(float)*nrows);
	cl8=(float*)malloc(sizeof(float)*nrows);
	cl9=(float*)malloc(sizeof(float)*nrows);
	cl10=(float*)malloc(sizeof(float)*nrows);
	cl11=(float*)malloc(sizeof(float)*nrows);
	cl12=(float*)malloc(sizeof(float)*nrows);
	cl13=(float*)malloc(sizeof(float)*nrows);
	cl14=(float*)malloc(sizeof(float)*nrows);
	cl15=(float*)malloc(sizeof(float)*nrows);
	cl16=(float*)malloc(sizeof(float)*nrows);

	LOG(INFO)<<"Reading Input file";
	fits_read_col(fptr,TDOUBLE,1,1,1,nrows,NULL,TIME,NULL,&status);
	fits_read_col(fptr,TFLOAT,2,1,1,nrows,NULL,cl2,NULL,&status);
	fits_read_col(fptr,TFLOAT,3,1,1,nrows,NULL,cl3,NULL,&status);
	fits_read_col(fptr,TFLOAT,4,1,1,nrows,NULL,cl4,NULL,&status);
	fits_read_col(fptr,TFLOAT,5,1,1,nrows,NULL,cl5,NULL,&status);
	fits_read_col(fptr,TFLOAT,6,1,1,nrows,NULL,cl6,NULL,&status);
	fits_read_col(fptr,TFLOAT,7,1,1,nrows,NULL,cl7,NULL,&status);	
	fits_read_col(fptr,TFLOAT,8,1,1,nrows,NULL,cl8,NULL,&status);
	fits_read_col(fptr,TFLOAT,9,1,1,nrows,NULL,cl9,NULL,&status);
	fits_read_col(fptr,TFLOAT,10,1,1,nrows,NULL,cl10,NULL,&status);
	fits_read_col(fptr,TFLOAT,11,1,1,nrows,NULL,cl11,NULL,&status);
	fits_read_col(fptr,TFLOAT,12,1,1,nrows,NULL,cl12,NULL,&status);
	fits_read_col(fptr,TFLOAT,13,1,1,nrows,NULL,cl13,NULL,&status);
	fits_read_col(fptr,TFLOAT,14,1,1,nrows,NULL,cl14,NULL,&status);
	fits_read_col(fptr,TFLOAT,15,1,1,nrows,NULL,cl15,NULL,&status);	
	fits_read_col(fptr,TFLOAT,16,1,1,nrows,NULL,cl16,NULL,&status);
	if(status) {fits_report_error(stderr,status); return(EXIT_FAILURE);}

		
	long first_time;
	last_time=TIME[nrows-1];

	//getting UTtime_arr array having timedel=1sec from start time+1 of orb
	first_time=(long)floor(TIME[0]);
	UTtime_arr[0]=first_time;
	LOG(INFO)<<"START TIME: "<<UTtime_arr[0];
	i=0;
	while(UTtime_arr[i]<last_time){
		i++;
		UTtime_arr[i]=first_time+i;
	}
	if(UTtime_arr[i]>last_time){
		i--;
	}
	UTtimearr_length=i+1;


	LOG(INFO)<<"Searching Times and interpolating......";
	searchgetlag(UTtimearr_length,TIME,cl2,UTtime_arr,outvaluecol2);
	searchgetlag(UTtimearr_length,TIME,cl3,UTtime_arr,outvaluecol3);
	searchgetlag(UTtimearr_length,TIME,cl4,UTtime_arr,outvaluecol4);
	searchgetlag(UTtimearr_length,TIME,cl5,UTtime_arr,outvaluecol5);
	searchgetlag(UTtimearr_length,TIME,cl6,UTtime_arr,outvaluecol6);
	searchgetlag(UTtimearr_length,TIME,cl7,UTtime_arr,outvaluecol7);
	searchgetlag(UTtimearr_length,TIME,cl8,UTtime_arr,outvaluecol8);
	searchgetlag(UTtimearr_length,TIME,cl9,UTtime_arr,outvaluecol9);
	searchgetlag(UTtimearr_length,TIME,cl10,UTtime_arr,outvaluecol10);
	searchgetlag(UTtimearr_length,TIME,cl11,UTtime_arr,outvaluecol11);
	searchgetlag(UTtimearr_length,TIME,cl12,UTtime_arr,outvaluecol12);
	searchgetlag(UTtimearr_length,TIME,cl13,UTtime_arr,outvaluecol13);
	searchgetlag(UTtimearr_length,TIME,cl14,UTtime_arr,outvaluecol14);
	searchgetlag(UTtimearr_length,TIME,cl15,UTtime_arr,outvaluecol15);
	searchgetlag(UTtimearr_length,TIME,cl16,UTtime_arr,outvaluecol16);
	
	//filter the spikes in longitude and arg of perigee
	LOG(INFO)<<"filtering the spikes in longitude and arg of perigee";
	for(k=0;k<UTtimearr_length;k++){
		if(fabs(outvaluecol9[k])>180){
			if(outvaluecol9[k]>180){
				outvaluecol9[k]=180;
			}
			if(outvaluecol9[k]<-180){
				outvaluecol9[k]=-180;
			}	
		}
		if(outvaluecol14[k]>360 || outvaluecol14[k]<0){
			if(outvaluecol14[k]>360){
				outvaluecol14[k]=360;
			}
			if(outvaluecol14[k]<0){
				outvaluecol14[k]=0;
			}
		}	
	}
	//creating new fits file
	LOG(INFO)<<"Creating output file..... "<<outfile;
	fits_create_file(&fout,outfile,&status);
	if(status) {LOG(ERROR)<<"Error in creating output file "<<outfile;fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_movabs_hdu(fptr,1,NULL, &status);
	if(status) {LOG(ERROR)<<"Error in moving 1st extension of "<<infile;fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_copy_hdu(fptr, fout,NULL,&status);
	if(status) {LOG(ERROR)<<"Error in coping hdu to output file "<<outfile;fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_update_key(fout,TFLOAT,"TIMEDEL",&timedel,NULL,&status);
	if(status) {LOG(ERROR)<<"Error in updating key in output file "<<outfile;fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_update_key(fout,TFLOAT,"DELTAT",&deltat,NULL,&status);
	if(status) {LOG(ERROR)<<"Error in updating key in output file "<<outfile;fits_report_error(stderr,status); return(EXIT_FAILURE);}
	


	//fits_movabs_hdu(fout,2,NULL, &status);
	LOG(INFO)<<"Creating BINTABLE HDU.....";
	char *ttype[16]={"TIME","X","Y","Z","Vx","Vy","Vz","Latitude","Longitude","Altitude","SemiMajor_Axis","Ecentricity","Inclination","ArgOfPerigee","RAAN","TrueAnomaly"};
	char *tform[16]={"D","D","D","D","D","D","D","D","D","D","D","D","D","D","D","D",};
	char *tunit[16]={"s","km","km","km","km/s","km/s","km/s","deg","deg","km","km"," ","deg","deg","deg","deg"};
	//char *ext[10]="ORBIT";
	const char *extname="ORBIT";
	//extname=ext;
	
	fits_create_tbl(fout, BINARY_TBL, 0, 16, ttype, tform, tunit,extname,&status);
	if(status) {LOG(ERROR)<<"Error in Creating BinTable";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	
	//writing interpolated values to columns
	fits_write_col(fout,TDOUBLE,1,1,1,UTtimearr_length,UTtime_arr,&status);
	fits_write_col(fout,TDOUBLE,2,1,1,UTtimearr_length,outvaluecol2,&status);
	fits_write_col(fout,TDOUBLE,3,1,1,UTtimearr_length,outvaluecol3,&status);
	fits_write_col(fout,TDOUBLE,4,1,1,UTtimearr_length,outvaluecol4,&status);
	fits_write_col(fout,TDOUBLE,5,1,1,UTtimearr_length,outvaluecol5,&status);
	fits_write_col(fout,TDOUBLE,6,1,1,UTtimearr_length,outvaluecol6,&status);
	fits_write_col(fout,TDOUBLE,7,1,1,UTtimearr_length,outvaluecol7,&status);
	fits_write_col(fout,TDOUBLE,8,1,1,UTtimearr_length,outvaluecol8,&status);
	fits_write_col(fout,TDOUBLE,9,1,1,UTtimearr_length,outvaluecol9,&status);
	fits_write_col(fout,TDOUBLE,10,1,1,UTtimearr_length,outvaluecol10,&status);
	fits_write_col(fout,TDOUBLE,11,1,1,UTtimearr_length,outvaluecol11,&status);
	fits_write_col(fout,TDOUBLE,12,1,1,UTtimearr_length,outvaluecol12,&status);
	fits_write_col(fout,TDOUBLE,13,1,1,UTtimearr_length,outvaluecol13,&status);
	fits_write_col(fout,TDOUBLE,14,1,1,UTtimearr_length,outvaluecol14,&status);
	fits_write_col(fout,TDOUBLE,15,1,1,UTtimearr_length,outvaluecol15,&status);
	fits_write_col(fout,TDOUBLE,16,1,1,UTtimearr_length,outvaluecol16,&status);

	if(status) {LOG(ERROR)<<"Error in writing columns in file "<<outfile;fits_report_error(stderr,status); return(EXIT_FAILURE);}

	//printf("\n#### column status is %d ",status);
	fits_close_file(fout,&status);
	if(status) {LOG(ERROR)<<"Error in closing file "<<outfile;fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_open_file(&fout,outfile,READWRITE,&status);
	fits_movabs_hdu(fout,2,NULL, &status);
	fits_write_key(fout,TFLOAT,"TIMEDEL",&timedel,"sampling interval in seconds",&status);
	fits_write_key(fout,TFLOAT,"DELTAT",&deltat,"sampling interval in seconds",&status);


	char str[80];
	char comment[80];
	float fkeyval;
	LOG(INFO)<<"Modifing primary header and extension........";
	fits_read_card(fptr,"TELESCOP",str,&status);
	fits_write_record(fout,str,&status);
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
	fits_read_card(fptr,"TSTOP",str,&status);
	fits_write_record(fout,str,&status);
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

	double zero=0.0;
	long end_time=(long)UTtime_arr[UTtimearr_length-1];
	fits_update_key(fout,TDOUBLE,"TSTART",&UTtime_arr[0],"Start time",&status);
	fits_update_key(fout,TLONG,"TSTARTI",&first_time,"integer portion of start time",&status);
	fits_update_key(fout,TDOUBLE,"TSTARTF",&zero,"fractional observation start time",&status);
	fits_update_key(fout,TDOUBLE,"TSTOP",&UTtime_arr[UTtimearr_length-1],"Stop time",&status);
	fits_update_key(fout,TLONG,"TSTOPI",&end_time,"integer observation stop time",&status);
	fits_update_key(fout,TDOUBLE,"TSTOPF",&zero,"fractional observation stop time",&status);


	fits_write_chksum(fout,&status);
	fits_movabs_hdu(fout,1,NULL,&status);
	fits_update_key(fout,TDOUBLE,"TSTART",&UTtime_arr[0],"Start time",&status);
	fits_update_key(fout,TLONG,"TSTARTI",&first_time,"integer portion of start time",&status);
	fits_update_key(fout,TDOUBLE,"TSTARTF",&zero,"fractional observation start time",&status);
	fits_update_key(fout,TDOUBLE,"TSTOP",&UTtime_arr[UTtimearr_length-1],"Stop time",&status);
	fits_update_key(fout,TLONG,"TSTOPI",&end_time,"integer observation stop time",&status);
	fits_update_key(fout,TDOUBLE,"TSTOPF",&zero,"fractional observation stop time",&status);

	fits_write_chksum(fout,&status);
	if((*History=='y') && status==0){
		//cout<<"\n####writing history";
		fits_movabs_hdu(fout,1,NULL,&status);
		if(status) { LOG(ERROR)<<"Error in moving to Extension in file"<<outfile;fits_report_error(stderr,status); return(EXIT_FAILURE);}
		char history[1024]="executed cztorbcorrect (orbit file interpolation every 1sec interval)";
		fits_write_history(fout, history,&status);
		if(status) { LOG(ERROR)<<"Error in writting history to primary HDU ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	}



	//closing all fits file
	fits_close_file(fout,&status);
	if(status) { LOG(ERROR)<<"Error in closing file "<<outfile;fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_close_file(fptr,&status);
	if(status) { LOG(ERROR)<<"Error in closing file "<<infile;fits_report_error(stderr,status); return(EXIT_FAILURE);}
	LOG(INFO)<<"INTERPOLATION COMPLETED......";


}
	
double searchgetlag(long UTtimearr_length,double TIME[],float col2[],double UTtime_arr[],double outvaluecol15[]){
       	
	long index,index_new,f,e;
	double nearer[4];
	double nearer_value[4];
	
	for(f=0;f<UTtimearr_length;f++){
	if(f==0){
		index=0;
	}
	
	index_new=index;
	while(TIME[index_new]<=UTtime_arr[f]){
		index_new++;
	}
	index=index_new;
	//printf("\nfound element at: %ld",index);
	

	if(TIME[index]-TIME[index-1]>100 || TIME[index+1]-TIME[index]>100){
		outvaluecol15[f]=col2[index];
		//cout<<"\nGAp FOUND..###";
	}		

	if(!(TIME[index]-TIME[index-1]>100 || TIME[index+1]-TIME[index]>100)){
		if(index!=0){
			nearer[0]=TIME[index-2];
			nearer[1]=TIME[index-1];
			nearer[2]=TIME[index];
			nearer[3]=TIME[index+1];

			nearer_value[0]=col2[index-2];
			nearer_value[1]=col2[index-1];
			nearer_value[2]=col2[index];
			nearer_value[3]=col2[index+1];
		}
		if(index==0){
			nearer[0]=TIME[index];
			nearer[1]=TIME[index+1];
			nearer[2]=TIME[index+2];
			nearer[3]=TIME[index+3];

			nearer_value[0]=col2[index];
			nearer_value[1]=col2[index+1];
			nearer_value[2]=col2[index+2];
			nearer_value[3]=col2[index+3];
		}

	//getiing lagren value array 
	//printf("\nUTTIME value to be interpolated is: %lf\n",UTtime_arr[j]);
/*	printf("\n UTtime_arr: %lf",UTtime_arr[f]);
	
	for(e=0;e<4;e++){
		printf("\n nearer: %lf",nearer[e]);
		cout<<"\n nearer_value: "<<nearer_value[e];
	}	
	cout<<"\n f: "<<f;	

*/	outvaluecol15[f]=getlagvalue(4,nearer,nearer_value,UTtime_arr[f]);
	//printf("\n The respective outvaluecol15 of the variable value_arr is: %ld  %lf",j,outvaluecol15[j]);
	}
	}
}




double getlagvalue(long n, double time_arr[],double value_arr[],double sectime)
{	
	int c,d;
	double s,t,k;
	s=1;
	t=1;
	k=0;	

        for(c=0; c<n; c++)
        {
            s=1;
            t=1;
            for(d=0; d<n; d++)
            {
                if(d!=c)
                {
                    s=s*(sectime-time_arr[d]);
                    t=t*(time_arr[c]-time_arr[d]);
                }
            }
            k=k+((s/t)*value_arr[c]);
        }
       //printf("\n\n The respective value of the variable value_arr is: %lf",k);
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

