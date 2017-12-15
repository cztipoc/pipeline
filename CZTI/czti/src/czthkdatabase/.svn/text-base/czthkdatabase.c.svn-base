/*Created and Modified by Shrikant ,Vedant on 18th feb 2016
 * This code decodes the hk parameters with help of science file ,tct file and orb file(latitude,longitude,altitude) and insert it into the mysql database table hk_para
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fitsio.h>
#include<time.h>
#include <mysql.h>
#include<pil.h>
#include "level1handler.h"
#define BUFSIZE 1024
#define PACKETSIZE 2048
#define tct_PACKET 1

void finish_with_error(MYSQL *con)
{
  fprintf(stderr, "%s\n", mysql_error(con));
  mysql_close(con);
  exit(1);        
}
void toWords(unsigned char *data,long size,unsigned short *word_data,int bigendian);
int getPackets(fitsfile *fptr,unsigned char *pktarray,long startRowNumber,long endRowNumber);
int tct_getPackets(fitsfile *fptr,unsigned char *pktarray,long startRowNumber,long endRowNumber);
long getNumrows(char *infile, int hdunum);
double getlagvalue(long n, double time_arr[],float value_arr[],double sectime);
int getlagvalue_int(long n, double time_arr[],int value_arr[],double sectime);
double searchgetlag(double outtime,double orbTime[],float *Latitude,float *Longitude,float *Altitude,long orbrows,float *latitude,float *longitude,float *altitude);
double searchgetlag_lbt(double outtime,double lbtTime[],long lbtrows,int *CPM_Rate,int *cpm_count);


int main(int argc,char*argv[])
{
	clock_t start_time, end_time;
	double cpu_time_used;
	char query[1000];
	char qua[10];
	start_time=clock();
	fitsfile *infptr, *tctin, *orbin, *lbtin;
	int status=0, morekeys=0,hdutype, tct_status=0;
	int quad=0;
	char infile[1024];//="/home/guest/test_area/01961/modeM0/AS1T01_052T01_9000000316_01961cztM0_level2.fits";
	char tct_infile[1024];//= "/home/guest/murali/AS1T01_052T01_9000000316czt_level1.tct";
	char orb_infile[1024];
	char lbt_infile[1024];
	double outtime;
	long nrows=0,tct_nrows=0,i,lbtrows,z;
	double laxpctime;
	float latitude,longitude,altitude;
	int endrow=0,startrow=1, vold=0;
	unsigned char *event_data, *tct_event_data;
	unsigned short *data, *tct_data;


	
    unsigned short febcommand=0,alphacount=0,vetocount=0,countlud=0,countgud=0, Phoenix, zang;
	unsigned short adcoutput=0;
	unsigned char dataid=0,pktid=0,modeid=0, cztnum, hkchannel_no, Dumbledore=200;
	int second_count,command,cpm_count,pre_command;
	//long startindex=0;
	long orbrows;
	float *Latitude,*Longitude,*Altitude,**TEMP,temperature;
	double *orbTime, *lbtTime;
	int *CPM_Rate,*QUAD_FLAG;

	//FILE *fs;
	Tct TCT;
	
	MYSQL *con = mysql_init(NULL);
 	 if (con == NULL) 
  	 {
      		fprintf(stderr, "%s\n", mysql_error(con));
      		exit(1);
  	 }     
	if (mysql_real_connect(con, "localhost", "root", "C@dm1um", 
				"czti_hk", 0, NULL, 0) == NULL) 
  	{
      		finish_with_error(con);
  	} 
	if(argc<4)
	{
		printf("\nEnter input fits file name ,tct file name , orb file name and lbt file \n");
		exit(0);
	}

	strcpy(infile,argv[1]);
	strcpy(tct_infile,argv[2]);
	strcpy(orb_infile,argv[3]);
	strcpy(lbt_infile,argv[4]);

	event_data=(unsigned char*) malloc(sizeof(unsigned char)*PACKETSIZE);
	tct_event_data=(unsigned char*) malloc(sizeof(unsigned char)*tct_PACKET);
	data=(unsigned short*)malloc(sizeof(unsigned short)*BUFSIZE);
	tct_data=(unsigned short*)malloc(sizeof(unsigned short)*BUFSIZE);
	
	//reading rows from files
	orbrows=getNumrows(orb_infile,2);
	lbtrows=getNumrows(lbt_infile,2);


	
	//dynamic allocating orb data
	Latitude=(float*)malloc(sizeof(float*)*orbrows);
	Longitude=(float*)malloc(sizeof(float*)*orbrows);
	Altitude=(float*)malloc(sizeof(float*)*orbrows);
	orbTime=(double*)malloc(sizeof(double*)*orbrows);




	//dynamic allocating lbt data
	lbtTime=(double*)malloc(sizeof(double*)*lbtrows);
	CPM_Rate=(int*)malloc(sizeof(int*)*lbtrows);


	
	//opening fits file
	printf("Input fits file name:%s\n",infile);
	fits_open_file(&infptr, infile, READONLY, &status) ;
	if(status)
	{
		printf("Error in opening the input fits file\n");        
		fits_report_error(stderr,status);
	}
	printf("Tct file name:%s\n",tct_infile);

	
	//opening orb file
	printf("orb file name:%s\n",orb_infile);
    fits_open_file(&orbin,orb_infile,READONLY,&status);
	fits_movabs_hdu(orbin,2,NULL,&status);
	
	if(status)
	{
		printf("Error in moving HDU\n");        
		fits_report_error(stderr,status);
	}



    printf("\n#####TEST HERE");	

	
	//reading Time,Latitude,Longitude,Altitude columns
	fits_read_col(orbin,TDOUBLE,1,1,1,orbrows,NULL,orbTime,NULL,&status);
	fits_read_col(orbin,TFLOAT,8,1,1,orbrows,NULL,Latitude,NULL,&status);
	fits_read_col(orbin,TFLOAT,9,1,1,orbrows,NULL,Longitude,NULL,&status);
	fits_read_col(orbin,TFLOAT,10,1,1,orbrows,NULL,Altitude,NULL,&status);
	
	//reading lbt file
	
	fits_open_file(&lbtin, lbt_infile, READONLY, &status);
	if(status)
	{
		printf("Error in opening the input lbt file\n");        
		fits_report_error(stderr,status);
	}
	fits_movabs_hdu(lbtin,2,NULL,&status);
	fits_read_col(lbtin,TDOUBLE,1,1,1,lbtrows,NULL,lbtTime,NULL,&status);
	fits_read_col(lbtin,TINT,50,1,1,lbtrows,NULL,CPM_Rate,NULL,&status);
	if(status)
	{
		printf("Error in reading the input file\n");        
		fits_report_error(stderr,status);
	}

	//reading tct file

	if (TCT.read_tct_file(tct_infile)){
	LOG(ERROR) << "*** Error reading TCT file***";
	return (EXIT_FAILURE);
	}

	//printf("till here");


	
	for(quad=0;quad<4;quad++)
	{

		//sprintf(qua,"Quadrant%d",quad);
		//fs=fopen(qua, "w");
		//
		
		fits_movabs_hdu(infptr,quad+2,NULL,&status);
		if(status)
		{
			printf("Error in moving hdu\n");
			fits_report_error(stderr,status);
			return (EXIT_FAILURE);
		}

		fits_get_num_rows(infptr,&nrows,&status);
		if(status)
		{
			printf("Error in getting number of rows\n");
			fits_report_error(stderr,status);
			return (EXIT_FAILURE);
		}
		
		startrow=1;
		endrow=1;
		//cout<<"\n nrows : "<<nrows;
		
		do
		{
			
			endrow=startrow;//(nrows<=(startrow)) ? nrows : startrow-1;
			getPackets(infptr,event_data,startrow,endrow);
			toWords(event_data,PACKETSIZE,data,1);
			dataid=(unsigned char)((data[2]>>8) & 0x1f);
			pktid=(unsigned char)((data[2]>>4) & 0xf);
			modeid=(unsigned char)(data[2] & 0xf);
			
			if(pktid==0)
			//printf("\n paketid:%d\n",pktid);

			if(pktid==0)
			{
				second_count=(unsigned int)(((data[10]<<16) & 0xffff0000) | (data[11] & 0xffff));
        		//laxpctime=(long)(((((long)data[14] & 0xff)<<32) & 0xff) | ((data[13]<<16) & 0xffff0000) | (data[12] & 0xffff));		
        		laxpctime=((double)((data[14]<<12)&0xff000 |(data[13]>>4)) + ((double)((data[12]>>10)|((data[13]&8)<<6)))/1000.0 + ((double)((data[12] & 0x03ff))/1000000.0));
				hkchannel_no=(unsigned char)((data[17]>>12) & 0x7);
				adcoutput=(unsigned short)(data[17] & (0xfff));
				febcommand=(unsigned short)data[18];
				alphacount=(unsigned short)data[19];
				vetocount=(unsigned short)data[20];
				countlud=(unsigned short)data[21];
				countgud=(unsigned short)data[22];
                command=(unsigned int)((data[9] * 65536) + (data[8] & 0xffff));
				if(command!=0 && pre_command!=command){
					//printf("%X\n",command);
				}
               //#pre_command = command;
				
				//command=(int)(((data[9]<<16) & 0xffff0000) | (data[8] & 0xffff));
		               	if(TCT.interpolate_time(second_count,outtime)){
               				 LOG(ERROR)<<"Error in interpolating INSTRUMENT TIME: ";
                			return (EXIT_FAILURE);
          			}
				//printf("\n adc is: %ld \n",adcoutput);
				//printf("\n laxpctime is: %lf \n",laxpctime);
				//printf("\nouttime:%lf",outtime);
				//for(i=0;i<5;i++){
				//	printf("\nlatitude:%lf\tlongitude:%lf\taltitude:%lf",Latitude[i],Longitude[i],Altitude[i]);
				//}	
			
				if(quad=0){
                searchgetlag(outtime,orbTime,Latitude,Longitude,Altitude,orbrows,&latitude,&longitude,&altitude);
				searchgetlag_lbt(outtime,lbtTime,lbtrows,CPM_Rate,&cpm_count);
				//searchgetlag_evt(outtime,evtTime,evtrows,TEMP,&temperature);
				//printf("\nlatitude=%lf\tlongitude=%lf\taltitude=%lf");
				//printf("\n interpolated latitude:%f\tinterpolated longitude:%f\tinterpolated altitude:%f",latitude,longitude,altitude);
				//sprintf(query,"REPLACE INTO hk_para_v1(dataid, pktid , modeid , second_count , laxpctime , hkchannel_no , adcoutput , febcommand , alphacount , vetocount , countlud , countgud , universal_time , command ,latitude ,longitude ,altitude,cpm_count) VALUES(%d,%d,%d,%ld,%lf,%d,%d,%d,%d,%d,%d,%d,%lf,%d,%f,%f,%f,%d,%f)",dataid, pktid , modeid , second_count , laxpctime , hkchannel_no , adcoutput , febcommand , alphacount , vetocount , countlud , countgud , outtime, command ,latitude ,longitude ,altitude,cpm_count,temperature);

				if (mysql_query(con, query)) {
      					finish_with_error(con);
  				}
				//printf("%s\n",query);
				//printf("%d\t%d\t%d\t%d\t%ld\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n",dataid,pktid,modeid, second_count, laxpctime, hkchannel_no, adcoutput, febcommand, alphacount, vetocount, countlud, countgud,command);
			}
			
			if((modeid>=8) && (modeid<=11))
			{
				//printf("The data run starts\n");
				for(vold=99;vold>=0;vold--)
				{
					//printf("The current time----> %d\n", laxpctime-vold);
					Phoenix=(unsigned short)data[Dumbledore];
					//printf("\n%d\t", Phoenix);
					Dumbledore++;
					Phoenix=(unsigned short)data[Dumbledore];
					//printf("%d\t", Phoenix);
					Dumbledore++;
					Phoenix=(unsigned short)data[Dumbledore];
					//printf("%d\t", Phoenix);
					Dumbledore++;
					Phoenix=(unsigned short)data[Dumbledore];
					//printf("%d\t", Phoenix);
					Dumbledore++;
					Phoenix=(unsigned short)data[Dumbledore];
					//printf("%d\t", Phoenix);
					Dumbledore++;
					Phoenix=(unsigned short)data[Dumbledore];
				//	printf("%d\t", Phoenix);
					Dumbledore++;
					Phoenix=(unsigned short)data[Dumbledore];
				//	printf("%d\t", Phoenix);
					Dumbledore++;
					Phoenix=(unsigned short)data[Dumbledore];
				//	printf("%d\n", Phoenix);
					Dumbledore++;
				}
			}
			//if my data id is between 8 to 11 decode next 100 frame header
			
		    // start from 200 to 999
			
			
		}
			startrow++;
			//printf("%d", startrow);
        	//printf("and here......................\n");
		}
		while(endrow<(nrows));
		//printf("Number of rows Q%d\t%d\n",quad,nrows);
		//fclose(fs);
		
		//for(vold=1;vold<tct_nrows;vold++)
		//{
		//	tct_getPackets(tctin,tct_event_data,vold,vold);
		//	toWords(tct_event_data,tct_PACKET,tct_data,1);
		//	zang=(unsigned short)tct_data[3];
		//	printf("%d---%d---%d\n", zang, vold, tct_nrows);
		//}
		
	}
	//readtable(tct_nrows, tct_infile, strlen(tct_infile));
	if (fits_close_file(infptr, &status))
	{
		printf("Error in closing the file\n");		
		fits_report_error(stderr,status);
	}
	//if (fits_close_file(tctin, &status))
	//{
	//	printf("Error in closing the file\n");		
	//	fits_report_error(stderr,tct_status);
	//}
	//end_time=clock();
	//cpu_time_used=end_time-start_time;
	//printf("Total time taken--%lf seconds\n", cpu_time_used);
	//printf("till here done");
	mysql_close(con);
	free(event_data);
	free(tct_event_data);
	free(data);
	free(tct_data);
	free(Latitude);
	free(Longitude);
	free(Altitude);
	free(orbTime);
	free(lbtTime);
	free(CPM_Rate);
  	exit(0);	
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
    //cout<<"\nSize of packet array:"<<m<<" x "<<n<<"\n";
    int status=0;
    int colnum=4,i=0;
    int firstelem=1;
    int nelements=PACKETSIZE;//(endRowNumber-startRowNumber+1);
    unsigned char *array=(unsigned char*) malloc(sizeof(unsigned char)*nelements);
    if(array==NULL){

        return (EXIT_FAILURE);
    }

    fits_read_col(fptr,TBYTE,colnum,startRowNumber,firstelem,nelements,NULL,array,NULL,&status);
    if(status)  {fits_report_error(stderr,status);  return (EXIT_FAILURE); }
    for(i=0;i<PACKETSIZE;i++){
       pktarray[i]=array[i];
    }
    return (EXIT_SUCCESS);
}



int tct_getPackets(fitsfile *fptr,unsigned char *pktarray,long startRowNumber,long endRowNumber)
{
    //cout<<"\nSize of packet array:"<<m<<" x "<<n<<"\n";
    int status=0;
    int colnum=1,i=0;
    int firstelem=1;
    int nelements=tct_PACKET;//(endRowNumber-startRowNumber+1);
    unsigned char *array=(unsigned char*) malloc(sizeof(unsigned char)*nelements);
    if(array==NULL){

        return (EXIT_FAILURE);
    }

    fits_read_col(fptr,TDOUBLE,colnum,startRowNumber,firstelem,nelements,NULL,array,NULL,&status);
    if(status)  {fits_report_error(stderr,status);  return (EXIT_FAILURE); }
    for(i=0;i<tct_PACKET;i++){
       pktarray[i]=array[i];
    }
    return (EXIT_SUCCESS);
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

double searchgetlag(double outtime,double orbTime[],float *Latitude,float *Longitude,float *Altitude,long orbrows,float *latitude,float *longitude,float *altitude)
{

	long index,i;
	//double latitude,longitude,altitude;
	//index=*startindex;
	index=0;
	double nearer[4];
	float nearer_latvalue[4],nearer_longvalue[4],nearer_altvalue[4];
	while(orbTime[index]<=outtime){
		index++;
		if(index>=orbrows){
			index--;
			break;
		}	
	}
	//printf("\nfound@: %ld",index);
	//startindex=index-2;
	
	
	if(index<=0){
	//printf("\nThis is 1st if");
	nearer[0]=orbTime[index];
	nearer[1]=orbTime[index+1];
	nearer[2]=orbTime[index+2];
	nearer[3]=orbTime[index+3];
	
	
	nearer_latvalue[0]=Latitude[index];
	nearer_latvalue[1]=Latitude[index+1];
	nearer_latvalue[2]=Latitude[index+2];
	nearer_latvalue[3]=Latitude[index+3];

	nearer_longvalue[0]=Longitude[index];
	nearer_longvalue[1]=Longitude[index+1];
	nearer_longvalue[2]=Longitude[index+2];
	nearer_longvalue[3]=Longitude[index+3];

	nearer_altvalue[0]=Altitude[index];
	nearer_altvalue[1]=Altitude[index+1];
	nearer_altvalue[2]=Altitude[index+2];
	nearer_altvalue[3]=Altitude[index+3];

	}
	if(index>=orbrows-3){
	
	//printf("\nThis is 2nd if");	
	nearer[0]=orbTime[index];
	nearer[1]=orbTime[index-1];
	nearer[2]=orbTime[index-2];
	nearer[3]=orbTime[index-3];
	//printf("\nsearch time:%lf",outtime);
	nearer_latvalue[0]=Latitude[index];
	nearer_latvalue[1]=Latitude[index-1];
	nearer_latvalue[2]=Latitude[index-2];
	nearer_latvalue[3]=Latitude[index-3];

	nearer_longvalue[0]=Longitude[index];
	nearer_longvalue[1]=Longitude[index-1];
	nearer_longvalue[2]=Longitude[index-2];
	nearer_longvalue[3]=Longitude[index-3];

	nearer_altvalue[0]=Altitude[index];
	nearer_altvalue[1]=Altitude[index-1];
	nearer_altvalue[2]=Altitude[index-2];
	nearer_altvalue[3]=Altitude[index-3];

	}
	if(index!=0 && !(index>=orbrows-3)){
	//printf("\nThis is else");
	nearer[0]=orbTime[index-2];
	nearer[1]=orbTime[index-1];
	nearer[2]=orbTime[index];
	nearer[3]=orbTime[index+1];
	
	nearer_latvalue[0]=Latitude[index-2];
	nearer_latvalue[1]=Latitude[index-1];
	nearer_latvalue[2]=Latitude[index];
	nearer_latvalue[3]=Latitude[index+1];

	nearer_longvalue[0]=Longitude[index-2];
	nearer_longvalue[1]=Longitude[index-1];
	nearer_longvalue[2]=Longitude[index];
	nearer_longvalue[3]=Longitude[index+1];

	nearer_altvalue[0]=Altitude[index-2];
	nearer_altvalue[1]=Altitude[index-1];
	nearer_altvalue[2]=Altitude[index];
	nearer_altvalue[3]=Altitude[index+1];

		
	}
	/*
	for(i=0;i<4;i++){
		printf("\nnearer values:\t%lf",nearer_latvalue[i]);
	}
    */
	*latitude=getlagvalue(4,nearer,nearer_latvalue,outtime);
	*longitude=getlagvalue(4,nearer,nearer_longvalue,outtime);
	*altitude=getlagvalue(4,nearer,nearer_altvalue,outtime);
	
}	



