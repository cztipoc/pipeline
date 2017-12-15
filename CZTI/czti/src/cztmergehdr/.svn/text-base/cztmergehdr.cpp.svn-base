#include<stdlib.h>
#include<fitsio.h>
#include<string.h>
#include<iostream>
using namespace std;
long getnumrows(char *filename);
void readBintable(char *filename,double *Time,double *CztSecCnt,double *LAXPCTime,long *TotalEvents,long *FrameNo,long *StatusWord,long *WritePKTNo,long *ReadPKTNo,long *Command,long *DCNT,long *ADCOutput,long *FEBCommand,long *AlphaCount,long *VetoCount,long *CountLTULD,long *CountGTULD,long *CZTDataRead,long *VetoSpect,int *DataID,int *ModeID,int *ErrorCount,int *ErrorFlag,int *BootPageNo,int *CZTNumber,int *CZTStatus,int *CZTEventReadMode,int *CommandStatus,int *BufferNo,int *BaseAddress,int *VetoSpecRange,int *HKChannelNo,long *VetoSpectrum, long *rows);

void q_sort(double *time,double *CztSecCnt,double *LAXPCTime,long *TotalEvents,long *FrameNo,long *StatusWord,long *WritePKTNo,long *ReadPKTNo,long *Command,long *DCNT,long *ADCOutput,long *FEBCommand,long *AlphaCount,long *VetoCount,long *CountLTULD,long *CountGTULD,long *CZTDataRead,long *VetoSpect,int *DataID,int *ModeID,int *ErrorCount,int *ErrorFlag,int *BootPageNo,int *CZTNumber,int *CZTStatus,int *CZTEventReadMode,int *CommandStatus,int *BufferNo,int *BaseAddress,int *VetoSpecRange,int *HKChannelNo,long *VetoSpectrum, int left, int right);

int writebinTable(char *inputfilename,char *outputFileName,int rows,double *Time,double *CztSecCnt,double *LAXPCTime,long *TotalEvents,long *FrameNo,long *StatusWord,long *WritePKTNo,long *ReadPKTNo,long *Command,long *DCNT,long *ADCOutput,long *FEBCommand,long *AlphaCount,long *VetoCount,long *CountLTULD,long *CountGTULD,long *CZTDataRead,long *VetoSpect,int *DataID,int *ModeID,int *ErrorCount,int *ErrorFlag,int *BootPageNo,int *CZTNumber,int *CZTStatus,int *CZTEventReadMode,int *CommandStatus,int *BufferNo,int *BaseAddress,int *VetoSpecRange,int *HKChannelNo,long *VetoSpectrum);

