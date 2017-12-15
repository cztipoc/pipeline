/*created by Shrikant on 29th march 2016
 * this code is basically computes the difference between two consecative time stamps in level2 evt and report if it is negative.
 * output is two consecative time and the negative difference
*/

#include<fitsio.h>
#include<stdio.h>
#include<iostream>
#include<string.h>
using namespace std;
int main(int argc,char*argv[]){
	if(argc<2){
		cout<<"\nPlease Enter the commandline argument(level2 evt file) and hdr file\n";
		exit(0);
	}
	char evtfile[2048];
	char hdrfile[2048];
	fitsfile *fptr;
	fitsfile *hdr;
	double *Time;
	int *CountLTULD;
	int *DataID;
	long numrows,i,numrowshdr,x,sumCountLTULDQ0=0,sumCountLTULDQ1=0,sumCountLTULDQ2=0,sumCountLTULDQ3=0;
	int status=0;
	double tdiff;
	long count=0;
	strcpy(evtfile,argv[1]);
	strcpy(hdrfile,argv[2]);
	cout<<"\n"<<evtfile;
	cout<<"\n"<<hdrfile;
	
	//opening the input evt file
	fits_open_file(&fptr,evtfile,READONLY,&status);
	if(status)
	{
		printf("Error in opening the input file\n");        
		fits_report_error(stderr,status);
	}

	fits_open_file(&hdr,hdrfile,READONLY,&status);
	if(status)
	{
		printf("Error in opening the input file\n");        
		fits_report_error(stderr,status);
	}

	//reading hdr file
	fits_movabs_hdu(hdr,2,NULL,&status);
	if(status){fits_report_error(stderr,status);}
	fits_get_num_rows(hdr,&numrowshdr,&status);
	if(status){fits_report_error(stderr,status);}
	DataID=(int*)malloc(sizeof(int)*numrowshdr);
	CountLTULD=(int*)malloc(sizeof(int)*numrowshdr);
	fits_read_col(hdr,TINT,3,1,1,numrowshdr,NULL,DataID,NULL,&status);
	fits_read_col(hdr,TINT,28,1,1,numrowshdr,NULL,CountLTULD,NULL,&status);

	//reading evt file
	fits_movabs_hdu(fptr,2,NULL,&status);
	if(status){fits_report_error(stderr,status);}
	fits_get_num_rows(fptr,&numrows,&status);
	cout<<"\nNo of rows in evt file: "<<numrows;
	if(status){fits_report_error(stderr,status);}
	Time=(double*)malloc(sizeof(double)*numrows);
	
	fits_read_col(fptr,TDOUBLE,1,1,1,numrows,NULL,Time,NULL,&status);
	if(status){fits_report_error(stderr,status);}
	/*for(i=0;i<5;i++){
		printf("\n time:%lf ",Time[i]);
	}*/
	
	//calculating summation of CountLTULD for each quadrant from hdr file
	cout<<"\nQuadrant Q0:\n";
	for(x=0;x<numrowshdr;x++){
		if(DataID[x]==0){
			if(CountLTULD[x]>3072){
				CountLTULD[x]=3072;
			}	
			sumCountLTULDQ0=sumCountLTULDQ0+CountLTULD[x];
		}	
		if(DataID[x]==1){
			if(CountLTULD[x]>3072){
				CountLTULD[x]=3072;
			}	
			sumCountLTULDQ1=sumCountLTULDQ1+CountLTULD[x];
		}	
		if(DataID[x]==2){
			if(CountLTULD[x]>3072){
				CountLTULD[x]=3072;
			}	
			sumCountLTULDQ2=sumCountLTULDQ2+CountLTULD[x];
		}	
		if(DataID[x]==3){
			if(CountLTULD[x]>3072){
				CountLTULD[x]=3072;
			}	
			sumCountLTULDQ3=sumCountLTULDQ3+CountLTULD[x];
		}	

	}
	cout<<"sumation of CountLTULD for Q0: "<<sumCountLTULDQ0<<"\n";


	//calculating tdiff for evt file
	for(i=0;i<numrows-1;i++){
		tdiff=Time[i+1]-Time[i];
		tdiff=1.e6*tdiff;
		if(tdiff<-0.05){	
			count++;
			cout<<"  Time difference is negative...for the Times";
			printf(" %lf %lf difference:%lf\n",Time[i+1],Time[i],tdiff*1.e-6);
			
		}
	}

	fits_movabs_hdu(fptr,3,NULL,&status);
	if(status){fits_report_error(stderr,status);}
	fits_get_num_rows(fptr,&numrows,&status);
	//cout<<"\n no of rows: "<<numrows;
	if(status){fits_report_error(stderr,status);}
	Time=(double*)malloc(sizeof(double)*numrows);
	
	fits_read_col(fptr,TDOUBLE,1,1,1,numrows,NULL,Time,NULL,&status);
	if(status){fits_report_error(stderr,status);}
	/*for(i=0;i<5;i++){
		printf("\n time:%lf ",Time[i]);
	}*/
	cout<<"\nQuadrant Q1:\n";

	cout<<"sumation of CountLTULD for Q1: "<<sumCountLTULDQ1<<"\n";
	for(i=0;i<numrows-1;i++){
		tdiff=Time[i+1]-Time[i];
		tdiff=1.e6*tdiff;
		if(tdiff<-0.05){	
			count++;
			cout<<"  Time difference is negative...for the Times";
			printf(" %lf %lf difference:%lf\n",Time[i+1],Time[i],tdiff*1.e-6);
			
		}
	}
	fits_movabs_hdu(fptr,4,NULL,&status);
	if(status){fits_report_error(stderr,status);}
	fits_get_num_rows(fptr,&numrows,&status);
	//cout<<"\n no of rows: "<<numrows;
	if(status){fits_report_error(stderr,status);}
	Time=(double*)malloc(sizeof(double)*numrows);
	
	fits_read_col(fptr,TDOUBLE,1,1,1,numrows,NULL,Time,NULL,&status);
	if(status){fits_report_error(stderr,status);}
	/*for(i=0;i<5;i++){
		printf("\n time:%lf ",Time[i]);
	}*/
	cout<<"\nQuadrant Q2:\n";

	cout<<"sumation of CountLTULD for Q2: "<<sumCountLTULDQ2<<"\n";
	for(i=0;i<numrows-1;i++){
		tdiff=Time[i+1]-Time[i];
		tdiff=1.e6*tdiff;
		if(tdiff<-0.05){	
			count++;
			cout<<"  Time difference is negative...for the Times";
			printf(" %lf %lf difference:%lf\n",Time[i+1],Time[i],tdiff*1.e-6);
			
		}
	}
	fits_movabs_hdu(fptr,5,NULL,&status);
	if(status){fits_report_error(stderr,status);}
	fits_get_num_rows(fptr,&numrows,&status);
	//cout<<"\n no of rows: "<<numrows;
	if(status){fits_report_error(stderr,status);}
	Time=(double*)malloc(sizeof(double)*numrows);
	
	fits_read_col(fptr,TDOUBLE,1,1,1,numrows,NULL,Time,NULL,&status);
	if(status){fits_report_error(stderr,status);}
	/*for(i=0;i<5;i++){
		printf("\n time:%lf ",Time[i]);
	}*/
	cout<<"\nQuadrant Q3:\n";
	cout<<"sumation of CountLTULD for Q3: "<<sumCountLTULDQ3<<"\n";
	for(i=0;i<numrows-1;i++){
		tdiff=Time[i+1]-Time[i];
		tdiff=1.e6*tdiff;
		if(tdiff<-0.05){	
			count++;
			cout<<"  Time difference is negative...for the Times";
			printf(" %lf %lf difference:%lf\n",Time[i+1],Time[i],tdiff*1.e-6);
			
		}
	}
	cout<<"\n";

	//cout<<"\ncounter: "<<count;
	delete []Time;
	delete []DataID;
	delete []CountLTULD;
}

