#include<stdio.h>
#include<fitsio.h>
#include<pil.h>
#include<iostream>
#include <vector>

using namespace std;
//void read_all_col(fitsfile *fptr,long nrows1,double* Time,double* CztSecCnt,int* DataID,int* ModeID,long int* TotalEvents,long int* FrameNo,long int* StatusWord,long int* WritePKTNo,long int* ReadPKTNo,long int* Command,double* LAXPCTime,long int* DCNT,int* ErrorCount,int* ErrorFlag,int* BootPageNo,int* CZTNumber,int* CZTStatus,int* CZTEventReadMode,int* CommandStatus,int* BufferNo,int* BaseAddress,int* VetoSpecRange,int* HKChannelNo,long int* ADCOutput,long int* FEBCommand,long int* AlphaCount,long int* VetoCount,long int* CountLTULD,long int* CountGTULD,long int* CZTDataRead,long int* VetoSpectrum);
void quicksort(double Time[],double CztSecCnt[],int DataID[],int ModeID[],long TotalEvents[],long FrameNo[],long StatusWord[],long WritePKTNo[],long ReadPKTNo[],long Command[],double LAXPCTime[],long DCNT[],int ErrorCount[],int ErrorFlag[],int BootPageNo[],int CZTNumber[],int CZTStatus[],int CZTEventReadMode[],int CommandStatus[],int BufferNo[],int BaseAddress[],int VetoSpecRange[],int HKChannelNo[],long ADCOutput[],long FEBCommand[],long AlphaCount[],long VetoCount[],long CountLTULD[],long CountGTULD[],long CZTDataRead[],long VetoSpectrum[],long first,long last);
int main()
{	


	fitsfile *fptr1;
	fitsfile *fptr2;
	fitsfile *fout1;
	fitsfile *fout2;
	int status=0;
	int ifile;	
	long i=0;
	long j;
	int nfiles;
	//char keyname[]="NAXIS1";
	long key_value;
	char infile[PIL_LINESIZE];
	char outfile[PIL_LINESIZE];
	unsigned char* merged_table1;
	unsigned char* merged_table2;
	double* Time;
	double* CztSecCnt;
	int* DataID;
	int* ModeID;
	long* TotalEvents;
	long* FrameNo;
	long* StatusWord;
	long* WritePKTNo;
	long* ReadPKTNo;
	long* Command;
	double* LAXPCTime;
	long* DCNT;
	int* ErrorCount;
	int* ErrorFlag;
	int* BootPageNo;
	int* CZTNumber;
	int* CZTStatus;
	int* CZTEventReadMode;
	int* CommandStatus;
	int* BufferNo;	
	int* BaseAddress;
	int* VetoSpecRange;
	int* HKChannelNo;
	long* ADCOutput;
	long* FEBCommand;
	long* AlphaCount;
	long* VetoCount;
	long* CountLTULD;
	long* CountGTULD;
	long* CZTDataRead;
	long* VetoSpectrum;


merged_table1 = new unsigned char[99999999];
merged_table2 = new unsigned char[99999999];

Time = new double[99999999];
CztSecCnt=new double[99999999];
DataID=new int[99999999];
ModeID=new int[99999999];
TotalEvents=new long[99999999];
FrameNo=new long[99999999];
StatusWord=new long[99999999];
WritePKTNo=new long[99999999];
ReadPKTNo=new long[99999999];
Command=new long[99999999];
LAXPCTime=new double[99999999];
DCNT=new long[99999999];
ErrorCount=new int[99999999];
ErrorFlag=new int[99999999];
BootPageNo=new int[99999999];
CZTNumber=new int[99999999];
CZTStatus=new int[99999999];
CZTEventReadMode=new int[99999999];
CommandStatus=new int[99999999];
BufferNo=new int[99999999];
BaseAddress=new int[99999999];
VetoSpecRange=new int[99999999];
HKChannelNo=new int[99999999];
ADCOutput=new long[99999999];
FEBCommand=new long[99999999];
AlphaCount=new long[99999999];
VetoCount=new long[99999999];
CountLTULD=new long[99999999];
CountGTULD=new long[99999999];
CZTDataRead=new long[99999999];
VetoSpectrum=new long[99999999];

	long nrows1;
	long nrows2;
	long nrows;
	long k;
	long fst=0;
	long last;
	cout<<"\nEnter the no of files: ";
	cin>>nfiles;
	if(nfiles<1){
	    cout << "\n***Invalid number of input files. It should be >=1";
	    return(EXIT_FAILURE);	
	}
	cout<<"\nEnter the output file:";
	cin>>outfile;
	if(nfiles==1){
		cout<<"\nEnter the input files: ";
       		cin>>infile;
		fits_open_file(&fptr1,infile,READONLY,&status);
		cout<<"\nreading file: "<<infile;
		fits_movabs_hdu(fptr1,2,NULL, &status);
		fits_get_num_rows(fptr1,&nrows1,&status);
		fits_read_col(fptr1,TDOUBLE,1,1,1,nrows1,NULL,Time,NULL,&status);
		fits_read_key(fptr1,TLONG,"NAXIS1",&key_value,NULL,&status);
		printf("\n NAXIS1: %ld",key_value);
		fits_read_tblbytes(fptr1,1,1,key_value,merged_table1,&status);
	
		//creating new fits file
		fits_create_file(&fout1,outfile,&status);
		if(status) {fits_report_error(stderr,status); return(EXIT_FAILURE);}
		fits_movabs_hdu(fptr1,1,NULL, &status);
		fits_copy_hdu(fptr1, fout1,NULL,&status);
		fits_movabs_hdu(fptr1,2,NULL, &status);
		fits_copy_hdu(fptr1, fout1,NULL,&status);
		cout<<"\n new output file created.... "<<outfile;
		fits_write_tblbytes(fout1,1,1,key_value,merged_table1,&status);
		if(status) {fits_report_error(stderr,status); return(EXIT_FAILURE);}
		fits_close_file(fptr1,&status);
		fits_close_file(fout1,&status);
		
	}

	if(nfiles>1){
	for(ifile=0; ifile<nfiles; ifile++) {
		cout<<"\nEnter the input files: ";
       		cin>>infile;
		if(ifile==0){
		fits_open_file(&fptr1,infile,READONLY,&status);
		cout<<"\nreading file: "<<infile;
		fits_movabs_hdu(fptr1,2,NULL, &status);
		fits_get_num_rows(fptr1,&nrows1,&status);
		//fits_read_col(fptr1,TDOUBLE,1,1,1,nrows1,NULL,Time,NULL,&status);
		fits_read_key(fptr1,TLONG,"NAXIS1",&key_value,NULL,&status);
		fits_read_tblbytes(fptr1,1,1,key_value,merged_table1,&status);

		//creating new fits file
		fits_create_file(&fout1,outfile,&status);
		if(status) {fits_report_error(stderr,status); return(EXIT_FAILURE);}
		fits_movabs_hdu(fptr1,1,NULL, &status);
		fits_copy_hdu(fptr1, fout1,NULL,&status);
		fits_movabs_hdu(fptr1,2,NULL, &status);
		fits_copy_hdu(fptr1, fout1,NULL,&status);
		cout<<"\n new output file created.... "<<outfile;
		fits_write_tblbytes(fout1,1,1,key_value,merged_table1,&status);
		if(status) {fits_report_error(stderr,status); return(EXIT_FAILURE);}
		fits_close_file(fptr1,&status);
		}

	if(ifile>0){
		fits_open_file(&fptr2,infile,READONLY,&status);
		cout<<"\nreading file: "<<infile;
		if(status) {fits_report_error(stderr,status); return(EXIT_FAILURE);}
		fits_movabs_hdu(fptr2,2,NULL, &status);
		fits_get_num_rows(fptr2,&nrows2,&status);
		//fits_read_col(fptr2,TDOUBLE,1,1,1,nrows2,NULL,Time,NULL,&status);
		if(status) {fits_report_error(stderr,status); return(EXIT_FAILURE);}
		fits_read_key(fptr2,TINT,"NAXIS1",&key_value,NULL,&status);
		fits_movabs_hdu(fout1,2,NULL, &status);

		for(k=0;k<nrows2;k++){
		fits_read_tblbytes(fptr2,k+1,1,key_value,merged_table2,&status);
		if(status) {fits_report_error(stderr,status); return(EXIT_FAILURE);}
		fits_write_tblbytes(fout1,nrows1+k+1,1,key_value,merged_table2,&status);
		}
		cout<<"\nstatus: "<<status;
		
		if(status) {fits_report_error(stderr,status); return(EXIT_FAILURE);}
		fits_close_file(fptr2,&status);
		}
	}

	fits_close_file(fout1,&status);	
	}
//reopening output file again for sorting
fits_open_file(&fout1,outfile,READWRITE,&status);
fits_movabs_hdu(fout1,2,NULL, &status);
fits_get_num_rows(fout1,&nrows2,&status);
fits_read_col(fout1,TDOUBLE,1,1,1,nrows2,NULL,Time,NULL,&status);
fits_read_col(fout1,TDOUBLE,1,1,1,nrows2,NULL,CztSecCnt,NULL,&status);
fits_read_col(fout1,TINT,1,1,1,nrows2,NULL,DataID,NULL,&status);
fits_read_col(fout1,TINT,1,1,1,nrows2,NULL,ModeID,NULL,&status);
fits_read_col(fout1,TLONG,1,1,1,nrows2,NULL,TotalEvents,NULL,&status);
fits_read_col(fout1,TLONG,1,1,1,nrows2,NULL,FrameNo,NULL,&status);
fits_read_col(fout1,TLONG,1,1,1,nrows2,NULL,StatusWord,NULL,&status);
fits_read_col(fout1,TLONG,1,1,1,nrows2,NULL,WritePKTNo,NULL,&status);
fits_read_col(fout1,TLONG,1,1,1,nrows2,NULL,ReadPKTNo,NULL,&status);
fits_read_col(fout1,TLONG,1,1,1,nrows2,NULL,Command,NULL,&status);
fits_read_col(fout1,TDOUBLE,1,1,1,nrows2,NULL,LAXPCTime,NULL,&status);
fits_read_col(fout1,TLONG,1,1,1,nrows2,NULL,DCNT,NULL,&status);
fits_read_col(fout1,TINT,1,1,1,nrows2,NULL,ErrorCount,NULL,&status);
fits_read_col(fout1,TINT,1,1,1,nrows2,NULL,ErrorFlag,NULL,&status);
fits_read_col(fout1,TINT,1,1,1,nrows2,NULL,BootPageNo,NULL,&status);
fits_read_col(fout1,TINT,1,1,1,nrows2,NULL,CZTNumber,NULL,&status);
fits_read_col(fout1,TINT,1,1,1,nrows2,NULL,CZTStatus,NULL,&status);
fits_read_col(fout1,TINT,1,1,1,nrows2,NULL,CZTEventReadMode,NULL,&status);
fits_read_col(fout1,TINT,1,1,1,nrows2,NULL,CommandStatus,NULL,&status);
fits_read_col(fout1,TINT,1,1,1,nrows2,NULL,BufferNo,NULL,&status);
fits_read_col(fout1,TINT,1,1,1,nrows2,NULL,BaseAddress,NULL,&status);
fits_read_col(fout1,TINT,1,1,1,nrows2,NULL,VetoSpecRange,NULL,&status);
fits_read_col(fout1,TINT,1,1,1,nrows2,NULL,HKChannelNo,NULL,&status);
fits_read_col(fout1,TLONG,1,1,1,nrows2,NULL,ADCOutput,NULL,&status);
fits_read_col(fout1,TLONG,1,1,1,nrows2,NULL,FEBCommand,NULL,&status);
fits_read_col(fout1,TLONG,1,1,1,nrows2,NULL,AlphaCount,NULL,&status);
fits_read_col(fout1,TLONG,1,1,1,nrows2,NULL,VetoCount,NULL,&status);
fits_read_col(fout1,TLONG,1,1,1,nrows2,NULL,CountLTULD,NULL,&status);
fits_read_col(fout1,TLONG,1,1,1,nrows2,NULL,CountGTULD,NULL,&status);
fits_read_col(fout1,TLONG,1,1,1,nrows2,NULL,CZTDataRead,NULL,&status);
fits_read_col(fout1,TLONG,1,1,1,nrows2,NULL,VetoSpectrum,NULL,&status);

		


//sorting algorithm
quicksort(Time,CztSecCnt,DataID,ModeID,TotalEvents,FrameNo,StatusWord,WritePKTNo,ReadPKTNo,Command,LAXPCTime,DCNT,ErrorCount,ErrorFlag,BootPageNo,CZTNumber,CZTStatus,CZTEventReadMode,CommandStatus,BufferNo,BaseAddress,VetoSpecRange,HKChannelNo,ADCOutput,FEBCommand,AlphaCount,VetoCount,CountLTULD,CountGTULD,CZTDataRead,VetoSpectrum,0,nrows2);

//creating new output file
fits_create_file(&fout2,outfile,&status);
if(status) {fits_report_error(stderr,status); return(EXIT_FAILURE);}
fits_movabs_hdu(fout2,1,NULL, &status);
fits_movabs_hdu(fout1,1,NULL, &status);

fits_copy_hdu(fout1,fout2,NULL,&status);
fits_movabs_hdu(fout2,2,NULL, &status);
fits_movabs_hdu(fout1,2,NULL, &status);

fits_copy_hdu(fout1, fout2,NULL,&status);

cout<<"\n new output file created.... "<<outfile;
fits_write_col(fout1,TDOUBLE,1,1,1,nrows2,Time,&status);
fits_write_col(fout1,TDOUBLE,1,1,1,nrows2,CztSecCnt,&status);
fits_write_col(fout1,TINT,1,1,1,nrows2,DataID,&status);
fits_write_col(fout1,TINT,1,1,1,nrows2,ModeID,&status);
fits_write_col(fout1,TLONG,1,1,1,nrows2,TotalEvents,&status);
fits_write_col(fout1,TLONG,1,1,1,nrows2,FrameNo,&status);
fits_write_col(fout1,TLONG,1,1,1,nrows2,StatusWord,&status);
fits_write_col(fout1,TLONG,1,1,1,nrows2,WritePKTNo,&status);
fits_write_col(fout1,TLONG,1,1,1,nrows2,ReadPKTNo,&status);
fits_write_col(fout1,TLONG,1,1,1,nrows2,Command,&status);
fits_write_col(fout1,TDOUBLE,1,1,1,nrows2,LAXPCTime,&status);
fits_write_col(fout1,TLONG,1,1,1,nrows2,DCNT,&status);
fits_write_col(fout1,TINT,1,1,1,nrows2,ErrorCount,&status);
fits_write_col(fout1,TINT,1,1,1,nrows2,ErrorFlag,&status);
fits_write_col(fout1,TINT,1,1,1,nrows2,BootPageNo,&status);
fits_write_col(fout1,TINT,1,1,1,nrows2,CZTNumber,&status);
fits_write_col(fout1,TINT,1,1,1,nrows2,CZTStatus,&status);
fits_write_col(fout1,TINT,1,1,1,nrows2,CZTEventReadMode,&status);
fits_write_col(fout1,TINT,1,1,1,nrows2,CommandStatus,&status);
fits_write_col(fout1,TINT,1,1,1,nrows2,BufferNo,&status);
fits_write_col(fout1,TINT,1,1,1,nrows2,BaseAddress,&status);
fits_write_col(fout1,TINT,1,1,1,nrows2,VetoSpecRange,&status);
fits_write_col(fout1,TINT,1,1,1,nrows2,HKChannelNo,&status);
fits_write_col(fout1,TLONG,1,1,1,nrows2,ADCOutput,&status);
fits_write_col(fout1,TLONG,1,1,1,nrows2,FEBCommand,&status);
fits_write_col(fout1,TLONG,1,1,1,nrows2,AlphaCount,&status);
fits_write_col(fout1,TLONG,1,1,1,nrows2,VetoCount,&status);
fits_write_col(fout1,TLONG,1,1,1,nrows2,CountLTULD,&status);
fits_write_col(fout1,TLONG,1,1,1,nrows2,CountGTULD,&status);
fits_write_col(fout1,TLONG,1,1,1,nrows2,CZTDataRead,&status);
fits_write_col(fout1,TLONG,1,1,1,nrows2,VetoSpectrum,&status);



//clearing all arrays
	delete [] merged_table1;
	delete [] merged_table2;	
	delete [] Time;    	
delete []CztSecCnt;	
delete []DataID	;	
delete []ModeID	;	
delete []TotalEvents;	
delete []FrameNo	;	
delete []StatusWord;	
delete []WritePKTNo;	
delete []ReadPKTNo;	
delete []Command	;	
delete []LAXPCTime;	
delete []DCNT	;	
delete []ErrorCount;	
delete []ErrorFlag;	
delete []BootPageNo;	
delete []CZTNumber;	
delete []CZTStatus;	
delete []CZTEventReadMode;
delete []CommandStatus	;
delete []BufferNo	;
delete []BaseAddress	;
delete []VetoSpecRange	;
delete []HKChannelNo	;
delete []ADCOutput	;
delete []FEBCommand	;
delete []AlphaCount	;
delete []VetoCount	;
delete []CountLTULD	;
delete []CountGTULD	;
delete []CZTDataRead	;
delete []VetoSpectrum	;




	fits_close_file(fout2,&status);
	fits_close_file(fout1,&status);
}