int main(int argc,char *argv[])
{
double *Time=NULL,*CztSecCnt=NULL,*LAXPCTime=NULL;
long *TotalEvents=NULL,*FrameNo=NULL,*StatusWord=NULL,*WritePKTNo=NULL,*ReadPKTNo=NULL,*Command=NULL,*DCNT=NULL,*ADCOutput=NULL,*FEBCommand=NULL,*AlphaCount=NULL,*VetoCount=NULL,*CountLTULD=NULL,*CountGTULD=NULL,*CZTDataRead=NULL,*VetoSpect=NULL,*VetoSpectrum=NULL;
int *DataID=NULL,*ModeID=NULL,*ErrorCount=NULL,*ErrorFlag=NULL,*BootPageNo=NULL,*CZTNumber=NULL,*CZTStatus=NULL,*CZTEventReadMode=NULL,*CommandStatus=NULL,*BufferNo=NULL,*BaseAddress=NULL,*VetoSpecRange=NULL,*HKChannelNo=NULL;
long i=0,k=0,totevt=0,tempCount=0,totalCount=0;

if(argc<3)
{
	printf("Please provide list of files to be added\n");
	exit(0);
}


Time=(double*)malloc(sizeof(double));
CztSecCnt=(double*)malloc(sizeof(double));
LAXPCTime=(double*)malloc(sizeof(double));
TotalEvents=(long*)malloc(sizeof(long));
FrameNo=(long*)malloc(sizeof(long));
StatusWord=(long*)malloc(sizeof(long));
WritePKTNo=(long*)malloc(sizeof(long));
ReadPKTNo=(long*)malloc(sizeof(long));
Command=(long*)malloc(sizeof(long));
DCNT=(long*)malloc(sizeof(long));
ADCOutput=(long*)malloc(sizeof(long));
FEBCommand=(long*)malloc(sizeof(long));
AlphaCount=(long*)malloc(sizeof(long));
VetoCount=(long*)malloc(sizeof(long));
CountLTULD=(long*)malloc(sizeof(long));
CountGTULD=(long*)malloc(sizeof(long));
CZTDataRead=(long*)malloc(sizeof(long));
VetoSpect=(long*)malloc(sizeof(long));
DataID=(int*)malloc(sizeof(int));
ModeID=(int*)malloc(sizeof(int));
ErrorCount=(int*)malloc(sizeof(int));
ErrorFlag=(int*)malloc(sizeof(int));
BootPageNo=(int*)malloc(sizeof(int));
CZTNumber=(int*)malloc(sizeof(int));
CZTStatus=(int*)malloc(sizeof(int));
CZTEventReadMode=(int*)malloc(sizeof(int));
CommandStatus=(int*)malloc(sizeof(int));
BufferNo=(int*)malloc(sizeof(int));
BaseAddress=(int*)malloc(sizeof(int));
VetoSpecRange=(int*)malloc(sizeof(int));
HKChannelNo=(int*)malloc(sizeof(int));
VetoSpectrum=(long*)malloc(sizeof(long));

for(i=1;i<argc-1;i++)
	totevt+=getnumrows(argv[i]);
	cout<<"\n totevt: "<<totevt;
	Time=(double*)malloc(sizeof(double)*totevt);
	CztSecCnt=(double*)malloc(sizeof(double)*totevt);
	LAXPCTime=(double*)malloc(sizeof(double)*totevt);
	TotalEvents=(long*)malloc(sizeof(long)*totevt);
	FrameNo=(long*)malloc(sizeof(long)*totevt);
	StatusWord=(long*)malloc(sizeof(long)*totevt);
	WritePKTNo=(long*)malloc(sizeof(long)*totevt);
	ReadPKTNo=(long*)malloc(sizeof(long)*totevt);
	Command=(long*)malloc(sizeof(long)*totevt);
	DCNT=(long*)malloc(sizeof(long)*totevt);
	ADCOutput=(long*)malloc(sizeof(long)*totevt);
	FEBCommand=(long*)malloc(sizeof(long)*totevt);
	AlphaCount=(long*)malloc(sizeof(long)*totevt);
	VetoCount=(long*)malloc(sizeof(long)*totevt);
	CountLTULD=(long*)malloc(sizeof(long)*totevt);
	CountGTULD=(long*)malloc(sizeof(long)*totevt);
	CZTDataRead=(long*)malloc(sizeof(long)*totevt);
	VetoSpect=(long*)malloc(sizeof(long)*totevt);
	DataID=(int*)malloc(sizeof(int)*totevt);
	ModeID=(int*)malloc(sizeof(int)*totevt);
	ErrorCount=(int*)malloc(sizeof(int)*totevt);
	ErrorFlag=(int*)malloc(sizeof(int)*totevt);
	BootPageNo=(int*)malloc(sizeof(int)*totevt);
	CZTNumber=(int*)malloc(sizeof(int)*totevt);
	CZTStatus=(int*)malloc(sizeof(int)*totevt);
	CZTEventReadMode=(int*)malloc(sizeof(int)*totevt);
	CommandStatus=(int*)malloc(sizeof(int)*totevt);
	BufferNo=(int*)malloc(sizeof(int)*totevt);
	BaseAddress=(int*)malloc(sizeof(int)*totevt);
	VetoSpecRange=(int*)malloc(sizeof(int)*totevt);
	HKChannelNo=(int*)malloc(sizeof(int)*totevt);
	VetoSpectrum=(long*)malloc(sizeof(long)*totevt*256);
for(i=1;i<argc-1;i++)
{
readBintable(argv[i],&Time[totalCount],&CztSecCnt[totalCount],&LAXPCTime[totalCount],&TotalEvents[totalCount],&FrameNo[totalCount],&StatusWord[totalCount],&WritePKTNo[totalCount],&ReadPKTNo[totalCount],&Command[totalCount],&DCNT[totalCount],&ADCOutput[totalCount],&FEBCommand[totalCount],&AlphaCount[totalCount],&VetoCount[totalCount],&CountLTULD[totalCount],&CountGTULD[totalCount],&CZTDataRead[totalCount],&VetoSpect[totalCount],&DataID[totalCount],&ModeID[totalCount],&ErrorCount[totalCount],&ErrorFlag[totalCount],&BootPageNo[totalCount],&CZTNumber[totalCount],&CZTStatus[totalCount],&CZTEventReadMode[totalCount],&CommandStatus[totalCount],&BufferNo[totalCount],&BaseAddress[totalCount],&VetoSpecRange[totalCount],&HKChannelNo[totalCount],&VetoSpectrum[totalCount],&tempCount);

totalCount+=tempCount;
}
q_sort(Time,CztSecCnt, LAXPCTime, TotalEvents, FrameNo, StatusWord, WritePKTNo, ReadPKTNo, Command, DCNT, ADCOutput, FEBCommand, AlphaCount, VetoCount, CountLTULD, CountGTULD, CZTDataRead, VetoSpect, DataID, ModeID, ErrorCount, ErrorFlag, BootPageNo, CZTNumber, CZTStatus, CZTEventReadMode, CommandStatus, BufferNo, BaseAddress, VetoSpecRange, HKChannelNo, VetoSpectrum,0,totalCount-1);

writebinTable(argv[1],argv[argc-1],totalCount,Time,CztSecCnt, LAXPCTime, TotalEvents, FrameNo, StatusWord, WritePKTNo, ReadPKTNo, Command, DCNT, ADCOutput, FEBCommand, AlphaCount, VetoCount, CountLTULD, CountGTULD, CZTDataRead, VetoSpect, DataID, ModeID, ErrorCount, ErrorFlag, BootPageNo, CZTNumber, CZTStatus, CZTEventReadMode, CommandStatus, BufferNo, BaseAddress, VetoSpecRange, HKChannelNo, VetoSpectrum);



}

