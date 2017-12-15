#include<stdio.h>
#include<fitsio.h>
#include<string.h>
#include<pil.h>
#include<math.h>
#include<iostream>

using namespace std;
long getNumrows(char *infile, int hdunum);
long searchgethk(int DataID[],int HKChannelNo[],double time[],long nrows,int quadrant,double search_time,int hkchannelvalue[],long adc[],long ADCOutput[],float output[]);
long nearertime(int hkchannel[],double time[],long start_index,long stop_index,double search_time,int hkchannelvalue[]);
float output[7];
int error_count,boot_page_no,modeid,dataid;
long alphacounter,vetocounter;

int main()
{	
	//dynamic memory allocation
	double * Time;
	int * DataID;
	int * HKChannelNo;
	long * ADCOutput;
	int quadrant;
	int status=0;
	double search_time;
	long adc[8];
	int hkchannelvalue[1];
	long nrows,nearhkindex;
		

	char infile[PIL_LINESIZE];
	cout<<"\nEnter the input hdr File: ";
	cin>>infile;
	fitsfile *fptr;
	//cout<<"\nEnter the mode to be processed[0=modeM0,1=modeSS,2=modeM9] : ";
	//cin>>mode;
	cout<<"\nEnter the quadrant[0,1,2,3]: ";
	cin>>quadrant;
	cout<<"\nEnter the time to search hkparameters: ";
	cin>>search_time;
	nrows=getNumrows(infile,2);
	//printf("No of rows in infile: %d\n ",nrows);
	
	Time=new double[nrows];
	DataID=new int[nrows];
	HKChannelNo=new int[nrows];
	ADCOutput= new long[nrows];


	
	//opening fits file
	fits_open_file(&fptr,infile,READONLY,&status);
	fits_movabs_hdu(fptr,2,NULL, &status);
	if(status) {fits_report_error(stderr,status); return(EXIT_FAILURE);}

	//reading perticular columns
	fits_read_col(fptr,TDOUBLE,1,1,1,nrows,NULL,Time,NULL,&status);
	fits_read_col(fptr,TINT,3,1,1,nrows,NULL,DataID,NULL,&status);
	fits_read_col(fptr,TINT,23,1,1,nrows,NULL,HKChannelNo,NULL,&status);
	fits_read_col(fptr,TLONG,24,1,1,nrows,NULL,ADCOutput,NULL,&status);
	if(status) {fits_report_error(stderr,status); return(EXIT_FAILURE);}
	//for(x=0;x<nrows;x++){
	//	cout<<"\t#"<<DataID[x];
	//}
	//printf("\n time[0]: %lf",time[0]);

	nearhkindex=searchgethk(DataID,HKChannelNo,Time,nrows,quadrant,search_time,hkchannelvalue,adc,ADCOutput,output);
	//redaing other parameters like error count,modeid,dataid,alphacounter,vetocounter
	fits_read_col(fptr,TINT,4,nearhkindex,1,1,NULL,&modeid,NULL,&status);
	fits_read_col(fptr,TINT,13,nearhkindex,1,1,NULL,&error_count,NULL,&status);
	fits_read_col(fptr,TINT,15,nearhkindex,1,1,NULL,&boot_page_no,NULL,&status);
	fits_read_col(fptr,TLONG,26,nearhkindex,1,1,NULL,&alphacounter,NULL,&status);
	fits_read_col(fptr,TLONG,27,nearhkindex,1,1,NULL,&vetocounter,NULL,&status);
	dataid=DataID[nearhkindex];

	
	cout<<"\nCalculating HK Parameter.......";
	cout<<"\n##### HK parameters ######";
	cout<<"\n Supply Voltage: "<<output[0];
	cout<<"\n Temperature:    "<<output[1];
	cout<<"\n VCCA:		  "<<output[2];
	cout<<"\n CZT HV:	  "<<output[3];
	cout<<"\n VETO HV:	  "<<output[4];
	cout<<"\n DVDD:		  "<<output[5];
	cout<<"\n VETO LLD:	  "<<output[6];
	cout<<"\n modeID: 	  "<<modeid;
	cout<<"\n dataID:	  "<<dataid;
	cout<<"\n error_count:    "<<error_count;
	cout<<"\n boot_page_no:   "<<boot_page_no;
	printf("\n alphacounter:  %ld ",alphacounter);
	printf("\n vetocounter:  %ld ",vetocounter);
/*	cout<<"\n ignore	: "<<adc[0];
	cout<<"\n Supply Voltage: "<<adc[1];
	cout<<"\n Temperature:    "<<adc[2];
	cout<<"\n VCCA:		  "<<adc[3];
	cout<<"\n CZT HV:	  "<<adc[4];
	cout<<"\n VETO HV:	  "<<adc[5];
	cout<<"\n DVDD:		  "<<adc[6];
	cout<<"\n VETO LLD:	  "<<adc[7];
*/
}

