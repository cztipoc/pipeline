/* cztmergescience.cpp

This module removes the czti data frames which are either corrupted or not having ex-
pected number of packets. Another major functionality of this module is merging science
data files of the same observation id. As mentioned earlier, data from each orbit is received
and it can have overlaps with adjacent orbit data. Such overlaps are removed and unique
good data frames are written out to merged science data file. Good Time Intervals where
each quadrant data was present is also written as extensions to the output science file


Mithun N P S, Rakesh Khanna
07/12/15

Edits (Mithun N P S):

23/02/16

* Various bug fixes

04/03/16

* Creation of new outputs on data integrity and saturation time fraction only for orbit wise 
  data

29/03/16

* Update to take care repeating command mode frames present in two different orbit files

01/04/16

* Memory leaks fixed

22/04/16

* Changed tstart,tstop keywords of primary header to min{tstart(quad)} and max{tstop{quad}}

19/05/16

* Provision to create saturation and integrity files for merged data
* Bug fix in last entry in GTI file

14/06/16

* File names of saturation and integrity now generated from output file name.
  In case of non-standard file names, these files will be created in pwd

*/

#include "cztmergescience.h"

//Constructor
cztmergescience::cztmergescience(){
	strcpy(modulename,"cztmergescience");
	badhduflag=0;
}

//Destructor
cztmergescience::~cztmergescience(){
}

int cztmergescience::read(int argc,char **argv)
{
    int status=0,ifile;
    char infile[PIL_LINESIZE];

    if (PIL_OK != (status = PILInit(argc, argv))) {
        LOG(ERROR) << "***Error Initializing PIL***";
        return status;
    	}

    if(PIL_OK!=(status=PILGetString("par_modename", modename))){
        LOG(ERROR)<<"***Error reading modename:"<<modename<<"***";
        return status;
    }
	
        if (PIL_OK != (status = PILGetInt("par_nscienceFiles", &nscienceFiles))) {
            LOG(ERROR) << "***Error Reading number of Input Science files***";
            return status;
        }

	if(nscienceFiles<1){
	
	    LOG(ERROR) << "***Invalid number of input files. It should be >=1";
	    return(EXIT_FAILURE);	

	}

    for(ifile=0; ifile<nscienceFiles; ifile++) {
        if (PIL_OK != (status = PILGetFname("par_infile", infile))) {
            LOG(ERROR) << "***Error Reading input science file " << ifile << " : " << infile << "***";
            return status;
        }
        scienceFiles.push_back((string)infile);
    }

    if (PIL_OK != (status = PILGetFname("par_tctfile", tctfile))) {
        LOG(ERROR) << "***Error reading merged tct file***";
        return status;
    }


    if (PIL_OK != (status = PILGetFname("par_mcapfile", mcapfile))) {
        LOG(ERROR) << "***Error reading mcap file***";
        return status;
    }

	
    if (PIL_OK != (status = PILGetFname("par_outfile", outfile))) {
        LOG(ERROR) << "***Error reading output file***";
        return status;
    }

    if (PIL_OK != (status = PILGetBool("clobber", &clobber))) {
        LOG(ERROR) << "***Error Reading clobber:" << clobber << "***";
        return status;
    }

    if (PIL_OK != (status = PILGetBool("history", &history))) {
        LOG(ERROR) << "***Error Reading history parameter" << history << "***";
        return status;
    }

    PILClose(status);

    return (EXIT_SUCCESS);	
	
}

int cztmergescience::read(char *modename, int nscienceFiles, vector <string> scienceFiles, char *tctfile , char *mcapfile,char *outfile,int clobber,int history)
{
	strcpy(this->modename,modename);
    	this->nscienceFiles=nscienceFiles;
	this->scienceFiles=scienceFiles;
    	strcpy(this->tctfile, tctfile);
    	strcpy(this->mcapfile, mcapfile);
    	strcpy(this->outfile, outfile);
	
	return (EXIT_SUCCESS);
}