void quicksort(double Time[],double CztSecCnt[],int DataID[],int ModeID[],long TotalEvents[],long FrameNo[],long StatusWord[],long WritePKTNo[],long ReadPKTNo[],long Command[],double LAXPCTime[],long DCNT[],int ErrorCount[],int ErrorFlag[],int BootPageNo[],int CZTNumber[],int CZTStatus[],int CZTEventReadMode[],int CommandStatus[],int BufferNo[],int BaseAddress[],int VetoSpecRange[],int HKChannelNo[],long ADCOutput[],long FEBCommand[],long AlphaCount[],long VetoCount[],long CountLTULD[],long CountGTULD[],long CZTDataRead[],long VetoSpectrum[],long first,long last){
	last--;
	long pivot,j,i;
	double dtemp;
	int itemp;
	long ltemp;
	
     if(first<last){
         pivot=first;
         i=first;
         j=last;

         while(i<j){
             while(Time[i]<=Time[pivot]&&i<last)
                 i++;
             while(Time[j]>Time[pivot])
                 j--;
             if(i<j){
                 dtemp=Time[i];
                 Time[i]=Time[j];
                 Time[j]=dtemp;

                 dtemp=CztSecCnt[i];
                 CztSecCnt[i]=CztSecCnt[j];
                 CztSecCnt[j]=dtemp;

                 itemp=DataID[i];
                 DataID[i]=DataID[j];
                 DataID[j]=itemp;
		
                 itemp=ModeID[i];
                 ModeID[i]=ModeID[j];
                 ModeID[j]=itemp;

                 ltemp=TotalEvents[i];
                 TotalEvents[i]=TotalEvents[j];
                 TotalEvents[j]=ltemp;
		
		 ltemp=FrameNo[i];
                 FrameNo[i]=FrameNo[j];
                 FrameNo[j]=ltemp;

		 ltemp=StatusWord[i];
                 StatusWord[i]=StatusWord[j];
                 StatusWord[j]=ltemp;

		 ltemp=WritePKTNo[i];
                 WritePKTNo[i]=WritePKTNo[j];
                 WritePKTNo[j]=ltemp;

		 ltemp=ReadPKTNo[i];
                 ReadPKTNo[i]=ReadPKTNo[j];
                 ReadPKTNo[j]=ltemp;

		 ltemp=Command[i];
                 Command[i]=Command[j];
                 Command[j]=ltemp;

		 dtemp=LAXPCTime[i];
                 LAXPCTime[i]=LAXPCTime[j];
                 LAXPCTime[j]=dtemp;

		 ltemp=DCNT[i];
                 DCNT[i]=DCNT[j];
                 DCNT[j]=ltemp;

		 itemp=ErrorCount[i];
                 ErrorCount[i]=ErrorCount[j];
                 ErrorCount[j]=itemp;

		 itemp=ErrorFlag[i];
                 ErrorFlag[i]=ErrorFlag[j];
                 ErrorFlag[j]=itemp;

		 itemp=BootPageNo[i];
                 BootPageNo[i]=BootPageNo[j];
                 BootPageNo[j]=itemp;

		 itemp=CZTNumber[i];
                 CZTNumber[i]=CZTNumber[j];
                 CZTNumber[j]=itemp;

		 itemp=CZTStatus[i];
                 CZTStatus[i]=CZTStatus[j];
                 CZTStatus[j]=itemp;

		 itemp=CZTEventReadMode[i];
                 CZTEventReadMode[i]=CZTEventReadMode[j];
                 CZTEventReadMode[j]=itemp;

		 itemp=CommandStatus[i];
                 CommandStatus[i]=CommandStatus[j];
                 CommandStatus[j]=itemp;

		 itemp=BufferNo[i];
                 BufferNo[i]=BufferNo[j];
                 BufferNo[j]=itemp;

		 itemp=BaseAddress[i];
                 BaseAddress[i]=BaseAddress[j];
                 BaseAddress[j]=itemp;

		 itemp=VetoSpecRange[i];
                 VetoSpecRange[i]=VetoSpecRange[j];
                 VetoSpecRange[j]=itemp;

		 itemp=HKChannelNo[i];
                 HKChannelNo[i]=HKChannelNo[j];
                 HKChannelNo[j]=itemp;

		 ltemp=ADCOutput[i];
                 ADCOutput[i]=ADCOutput[j];
                 ADCOutput[j]=ltemp;

		 ltemp=FEBCommand[i];
                 FEBCommand[i]=FEBCommand[j];
                 FEBCommand[j]=ltemp;
		
		 ltemp=AlphaCount[i];
                 AlphaCount[i]=AlphaCount[j];
                 AlphaCount[j]=ltemp;
		
		 ltemp=VetoCount[i];
                 VetoCount[i]=VetoCount[j];
                 VetoCount[j]=ltemp;
		
		 ltemp=CountLTULD[i];
                 CountLTULD[i]=CountLTULD[j];
                 CountLTULD[j]=ltemp;
		
		 ltemp=CountGTULD[i];
                 CountGTULD[i]=CountGTULD[j];
                 CountGTULD[j]=ltemp;
		
		 ltemp=CZTDataRead[i];
                 CZTDataRead[i]=CZTDataRead[j];
                 CZTDataRead[j]=ltemp;
		
		 ltemp=VetoSpectrum[i];
                 VetoSpectrum[i]=VetoSpectrum[j];
                 VetoSpectrum[j]=ltemp;
		
	      }
         }

         dtemp=Time[pivot];
         Time[pivot]=Time[j];
         Time[j]=dtemp;
	quicksort(Time,CztSecCnt,DataID,ModeID,TotalEvents,FrameNo,StatusWord,WritePKTNo,ReadPKTNo,Command,LAXPCTime,DCNT,ErrorCount,ErrorFlag,BootPageNo,CZTNumber,CZTStatus,CZTEventReadMode,CommandStatus,BufferNo,BaseAddress,VetoSpecRange,HKChannelNo,ADCOutput,FEBCommand,AlphaCount,VetoCount,CountLTULD,CountGTULD,CZTDataRead,VetoSpectrum,first,j);
	quicksort(Time,CztSecCnt,DataID,ModeID,TotalEvents,FrameNo,StatusWord,WritePKTNo,ReadPKTNo,Command,LAXPCTime,DCNT,ErrorCount,ErrorFlag,BootPageNo,CZTNumber,CZTStatus,CZTEventReadMode,CommandStatus,BufferNo,BaseAddress,VetoSpecRange,HKChannelNo,ADCOutput,FEBCommand,AlphaCount,VetoCount,CountLTULD,CountGTULD,CZTDataRead,VetoSpectrum,j+1,last);
       //  quicksort(Time,first,j-1);
         //quicksort(Time,j+1,last);

    }
}

		




