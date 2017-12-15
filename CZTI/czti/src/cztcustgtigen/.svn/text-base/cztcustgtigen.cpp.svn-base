///modified

#include"cztgtigenv2.h"
#include<stdio.h>

using namespace std;

cztgtigen::cztgtigen(){
    
    strcpy(modulename,"cztgtigen_v");
    strcat(modulename,VERSION);
    strcpy(thresholdfile,"");
    strcpy(mkffile,"");
}

int cztgtigen::read(int argc, char** argv){
    int status=0;
    
    if(PIL_OK!=(status=PILInit(argc,argv))){
        LOG(ERROR)<<"***Error Initializing PIL***";
        return status;
    }
/*    
    LOG(INFO)<<"--------------------------------------";
    LOG(INFO)<<"Description for run mode parameter for CZTGTIGEN";
    LOG(INFO)<<"--------------------------------------";
    LOG(INFO)<<"It can have 4 values- copy/mkf/custom/custom-refined";
    LOG(INFO)<<"1) 'mkf'          - In mkf mode, it creates a gti based on the valid ranges given in ";
    LOG(INFO)<<"                    threshold file for each mkf parameter";
    LOG(INFO)<<"2) 'custom'       - In custom mode, it creates gti based on intervals given by user";
    LOG(INFO)<<"3) 'mkf-refined'  - In this mode it refines the gti produced in mkf mode with level2 gti";
    LOG(INFO)<<"4) 'custom-refined'- In this mode it refines the gti produced in custom mode with level2 gti";
    LOG(INFO)<<"5) 'copy' - copies input file to output file";
    LOG(INFO);

*/
/*    if(PIL_OK!=(status=PILGetFname("eventfile",eventfile))){
        LOG(ERROR)<<"***Error Reading eventfile name***";
        return status;
    }
  */  
    if(PIL_OK!=(status=PILGetFname("mkffile",mkffile))){
        LOG(ERROR)<<"***Error reading mkffile file:"<<mkffile<<"***";
        return status;
    }
    if(PIL_OK!=(status=PILGetFname("thresholdfile",thresholdfile))){
        LOG(ERROR)<<"***Error reading threshold file:"<<thresholdfile<<"***";
        return status;
    }

    if(PIL_OK!=(status=PILGetFname("outfile",outfile))){
        LOG(ERROR)<<"***Error reading outfile:"<<outfile<<"***";
        return status;
    }
/*
    if(PIL_OK!=(status=PILGetFname("usergtifile",usergtifile))){
        LOG(ERROR)<<"***Error reading threshold file:"<<usergtifile<<"***";
        return status;
    } 
    
    if(PIL_OK!=(status=PILGetBool("history",&history))){
        LOG(ERROR)<<"***Error reading history parameter***";
        return status;
    }
  */  
    if(PIL_OK!=(status=PILGetBool("clobber",&clobber))){
        LOG(ERROR)<<"***Error reading clobber parameter***";
        return status;
    }

    PILClose(status);
    return (EXIT_SUCCESS);
}

int cztgtigen::read(char *eventfile,char *mkffile,char *thresholdfile,char *outfile, char *usergtifile,int clobber,int history)
{    
    strcpy(this->eventfile,eventfile);
    strcpy(this->mkffile,mkffile);
    strcpy(this->thresholdfile,thresholdfile);
    strcpy(this->outfile,outfile);
	strcpy(this->usergtifile,usergtifile);
    this->clobber=clobber;
    this->history=history;
    return (EXIT_SUCCESS);
}

void cztgtigen::display(){
    LOG(INFO)<<"----------------------------------------------------------------------------";
    LOG(INFO)<<"                           CZTGTIGEN PARAMETERS                               ";
    LOG(INFO)<<"----------------------------------------------------------------------------";
    LOG(INFO)<<"Eventfile          : "<<eventfile;
    LOG(INFO)<<"MKF file           : "<<mkffile;     
    LOG(INFO)<<"MKF Threshold file : "<<thresholdfile;
    LOG(INFO)<<"Output gti file    : "<<outfile;      
	LOG(INFO)<<"User gti file      : "<<usergtifile; 
    LOG(INFO)<<"Clobber            : "<<clobber;
    LOG(INFO)<<"History            : "<<history;
    LOG(INFO)<<"----------------------------------------------------------------------------";
}