int cztmergescience::cztmergescienceProcess()
{

	long last_sec_cnt[4],i=0;
	int QID=0,num_quads=4;
	
	fitsfile *fptr,*fout;
	FILE *fsat,*fintg;

	string fsatfilename;
	string fintgfilename;

    string str_outfile;
    str_outfile=(string)outfile;

	string out_path,in_path;

	in_path=scienceFiles[0];
	out_path=str_outfile;
    int fnam_char=0;

	while(strcmp(&out_path[out_path.size()-1],"/")!=0 && out_path.size()>1)
		out_path.erase(out_path.size()-1,1);

	while(strcmp(&in_path[in_path.size()-1],"/")!=0 && in_path.size()>1)
	{
		in_path.erase(in_path.size()-1,1);
		fnam_char++;
	}
	
	if(in_path.size()==1)
		fnam_char=scienceFiles[0].size();

	if(out_path.size()==1)
		out_path=string("./");
	
	fsatfilename.append(out_path);
	fsatfilename.append(scienceFiles[0],(scienceFiles[0].size()-fnam_char),fnam_char); 
	fsatfilename.erase(fsatfilename.size()-5,5);

	fintgfilename=fsatfilename;
    fsatfilename.append("_saturation_time.txt");
    fintgfilename.append("_integrity.txt");

    LOG(INFO)<<"Saturation file name "<<fsatfilename;
    LOG(INFO)<<"Integrity file name "<<fintgfilename;
    
	fintg=fopen(fintgfilename.c_str(),"w");
    fsat=fopen(fsatfilename.c_str(),"w");

/*
	if(str_outfile.compare(str_outfile.size()-5,5,".fits") ==0)
	{
		LOG(INFO)<<"This is merged output";
		orbit_or_merge=2;	
	}
	else if(str_outfile.compare(str_outfile.size()-10,10,".fits.temp") ==0)
	{
		LOG(INFO)<<"This is orbit-wise output";
		orbit_or_merge=1;
	}
	else
	{
		LOG(INFO)<<"****** Non-standard file name. Creating saturation and integrity files at present directory *******";	
		fsatfilename.append("./saturation_time.txt");
		fsat=fopen(fsatfilename.c_str(),"w");
		fintgfilename.append("./integrity.txt");	
		fintg=fopen(fintgfilename.c_str(),"w");
	}

	if(orbit_or_merge==1)
	{
        fsatfilename.append(str_outfile,0,str_outfile.size()-10);
        fsatfilename.append("_saturation_time.txt");
        LOG(INFO)<<"Saturation file name "<<fsatfilename;
        fsat=fopen(fsatfilename.c_str(),"w");

        fintgfilename.append(str_outfile,0,str_outfile.size()-10);
        fintgfilename.append("_integrity.txt");
        LOG(INFO)<<"Integrity file name "<<fintgfilename;
        fintg=fopen(fintgfilename.c_str(),"w");
	}
	else if(orbit_or_merge==2)
	{

		fsatfilename.append(str_outfile,0,str_outfile.size()-5);
		fsatfilename.append("_saturation_time.txt");
		LOG(INFO)<<"Saturation file name "<<fsatfilename;
		fsat=fopen(fsatfilename.c_str(),"w");

		fintgfilename.append(str_outfile,0,str_outfile.size()-5);
		fintgfilename.append("_integrity.txt");
		LOG(INFO)<<"Integrity file name "<<fintgfilename;
		fintg=fopen(fintgfilename.c_str(),"w");
	}
*/

	int hdunum,status=0,hdutype=0;
	double exposure_timeAll=0,dummy_exposure=0;
	char mode[30];
	int data_type=0;
	long n_rows=0;
	vector <GTIrecord> gti_final;
        vector <GTIrecord> gti_quad;

        if (clobber == YES) {
            if (FileExists(outfile)) {
            LOG(INFO) << outfile << "  :FileExists...Replacing the file. ";
            if (unlink(outfile) != 0) {
                LOG(ERROR) << "***Error in deleting " << outfile << "***";
                return (EXIT_FAILURE);
            }
            }
	}
	else{
	if (FileExists(outfile)){
            LOG(ERROR) << "***Output file already exists***";
            LOG(ERROR) << "Use clobber=yes for overwriting the file";
            return (EXIT_FAILURE);
	    }
        }

	//Set the header parameter defaults
	l1head.set_default_values();

	//Reading TCT file
	if (TCT.read_tct_file(tctfile)) 
	{LOG(ERROR) << "*** Error reading TCT file***"; }

	char infile[PIL_LINESIZE];

    	for(i=0;i<4;i++) last_sec_cnt[i]=0;

	// For each Quadrant and for each file remove the wrong packets and merge all correct files
	for(QID=0;QID<num_quads;QID++)
	{
		tstart.clear();
		tstop.clear();
	        prev_cztsec=0;
        	gtiflag=0;

		if(!strcmp("modeM9",modename) || !strcmp("modeSS",modename))
		{
			for(i=0;i<nscienceFiles;i++)
                	{
                	strcpy(infile,scienceFiles[i].c_str());
                	status=this->clean_l1_100sec_fits(infile,i,QID);
                	}

            //if(nscienceFiles==1)
            //{
                    fprintf(fintg,"%d\t%d\t%ld\t%ld\n",badhduflag,QID,totpkts,badpkts);
            //}

            if(status)
            {
				if(QID!=3)
				{
					totpkts=-1;
					badpkts=-1;
					for(i=QID+1;i<num_quads;i++)
						fprintf(fintg,"%d\t%d\t%ld\t%ld\n",badhduflag,i,totpkts,badpkts);
				}
                fclose(fintg);
                return(EXIT_FAILURE);
            }
		
		}


		if(!strcmp("modeM0",modename))
		{

			
			ntotal_sec=0;
			nsaturated_sec=0;

			for(i=0;i<nscienceFiles;i++)
	    		{
			strcpy(infile,scienceFiles[i].c_str());
			status=this->clean_l1_fits(infile,i,QID);
			LOG(INFO)<<"LAST CZTSEC "<<prev_cztsec;
			}

            //if(nscienceFiles==1)
            //{
	                fprintf(fintg,"%d\t%d\t%ld\t%ld\n",badhduflag,QID,totpkts,badpkts);
                    fprintf(fsat,"%d\t%ld\t%ld\t%lf\n",QID,ntotal_sec,nsaturated_sec,(nsaturated_sec/(double)ntotal_sec)*100);
            //}

			if(status)
			{
                if(QID!=3)
                {
                    totpkts=-1;
                    badpkts=-1;
                    for(i=QID+1;i<num_quads;i++)
                        fprintf(fintg,"%d\t%d\t%ld\t%ld\n",badhduflag,i,totpkts,badpkts);
                }
				
				fclose(fintg);
				return(EXIT_FAILURE);
			}
			//Print saturated seconds stats
			LOG(INFO)<<"Quadrant "<<QID<<" totsec "<<ntotal_sec<<" saturated sec "<<nsaturated_sec;
			LOG(INFO)<<"Quadrant "<<QID<<" Fraction of time Quadrant was saturated "<<(nsaturated_sec/(double)ntotal_sec)*100<<" %";
			
			//Interpolate TCT to get UT for GTI
			interpolateGtiTime();

			switch(QID)
			{
			        case 0:
				            gti_q0.set_GTI(tstart,tstop);
                            gti_q0.cleanup_GTI();
                            
				            break;
                	case 1:
                        	gti_q1.set_GTI(tstart,tstop);
                            gti_q1.cleanup_GTI();
                        	break;
                	case 2:
                        	gti_q2.set_GTI(tstart,tstop);
                            gti_q2.cleanup_GTI();
                        	break;
                	case 3:
                        	gti_q3.set_GTI(tstart,tstop);
                            gti_q3.cleanup_GTI();
                        	break;
			default:
				break;
			}
		}
	}

	if(!strcmp("modeM9",modename) || !strcmp("modeSS",modename))
        {
         	 // Add zero exposure time in Mode M9 and Mode SS 
              fits_open_file(&fout, outfile, READWRITE, &status);
              if(status) { fits_report_error(stderr,status);  return (EXIT_FAILURE); }

                        for(hdunum=2;hdunum<6;hdunum++)
                        {
                        fits_movabs_hdu(fout,hdunum,&hdutype,&status);
                        if(status) {fits_report_error(stderr,status); return(EXIT_FAILURE);}

                        fits_update_key(fout,TDOUBLE,"EXPOSURE",&dummy_exposure, NULL, &status);
                        if(status) {fits_report_error(stderr,status); return(EXIT_FAILURE);}
                        }

              fits_close_file(fout,&status);
              if(status){fits_report_error(stderr,status); return(EXIT_FAILURE);}
	}

	if(!strcmp("modeM0",modename))	 			
	{
	find_intersecting_range(gti_q0.get_GTI(), gti_q1.get_GTI(), &gti_final);
	find_intersecting_range(gti_final, gti_q2.get_GTI(), &gti_final);
	find_intersecting_range(gti_final, gti_q3.get_GTI(), &gti_final);
	//Create GTI extension 

        fits_open_file(&fptr,infile,READONLY,&status);
        if(status) {fits_report_error(stderr,status); return(EXIT_FAILURE);}
  
      	fits_movabs_hdu(fptr,6,&hdutype,&status);
        if(status) {fits_report_error(stderr,status); return(EXIT_FAILURE);}

        fits_open_file(&fout, outfile, READWRITE, &status);
        if(status) { fits_report_error(stderr,status);  return (EXIT_FAILURE); }

        fits_copy_header(fptr, fout,  &status);
        if(status) { fits_report_error(stderr,status);  return (EXIT_FAILURE); }

	    for(i=0;i<gti_final.size();i++)
	    {
	    fits_write_col(fout,TDOUBLE,1,i+1,1,1,&(gti_final[i].tstart),&status);	
        fits_write_col(fout,TDOUBLE,2,i+1,1,1,&(gti_final[i].tstop),&status);

	    exposure_timeAll+=(gti_final[i].tstop - gti_final[i].tstart);
        if(status) { fits_report_error(stderr,status);  return (EXIT_FAILURE); }
	    }

	    long ngti=gti_final.size();

        l1head.writeToHeader(fout);

        fits_update_key(fout, TLONG, "NAXIS2",&ngti, NULL, &status);
        if(status) {fits_report_error(stderr,status); return(EXIT_FAILURE);}

	    fits_update_key(fout,TDOUBLE,"EXPOSURE",&exposure_timeAll, NULL, &status);
        if(status) {fits_report_error(stderr,status); return(EXIT_FAILURE);}

    	l1head.writeTimekey(gti_final[0].tstart,gti_final[gti_final.size()-1].tstop,fout);
	
	
		double tstartmin=0,tstopmax=0;
	    // Create Quadrants wise GTI Extension	
		for(QID=0;QID<4;QID++)
		{
			switch(QID)
			{
					case 0:
							gti_quad=gti_q0.get_GTI();
							tstartmin=gti_quad[0].tstart;
							tstopmax=gti_quad[gti_quad.size()-1].tstop;
							break;
                	case 1:
                        	gti_quad=gti_q1.get_GTI();
                        	break;
                	case 2:
                        	gti_quad=gti_q2.get_GTI();
                        	break;
                	case 3:
                        	gti_quad=gti_q3.get_GTI();
                        	break;
					default:
							break;
			}
            
			if(gti_quad[0].tstart<tstartmin) tstartmin=gti_quad[0].tstart;
            if(gti_quad[gti_quad.size()-1].tstop>tstopmax) tstopmax=gti_quad[gti_quad.size()-1].tstop;
		
		create_GTIExtension(infile,outfile,gti_quad,QID);

		}
 
        // Update exposure time in primary header
        fits_movabs_hdu(fout,1,&hdutype,&status);
        if(status) {fits_report_error(stderr,status); return(EXIT_FAILURE);}

        fits_update_key(fout,TDOUBLE,"EXPOSURE",&exposure_timeAll, NULL, &status);
        if(status) {fits_report_error(stderr,status); return(EXIT_FAILURE);}

		//Update tstart,tstop etc with min{tstart} and max{tstop}
        l1head.writeTimekey(tstartmin,tstopmax,fout);

 		//if(nscienceFiles==1) 
		//{
			fclose(fsat);
			fclose(fintg);
		//}

	    // Close FITS files
	
        fits_close_file(fptr,&status);
        if(status){fits_report_error(stderr,status); return(EXIT_FAILURE);}

        fits_close_file(fout,&status);
        if(status){fits_report_error(stderr,status); return(EXIT_FAILURE);}

	}
	
	// Update all the modified keywords and checksum
	updateKeywords(outfile,modulename);

	return (EXIT_SUCCESS);

}