long searchgethk(int DataID[],int HKChannelNo[],double time[],long nrows,int quadrant,double search_time,int hkchannelvalue[],long adc[],long ADCOutput[],float output[])
{
	int i;
	long j,k,indexQ0=0,indexQ1,indexQ2,indexQ3;
	long nearhk;


	for(j=0;j<nrows;j++){
		if(DataID[j]==0||DataID[j]==8){
			indexQ0++;
		
		}
	}
	indexQ1=indexQ0+1;
	for(j=indexQ1;j<nrows;j++){
		if(DataID[j]==1||DataID[j]==9){
			indexQ1++;
		}
		
	}indexQ2=indexQ1+1;
	for(j=indexQ2;j<nrows;j++){
		if(DataID[j]==2||DataID[j]==10){
			indexQ2++;
		}
		
	}indexQ3=indexQ2+1;
	for(j=indexQ3;j<nrows;j++){
		if(DataID[j]==3||DataID[j]==11){
			indexQ3++;
		}
		
	}

	indexQ0--;
	indexQ1--;
	indexQ2--;
	

	//cout<<"\n start  Q1 index: "<<indexQ0+1;
	//cout<<"\n stop Q1 index: "<<indexQ1;

	//dunamically allocating memory for Time array
	if (quadrant==0){
	nearhk=nearertime(HKChannelNo,time,0,indexQ0,search_time,hkchannelvalue);

	//cout<<"\n nearerhk at: "<<nearhk;
	//cout<<"\n indexQ0 at: "<<indexQ0;
	i=0;
		while(i<9){
			if(nearhk<=indexQ0){
				adc[hkchannelvalue[0]]=ADCOutput[nearhk];
				//cout<<"\n nearhk 1st if: "<<nearhk;
				nearhk++;
				hkchannelvalue[0]=HKChannelNo[nearhk];
						
			}
			if(nearhk>indexQ0){
				nearhk=nearhk-9;
				//cout<<"\n nearhk 2nd if: "<<nearhk;
				hkchannelvalue[0]=HKChannelNo[nearhk];
				adc[hkchannelvalue[0]]=ADCOutput[nearhk];
			}
		i++;	
		}
	output[0]=adc[1]/409.6;
	output[1]=100*(1.67-(adc[2]/409.6));
	output[2]=adc[3]/409.6;
	output[3]=adc[4]/409.6;
	output[4]=adc[5]/409.6;
	output[5]=adc[6]/409.6;
	output[6]=adc[7]/409.6;


		
	}
	if (quadrant==1){
	indexQ0=indexQ0+1;
	nearhk=nearertime(HKChannelNo,time,indexQ0,indexQ1,search_time,hkchannelvalue);

	//cout<<"\n nearerhk at: "<<nearhk;
	//cout<<"\n indexQ0 at: "<<indexQ0;
	i=0;
		while(i<9){
			if(nearhk<=indexQ1){
				adc[hkchannelvalue[0]]=ADCOutput[nearhk];
				//cout<<"\n nearhk 1st if: "<<nearhk;
				nearhk++;
				hkchannelvalue[0]=HKChannelNo[nearhk];
				
			
			}
			if(nearhk>indexQ1){
				nearhk=nearhk-9;
				//cout<<"\n nearhk 2nd if: "<<nearhk;
				hkchannelvalue[0]=HKChannelNo[nearhk];
				adc[hkchannelvalue[0]]=ADCOutput[nearhk];
				
			}
		i++;	
		}
	output[0]=adc[1]/409.6;
	output[1]=100*(1.67-(adc[2]/409.6));
	output[2]=adc[3]/409.6;
	output[3]=adc[4]/409.6;
	output[4]=adc[5]/409.6;
	output[5]=adc[6]/409.6;
	output[6]=adc[7]/409.6;


		
	}
	if (quadrant==2){
	indexQ1=indexQ1+1;
	nearhk=nearertime(HKChannelNo,time,indexQ1,indexQ2,search_time,hkchannelvalue);

	//cout<<"\n nearerhk at: "<<nearhk;
	//cout<<"\n indexQ0 at: "<<indexQ0;
	i=0;
		while(i<9){
			if(nearhk<=indexQ2){
				adc[hkchannelvalue[0]]=ADCOutput[nearhk];
				//cout<<"\n nearhk 1st if: "<<nearhk;
				nearhk++;
				hkchannelvalue[0]=HKChannelNo[nearhk];
				
			
			}
			if(nearhk>indexQ2){
				nearhk=nearhk-9;
				//cout<<"\n nearhk 2nd if: "<<nearhk;
				hkchannelvalue[0]=HKChannelNo[nearhk];
				adc[hkchannelvalue[0]]=ADCOutput[nearhk];
				
			}
		i++;	
		}
	output[0]=adc[1]/409.6;
	output[1]=100*(1.67-(adc[2]/409.6));
	output[2]=adc[3]/409.6;
	output[3]=adc[4]/409.6;
	output[4]=adc[5]/409.6;
	output[5]=adc[6]/409.6;
	output[6]=adc[7]/409.6;


		
	}
	if (quadrant==3){
	indexQ2=indexQ2+1;
	nearhk=nearertime(HKChannelNo,time,indexQ2,indexQ3,search_time,hkchannelvalue);

	//cout<<"\n nearerhk at: "<<nearhk;
	//cout<<"\n indexQ0 at: "<<indexQ0;
	i=0;
		while(i<9){
			if(nearhk<=indexQ3){
				adc[hkchannelvalue[0]]=ADCOutput[nearhk];
				//cout<<"\n nearhk 1st if: "<<nearhk;
				nearhk++;
				hkchannelvalue[0]=HKChannelNo[nearhk];
				
			
			}
			if(nearhk>indexQ3){
				nearhk=nearhk-9;
				//cout<<"\n nearhk 2nd if: "<<nearhk;
				hkchannelvalue[0]=HKChannelNo[nearhk];
				adc[hkchannelvalue[0]]=ADCOutput[nearhk];
				
			}
		i++;	
		}
	output[0]=adc[1]/409.6;
	output[1]=100*(1.67-(adc[2]/409.6));
	output[2]=adc[3]/409.6;
	output[3]=adc[4]/409.6;
	output[4]=adc[5]/409.6;
	output[5]=adc[6]/409.6;
	output[6]=adc[7]/409.6;


		
	}


	return nearhk;
}

