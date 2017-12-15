/*
 ============================================================================
 Name        : DataQualityCheck.c
 Author      : Ajay Vibhute
 Version     : 1.0
 Description : This code generates the data quality report from level1 data.
 Data can has quality as Good/Marginal/Bad. We decide the quality of the data
 based on fraction of correct frame headers, grade of health parameters, fraction
 of data lost by noisy pixels and quality grade conditions.
 ============================================================================
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "fitsio.h"
#include <string.h>
#include <libgen.h>
#include <time.h>
#include "DataQualityCheck.h"
#include "glog/logging.h"
using namespace std;
int main(int argc,char*argv[])
{
	string log_file;
	log_file=getenv("logfile_env");//+std::string("czttctheal");
	google::InitGoogleLogging(log_file.c_str());
	google::SetStderrLogging(google::INFO);

	if(argc<8)
	{
		printf("Error(%s:%d): Please enter all command line inputs\n",__FILE__,__LINE__);
		printf("1: Input file name\n");
		printf("2: MKF file name\n");
		printf("3: SSM file name\n");
		printf("4: HK Range file name\n");
		printf("5: OC Range file name\n");
		printf("6: Quality log file name (fraction of noisy data)\n");
		printf("7: Output file name\n");
		exit(0);
	}
	char l1filename[BUFFSIZE],init_hk_filename[BUFFSIZE],mkf_filename[BUFFSIZE],ssm_filename[BUFFSIZE],ocinitfile[BUFFSIZE],dataquality[10],outfile[BUFFSIZE],*filename;
	char qualitylogfile[BUFFSIZE];
	int curquad=0,status=0,hdutype=0,startrow=0,endrow=0,i=0;
	int total_frames=0,num_corrupt_frames=0;
	int total_hk[NUMHKPARAM],num_corrupt_hk[NUMHKPARAM];
	double xx=0,yy=0;
	double min_hk[NUMHKPARAM],max_hk[NUMHKPARAM];
	double frac_data_correct_frames=0,temp=0,HQ=0,OC=0,dbl_adc=0;
	long int numrows;
	fitsfile *fptr;
	unsigned char *event_data,hkchannelno=0,packetid=0;
	unsigned short *data,adcoutput=0;
	clock_t start_t,stop_t;
	strcpy(l1filename,argv[1]);
	strcpy(mkf_filename,argv[2]);
	strcpy(ssm_filename,argv[3]);
	strcpy(init_hk_filename,argv[4]);
	strcpy(ocinitfile,argv[5]);
	strcpy(qualitylogfile,argv[6]);
	strcpy(outfile,argv[7]);

	event_data=(unsigned char*) malloc(sizeof(unsigned char)*PACKETSIZE);
	data=(unsigned short*)malloc(sizeof(unsigned short)*BUFFSIZE);

	initHK(init_hk_filename,&min_hk[0],&max_hk[0]);
	for(i=0;i<NUMHKPARAM;i++)
	{
		num_corrupt_hk[i]=0;
		total_hk[i]=0;
	}

	for(curquad=0;curquad<4;curquad++)
	{
		fits_open_file(&fptr,l1filename,READONLY,&status);
		if(status)
		{
			//printf("Error(%s:%d): while opening output fits file %s\n",__FILE__,__LINE__,l1filename);
			LOG(ERROR)<<"Error in opening output fits file "<<l1filename;
			return (EXIT_FAILURE);
		}
		fits_movabs_hdu(fptr,curquad+2,&hdutype,&status);
		if(status)
		{
			//printf("Error(%s:%d): while moving the HDU\n",__FILE__,__LINE__);
			LOG(ERROR)<<"Error in moving the HDU";
			return (EXIT_FAILURE);
		}
		fits_get_num_rows(fptr,&numrows,&status);
		if(status)
		{
			//printf("Error(%s:%d): while getting number of rows\n",__FILE__,__LINE__);
			LOG(ERROR)<<"Error in getting number of rows";
			return (EXIT_FAILURE);
		}
		startrow=1;
		do
		{

			endrow=startrow;
			getPackets(fptr,event_data,startrow,endrow);
			toWords(event_data,PACKETSIZE,data,1);
			packetid=(unsigned char)((data[2]>>4) & 0xf);

			/*Checking fraction of corrupted data*/
			if(!(data[0]==0Xf9a4 && data[1]==0X2bb1))
				num_corrupt_frames++;
			total_frames++;
			if(packetid==0)
			{
				hkchannelno=(unsigned char)((data[17]>>12) & 0x7);
				adcoutput=(unsigned short)((data[17]) & 0xfff);
				//printf("%d\n",adcoutput);
				if(hkchannelno==2)
				{
					dbl_adc=100*(1.67-((double)adcoutput/CF));
				}
				else
				{
					dbl_adc=(double)adcoutput/CF;
					//printf("%d\t%f\t%d\n",hkchannelno,dbl_adc,adcoutput);
					if(!(min_hk[hkchannelno]<dbl_adc && dbl_adc< max_hk[hkchannelno]))
					{
						num_corrupt_hk[hkchannelno]++;
					}

					total_hk[hkchannelno]++;
				}
			}
			startrow++;
		}
		while(endrow<(numrows));
	}
	frac_data_correct_frames=100-(num_corrupt_frames/total_frames*1.0);
	for(i=1;i<NUMHKPARAM;i++)
	{
		if(i!=2)
		temp+=(((double)num_corrupt_hk[i])/total_hk[i]);
	}
	HQ=10-temp;
	temp=0;
	OC=getOC(mkf_filename,ssm_filename,ocinitfile);
	xx=frac_data_correct_frames;
	yy=0;
	if(xx>90  && yy<10 && HQ>8 && OC>8)
		strcpy(dataquality,"GOOD");

	else if(xx <80 || yy >20 || HQ<5 || OC<5)
		strcpy(dataquality,"BAD");

	else if((90 >xx && xx> 80) || (10 <yy && yy <20) || (7> HQ && HQ>5 )||( 7 > OC && OC>5))
		strcpy(dataquality,"MARGINAL");
	else
		strcpy(dataquality,"UNDEFINED");
	yy=getNoisyFraction(qualitylogfile);
	filename=basename(l1filename);
	writeOutput(outfile,filename,dataquality,xx,yy,HQ,OC);
	fits_close_file(fptr,&status);
	//printf("Data quality:%s\n",dataquality);
	LOG(INFO)<<"Data quality: "<<dataquality;
	//printf("Fraction of data with correct frames:%0.2f\%\n",frac_data_correct_frames);
	LOG(INFO)<<"Fraction of data with correct frames: "<<frac_data_correct_frames;
	//printf("Data fraction lost due to noisy pixel:%0.2f\%\n",yy);
	LOG(INFO)<<"Data fraction lost due to noisy pixel"<<yy;
	//printf("HQ:%0.2f\n",HQ);
	LOG(INFO)<<"HQ: "<<HQ;
	//printf("OC:%0.2f\n",OC);
	LOG(INFO)<<"OC: "<<OC;


	return EXIT_SUCCESS;
}
double getNoisyFraction(char*quality_logfile)
{
	FILE*fp;
	fp=fopen(quality_logfile,"r");
	int tmp=0,i=0,quad=0;
	char line[BUFFSIZE];
	double yy=0,dbltmp=0;
	if(fp==NULL)
	{
		//printf("Error(%s:%d): %s file not found\n",__FILE__,__LINE__,quality_logfile);
		LOG(ERROR)<<"Quality2.LOG File Not FOUND "<<quality_logfile;
		exit(0);
	}
	fscanf(fp,"%d",&tmp);
	for(quad=0;quad<4;quad++)
	{
		for(i=0;i<7;i++)
			fscanf(fp,"%s",line);
		yy+=atof(line);
	}
	yy/=4;
	return yy;
}
int writeOutput(char*outfile, char*l1file,char*dataquality, double xx,double yy, double HQ,double OC)
{
	FILE *fp;
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	fp=fopen(outfile,"w");
	if(fp==NULL)
	{
		//printf("Error(%s:%d): Error while creating output file %s\n",__FILE__,__LINE__,outfile);
		LOG(ERROR)<<"Error while creating output file "<<outfile;
		exit(0);
	}
	fprintf(fp,"<Quality_Flag_L1>\n");
	fprintf(fp,"<Data_file_name>%s</Data_file_name>\n",l1file);
	fprintf(fp,"<Quality_FLAG>%s</Quality_FLAG>\n",dataquality);
	fprintf(fp,"<Quality_details>\n");
	fprintf(fp,"Fraction of Data with Correct Headers: %0.2f\% Quality Grade of Health parameters (HQ):%0.2f Fraction of Data lost due to noisy pixels: %0.2f\% Quality Grade of Operating Conditions (OC): %0.2f\n",xx,HQ,yy,OC);
	fprintf(fp,"</Quality_details>\n");
	fprintf(fp,"<Creator>CZTI-POC IUCAA</Creator>");
	fprintf(fp,"<Creation_date>%d-%2.2d-%dT%d:%2.2d:%d</Creation_date>", tm.tm_year+ 1900,tm.tm_mon,tm.tm_mday,tm.tm_hour, tm.tm_min, tm.tm_sec);
	fprintf(fp,"</Quality_Flag_L1>\n");
	return 0;
}
double getModuleOC(char*ssmfilename,double mintemp,double maxtemp)
{
	fitsfile *fptr;
	unsigned char *event_data,packetid=0;
	unsigned short *data;
	int startrow=1,endrow=0,hdunum=0,status=0,hdutype=0,i=0,count=0,totcount=0;
	long int numrows=0;
	double tempoc=0;	
	event_data=(unsigned char*) malloc(sizeof(unsigned char)*PACKETSIZE);
	data=(unsigned short*)malloc(sizeof(unsigned short)*BUFFSIZE);
	fits_open_file(&fptr,ssmfilename,READONLY,&status);
	if(status){fits_report_error(stderr,status);}
	for(hdunum=0;hdunum<4;hdunum++)
	{
		//1199,1203,1198,1203
		fits_movabs_hdu(fptr,hdunum+2,&hdutype,&status);
		if(status){fits_report_error(stderr,status);}
		fits_get_num_rows(fptr,&numrows,&status);
		if(status){fits_report_error(stderr,status);}
		startrow=1;
		do
		{
		
			endrow=startrow;
			getPackets(fptr,event_data,startrow,endrow);
			toWords(event_data,PACKETSIZE,data,1);
			packetid=(unsigned char)((data[2]>>4) & 0xf);
			if(packetid==0)
			{
				for(i=24;i<40;i++)
				{
					if(!(mintemp<data[i] && data[i]<maxtemp))
						count++;
					totcount++;
				}
			}
			startrow++;
		}
		while(endrow<(numrows));
	}
	tempoc=((double)count/totcount);
	if(0<tempoc && tempoc<=0.25)
		tempoc=1;
	else if(0.25<tempoc && tempoc<=0.5)
		tempoc=2;
	else if(0.5 <tempoc && tempoc<=1)
		tempoc=4;
	else tempoc=0;	
        tempoc=4-tempoc;
// bypassed tempoc computation temporarily till the reading of
// module temperature values are verified 
// DB 18.03.2016
// tempoc=3;
	return tempoc;
}
double getOC(char*mkffile,char*ssmfile,char*ocrangefile)
{
	fitsfile *mkfptr;
	long int numrows=0;
	int startrow=1,endrow=1,nelements=1,colnum_angoff=23,status=0,colnum_cpmrate=25;
        int badrow=0;
	int colnum_q1temp=29,colnum_q2temp=40,colnum_q3temp=51,colnum_q4temp=62;
	double*angoff,*q1temp,*q2temp,*q3temp,*q4temp;
	int*cpmrate,hdutype,count=0,i=0;
	double threashold_angoff=0,threashold_cmprate=0,mintemp=0,maxtemp=0,therm_mintemp=0,therm_maxtemp=0;
	double ocval=0,tempoc=0;
	initOC(ocrangefile,&threashold_angoff,&threashold_cmprate,&mintemp,&maxtemp);

// hardcoded range to temporarily avoid problem created by wrong scaling in mkf
// DB 18.3.2016
//
	cpmrate=(int*)malloc(sizeof(int)*BUFFSIZE);	
	angoff=(double*)malloc(sizeof(double)*BUFFSIZE);
	q1temp=(double*)malloc(sizeof(double)*BUFFSIZE);
	q2temp=(double*)malloc(sizeof(double)*BUFFSIZE);
	q3temp=(double*)malloc(sizeof(double)*BUFFSIZE);
	q4temp=(double*)malloc(sizeof(double)*BUFFSIZE);
	fits_open_file(&mkfptr,mkffile,READONLY,&status);
	if(status)
	{	//printf("Error(%s:%d): while opening output fits file %s\n",__FILE__,__LINE__,mkffile);
		LOG(ERROR)<<"Error while opening output fits file "<<mkffile;
		return (EXIT_FAILURE);
	}
	fits_movabs_hdu(mkfptr,2,&hdutype,&status);
	if(status){fits_report_error(stderr,status);return(EXIT_FAILURE);}
	fits_get_num_rows(mkfptr,&numrows,&status);
	if(status)
	{
		//printf("Error(%s:%d): while getting number of rows\n",__FILE__,__LINE__);	
		LOG(ERROR)<<"Error while getting number of rows";	
		return (EXIT_FAILURE);
	}
	do
	{
		endrow=startrow;
		fits_read_col(mkfptr,TDOUBLE,colnum_angoff,startrow,1,nelements,NULL,angoff,NULL,&status);
		if(status)  {fits_report_error(stderr,status);  return (EXIT_FAILURE); }
		fits_read_col(mkfptr,TINT,colnum_cpmrate,startrow,1,nelements,NULL,cpmrate,NULL,&status);
		if(status)  {fits_report_error(stderr,status);  return (EXIT_FAILURE); }
		fits_read_col(mkfptr,TDOUBLE,colnum_q1temp,startrow,1,nelements,NULL,q1temp,NULL,&status);
		if(status)  {fits_report_error(stderr,status);  return (EXIT_FAILURE); }
		fits_read_col(mkfptr,TDOUBLE,colnum_q2temp,startrow,1,nelements,NULL,q2temp,NULL,&status);
		if(status)  {fits_report_error(stderr,status);  return (EXIT_FAILURE); }
		fits_read_col(mkfptr,TDOUBLE,colnum_q3temp,startrow,1,nelements,NULL,q3temp,NULL,&status);
		if(status)  {fits_report_error(stderr,status);  return (EXIT_FAILURE); }
		fits_read_col(mkfptr,TDOUBLE,colnum_q4temp,startrow,1,nelements,NULL,q4temp,NULL,&status);
		if(status)  {fits_report_error(stderr,status);  return (EXIT_FAILURE); }
	//	printf("%f\t%d\t%f\t%f\t%f\t%f\n",angoff[0],cpmrate[0],q1temp[0],q2temp[0],q3temp[0],q4temp[0]);
		therm_mintemp=mintemp-10;
		therm_maxtemp=maxtemp-10;
		for(i=0;i<nelements;i++)
		{
			if(angoff[i]>threashold_angoff)
				count++;
			if(cpmrate[i]>threashold_cmprate)
				count++;
			if(!(therm_mintemp<q1temp[i] && q1temp[i]< therm_maxtemp))
				count++;
			if(!(therm_mintemp<q2temp[i] && q2temp[i] < therm_maxtemp))
				count++;
			if(!(therm_mintemp<q3temp[i] && q3temp[i] < therm_maxtemp))
				count++;
			if(!(therm_mintemp<q4temp[i] && q4temp[i] < therm_maxtemp))
				count++;
		}	
		startrow++;
	}
	while(endrow<numrows);
	fits_close_file(mkfptr,&status);
	ocval=6-((double)count/numrows);
	tempoc=getModuleOC(ssmfile,mintemp,maxtemp);
	ocval+=tempoc;	
	return ocval;
}
int initOC(char*ocrangefile,double *threashold_angoff,double*threashold_cpm,double*mintemp,double*maxtemp)
{
	*threashold_angoff=5;
	*threashold_cpm=10;
	*mintemp=5;
	*maxtemp=25;
	FILE *fp;
	fp=fopen(ocrangefile,"r");
	if(fp==NULL)
	{
		//printf("Error(%s:%d):Error while reading %s file\n",__FILE__,__LINE__,ocrangefile);
		LOG(ERROR)<<"Error while reading file: "<<ocrangefile;
		exit(0);
	}
	fscanf(fp,"%lf",threashold_angoff);
	fscanf(fp,"%lf",threashold_cpm);
	fscanf(fp,"%lf",mintemp);
	fscanf(fp,"%lf",maxtemp);
	fclose(fp);

	return 0 ;
}
int initHK(char*hkrangefile,double *hkmin,double*hkmax)
{
	int i=0;
	FILE *fp;
	fp=fopen(hkrangefile,"r");
	if(fp==NULL)
	{
		//printf("Error(%s:%d):Error while reading %s file\n",__FILE__,__LINE__,hkrangefile);
		LOG(ERROR)<<"Error while reading file: "<<hkrangefile;
		exit(0);
	}
	for(i=1;i<NUMHKPARAM;i++)
	{
		fscanf(fp,"%lf",&hkmin[i]);
		fscanf(fp,"%lf",&hkmax[i]);
	//	printf("%d\t%f\t%f\n",i,hkmin[i],hkmax[i]);
	}
	fclose(fp);
	return 0 ;
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
    int nelements=PACKETSIZE;//*(endRowNumber-startRowNumber+1);
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
int getTime(fitsfile *fptr,double *time,long startRowNumber,long endRowNumber)
{
	int status=0;
	int colnum=1,i=0;
	int firstelem=1;
	int nelements=1;//*(endRowNumber-startRowNumber+1);
	fits_read_col(fptr,TDOUBLE,colnum,startRowNumber,firstelem,nelements,NULL,time,NULL,&status);
	if(status)  {fits_report_error(stderr,status);  return (EXIT_FAILURE); }
	return 0;
}
/*
char *basename(char const *path)
{
    char *s = strrchr(path, '/');
    if (s!=NULL)
        return strdup(path);
    else
        return strdup(s + 1);
}
*/