int cztmergescience::clean_l1_fits(char *infile,int file_num,int QID)
{

	fitsfile *fptr;
	int DecodingStatusFlag;
	int hdunum,status=0,hdutype=0;
	unsigned char packet[2048],boot_page;
	long row,n_rows;
	long mode_word;
	int MID,PNO,DID,MML,dcnt=0,cztluld=0;
	int num_quads=4,expected_pkts=0,actual_pkts=0,pkt_cntr=0;
	int i=0,j=0,pre_PNO=-1;
	long FNO=0,start_FNO=0;
	long sec_UW,sec_LW,sec_cnt,start_sec,sec_diff=0;
	long GTI_start=0,GTI_end=0,BTI_start=0,BTI_end=0;
	double GTI_startUT=0,GTI_endUT=0,BTI_startUT=0,BTI_endUT=0;
	long sync_UW, sync_LW,valid_words=0;
	int GT_flag=1,pre_GT_flag=1;
	int *error_flag,*pkt_num,*cztluld_count,*dcnt_count,*last_ntick;
	long *czt_sec;
	double UT=0;
	int start_flag=0,num_good_pkts=0,bunch_sync1=0,bunch_sync2=0,bunch_sync3=0;
	long cztseccnt;
	bool *febevtmode;
	int bunch_mode_flag=0;
	vector <int> frame_pkt_num;
	vector <long> frame_pkt_row;

	fits_open_file(&fptr,infile,READONLY,&status);
    	if(status) {fits_report_error(stderr,status); return(EXIT_FAILURE);}

	int  numOfHdus;
	fits_get_num_hdus(fptr,&numOfHdus, &status);
	if(status) {fits_report_error(stderr,status); return(EXIT_FAILURE);}

	// Check for the existence of all extensions
	if(numOfHdus<7)
	{
		if(file_num==0)
		{
			badhduflag=1;
			LOG(ERROR)<<"****The first file "<<infile<<" doesn't have all required extensions. Can't continue processing***ERROR***";
			return(EXIT_FAILURE);
		}
		
		LOG(WARNING)<<"This file "<<infile<<" doesn't have all required extensions. Skipping this file";
		return(0);
	}	

        hdunum=QID+2;

        fits_movabs_hdu(fptr,hdunum,&hdutype,&status);
        if(status) {fits_report_error(stderr,status); return(EXIT_FAILURE);}

        fits_get_num_rows(fptr,&n_rows,&status);
        if(status) {fits_report_error(stderr,status); return(EXIT_FAILURE);}

	if(file_num==0&&n_rows==0) 
	{
            badhduflag=1;
            LOG(ERROR)<<"****The first file "<<infile<<" has zero rows in Quad "<<QID<<" Can't continue processing***ERROR***";
            return(EXIT_FAILURE);			
	}

	LOG(INFO)<<"Quadrant"<<QID<<" Total Packets "<<n_rows;

	error_flag=(int*)malloc(sizeof(int)*(n_rows));
	czt_sec=(long*)malloc(sizeof(long)*(n_rows));
	pkt_num=(int*)malloc(sizeof(int)*(n_rows));
	cztluld_count=(int*)malloc(sizeof(int)*(n_rows));
    dcnt_count=(int*)malloc(sizeof(int)*(n_rows));
	febevtmode=(bool *)malloc(sizeof(bool)*(n_rows));
    last_ntick=(int*)malloc(sizeof(int)*(n_rows));

	for(i=0;i<n_rows;i++)
	{
	error_flag[i]=0;
	czt_sec[i]=0;
	pkt_num[i]=0;
	cztluld_count[i]=0;
    dcnt_count[i]=0;
    last_ntick[i]=0;
	}
		
   	row=0;
   	while (row < n_rows)
   	{

	    fits_read_col(fptr,TBYTE,4,row+1,1,2048,NULL,&packet,NULL,&status);
        if(status) {fits_report_error(stderr,status); return(EXIT_FAILURE);}

        fits_read_col(fptr,TINT,5,row+1,1,1,NULL,&DecodingStatusFlag,NULL,&status);
        if(status) {fits_report_error(stderr,status); return(EXIT_FAILURE);}


	    if(DecodingStatusFlag==0)
	    {
	    sync_UW=word(packet[0],packet[1]);		// SYNC Upper Word 
    	sync_LW=word(packet[2],packet[3]);		// SYNC Lower Word
	    mode_word=word(packet[4],packet[5]);
    	MID=mode_word & 0xF;            		// Mode ID
	    PNO=mode_word >> 4 & 0xF;     		// Packet Number
    	DID=mode_word >> 8 & 0x1F;    		// Data ID

	    pkt_num[row]=PNO;

	    if(PNO==0)
	    {
		boot_page=(word(packet[30],packet[31])>>14)&0x3;// Boot Page number	
		bunch_mode_flag=0;
	    sec_UW = word(packet[20],packet[21]);       // Second Count UW
        sec_LW = word(packet[22],packet[23]);       // Second Count LW
        sec_cnt = sec_LW + (65536*sec_UW);          // 32 Bit Seconds count
	    dcnt = word(packet[30],packet[31]) & 0x3FFF;//Double words of detector data read
        cztluld=word(packet[42],packet[43]); // CZT < ULD Counts

        long feb_status_word = word(packet[32],packet[33]);
	    febevtmode[row]=feb_status_word>>5 & 0x1;	

	    cztluld_count[row]=cztluld;
	    dcnt_count[row]=dcnt;
        czt_sec[row]=sec_cnt;	
		}
			    
        valid_words=word(packet[6],packet[7]);
    
		if(valid_words==1020)
		{
			bunch_sync1=word(packet[(valid_words+3-2)*2],packet[(valid_words+3-2)*2+1])>>13; //Get the bunch flag of last event
			bunch_sync2=word(packet[(valid_words+3-5)*2],packet[(valid_words+3-5)*2+1])>>13; //Get the bunch flag of second last event
			bunch_sync3=word(packet[(valid_words+3-8)*2],packet[(valid_words+3-8)*2+1])>>13; //Get the bunch flag of third last event
			if(bunch_sync1!=7&&bunch_sync2!=7&&bunch_sync3!=7)
			{
				bunch_mode_flag=0;
				last_ntick[row]=word(packet[(valid_words+3-2)*2],packet[(valid_words+3-2)*2+1]);
			}
			else if(bunch_sync1==7)
			{
				bunch_mode_flag=1;
				last_ntick[row]=((word(packet[(valid_words+3-2)*2],packet[(valid_words+3-2)*2+1])>>1) & 0x3f); //Assign the diff as the last_ntick
			}
			else if(bunch_sync2==7)	
			{
				bunch_mode_flag=2;
				last_ntick[row]=word(packet[(valid_words+3-2)*2],packet[(valid_words+3-2)*2+1])+((word(packet[(valid_words+3-5)*2],packet[(valid_words+3-5)*2+1])>>1) & 0x3f);
			}
			else if(bunch_sync3==7)
			{
				bunch_mode_flag=0;
				last_ntick[row]=word(packet[(valid_words+3-5)*2],packet[(valid_words+3-5)*2+1])+((word(packet[(valid_words+3-8)*2],packet[(valid_words+3-8)*2+1])>>1) & 0x3f);	
			}

		}
		else if (valid_words>=9)
		{
			bunch_sync3=word(packet[(valid_words+3-8)*2],packet[(valid_words+3-8)*2+1])>>13;
			if(bunch_sync3!=7)
				last_ntick[row]=word(packet[(valid_words+3-2)*2],packet[(valid_words+3-2)*2+1]);
			else
				last_ntick[row]=word(packet[(valid_words+3-5)*2],packet[(valid_words+3-5)*2+1])+((word(packet[(valid_words+3-8)*2],packet[(valid_words+3-8)*2+1])>>1) & 0x3f);
			
		}
		else if (valid_words<9)
		{
			if(bunch_mode_flag==1&&valid_words==6)
			{
				last_ntick[row]=last_ntick[row-1]+word(packet[(valid_words+3-5)*2],packet[(valid_words+3-5)*2+1]);
			}
			else if(bunch_mode_flag==2&&valid_words==3)
			{
				last_ntick[row]=last_ntick[row-1];
			}
			else
			{
				last_ntick[row]=word(packet[(valid_words+3-2)*2],packet[(valid_words+3-2)*2+1]);	
			}
		}

	    }	
	    
	    if(DecodingStatusFlag!=0 || sync_UW!=0xF9A4 || sync_LW!=0x2BB1 || QID!=DID)
	    	error_flag[row]=1;

    //printf("%ld\t%d\n",czt_sec[row],pkt_num[row]);
	row++;

	}

	LOG(INFO)<<"Done reading frame and packet headers";
        fits_close_file(fptr,&status);
        if(status){fits_report_error(stderr,status); return(EXIT_FAILURE);}


	row=0;
   	while (row < n_rows)
   	{
		if(pkt_num[row]==0 && error_flag[row]==0 && febevtmode[row]==1)
		{
			cztseccnt=czt_sec[row];	
			//Calculate num of expected pkts based on dcnt count
			
            /*if(dcnt_count[row]*2<=1024)
				expected_pkts=1;
			else
				expected_pkts=ceil(((dcnt_count[row]*2-1024)/1020.))+1;
            */

            //Calculate num of expected pkts based on czt<uld counts
            if(cztluld_count[row]>3316) cztluld_count[row]=3315;

            if(cztluld_count[row]<=256)
                expected_pkts=1;
            else
                expected_pkts=ceil(((cztluld_count[row]-256)*3)/1020.)+1;   

			frame_pkt_num.clear();
			frame_pkt_row.clear();
			
			//Keep all packets till next zero packet in vectors including zero
			frame_pkt_num.push_back(pkt_num[row]);
			frame_pkt_row.push_back(row);
			i=row+1;			
			while(i<n_rows && pkt_num[i]!=0)
			{
				if(error_flag[i]==0)
				{
					frame_pkt_num.push_back(pkt_num[i]);
					frame_pkt_row.push_back(i);	
				}
			
				i++;
			}

			actual_pkts=frame_pkt_num.size();

            //printf("%ld\t%d\t%d\t%d\n",czt_sec[row],expected_pkts,actual_pkts,cztluld_count[row])   ;

			// Check whether the packet number increments 
			int k=0,diff=0;
			for(k=1;k<actual_pkts;k++)
			{
				diff =frame_pkt_num[k]-frame_pkt_num[k-1];
				if(diff!=1)
				{
				k-=1;
				break;
				}
			}

			num_good_pkts=k;


			for(int r=num_good_pkts;r<actual_pkts;r++)
				error_flag[frame_pkt_row[r]]=1;

                        sec_diff = (czt_sec[row] - prev_cztsec);

			//fprintf(f1,"Q%d\t%ld\t%ld\n",QID,czt_sec[row],sec_diff);

			if(num_good_pkts==expected_pkts) //Potentially good frames
			{
				
				if(sec_diff<1) //Repeating frame, hence bad
				{
					if(gtiflag==1)
					{
						tstop.push_back(prev_cztsec+1);
						gtiflag=0;
					}

					//Flag all packets in the frame as bad
					for(int r=0;r<num_good_pkts;r++)
						error_flag[frame_pkt_row[r]]=1;

				}
				else if(sec_diff==1)
				{
					if(gtiflag==0)
					{
						tstart.push_back(czt_sec[row]);
						gtiflag=1;
					}
					prev_cztsec=czt_sec[row];
            		ntotal_sec++;
			            
					if(dcnt_count[row]==4736) nsaturated_sec++;
							

					//Every 100s ignore first 400ms
                    if(czt_sec[row]%100==0)
                    {
					    if(gtiflag==1){
                            tstop.push_back(czt_sec[row]);
                            gtiflag=0;
					    if((dcnt_count[row]==4736&&last_ntick[row+expected_pkts-1]*2e-5>0.400)||dcnt_count[row]<4736)
				                {
					        tstart.push_back(czt_sec[row]+0.400);
                            	gtiflag=1;
					            }       
					    }
                    }
  					
					if(dcnt_count[row]==4736)// Hit max limit of FEB
                    {
                        //LOG(INFO)<<row<<" "<<czt_sec[row]<<" "<<expected_pkts<<" "<<cztluld_count[row];

					    if(gtiflag==1)
						{
                            tstop.push_back(czt_sec[row]+last_ntick[row+expected_pkts-1]*2e-5); //Last event time gti end
							//LOG(INFO)<<"Start stop "<<tstart[tstart.size()-1]<<" "<<tstop[tstart.size()-1];
							//printf("Start stop %lf\t%lf\n",tstart[tstart.size()-1],tstop[tstart.size()-1]);
                            gtiflag=0;
						}

                    }


				}
				else
				{
					if(gtiflag==1)
					{
						tstop.push_back(prev_cztsec+1);
					}

					tstart.push_back(czt_sec[row]);
					gtiflag=1;
					
					prev_cztsec=czt_sec[row];
                    ntotal_sec++;
					
					if(dcnt_count[row]==4736) nsaturated_sec++;
										                                        
					//Every 100s ignore first 400ms 
                    if(czt_sec[row]%100==0)
                    {
                            if(gtiflag==1){
                            tstop.push_back(czt_sec[row]);
                            gtiflag=0;
                            if((dcnt_count[row]==4736&&last_ntick[row+expected_pkts-1]*2e-5>0.400)||dcnt_count[row]<4736)
                                    {
                                    tstart.push_back(czt_sec[row]+0.400);
                                    gtiflag=1;
                                    }
                            }
                    }

                    if(dcnt_count[row]==4736)// Hit max limit of FEB
                    {
                            //LOG(INFO)<<row<<" "<<czt_sec[row]<<" "<<expected_pkts<<" "<<cztluld_count[row];

                            if(gtiflag==1)
                            {
                            tstop.push_back(czt_sec[row]+last_ntick[row+expected_pkts-1]*2e-5); //Last event time gti end
                            gtiflag=0;
                            }
                    }
					
                    
		
				}

			}
			else //Definitely bad frame
			{	
				//Flag all packets in frame as bad
				 for(int r=0;r<num_good_pkts;r++)
                                 	error_flag[frame_pkt_row[r]]=1;

				if(gtiflag==1)
				{
					if(sec_diff<=1)
						tstop.push_back(czt_sec[row]);
					else
						tstop.push_back(prev_cztsec+1);
					gtiflag=0;
				}
			}

			row=i; //set row to the next packet zero 

		}
        else if (pkt_num[row]==0 && error_flag[row]==0 && febevtmode[row]==0) //FEB in command mode
        {
	    sec_diff = (czt_sec[row] - prev_cztsec);

	    if(sec_diff<1)
	         error_flag[row]=1;
	    else
		 prev_cztsec=czt_sec[row];
	    
            if(gtiflag==1)
            {
                tstop.push_back(czt_sec[row]);
                gtiflag=0;
            }
		row++;
        }
		else// If packet number is not zero or error_flag is set
		{
			error_flag[row]=1;
			row++;
		}
		
	}	

	if(gtiflag==1)
	{
		tstop.push_back(prev_cztsec+1);
		gtiflag=0;
	}	

	this->copy_l1_fits(infile,error_flag,QID,file_num);


    free(error_flag);
    free(czt_sec);
    free(pkt_num);
    free(cztluld_count);
    free(dcnt_count);
    free(febevtmode);
    free(last_ntick);
	
	return(EXIT_SUCCESS);
}