double searchgetlag_lbt(double outtime,double lbtTime[],long lbtrows,int *CPM_Rate,int *cpm_count)
{

	long indexlbt,i;
	//double latitude,longitude,altitude;
	//index=*startindex;
	indexlbt=0;
	double nearer[4];
	int nearer_cpm[4];
	while(lbtTime[indexlbt]<=outtime){
		indexlbt++;
		if(indexlbt>=lbtrows){
			indexlbt--;
			break;
		}	
	}
	//printf("\nfound@: %ld",index);
	//startindex=index-2;
	
	
	if(indexlbt<=0){
	//printf("\nThis is 1st if");
	nearer[0]=lbtTime[indexlbt];
	nearer[1]=lbtTime[indexlbt+1];
	nearer[2]=lbtTime[indexlbt+2];
	nearer[3]=lbtTime[indexlbt+3];
	
	
	nearer_cpm[0]=CPM_Rate[indexlbt];
	nearer_cpm[1]=CPM_Rate[indexlbt+1];
	nearer_cpm[2]=CPM_Rate[indexlbt+2];
	nearer_cpm[3]=CPM_Rate[indexlbt+3];


	}
	if(indexlbt>=lbtrows-3){
	
	
	nearer[0]=lbtTime[indexlbt];
	nearer[1]=lbtTime[indexlbt-1];
	nearer[2]=lbtTime[indexlbt-2];
	nearer[3]=lbtTime[indexlbt-3];
	
	nearer_cpm[0]=CPM_Rate[indexlbt];
	nearer_cpm[1]=CPM_Rate[indexlbt-1];
	nearer_cpm[2]=CPM_Rate[indexlbt-2];
	nearer_cpm[3]=CPM_Rate[indexlbt-3];


	
	}
	if(indexlbt!=0 && !(indexlbt>=lbtrows-3)){
	//printf("\nThis is else");
	nearer[0]=lbtTime[indexlbt-2];
	nearer[1]=lbtTime[indexlbt-1];
	nearer[2]=lbtTime[indexlbt];
	nearer[3]=lbtTime[indexlbt+1];
	
	
	nearer_cpm[0]=CPM_Rate[indexlbt-2];
	nearer_cpm[1]=CPM_Rate[indexlbt-1];
	nearer_cpm[2]=CPM_Rate[indexlbt];
	nearer_cpm[3]=CPM_Rate[indexlbt+1];




		
	}

	*cpm_count=getlagvalue_int(4,nearer,nearer_cpm,outtime);

}	

int getlagvalue_int(long n, double time_arr[],int value_arr[],double sectime)
{	
	long b,length;
	
	//length=(sizeof(sectime)/sizeof(double));
	//length=2;
	//for(b=0;b<length;b++){
	int i,j;
	float s,t,k;
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
        //printf("\n\n The respective value of the variable value_arr is: %lf",k);
	return k;
}



double getlagvalue(long n, double time_arr[],float value_arr[],double sectime)
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
        //printf("\n\n The respective value of the variable value_arr is: %lf",k);
	return k;
}