void readBintable(char *filename,double *Time,double *CztSecCnt,double *LAXPCTime,long *TotalEvents,long *FrameNo,long *StatusWord,long *WritePKTNo,long *ReadPKTNo,long *Command,long *DCNT,long *ADCOutput,long *FEBCommand,long *AlphaCount,long *VetoCount,long *CountLTULD,long *CountGTULD,long *CZTDataRead,long *VetoSpect,int *DataID,int *ModeID,int *ErrorCount,int *ErrorFlag,int *BootPageNo,int *CZTNumber,int *CZTStatus,int *CZTEventReadMode,int *CommandStatus,int *BufferNo,int *BaseAddress,int *VetoSpecRange,int *HKChannelNo,long *VetoSpectrum, long *rows){

fitsfile *fptr;  
int status=0;
long nelem,naxis2;
fits_open_file(&fptr, filename, READONLY, &status) ;
fits_movabs_hdu(fptr,2,NULL, &status);

fits_read_key(fptr,TLONG,"NAXIS2",&naxis2,NULL,&status);
nelem     = naxis2;
*rows=nelem;

cout<<"\nreading file: "<<filename;

fits_read_col(fptr,TDOUBLE,1,1,1,nelem,NULL,Time,NULL,&status);
fits_read_col(fptr,TDOUBLE,2,1,1,nelem,NULL,CztSecCnt,NULL,&status);
fits_read_col(fptr,TINT,3,1,1,nelem,NULL,DataID,NULL,&status);
fits_read_col(fptr,TINT,4,1,1,nelem,NULL,ModeID,NULL,&status);
fits_read_col(fptr,TLONG,5,1,1,nelem,NULL,TotalEvents,NULL,&status);
fits_read_col(fptr,TLONG,6,1,1,nelem,NULL,FrameNo,NULL,&status);
fits_read_col(fptr,TLONG,7,1,1,nelem,NULL,StatusWord,NULL,&status);
fits_read_col(fptr,TLONG,8,1,1,nelem,NULL,WritePKTNo,NULL,&status);
fits_read_col(fptr,TLONG,9,1,1,nelem,NULL,ReadPKTNo,NULL,&status);
fits_read_col(fptr,TLONG,10,1,1,nelem,NULL,Command,NULL,&status);
fits_read_col(fptr,TDOUBLE,11,1,1,nelem,NULL,LAXPCTime,NULL,&status);
fits_read_col(fptr,TLONG,12,1,1,nelem,NULL,DCNT,NULL,&status);
fits_read_col(fptr,TINT,13,1,1,nelem,NULL,ErrorCount,NULL,&status);
fits_read_col(fptr,TINT,14,1,1,nelem,NULL,ErrorFlag,NULL,&status);
fits_read_col(fptr,TINT,15,1,1,nelem,NULL,BootPageNo,NULL,&status);
fits_read_col(fptr,TINT,16,1,1,nelem,NULL,CZTNumber,NULL,&status);
fits_read_col(fptr,TINT,17,1,1,nelem,NULL,CZTStatus,NULL,&status);
fits_read_col(fptr,TINT,18,1,1,nelem,NULL,CZTEventReadMode,NULL,&status);
fits_read_col(fptr,TINT,19,1,1,nelem,NULL,CommandStatus,NULL,&status);
fits_read_col(fptr,TINT,20,1,1,nelem,NULL,BufferNo,NULL,&status);
fits_read_col(fptr,TINT,21,1,1,nelem,NULL,BaseAddress,NULL,&status);
fits_read_col(fptr,TINT,22,1,1,nelem,NULL,VetoSpecRange,NULL,&status);
fits_read_col(fptr,TINT,23,1,1,nelem,NULL,HKChannelNo,NULL,&status);
fits_read_col(fptr,TLONG,24,1,1,nelem,NULL,ADCOutput,NULL,&status);
fits_read_col(fptr,TLONG,25,1,1,nelem,NULL,FEBCommand,NULL,&status);
fits_read_col(fptr,TLONG,26,1,1,nelem,NULL,AlphaCount,NULL,&status);
fits_read_col(fptr,TLONG,27,1,1,nelem,NULL,VetoCount,NULL,&status);
fits_read_col(fptr,TLONG,28,1,1,nelem,NULL,CountLTULD,NULL,&status);
fits_read_col(fptr,TLONG,29,1,1,nelem,NULL,CountGTULD,NULL,&status);
fits_read_col(fptr,TLONG,30,1,1,nelem,NULL,CZTDataRead,NULL,&status);
fits_read_col(fptr,TLONG,31,1,1,nelem,NULL,VetoSpectrum,NULL,&status);
fits_close_file(fptr, &status);
return;
}