int cztmergescience::clean_l1_100sec_fits(char *infile,int file_num,int QID)
{

	fitsfile *fptr;
	int DecodingStatusFlag;
	int hdunum,status=0,hdutype=0;
	unsigned char packet[2048];
	long row,n_rows;
	long mode_word;
	int MID,PNO,DID,MML,dcnt=0;
	int num_quads=4,expected_pkts=0,actual_pkts=0,pkt_cntr=0;
	int i=0,j=0,pre_PNO=-1;
	long FNO=0,start_FNO=0;
	long sec_UW,sec_LW,sec_cnt,start_sec,sec_diff=0;
	long GTI_start=0,GTI_end=0,BTI_start=0,BTI_end=0;
	double GTI_startUT=0,GTI_endUT=0,BTI_startUT=0,BTI_endUT=0;
	long sync_UW, sync_LW,valid_words=0;
	int GT_flag=1,pre_GT_flag=1;
	int *error_flag;
	double UT=0;
	int start_flag=0,num_good_pkts=0;
	long cztseccnt;
	bool *febevtmode;
	vector <int> frame_pkt_num;
	vector <long> frame_pkt_row;

	fits_open_file(&fptr,infile,READONLY,&status);
    	if(status) {fits_report_error(stderr,status); return(EXIT_FAILURE);}
	
	int  numOfHdus;
	fits_get_num_hdus(fptr,&numOfHdus, &status);
	if(status) {fits_report_error(stderr,status); return(EXIT_FAILURE);}

	//Check if all extensions are present
	if(numOfHdus<5)
	{
		if(file_num==0)
		{
			LOG(ERROR)<<"The first file "<<infile<<" doesn't have all required extensions. Can't continue processing";
			badhduflag=1;	
			return(EXIT_FAILURE);
		}
		
		LOG(WARNING)<<"This file "<<infile<<" doesn't have all required extensions. Skipping this file";
		return(0);
	}	
        hdunum=QID+2;

        fits_movabs_hdu(fptr,hdunum,&hdutype,&status);
        if(status) {fits_report_error(stderr,status); return(EXIT_FAILURE);}

        fits_get_num_rows(fptr,&n_rows,&status);
        if(status) {fits_report_error(stderr,status); return(EXIT_FAILURE);}

    if(file_num==0&&n_rows==0)
    {
            badhduflag=1;
            LOG(ERROR)<<"****The first file "<<infile<<" has zero rows in Quad "<<QID<<" Can't continue processing***ERROR***";
            return(EXIT_FAILURE);
    }

	LOG(INFO)<<"Quadrant  :"<<QID<<"\tTotal Packets "<<n_rows;

	error_flag=(int*)malloc(sizeof(int)*(n_rows));

	for(i=0;i<n_rows;i++)
	{
	error_flag[i]=0;
	}
		
   	row=0;
   	while (row < n_rows)
   	{
	    fits_read_col(fptr,TBYTE,4,row+1,1,2048,NULL,&packet,NULL,&status);
            if(status) {fits_report_error(stderr,status); return(EXIT_FAILURE);}

	    sync_UW=word(packet[0],packet[1]);		// SYNC Upper Word 
    	    sync_LW=word(packet[2],packet[3]);		// SYNC Lower Word
	    mode_word=word(packet[4],packet[5]);
    	    MID=mode_word & 0xF;            		// Mode ID
	    PNO=mode_word >> 4 & 0xF;     		// Packet Number
    	    DID=mode_word >> 8 & 0x1F;    		// Data ID

	    if(PNO==0)
	    {
	    sec_UW = word(packet[20],packet[21]);       // Second Count UW
            sec_LW = word(packet[22],packet[23]);       // Second Count LW
            sec_cnt = sec_LW + (65536*sec_UW);          // 32 Bit Seconds count

            fits_read_col(fptr,TINT,5,row+1,1,1,NULL,&DecodingStatusFlag,NULL,&status);
            if(status) {fits_report_error(stderr,status); return(EXIT_FAILURE);}


            if(DecodingStatusFlag!=0 || sync_UW!=0xF9A4 || sync_LW!=0x2BB1 || QID!=DID%4)
            {
                error_flag[row]=1;
            }
  	    else if(sec_cnt-prev_cztsec<100)
            {
                error_flag[row]=1;
            }
 	    else if (sec_cnt-prev_cztsec>=100)
            {
                prev_cztsec=sec_cnt;

           
	    	if(DID>=8 && DID<=11)//Read packet 1 if it is modeSS (SSM Mode)
            	{
            	fits_read_col(fptr,TBYTE,4,row+2,1,2048,NULL,&packet,NULL,&status);
            	if(status) {fits_report_error(stderr,status); return(EXIT_FAILURE);}

            		sync_UW=word(packet[0],packet[1]);          // SYNC Upper Word 
            		sync_LW=word(packet[2],packet[3]);          // SYNC Lower Word
            		mode_word=word(packet[4],packet[5]);
            		MID=mode_word & 0xF;                        // Mode ID
            		PNO=mode_word >> 4 & 0xF;                   // Packet Number
            		DID=mode_word >> 8 & 0x1F;                  // Data ID


            	fits_read_col(fptr,TINT,5,row+2,1,1,NULL,&DecodingStatusFlag,NULL,&status);
            	if(status) {fits_report_error(stderr,status); return(EXIT_FAILURE);}

                	if(PNO==1)
                    	{
                        	if(DecodingStatusFlag!=0 || sync_UW!=0xF9A4 || sync_LW!=0x2BB1 || QID!=DID%4)
                         	{
                         	error_flag[row]=1;
                         	error_flag[row+1]=1;
                         	}
                    	row++;
                    	}

            	}

	    }
	    }
	    else	// If Other than Packet 0  comes
	    error_flag[row]=1;
	row++;

        }

	
	fits_close_file(fptr,&status);
        if(status){fits_report_error(stderr,status); return(EXIT_FAILURE);}

	this->copy_l1_fits(infile,error_flag,QID,file_num);
	return(EXIT_SUCCESS);
}