int cztgtigen::cztgtigenProcess(){

    if(clobber==YES){
        unlink(outfile);
    }
    else{
        LOG(ERROR)<<""<<outfile<<"  already exists";
        LOG(ERROR)<<" Use the option 'clobber=yes' to overwrite the file";
        return (EXIT_FAILURE);
    }
    
    vector<GTIrecord> mkfgti; 

    fitsfile *fgti,*fout,*fevt,*fptrmkf,*fuser;
    int status=0;
    
        GTIrecord objgti;
        mkfgti.clear();

       GTIhandler GTIfinal;
        Mkf mkf;
        vector <double> tstart, tstop;    //mayuri

        tstart.clear(); tstop.clear();
        if(mkf.read_mkf_file((string) mkffile)){
            LOG(ERROR) << "Error in reading mkf file: " << mkffile;
            return EXIT_FAILURE;
        }


	
	/************************start of mayuri's code*******************/


	int hdunum, hdutype,  nfound, anynull, ii,i,intnull,ncols,cnumtime,cnumlat,cnumlon;
  	long frow, felem, nelem,nrows, longnull;
  	float floatnull, *lat,*lon;
  	char strnull[10], *name[50];
 	double doublenull,*time,starttime[100],endtime[100],exposure=0.0,start,stop;
 	int SAAlatref=-6,SAAlongmin=-110,SAAlongmax=0,index=0,m=0,n=0,k=1,l=0;
  	float SAAshift1,SAAshift2,SAAslopeleft=3.5,SAAsloperight=0;
	 long firstrow, firstelem;
		
	
	 status = 0;
 	 hdunum = 2;
  	//printf("Input File name : %s",inputfile);
  
      fits_open_file(&fptrmkf, mkffile, READONLY, &status);
      if(status){
            fits_report_error(stderr,status);
            LOG(ERROR)<<"***Error in opening output gti file";
            return (EXIT_FAILURE);
      }
 
      // move to the HDU 
      fits_movabs_hdu(fptrmkf, hdunum, &hdutype, &status); 
	  if(status){
            fits_report_error(stderr,status);
            LOG(ERROR)<<"***Error in moving ";
            return (EXIT_FAILURE);
      }

	   
      if (hdutype == BINARY_TBL)
			LOG(ERROR)<<"\nReading binary table in HDU"<<hdunum<<"\n";
      else
		{
          LOG(ERROR)<<"Error: this HDU is not an ASCII or binary table\n";
          fits_report_error(stderr,status);
		}


 	fits_get_num_rows(fptrmkf, &nrows, &status);
  	
	//printf("Number of rows %d\n",nrows);

   	lat  = (float*)malloc(nrows * sizeof(float));
	lon  = (float*)malloc(nrows * sizeof(float));
	time  = (double*)malloc(nrows * sizeof(double));

	fits_get_colnum(fptrmkf,CASEINSEN, "TIME",&cnumtime, &status);
   	fits_get_colnum(fptrmkf,CASEINSEN, "EARTHLAT", &cnumlat, &status);
	fits_get_colnum(fptrmkf,CASEINSEN, "EARTHLON",&cnumlon, &status);
	
      //	printf("Column Number: TIME=%d,LAT=%d,LON=%d\n",cnumtime,cnumlat,cnumlon);
      frow      = 1;
      felem     = 1;
      strcpy(strnull, " ");
      longnull  = 0;
      floatnull = 0.;

      fits_read_col(fptrmkf, TDOUBLE, cnumtime, frow, felem, nrows, &doublenull, time, 
                   &anynull, &status);      
      fits_read_col(fptrmkf,TFLOAT, cnumlat, frow, felem, nrows, &floatnull, lat, 
                   &anynull, &status); 
      fits_read_col(fptrmkf,TFLOAT, cnumlon, frow, felem, nrows, &floatnull, lon,
		    &anynull, &status);

      for (ii = 0; ii < nrows; ii++){
	//calculate SAAshift1 and SAAshift2
		SAAshift1=SAAslopeleft*(lat[ii]-SAAlatref);
		SAAshift2=SAAsloperight*(lat[ii]-SAAlatref);
		
			if((lon[ii]>(SAAlongmin+SAAshift1)) &&
				(lon[ii]<(SAAlongmax+SAAshift2)) ){
				
				//printf("SAA %f\t%f\t%d\n",time[ii],time[ii-1],ii);
				n=0;
				if(k==0){
					endtime[l]=time[ii-1];
					l++;
				}
				 k++;
				
			}
			else
			{
				k=0;			
				if(n==0){
				         starttime[m]=time[ii];
					 m++;
				}
				n++;

			}
			if(ii==nrows-1) endtime[l]=time[nrows-1];
			
			
	}
	//tstart.push_back(starttime[0]);
	//tstop.push_back(endtime[l]);
 
	start=starttime[0];
	stop=endtime[l];
	printf("STARTTIME \t ENDTIME\n");
	
	tstart.assign(starttime, starttime+4);
	tstop.assign(endtime, endtime+4);
	
        for (ii = 0; ii <m && ii <=l; ii++){
		printf("%10.2f\t%10.2f\n",starttime[ii],endtime[ii]);
		exposure=exposure+(endtime[ii]-starttime[ii]);
		}
		
	 printf("TSTART : %10.2f\tTSTOP : %10.2f\n",start,stop);	
	printf("EXPOSURE : %10.2f\n",exposure);


  	/************End of Mayuri's code********************/	
  	
  	  LOG(INFO)<<"Using threadshold file:  "<<thresholdfile;
	//mkf gti contains the GTI generated using MKF file
        if(mkf.get_filtered_gti(thresholdfile, tstart, tstop)){
            LOG(ERROR) << "Error in generating mkf filtered gti";
            return EXIT_FAILURE;
        }

		//commented by mayuri
       /*
	   	for(int itime=0; itime<tstart.size(); itime++){
            objgti.tstart= tstart[itime];
            objgti.tstop= tstop[itime];
            mkfgti.push_back(objgti);
	   // LOG(INFO)<<objgti.tstart<<"\t"<<objgti.tstop;
	//	printf("%f\t%f\n",objgti.tstart,objgti.tstop);
        }
    
        LOG(INFO) << "Completed GTI creation based on mkf";

    	if(tstart.size() == 0) 
	{
		LOG(ERROR)<<"No Good time intervals found with MKF thresholds... EXITING....";
		return(EXIT_FAILURE);
	}

 	if (create_empty_fitsfile(outfile, GTITEMPLATE))
	{
        	LOG(ERROR) << "Error in creating empty GTI file " << outfile;
        	return EXIT_FAILURE;
    	}
	//this section copies all the required header parameters, 
	//need to add part which will write it
 	fits_open_file(&fgti, outfile, READWRITE, &status);
        if(status){
            fits_report_error(stderr,status);
            LOG(ERROR)<<"***Error in opening output gti file";
            return (EXIT_FAILURE);
        }
*/



    int tfields   = 2;      
    char extname[] = "GTI";           /* extension name */
    
   
	/* define the name, datatype, and physical units for the 2 columns */
    char *ttype[] = { "START", "STOP"};
    char *tform[] = { "1D","1D"};
    char *tunit[] = { "sec", "sec"};
    //remove(outfile);
    status=0;
    LOG(ERROR)<<"Filename is:"<<outfile;

    if (fits_create_file(&fgti, outfile, &status)) /* create new FITS file */
         LOG(ERROR)<<status;           /* call printerror if error occurs */

    /* open the FITS file containing a primary array and an ASCII table */
    fits_open_file(&fgti, outfile, READWRITE, &status);
         if(status)
            LOG(ERROR)<<status;

    /* append a new empty binary table onto the FITS file */
    if ( fits_create_tbl( fgti, BINARY_TBL, l+1, tfields, ttype, tform,
                tunit, extname, &status) )
         LOG(ERROR)<<status;
   
    fits_write_key(fgti, TDOUBLE,"TSTART", &start,"Start time of observation", &status);
    fits_write_key(fgti, TDOUBLE,"TSTOP", &stop,"Stop time of observation", &status);
    fits_write_key(fgti, TDOUBLE,"EXPOSURE", &exposure,"Exposure time", &status);

    firstrow  = 1;  /* first row in table to write   */
    firstelem = 1;  /* first element in row  (ignored in ASCII tables) */


    fits_write_col(fgti, TDOUBLE, 1, firstrow, firstelem, l+1, starttime,
                   &status);
    //fits_write_col(fgti, TDOUBLE, 2,  i+1,        1,        1,   &GTI[i].tstop, &status);
    fits_write_col(fgti, TDOUBLE, 2, firstrow, firstelem, l+1, endtime,
                   &status);
   

        //char extname[20];  //mayuri

       // sprintf(extname,"GTI");       //mayuri
       //	GTIfinal.set_GTI(mkfgti);    //mayuri
       //	GTIfinal.write_gti_file(fgti,extname);   //mayuri
        fits_close_file(fgti,&status);
        if(status){
            fits_report_error(stderr,status);
            LOG(ERROR)<<"***Error in closing GTI *** "<<status;
            return (EXIT_FAILURE);
        }
    return (EXIT_SUCCESS);
}