long getnumrows(char *filename){
fitsfile *fptr;
int status=0;
long naxis2;
fits_open_file(&fptr, filename, READONLY, &status);
fits_movabs_hdu(fptr,2,NULL, &status);
fits_read_key(fptr,TLONG,"NAXIS2",&naxis2,NULL,&status);
fits_close_file(fptr,&status);
return naxis2;
}

void q_sort(double *time,double *CztSecCnt,double *LAXPCTime,long *TotalEvents,long *FrameNo,long *StatusWord,long *WritePKTNo,long *ReadPKTNo,long *Command,long *DCNT,long *ADCOutput,long *FEBCommand,long *AlphaCount,long *VetoCount,long *CountLTULD,long *CountGTULD,long *CZTDataRead,long *VetoSpect,int *DataID,int *ModeID,int *ErrorCount,int *ErrorFlag,int *BootPageNo,int *CZTNumber,int *CZTStatus,int *CZTEventReadMode,int *CommandStatus,int *BufferNo,int *BaseAddress,int *VetoSpecRange,int *HKChannelNo,long *VetoSpectrum, int left, int right)
{
 	long  l_hold, r_hold;
 	double pivot=0;
	double temp_time,temp_CztSecCnt,temp_LAXPCTime;
	long temp_TotalEvents,temp_FrameNo,temp_StatusWord,temp_WritePKTNo,temp_ReadPKTNo,temp_Command,temp_DCNT,temp_ADCOutput,temp_FEBCommand,temp_AlphaCount,temp_VetoCount,temp_CountLTULD,temp_CountGTULD,temp_CZTDataRead,temp_VetoSpect,temp_VetoSpectrum;
	int temp_DataID,temp_ModeID,temp_ErrorCount,temp_ErrorFlag,temp_BootPageNo,temp_CZTNumber,temp_CZTStatus,temp_CZTEventReadMode,temp_CommandStatus,temp_BufferNo,temp_BaseAddress,temp_VetoSpecRange,temp_HKChannelNo;
  	l_hold = left;
  	r_hold = right;
  	pivot = time[left];
temp_CztSecCnt=CztSecCnt[left];
temp_LAXPCTime=LAXPCTime[left];
temp_TotalEvents=TotalEvents[left];
temp_FrameNo=FrameNo[left];
temp_StatusWord=StatusWord[left];
temp_WritePKTNo=WritePKTNo[left];
temp_ReadPKTNo=ReadPKTNo[left];
temp_Command=Command[left];
temp_DCNT=DCNT[left];
temp_ADCOutput=ADCOutput[left];
temp_FEBCommand=FEBCommand[left];
temp_AlphaCount=AlphaCount[left];
temp_VetoCount=VetoCount[left];
temp_CountLTULD=CountLTULD[left];
temp_CountGTULD=CountGTULD[left];
temp_CZTDataRead=CZTDataRead[left];
temp_VetoSpect=VetoSpect[left];
temp_VetoSpectrum=VetoSpectrum[left];
temp_DataID=DataID[left];
temp_ModeID=ModeID[left];
temp_ErrorCount=ErrorCount[left];
temp_ErrorFlag=ErrorFlag[left];
temp_BootPageNo=BootPageNo[left];
temp_CZTNumber=CZTNumber[left];
temp_CZTStatus=CZTStatus[left];
temp_CZTEventReadMode=CZTEventReadMode[left];
temp_CommandStatus=CommandStatus[left];
temp_BufferNo=BufferNo[left];
temp_BaseAddress=BaseAddress[left];
temp_VetoSpecRange=VetoSpecRange[left];
temp_HKChannelNo=HKChannelNo[left];
  	while (left < right)
  	{
    		while ((time[right] >= pivot) && (left < right))
      			right--;
    		if (left != right)
    		{
      			time[left] = time[right];
			CztSecCnt[left]=CztSecCnt[right];
			LAXPCTime[left]=LAXPCTime[right];
			TotalEvents[left]=TotalEvents[right];
			FrameNo[left]=FrameNo[right];
			StatusWord[left]=StatusWord[right];
			WritePKTNo[left]=WritePKTNo[right];
			ReadPKTNo[left]=ReadPKTNo[right];
			Command[left]=Command[right];
			DCNT[left]=DCNT[right];
			ADCOutput[left]=ADCOutput[right];
			FEBCommand[left]=FEBCommand[right];
			AlphaCount[left]=AlphaCount[right];
			VetoCount[left]=VetoCount[right];
			CountLTULD[left]=CountLTULD[right];
			CountGTULD[left]=CountGTULD[right];	
			CZTDataRead[left]=CZTDataRead[right];
			VetoSpect[left]=VetoSpect[right];
			VetoSpectrum[left]=VetoSpectrum[right];
			DataID[left]=DataID[right];
			ModeID[left]=ModeID[right];
			ErrorCount[left]=ErrorCount[right];
			ErrorFlag[left]=ErrorFlag[right];
			BootPageNo[left]=BootPageNo[right];
			CZTNumber[left]=CZTNumber[right];
			CZTStatus[left]=CZTStatus[right];
			CZTEventReadMode[left]=CZTEventReadMode[right];
			CommandStatus[left]=CommandStatus[right];
			BufferNo[left]=BufferNo[right];
			BaseAddress[left]=BaseAddress[right];
			VetoSpecRange[left]=VetoSpecRange[right];
			HKChannelNo[left]=HKChannelNo[right];
			left++;
    		}
    		while ((time[left] <= pivot) && (left < right))
      			left++;
    		if (left != right)
    		{
      			time[right] = time[left];
			CztSecCnt[right]=CztSecCnt[left];
			LAXPCTime[right]=LAXPCTime[left];
			TotalEvents[right]=TotalEvents[left];
			FrameNo[right]=FrameNo[left];
			StatusWord[right]=StatusWord[left];
			WritePKTNo[right]=WritePKTNo[left];
			ReadPKTNo[right]=ReadPKTNo[left];
			Command[right]=Command[left];
			DCNT[right]=DCNT[left];
			ADCOutput[right]=ADCOutput[left];
			FEBCommand[right]=FEBCommand[left];
			AlphaCount[right]=AlphaCount[left];
			VetoCount[right]=VetoCount[left];
			CountLTULD[right]=CountLTULD[left];
			CountGTULD[right]=CountGTULD[left];	
			CZTDataRead[right]=CZTDataRead[left];
			VetoSpect[right]=VetoSpect[left];
			VetoSpectrum[right]=VetoSpectrum[left];
			DataID[right]=DataID[left];
			ModeID[right]=ModeID[left];
			ErrorCount[right]=ErrorCount[left];
			ErrorFlag[right]=ErrorFlag[left];
			BootPageNo[right]=BootPageNo[left];
			CZTNumber[right]=CZTNumber[left];
			CZTStatus[right]=CZTStatus[left];
			CZTEventReadMode[right]=CZTEventReadMode[left];
			CommandStatus[right]=CommandStatus[left];
			BufferNo[right]=BufferNo[left];
			BaseAddress[right]=BaseAddress[left];
			VetoSpecRange[right]=VetoSpecRange[left];
			HKChannelNo[right]=HKChannelNo[left];
      			right--;
    		}
  	}
 time[left] = pivot;
CztSecCnt[left]=	temp_CztSecCnt;		
LAXPCTime[left]= 	temp_LAXPCTime;		
TotalEvents[left]=	temp_TotalEvents;	
FrameNo[left]=   	temp_FrameNo	;	
StatusWord[left]=	temp_StatusWord	;	
WritePKTNo[left]=	temp_WritePKTNo	;	
ReadPKTNo[left]= 	temp_ReadPKTNo	;	
Command[left]=   	temp_Command	;	
DCNT[left]=      	temp_DCNT	;	
ADCOutput[left]= 	temp_ADCOutput;
FEBCommand[left]=	temp_FEBCommand;	
AlphaCount[left]=	temp_AlphaCount	;
VetoCount[left]= 	temp_VetoCount	;
CountLTULD[left]=	temp_CountLTULD	;
CountGTULD[left]=	temp_CountGTULD	;	
CZTDataRead[left]=	temp_CZTDataRead;	
VetoSpect[left]= 	temp_VetoSpect	;
VetoSpectrum[left]=	temp_VetoSpectrum;
DataID[left]=    	temp_DataID	;	
ModeID[left]=    	temp_ModeID	;	
ErrorCount[left]=	temp_ErrorCount	;
ErrorFlag[left]= 	temp_ErrorFlag	;	
BootPageNo[left]=	temp_BootPageNo	;
CZTNumber[left]= 	temp_CZTNumber	;
CZTStatus[left]= 	temp_CZTStatus	;
CZTEventReadMode[left]=temp_CZTEventReadMode	;
CommandStatus[left]=temp_CommandStatus;
BufferNo[left]=        	temp_BufferNo;	
BaseAddress[left]=     	temp_BaseAddress;	
VetoSpecRange[left]=   	temp_VetoSpecRange;	
HKChannelNo[left]=     	temp_HKChannelNo;	

  	pivot = left;
  	left = l_hold;
  	right = r_hold;
  	if (left < pivot)
  		q_sort(time, CztSecCnt, LAXPCTime, TotalEvents, FrameNo, StatusWord, WritePKTNo, ReadPKTNo, Command, DCNT, ADCOutput, FEBCommand, AlphaCount, VetoCount, CountLTULD, CountGTULD, CZTDataRead, VetoSpect, DataID, ModeID, ErrorCount, ErrorFlag, BootPageNo, CZTNumber, CZTStatus, CZTEventReadMode, CommandStatus, BufferNo, BaseAddress, VetoSpecRange, HKChannelNo, VetoSpectrum,left, pivot-1);
  	if (right > pivot)
    		q_sort(time,CztSecCnt, LAXPCTime, TotalEvents, FrameNo, StatusWord, WritePKTNo, ReadPKTNo, Command, DCNT, ADCOutput, FEBCommand, AlphaCount, VetoCount, CountLTULD, CountGTULD, CZTDataRead, VetoSpect, DataID, ModeID, ErrorCount, ErrorFlag, BootPageNo, CZTNumber, CZTStatus, CZTEventReadMode, CommandStatus, BufferNo, BaseAddress, VetoSpecRange, HKChannelNo, VetoSpectrum, pivot+1, right);
}