// Function to merge the files
int cztmergescience::copy_l1_fits(char *infile,int *error_flag,int QID,int file_num)
{
	int hdutype=0,naxis1=0,status=0;
	long i=0,n_rows=0,naxis2=0;
	char *date_end,*time_end;
	long tstopf=0,tstopi=0,mjdreff=0,mjdrefi=55197;
	string timesys="UTC";
	string radecsys="ICRS";	
	long tstarti=0,tstartf=0;
	double tstart=0,tstop=0;
	char str_tstartf[50], str_tstopf[50];
	float ra_obj=0,dec_obj=0;
	string obs_mode;
	long start_orbNum=0,end_orbNum=0;
	char detnam[10];
	long good_pkt_cntr=0;
	
	LOG(INFO)<<"Start copying quad "<<QID<<" file "<<file_num;

	fitsfile *fptr,*fout;
    	fits_open_file(&fptr,infile,READONLY,&status);
    	if(status) {fits_report_error(stderr,status); return(EXIT_FAILURE);}

	if(file_num==0 && QID==0)// If file num is 0 create a new output file
	{
	    	fits_create_file(&fout,outfile,&status);
	    	if(status) { fits_report_error(stderr,status);  return (EXIT_FAILURE); }

		// Copy primary header for the first time
		fits_copy_header(fptr, fout,  &status);
		if(status) { fits_report_error(stderr,status);  return (EXIT_FAILURE); }

		//Update Timedel
        
		double timedel=0.000020,clock_correction_factor;

        TCT.get_clockCorrectionFactor(clock_correction_factor);
    
        timedel*=clock_correction_factor;

		//printf("Timedel is %24.20lf\n",timedel);

        fits_update_key(fout,TDOUBLE,"TIMEDEL",&timedel,"Time resolution",&status);
		if(status) {fits_report_error(stderr,status);return(EXIT_FAILURE);}


        // Update Start Orbit number
        fits_read_key(fptr,TLONG,"ORB_NUM",&start_orbNum,NULL,&status);
		if(status) {fits_report_error(stderr,status);return(EXIT_FAILURE);}

		fits_update_key(fout,TLONG,"STARTORB",&start_orbNum,"Start Orbit number",&status);
		if(status) {fits_report_error(stderr,status);return(EXIT_FAILURE);}	

	    // MJDREFI
        fits_update_key(fout, TLONG, "MJDREFI",&mjdrefi, NULL, &status);
        if(status) {fits_report_error(stderr,status); return(EXIT_FAILURE);}

		// MJDREFF
		fits_update_key(fout, TLONG, "MJDREFF",&mjdreff, NULL, &status);
        if(status) {fits_report_error(stderr,status); return(EXIT_FAILURE);}

		//TIMESYS
		fits_update_key(fout, TSTRING, "TIMESYS",(char *)timesys.c_str(), NULL, &status);
        if(status) {fits_report_error(stderr,status); return(EXIT_FAILURE);}

		//RADECSYS
		fits_update_key(fout, TSTRING, "RADECSYS",(char *)radecsys.c_str(), NULL, &status);
        if(status) {fits_report_error(stderr,status); return(EXIT_FAILURE);}

/*
		// Read TSTARTI
		fits_read_key(fout,TLONG,"TSTARTI",&tstarti,NULL, &status);
		if(status) {fits_report_error(stderr,status); return(EXIT_FAILURE);}

		// Read TSTARTF
		fits_read_key(fout,TLONG,"TSTARTF",&tstartf,NULL, &status);
		if(status) {fits_report_error(stderr,status); return(EXIT_FAILURE);}

		// Write TSTART
		sprintf(str_tstartf,"%ld",tstartf);
		tstart=tstarti+((tstartf*1.0)/pow(10,strlen(str_tstartf)));
*/
		tstart=0;
		fits_update_key(fout, TDOUBLE, "TSTART",&tstart, NULL, &status);
        if(status) {fits_report_error(stderr,status); return(EXIT_FAILURE);}
	
		this->readFromMcap(&ra_obj,&dec_obj,&obs_mode);

		//RA_OBJ from xml
		fits_update_key(fout, TFLOAT, "RA_OBJ",&ra_obj, NULL, &status);
        if(status) {fits_report_error(stderr,status); return(EXIT_FAILURE);}

		//DEC_OBJ from xml
		fits_update_key(fout, TFLOAT, "DEC_OBJ",&dec_obj, NULL, &status);
        if(status) {fits_report_error(stderr,status); return(EXIT_FAILURE);}

		//OBS_MODE from xml
		fits_update_key(fout, TSTRING, "OBS_MODE",(char *)obs_mode.c_str(), NULL, &status);
        if(status) {fits_report_error(stderr,status); return(EXIT_FAILURE);}

	}
    	else // else open the file 
    	{
		fits_open_file(&fout, outfile, READWRITE, &status);
		if(status) { fits_report_error(stderr,status);  return (EXIT_FAILURE); }
		
		// Adding few primary header parameters	only once 	
		if(QID==0)
		{
		// Date End 
		fits_read_key(fptr,TSTRING,"DATE-END",&date_end,NULL, &status);
		if(status) {fits_report_error(stderr,status); return(EXIT_FAILURE);}

		fits_update_key(fout, TSTRING, "DATE-END",&date_end, NULL, &status);
		if(status) {fits_report_error(stderr,status); return(EXIT_FAILURE);}
	
		// Time End 
		fits_read_key(fptr,TSTRING,"TIME-END",&time_end,NULL, &status);
		if(status) {fits_report_error(stderr,status); return(EXIT_FAILURE);}

		fits_update_key(fout, TSTRING, "TIME-END",&time_end, NULL, &status);
		if(status) {fits_report_error(stderr,status); return(EXIT_FAILURE);}
		
		}

   	}


	if(QID==0)
	{

/*
		// TSTOP I 
		fits_read_key(fptr,TLONG,"TSTOPI",&tstopi,NULL, &status);
		if(status) {fits_report_error(stderr,status); return(EXIT_FAILURE);}

		fits_update_key(fout, TLONG, "TSTOPI",&tstopi, NULL, &status);
		if(status) {fits_report_error(stderr,status); return(EXIT_FAILURE);}
	
		// TSTOP F 
		fits_read_key(fptr,TLONG,"TSTOPF",&tstopf,NULL, &status);
		if(status) {fits_report_error(stderr,status); return(EXIT_FAILURE);}

		fits_update_key(fout, TLONG, "TSTOPF",&tstopf, NULL, &status);
		if(status) {fits_report_error(stderr,status); return(EXIT_FAILURE);}// Write TSTOP
		sprintf(str_tstopf,"%ld",tstopf);
		tstop=tstopi+((tstopf*1.0)/pow(10,strlen(str_tstopf)));
*/

		tstop=0;
		fits_update_key(fout, TDOUBLE, "TSTOP",&tstop, NULL, &status);
        if(status) {fits_report_error(stderr,status); return(EXIT_FAILURE);}

		double telapse=tstop-tstart;

        fits_update_key(fout, TDOUBLE, "TELAPSE",&telapse, "Elapsed time", &status);
        if (status) {
               LOG(ERROR) <<"Error in updating key Telapse";
                  fits_report_error(stderr, status);
        }
	
		// Update End Orbit Number
                fits_read_key(fptr,TLONG,"ORB_NUM",&end_orbNum,NULL,&status);
                if(status) {fits_report_error(stderr,status);return(EXIT_FAILURE);}

                fits_update_key(fout,TLONG,"ENDORB",&end_orbNum,"End Orbit number",&status);
                if(status) {fits_report_error(stderr,status);return(EXIT_FAILURE);}

	}

	//Read all necessary keywords from primary header
	l1head.readFromHeader(fout);
    l1head.writeToHeader(fout);

	fits_movabs_hdu(fptr, QID+2, &hdutype, &status);
    if(status) { fits_report_error(stderr,status);  return (EXIT_FAILURE); }

	// Copy the Quadrant header for first time	
	if(file_num==0)
	{
		fits_copy_header(fptr, fout,  &status);
		if(status) { fits_report_error(stderr,status);  return (EXIT_FAILURE); }
	}
	else
	{
		fits_movabs_hdu(fout, QID+2, &hdutype, &status);
		if(status) { fits_report_error(stderr,status);  return (EXIT_FAILURE); }
	
		fits_read_key(fout,TLONG,"NAXIS2",&naxis2,NULL, &status);//get number of rows
		if(status) { fits_report_error(stderr,status);  return (EXIT_FAILURE); }		
	}

	//Write header parameters to the header of quadrant extension
	l1head.writeToHeader(fout);

	fits_read_key(fptr,TINT,"NAXIS1",&naxis1,NULL, &status);
	if(status) {fits_report_error(stderr,status); return(EXIT_FAILURE);}

	unsigned char *tblrow;
	tblrow=(unsigned char*)malloc(sizeof(unsigned char)*naxis1);

	fits_get_num_rows(fptr,&n_rows,&status);
        if(status) {fits_report_error(stderr,status); return(EXIT_FAILURE);}

	// Copy data to output file
	good_pkt_cntr=0;
	for(i=0;i<n_rows;i++)
	{
	
		if(error_flag[i]==0)
		{		
		fits_read_tblbytes(fptr, i+1, 1,naxis1, tblrow, &status);
		fits_write_tblbytes (fout, naxis2+1, 1,naxis1, tblrow, &status);
		naxis2++;
		good_pkt_cntr++;	
		}
	}

	//Free allocated memory
	free(tblrow);

	// Update Quadrant ID in header
	sprintf(detnam,"QUADRANT%d",QID);
	
	fits_update_key(fout,TSTRING,"DETNAM",detnam,"Quadrant number",&status);
	if(status) {fits_report_error(stderr,status); return(EXIT_FAILURE);}

	fits_update_key(fout, TLONG, "NAXIS2",&naxis2, NULL, &status);
	if(status) {fits_report_error(stderr,status); return(EXIT_FAILURE);}

	fits_close_file(fptr,&status);
	if(status){fits_report_error(stderr,status); return(EXIT_FAILURE);}

	fits_close_file(fout,&status);
	if(status){fits_report_error(stderr,status); return(EXIT_FAILURE);}
	
	LOG(INFO)<<"Copied data for quad "<<QID<<" file "<<file_num;
	LOG(INFO)<<"Quadrant  :"<<QID<<"\tDiscarded packets "<<n_rows-good_pkt_cntr;
	badpkts=n_rows-good_pkt_cntr;
	totpkts=n_rows;

	if(file_num!=0)
	{
		totpkts=naxis2;
		badpkts=0;
	}
	return 0;

}