/*
int refineGTI(vector<GTI> &gti2refine,vector<GTI> &ingti){
    int s1=gti2refine.size();
    int s2=ingti.size();
    if(s1<=0){
        LOG(ERROR)<<"Number of intervals are zero/less than zero in GTI";
        return (EXIT_FAILURE);
    }
    if(s2<=0){
        LOG(INFO)<<"Number of intervals in level2 GTI is zero";
        return (EXIT_SUCCESS);
    }
    int i,j;
   
    vector<GTI> outgti;
    
    GTI objgti;
    
    for(i=0;i<s1;i++){
        for(j=0;j<s2;j++){
            if(gti2refine[i].tstart>=ingti[j].tstart && gti2refine[i].tstop<=ingti[j].tstop)
                objgti.tstart=gti2refine[i].tstart;  
            //cout<<gti2refine[i].tstart<<" "<<ingti[i].tstart;
            if(gti2refine[i].tstart>ingti[j].tstart)
                objgti.tstart=gti2refine[i].tstart;
            else
                objgti.tstart=ingti[j].tstart;
            //cout<<gti2refine[i].tstop<<" "<<ingti[i].tstop;
            if(gti2refine[i].tstop<ingti[j].tstop)
                objgti.tstop=gti2refine[i].tstop;
            else
                objgti.tstop=ingti[j].tstop;
            outgti.push_back(objgti);
            //cout<<objgti.tstart<<" - "<<objgti.tstop;
            
        }
    }
    
    gti2refine.clear();
    vector<GTI>::iterator iter=outgti.begin();
    while(iter!=outgti.end()){
        if((*iter).tstart<=(*iter).tstop){
            gti2refine.push_back((*iter));
            LOG(INFO)<<(*iter).tstart<<"-"<<(*iter).tstop;
        }
        iter++;    
    }
   return (EXIT_SUCCESS);
}  

*/
void cztgtigen::getHistory(vector<string> &vhistory){
    char *user=getlogin();
    string str="Module run by "+(string)user;
    vhistory.push_back(str);
    vhistory.push_back("Parameter List START for "+(string)modulename);
    vhistory.push_back("P1 eventfile="+(string)eventfile);
    vhistory.push_back("P2 mkffile="+(string)mkffile);
    vhistory.push_back("P3 thresholdfile="+(string)thresholdfile);
    vhistory.push_back("P4 outfile="+(string)outfile);
    if(clobber==YES) 
        vhistory.push_back("P5 clobber=yes");
    else
        vhistory.push_back("P5 clobber=no");
    if(history==YES)
        vhistory.push_back("P6 history=yes");
    else
        vhistory.push_back("P6 history=no");
    vhistory.push_back("Parameter List END");
}




