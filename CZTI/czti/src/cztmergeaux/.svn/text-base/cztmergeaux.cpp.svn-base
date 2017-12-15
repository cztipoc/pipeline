#include<stdio.h>
#include<fitsio.h>
#include<string.h>
#include<math.h>
#include<iostream>
#include<pil.h>
#include<cmath>
#include<vector>
#include <algorithm>
#include "glog/logging.h"
using namespace std;
long *merged_index;
double *mergeTime;

//void hpsort(long n);
void heapSort(long n);
void heapify(long n, long i);
int main()
{
   	    char const* log_file=std::getenv("logfile_env");
    if ( log_file == NULL){
		log_file="cztmergeaux.log";
    }    
    google::InitGoogleLogging(log_file);
google::SetStderrLogging(google::INFO);	

	char file1[PIL_LINESIZE];
	char outfile[PIL_LINESIZE];
	char History[1024]="y";
	fitsfile *fptr1;
	fitsfile *ftemp;
	fitsfile *fout;
	double tstop;
	long tstopi;
	float tstopf;
	double tstart;
	long tstarti;
	float tstartf;
	unsigned char merged_table1[200];
	int status=0,ifile,nfiles;
	long nrows1,mergednrowsend=0,mergednrowsstart=0,k=0,i=0,mergednrows;
	int key_value=0;
	//long *merged_index;
	char record[80];
	long *removerows_arr;
	std::vector<long> removerows;
	removerows.clear();
	
	cout<<"\nEnter the no of files: ";
	cin>>nfiles;
	if(nfiles<1){
	    LOG(ERROR)<<"Invalid number of input files. It should be >=1";
	    //cout << "\n***Invalid number of input files. It should be >=1";
	    return(EXIT_FAILURE);	
	}
	cout<<"\nEnter the output file: ";
	cin>>outfile;
	cout<<"\nHistory[y/n]: ";
	cin>>History;


for(ifile=0; ifile<nfiles; ifile++){
//	if(nfiles>=1){
		cout<<"\nEnter the input files: ";
        	cin>>file1;
		//cout<<"\n ifile: "<<ifile;
		fits_open_file(&fptr1,file1,READONLY,&status);
		if(status) {LOG(ERROR)<<"Error in Opening Science File "<<file1;fits_report_error(stderr,status); return(EXIT_FAILURE);}
		LOG(INFO)<<"Reading file: "<<file1;
		//cout<<"\nReading file: "<<file1;
		fits_movabs_hdu(fptr1,2,NULL, &status);
		if(status) {LOG(ERROR)<<"Error in Moving to 2nd extension"<<file1;fits_report_error(stderr,status); return(EXIT_FAILURE);}
		fits_get_num_rows(fptr1,&nrows1,&status);
	        if(status) {LOG(ERROR)<<"Error in reading no of rows of file"<<file1;fits_report_error(stderr,status); return(EXIT_FAILURE);}	
		//cout<<"\n numrows1: "<<nrows1;
		//fits_read_col(fptr1,TDOUBLE,1,1,1,nrows1,NULL,Time,NULL,&status);
		fits_read_key(fptr1,TINT,"NAXIS1",&key_value,NULL,&status);

		if(status) {LOG(ERROR)<<"Error in reading NAXIS key";fits_report_error(stderr,status); return(EXIT_FAILURE);}
		//creating new fits file
		if(ifile==0){
		fits_create_file(&ftemp,"!temp_combined.fits",&status);
		if(status) {LOG(ERROR)<<"Error in creating temporary fits file temp_combined.fits";fits_report_error(stderr,status); return(EXIT_FAILURE);}
		fits_movabs_hdu(fptr1,1,NULL, &status);
		if(status) {LOG(ERROR)<<"Error in moving hdu "<<file1;fits_report_error(stderr,status); return(EXIT_FAILURE);}
		fits_copy_hdu(fptr1, ftemp,NULL,&status);
		if(status) {LOG(ERROR)<<"Error in copying hdu";fits_report_error(stderr,status); return(EXIT_FAILURE);}
		fits_movabs_hdu(fptr1,2,NULL, &status);
		if(status) {LOG(ERROR)<<"Error in moving hdu "<<file1;fits_report_error(stderr,status); return(EXIT_FAILURE);}
		fits_copy_hdu(fptr1, ftemp,NULL,&status);
		if(status) {LOG(ERROR)<<"Error in copying hdu";fits_report_error(stderr,status); return(EXIT_FAILURE);}
		fits_delete_rows(ftemp,1,nrows1, &status);
		if(status) {LOG(ERROR)<<"Error in deleting all rows of newly generated temporary file temp_combined.fits";fits_report_error(stderr,status); return(EXIT_FAILURE);}
		}
		//header modification of output temp file
		if(ifile==(nfiles-1)){
			fits_movabs_hdu(fptr1,1,NULL, &status);
			if(status) {fits_report_error(stderr,status); return(EXIT_FAILURE);}
			fits_movabs_hdu(ftemp,1,NULL, &status);
			if(status) {fits_report_error(stderr,status); return(EXIT_FAILURE);}
			fits_delete_key(ftemp,"DATE-END",&status);
			status=0;
			fits_delete_key(ftemp,"TIME-END",&status);
			status=0;
			fits_read_card(fptr1,"DATE-END",record,&status);
			status=0;
			fits_write_record(ftemp,record,&status);
			if(status) { LOG(ERROR)<<"\n";fits_report_error(stderr,status); return(EXIT_FAILURE);}
			fits_read_card(fptr1,"TIME-END",record,&status);
			status=0;
			fits_write_record(ftemp,record,&status);
			if(status) { LOG(ERROR)<<"\n";fits_report_error(stderr,status); return(EXIT_FAILURE);}
		}


		fits_movabs_hdu(fptr1,2,NULL, &status);
		if(status) {LOG(ERROR)<<"Error in moving hdu ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
		
		fits_movabs_hdu(ftemp,2,NULL, &status);
		if(status) {LOG(ERROR)<<"Error in moving hdu ";fits_report_error(stderr,status); return(EXIT_FAILURE);}

		//cout<<"\n mergednrowsend: "<<mergednrowsend;
		//cout<<"\n mergednrowsstart: "<<mergednrowsstart;
		
		//merged_table1=(unsigned char*)malloc(sizeof(unsigned char)*key_value);
		//printf("\nKey Value:%d\n",key_value);
		for(k=0;k<nrows1;k++)
		{
			fits_read_tblbytes(fptr1, k+1, 1,key_value, merged_table1, &status);
			//cout<<"status: "<<status;
			if(status) 
			{
				LOG(ERROR)<<"Error in reading Tablebytes from file "<<file1;
				fits_report_error(stderr,status);
				///printf("\nError(" in reading the input file\nexiting....\n";
				exit(0);
			}
			fits_write_tblbytes(ftemp,mergednrowsstart+k+1,1,key_value,merged_table1,&status);
			if(status) 
			{
				LOG(ERROR)<<"Error in writing Tablebytes from file "<<file1;
				//LOG(ERROR)<<"in writing file \n";
				fits_report_error(stderr,status);
			   	return(EXIT_FAILURE);
			}

		}	

		fits_close_file(fptr1,&status);
		if(status) { LOG(ERROR)<<"Error in closing file"<<file1;fits_report_error(stderr,status); return(EXIT_FAILURE);}
		mergednrowsstart=mergednrowsstart+nrows1;
		//cout<<"\n mergednrowsstart: "<<mergednrowsstart;
}
fits_close_file(ftemp,&status);
if(status) {LOG(ERROR)<<"Error in closing temprary file temp_combined.fits";fits_report_error(stderr,status); return(EXIT_FAILURE);}
//reading output time column

status=0;
fits_open_file(&ftemp,"temp_combined.fits",READWRITE,&status);
if(status) { LOG(ERROR)<<"Error in opening temporary temp_combined.fits";fits_report_error(stderr,status); return(EXIT_FAILURE);}
//cout<<"\nreading output file: "<<outfile;
fits_movabs_hdu(ftemp,2,NULL, &status);
if(status) { LOG(ERROR)<<"Error in moving to 2nd hdu";fits_report_error(stderr,status); return(EXIT_FAILURE);}
fits_get_num_rows(ftemp,&mergednrows,&status);
if(status) { LOG(ERROR)<<"Error in getting merged no of rows";fits_report_error(stderr,status); return(EXIT_FAILURE);}
//printf("\nMerged rows:%d",mergednrows);
mergeTime=(double*)malloc(sizeof(double)*mergednrows);
merged_index=(long*)calloc(mergednrows,sizeof(long));
status=0;
fits_read_col(ftemp,TDOUBLE,1,1,1,mergednrows,NULL,mergeTime,NULL,&status);
if(status) { LOG(ERROR)<<"Error in reading row of merged fits file";fits_report_error(stderr,status); return(EXIT_FAILURE);}


//creating merged_index array
for(i=0;i<mergednrows;i++){
	merged_index[i]=i;
}	

//cout<<"\nindex array created.....!!";
//sorting the time 
//cout<<"\nsorting.......!!!";
//std::sort( mergeTime[0],mergeTime[mergednrows-1] );


//hpsort(mergednrows-1);
heapSort(mergednrows);
LOG(INFO)<<"SORTING DONE....";
//cout<<"\n sorting done.....";
/*for(k=0;k<mergednrows;k++){
	//printf("%f\n",mergeTime[k]);
	if(merged_index[k+1]<merged_index[k]){
	cout<<"\n sorted merged_index: "<<merged_index[k];
	}
}*/
//fits_write_col(ftemp,TDOUBLE,1,1,1,mergednrows,mergeTime,&status);


fits_create_file(&fout,outfile,&status);
if(status) { LOG(ERROR)<<"Error in creating new output file (Already exists?)"<<outfile;fits_report_error(stderr,status); return(EXIT_FAILURE);}
fits_movabs_hdu(ftemp,1,NULL, &status);
if(status) { LOG(ERROR)<<"Error in moving 1st extension";fits_report_error(stderr,status); return(EXIT_FAILURE);}
fits_copy_hdu(ftemp, fout,NULL,&status);
if(status) { LOG(ERROR)<<"Error in copying hdu";fits_report_error(stderr,status); return(EXIT_FAILURE);}
fits_movabs_hdu(ftemp,2,NULL, &status);
if(status) { LOG(ERROR)<<"error in moving 2nd extension";fits_report_error(stderr,status); return(EXIT_FAILURE);}
fits_copy_hdu(ftemp, fout,NULL,&status);
if(status) { LOG(ERROR)<<"Error in copying hdu";fits_report_error(stderr,status); return(EXIT_FAILURE);}
LOG(INFO)<<"OUTPUT FILE GENERATED...."<<outfile;
fits_delete_rows(fout,1,mergednrows, &status);

long rowcounter=0;
double lasttime=0.0;
long temp_index1;
/*(temp_index1=merged_index[mergednrows-1];
for(k=mergednrows-1;k>0;k--){
	merged_index[k]=merged_index[k-1];
}	
merged_index[0]=temp_index1;
*/	
for(k=0;k<mergednrows;k++)
{
//	fits_read_tblbytes(ftemp,merged_index[k]+1,1,key_value,merged_table1,&status);
/*if(lasttime==mergeTime[k] || (mergeTime[k]-lasttime)<=2){
	printf("\n%lf",k,mergeTime[k]);
	printf("\t%lf\n",lasttime);
}*/	
	if(lasttime!=mergeTime[k])
	{
		//cout<<"\nmerged_index[: "<<k<<"]:"<<merged_index[k];
		fits_read_tblbytes(ftemp,merged_index[k]+1,1,key_value,merged_table1,&status);		
		if(status) { LOG(ERROR)<<"Error in reading tablebytes from temporary file";fits_report_error(stderr,status); return(EXIT_FAILURE);}
		fits_write_tblbytes(fout,rowcounter+1,1,key_value,merged_table1,&status);
		if(status) { LOG(ERROR)<<"Error in writing tablebytes from temporary file to outfile";fits_report_error(stderr,status); return(EXIT_FAILURE);}
		rowcounter++;
	}
	lasttime=mergeTime[k];
}
/*fits_read_tblbytes(ftemp,merged_index[mergednrows-1]+1,1,key_value,merged_table1,&status);
if(status) { LOG(ERROR)<<"\n";fits_report_error(stderr,status); return(EXIT_FAILURE);}
fits_write_tblbytes(fout,1,1,key_value,merged_table1,&status);
if(status) { LOG(ERROR)<<"\n";fits_report_error(stderr,status); return(EXIT_FAILURE);}
*/

/*tstarti=(long)mergeTime[0];
tstopi=(long)mergeTime[mergednrows-1];
tstartf=(mergeTime[0]-((double)mergeTime[0]));
tstopf=(mergeTime[mergednrows-1]-((double)mergeTime[mergednrows-2]));
tstop=(double)tstopi+tstopf;
tstart=tstarti+tstartf;
*/

tstart=mergeTime[0];
tstarti=mergeTime[0];
tstartf=mergeTime[0]-tstarti;
tstop=mergeTime[mergednrows-1];
tstopi=mergeTime[mergednrows-1];
tstopf=mergeTime[mergednrows-1]-tstopi;

//printf("tstartf:%lf\t tstopf:%lf",tstartf,tstopf);
//modifying primary header of final output file
fits_movabs_hdu(fout,1,NULL, &status);
fits_update_key(fout,TDOUBLE,"TSTART",&tstart,"observation start time",&status);
if(status) { LOG(ERROR)<<"Error in updating key TSTART";fits_report_error(stderr,status); return(EXIT_FAILURE);}
fits_update_key(fout,TLONG,"TSTARTI",&tstarti,"integer portion of start time",&status);
if(status) { LOG(ERROR)<<"Error in updating key TSTARTI";fits_report_error(stderr,status); return(EXIT_FAILURE);}
fits_update_key(fout,TDOUBLE,"TSTARTF",&tstartf,"fractional observation start time",&status);
if(status) { LOG(ERROR)<<"Error in updating key TSTARTF";fits_report_error(stderr,status); return(EXIT_FAILURE);}
fits_update_key(fout,TDOUBLE,"TSTOP",&tstop,"observation stop time",&status);
if(status) { LOG(ERROR)<<"Error in updating key TSTOP";fits_report_error(stderr,status); return(EXIT_FAILURE);}
fits_update_key(fout,TLONG,"TSTOPI",&tstopi,"Integer observation stop time",&status);
if(status) { LOG(ERROR)<<"Error in updating key TSTOPI";fits_report_error(stderr,status); return(EXIT_FAILURE);}
fits_update_key(fout,TDOUBLE,"TSTOPF",&tstopf,"Fractional observation stop time",&status);
if(status) { LOG(ERROR)<<"Error in updating key TSTOPF";fits_report_error(stderr,status); return(EXIT_FAILURE);}
char *utc="UTC";
fits_update_key(fout,TSTRING,"TIMESYS",utc,"Time will be UTC (Universal Time)",&status);
if(status) { LOG(ERROR)<<"Error in updating key TIMESYS";fits_report_error(stderr,status); return(EXIT_FAILURE);}
long MJDREFI=55197;
fits_update_key(fout,TINT,"MJDREFI",&MJDREFI,"2010 expressed in UTC/Integer",&status);
if(status) { LOG(ERROR)<<"Error in updating key MJDREFI";fits_report_error(stderr,status); return(EXIT_FAILURE);}
long MJDREFF=0;
fits_update_key(fout,TINT,"MJDREFF",&MJDREFF,"2010 expressed in UTC/Integer",&status);
if(status) { LOG(ERROR)<<"Error in updating key MJDREFF";fits_report_error(stderr,status); return(EXIT_FAILURE);}
float timedel;
timedel=mergeTime[1]-mergeTime[0];
fits_update_key(fout,TFLOAT,"TIMEDEL",&timedel,"integration time",&status);
if(status) { LOG(ERROR)<<"Error in updating key TIMEDEL";fits_report_error(stderr,status); return(EXIT_FAILURE);}
fits_update_key(fout,TFLOAT,"DELTAT",&timedel,"integration time",&status);
if(status) { LOG(ERROR)<<"Error in updating key  DELTAT";fits_report_error(stderr,status); return(EXIT_FAILURE);}

//modifying secondory header
fits_movabs_hdu(fout,2,NULL, &status);
fits_update_key(fout,TDOUBLE,"TSTART",&tstart,"observation start time",&status);
if(status) { LOG(ERROR)<<"Error in updating key TSTART";fits_report_error(stderr,status); return(EXIT_FAILURE);}
fits_update_key(fout,TLONG,"TSTARTI",&tstarti,"integer portion of start time",&status);
if(status) { LOG(ERROR)<<"Error in updating key TSTARTI";fits_report_error(stderr,status); return(EXIT_FAILURE);}
fits_update_key(fout,TDOUBLE,"TSTARTF",&tstartf,"fractional observation start time",&status);
if(status) { LOG(ERROR)<<"Error in updating key TSTARTF";fits_report_error(stderr,status); return(EXIT_FAILURE);}
fits_update_key(fout,TDOUBLE,"TSTOP",&tstop,"observation stop time",&status);
if(status) { LOG(ERROR)<<"Error in updating key TSTOP";fits_report_error(stderr,status); return(EXIT_FAILURE);}
fits_update_key(fout,TLONG,"TSTOPI",&tstopi,"Integer observation stop time",&status);
if(status) { LOG(ERROR)<<"Error in updating key TSTOPI";fits_report_error(stderr,status); return(EXIT_FAILURE);}
fits_update_key(fout,TDOUBLE,"TSTOPF",&tstopf,"Fractional observation stop time",&status);
if(status) { LOG(ERROR)<<"Error in updating key TSTOPF";fits_report_error(stderr,status); return(EXIT_FAILURE);}
fits_update_key(fout,TSTRING,"TIMESYS",utc,"Time will be UTC (Universal Time)",&status);
if(status) { LOG(ERROR)<<"Error in updating key TIMESYS";fits_report_error(stderr,status); return(EXIT_FAILURE);}
fits_update_key(fout,TINT,"MJDREFI",&MJDREFI,"2010 expressed in UTC/Integer",&status);
if(status) { LOG(ERROR)<<"Error in updating key MJDREFI";fits_report_error(stderr,status); return(EXIT_FAILURE);}
fits_update_key(fout,TINT,"MJDREFF",&MJDREFF,"2010 expressed in UTC/Integer",&status);
if(status) { LOG(ERROR)<<"Error in updating key MJDREFF";fits_report_error(stderr,status); return(EXIT_FAILURE);}
timedel=mergeTime[1]-mergeTime[0];
fits_update_key(fout,TFLOAT,"TIMEDEL",&timedel,"integration time",&status);
if(status) { LOG(ERROR)<<"Error in updating key TIMEDEL";fits_report_error(stderr,status); return(EXIT_FAILURE);}
fits_update_key(fout,TFLOAT,"DELTAT",&timedel,"integration time",&status);
if(status) { LOG(ERROR)<<"Error in updating key DELTAT";fits_report_error(stderr,status); return(EXIT_FAILURE);}
fits_movabs_hdu(fout,1,NULL,&status);
fits_read_card(fout,"TELESCOP",record,&status);
status=0;
fits_movabs_hdu(fout,2,NULL,&status);
fits_write_record(fout,record,&status);
fits_movabs_hdu(fout,1,NULL,&status);
fits_read_card(fout,"TIME-OBS",record,&status);
status=0;
fits_movabs_hdu(fout,2,NULL,&status);
fits_write_record(fout,record,&status);
fits_movabs_hdu(fout,1,NULL,&status);
fits_read_card(fout,"DATE-OBS",record,&status);
status=0;
fits_movabs_hdu(fout,2,NULL,&status);
fits_write_record(fout,record,&status);
fits_movabs_hdu(fout,1,NULL,&status);
fits_read_card(fout,"DATE-END",record,&status);
status=0;
fits_movabs_hdu(fout,2,NULL,&status);
fits_write_record(fout,record,&status);
fits_movabs_hdu(fout,1,NULL,&status);
fits_read_card(fout,"TIME-END",record,&status);
status=0;
	if((*History=='y') && status==0){
		//cout<<"\n####writing history";
		fits_movabs_hdu(fout,1,NULL,&status);
		if(status) { LOG(ERROR)<<"Error in moving to Extension in tct";fits_report_error(stderr,status); return(EXIT_FAILURE);}
		char history[1024]="executed cztmergeaux";
		fits_write_history(fout, history,&status);
		if(status) { LOG(ERROR)<<"Error in writting history to primary HDU";fits_report_error(stderr,status); return(EXIT_FAILURE);}
	}


fits_close_file(ftemp,&status);
if(status) { LOG(ERROR)<<"Error in closing temp file";fits_report_error(stderr,status); return(EXIT_FAILURE);}
fits_close_file(fout,&status);
if(status) { LOG(ERROR)<<"Error in closing outfile";fits_report_error(stderr,status); return(EXIT_FAILURE);}

//avoiding combing by removing values which are less than 0.1sec in aux files
fits_open_file(&fout,outfile,READWRITE,&status);
if(status) { LOG(ERROR)<<"Error in opening outfile"<<outfile;fits_report_error(stderr,status); return(EXIT_FAILURE);}
fits_movabs_hdu(fout,2,NULL, &status);
if(status) { LOG(ERROR)<<"Error in moving to hdu";fits_report_error(stderr,status); return(EXIT_FAILURE);}
fits_get_num_rows(fout,&mergednrows,&status);
if(status) { LOG(ERROR)<<"Error in getting no of rows of merged file";fits_report_error(stderr,status); return(EXIT_FAILURE);}


delete []mergeTime;
//cout<<"\nmergedrows: "<<mergednrows;
mergeTime=(double*)malloc(sizeof(double)*mergednrows);
//removerows_arr=(long*)calloc(1,sizeof(long));
long rem=0;
fits_read_col(fout,TDOUBLE,1,1,1,mergednrows,NULL,mergeTime,NULL,&status);
if(status) { LOG(ERROR)<<"Error in reading time column of merged fits file "<<outfile;fits_report_error(stderr,status); return(EXIT_FAILURE);}
for(i=0;i<mergednrows-1;i++){
	if(abs(mergeTime[i]-mergeTime[i+1])<0.0001){
		//removerows_arr[rem]=i+1;							
		//removerows_arr=(long*)realloc(removerows_arr,1);
		//rem++;
		//cout<<"\nremoving row: "<<i+1;
		//cout<<"\nrem: "<<rem;
		removerows.push_back(i+1);
	}
}	
	removerows_arr=(long*)malloc(sizeof(long)*removerows.size());
	sort( removerows.begin(),removerows.end() );
	//removerows.erase( unique( removerows.begin(), removerows.end() ), removerows.end() );
	for(k=0;k<removerows.size();k++){
		removerows_arr[k]=removerows[k];
	}
	fits_delete_rowlist(fout, removerows_arr,removerows.size(), &status);	
	if(status) { LOG(ERROR)<<"Error in deleting rows from file "<<outfile;fits_report_error(stderr,status); return(EXIT_FAILURE);}

//fits_delete_rowlist(fout, removerows_arr,rem+2, &status);
//if(status) { printf("\nError("\Error in deleting rows ";fits_report_error(stderr,status); return(EXIT_FAILURE);}
fits_close_file(fout,&status);
if(status) { LOG(ERROR)<<"Error in closing file "<<outfile;fits_report_error(stderr,status); return(EXIT_FAILURE);}

	

delete []merged_index;
delete []mergeTime;
//delete []merged_table1;
LOG(INFO)<<"Removing temporary generated merged file temp_combined.fits";
remove("temp_combined.fits");
}

void heapify(long n, long i)
{
    long largest = i;  // Initialize largest as root
    long l = 2*i + 1;  // left = 2*i + 1
    long r = 2*i + 2;  // right = 2*i + 2
 	double temp=0.0;
	long temp_index=0;

    // If left child is larger than root
    if (l < n && mergeTime[l] > mergeTime[largest])
        largest = l;
 
    // If right child is larger than largest so far
    if (r < n && mergeTime[r] > mergeTime[largest])
        largest = r;
 
    // If largest is not root
    if (largest != i)
    {
        //swap(mergeTime[i], mergeTime[largest]);
 		temp=mergeTime[i];
		mergeTime[i]=mergeTime[largest];
		mergeTime[largest]=temp;

 		temp_index=merged_index[i];
		merged_index[i]=merged_index[largest];
		merged_index[largest]=temp_index;
		
        // Recursively heapify the affected sub-tree
        heapify(n, largest);
    }
}
 
// main function to do heap sort
void heapSort(long n)
{
	double temp1=0.0;
	long temp1_index=0;
    // Build heap (rearrange array)
    for (long i = n / 2 - 1; i >= 0; i--)
        heapify(n, i);
 
    // One by one extract an element from heap
    for (long i=n-1; i>=0; i--)
    {
        // Move current root to end
        //swap(mergeTime[0], mergeTime[i]);
 		temp1=mergeTime[0];
		mergeTime[0]=mergeTime[i];
		mergeTime[i]=temp1;

 		temp1_index=merged_index[0];
		merged_index[0]=merged_index[i];
		merged_index[i]=temp1_index;

        // call max heapify on the reduced heap
        heapify(i, 0);
    }
}