int cztmergescience::readFromMcap(float *ra_obj,float *dec_obj, string *obs_mode)
{
	ifstream file(mcapfile);

	string str;
	string file_contents;


	while(getline(file,str))
	{
	file_contents+=str;
	}


	size_t pos1 = file_contents.find("<ra_deg>");
	size_t pos2 = file_contents.find("</ra_deg>");
	
	string radeg;
	
	if (pos1!=string::npos && pos2!=string::npos)
	radeg = file_contents.substr(pos1+(strlen("<ra_deg>")),pos2-pos1-strlen("<ra_deg>"));
	
	sscanf(radeg.c_str(),"%f",ra_obj);

        pos1 = file_contents.find("<dec_deg>");
        pos2 = file_contents.find("</dec_deg>");
       
	string decdeg;
        
	if (pos1!=string::npos && pos2!=string::npos)
        decdeg = file_contents.substr(pos1+(strlen("<dec_deg>")),pos2-pos1-strlen("<dec_deg>"));

        sscanf(decdeg.c_str(),"%f",dec_obj);
        
	pos1 = file_contents.find("<scanrate>");
        pos2 = file_contents.find("</scanrate>");
        
	if (pos1!=string::npos && pos2!=string::npos)
        *obs_mode= file_contents.substr(pos1+(strlen("<scanrate>")),pos2-pos1-strlen("<scanrate>"));
	else
	obs_mode->assign("UNKNOWN");
        
	pos1 = file_contents.find("<observation_id>");
        pos2 = file_contents.find("</observation_id>");
        
	string observation_id;

        if (pos1!=string::npos && pos2!=string::npos)
        observation_id = file_contents.substr(pos1+(strlen("<observation_id>")),4);
	else
	obs_mode->assign("UNKNOWN");

	int obs_modeValue;

        sscanf(obs_mode->c_str(),"%d",&obs_modeValue);

	if(!strcmp(observation_id.c_str(),"SLEW"))
	{
	obs_mode->assign("SLEW");
	}
	else
	{
		 if(obs_modeValue>0)
		 obs_mode->assign("SCAN");
		 else
	 	 obs_mode->assign("POINTING");
	}

}