int writebinTable(char *inputfilename,char *outputFileName,int rows,double *Time,double *CztSecCnt,double *LAXPCTime,long *TotalEvents,long *FrameNo,long *StatusWord,long *WritePKTNo,long *ReadPKTNo,long *Command,long *DCNT,long *ADCOutput,long *FEBCommand,long *AlphaCount,long *VetoCount,long *CountLTULD,long *CountGTULD,long *CZTDataRead,long *VetoSpect,int *DataID,int *ModeID,int *ErrorCount,int *ErrorFlag,int *BootPageNo,int *CZTNumber,int *CZTStatus,int *CZTEventReadMode,int *CommandStatus,int *BufferNo,int *BaseAddress,int *VetoSpecRange,int *HKChannelNo,long *VetoSpectrum){
	
	fitsfile *infptr;
	fitsfile *outfptr;
	int status=0;
	fits_open_file(&infptr,inputfilename,READONLY,&status);
	fits_create_file(&outfptr,outputFileName,&status);
	if(status) {fits_report_error(stderr,status); return(EXIT_FAILURE);}
	fits_movabs_hdu(infptr,1,NULL, &status);
	fits_copy_hdu(infptr,outfptr,NULL,&status);
	fits_movabs_hdu(infptr,2,NULL, &status);
	fits_copy_hdu(infptr,outfptr,NULL,&status);
	cout<<"\n new output file created.... "<<outputFileName;

fits_write_col(outfptr,TDOUBLE,1,1,1,rows,Time,&status);
fits_write_col(outfptr,TDOUBLE,2,1,1,rows,CztSecCnt,&status);
fits_write_col(outfptr,TINT,3,1,1,rows,DataID,&status);
fits_write_col(outfptr,TINT,4,1,1,rows,ModeID,&status);
fits_write_col(outfptr,TLONG,5,1,1,rows,TotalEvents,&status);
fits_write_col(outfptr,TLONG,6,1,1,rows,FrameNo,&status);
fits_write_col(outfptr,TLONG,7,1,1,rows,StatusWord,&status);
fits_write_col(outfptr,TLONG,8,1,1,rows,WritePKTNo,&status);
fits_write_col(outfptr,TLONG,9,1,1,rows,ReadPKTNo,&status);
fits_write_col(outfptr,TLONG,10,1,1,rows,Command,&status);
fits_write_col(outfptr,TDOUBLE,11,1,1,rows,LAXPCTime,&status);
fits_write_col(outfptr,TLONG,12,1,1,rows,DCNT,&status);
fits_write_col(outfptr,TINT,13,1,1,rows,ErrorCount,&status);
fits_write_col(outfptr,TINT,14,1,1,rows,ErrorFlag,&status);
fits_write_col(outfptr,TINT,15,1,1,rows,BootPageNo,&status);
fits_write_col(outfptr,TINT,16,1,1,rows,CZTNumber,&status);
fits_write_col(outfptr,TINT,17,1,1,rows,CZTStatus,&status);
fits_write_col(outfptr,TINT,18,1,1,rows,CZTEventReadMode,&status);
fits_write_col(outfptr,TINT,19,1,1,rows,CommandStatus,&status);
fits_write_col(outfptr,TINT,20,1,1,rows,BufferNo,&status);
fits_write_col(outfptr,TINT,21,1,1,rows,BaseAddress,&status);
fits_write_col(outfptr,TINT,22,1,1,rows,VetoSpecRange,&status);
fits_write_col(outfptr,TINT,23,1,1,rows,HKChannelNo,&status);
fits_write_col(outfptr,TLONG,24,1,1,rows,ADCOutput,&status);
fits_write_col(outfptr,TLONG,25,1,1,rows,FEBCommand,&status);
fits_write_col(outfptr,TLONG,26,1,1,rows,AlphaCount,&status);
fits_write_col(outfptr,TLONG,27,1,1,rows,VetoCount,&status);
fits_write_col(outfptr,TLONG,28,1,1,rows,CountLTULD,&status);
fits_write_col(outfptr,TLONG,29,1,1,rows,CountGTULD,&status);
fits_write_col(outfptr,TLONG,30,1,1,rows,CZTDataRead,&status);
fits_write_col(outfptr,TLONG,31,1,1,rows,VetoSpectrum,&status);
if(status) {fits_report_error(stderr,status); return(EXIT_FAILURE);}
fits_close_file(infptr,&status);
fits_close_file(outfptr,&status);
if(status) {fits_report_error(stderr,status); return(EXIT_FAILURE);}
}



