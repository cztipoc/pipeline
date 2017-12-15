#ifndef CZTMERGESCIENCE_H
#define CZTMERGESCIENCE_H

#include<level1handler.h>
#include<l1evtdecode.h>
#include<cztHeaderParam.h>
#include "gtiHandler.h"
#include<stdio.h>
#include<stdlib.h>
#include<fitsio.h>
#include<string.h>
#include<math.h>
#include<vector>
#include<fstream>
#define BUFF_SIZE 1024
#define word(x,y) (256*x+y)

class cztmergescience{
private:
	char modulename[NAMESIZE];
	char modename[PIL_LINESIZE];
    	int nscienceFiles;
	vector <string> scienceFiles;
	char tctfile[PIL_LINESIZE];
	char mcapfile[PIL_LINESIZE];
    	char outfile[PIL_LINESIZE];
	int clobber;                      
    	int history;

	Tct TCT;

	long prev_cztsec;
	int gtiflag;
	int badhduflag;
	long ntotal_sec;
	long nsaturated_sec;
	long badpkts;
	long totpkts;

	cztHeaderParam l1head;

	vector <double> tstart;
	vector <double> tstop;

	GTIhandler gti_q0;
	GTIhandler gti_q1;
	GTIhandler gti_q2;
	GTIhandler gti_q3;

public:
	cztmergescience();
	~cztmergescience();
	int cztmergescienceProcess();
	int read(int argc,char **argv);
	int read(char *modename, int nscienceFiles, vector <string> scienceFiles, char *tctfile , char *mcapfile, char *outfile, int clobber,int history);	
	int clean_l1_fits(char *infile,int file_num,int QID);
	int clean_l1_100sec_fits(char *infile,int file_num,int QID);
	int copy_l1_fits(char *infile,int *pkt_index,int QID,int file_num);
	int readFromMcap(float *ra_obj, float *dec_obj,string *obs_mode);
	int create_GTIExtension(char *infile,char *outfile,vector <GTIrecord> gti_quad,int QID);
	int interpolateGtiTime();

};



#endif