int cztmergescience::interpolateGtiTime()
{
	if(tstart.size()!=tstop.size())
	{
		LOG(ERROR)<<"Tstart and tstop are not of same size";
		return(EXIT_FAILURE);
	}


	long nrows=tstart.size(),i;
	vector <double> UTstart, UTstop;

	for(i=0;i<nrows;i++)

	{
		double tstart_UT;
		double tstop_UT;

		if(TCT.interpolate_time((double)tstart[i], tstart_UT)||TCT.interpolate_time((double)tstop[i], tstop_UT))
		{
                	LOG(ERROR) << "Error in TCT time interpolation.";
                        return EXIT_FAILURE;
	        }

		UTstart.push_back(tstart_UT);
		UTstop.push_back(tstop_UT);
	}

	tstart.clear();
	tstop.clear();	

	tstart=UTstart;
	tstop=UTstop;

	return(EXIT_SUCCESS);
}


int cztmergescience::create_GTIExtension(char *infile,char *outfile,vector <GTIrecord> gti_quad,int QID)
{

int hdunum=0,hdutype=0;
int i=0,status=0;
double exposure_time=0;
fitsfile *fptr,*fout;
char extname[20];

	fits_open_file(&fptr,infile,READONLY,&status);
        if(status) {fits_report_error(stderr,status); return(EXIT_FAILURE);}

        fits_open_file(&fout, outfile, READWRITE, &status);
        if(status) { fits_report_error(stderr,status);  return (EXIT_FAILURE); }

	hdunum=6;	
	

	fits_movabs_hdu(fptr,hdunum,&hdutype,&status);
        if(status) {fits_report_error(stderr,status); return(EXIT_FAILURE);}

        fits_copy_header(fptr, fout, &status);
        if(status) { fits_report_error(stderr,status);  return (EXIT_FAILURE); }

	for(i=0;i<gti_quad.size();i++)
	{
	fits_write_col(fout,TDOUBLE,1,i+1,1,1,&(gti_quad[i].tstart),&status);	

        fits_write_col(fout,TDOUBLE,2,i+1,1,1,&(gti_quad[i].tstop),&status);
        if(status) { fits_report_error(stderr,status);  return (EXIT_FAILURE); }
	exposure_time+=(gti_quad[i].tstop - gti_quad[i].tstart);
        if(status) { fits_report_error(stderr,status);  return (EXIT_FAILURE); }
	}

	long ngti=gti_quad.size();
	sprintf(extname,"Q%d_GTI",QID);

	fits_update_key(fout,TSTRING,"EXTNAME",extname, NULL, &status);
        if(status) {fits_report_error(stderr,status); return(EXIT_FAILURE);}

        fits_update_key(fout, TLONG, "NAXIS2",&ngti, NULL, &status);
        if(status) {fits_report_error(stderr,status); return(EXIT_FAILURE);}

    l1head.writeToHeader(fout);

    	fits_update_key(fout,TDOUBLE,"EXPOSURE",&exposure_time, NULL, &status);
        if(status) {fits_report_error(stderr,status); return(EXIT_FAILURE);}
	
	l1head.writeTimekey(gti_quad[0].tstart,gti_quad[gti_quad.size()-1].tstop,fout);

	hdunum = QID+2;

	fits_movabs_hdu(fout,hdunum,&hdutype,&status);
        if(status) {fits_report_error(stderr,status); return(EXIT_FAILURE);}

        fits_update_key(fout,TDOUBLE,"EXPOSURE",&exposure_time, NULL, &status);
        if(status) {fits_report_error(stderr,status); return(EXIT_FAILURE);}
	
	l1head.writeTimekey(gti_quad[0].tstart,gti_quad[gti_quad.size()-1].tstop,fout);

	fits_close_file(fptr,&status);
        if(status){fits_report_error(stderr,status); return(EXIT_FAILURE);}

        fits_close_file(fout,&status);
        if(status){fits_report_error(stderr,status); return(EXIT_FAILURE);}
}

