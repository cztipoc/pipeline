/* 
 * File:   cztpipeline.h
 * Author: preeti
 *
 * Created on July 21, 2012, 3:28 PM
 */

#ifndef CZTPIPELINE_H
#define	CZTPIPELINE_H

//----------------Standard Include Libraries----------------
#include<pil.h>
#include<fitsio.h>
#include<errno.h>
#include<iostream>
#include<fnmatch.h>
#include<dirent.h>
#include<unistd.h>
#include<strings.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<vector>
#include<string>
#include<libgen.h>
#include "glog/logging.h"
#include "utils.h"              //common
#include "ExpMap.h"             //common
#include "cztstring.h"          //common
#include "glog/logging.h"       //common
#include "l1evtdecode.h"
#include "cztscience2event.h"    //cztscience2event
#include "cztpha2energyv2.h"
#include "cztgtigen.h"
#include "cztevtclean.h"
#include "cztflagbadpix.h"
#include "cztfilterbadpix.h"
#include "cztgaas_v2.h"
#include "cztdatasel.h"
#include "cztdpigen_v2.h"
#include "cztpsfshadowgen.h"
#include "cztbindatanew.h"
#include "cztimage.h"
#include "detectorGeometry.h"
#include "maskGeometry.h"
#include "gtiHandler.h"
#include "Exposure.h"
#include "cztmkfgen.h"
#include "Mvector.h"
#include "mkfRegeneration.h"


#define CZTIDIRNAME "czti"
#define MODELIST {"00","01","02","03","04","05","06","07","08","09","10","11","12","13","14","15"}
#define FILEPATHSEPARATOR "/"
#define FILEPATHSEPARATOR_C '/'
#define MODEDIRPREFIX "Mode"
#define MODEIDSIZE 3           //mode id string size in filenames and directory names
#define SATID  "AS1"        //Satellite id
#define INSTRUMENTID "czt"   //Instrument id
#define PATHSIZE 2048
#define NUMOFMODES 16

#define EVENTDATA_MODE_END 7

using namespace std;

//structure to hold absolute paths for directories for both input and output
typedef struct DirPath{

    char indir[PATHSIZE];
    char outpath[PATHSIZE];   //path for output directory to be created
    char outdir[PATHSIZE];    //output directory name
    char cztidir_in[PATHSIZE],cztidir_out[PATHSIZE];
    char hkdir_in[PATHSIZE],hkdir_out[PATHSIZE];
    char auxdir_in[PATHSIZE],auxdir_out[PATHSIZE];
    char aux1dir_in[PATHSIZE],aux1dir_out[PATHSIZE];
    char aux2dir_in[PATHSIZE],aux2dir_out[PATHSIZE];
    char aux3dir_in[PATHSIZE],aux3dir_out[PATHSIZE];
    string modedir_in[NUMOFMODES],modedir_out[NUMOFMODES];
    bool modepresent[NUMOFMODES];
    
    DirPath(){};
    int setAllDirPaths(char indir[],char outpath[]);  //function to set all directory paths
    int createOutDir();    //function for creation of output directories
    void display();
} DIRPATH; 

class cztpipeline{

private:
    //Data Members
    char modulename[NAMESIZE];
    char inTAR[PIL_LINESIZE];
    char indir[PIL_LINESIZE];   //Input level1 directory name 
    char outdir[PIL_LINESIZE];  //path for output directory to be created
    char caldb[PIL_LINESIZE];
    
    //booleans for optional modules
    int cztgtigenflag;  //if true cztdatasel will also be run
    int cztevtcleanflag;
    int cztfindbadpixflag;   //if true then cztfilterbadpix will also be run
    
    int bigendian;             //whether data in data packet is bigendian  (for cztscience2event module)

    //cztscience2event
    int nPackets;
    //for cztpha2energy
    char alphaval[10];
    char vetorange[512];
    
    //for cztgtigen
    char runmode[SHORT_STR_SIZE];
    char lbtextname[SHORT_STR_SIZE];
    char thresholdfile[PIL_LINESIZE];
    char customgti[PIL_LINESIZE];
    char gtiextname[SHORT_STR_SIZE];
    char usergti[PIL_LINESIZE];  //output gti from cztgtigen
    int runmodeflag;
    
    //for cztdpigen
    char quadsToProcess[25]; //new addition to incorporate quadrant wise functionality
    char ebins[PIL_LINESIZE];
    char tbins[PIL_LINESIZE];
    
    //for cztfindbadpix
    float badpixthreshold;
    char ra[SHORT_STR_SIZE];
    char dec[SHORT_STR_SIZE];
    
    //for cztfilterbadpix
    char outputquality[SHORT_STR_SIZE];   //good/bad/dead/noisy
        
    //for cztpsfshadowgen
    int catalogextnum;
    char RA_colname[SHORT_STR_SIZE];
    char DEC_colname[SHORT_STR_SIZE];
    char FLUX_colname[SHORT_STR_SIZE];
    
