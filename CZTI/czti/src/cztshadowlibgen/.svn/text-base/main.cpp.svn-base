/* 
 * @file  Cztshadowlibgen
 * @author Ajay Vibhute
 * @date Created on 12 Jan 2016 1:21 PM
 * @version 0.1
 * @description: This code generates a shadow library
 */
#include "glog/logging.h"
#include <iostream>
#include <vector>
#include "maskGeometry.h"
#include "Exposure.h"
#include <unistd.h> 
   #include <sys/types.h>
       #include <sys/stat.h>
       #include <fcntl.h>
using namespace std;

int main(int argc, char** argv) 
{
    google::InitGoogleLogging(argv[0]);
    google::SetStderrLogging(google::INFO);
    float*shadow=(float*)malloc(sizeof(float)*4*64*64);

    int i=0,j=0,k=0;
    /*
    time_t st,et;
    st=time(NULL);
    checkPFILESenv();
    checkParFile("cztexpmap");

    Cztexpmap par;
    par.read(argc,argv);
    par.display();
    LOG(INFO)<<"CZTEXPMAP STARTED....................";
    int status=0;
    status=par.cztexpmap_process();
    if(status){
        LOG(ERROR)<<"***Error in cztexpmap process***";
    }else
        LOG(INFO)<<"CZTEXPMAP COMPLETED SUCCESSFULLY";
    et=time(NULL);
    LOG(INFO)<<"Time Elapsed---"<<et-st<<" seconds"<<endl;
    return 0;
    */
	int filedesc = open("shadow_lib_-3_3.dat", O_CREAT|O_WRONLY);
	TanMaskGeometry tanmask;
	vector <vector <unsigned char> > fullfinermask;
	string compMaskFile;
	string camGeomFile;
	ExposureTable expTable;

	LOG(INFO) << "Reading compressed mask file to create 18200x18200 full uncompressed mask.";
	if (tanmask.read_compressed_mask("/home/cztipoc/latest_CZTI_pipeline/CZTI/new_CALDB/data/as1/czti/bcf/AS1cztmask_oversampled20150707v01.fits")) {
	    return EXIT_FAILURE;
	}
	if (tanmask.get_full_finer_mask()) {
	    LOG(ERROR) << "Error in generating full finer mask from individual quadrant masks.";
	    return EXIT_FAILURE;
	}

	//storing full finer mask
	fullfinermask = tanmask.get_fullUnMask();
	//clearing tanmask stored variables to save memory
	tanmask.reset_full_uncomressed_mask();
	//COMPRESSED MASK READING END

	//Call create_exposure_array function (common/Exposure.cpp)

	camGeomFile="/home/cztipoc/latest_CZTI_pipeline/CZTI/new_CALDB/data/as1/czti/bcf/AS1cztgeometry20150707v01.fits";
	float tx=0,ty=0,resol=0.1;
	float start=-3,end=3;

	int detx,dety,locx,locy;
	int indexPix=0,nrows=0;
	float fi;
	int detid=0,pixid=0,quad=0;
	detx=0;dety=0;

 	fitsfile *fptr;       /* pointer to the FITS file, defined in fitsio.h */
    	int status=0, hdutype;
    	long firstrow, firstelem;
	int tfields   = 6;       /* table will have 3 columns */
	char *ttype[] = { "ThetaX", "ThetaY", "Q0","Q1","Q2","Q3"};
	char *tform[] = { "1E",     "1E",       "4096E","4096E","4096E" ,"4096E"   };
    	char *tunit[] = { "",      "",       ""  ,"","",""  };
	char *filename="shadowlib.fits",*extname="ShadowLibrary";
	status=0;
	remove(filename);
 	if ( fits_create_file(&fptr, filename,  &status) ) 
	{
			printf("Error while creating new file\n");
			exit(0);
	}
printf("Creating new table\n");
	if ( fits_create_tbl( fptr, BINARY_TBL, nrows, tfields, ttype, tform,tunit, extname, &status) )
	{
		printf("Error while creating output file\n");
		exit(0);
	}
printf("Table created\n");

	firstrow=1;
	firstelem=1;
	for(tx=start;tx<end;tx+=resol)
	{
		for(ty=start;ty<end;ty+=resol)
		{
			printf("******************* %f\t%f\n",tx,ty);
			create_exposure_array(tx,ty, camGeomFile,fullfinermask, expTable, 1, 10,"", 40, 240, 1);

	try
	{
        	expTable.generate_index_table(false);
    	} 
	catch (ErrorHandler errHandler)
	{
        	throw errHandler;
    	}
			fits_write_col(fptr, TFLOAT, 1, firstrow, firstelem, 1, &tx,&status);
			fits_write_col(fptr, TFLOAT, 2, firstrow, firstelem, 1, &ty,&status);
			for(quad=0;quad<4;quad++)
			{
			for(i=0;i<64;i++)
			{	
				for(j=0;j<64;j++)
				{

					detx=i;	
					dety=j;
 					generate_locx_locy(detx, quad,  dety,  locx,  locy);
					indexPix = expTable.indexTable[locy][locx];
					fi=expTable.openfracArray[indexPix][0];
					shadow[(j*64)+i]=fi;
				}
			}
	//write file here		
			write(filedesc,shadow,sizeof(float)*4096);

			fits_write_col(fptr, TFLOAT, quad+3, firstrow, firstelem, 4096, shadow,&status);
			}
			firstrow++;
		}
	}


	close(filedesc);
	  if ( fits_close_file(fptr, &status) )
		  printf("Error while closign th eoutput file\n");

	//int create_exposure_array(float thetaxd, float thetayd, string cameraGeomFilename,vector <vector <unsigned char> > &fullfinermask, ExposureTable &expTable, int includeMask, int maskOversampling, string eboundsFilename, float minEnergy, float maxEnergy, int nchannels)
    /*fitsfile *fptr;      
    int status, ii, jj;
    long  fpixel=1, nelements=64*64, exposure;
    unsigned short *array[200];
printf("Writign output to fits file\n");
    char filename[] = "atestfil.fit";             
    int bitpix   =  FLOAT_IMG; 
    long naxis    =   2;  
    long naxes[2] = { 64,64 };  
   status=0; 
    if (fits_create_file(&fptr, "shadow.fits", &status)) 
     printf("Error in createing file\n");
if ( fits_create_img(fptr,  bitpix, naxis, naxes, &status) )
	printf("Errorin creating image\n");
printf("Created output file\n");
if ( fits_write_img(fptr, TFLOAT, fpixel, nelements, shadow, &status) )
	printf("Error in writoing image\n");
printf("Wrote values to file\n");
if ( fits_close_file(fptr, &status) ) 
	printf("Error in closing the file\n");
printf("Closed the output file\n");*/
}
