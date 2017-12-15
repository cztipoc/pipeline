#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fitsio.h>
#include "level1handler.h"
#include<iostream>
#include <vector>
#include <algorithm>
#include "glog/logging.h"
#define PACKETSIZE 2048
#define BUFSIZE 1024

using namespace std;
void toWords(unsigned char *data,long size,unsigned short *word_data,int bigendian);
int getPackets(fitsfile *fptr,unsigned char *pktarray,long startRowNumber,long endRowNumber);
void heapSort(double *sps_time,double *sps_obt,double *instru_time,long n);
void heapify(double *sps_time,double *sps_obt,double *instru_time,long n, long i);
int main()
{
   	    char const* log_file=std::getenv("logfile_env");
    if ( log_file == NULL){
		log_file="czttctheal.log";
    }    
    google::InitGoogleLogging(log_file);
google::SetStderrLogging(google::INFO);

	char **scifiles;
	char tct[2048];
	char History[1024]="y";
	unsigned char tblbytes[1024];
	int nfiles,i,j,k,y,z,status=0,quad,rollover_flag=0,naxis1,rollover_at_end_flag=0,rollover_at_start_flag=0,execute=00,rollover_in_instrumenttime_flag=2;
	fitsfile *fsci;
	fitsfile *ftct,*ftemp0,*ftemp1;
	std::vector<double> LAXPCTIME;
	std::vector<double> LAXPCTIMEU;
	std::vector<double> LAXPCTIME0;
	std::vector<double> LAXPCTIME1;

	std::vector<double> INSTRUMENT_TIME;
	std::vector<long> removerows;
	int *DecodingStatusFlag,*DecodingErrorCount;
	long *removerows_arr,delta_instru,start_new=0,rollover_decoded_laxpc=0,decoded_laxpc_rollover_at=0;
	double delta_t,	first_instru_part1,first_spstime_part1,first_spsobt_part1,last_instru_part0,last_spstime_part0,last_spsobt_part0;
	double laxpctime,*SPS_TIME_temp,*SPS_OBT_temp,*INSTRUMENT_TIME_temp,first_instru_tct0,last_instru_tct1,first_instru_tct1,last_instru_tct0;
	long nrows=0,vec_size=0;
	unsigned char *event_data;
	unsigned short *data;
	unsigned char dataid=0,pktid=0,modeid=0;
	cout<<"\nEnter the tct file: ";
	cin>>tct;

	INSTRUMENT_TIME_temp=(double*)malloc(sizeof(double)*1);
	SPS_TIME_temp=(double*)malloc(sizeof(double)*1);
	SPS_OBT_temp=(double*)malloc(sizeof(double)*1);
	removerows_arr=(long*)malloc(sizeof(long)*1);
	DecodingStatusFlag=(int*)malloc(sizeof(int)*1);
	DecodingErrorCount=(int*)malloc(sizeof(int)*1);

	cout<<"\nEnter the no of level1 science files: ";
	cin>>nfiles;

	scifiles=(char **)malloc(sizeof(char)*50);
	event_data=(unsigned char*) malloc(sizeof(unsigned char)*PACKETSIZE);
	data=(unsigned short*)malloc(sizeof(unsigned short)*BUFSIZE);
	LAXPCTIME.clear();
	INSTRUMENT_TIME.clear();
	LAXPCTIME0.clear();
	LAXPCTIME1.clear();
	LAXPCTIMEU.clear();
	removerows.clear();
	
	for(z=0;z<nfiles;z++){
	scifiles[z]=(char*)malloc(sizeof(char)*1024);
	}

	for(i=0;i<nfiles;i++){
		cout<<"\nEnter the science files: ";
		cin>>scifiles[i];
		fits_open_file(&fsci,scifiles[i],READONLY,&status);
		if(status) { LOG(ERROR)<<"***Error in opening science file "<<scifiles[i];fits_report_error(stderr,status); return(EXIT_FAILURE);}
		LOG(INFO)<<"Opening science file "<<scifiles[i];
		for(quad=0;quad<4;quad++){
			fits_movabs_hdu(fsci,quad+2,NULL,&status);
			if(status) { LOG(INFO)<<"\:Error in moving to Extension ";LOG(ERROR)<<"***Error in moving to Extension for file "<<scifiles[i];fits_report_error(stderr,status); return(EXIT_FAILURE);}
			fits_get_num_rows(fsci,&nrows,&status);
			if(status) { LOG(INFO)<<"\:Error in reading no of rows.";LOG(ERROR)<<"***Error in reading no of rows for file "<<scifiles[i];fits_report_error(stderr,status); return(EXIT_FAILURE);}

			DecodingStatusFlag=(int*)malloc(sizeof(int)*nrows);
			DecodingErrorCount=(int*)malloc(sizeof(int)*nrows);

			if(nrows==0){
				LOG(INFO)<<"\: Empty extension Q"<<quad;
				LOG(ERROR)<<"*** Empty extension Q"<<quad<<" for file "<<scifiles[i];
				status=0;
				//cout<<"\n Empty extension Q"<<quad;
			}
		if(nrows!=0){	
		fits_read_col(fsci,TINT,5,1,1,nrows,NULL,DecodingStatusFlag,NULL,&status);
		if(status) { LOG(INFO)<<"\: Error in Reading DecodingStatusFlag column ";LOG(ERROR)<<"***Error in Reading DecodingStatusFlag column for file "<<scifiles[i];fits_report_error(stderr,status); return(EXIT_FAILURE);}
		fits_read_col(fsci,TINT,6,1,1,nrows,NULL,DecodingErrorCount,NULL,&status);
		if(status) { LOG(INFO)<<"\:  Error in Reading DecodingErrorCount column ";LOG(ERROR)<<"***Error in Reading DecodingErrorCount column for file "<<scifiles[i];fits_report_error(stderr,status); return(EXIT_FAILURE);}
		
			long startrow=1;
			long endrow=1;
		do	
		{
			
			endrow=startrow;//(nrows<=(startrow)) ? nrows : startrow-1;
			getPackets(fsci,event_data,startrow,endrow);
			toWords(event_data,PACKETSIZE,data,1);
			dataid=(unsigned char)((data[2]>>8) & 0x1f);
			pktid=(unsigned char)((data[2]>>4) & 0xf);
			modeid=(unsigned char)(data[2] & 0xf);
			
			//LOG(INFO)<<"\n paketid:%d\,pktid);
			if(DecodingStatusFlag[endrow-1]!=0 && DecodingErrorCount[endrow-1]!=0){
				//LOG(INFO)<<"\: DECODING ERROR FOUND ";
				laxpctime=((double)((data[14]<<12)&0xff000 |(data[13]>>4)) + ((double)((data[12]>>10)|((data[13]&15)<<6)))/1000.0 + ((double)((data[12] & 0x03ff))/1000000.0));
				//LOG(INFO)<<"ERROR LAXPCTIME DECODED: %lf",laxpctime);
			}	
			if(pktid==0 && DecodingStatusFlag[endrow-1]==0 && DecodingErrorCount[endrow-1]==0 )
			{
        		laxpctime=((double)((data[14]<<12)&0xff000 |(data[13]>>4)) + ((double)((data[12]>>10)|((data[13]&15)<<6)))/1000.0 + ((double)((data[12] & 0x03ff))/1000000.0));
				if(laxpctime!=0){
					LAXPCTIME.push_back (laxpctime);
				}	
			}		
			startrow++;
		}while(endrow<(nrows));
		
		//LOG(INFO)<<"Number of rows Q%d\t%d\,quad,nrows);
		}
		}
	fits_close_file(fsci, &status);
	if(status) { LOG(INFO)<<"\:Error in closing the science file ";LOG(ERROR)<<"***Error in closing the science file "<<scifiles[i];fits_report_error(stderr,status); return(EXIT_FAILURE);}	
	}
	status=0;
	cout<<"\n History[y/n]: ";
	cin>>History;



//rollover checking in tct file
	fits_open_file(&ftct,tct,READWRITE,&status);
	if(status) { LOG(INFO)<<"\:Error in opening tct file ";LOG(ERROR)<<"***Error in opening tct file "<<tct;fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_movabs_hdu(ftct,2,NULL,&status);
	if(status) { LOG(INFO)<<"\:Error in moving to Extension in tct ";LOG(ERROR)<<"*** Error in moving to Extension in tct "<<tct;fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_get_num_rows(ftct,&nrows,&status);
	if(status) { LOG(INFO)<<"\:Error in reading no of rows.";LOG(ERROR)<<"*** Error in reading no of rows for file "<<tct;fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_read_key(ftct,TINT,"NAXIS1",&naxis1,NULL,&status);
	if(status) { LOG(INFO)<<"\:Error in reading NAXIS1 key";LOG(ERROR)<<"*** Error in reading NAXIS1 key for file "<<tct;fits_report_error(stderr,status); return(EXIT_FAILURE);}
	
	SPS_TIME_temp=(double*)malloc(sizeof(double)*nrows);
	SPS_OBT_temp=(double*)malloc(sizeof(double)*nrows);
	INSTRUMENT_TIME_temp=(double*)malloc(sizeof(double)*nrows);

	fits_read_col(ftct,TDOUBLE,1,1,1,nrows,NULL,SPS_TIME_temp,NULL,&status);
	if(status) { LOG(INFO)<<"\:Error in reading SPS_TIME_temp";LOG(ERROR)<<"*** Error in reading SPS_TIME_temp for tct file "<<tct;fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_read_col(ftct,TDOUBLE,2,1,1,nrows,NULL,SPS_OBT_temp,NULL,&status);
	if(status) { LOG(INFO)<<"\:Error in reading SPS_OBT_temp";LOG(ERROR)<<"*** Error in reading SPS_OBT_temp for tct file "<<tct;fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_read_col(ftct,TDOUBLE,3,1,1,nrows,NULL,INSTRUMENT_TIME_temp,NULL,&status);
	if(status) { LOG(INFO)<<"\:Error in reading INSTRUMENT_TIME_temp";LOG(ERROR)<<"*** Error in reading INSTRUMENT_TIME_temp for tct file "<<tct;fits_report_error(stderr,status); return(EXIT_FAILURE);}

/*	for(z=0;z<LAXPCTIME.size();z++){
		LOG(INFO)<<"\ndecoded laxpctime: %lf",LAXPCTIME[z]);
	}			
*/
	for(i=0;i<nrows-1;i++){
		//cout<<"difference: "<<INSTRUMENT_TIME_temp[i+1]-INSTRUMENT_TIME_temp[i];
		//if((INSTRUMENT_TIME_temp[i+1]-INSTRUMENT_TIME_temp[i])<0 && abs(INSTRUMENT_TIME_temp[i+1]-INSTRUMENT_TIME_temp[i])>100000 && SPS_TIME_temp[i+1]-SPS_TIME_temp[i]>0){
		if(INSTRUMENT_TIME_temp[i]-INSTRUMENT_TIME_temp[i+1]>100000){
			//cout<<"\nRollover found in Instrument time";
			LOG(INFO)<<i+1;
			LOG(INFO)<<"Rollover found in INSTRUMENT_TIME ";
			LOG(INFO)<<"Rollover found in INSTRUMENT_TIME for tct";
			rollover_flag=1;
			rollover_in_instrumenttime_flag=1;
			break;
		}
	}
	if(i==nrows-1)
		rollover_in_instrumenttime_flag=0;


	//cout<<"\ti: "<<i;

//rollover checking in decoded laxpctime
	std::sort(LAXPCTIME.begin(), LAXPCTIME.end());
	//std::unique(LAXPCTIME.begin(), LAXPCTIME.end());
	long l;
	LAXPCTIMEU.push_back(LAXPCTIME[0]);
	for(l=1;l<LAXPCTIME.size();l++){
		if( LAXPCTIME[l] != LAXPCTIME[l-1] ){
			LAXPCTIMEU.push_back(LAXPCTIME[l]);
		}
	}
	LAXPCTIME.clear();

	for(l=0;l<LAXPCTIMEU.size();l++){
		//printf("\n%lf",LAXPCTIMEU[l]);
		LAXPCTIME.push_back(LAXPCTIMEU[l]);

	}
		
if(rollover_in_instrumenttime_flag==0){
		for(y=0;y<LAXPCTIME.size()-1;y++){
			if((LAXPCTIME[y]-LAXPCTIME[y+1])>300000){
				LOG(INFO)<<": Rollover found in decoded LAXPCTIME"<<LAXPCTIME[y]<<"\t"<<LAXPCTIME[y+1]<<"\t"<<LAXPCTIME[y]-LAXPCTIME[y+1];
				//cout<<"\nRollover found in decoded LAXPCTIME "<< LAXPCTIME[y] <<" "<<LAXPCTIME[y+1];
				rollover_flag=1;
				//i=y;
				break;
			}
		}
		if(LAXPCTIME[y]>INSTRUMENT_TIME_temp[i]){
			rollover_at_end_flag=1;
			//LOG(INFO)<<"\nROLLOVER AT END OF THE FILE";
			i=nrows;
		}
		if(LAXPCTIME[y]<INSTRUMENT_TIME_temp[i]){
			rollover_at_start_flag=1;
			//LOG(INFO)<<"\nROLLOVER AT START OF THE FILE";
			i=0;
		}	
	}
//cout<<"\ni: "<<i;	
//condition if rollover is occured after last sample in tct
//cout<<"\ni: "<<i;
if(rollover_flag==1){
		LOG(INFO)<<"\: PROCESSING FOR ROLLOVER";
		//cout<<"\nFOUND ROLLOVER ";	
		//cout<<"\ninput tct file: "<<tct;	
		//when rollover occurs in input tct file

		//spliting input tct at rollover and creating tct0
		fits_create_file(&ftemp0,"!temp.tct0",&status);
		if(status) {fits_report_error(stderr,status);LOG(ERROR)<<"*** Error in creating temp.tct0 (split tct part)"; return(EXIT_FAILURE);}
		fits_movabs_hdu(ftct,1,NULL, &status);
		if(status) {fits_report_error(stderr,status);LOG(ERROR)<<"*** Error in moving to hdu1 of temp.tct0 (split tct part)"; return(EXIT_FAILURE);}
		fits_copy_hdu(ftct, ftemp0,NULL,&status);
		if(status) {fits_report_error(stderr,status);LOG(ERROR)<<"*** Error in coping HDU to temp.tct0"; return(EXIT_FAILURE);}
		fits_movabs_hdu(ftct,2,NULL, &status);
		if(status) {fits_report_error(stderr,status);LOG(ERROR)<<"*** Error in moving to hdu2 of temp.tct0 (split tct part)"; return(EXIT_FAILURE);}
		fits_copy_hdu(ftct, ftemp0,NULL,&status);
		if(status) {fits_report_error(stderr,status);LOG(ERROR)<<"*** Error in coping HDU to temp.tct0"; return(EXIT_FAILURE);}
		fits_delete_rows(ftemp0,1,nrows, &status);
	if(rollover_at_end_flag==0 && rollover_at_start_flag==0){		
		for(k=0;k<i+1;k++){
			//cout<<"\nk:"<<k;
			
			fits_read_tblbytes(ftct, k+1, 1,naxis1,tblbytes, &status);
			if(status) { LOG(INFO)<<"\:Error in reading table bytes ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
			fits_write_tblbytes(ftemp0,k+1,1,naxis1,tblbytes,&status);
			if(status) { LOG(INFO)<<"\:Error in writing table bytes ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
		}
		start_new=k;
		//cout<<"start_new: "<<start_new;
		
		//spliting input tct at rollover and creating tct1
		fits_create_file(&ftemp1,"!temp.tct1",&status);
		if(status) {fits_report_error(stderr,status);LOG(ERROR)<<"*** Error in creating temp.tct1 (split tct part)"; return(EXIT_FAILURE);}
		fits_movabs_hdu(ftct,1,NULL, &status);
		if(status) {fits_report_error(stderr,status);LOG(ERROR)<<"*** Error in moving to hdu1 of temp.tct1 (split tct part)"; return(EXIT_FAILURE);}
		fits_copy_hdu(ftct, ftemp1,NULL,&status);
		if(status) {fits_report_error(stderr,status);LOG(ERROR)<<"*** Error in coping HDU to temp.tct1"; return(EXIT_FAILURE);}
		fits_movabs_hdu(ftct,2,NULL, &status);
		if(status) {fits_report_error(stderr,status);LOG(ERROR)<<"*** Error in moving to hdu2 of temp.tct1 (split tct part)"; return(EXIT_FAILURE);}
		fits_copy_hdu(ftct, ftemp1,NULL,&status);
		if(status) {fits_report_error(stderr,status);LOG(ERROR)<<"*** Error in coping HDU to temp.tct1"; return(EXIT_FAILURE);}
		fits_delete_rows(ftemp1,1,nrows, &status);
		j=0;
		for(k=start_new;k<nrows;k++){
			fits_read_tblbytes(ftct, k+1, 1,naxis1,tblbytes, &status);
			if(status) { LOG(INFO)<<"\:Error in reading table bytes ";LOG(ERROR)<<"*** Error in reading table bytes for file "<<tct;fits_report_error(stderr,status); return(EXIT_FAILURE);}
			
			fits_write_tblbytes(ftemp1,j+1,1,naxis1,tblbytes,&status);
			if(status) { LOG(INFO)<<"\:Error in writing table bytes ";LOG(ERROR)<<"*** Error in writing table bytes for file temp.tct1";fits_report_error(stderr,status); return(EXIT_FAILURE);}
			j++;
		}

	
		fits_close_file(ftemp0,&status);
		fits_close_file(ftemp1,&status);
	}
//when rollover after end of tct		
	if(rollover_at_end_flag==1){	
			for(k=0;k<i;k++){
			//cout<<"\nk:"<<k;
			
			fits_read_tblbytes(ftct, k+1, 1,naxis1,tblbytes, &status);
			if(status) { LOG(INFO)<<"\:Error in reading table bytes ";LOG(ERROR)<<"*** Error in reading table bytes for file "<<tct;fits_report_error(stderr,status); return(EXIT_FAILURE);}
			fits_write_tblbytes(ftemp0,k+1,1,naxis1,tblbytes,&status);
			if(status) { LOG(INFO)<<"\:Error in writing table bytes ";LOG(ERROR)<<"*** Error in writing table bytes for file "<<tct;fits_report_error(stderr,status); return(EXIT_FAILURE);}
		}
		start_new=k;
		//cout<<"start_new: "<<start_new;
		
		//spliting input tct at rollover and creating tct1
		fits_create_file(&ftemp1,"!temp.tct1",&status);
		if(status) {fits_report_error(stderr,status);LOG(ERROR)<<"*** Error in creating temp.tct1 (split tct part)"; return(EXIT_FAILURE);}
		fits_movabs_hdu(ftct,1,NULL, &status);
		if(status) {fits_report_error(stderr,status);LOG(ERROR)<<"*** Error in moving to hdu1 of temp.tct1 (split tct part)"; return(EXIT_FAILURE);}
		fits_copy_hdu(ftct, ftemp1,NULL,&status);
		if(status) {fits_report_error(stderr,status);LOG(ERROR)<<"*** Error in coping HDU to temp.tct1"; return(EXIT_FAILURE);}
		fits_movabs_hdu(ftct,2,NULL, &status);
		if(status) {fits_report_error(stderr,status);LOG(ERROR)<<"*** Error in moving to hdu2 of temp.tct1 (split tct part)"; return(EXIT_FAILURE);}
		fits_copy_hdu(ftct, ftemp1,NULL,&status);
		if(status) {fits_report_error(stderr,status);LOG(ERROR)<<"*** Error in coping HDU to temp.tct1"; return(EXIT_FAILURE);}
		fits_delete_rows(ftemp1,1,nrows, &status);
		j=0;
		for(k=start_new;k<nrows-1;k++){
			fits_read_tblbytes(ftct, k+1, 1,naxis1,tblbytes, &status);
			if(status) { LOG(INFO)<<"\:Error in reading table bytes ";LOG(ERROR)<<"*** Error in reading table bytes for file "<<tct;fits_report_error(stderr,status); return(EXIT_FAILURE);}
			
			fits_write_tblbytes(ftemp1,j+1,1,naxis1,tblbytes,&status);
			if(status) { LOG(INFO)<<"\:Error in writing table bytes ";LOG(ERROR)<<"*** Error in writing table bytes for file "<<tct;fits_report_error(stderr,status); return(EXIT_FAILURE);}
			j++;
		}

	
		fits_close_file(ftemp0,&status);
		fits_close_file(ftemp1,&status);
	}

//when rollover before the start of the file
	if(rollover_at_start_flag==1){	
			for(k=0;k<i;k++){
			//cout<<"\nk:"<<k;
			
			fits_read_tblbytes(ftct, k+1, 1,naxis1,tblbytes, &status);
			if(status) { LOG(INFO)<<"\:Error in reading table bytes ";LOG(ERROR)<<"*** Error in reading table bytes for file "<<tct;fits_report_error(stderr,status); return(EXIT_FAILURE);}
			fits_write_tblbytes(ftemp0,k+1,1,naxis1,tblbytes,&status);
			if(status) { LOG(INFO)<<"\:Error in writing table bytes ";LOG(ERROR)<<"*** Error in writing table bytes for file "<<tct;fits_report_error(stderr,status); return(EXIT_FAILURE);}
		}
		start_new=k;
		//cout<<"start_new: "<<start_new;
		
		//spliting input tct at rollover and creating tct1
		fits_create_file(&ftemp1,"!temp.tct1",&status);
		if(status) {fits_report_error(stderr,status);LOG(ERROR)<<"*** Error in creating temp.tct1 (split tct part)"; return(EXIT_FAILURE);}
		fits_movabs_hdu(ftct,1,NULL, &status);
		if(status) {fits_report_error(stderr,status);LOG(ERROR)<<"*** Error in moving to hdu1 of temp.tct1 (split tct part)"; return(EXIT_FAILURE);}
		fits_copy_hdu(ftct, ftemp1,NULL,&status);
		if(status) {fits_report_error(stderr,status);LOG(ERROR)<<"*** Error in coping HDU to temp.tct1"; return(EXIT_FAILURE);}
		fits_movabs_hdu(ftct,2,NULL, &status);
		if(status) {fits_report_error(stderr,status);LOG(ERROR)<<"*** Error in moving to hdu2 of temp.tct1 (split tct part)"; return(EXIT_FAILURE);}
		fits_copy_hdu(ftct, ftemp1,NULL,&status);
		if(status) {fits_report_error(stderr,status);LOG(ERROR)<<"*** Error in coping HDU to temp.tct1"; return(EXIT_FAILURE);}
		fits_delete_rows(ftemp1,1,nrows, &status);
		j=0;
		for(k=start_new;k<nrows-1;k++){
			fits_read_tblbytes(ftct, k+1, 1,naxis1,tblbytes, &status);
			if(status) { LOG(INFO)<<"\:Error in reading table bytes ";LOG(ERROR)<<"*** Error in reading table bytes for file "<<tct;fits_report_error(stderr,status); return(EXIT_FAILURE);}
			
			fits_write_tblbytes(ftemp1,j+1,1,naxis1,tblbytes,&status);
			if(status) { LOG(INFO)<<"\:Error in writing table bytes ";LOG(ERROR)<<"*** Error in writing table bytes for file "<<tct;fits_report_error(stderr,status); return(EXIT_FAILURE);}
			j++;
		}

	
		fits_close_file(ftemp0,&status);
		if(status) { LOG(INFO)<<"\:Error in closing temp0 tct file ";LOG(ERROR)<<"*** Error in closing temp.tct0";return(EXIT_FAILURE);}
		fits_close_file(ftemp1,&status);
		if(status) { LOG(INFO)<<"\:Error in closing temp1 tct file ";LOG(ERROR)<<"*** Error in closing temp.tct1";return(EXIT_FAILURE);}
	}

LOG(INFO)<<"\: TCT SPLITING COMPLETED.. ";
//cout<<"\nTCT SPLITING COMPLETED..";
//###############################################################################################################################

//spliting decoded laxpc_time according to INSTRUMENT_TIME Rollover in tct
if(rollover_at_end_flag==0 && rollover_at_start_flag==0){
	fits_open_file(&ftemp0,"temp.tct0",READWRITE,&status);
	if(status) { LOG(INFO)<<"\:Error in opening tct file ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_movabs_hdu(ftemp0,2,NULL,&status);
	if(status) { LOG(INFO)<<"\:Error in moving to Extension in tct ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_get_num_rows(ftemp0,&nrows,&status);
	if(status) { LOG(INFO)<<"\:Error in reading no of rows.";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	//cout<<"\nnrows of tct0: "<<nrows;
	delete []INSTRUMENT_TIME_temp;
	INSTRUMENT_TIME_temp=(double*)malloc(sizeof(double)*nrows);

	fits_read_col(ftemp0,TDOUBLE,3,1,1,nrows,NULL,INSTRUMENT_TIME_temp,NULL,&status);
	if(status) { LOG(INFO)<<"\::Error in reading INSTRUMENT_TIME_temp";LOG(ERROR)<<"*** Error in reading INSTRUMENT_TIME_temp for temp.tct0";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	//first_instru_tct0=INSTRUMENT_TIME_temp[0];
	//first_instru_tct0=LAXPCTIME[0];   
	
/*	first_instru_tct0=min(INSTRUMENT_TIME_temp[0],LAXPCTIME[0]);	//modified on 6 dec 2016 for proper spliting of decoded laxpc time
		//spliting decoded laxpc time
		for(y=0;y<LAXPCTIME.size();y++){
			printf("\n%lf",LAXPCTIME[y]);
			//LOG(INFO)<<"\ndecoed laxpctime from file 2: %lf",LAXPCTIME[y]);
			if(LAXPCTIME[y]>=first_instru_tct0-1 && LAXPCTIME[y]<=1048576){    // this step is done due to time stamps are not exactly same match so +1,-1 offset is added
				//cout<<"\npushing values in vector LAXPCTIME0 ";
				//LOG(INFO)<<" :%lf",LAXPCTIME[y]);
				LAXPCTIME0.push_back(LAXPCTIME[y]);
			}
			else{
				LAXPCTIME1.push_back(LAXPCTIME[y]);
			}	
		}

		//for(k=0;k<LAXPCTIME0.size();k++){
		//	LOG(INFO)<<"\nlaxpctime partition 1: %lf",LAXPCTIME0[k]);
		//	printf("\nlaxpctime0: %lf",LAXPCTIME0[k]);
		//}

		//for(k=0;k<LAXPCTIME1.size();k++){
			//LOG(INFO)<<"\n\nlaxpctime partition 2: %lf",LAXPCTIME1[k]);
			//printf("\nlaxpctime2: %lf",LAXPCTIME1[k]);
		//}


*/
		long laxpc_rollover_at=0;
		for(y=0;y<LAXPCTIME.size()-1;y++){

			if((LAXPCTIME[y+1]-LAXPCTIME[y])>300000){
				LOG(INFO)<<": Rollover found in decoded sorted and unique LAXPCTIME"<<LAXPCTIME[y]<<"\t"<<LAXPCTIME[y+1]<<"\t"<<LAXPCTIME[y]-LAXPCTIME[y+1];
				laxpc_rollover_at=y+1;
				break;
			}

		}
		//printf("\nlaxpc rollover at %ld",laxpc_rollover_at);
		LAXPCTIME0.clear();
		LAXPCTIME1.clear();
		for(y=laxpc_rollover_at;y<LAXPCTIME.size();y++){
			LAXPCTIME0.push_back(LAXPCTIME[y]);
		}	
		for(y=0;y<laxpc_rollover_at;y++){
			LAXPCTIME1.push_back(LAXPCTIME[y]);
		}
		long p;
		//for(p=0;p<LAXPCTIME0.size();p++){
			//printf("\n%lf",LAXPCTIME0[p]);
		//}
		//printf("\n************************************************************");
		//for(p=0;p<LAXPCTIME1.size();p++){
		//	printf("\n%lf",LAXPCTIME1[p]);
		//}

	
		//LOG(INFO)<<"\nlast laxpc sample in tct0: %lf",LAXPCTIME[i]);
		//LOG(INFO)<<"\nfirst laxpc sample in tct1: %lf",LAXPCTIME[i+1]);
		//cout<<"\nlaxpc0 size: "<<LAXPCTIME0.size();
		//cout<<"\nlaxpc1 size: "<<LAXPCTIME1.size();
		//fits_close_file(ftemp0,&status);

}		
if(rollover_at_start_flag==1){
	fits_open_file(&ftemp1,"temp.tct1",READWRITE,&status);
	if(status) { LOG(INFO)<<"\:Error in opening temporary tct file ";LOG(ERROR)<<"*** Error in opening temporary temp.tct1 (split tct part)";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_movabs_hdu(ftemp1,2,NULL,&status);
	if(status) { LOG(INFO)<<"\:Error in moving to Extension in tct ";LOG(ERROR)<<"*** Error in moving to Extension of temporary temp.tct1 (split tct part)";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_get_num_rows(ftemp1,&nrows,&status);
	if(status) { LOG(INFO)<<"\:Error in reading no of rows ";LOG(ERROR)<<"*** Error in reading no of rows for temp.tct1 (split tct part)";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	//cout<<"\nnrows of tct1: "<<nrows;
	delete []INSTRUMENT_TIME_temp;
	INSTRUMENT_TIME_temp=(double*)malloc(sizeof(double)*nrows);

	fits_read_col(ftemp1,TDOUBLE,3,1,1,nrows,NULL,INSTRUMENT_TIME_temp,NULL,&status);
	if(status) { LOG(INFO)<<"\:Error in reading INSTRUMENT_TIME_temp";LOG(ERROR)<<"*** Error in reading INSTRUMENT_TIME_temp for temp.tct1";fits_report_error(stderr,status); return(EXIT_FAILURE);}

/*	last_instru_tct1=INSTRUMENT_TIME_temp[nrows-1];
	first_instru_tct1=INSTRUMENT_TIME_temp[0];
		//spliting decoded laxpc time
		for(y=0;y<LAXPCTIME.size();y++){
			
			//LOG(INFO)<<"\ndecoed laxpctime from file 1: %lf",LAXPCTIME[y]);
			if((LAXPCTIME[y+1]-LAXPCTIME[y])<0 && abs(LAXPCTIME[y+1]-LAXPCTIME[y])>10000)
			{	//LOG(INFO)<<"\nsplit at:######## %lf",LAXPCTIME[y]);
				rollover_decoded_laxpc=LAXPCTIME[y+1];
				decoded_laxpc_rollover_at=y;
				break;
			}
		}
		for(y=0;y<LAXPCTIME.size();y++){
			//LOG(INFO)<<"\ndecoed laxpctime from file 2: %lf",LAXPCTIME[y]);
			//if(LAXPCTIME[y]>=rollover_decoded_laxpc-10000 && LAXPCTIME[y]<=last_instru_tct1+10000){    // this step is done due to time stamps are not exactly same match so +1,-1 offset is added
			if(LAXPCTIME[y]>=first_instru_tct1-1 && LAXPCTIME[y]<=last_instru_tct1){
				//cout<<"\npushing values in vector LAXPCTIME0 ";
				//LOG(INFO)<<" :%lf",LAXPCTIME[y]);
				LAXPCTIME1.push_back(LAXPCTIME[y]);
			}
			else{
				LAXPCTIME0.push_back(LAXPCTIME[y]);
			}	
		}
*/		
/*		for(k=0;k<LAXPCTIME0.size();k++){
			LOG(INFO)<<"\nlaxpctime partition 1: %lf",LAXPCTIME0[k]);
		}

		for(k=0;k<LAXPCTIME1.size();k++){
			LOG(INFO)<<"\nlaxpctime partition 2: %lf",LAXPCTIME1[k]);
		}
*/		
		long laxpc_rollover_at=0;
		for(y=0;y<LAXPCTIME.size()-1;y++){

			if((LAXPCTIME[y+1]-LAXPCTIME[y])>300000){
				LOG(INFO)<<": Rollover found in decoded sorted and unique LAXPCTIME"<<LAXPCTIME[y]<<"\t"<<LAXPCTIME[y+1]<<"\t"<<LAXPCTIME[y]-LAXPCTIME[y+1];
				laxpc_rollover_at=y+1;
				break;
			}

		}
		//printf("\nlaxpc rollover at %ld",laxpc_rollover_at);
		LAXPCTIME0.clear();
		LAXPCTIME1.clear();
		for(y=laxpc_rollover_at;y<LAXPCTIME.size();y++){
			LAXPCTIME0.push_back(LAXPCTIME[y]);
		}	
		for(y=0;y<laxpc_rollover_at;y++){
			LAXPCTIME1.push_back(LAXPCTIME[y]);
		}
		long p;
		//for(p=0;p<LAXPCTIME0.size();p++){
			//printf("\n%lf",LAXPCTIME0[p]);
		//}
		//printf("\n************************************************************");
		//for(p=0;p<LAXPCTIME1.size();p++){
		//	printf("\n%lf",LAXPCTIME1[p]);
		//}
		
		//LOG(INFO)<<"\nlast laxpc sample in tct0: %lf",LAXPCTIME[i]);
		//LOG(INFO)<<"\nfirst laxpc sample in tct1: %lf",LAXPCTIME[i+1]);
		//cout<<"\nlaxpc0 size: "<<LAXPCTIME0.size();
		//cout<<"\nlaxpc1 size: "<<LAXPCTIME1.size();
		//fits_close_file(ftemp1,&status);

}		

if(rollover_at_end_flag==1){
	fits_open_file(&ftemp0,"temp.tct0",READWRITE,&status);
	if(status) { LOG(INFO)<<"\:Error in opening temporary tct file temp.tct0";LOG(ERROR)<<"***Error in opening temporary tct file temp.tct0 ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_movabs_hdu(ftemp0,2,NULL,&status);
	if(status) { LOG(INFO)<<"\:Error in moving to Extension in tct temp.tct0 ";LOG(ERROR)<<"*** Error in moving to Extension in tct temp.tct0";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_get_num_rows(ftemp0,&nrows,&status);
	if(status) { LOG(INFO)<<"\:Error in reading no of rows for file temp.tct0";LOG(ERROR)<<"*** Error in reading no of rows for file temp.tct0";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	//cout<<"\nnrows of tct0: "<<nrows;
	delete []INSTRUMENT_TIME_temp;
	INSTRUMENT_TIME_temp=(double*)malloc(sizeof(double)*nrows);

	fits_read_col(ftemp0,TDOUBLE,3,1,1,nrows,NULL,INSTRUMENT_TIME_temp,NULL,&status);
	if(status) { LOG(INFO)<<"\:Error in reading INSTRUMENT_TIME_temp";LOG(ERROR)<<"*** Error in reading INSTRUMENT_TIME_temp for temp.tct1";fits_report_error(stderr,status); return(EXIT_FAILURE);}

/*	first_instru_tct0=INSTRUMENT_TIME_temp[0];
	last_instru_tct0=INSTRUMENT_TIME_temp[nrows-1];


		//spliting decoded laxpc time
		for(y=0;y<LAXPCTIME.size();y++){
			//LOG(INFO)<<"\ndecoed laxpctime from file 1: %lf",LAXPCTIME[y]);
			if((LAXPCTIME[y+1]-LAXPCTIME[y])<0 && abs(LAXPCTIME[y+1]-LAXPCTIME[y])>10000)
			{	//LOG(INFO)<<"\nsplit at:######## %lf",LAXPCTIME[y]);
				rollover_decoded_laxpc=LAXPCTIME[y];
				decoded_laxpc_rollover_at=y;
				break;
			}
		}
		for(y=0;y<LAXPCTIME.size();y++){
			//LOG(INFO)<<"\ndecoed laxpctime from file 2: %lf",LAXPCTIME[y]);
			if(LAXPCTIME[y]>=first_instru_tct0-1 && LAXPCTIME[y]<=1048576){    // this step is done due to time stamps are not exactly same match so +1,-1 offset is added
				//cout<<"\npushing values in vector LAXPCTIME0 ";
				//LOG(INFO)<<" :%lf",LAXPCTIME[y]);
				LAXPCTIME0.push_back(LAXPCTIME[y]);
			}
			else{
				LAXPCTIME1.push_back(LAXPCTIME[y]);
			}	
		}
*/
/*		for(k=0;k<LAXPCTIME0.size();k++){
			LOG(INFO)<<"\nlaxpctime partition 1: %lf",LAXPCTIME0[k]);
		}

		for(k=0;k<LAXPCTIME1.size();k++){
			LOG(INFO)<<"\nlaxpctime partition 2: %lf",LAXPCTIME1[k]);
		}
*/		
		long laxpc_rollover_at=0;
		for(y=0;y<LAXPCTIME.size()-1;y++){

			if((LAXPCTIME[y+1]-LAXPCTIME[y])>300000){
				LOG(INFO)<<": Rollover found in decoded sorted and unique LAXPCTIME"<<LAXPCTIME[y]<<"\t"<<LAXPCTIME[y+1]<<"\t"<<LAXPCTIME[y]-LAXPCTIME[y+1];
				laxpc_rollover_at=y+1;
				break;
			}

		}
		//printf("\nlaxpc rollover at %ld",laxpc_rollover_at);
		LAXPCTIME0.clear();
		LAXPCTIME1.clear();
		for(y=laxpc_rollover_at;y<LAXPCTIME.size();y++){
			LAXPCTIME0.push_back(LAXPCTIME[y]);
		}	
		for(y=0;y<laxpc_rollover_at;y++){
			LAXPCTIME1.push_back(LAXPCTIME[y]);
		}
		long p;
		//for(p=0;p<LAXPCTIME0.size();p++){
			//printf("\n%lf",LAXPCTIME0[p]);
		//}
		//printf("\n************************************************************");
		//for(p=0;p<LAXPCTIME1.size();p++){
		//	printf("\n%lf",LAXPCTIME1[p]);
		//}
	
		//LOG(INFO)<<"\nlast laxpc sample in tct0: %lf",LAXPCTIME[i]);
		//LOG(INFO)<<"\nfirst laxpc sample in tct1: %lf",LAXPCTIME[i+1]);
		//cout<<"\nlaxpc0 size: "<<LAXPCTIME0.size();
		//cout<<"\nlaxpc1 size: "<<LAXPCTIME1.size();
}		


//###############################################################################################################################################################################################
//checking which part should be corrected first
if(rollover_at_start_flag==1){
	LOG(INFO)<<"\: ROLLOVER BEFORE START TIME OF TCT ";
	//cout<<"\nROLLOVER BEFORE START TIME OF TCT";
	execute=10;				//correct part1 and then part0 tct
}	
if(rollover_at_end_flag==1){
	LOG(INFO)<<"\: ROLLOVER AFTER END OF TCT";
	//cout<<"\nROLLOVER AFTER END OF TCT";
	execute=01;					//correct part0 and then part1 tct
}	
if(rollover_at_start_flag==0 && rollover_at_end_flag==0 ){
	LOG(INFO)<<"\: ROLLOVER IN BETWEEN START AND END TIME OF TCT ";
	execute=01;
}	
switch(execute){
	case 01:
	//cout<<"\nEXECUTING CASE 01";
	//correcting splited tct0	
	//sort( LAXPCTIME0.begin(), LAXPCTIME0.end() );
	//LAXPCTIME0.erase( unique( LAXPCTIME0.begin(), LAXPCTIME0.end() ), LAXPCTIME0.end() );
	vec_size=LAXPCTIME0.size();
	//modyfying tct file
	fits_open_file(&ftct,"temp.tct0",READWRITE,&status);
	if(status) { LOG(INFO)<<"\:Error in opening tct file temp.tct0";LOG(ERROR)<<"*** Error in opening tct file temp.tct0";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_movabs_hdu(ftct,2,NULL,&status);
	if(status) { LOG(INFO)<<"\:Error in moving to Extension in tct temp.tct0 ";LOG(ERROR)<<"*** Error in moving to Extension in tct temp.tct0";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_get_num_rows(ftct,&nrows,&status);
	if(status) { LOG(INFO)<<"\:Error in reading no of rows for file temp.tct0";LOG(ERROR)<<"*** Error in reading no of rows for file temp.tct0";fits_report_error(stderr,status); return(EXIT_FAILURE);}

	delete []INSTRUMENT_TIME_temp;
   	INSTRUMENT_TIME_temp=(double*)malloc(sizeof(double)*vec_size);
	for(i=0;i<vec_size;i++){
		INSTRUMENT_TIME_temp[i]=LAXPCTIME0[i];
		//LOG(INFO)<<"%lf\,LAXPCTIME[i]);
	}
		
	fits_insert_rows(ftct, nrows,vec_size,&status);
	if(status) { LOG(INFO)<<"\:Error in adding rows to tct file.. temp.tct0";LOG(ERROR)<<"***Error in adding rows to tct file.. temp.tct0";fits_report_error(stderr,status); return(EXIT_FAILURE);}	
	fits_write_col(ftct, TDOUBLE,3,nrows+1,1,vec_size,INSTRUMENT_TIME_temp,&status);
	if(status) { LOG(INFO)<<"\:Error in writing decoded laxpctime temp.tct0";LOG(ERROR)<<"***Error in writing decoded laxpctime temp.tct0";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_close_file(ftct,&status);
	if(status) { LOG(INFO)<<"\:Error in closing tct file temp.tct0";LOG(ERROR)<<"*** Error in closing tct file temp.tct0";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	
	LAXPCTIME0.clear();
	INSTRUMENT_TIME.clear();
	
	fits_open_file(&ftct,"temp.tct0",READWRITE,&status);
	if(status) { LOG(INFO)<<"\:Error in opening tct file temp.tct0";LOG(ERROR)<<"*** Error in opening tct file temp.tct0";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_movabs_hdu(ftct,2,NULL,&status);
	if(status) { LOG(INFO)<<"\:Error in moving to Extension in tct temp.tct0";LOG(ERROR)<<"*** Error in moving to Extension in tct temp.tct0";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_get_num_rows(ftct,&nrows,&status);
	if(status) { LOG(INFO)<<"\:Error in reading no of rows temp.tct0";LOG(ERROR)<<"*** Error in reading no of rows temp.tct0";fits_report_error(stderr,status); return(EXIT_FAILURE);}

	delete []INSTRUMENT_TIME_temp;
	delete []SPS_TIME_temp;
	delete []SPS_OBT_temp;
	INSTRUMENT_TIME_temp=(double*)malloc(sizeof(double)*nrows);
	SPS_TIME_temp=(double*)malloc(sizeof(double)*nrows);
	SPS_OBT_temp=(double*)malloc(sizeof(double)*nrows);

	fits_read_col(ftct,TDOUBLE,1,1,1,nrows,NULL,SPS_TIME_temp,NULL,&status);
	if(status) { LOG(INFO)<<"Error in reading SPS_TIME_temp";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_read_col(ftct,TDOUBLE,2,1,1,nrows,NULL,SPS_OBT_temp,NULL,&status);
	if(status) { LOG(INFO)<<"Error in reading SPS_OBT_temp";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_read_col(ftct,TDOUBLE,3,1,1,nrows,NULL,INSTRUMENT_TIME_temp,NULL,&status);
	if(status) { LOG(INFO)<<"Error in reading INSTRUMENT_TIME_temp";fits_report_error(stderr,status); return(EXIT_FAILURE);}

	heapSort(SPS_TIME_temp,SPS_OBT_temp,INSTRUMENT_TIME_temp,nrows);

	fits_write_col(ftct, TDOUBLE,1,1,1,nrows,SPS_TIME_temp,&status);
	if(status) { LOG(INFO)<<"Error in writing  SPS_TIME colum";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_write_col(ftct, TDOUBLE,2,1,1,nrows,SPS_OBT_temp,&status);
	if(status) { LOG(INFO)<<"Error in writing  SPS_OBT colum";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_write_col(ftct, TDOUBLE,3,1,1,nrows,INSTRUMENT_TIME_temp,&status);
	if(status) { LOG(INFO)<<"Error in writing  INSTRUMENT_TIME colum";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_close_file(ftct,&status);
	if(status) { LOG(INFO)<<"Error in closing tct file ";fits_report_error(stderr,status); return(EXIT_FAILURE);}

	fits_open_file(&ftct,"temp.tct0",READWRITE,&status);
	if(status) { LOG(INFO)<<"\:Error in opening tct file ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_movabs_hdu(ftct,2,NULL,&status);
	if(status) { LOG(INFO)<<"\:Error in moving to Extension in tct ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_get_num_rows(ftct,&nrows,&status);
	if(status) { LOG(INFO)<<"\:Error in reading no of rows.";fits_report_error(stderr,status); return(EXIT_FAILURE);}

	fits_read_col(ftct,TDOUBLE,1,1,1,nrows,NULL,SPS_TIME_temp,NULL,&status);
	if(status) { LOG(INFO)<<"SPS_TIME_temp";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_read_col(ftct,TDOUBLE,2,1,1,nrows,NULL,SPS_OBT_temp,NULL,&status);
	if(status) { LOG(INFO)<<"SPS_OBT_temp";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_read_col(ftct,TDOUBLE,3,1,1,nrows,NULL,INSTRUMENT_TIME_temp,NULL,&status);
	if(status) { LOG(INFO)<<"INSTRUMENT_TIME_temp";fits_report_error(stderr,status); return(EXIT_FAILURE);}

	for(k=0;k<nrows-1;k++){
		if(abs(INSTRUMENT_TIME_temp[k+1]-INSTRUMENT_TIME_temp[k])<=0.001){
			//cout<<"\nk:"<<k;
			//cout<<"\n found duplicates...";
			if(SPS_TIME_temp[k]==0){
				//cout<<"\n removing duplicates... row : "<<k+1;
				removerows.push_back(k+1);
				
			}
			if(SPS_TIME_temp[k+1]==0){
				//cout<<"\n removing duplicates (2nd if) row : "<<k+2;
					if(k!=nrows-1)
						removerows.push_back(k+2);		
			}
		}
	}	
	//cout<<"\n size of removerows vector: "<<removerows.size();
	
	removerows_arr=(long*)malloc(sizeof(long)*removerows.size());
	sort( removerows.begin(),removerows.end() );
	//removerows.erase( unique( removerows.begin(), removerows.end() ), removerows.end() );
	for(k=0;k<removerows.size();k++){
		removerows_arr[k]=removerows[k];
	}
	fits_delete_rowlist(ftct, removerows_arr,removerows.size(), &status);	
	if(status) { LOG(INFO)<<"\:Error in deleting rows ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	removerows.clear();
	fits_close_file(ftct,&status);
	if(status) { LOG(INFO)<<"\:Error in closing tct file ";fits_report_error(stderr,status); return(EXIT_FAILURE);}

//filter 1
	fits_open_file(&ftct,"temp.tct0",READWRITE,&status);
	if(status) { LOG(INFO)<<"\:Error in opening tct file ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_movabs_hdu(ftct,2,NULL,&status);
	if(status) { LOG(INFO)<<"\:Error in moving to Extension in tct ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_get_num_rows(ftct,&nrows,&status);
	if(status) { LOG(INFO)<<"\:Error in reading no of rows.";fits_report_error(stderr,status); return(EXIT_FAILURE);}

	delete []INSTRUMENT_TIME_temp;
	delete []SPS_TIME_temp;
	delete []SPS_OBT_temp;


	INSTRUMENT_TIME_temp=(double*)malloc(sizeof(double)*nrows);
	SPS_TIME_temp=(double*)malloc(sizeof(double)*nrows);
	SPS_OBT_temp=(double*)malloc(sizeof(double)*nrows);



	fits_read_col(ftct,TDOUBLE,1,1,1,nrows,NULL,SPS_TIME_temp,NULL,&status);
	if(status) { LOG(INFO)<<":Error in reading column SPS_TIME_temp";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_read_col(ftct,TDOUBLE,2,1,1,nrows,NULL,SPS_OBT_temp,NULL,&status);
	if(status) { LOG(INFO)<<":Error in reading column SPS_OBT_temp";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_read_col(ftct,TDOUBLE,3,1,1,nrows,NULL,INSTRUMENT_TIME_temp,NULL,&status);
	if(status) { LOG(INFO)<<":Error in reading column INSTRUMENT_TIME_temp";fits_report_error(stderr,status); return(EXIT_FAILURE);}

	for(k=0;k<nrows-1;k++){	
		if(abs(INSTRUMENT_TIME_temp[k+1]-INSTRUMENT_TIME_temp[k])<1){
			if( (SPS_TIME_temp[k]==0||SPS_TIME_temp[k+1]!=0)|| (SPS_TIME_temp[k]!=0||SPS_TIME_temp[k+1]==0)){
				if(SPS_TIME_temp[k]==0){
					INSTRUMENT_TIME_temp[k+1]=INSTRUMENT_TIME_temp[k];
					//fits_delete_rows(ftct,k+1,1,&status);
					removerows.push_back(k+1);
				}
				if(SPS_TIME_temp[k+1]==0){
					INSTRUMENT_TIME_temp[k]=INSTRUMENT_TIME_temp[k+1];
					//fits_delete_rows(ftct,k+1+1,1,&status);
					removerows.push_back(k+2);
				}
			}

			else{
					//fits_delete_rows(ftct,k+1,1,&status);
					removerows.push_back(k+1);
					//fits_delete_rows(ftct,k+1+1,1,&status);
					removerows.push_back(k+2);
			}
		}
	
//filter 2	
		if(abs(INSTRUMENT_TIME_temp[k+1]-INSTRUMENT_TIME_temp[k])<14.9){
			if(k!=0 || k!=nrows-1){
				//condition when k+1 sample is smaller
				if((INSTRUMENT_TIME_temp[k+1]-INSTRUMENT_TIME_temp[k])<0){
					if(abs(INSTRUMENT_TIME_temp[k+1]-INSTRUMENT_TIME_temp[k])<14.9){
						//fits_delete_rows(ftct,k+1+1,1,&status);
						removerows.push_back(k+2);
					}
					if(abs(INSTRUMENT_TIME_temp[k+1]-INSTRUMENT_TIME_temp[k])>17.2){
						//fits_delete_rows(ftct,k+1+1,1,&status);
						removerows.push_back(k+2);
					}
					//condition when k is larger
					if(abs(INSTRUMENT_TIME_temp[k]-INSTRUMENT_TIME_temp[k+1])<14.9){
						//fits_delete_rows(ftct,k+1,1,&status);
						removerows.push_back(k+1);
					}
					if(abs(INSTRUMENT_TIME_temp[k]-INSTRUMENT_TIME_temp[k+1])>17.2){
						//fits_delete_rows(ftct,k+1,1,&status);
						removerows.push_back(k+1);
					}

				
				
				}	
				

				//condition when k sample is smaller
				if((INSTRUMENT_TIME_temp[k+1]-INSTRUMENT_TIME_temp[k])>0){
					if(abs(INSTRUMENT_TIME_temp[k]-INSTRUMENT_TIME_temp[k-1])<14.9){
						//fits_delete_rows(ftct,k+1,1,&status);
						removerows.push_back(k+1);
					}
					if(abs(INSTRUMENT_TIME_temp[k]-INSTRUMENT_TIME_temp[k-1])>17.2){
						//fits_delete_rows(ftct,k+1,1,&status);
						removerows.push_back(k+1);
					}
					//condition when k+1 is larger
					if(abs(INSTRUMENT_TIME_temp[k+1]-INSTRUMENT_TIME_temp[k+2])<14.9){
						//fits_delete_rows(ftct,k+1+1,1,&status);
						removerows.push_back(k+2);
					}
					if(abs(INSTRUMENT_TIME_temp[k+1]-INSTRUMENT_TIME_temp[k+2])>17.2){
						//fits_delete_rows(ftct,k+1+1,1,&status);
						removerows.push_back(k+2);
					}


				}
			}	
		}
	}


	for(k=0;k<nrows;k++){
		if(k==0){
			for(j=0;j<nrows;j++){
				if(SPS_TIME_temp[j]!=0){
					break;
				}
			}
			delta_instru=round(abs(INSTRUMENT_TIME_temp[j]-INSTRUMENT_TIME_temp[0]));
			SPS_TIME_temp[0]=SPS_TIME_temp[j]-delta_instru;
			SPS_OBT_temp[0]=SPS_OBT_temp[j]-delta_instru;
			
		}

		if(SPS_TIME_temp[k]==0){
			delta_instru=round(abs(INSTRUMENT_TIME_temp[k]-INSTRUMENT_TIME_temp[k-1]));
			SPS_TIME_temp[k]=SPS_TIME_temp[k-1]+delta_instru;
			SPS_OBT_temp[k]=SPS_OBT_temp[k-1]+delta_instru;
		}
	}	

	fits_write_col(ftct, TDOUBLE,1,1,1,nrows,SPS_TIME_temp,&status);
	if(status) { LOG(INFO)<<":Error in writing  SPS_TIME colum";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_write_col(ftct, TDOUBLE,2,1,1,nrows,SPS_OBT_temp,&status);
	if(status) { LOG(INFO)<<":Error in writing  SPS_OBT colum";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_write_col(ftct, TDOUBLE,3,1,1,nrows,INSTRUMENT_TIME_temp,&status);
	if(status) { LOG(INFO)<<":Error in writing  INSTRUMENT_TIME colum";fits_report_error(stderr,status); return(EXIT_FAILURE);}

	delete []removerows_arr;
	removerows_arr=(long*)malloc(sizeof(long)*removerows.size());
	std::sort( removerows.begin(),removerows.end() );
	removerows.erase( unique( removerows.begin(), removerows.end() ), removerows.end() );
	for(k=0;k<removerows.size();k++){
			//cout<<"\nremoverows: "<<removerows[k];
	//	if(removerows[k+1]<=removerows[k])
	//		cout<<"removerows["<<k+1<<"]:"<<removerows[k+1]<<" less than removerows["<<k<<"]:"<<removerows[k];
	}	
	for(k=0;k<removerows.size();k++){
		removerows_arr[k]=removerows[k];
	}
	fits_delete_rowlist(ftct, removerows_arr,removerows.size(), &status);	
	if(status) { LOG(INFO)<<"\:Error in deleting rows ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	removerows.clear();
	fits_close_file(ftct,&status);
	if(status) { LOG(INFO)<<"\:Error in closing tct file ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	//tct0 correction done
	
	
	//buffering the last sps_time and sps_obt in tct0
	fits_open_file(&ftemp0,"temp.tct0",READONLY,&status);
	if(status) { LOG(INFO)<<"\:Error in opening tct file ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_movabs_hdu(ftemp0,2,NULL,&status);
	if(status) { LOG(INFO)<<"\:Error in moving to Extension in tct ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_get_num_rows(ftemp0,&nrows,&status);
	if(status) { LOG(INFO)<<"\:Error in reading no of rows.";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	
	delete []INSTRUMENT_TIME_temp;
	delete []SPS_TIME_temp;
	delete []SPS_OBT_temp;

	
	INSTRUMENT_TIME_temp=(double*)malloc(sizeof(double)*nrows);
	SPS_TIME_temp=(double*)malloc(sizeof(double)*nrows);
	SPS_OBT_temp=(double*)malloc(sizeof(double)*nrows);

	fits_read_col(ftemp0,TDOUBLE,1,1,1,nrows,NULL,SPS_TIME_temp,NULL,&status);
	if(status) { LOG(INFO)<<"SPS_TIME_temp";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_read_col(ftemp0,TDOUBLE,2,1,1,nrows,NULL,SPS_OBT_temp,NULL,&status);
	if(status) { LOG(INFO)<<"SPS_OBT_temp";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_read_col(ftemp0,TDOUBLE,3,1,1,nrows,NULL,INSTRUMENT_TIME_temp,NULL,&status);
	if(status) { LOG(INFO)<<"INSTRUMENT_TIME_temp";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	
	last_instru_part0=INSTRUMENT_TIME_temp[nrows-1];
	last_spstime_part0=SPS_TIME_temp[nrows-1];
	last_spsobt_part0=SPS_OBT_temp[nrows-1];
	//LOG(INFO)<<"\nlast_instru_part0: %lf\tlast_spstime_part0: %lf\tlast_spsobt_part0: %lf",last_instru_part0,last_spstime_part0,last_spsobt_part0);
	fits_close_file(ftemp0,&status);

//################################
	//correcting splited tct1	
	//sort( LAXPCTIME1.begin(), LAXPCTIME1.end() );
	//LAXPCTIME1.erase( unique( LAXPCTIME1.begin(), LAXPCTIME1.end() ), LAXPCTIME1.end() );
	vec_size=LAXPCTIME1.size();

	//modyfying tct file
	fits_open_file(&ftct,"temp.tct1",READWRITE,&status);
	if(status) { LOG(INFO)<<"\:Error in opening tct file ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_movabs_hdu(ftct,2,NULL,&status);
	if(status) { LOG(INFO)<<"\:Error in moving to Extension in tct ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_get_num_rows(ftct,&nrows,&status);
	if(status) { LOG(INFO)<<"\:Error in reading no of rows.";fits_report_error(stderr,status); return(EXIT_FAILURE);}

	delete []INSTRUMENT_TIME_temp;
   	INSTRUMENT_TIME_temp=(double*)malloc(sizeof(double)*vec_size);
	for(i=0;i<vec_size;i++){
		INSTRUMENT_TIME_temp[i]=LAXPCTIME1[i];
		//LOG(INFO)<<"%lf\,LAXPCTIME[i]);
	}
		
	fits_insert_rows(ftct, nrows,vec_size,&status);
	if(status) { LOG(INFO)<<"\:Error in adding rows to tct file..";fits_report_error(stderr,status); return(EXIT_FAILURE);}	
	fits_write_col(ftct, TDOUBLE,3,nrows+1,1,vec_size,INSTRUMENT_TIME_temp,&status);
	if(status) { LOG(INFO)<<"\:Error in writing decoded laxpctime ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_close_file(ftct,&status);
	if(status) { LOG(INFO)<<"\:Error in closing tct file ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	
	LAXPCTIME1.clear();
	INSTRUMENT_TIME.clear();
	
	fits_open_file(&ftct,"temp.tct1",READWRITE,&status);
	if(status) { LOG(INFO)<<"\:Error in opening tct file ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_movabs_hdu(ftct,2,NULL,&status);
	if(status) { LOG(INFO)<<"\:Error in moving to Extension in tct ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_get_num_rows(ftct,&nrows,&status);
	if(status) { LOG(INFO)<<"\:Error in reading no of rows.";fits_report_error(stderr,status); return(EXIT_FAILURE);}

	delete []INSTRUMENT_TIME_temp;
	delete []SPS_TIME_temp;
	delete []SPS_OBT_temp;



	INSTRUMENT_TIME_temp=(double*)malloc(sizeof(double)*nrows);
	SPS_TIME_temp=(double*)malloc(sizeof(double)*nrows);
	SPS_OBT_temp=(double*)malloc(sizeof(double)*nrows);

	fits_read_col(ftct,TDOUBLE,1,1,1,nrows,NULL,SPS_TIME_temp,NULL,&status);
	if(status) { LOG(INFO)<<"Error in reading SPS_TIME_temp";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_read_col(ftct,TDOUBLE,2,1,1,nrows,NULL,SPS_OBT_temp,NULL,&status);
	if(status) { LOG(INFO)<<"Error in reading SPS_OBT_temp";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_read_col(ftct,TDOUBLE,3,1,1,nrows,NULL,INSTRUMENT_TIME_temp,NULL,&status);
	if(status) { LOG(INFO)<<"Error in reading INSTRUMENT_TIME_temp";fits_report_error(stderr,status); return(EXIT_FAILURE);}

	heapSort(SPS_TIME_temp,SPS_OBT_temp,INSTRUMENT_TIME_temp,nrows);

	fits_write_col(ftct, TDOUBLE,1,1,1,nrows,SPS_TIME_temp,&status);
	if(status) { LOG(INFO)<<":Error in writing  SPS_TIME colum";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_write_col(ftct, TDOUBLE,2,1,1,nrows,SPS_OBT_temp,&status);
	if(status) { LOG(INFO)<<":Error in writing  SPS_OBT colum";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_write_col(ftct, TDOUBLE,3,1,1,nrows,INSTRUMENT_TIME_temp,&status);
	if(status) { LOG(INFO)<<":Error in writing  INSTRUMENT_TIME colum";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_close_file(ftct,&status);
	if(status) { LOG(INFO)<<":Error in closing tct file ";fits_report_error(stderr,status); return(EXIT_FAILURE);}

	fits_open_file(&ftct,"temp.tct1",READWRITE,&status);
	if(status) { LOG(INFO)<<"\:Error in opening tct file ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_movabs_hdu(ftct,2,NULL,&status);
	if(status) { LOG(INFO)<<"\:Error in moving to Extension in tct ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_get_num_rows(ftct,&nrows,&status);
	if(status) { LOG(INFO)<<"\:Error in reading no of rows.";fits_report_error(stderr,status); return(EXIT_FAILURE);}

	fits_read_col(ftct,TDOUBLE,1,1,1,nrows,NULL,SPS_TIME_temp,NULL,&status);
	if(status) { LOG(INFO)<<" Error in reading SPS_TIME_temp";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_read_col(ftct,TDOUBLE,2,1,1,nrows,NULL,SPS_OBT_temp,NULL,&status);
	if(status) { LOG(INFO)<<"Error in reading SPS_OBT_temp";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_read_col(ftct,TDOUBLE,3,1,1,nrows,NULL,INSTRUMENT_TIME_temp,NULL,&status);
	if(status) { LOG(INFO)<<"Error in reading INSTRUMENT_TIME_temp";fits_report_error(stderr,status); return(EXIT_FAILURE);}

	for(k=0;k<nrows-1;k++){
		if(abs(INSTRUMENT_TIME_temp[k+1]-INSTRUMENT_TIME_temp[k])<=0.001){
			//cout<<"\nk:"<<k;
			//cout<<"\n found duplicates...";
			if(SPS_TIME_temp[k]==0){
				//cout<<"\n removing duplicates... row : "<<k+1;
				removerows.push_back(k+1);
				
			}
			if(SPS_TIME_temp[k+1]==0){
				//cout<<"\n removing duplicates (2nd if) row : "<<k+2;
					if(k!=nrows-1)
						removerows.push_back(k+2);		
			}
		}
	}	
	//cout<<"\n size of removerows vector: "<<removerows.size();
	delete[] removerows_arr;
	removerows_arr=(long*)malloc(sizeof(long)*removerows.size());
	sort( removerows.begin(),removerows.end() );
	//removerows.erase( unique( removerows.begin(), removerows.end() ), removerows.end() );
	for(k=0;k<removerows.size();k++){
		removerows_arr[k]=removerows[k];
	}
	fits_delete_rowlist(ftct, removerows_arr,removerows.size(), &status);	
	if(status) { LOG(INFO)<<"\:Error in deleting rows ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	removerows.clear();
	fits_close_file(ftct,&status);
	if(status) { LOG(INFO)<<"\:Error in closing tct file ";fits_report_error(stderr,status); return(EXIT_FAILURE);}

//filter 1
	fits_open_file(&ftct,"temp.tct1",READWRITE,&status);
	if(status) { LOG(INFO)<<"\:Error in opening tct file ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_movabs_hdu(ftct,2,NULL,&status);
	if(status) { LOG(INFO)<<"\:Error in moving to Extension in tct ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	//fits_insert_rows(ftct,0,1,&status);
	//if(status) { LOG(INFO)<<"\:\Error in inserting rows ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_get_num_rows(ftct,&nrows,&status);
	if(status) { LOG(INFO)<<"\:Error in reading no of rows.";fits_report_error(stderr,status); return(EXIT_FAILURE);}

	delete []INSTRUMENT_TIME_temp;
	delete []SPS_TIME_temp;
	delete []SPS_OBT_temp;


	INSTRUMENT_TIME_temp=(double*)malloc(sizeof(double)*nrows);
	SPS_TIME_temp=(double*)malloc(sizeof(double)*nrows);
	SPS_OBT_temp=(double*)malloc(sizeof(double)*nrows);


	fits_read_col(ftct,TDOUBLE,1,1,1,nrows,NULL,SPS_TIME_temp,NULL,&status);
	if(status) { LOG(INFO)<<"Error in reading SPS_TIME_temp";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_read_col(ftct,TDOUBLE,2,1,1,nrows,NULL,SPS_OBT_temp,NULL,&status);
	if(status) { LOG(INFO)<<"Error in reading SPS_OBT_temp";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_read_col(ftct,TDOUBLE,3,1,1,nrows,NULL,INSTRUMENT_TIME_temp,NULL,&status);
	if(status) { LOG(INFO)<<"Error in reading INSTRUMENT_TIME_temp";fits_report_error(stderr,status); return(EXIT_FAILURE);}

	for(k=0;k<nrows-1;k++){	
		if(abs(INSTRUMENT_TIME_temp[k+1]-INSTRUMENT_TIME_temp[k])<1){
			if( (SPS_TIME_temp[k]==0||SPS_TIME_temp[k+1]!=0)|| (SPS_TIME_temp[k]!=0||SPS_TIME_temp[k+1]==0)){
				if(SPS_TIME_temp[k]==0){
					INSTRUMENT_TIME_temp[k+1]=INSTRUMENT_TIME_temp[k];
					//fits_delete_rows(ftct,k+1,1,&status);
					removerows.push_back(k+1);
				}
				if(SPS_TIME_temp[k+1]==0){
					INSTRUMENT_TIME_temp[k]=INSTRUMENT_TIME_temp[k+1];
					//fits_delete_rows(ftct,k+1+1,1,&status);
					removerows.push_back(k+2);
				}
			}

			else{
					//fits_delete_rows(ftct,k+1,1,&status);
					removerows.push_back(k+1);
					//fits_delete_rows(ftct,k+1+1,1,&status);
					removerows.push_back(k+2);
			}
		}
	//}//temp	
//filter 2	
		if(abs(INSTRUMENT_TIME_temp[k+1]-INSTRUMENT_TIME_temp[k])<14.9){
			if(k!=0 || k!=nrows-1){
				//condition when k+1 sample is smaller
				if((INSTRUMENT_TIME_temp[k+1]-INSTRUMENT_TIME_temp[k])<0){
					if(abs(INSTRUMENT_TIME_temp[k+1]-INSTRUMENT_TIME_temp[k])<14.9){
						//fits_delete_rows(ftct,k+1+1,1,&status);
						removerows.push_back(k+2);
					}
					if(abs(INSTRUMENT_TIME_temp[k+1]-INSTRUMENT_TIME_temp[k])>17.2){
						//fits_delete_rows(ftct,k+1+1,1,&status);
						removerows.push_back(k+2);
					}
					//condition when k is larger
					if(abs(INSTRUMENT_TIME_temp[k]-INSTRUMENT_TIME_temp[k+1])<14.9){
						//fits_delete_rows(ftct,k+1,1,&status);
						removerows.push_back(k+1);
					}
					if(abs(INSTRUMENT_TIME_temp[k]-INSTRUMENT_TIME_temp[k+1])>17.2){
						//fits_delete_rows(ftct,k+1,1,&status);
						removerows.push_back(k+1);
					}

				
				
				}	
				

				//condition when k sample is smaller
				if((INSTRUMENT_TIME_temp[k+1]-INSTRUMENT_TIME_temp[k])>0){
					if(abs(INSTRUMENT_TIME_temp[k]-INSTRUMENT_TIME_temp[k-1])<14.9){
						//fits_delete_rows(ftct,k+1,1,&status);
						removerows.push_back(k+1);
					}
					if(abs(INSTRUMENT_TIME_temp[k]-INSTRUMENT_TIME_temp[k-1])>17.2){
						//fits_delete_rows(ftct,k+1,1,&status);
						removerows.push_back(k+1);
					}
					//condition when k+1 is larger
					if(abs(INSTRUMENT_TIME_temp[k+1]-INSTRUMENT_TIME_temp[k+2])<14.9){
						//fits_delete_rows(ftct,k+1+1,1,&status);
						removerows.push_back(k+2);
					}
					if(abs(INSTRUMENT_TIME_temp[k+1]-INSTRUMENT_TIME_temp[k+2])>17.2){
						//fits_delete_rows(ftct,k+1+1,1,&status);
						removerows.push_back(k+2);
					}


				}
			}	
		}
	}
	
	//filling first row in tct1 which is empty tct so that other samples can be generated by this sample
	delta_t=round(INSTRUMENT_TIME_temp[0]+1048576.0-last_instru_part0);
	SPS_TIME_temp[0]=last_spstime_part0+delta_t;
	SPS_OBT_temp[0]=last_spsobt_part0+delta_t;
	

	//generating missing samples
	for(k=0;k<nrows;k++){
		if(k==0){
			for(j=0;j<nrows;j++){
				if(SPS_TIME_temp[j]!=0){
					break;
				}
			}
			delta_instru=round(abs(INSTRUMENT_TIME_temp[j]-INSTRUMENT_TIME_temp[0]));
			SPS_TIME_temp[0]=SPS_TIME_temp[j]-delta_instru;
			SPS_OBT_temp[0]=SPS_OBT_temp[j]-delta_instru;
			
		}

		if(SPS_TIME_temp[k]==0){
			delta_instru=round(abs(INSTRUMENT_TIME_temp[k]-INSTRUMENT_TIME_temp[k-1]));
			SPS_TIME_temp[k]=SPS_TIME_temp[k-1]+delta_instru;
			SPS_OBT_temp[k]=SPS_OBT_temp[k-1]+delta_instru;
		}
	}	
	
	fits_write_col(ftct, TDOUBLE,1,1,1,nrows,SPS_TIME_temp,&status);
	if(status) { LOG(INFO)<<":Error in writing  SPS_TIME colum";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_write_col(ftct, TDOUBLE,2,1,1,nrows,SPS_OBT_temp,&status);
	if(status) { LOG(INFO)<<":Error in writing  SPS_OBT colum";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_write_col(ftct, TDOUBLE,3,1,1,nrows,INSTRUMENT_TIME_temp,&status);
	if(status) { LOG(INFO)<<":Error in writing  INSTRUMENT_TIME colum";fits_report_error(stderr,status); return(EXIT_FAILURE);}

	delete []removerows_arr;
	removerows_arr=(long*)malloc(sizeof(long)*removerows.size());
	std::sort( removerows.begin(),removerows.end() );
	removerows.erase( unique( removerows.begin(), removerows.end() ), removerows.end() );
	
	//cout<<"\nremoverows size: "<<removerows.size();
/*	for(k=0;k<removerows.size();k++){
			//cout<<"\nk: "<<k;
			//cout<<"\nremoverows: "<<removerows[k];
	//	if(removerows[k+1]<=removerows[k])
	//		cout<<"removerows["<<k+1<<"]:"<<removerows[k+1]<<" less than removerows["<<k<<"]:"<<removerows[k];
	}	
*/	for(k=0;k<removerows.size();k++){
		removerows_arr[k]=removerows[k];
	}
	fits_delete_rowlist(ftct, removerows_arr,removerows.size(), &status);	
	if(status) { LOG(INFO)<<"\:Error in deleting rows ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	removerows.clear();
	fits_close_file(ftct,&status);
	if(status) { LOG(INFO)<<"\:Error in closing tct file ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	//tct1 correction done
	break;

//}//temp switch ends here

	case 10:
	//cout<<"\nEXECUTING CASE 10";
	//correcting splited tct1	
	//sort( LAXPCTIME1.begin(), LAXPCTIME1.end() );
	//LAXPCTIME1.erase( unique( LAXPCTIME1.begin(), LAXPCTIME1.end() ), LAXPCTIME1.end() );
	vec_size=LAXPCTIME1.size();

	//modyfying tct file
	fits_open_file(&ftct,"temp.tct1",READWRITE,&status);
	if(status) { LOG(INFO)<<"\:Error in opening tct file ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_movabs_hdu(ftct,2,NULL,&status);
	if(status) { LOG(INFO)<<"\:Error in moving to Extension in tct ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_get_num_rows(ftct,&nrows,&status);
	if(status) { LOG(INFO)<<"\:Error in reading no of rows.";fits_report_error(stderr,status); return(EXIT_FAILURE);}

	delete []INSTRUMENT_TIME_temp;
   	INSTRUMENT_TIME_temp=(double*)malloc(sizeof(double)*vec_size);
	for(i=0;i<vec_size;i++){
		INSTRUMENT_TIME_temp[i]=LAXPCTIME1[i];
		//LOG(INFO)<<"%lf\,LAXPCTIME[i]);
	}
		
	fits_insert_rows(ftct, nrows,vec_size,&status);
	if(status) { LOG(INFO)<<"\:Error in adding rows to tct file..";fits_report_error(stderr,status); return(EXIT_FAILURE);}	
	fits_write_col(ftct, TDOUBLE,3,nrows+1,1,vec_size,INSTRUMENT_TIME_temp,&status);
	if(status) { LOG(INFO)<<"\:Error in writing decoded laxpctime ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_close_file(ftct,&status);
	if(status) { LOG(INFO)<<"\:Error in closing tct file ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	
	LAXPCTIME1.clear();
	INSTRUMENT_TIME.clear();
	
	fits_open_file(&ftct,"temp.tct1",READWRITE,&status);
	if(status) { LOG(INFO)<<"\:Error in opening tct file ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_movabs_hdu(ftct,2,NULL,&status);
	if(status) { LOG(INFO)<<"\:Error in moving to Extension in tct ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_get_num_rows(ftct,&nrows,&status);
	if(status) { LOG(INFO)<<"\:Error in reading no of rows.";fits_report_error(stderr,status); return(EXIT_FAILURE);}

	delete []INSTRUMENT_TIME_temp;
	delete []SPS_TIME_temp;
	delete []SPS_OBT_temp;


	INSTRUMENT_TIME_temp=(double*)malloc(sizeof(double)*nrows);
	SPS_TIME_temp=(double*)malloc(sizeof(double)*nrows);
	SPS_OBT_temp=(double*)malloc(sizeof(double)*nrows);

	fits_read_col(ftct,TDOUBLE,1,1,1,nrows,NULL,SPS_TIME_temp,NULL,&status);
	if(status) { LOG(INFO)<<"Error in reading SPS_TIME_temp";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_read_col(ftct,TDOUBLE,2,1,1,nrows,NULL,SPS_OBT_temp,NULL,&status);
	if(status) { LOG(INFO)<<"Error in reading SPS_OBT_temp";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_read_col(ftct,TDOUBLE,3,1,1,nrows,NULL,INSTRUMENT_TIME_temp,NULL,&status);
	if(status) { LOG(INFO)<<"Error in reading INSTRUMENT_TIME_temp";fits_report_error(stderr,status); return(EXIT_FAILURE);}

	(SPS_TIME_temp,SPS_OBT_temp,INSTRUMENT_TIME_temp,nrows);

	fits_write_col(ftct, TDOUBLE,1,1,1,nrows,SPS_TIME_temp,&status);
	if(status) { LOG(INFO)<<":Error in writing  SPS_TIME colum";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_write_col(ftct, TDOUBLE,2,1,1,nrows,SPS_OBT_temp,&status);
	if(status) { LOG(INFO)<<":Error in writing  SPS_OBT colum";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_write_col(ftct, TDOUBLE,3,1,1,nrows,INSTRUMENT_TIME_temp,&status);
	if(status) { LOG(INFO)<<":Error in writing  INSTRUMENT_TIME colum";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_close_file(ftct,&status);
	if(status) { LOG(INFO)<<":Error in closing tct file ";fits_report_error(stderr,status); return(EXIT_FAILURE);}

	fits_open_file(&ftct,"temp.tct1",READWRITE,&status);
	if(status) { LOG(INFO)<<"\:Error in opening tct file ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_movabs_hdu(ftct,2,NULL,&status);
	if(status) { LOG(INFO)<<"\:Error in moving to Extension in tct ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_get_num_rows(ftct,&nrows,&status);
	if(status) { LOG(INFO)<<"\:Error in reading no of rows.";fits_report_error(stderr,status); return(EXIT_FAILURE);}

	fits_read_col(ftct,TDOUBLE,1,1,1,nrows,NULL,SPS_TIME_temp,NULL,&status);
	if(status) { LOG(INFO)<<"Error in reading SPS_TIME_temp";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_read_col(ftct,TDOUBLE,2,1,1,nrows,NULL,SPS_OBT_temp,NULL,&status);
	if(status) { LOG(INFO)<<"Error in reading SPS_OBT_temp";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_read_col(ftct,TDOUBLE,3,1,1,nrows,NULL,INSTRUMENT_TIME_temp,NULL,&status);
	if(status) { LOG(INFO)<<"Error in reading INSTRUMENT_TIME_temp";fits_report_error(stderr,status); return(EXIT_FAILURE);}

	for(k=0;k<nrows-1;k++){
		if(abs(INSTRUMENT_TIME_temp[k+1]-INSTRUMENT_TIME_temp[k])<=0.001){
			//cout<<"\nk:"<<k;
			//cout<<"\n found duplicates...";
			if(SPS_TIME_temp[k]==0){
				//cout<<"\n removing duplicates... row : "<<k+1;
				removerows.push_back(k+1);
				
			}
			if(SPS_TIME_temp[k+1]==0){
				//cout<<"\n removing duplicates (2nd if) row : "<<k+2;
					if(k!=nrows-1)
						removerows.push_back(k+2);		
			}
		}
	}	
	//cout<<"\n size of removerows vector: "<<removerows.size();
	delete []removerows_arr;
	removerows_arr=(long*)malloc(sizeof(long)*removerows.size());
	sort( removerows.begin(),removerows.end() );
	//removerows.erase( unique( removerows.begin(), removerows.end() ), removerows.end() );
	for(k=0;k<removerows.size();k++){
		removerows_arr[k]=removerows[k];
	}
	fits_delete_rowlist(ftct, removerows_arr,removerows.size(), &status);	
	if(status) { LOG(INFO)<<":Error in deleting rows ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	removerows.clear();
	fits_close_file(ftct,&status);
	if(status) { LOG(INFO)<<":Error in closing tct file ";fits_report_error(stderr,status); return(EXIT_FAILURE);}

//filter 1
	fits_open_file(&ftct,"temp.tct1",READWRITE,&status);
	if(status) { LOG(INFO)<<"\:Error in opening tct file ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_movabs_hdu(ftct,2,NULL,&status);
	if(status) { LOG(INFO)<<"\:Error in moving to Extension in tct ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_get_num_rows(ftct,&nrows,&status);
	if(status) { LOG(INFO)<<"\:Error in reading no of rows.";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	//cout<<"\nnrows tct1######: "<<nrows;
	
	delete []INSTRUMENT_TIME_temp;
	delete []SPS_TIME_temp;
	delete []SPS_OBT_temp;


	INSTRUMENT_TIME_temp=(double*)malloc(sizeof(double)*nrows);
	SPS_TIME_temp=(double*)malloc(sizeof(double)*nrows);
	SPS_OBT_temp=(double*)malloc(sizeof(double)*nrows);


	fits_read_col(ftct,TDOUBLE,1,1,1,nrows,NULL,SPS_TIME_temp,NULL,&status);
	if(status) { LOG(INFO)<<":Error in reading SPS_TIME_temp";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_read_col(ftct,TDOUBLE,2,1,1,nrows,NULL,SPS_OBT_temp,NULL,&status);
	if(status) { LOG(INFO)<<":Error in reading SPS_OBT_temp";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_read_col(ftct,TDOUBLE,3,1,1,nrows,NULL,INSTRUMENT_TIME_temp,NULL,&status);
	if(status) { LOG(INFO)<<":Error in reading INSTRUMENT_TIME_temp";fits_report_error(stderr,status); return(EXIT_FAILURE);}

	for(k=0;k<nrows-1;k++){	
		if(abs(INSTRUMENT_TIME_temp[k+1]-INSTRUMENT_TIME_temp[k])<1){
			if( (SPS_TIME_temp[k]==0||SPS_TIME_temp[k+1]!=0)|| (SPS_TIME_temp[k]!=0||SPS_TIME_temp[k+1]==0)){
				if(SPS_TIME_temp[k]==0){
					INSTRUMENT_TIME_temp[k+1]=INSTRUMENT_TIME_temp[k];
					//fits_delete_rows(ftct,k+1,1,&status);
					removerows.push_back(k+1);
				}
				if(SPS_TIME_temp[k+1]==0){
					INSTRUMENT_TIME_temp[k]=INSTRUMENT_TIME_temp[k+1];
					//fits_delete_rows(ftct,k+1+1,1,&status);
					removerows.push_back(k+2);
				}
			}

			else{
					//fits_delete_rows(ftct,k+1,1,&status);
					removerows.push_back(k+1);
					//fits_delete_rows(ftct,k+1+1,1,&status);
					removerows.push_back(k+2);
			}
		}
	
//filter 2	
		if(abs(INSTRUMENT_TIME_temp[k+1]-INSTRUMENT_TIME_temp[k])<14.9){
			if(k!=0 || k!=nrows-1){
				//condition when k+1 sample is smaller
				if((INSTRUMENT_TIME_temp[k+1]-INSTRUMENT_TIME_temp[k])<0){
					if(abs(INSTRUMENT_TIME_temp[k+1]-INSTRUMENT_TIME_temp[k])<14.9){
						//fits_delete_rows(ftct,k+1+1,1,&status);
						removerows.push_back(k+2);
					}
					if(abs(INSTRUMENT_TIME_temp[k+1]-INSTRUMENT_TIME_temp[k])>17.2){
						//fits_delete_rows(ftct,k+1+1,1,&status);
						removerows.push_back(k+2);
					}
					//condition when k is larger
					if(abs(INSTRUMENT_TIME_temp[k]-INSTRUMENT_TIME_temp[k+1])<14.9){
						//fits_delete_rows(ftct,k+1,1,&status);
						removerows.push_back(k+1);
					}
					if(abs(INSTRUMENT_TIME_temp[k]-INSTRUMENT_TIME_temp[k+1])>17.2){
						//fits_delete_rows(ftct,k+1,1,&status);
						removerows.push_back(k+1);
					}

				
				
				}	
				

				//condition when k sample is smaller
				if((INSTRUMENT_TIME_temp[k+1]-INSTRUMENT_TIME_temp[k])>0){
					if(abs(INSTRUMENT_TIME_temp[k]-INSTRUMENT_TIME_temp[k-1])<14.9){
						//fits_delete_rows(ftct,k+1,1,&status);
						removerows.push_back(k+1);
					}
					if(abs(INSTRUMENT_TIME_temp[k]-INSTRUMENT_TIME_temp[k-1])>17.2){
						//fits_delete_rows(ftct,k+1,1,&status);
						removerows.push_back(k+1);
					}
					//condition when k+1 is larger
					if(abs(INSTRUMENT_TIME_temp[k+1]-INSTRUMENT_TIME_temp[k+2])<14.9){
						//fits_delete_rows(ftct,k+1+1,1,&status);
						removerows.push_back(k+2);
					}
					if(abs(INSTRUMENT_TIME_temp[k+1]-INSTRUMENT_TIME_temp[k+2])>17.2){
						//fits_delete_rows(ftct,k+1+1,1,&status);
						removerows.push_back(k+2);
					}


				}
			}	
		}
	}
	
	for(k=0;k<nrows;k++){
		if(k==0){
			for(j=0;j<nrows;j++){
				if(SPS_TIME_temp[j]!=0){
					break;
				}
			}
			delta_instru=round(abs(INSTRUMENT_TIME_temp[j]-INSTRUMENT_TIME_temp[0]));
			SPS_TIME_temp[0]=SPS_TIME_temp[j]-delta_instru;
			SPS_OBT_temp[0]=SPS_OBT_temp[j]-delta_instru;
			
		}

		if(SPS_TIME_temp[k]==0){
			delta_instru=round(abs(INSTRUMENT_TIME_temp[k]-INSTRUMENT_TIME_temp[k-1]));
			SPS_TIME_temp[k]=SPS_TIME_temp[k-1]+delta_instru;
			SPS_OBT_temp[k]=SPS_OBT_temp[k-1]+delta_instru;
		}
	}	
	
	fits_write_col(ftct, TDOUBLE,1,1,1,nrows,SPS_TIME_temp,&status);
	if(status) { LOG(INFO)<<":Error in writing  SPS_TIME colum";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_write_col(ftct, TDOUBLE,2,1,1,nrows,SPS_OBT_temp,&status);
	if(status) { LOG(INFO)<<":Error in writing  SPS_OBT colum";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_write_col(ftct, TDOUBLE,3,1,1,nrows,INSTRUMENT_TIME_temp,&status);
	if(status) { LOG(INFO)<<":Error in writing  INSTRUMENT_TIME colum";fits_report_error(stderr,status); return(EXIT_FAILURE);}

	delete []removerows_arr;
	removerows_arr=(long*)malloc(sizeof(long)*removerows.size());
	std::sort( removerows.begin(),removerows.end() );
	removerows.erase( unique( removerows.begin(), removerows.end() ), removerows.end() );
/*	for(k=0;k<removerows.size();k++){
			cout<<"\nremoverows: "<<removerows[k];
	//	if(removerows[k+1]<=removerows[k])
	//		cout<<"removerows["<<k+1<<"]:"<<removerows[k+1]<<" less than removerows["<<k<<"]:"<<removerows[k];
	}	
*/	for(k=0;k<removerows.size();k++){
		removerows_arr[k]=removerows[k];
	}
	fits_delete_rowlist(ftct, removerows_arr,removerows.size(), &status);	
	if(status) { LOG(INFO)<<"\:Error in deleting rows ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	removerows.clear();
	fits_close_file(ftct,&status);
	if(status) { LOG(INFO)<<"\:Error in closing tct file ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	//tct1 correction done

	//buffering first row in tct1
	fits_open_file(&ftemp1,"temp.tct1",READONLY,&status);
	if(status) { LOG(INFO)<<"\:Error in opening tct file ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_movabs_hdu(ftemp1,2,NULL,&status);
	if(status) { LOG(INFO)<<"\:Error in moving to Extension in tct ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_get_num_rows(ftemp1,&nrows,&status);
	if(status) { LOG(INFO)<<"\:Error in reading no of rows.";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	
	delete []INSTRUMENT_TIME_temp;
	delete []SPS_TIME_temp;
	delete []SPS_OBT_temp;


	INSTRUMENT_TIME_temp=(double*)malloc(sizeof(double)*nrows);
	SPS_TIME_temp=(double*)malloc(sizeof(double)*nrows);
	SPS_OBT_temp=(double*)malloc(sizeof(double)*nrows);

	fits_read_col(ftemp1,TDOUBLE,1,1,1,nrows,NULL,SPS_TIME_temp,NULL,&status);
	if(status) { LOG(INFO)<<"Error in reading SPS_TIME_temp";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_read_col(ftemp1,TDOUBLE,2,1,1,nrows,NULL,SPS_OBT_temp,NULL,&status);
	if(status) { LOG(INFO)<<"Error in reading SPS_TIME_temp";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_read_col(ftemp1,TDOUBLE,3,1,1,nrows,NULL,INSTRUMENT_TIME_temp,NULL,&status);
	if(status) { LOG(INFO)<<"Error in reading SPS_TIME_temp";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	
	first_instru_part1	=INSTRUMENT_TIME_temp[0];
	first_spstime_part1	=SPS_TIME_temp[0];
	first_spsobt_part1	=SPS_OBT_temp[0];
	//LOG(INFO)<<"\nfirst_instru_part1: %lf\tfirst_spstime_part1: %lf\tfirst_spsobt_part1: %lf",first_instru_part1,first_spstime_part1,first_spsobt_part1);
	fits_close_file(ftemp1,&status);




	//correcting splited tct0	
	sort( LAXPCTIME0.begin(), LAXPCTIME0.end() );
	LAXPCTIME0.erase( unique( LAXPCTIME0.begin(), LAXPCTIME0.end() ), LAXPCTIME0.end() );
	vec_size=LAXPCTIME0.size();
	cout<<"\nLAXPCTIME0 size:  "<<vec_size;
	//modyfying tct file
	fits_open_file(&ftct,"temp.tct0",READWRITE,&status);
	if(status) { LOG(INFO)<<"\:Error in opening tct file ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_movabs_hdu(ftct,2,NULL,&status);
	if(status) { LOG(INFO)<<"\:Error in moving to Extension in tct ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_get_num_rows(ftct,&nrows,&status);
	if(status) { LOG(INFO)<<"\:Error in reading no of rows.";fits_report_error(stderr,status); return(EXIT_FAILURE);}

	delete[] INSTRUMENT_TIME_temp;
   	INSTRUMENT_TIME_temp=(double*)malloc(sizeof(double)*vec_size);
	for(i=0;i<vec_size;i++){
		INSTRUMENT_TIME_temp[i]=LAXPCTIME0[i];
		//LOG(INFO)<<"%lf\,LAXPCTIME[i]);
	}
		
	fits_insert_rows(ftct, nrows,vec_size,&status);
	if(status) { LOG(INFO)<<"\:Error in adding rows to tct file..";fits_report_error(stderr,status); return(EXIT_FAILURE);}	
	fits_write_col(ftct, TDOUBLE,3,nrows+1,1,vec_size,INSTRUMENT_TIME_temp,&status);
	if(status) { LOG(INFO)<<"\:Error in writing decoded laxpctime ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_close_file(ftct,&status);
	if(status) { LOG(INFO)<<"\:Error in closing tct file ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	
	LAXPCTIME0.clear();
	INSTRUMENT_TIME.clear();
	
	
	fits_open_file(&ftct,"temp.tct0",READWRITE,&status);
	if(status) { LOG(INFO)<<"\:Error in opening tct file ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_movabs_hdu(ftct,2,NULL,&status);
	if(status) { LOG(INFO)<<"\:Error in moving to Extension in tct ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_get_num_rows(ftct,&nrows,&status);
	if(status) { LOG(INFO)<<"\:Error in reading no of rows.";fits_report_error(stderr,status); return(EXIT_FAILURE);}

	delete []INSTRUMENT_TIME_temp;
	delete []SPS_TIME_temp;
	delete []SPS_OBT_temp;


	INSTRUMENT_TIME_temp=(double*)malloc(sizeof(double)*nrows);
	SPS_TIME_temp=(double*)malloc(sizeof(double)*nrows);
	SPS_OBT_temp=(double*)malloc(sizeof(double)*nrows);

	fits_read_col(ftct,TDOUBLE,1,1,1,nrows,NULL,SPS_TIME_temp,NULL,&status);
	if(status) { LOG(INFO)<<"Error in reading SPS_TIME_temp";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_read_col(ftct,TDOUBLE,2,1,1,nrows,NULL,SPS_OBT_temp,NULL,&status);
	if(status) { LOG(INFO)<<"Error in reading SPS_OBT_temp";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_read_col(ftct,TDOUBLE,3,1,1,nrows,NULL,INSTRUMENT_TIME_temp,NULL,&status);
	if(status) { LOG(INFO)<<"Error in reading INSTRUMENT_TIME_temp";fits_report_error(stderr,status); return(EXIT_FAILURE);}

	heapSort(SPS_TIME_temp,SPS_OBT_temp,INSTRUMENT_TIME_temp,nrows);

	fits_write_col(ftct, TDOUBLE,1,1,1,nrows,SPS_TIME_temp,&status);
	if(status) { LOG(INFO)<<":Error in writing  SPS_TIME colum";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_write_col(ftct, TDOUBLE,2,1,1,nrows,SPS_OBT_temp,&status);
	if(status) { LOG(INFO)<<":Error in writing  SPS_OBT colum";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_write_col(ftct, TDOUBLE,3,1,1,nrows,INSTRUMENT_TIME_temp,&status);
	if(status) { LOG(INFO)<<":Error in writing  INSTRUMENT_TIME colum";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_close_file(ftct,&status);
	if(status) { LOG(INFO)<<":Error in closing tct file ";fits_report_error(stderr,status); return(EXIT_FAILURE);}

	fits_open_file(&ftct,"temp.tct0",READWRITE,&status);
	if(status) { LOG(INFO)<<"\:Error in opening tct file ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_movabs_hdu(ftct,2,NULL,&status);
	if(status) { LOG(INFO)<<"\:Error in moving to Extension in tct ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_get_num_rows(ftct,&nrows,&status);
	if(status) { LOG(INFO)<<"\:Error in reading no of rows.";fits_report_error(stderr,status); return(EXIT_FAILURE);}

	fits_read_col(ftct,TDOUBLE,1,1,1,nrows,NULL,SPS_TIME_temp,NULL,&status);
	if(status) { LOG(INFO)<<"Error in reading SPS_TIME_temp";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_read_col(ftct,TDOUBLE,2,1,1,nrows,NULL,SPS_OBT_temp,NULL,&status);
	if(status) { LOG(INFO)<<"Error in reading SPS_OBT_temp";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_read_col(ftct,TDOUBLE,3,1,1,nrows,NULL,INSTRUMENT_TIME_temp,NULL,&status);
	if(status) { LOG(INFO)<<"Error in reading INSTRUMENT_TIME_temp";fits_report_error(stderr,status); return(EXIT_FAILURE);}

	for(k=0;k<nrows-1;k++){
		if(abs(INSTRUMENT_TIME_temp[k+1]-INSTRUMENT_TIME_temp[k])<=0.001){
			//cout<<"\nk:"<<k;
			//cout<<"\n found duplicates...";
			if(SPS_TIME_temp[k]==0){
				//cout<<"\n removing duplicates... row : "<<k+1;
				removerows.push_back(k+1);
				
			}
			if(SPS_TIME_temp[k+1]==0){
				//cout<<"\n removing duplicates (2nd if) row : "<<k+2;
					if(k!=nrows-1)
						removerows.push_back(k+2);		
			}
		}
	}	
	//cout<<"\n size of removerows vector: "<<removerows.size();
	delete[] removerows_arr;
	removerows_arr=(long*)malloc(sizeof(long)*removerows.size());
	sort( removerows.begin(),removerows.end() );
	//removerows.erase( unique( removerows.begin(), removerows.end() ), removerows.end() );
	for(k=0;k<removerows.size();k++){
		removerows_arr[k]=removerows[k];
	}
	fits_delete_rowlist(ftct, removerows_arr,removerows.size(), &status);	
	if(status) { LOG(INFO)<<"Error in deleting rows ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	removerows.clear();
	fits_close_file(ftct,&status);
	if(status) { LOG(INFO)<<"Error in closing tct file ";fits_report_error(stderr,status); return(EXIT_FAILURE);}

//filter 1
	fits_open_file(&ftct,"temp.tct0",READWRITE,&status);
	if(status) { LOG(INFO)<<"Error in opening tct file ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_movabs_hdu(ftct,2,NULL,&status);
	if(status) { LOG(INFO)<<"Error in moving to Extension in tct ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	//fits_insert_rows(ftct,0,1,&status);
	//if(status) { LOG(INFO)<<"\:\Error in inserting rows.";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_get_num_rows(ftct,&nrows,&status);
	if(status) { LOG(INFO)<<"Error in reading no of rows.";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	
	delete []INSTRUMENT_TIME_temp;
	delete []SPS_TIME_temp;
	delete []SPS_OBT_temp;


	INSTRUMENT_TIME_temp=(double*)malloc(sizeof(double)*nrows);
	SPS_TIME_temp=(double*)malloc(sizeof(double)*nrows);
	SPS_OBT_temp=(double*)malloc(sizeof(double)*nrows);

	fits_read_col(ftct,TDOUBLE,1,1,1,nrows,NULL,SPS_TIME_temp,NULL,&status);
	if(status) { LOG(INFO)<<"Error in reading SPS_TIME_temp";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_read_col(ftct,TDOUBLE,2,1,1,nrows,NULL,SPS_OBT_temp,NULL,&status);
	if(status) { LOG(INFO)<<"Error in reading SPS_OBT_temp";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_read_col(ftct,TDOUBLE,3,1,1,nrows,NULL,INSTRUMENT_TIME_temp,NULL,&status);
	if(status) { LOG(INFO)<<"Error in reading INSTRUMENT_TIME_temp";fits_report_error(stderr,status); return(EXIT_FAILURE);}

	for(k=0;k<nrows-1;k++){	
		if(abs(INSTRUMENT_TIME_temp[k+1]-INSTRUMENT_TIME_temp[k])<1){
			if( (SPS_TIME_temp[k]==0||SPS_TIME_temp[k+1]!=0)|| (SPS_TIME_temp[k]!=0||SPS_TIME_temp[k+1]==0)){
				if(SPS_TIME_temp[k]==0){
					INSTRUMENT_TIME_temp[k+1]=INSTRUMENT_TIME_temp[k];
					//fits_delete_rows(ftct,k+1,1,&status);
					removerows.push_back(k+1);
				}
				if(SPS_TIME_temp[k+1]==0){
					INSTRUMENT_TIME_temp[k]=INSTRUMENT_TIME_temp[k+1];
					//fits_delete_rows(ftct,k+1+1,1,&status);
					removerows.push_back(k+2);
				}
			}

			else{
					//fits_delete_rows(ftct,k+1,1,&status);
					removerows.push_back(k+1);
					//fits_delete_rows(ftct,k+1+1,1,&status);
					removerows.push_back(k+2);
			}
		}
	
//filter 2	
		if(abs(INSTRUMENT_TIME_temp[k+1]-INSTRUMENT_TIME_temp[k])<14.9){
			if(k!=0 || k!=nrows-1){
				//condition when k+1 sample is smaller
				if((INSTRUMENT_TIME_temp[k+1]-INSTRUMENT_TIME_temp[k])<0){
					if(abs(INSTRUMENT_TIME_temp[k+1]-INSTRUMENT_TIME_temp[k])<14.9){
						//fits_delete_rows(ftct,k+1+1,1,&status);
						removerows.push_back(k+2);
					}
					if(abs(INSTRUMENT_TIME_temp[k+1]-INSTRUMENT_TIME_temp[k])>17.2){
						//fits_delete_rows(ftct,k+1+1,1,&status);
						removerows.push_back(k+2);
					}
					//condition when k is larger
					if(abs(INSTRUMENT_TIME_temp[k]-INSTRUMENT_TIME_temp[k+1])<14.9){
						//fits_delete_rows(ftct,k+1,1,&status);
						removerows.push_back(k+1);
					}
					if(abs(INSTRUMENT_TIME_temp[k]-INSTRUMENT_TIME_temp[k+1])>17.2){
						//fits_delete_rows(ftct,k+1,1,&status);
						removerows.push_back(k+1);
					}

				
				
				}	
				

				//condition when k sample is smaller
				if((INSTRUMENT_TIME_temp[k+1]-INSTRUMENT_TIME_temp[k])>0){
					if(abs(INSTRUMENT_TIME_temp[k]-INSTRUMENT_TIME_temp[k-1])<14.9){
						//fits_delete_rows(ftct,k+1,1,&status);
						removerows.push_back(k+1);
					}
					if(abs(INSTRUMENT_TIME_temp[k]-INSTRUMENT_TIME_temp[k-1])>17.2){
						//fits_delete_rows(ftct,k+1,1,&status);
						removerows.push_back(k+1);
					}
					//condition when k+1 is larger
					if(abs(INSTRUMENT_TIME_temp[k+1]-INSTRUMENT_TIME_temp[k+2])<14.9){
						//fits_delete_rows(ftct,k+1+1,1,&status);
						removerows.push_back(k+2);
					}
					if(abs(INSTRUMENT_TIME_temp[k+1]-INSTRUMENT_TIME_temp[k+2])>17.2){
						//fits_delete_rows(ftct,k+1+1,1,&status);
						removerows.push_back(k+2);
					}


				}
			}	
		}
	}
	
	//filling last row in tct0 which is empty tct so that other samples can be generated by this sample
	delta_t=round(INSTRUMENT_TIME_temp[0]-(first_instru_part1+1048576.0));
	SPS_TIME_temp[0]=first_spstime_part1+delta_t;
	SPS_OBT_temp[0]=first_spsobt_part1+delta_t;

	//delta_t=round(first_instru_part1-INSTRUMENT_TIME_temp[nrows-1]+1048576.0);
	//SPS_TIME_temp[nrows-1]=first_spstime_part1-delta_t;
	//SPS_OBT_temp[nrows-1]=first_spsobt_part1-delta_t;


	for(k=0;k<nrows;k++){
		if(k==0){
			for(j=0;j<nrows;j++){
				if(SPS_TIME_temp[j]!=0){
					break;
				}
			}
			delta_instru=round(abs(INSTRUMENT_TIME_temp[j]-INSTRUMENT_TIME_temp[0]));
			SPS_TIME_temp[0]=SPS_TIME_temp[j]-delta_instru;
			SPS_OBT_temp[0]=SPS_OBT_temp[j]-delta_instru;
			
		}

		if(SPS_TIME_temp[k]==0){
			delta_instru=round(abs(INSTRUMENT_TIME_temp[k]-INSTRUMENT_TIME_temp[k-1]));
			SPS_TIME_temp[k]=SPS_TIME_temp[k-1]+delta_instru;
			SPS_OBT_temp[k]=SPS_OBT_temp[k-1]+delta_instru;
		}
	}	

	fits_write_col(ftct, TDOUBLE,1,1,1,nrows,SPS_TIME_temp,&status);
	if(status) { LOG(INFO)<<"Error in writing  SPS_TIME colum";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_write_col(ftct, TDOUBLE,2,1,1,nrows,SPS_OBT_temp,&status);
	if(status) { LOG(INFO)<<"Error in writing  SPS_OBT colum";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_write_col(ftct, TDOUBLE,3,1,1,nrows,INSTRUMENT_TIME_temp,&status);
	if(status) { LOG(INFO)<<"Error in writing  INSTRUMENT_TIME colum";fits_report_error(stderr,status); return(EXIT_FAILURE);}

	delete[] removerows_arr;
	removerows_arr=(long*)malloc(sizeof(long)*removerows.size());
	std::sort( removerows.begin(),removerows.end() );
	removerows.erase( unique( removerows.begin(), removerows.end() ), removerows.end() );
/*	for(k=0;k<removerows.size();k++){
			cout<<"\nremoverows: "<<removerows[k];
	//	if(removerows[k+1]<=removerows[k])
	//		cout<<"removerows["<<k+1<<"]:"<<removerows[k+1]<<" less than removerows["<<k<<"]:"<<removerows[k];
	}	
*/	for(k=0;k<removerows.size();k++){
		removerows_arr[k]=removerows[k];
	}
	fits_delete_rowlist(ftct, removerows_arr,removerows.size(), &status);	
	if(status) { LOG(INFO)<<"\:\Error in deleting rows ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	removerows.clear();
	fits_close_file(ftct,&status);
	if(status) { LOG(INFO)<<"\:\Error in closing tct file ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	//tct0 correction done
	break;
}//switch ends here	
	//merging two splited tct files
	fits_open_file(&ftct,tct,READWRITE,&status);
	if(status) { LOG(INFO)<<"\:\Error in opening input tct file ";fits_report_error(stderr,status); return(EXIT_FAILURE);}

	fits_open_file(&ftemp0,"temp.tct0",READWRITE,&status);
	if(status) { LOG(INFO)<<"\:\Error in opening tct file ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_movabs_hdu(ftemp0,2,NULL,&status);
	if(status) { LOG(INFO)<<"\:\Error in moving to Extension in tct ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_get_num_rows(ftemp0,&nrows,&status);
	if(status) { LOG(INFO)<<"\:\Error in reading no of rows.";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_movabs_hdu(ftct,2,NULL,&status);
	if(status) { LOG(INFO)<<"\:\Error in moving to Extension in tct ";fits_report_error(stderr,status); return(EXIT_FAILURE);}

	start_new=nrows;
	for(k=0;k<nrows;k++){
			fits_read_tblbytes(ftemp0, k+1, 1,naxis1,tblbytes, &status);
			if(status) { LOG(INFO)<<"\:\Error in reading table bytes ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
			fits_write_tblbytes(ftct,k+1,1,naxis1,tblbytes,&status);
			if(status) { LOG(INFO)<<"\:\Error in writing table bytes ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	}


	fits_close_file(ftemp0,&status);
	fits_close_file(ftct,&status);

	fits_open_file(&ftct,tct,READWRITE,&status);
	if(status) { LOG(INFO)<<"\:\Error in opening input tct file ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_movabs_hdu(ftct,2,NULL,&status);
	if(status) { LOG(INFO)<<"\:\Error in moving to Extension in tct ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	
	fits_open_file(&ftemp1,"temp.tct1",READWRITE,&status);
	if(status) { LOG(INFO)<<"\:\Error in opening input tct file ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_movabs_hdu(ftemp1,2,NULL,&status);
	if(status) { LOG(INFO)<<"\:\Error in moving to Extension in tct ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_get_num_rows(ftemp1,&nrows,&status);
	if(status) { LOG(INFO)<<"\:\Error in reading no of rows.";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	for(k=0;k<nrows;k++){
			fits_read_tblbytes(ftemp1, k+1, 1,naxis1,tblbytes, &status);
			if(status) { LOG(INFO)<<"\:\Error in reading table bytes ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
			fits_write_tblbytes(ftct,start_new+k+1,1,naxis1,tblbytes,&status);
			if(status) { LOG(INFO)<<"\:\Error in writing table bytes ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	}

	//cout<<"\n numrows: "<<nrows;
	fits_close_file(ftct,&status);
	if(status) { LOG(INFO)<<"\:\Error in closing tct ";fits_report_error(stderr,status); return(EXIT_FAILURE);}

	fits_close_file(ftemp1,&status);
	if(status) { LOG(INFO)<<"\:\Error in closing tct ";fits_report_error(stderr,status); return(EXIT_FAILURE);}


}//rollover =1 ends here

//###############################################################################################################################################################
//When no rollover occurs in input tct file	
if(rollover_flag==0 && execute==00){	
	//LOG(INFO)<<"\: NO LAXPCTIME,INSTRUMENT_TIME ROLLOVER FOUND ";
	LOG(INFO)<<": NO LAXPCTIME,INSTRUMENT_TIME ROLLOVER FOUND ";
	//cout<<"\nNO LAXPCTIME ROLLOVER FOUND";
	sort( LAXPCTIME.begin(), LAXPCTIME.end() );
	LAXPCTIME.erase( unique( LAXPCTIME.begin(), LAXPCTIME.end() ), LAXPCTIME.end() );
	vec_size=LAXPCTIME.size();
	//LOG(INFO)<<"\nlast LAXPCTIME: %lf",LAXPCTIME[vec_size-1]);
	//LOG(INFO)<<"\nlast+1 LAXPCTIME: %lf",LAXPCTIME[vec_size+2]);

	
	//modyfying tct file
	fits_open_file(&ftct,tct,READWRITE,&status);
	if(status) { LOG(INFO)<<"\:Error in opening tct file ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_movabs_hdu(ftct,2,NULL,&status);
	if(status) { LOG(INFO)<<"\:Error in moving to Extension in tct ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_get_num_rows(ftct,&nrows,&status);
	if(status) { LOG(INFO)<<"\:\Error in reading no of rows.";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	
	//SPS_TIME_temp=(double*)malloc(sizeof(double)*nrows);
	//SPS_OBT_temp=(double*)malloc(sizeof(double)*nrows);
	//cout<<"\nvec_size: "<<vec_size;
	delete[] INSTRUMENT_TIME_temp;
   	INSTRUMENT_TIME_temp=(double*)malloc(sizeof(double)*vec_size);
	for(i=0;i<vec_size;i++){
		INSTRUMENT_TIME_temp[i]=LAXPCTIME[i];
		//LOG(INFO)<<"%lf\,LAXPCTIME[i]);
	}
	//LOG(INFO)<<"\n last INSTRUMENT_TIME_temp: %lf",INSTRUMENT_TIME_temp[vec_size-1]);
	fits_insert_rows(ftct, nrows,vec_size,&status);
	if(status) { LOG(INFO)<<"\:Error in adding rows to tct file..";fits_report_error(stderr,status); return(EXIT_FAILURE);}	
	fits_write_col(ftct, TDOUBLE,3,nrows+1,1,vec_size,INSTRUMENT_TIME_temp,&status);
	if(status) { LOG(INFO)<<"\:Error in writing decoded laxpctime ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_close_file(ftct,&status);
	if(status) { LOG(INFO)<<"\:Error in closing tct file ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	
	LAXPCTIME.clear();
	INSTRUMENT_TIME.clear();

	
	fits_open_file(&ftct,tct,READWRITE,&status);
	if(status) { LOG(INFO)<<"Error in opening tct file ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_movabs_hdu(ftct,2,NULL,&status);
	if(status) { LOG(INFO)<<"Error in moving to Extension in tct ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_get_num_rows(ftct,&nrows,&status);
	if(status) { LOG(INFO)<<"Error in reading no of rows.";fits_report_error(stderr,status); return(EXIT_FAILURE);}

	delete[] INSTRUMENT_TIME_temp;
	delete[] SPS_TIME_temp;
	delete[] SPS_OBT_temp;


	INSTRUMENT_TIME_temp=(double*)malloc(sizeof(double)*nrows);
	SPS_TIME_temp=(double*)malloc(sizeof(double)*nrows);
	SPS_OBT_temp=(double*)malloc(sizeof(double)*nrows);

	fits_read_col(ftct,TDOUBLE,1,1,1,nrows,NULL,SPS_TIME_temp,NULL,&status);
	if(status) { LOG(INFO)<<"Error in reading SPS_TIME_temp";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_read_col(ftct,TDOUBLE,2,1,1,nrows,NULL,SPS_OBT_temp,NULL,&status);
	if(status) { LOG(INFO)<<"Error in reading SPS_OBT_temp";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_read_col(ftct,TDOUBLE,3,1,1,nrows,NULL,INSTRUMENT_TIME_temp,NULL,&status);
	if(status) { LOG(INFO)<<"Error in reading INSTRUMENT_TIME_temp";fits_report_error(stderr,status); return(EXIT_FAILURE);}

	heapSort(SPS_TIME_temp,SPS_OBT_temp,INSTRUMENT_TIME_temp,nrows);

	fits_write_col(ftct, TDOUBLE,1,1,1,nrows,SPS_TIME_temp,&status);
	if(status) { LOG(INFO)<<":Error in writing  SPS_TIME colum";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_write_col(ftct, TDOUBLE,2,1,1,nrows,SPS_OBT_temp,&status);
	if(status) { LOG(INFO)<<":Error in writing  SPS_OBT colum";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_write_col(ftct, TDOUBLE,3,1,1,nrows,INSTRUMENT_TIME_temp,&status);
	if(status) { LOG(INFO)<<":Error in writing  INSTRUMENT_TIME colum";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_close_file(ftct,&status);
	if(status) { LOG(INFO)<<":Error in closing tct file ";fits_report_error(stderr,status); return(EXIT_FAILURE);}

	
	fits_open_file(&ftct,tct,READWRITE,&status);
	if(status) { LOG(INFO)<<"\:Error in opening tct file ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_movabs_hdu(ftct,2,NULL,&status);
	if(status) { LOG(INFO)<<"\:Error in moving to Extension in tct ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_get_num_rows(ftct,&nrows,&status);
	if(status) { LOG(INFO)<<"\:Error in reading no of rows.";fits_report_error(stderr,status); return(EXIT_FAILURE);}

	fits_read_col(ftct,TDOUBLE,1,1,1,nrows,NULL,SPS_TIME_temp,NULL,&status);
	if(status) { LOG(INFO)<<"\:";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_read_col(ftct,TDOUBLE,2,1,1,nrows,NULL,SPS_OBT_temp,NULL,&status);
	if(status) { LOG(INFO)<<"\:";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_read_col(ftct,TDOUBLE,3,1,1,nrows,NULL,INSTRUMENT_TIME_temp,NULL,&status);
	if(status) { LOG(INFO)<<"\:";fits_report_error(stderr,status); return(EXIT_FAILURE);}

	for(k=0;k<nrows-1;k++){
		if(abs(INSTRUMENT_TIME_temp[k+1]-INSTRUMENT_TIME_temp[k])<=0.001){
			//cout<<"\nk:"<<k;
			//cout<<"\n found duplicates...";
			if(SPS_TIME_temp[k]==0){
				//cout<<"\n removing duplicates... row : "<<k+1;
				removerows.push_back(k+1);
				
			}
			if(SPS_TIME_temp[k+1]==0){
				//cout<<"\n removing duplicates (2nd if) row : "<<k+2;
					if(k!=nrows-1)
						removerows.push_back(k+2);		
			}
		}
	}	
	//cout<<"\n size of removerows vector: "<<removerows.size();
	delete [] removerows_arr;
	removerows_arr=(long*)malloc(sizeof(long)*removerows.size());
	sort( removerows.begin(),removerows.end() );
	//removerows.erase( unique( removerows.begin(), removerows.end() ), removerows.end() );
	for(k=0;k<removerows.size();k++){
		removerows_arr[k]=removerows[k];
	}
	fits_delete_rowlist(ftct, removerows_arr,removerows.size(), &status);	
	if(status) { LOG(INFO)<<"\:Error in deleting rows ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	removerows.clear();
	fits_close_file(ftct,&status);
	if(status) { LOG(INFO)<<"\:Error in closing tct file ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	removerows.clear();

//filter 1
	fits_open_file(&ftct,tct,READWRITE,&status);
	if(status) { LOG(INFO)<<"\:Error in opening tct file ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_movabs_hdu(ftct,2,NULL,&status);
	if(status) { LOG(INFO)<<"\:Error in moving to Extension in tct ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_get_num_rows(ftct,&nrows,&status);
	if(status) { LOG(INFO)<<"\:Error in reading no of rows.";fits_report_error(stderr,status); return(EXIT_FAILURE);}

	fits_read_col(ftct,TDOUBLE,1,1,1,nrows,NULL,SPS_TIME_temp,NULL,&status);
	if(status) { LOG(INFO)<<"\: Error in reading col1";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_read_col(ftct,TDOUBLE,2,1,1,nrows,NULL,SPS_OBT_temp,NULL,&status);
	if(status) { LOG(INFO)<<"\: Error in reading col2";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_read_col(ftct,TDOUBLE,3,1,1,nrows,NULL,INSTRUMENT_TIME_temp,NULL,&status);
	if(status) { LOG(INFO)<<"\: Error in reading col3";fits_report_error(stderr,status); return(EXIT_FAILURE);}

	for(k=0;k<nrows-1;k++){	
		if(abs(INSTRUMENT_TIME_temp[k+1]-INSTRUMENT_TIME_temp[k])<1){
			if( (SPS_TIME_temp[k]==0||SPS_TIME_temp[k+1]!=0)|| (SPS_TIME_temp[k]!=0||SPS_TIME_temp[k+1]==0)){
				if(SPS_TIME_temp[k]==0){
					INSTRUMENT_TIME_temp[k+1]=INSTRUMENT_TIME_temp[k];
					//fits_delete_rows(ftct,k+1,1,&status);
					removerows.push_back(k+1);
				}
				if(SPS_TIME_temp[k+1]==0){
					INSTRUMENT_TIME_temp[k]=INSTRUMENT_TIME_temp[k+1];
					//fits_delete_rows(ftct,k+1+1,1,&status);
					removerows.push_back(k+2);
				}
			}

			else{
					//fits_delete_rows(ftct,k+1,1,&status);
					removerows.push_back(k+1);
					//fits_delete_rows(ftct,k+1+1,1,&status);
					removerows.push_back(k+2);
			}
		}
		

//filter 2	
		if(abs(INSTRUMENT_TIME_temp[k+1]-INSTRUMENT_TIME_temp[k])<14.9){
			if(k!=0 || k!=nrows-1){
				//condition when k+1 sample is smaller
				if((INSTRUMENT_TIME_temp[k+1]-INSTRUMENT_TIME_temp[k])<0){
					if(abs(INSTRUMENT_TIME_temp[k+1]-INSTRUMENT_TIME_temp[k])<14.9){
						//fits_delete_rows(ftct,k+1+1,1,&status);
						removerows.push_back(k+2);
					}
					if(abs(INSTRUMENT_TIME_temp[k+1]-INSTRUMENT_TIME_temp[k])>17.2){
						//fits_delete_rows(ftct,k+1+1,1,&status);
						removerows.push_back(k+2);
					}
					//condition when k is larger
					if(abs(INSTRUMENT_TIME_temp[k]-INSTRUMENT_TIME_temp[k+1])<14.9){
						//fits_delete_rows(ftct,k+1,1,&status);
						removerows.push_back(k+1);
					}
					if(abs(INSTRUMENT_TIME_temp[k]-INSTRUMENT_TIME_temp[k+1])>17.2){
						//fits_delete_rows(ftct,k+1,1,&status);
						removerows.push_back(k+1);
					}

				
				
				}	
				

				//condition when k sample is smaller
				if((INSTRUMENT_TIME_temp[k+1]-INSTRUMENT_TIME_temp[k])>0){
					if(abs(INSTRUMENT_TIME_temp[k]-INSTRUMENT_TIME_temp[k-1])<14.9){
						//fits_delete_rows(ftct,k+1,1,&status);
						removerows.push_back(k+1);
					}
					if(abs(INSTRUMENT_TIME_temp[k]-INSTRUMENT_TIME_temp[k-1])>17.2){
						//fits_delete_rows(ftct,k+1,1,&status);
						removerows.push_back(k+1);
					}
					//condition when k+1 is larger
					if(abs(INSTRUMENT_TIME_temp[k+1]-INSTRUMENT_TIME_temp[k+2])<14.9){
						//fits_delete_rows(ftct,k+1+1,1,&status);
						removerows.push_back(k+2);
					}
					if(abs(INSTRUMENT_TIME_temp[k+1]-INSTRUMENT_TIME_temp[k+2])>17.2){
						//fits_delete_rows(ftct,k+1+1,1,&status);
						removerows.push_back(k+2);
					}


				}
			}	
		}
	}


	for(k=0;k<nrows;k++){
		if(k==0){
			for(j=0;j<nrows;j++){
				if(SPS_TIME_temp[j]!=0){
					break;
				}
			}
			delta_instru=round(abs(INSTRUMENT_TIME_temp[j]-INSTRUMENT_TIME_temp[0]));
			SPS_TIME_temp[0]=SPS_TIME_temp[j]-delta_instru;
			SPS_OBT_temp[0]=SPS_OBT_temp[j]+delta_instru;
			
		}

		if(SPS_TIME_temp[k]==0){
			delta_instru=round(abs(INSTRUMENT_TIME_temp[k]-INSTRUMENT_TIME_temp[k-1]));
			SPS_TIME_temp[k]=SPS_TIME_temp[k-1]+delta_instru;
			SPS_OBT_temp[k]=SPS_OBT_temp[k-1]+delta_instru;
		}
	}	

	fits_write_col(ftct, TDOUBLE,1,1,1,nrows,SPS_TIME_temp,&status);
	if(status) { LOG(INFO)<<":Error in writing  SPS_TIME colum";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_write_col(ftct, TDOUBLE,2,1,1,nrows,SPS_OBT_temp,&status);
	if(status) { LOG(INFO)<<":Error in writing  SPS_OBT colum";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_write_col(ftct, TDOUBLE,3,1,1,nrows,INSTRUMENT_TIME_temp,&status);
	if(status) { LOG(INFO)<<":Error in writing  INSTRUMENT_TIME colum";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	delete[] removerows_arr;
	removerows_arr=(long*)malloc(sizeof(long)*removerows.size());
	std::sort( removerows.begin(),removerows.end() );
	removerows.erase( unique( removerows.begin(), removerows.end() ), removerows.end() );
	for(k=0;k<removerows.size();k++){
			//cout<<"\nremoverows: "<<removerows[k];
	//	if(removerows[k+1]<=removerows[k])
	//		cout<<"removerows["<<k+1<<"]:"<<removerows[k+1]<<" less than removerows["<<k<<"]:"<<removerows[k];
	}	
	for(k=0;k<removerows.size();k++){
		removerows_arr[k]=removerows[k];
	}
	fits_delete_rowlist(ftct, removerows_arr,removerows.size(), &status);	
	if(status) { LOG(INFO)<<"\:Error in deleting rows ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	removerows.clear();
	fits_close_file(ftct,&status);
	if(status) { LOG(INFO)<<"\:Error in closing tct file ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
}//rollover==0 if ends here




//modifing primary header of output tct file
	fits_open_file(&ftct,tct,READWRITE,&status);
	if(status) { LOG(INFO)<<"\:Error in opening input tct file ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_movabs_hdu(ftct,2,NULL,&status);
	if(status) { LOG(INFO)<<"\:Error in moving to Extension in tct ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_get_num_rows(ftct,&nrows,&status);
	if(status) { LOG(INFO)<<"\:Error in reading no of rows.";fits_report_error(stderr,status); return(EXIT_FAILURE);}

	delete []SPS_TIME_temp;
	SPS_TIME_temp=(double*)malloc(sizeof(double)*nrows);

	fits_read_col(ftct,TDOUBLE,1,1,1,nrows,NULL,SPS_TIME_temp,NULL,&status);
	if(status) { LOG(INFO)<<"\:";fits_report_error(stderr,status); return(EXIT_FAILURE);}

	double tstart=SPS_TIME_temp[0];
	double tstop=SPS_TIME_temp[nrows-1];
	float timedel=SPS_TIME_temp[1]-SPS_TIME_temp[0];
	long tstarti=SPS_TIME_temp[0];
	long tstopi=SPS_TIME_temp[nrows-1];
	float tstartf=tstart-tstarti;
	float tstopf=tstop-tstopi;

	fits_movabs_hdu(ftct,1,NULL,&status);
	if(status) { LOG(INFO)<<"\:Error in moving to Extension in tct ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_update_key(ftct,TDOUBLE,"TSTART",&tstart,"observation start time",&status);
	if(status) { LOG(INFO)<<"\:";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_update_key(ftct,TLONG,"TSTARTI",&tstarti,"integer portion of start time",&status);
	if(status) { LOG(INFO)<<"\:";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_update_key(ftct,TFLOAT,"TSTARTF",&tstartf,"fractional observation start time",&status);
	if(status) { LOG(INFO)<<"\:";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_update_key(ftct,TDOUBLE,"TSTOP",&tstop,"observation stop time",&status);
	if(status) { LOG(INFO)<<"\:";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_update_key(ftct,TLONG,"TSTOPI",&tstopi,"Integer observation stop time",&status);
	if(status) { LOG(INFO)<<"\:";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_update_key(ftct,TFLOAT,"TSTOPF",&tstopf,"Fractional observation stop time",&status);
	if(status) { LOG(INFO)<<"\:";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	char *utc="UTC";
	fits_update_key(ftct,TSTRING,"TIMESYS",utc,"Time will be UTC (Universal Time)",&status);
	if(status) { LOG(INFO)<<"\:";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	long MJDREFI=55197;
	fits_update_key(ftct,TINT,"MJDREFI",&MJDREFI,"2010 expressed in UTC/Integer",&status);
	if(status) { LOG(INFO)<<"\:";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	long MJDREFF=0;
	fits_update_key(ftct,TINT,"MJDREFF",&MJDREFF,"2010 expressed in UTC/Integer",&status);
	if(status) { LOG(INFO)<<"\:";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_update_key(ftct,TFLOAT,"TIMEDEL",&timedel,"integration time",&status);
	if(status) { LOG(INFO)<<"\:";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_update_key(ftct,TFLOAT,"DELTAT",&timedel,"integration time",&status);
	if(status) { LOG(INFO)<<"\:";fits_report_error(stderr,status); return(EXIT_FAILURE);}


//modifing secondory header of output tct file
	fits_movabs_hdu(ftct,2,NULL,&status);
	if(status) { LOG(INFO)<<"\:Error in moving to Extension in tct ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_update_key(ftct,TDOUBLE,"TSTART",&tstart,"observation start time",&status);
	if(status) { LOG(INFO)<<"\:";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_update_key(ftct,TLONG,"TSTARTI",&tstarti,"integer portion of start time",&status);
	if(status) { LOG(INFO)<<"\:";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_update_key(ftct,TFLOAT,"TSTARTF",&tstartf,"fractional observation start time",&status);
	if(status) { LOG(INFO)<<"\:";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_update_key(ftct,TDOUBLE,"TSTOP",&tstop,"observation stop time",&status);
	if(status) { LOG(INFO)<<"\:";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_update_key(ftct,TLONG,"TSTOPI",&tstopi,"Integer observation stop time",&status);
	if(status) { LOG(INFO)<<"\:";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_update_key(ftct,TFLOAT,"TSTOPF",&tstopf,"Fractional observation stop time",&status);
	if(status) { LOG(INFO)<<"\:";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_update_key(ftct,TSTRING,"TIMESYS",utc,"Time will be UTC (Universal Time)",&status);
	if(status) { LOG(INFO)<<"\:";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_update_key(ftct,TINT,"MJDREFI",&MJDREFI,"2010 expressed in UTC/Integer",&status);
	if(status) { LOG(INFO)<<"\:";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_update_key(ftct,TINT,"MJDREFF",&MJDREFF,"2010 expressed in UTC/Integer",&status);
	if(status) { LOG(INFO)<<"\:";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_update_key(ftct,TFLOAT,"TIMEDEL",&timedel,"integration time",&status);
	if(status) { LOG(INFO)<<"\:";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_update_key(ftct,TFLOAT,"DELTAT",&timedel,"integration time",&status);
	if(status) { LOG(INFO)<<"\:";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	if((*History=='y') && status==0){
		//cout<<"\n####writing history";
		fits_movabs_hdu(ftct,1,NULL,&status);
		if(status) { LOG(INFO)<<"\:Error in moving to Extension in tct ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
		char history[1024]="executed czttctheal";
		fits_write_history(ftct, history,&status);
		if(status) { LOG(INFO)<<"\:Error in writting history to primary HDU ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	}
	fits_close_file(ftct,&status);
	if(status) { LOG(INFO)<<"\:Error in closing tct ";fits_report_error(stderr,status); return(EXIT_FAILURE);}

	fits_open_file(&ftct,tct,READWRITE,&status);
	if(status) { LOG(INFO)<<"\:Error in opening input tct file ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_movabs_hdu(ftct,2,NULL,&status);
	if(status) { LOG(INFO)<<"\:Error in moving to Extension in tct ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_get_num_rows(ftct,&nrows,&status);
	if(status) { LOG(INFO)<<"\:Error in reading no of rows.";fits_report_error(stderr,status); return(EXIT_FAILURE);}

	delete []INSTRUMENT_TIME_temp;
	INSTRUMENT_TIME_temp=(double*)malloc(sizeof(double)*nrows);

	fits_read_col(ftct,TDOUBLE,1,1,1,nrows,NULL,INSTRUMENT_TIME_temp,NULL,&status);
	if(status) { LOG(INFO)<<"\:";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	removerows.clear();
	for(k=1;k<nrows-1;k++){
		double diff_pre=INSTRUMENT_TIME_temp[k]-INSTRUMENT_TIME_temp[k-1];
		double diff_next=INSTRUMENT_TIME_temp[k]-INSTRUMENT_TIME_temp[k+1];
		if(diff_pre*diff_next>=0){
			removerows.push_back(k);
		}
	}
	for(k=0;k<removerows.size();k++){
		removerows_arr[k]=removerows[k];
	}
	fits_delete_rowlist(ftct, removerows_arr,removerows.size(), &status);	
	if(status) { LOG(INFO)<<"\:Error in deleting rows ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	removerows.clear();
	fits_close_file(ftct,&status);
	if(status) { LOG(INFO)<<"\:Error in closing tct file ";fits_report_error(stderr,status); return(EXIT_FAILURE);}




		

delete []SPS_TIME_temp;
//delete []SPS_OBT_temp;
delete []INSTRUMENT_TIME_temp;
delete []removerows_arr;
if(rollover_flag==1){
LOG(INFO)<<"\:Removing splited parts of tct";
remove("temp.tct0");
remove("temp.tct1");
}
LOG(INFO)<<"\: TCT HEAL COMPLETED SUCESSFULLY";

//cout<<"\nTCT HEAL COMPLETED SUCESSFULLY";
}

void toWords(unsigned char *data,long size,unsigned short *word_data,int bigendian)
{
    int i,j,msb,lsb;
    for(i=0,j=0;i<size;j++,i=i+2){
        if(bigendian){
            msb=data[i];
            lsb=data[i+1];
        }
        else{
            lsb=data[i];
            msb=data[i+1];
        }
        word_data[j]=(unsigned short)((msb & 0xff)<<8 | (lsb & 0xff));
    }
}

int getPackets(fitsfile *fptr,unsigned char *pktarray,long startRowNumber,long endRowNumber)
{
    //cout<<"\nSize of packet array:"<<m<<" x "<<n<<"\;
    int status=0;
    int colnum=4,i=0;
    int firstelem=1;
    int nelements=PACKETSIZE;//*(endRowNumber-startRowNumber+1);
    unsigned char *array=(unsigned char*) malloc(sizeof(unsigned char)*nelements);
    if(array==NULL){

        return (EXIT_FAILURE);
    }

    fits_read_col(fptr,TBYTE,colnum,startRowNumber,firstelem,nelements,NULL,array,NULL,&status);
	if(status) { printf("(%s:%d)\n:Error in reading column.",__FILE__,__LINE__);fits_report_error(stderr,status); return(EXIT_FAILURE);}
    
	for(i=0;i<PACKETSIZE;i++){
       pktarray[i]=array[i];
    }
    return (EXIT_SUCCESS);
}

void heapify(double *sps_time,double *sps_obt,double *instru_time,long n, long i)
{
    long largest = i;  // Initialize largest as root
    long l = 2*i + 1;  // left = 2*i + 1
    long r = 2*i + 2;  // right = 2*i + 2
 	double temp_instru_time=0.0;
	double temp_sps_time=0.0;
	double temp_sps_obt=0.0;

    // If left child is larger than root
    if (l < n && instru_time[l] > instru_time[largest])
        largest = l;
 
    // If right child is larger than largest so far
    if (r < n && instru_time[r] > instru_time[largest])
        largest = r;
 
    // If largest is not root
    if (largest != i)
    {
        //swap(instru_time[i], instru_time[largest]);
 		temp_instru_time=instru_time[i];
		instru_time[i]=instru_time[largest];
		instru_time[largest]=temp_instru_time;

 		temp_sps_time=sps_time[i];
		sps_time[i]=sps_time[largest];
		sps_time[largest]=temp_sps_time;
		
		temp_sps_obt=sps_obt[i];
		sps_obt[i]=sps_obt[largest];
		sps_obt[largest]=temp_sps_obt;

        // Recursively heapify the affected sub-tree
        heapify(sps_time,sps_obt,instru_time,n, largest);
    }
}
 
// main function to do heap sort
void heapSort(double *sps_time,double *sps_obt,double *instru_time,long n)
{
	//double temp1=0.0;
	//long temp1_index=0;
	double temp_instru_time1=0.0;
	double temp_sps_time1=0.0;
	double temp_sps_obt1=0.0;

    // Build heap (rearrange array)
    for (long i = n / 2 - 1; i >= 0; i--)
        heapify(sps_time,sps_obt,instru_time,n, i);
 
    // One by one extract an element from heap
    for (long i=n-1; i>=0; i--)
    {
        // Move current root to end
        //swap(instru_time[0], instru_time[i]);
 		temp_instru_time1=instru_time[0];
		instru_time[0]=instru_time[i];
		instru_time[i]=temp_instru_time1;

		temp_sps_time1=sps_time[0];
		sps_time[0]=sps_time[i];
		sps_time[i]=temp_sps_time1;
		
		temp_sps_obt1=sps_obt[0];
		sps_obt[0]=sps_obt[i];
		sps_obt[i]=temp_sps_obt1;


        // call max heapify on the reduced heap
        heapify(sps_time,sps_obt,instru_time,i, 0);
    }
}