long nearertime(int hkchannel[],double time[],long start_index,long stop_index,double search_time,int hkchannelvalue[])
{	
	long i,nearindex;
	double nearertime;
	double stime;
	stime=search_time;
	
	if(stime<=time[start_index]){
		stime=time[start_index];
	}
	if(stime>time[stop_index]){
		stime=time[stop_index];
	}
	for(i=start_index;i<stop_index;i++){
		if(stime>=time[i]){
			i++;
			nearindex=i;
		}
		if(stime<time[i]){
			stime=time[i];
			break;
		}	
	}
	if(stime=time[i]){
			i--;
			nearindex=i;
	}

	//printf("\nnearindex: %ld",nearindex);
	nearertime=time[nearindex];
	//printf("\n nearer time is: %lf",nearertime);
	hkchannelvalue[0]=hkchannel[nearindex];
	//printf("\n nearer hkchannel value is: %d",hkchannelvalue[0]);
	return nearindex;
		
}	
		



long getNumrows(char *infile, int hdunum)
{
	long numrows=0;
	int status=0;
	fitsfile *fptr;
	fits_open_file(&fptr,infile,READONLY, &status);
	if(status) {fits_report_error(stderr,status); return(EXIT_FAILURE);}

	//move hdu
	fits_movabs_hdu(fptr,hdunum,NULL,&status);
	fits_get_num_rows(fptr,&numrows,&status);
	fits_close_file(fptr,&status);

	return numrows;
}



