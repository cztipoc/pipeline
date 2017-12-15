/*
 * DataQualityCheck.h
 *
 *  Created on: Oct 10, 2015
 *  Author: Ajay Vibhute
 */

#ifndef DATAQUALITYCHECK_H_
#define DATAQUALITYCHECK_H_
#define BUFFSIZE 1024
#define PACKETSIZE 2048
#define NUMHKPARAM 8
#define CF 409.6
void toWords(unsigned char *data,long size,unsigned short *word_data,int bigendian);
int getPackets(fitsfile *fptr,unsigned char *pktarray,long startRowNumber,long endRowNumber);
int initHK(char*hkrangefile,double *hkmin,double*hkmax);
double getModuleOC(char*ssmfilename,double mintemp,double maxtemp);
int initOC(char*ocrangefile,double *threashold_angoff,double*threashold_cpm,double*mintemp,double*maxtemp);
double getOC(char*mkffile,char*ssmfile,char*ocrangefile);
int writeOutput(char*outfile, char*l1file,char*dataquality, double xx,double yy, double HQ,double OC);
double getNoisyFraction(char*quality_logfile);
//char *basename(char const *path);
#endif /* DATAQUALITYCHECK_H_ */