    //for cztbindata
    char energyrange[SHORT_STR_SIZE];
    char timerange[SHORT_STR_SIZE];
    float binsize_spec;
    float binsize_lc;
    
    //for cztimage
    float resolution_limit;
    int oversampfactor;
    float peak_thr;
    float source_strength_thr;
    int debugmode;             //if yes/true, creates intermediate file 
    
    //for cztremovebkgd
    int nBkgdCoef;
    
    int clobber;
    int history;
    
    char metafileprefix_in[PATHSIZE],metafileprefix_out[PATHSIZE];          //filename prefix for files other than data
    
    char hkfile[PATHSIZE],orbfile[PATHSIZE],attfile[PATHSIZE],
        tctfile[PATHSIZE],lbtfile[PATHSIZE], mkffile[PATHSIZE];
        char l2mkffile[PATHSIZE];
    
    DIRPATH objpath;
    
    //Member Functions
    /**
    * Function to extract observation ID from level1 directory name. The memory 
    * for level1dir must be freed in calling function. It is allocated using new in this function
    * @param level1dir
    * @return 
    */
    char *getObsID();
    
    /**
     * Function to set paths for aux files viz hk,orb,att,tct,lbt
     */
    int setAuxFiles();
    
    /**
     * Function to copy auxiliary data files to corresponding output level2 directory path
     * @return 
     */
    int copyAuxFiles();
    
public:
    cztpipeline();
    int read(int argc, char **argv);
    //int read(char *indir,char *outdir,char *caldb,int clobber,int history);
    void display();
    
    /**
    * The main function for czti data processing pipeline that calls all other 
    * routines for running the chain
    * @param param
    * @return 
    */
    int cztpipelineProcess();
};

/**
 * Class to contain all the file paths for one data set to run and 
 * its corresponding output file paths
 */
typedef struct DataSet{

    //Level1 input files
    char scifile[PATHSIZE];
    char l1gtifile[PATHSIZE];
    char l1btifile[PATHSIZE];
    char l1mkffile[PATHSIZE];
    char l1cfgfile[PATHSIZE];
    char l1logfile[PATHSIZE]; 
  
    //Level2 output files
    char hdrINFOfile[PATHSIZE];
    //char tempExtFilePath[PATHSIZE];
    char tempeventfile[PATHSIZE];
    char eventfile[PATHSIZE];
    char avgaspectfile[PATHSIZE];
    char DPHfile[PATHSIZE];
    char DPIfile[PATHSIZE];
    char l2gtifile[PATHSIZE];
    char l2btifile[PATHSIZE];
    char badpixelfile[PATHSIZE];
    char shadowfile[PATHSIZE];
    char sourcelist[PATHSIZE];   //ASCII
    char LCfile[PATHSIZE];
    char specLCfile[PATHSIZE];
    char outexparrFile[PATHSIZE];
    char specfile[PATHSIZE];
    char imagefile[PATHSIZE];
    char nobkgdfile[PATHSIZE];
    string fail_in_module; //used to store module name in which the dataset fails.
    
    void display();
};

typedef struct CALDBdatafile{
    char goFile[PATHSIZE];
    char badpixFile[PATHSIZE];
    char eboundsFile[PATHSIZE];
    char cameraGeomFile[PATHSIZE];
    char compMaskFile[PATHSIZE];
    char maskfile64x64[PATHSIZE];  //mask file same as pixels of quadrant
    char effAreaFile[PATHSIZE];
    char teldeffileQ0[PATHSIZE];
    char teldeffileQ1[PATHSIZE];
    char teldeffileQ2[PATHSIZE];
    char teldeffileQ3[PATHSIZE];
    char xrayCatalog[PATHSIZE];  
    
    /**Function to read all CALDB file paths to structure CALDBdatafile.
     * It appends the input caldb path to all filenames
     * @param CaldbDirPath
     * @return  
     */
    void getCaldbFiles(char *CaldbDirPath);
    int checkExistence();
    void display();
};







/**
 * Filter function to filter only the directories starting with MODEDIRPREFIX 
 * to find which mode data are present
 * @param dptr
 * @return 
 */
int modefilter(const struct dirent *dptr);

/**
 * Returns 1 for the files that ends with .fits extension
 * @param dptr
 * @return 
 */
int fitsfilter(const struct dirent *dptr);



/**
 * Function to read filenames with *.fits' extension from the given directory
 * fileadd is allocated memory inside with 'new' which must be deleted with 'delete' outside the function
 * @param modedir
 * @param fileadd
 * @param numberoffiles
 * @return 
 */
int getL1Files(char *modedir,char ***fileadd,int *numberoffiles);

int get_modeID(string temp);

int extractTars(string tar_name, string &fileoutname);

#endif	/* CZTPIPELINE_H */

