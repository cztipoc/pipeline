/* Edits:
 *
 *  Changes in input/output
 *  Handling both GTITYPE files
 *  Exposure fractions included in expmap
 *  
 *  Mithun (10/12/15)
 * */

#include "cztbindata.h"
#include "level2validation.h"
#include "gtiHandler.h"

using namespace std;

cztbindata::cztbindata(){
    strcpy(modulename,"cztbindata_v");
    strcat(modulename,VERSION);
    strcpy(quadsToProcess,"-");
    binsize=1.0;
	maskWeight=YES;
}

cztbindata::~cztbindata(){
}

int cztbindata::read(int argc,char **argv){
    int status=0;
    vector <string> inStrings;
    
    if(PIL_OK!=(status=PILInit(argc,argv))){
        LOG(ERROR)<<"***Error Initializing PIL***";
        return status;
    }
    
    //Input energy added event file
    if(PIL_OK!=(status=PILGetFname("par_infile",infile))){
        LOG(ERROR)<<"***Error reading input energy added event file"<<infile<<"***";
        return status;
 }
    

    //Coordinate type: camera/celestial
    inStrings.clear();
    inStrings.push_back("camera");
    inStrings.push_back("celestial");
    do {
        if (PIL_OK != (status = PILGetString("coordtype", coordtype))) {
            LOG(ERROR) << "***Error reading input type***";
            LOG(ERROR) << "***Type can be camera/celestial***";
            return status;
        }
    } while (isStringValid((string) coordtype, inStrings));

    if (strcasecmp(coordtype, "camera") == 0) {
        coordflag = CAMERA;
    } else if (strcasecmp(coordtype, "celestial") == 0) {
        coordflag = CELESTIAL;
    }
    
    //Aspect file names if coordinate type is celestial
    if (coordflag==CELESTIAL) {
        if (PIL_OK != (status = PILGetFname("aspectfileQ0", aspectfileQ0))) {
            LOG(ERROR) << "***Error reading input aspect file for Q0***";
            return status;
        }
        if (PIL_OK != (status = PILGetFname("aspectfileQ1", aspectfileQ1))) {
            LOG(ERROR) << "***Error reading input aspect file for Q1***";
            return status;
        }
        if (PIL_OK != (status = PILGetFname("aspectfileQ2", aspectfileQ2))) {
            LOG(ERROR) << "***Error reading input aspect file for Q2***";
            return status;
        }
        if (PIL_OK != (status = PILGetFname("aspectfileQ3", aspectfileQ3))) {
            LOG(ERROR) << "***Error reading input aspect file for Q3***";
            return status;
        }
    }

    //THETA1 AND THETA2 VALUES IN DEGREES
    //IF CELESTIAL THETA1=RA & THETA2=DEC
    //IF CAMERA THEN THETA1=thetax & THETA2=thetay
    if (PIL_OK != (status = PILGetReal4("theta1", &theta1))) {
        LOG(ERROR) << "***Error reading theta1***";
        return status;
    }

    if (PIL_OK != (status = PILGetReal4("theta2", &theta2))) {
        LOG(ERROR) << "***Error reading theta2***";
        return status;
    }
    
    //QUADS TO PROCESS
    do {
        if (PIL_OK != (status = PILGetString("par_quadsToProcess", quadsToProcess))) {
            LOG(ERROR) << "***Error reading quadrant:" << quadsToProcess << "***";
            return status;
        }
    } while (isQuadrantNumbersValid(quadsToProcess));
    
    //PIXEL QUALITY FILE
    if(PIL_OK!=(status=PILGetFname("par_pixelqualityfile",pixelqualityfile))){
        LOG(ERROR)<<"***Error reading pixel quality file***";
        return status;
    }
    
    //BADPIX THRESHOLD
    if (PIL_OK != (status=PILGetInt("badpixThreshold", &badpixThreshold))){
        LOG(ERROR) << "***Error reading badpix threshold.***";
        return  EXIT_FAILURE;
    }


/*
 CALDB FILES (TO BE READ BY QUERYING CIF)
*/

    //Input compressed mask file
    /*if(PIL_OK!=(status=PILGetFname("par_maskfile",compmaskfile))){
        LOG(ERROR)<<"***Error reading mask file"<<compmaskfile<<"***";
        return status;
    }


    //EBOUNDS FILE
    if(PIL_OK!=(status=PILGetFname("par_eboundsfile",eboundsfile))){
        LOG(ERROR)<<"***Error reading energy bounds CALDB file***";
        return status;
    }
    
    //CAMERA GEOMETRY FILE
    if(PIL_OK!=(status=PILGetFname("par_camgeomfile", cameraGeomFile))) {
        LOG(ERROR) << "***Error reading camera geometry CALDB file***";
        return status;
    }
    //Effective Area file
    if (PIL_OK != (status = PILGetFname("par_effareafile", effectiveAreafile))) {
        LOG(ERROR) << "***Error reading effective area CALDB file***";
        return status;
    }*/

/*
    END OF CALDB FILES
*/   
 
    //OUTTYPE: SPEC, LC, BOTH
    inStrings.clear();
    inStrings.push_back("lc");
    inStrings.push_back("spec");
    inStrings.push_back("both");
    do {
        if (PIL_OK != (status = PILGetString("par_outtype", outputtype))) {
            LOG(ERROR) << "***Error reading output type***";
            return status;
        }
    } while (isStringValid((string) outputtype, inStrings));


    if (strcasecmp(outputtype, "lc") == 0) {
        outtypeflag = LC;
    } else if (strcasecmp(outputtype, "spec") == 0) {
        outtypeflag = SPEC;
    } else if (strcasecmp(outputtype, "both") == 0) {
        outtypeflag = SPECLC;
    }

   
    //Get time binsize and energy range for lc 
    if(outtypeflag==LC || outtypeflag==SPECLC) {
        if (PIL_OK != (status = PILGetReal4("par_binsize", &binsize))) {
            LOG(ERROR) << "Error reading binsize***";
            return status;
        }
        do {

            if (PIL_OK != (status = PILGetString("par_energyrange", energyrange))) {
                LOG(ERROR) << "***Error Reading energyrange:" << energyrange << "***";
                return status;
            }
        } while (isBinsValid(energyrange));

        //Livetime file
        if (PIL_OK != (status = PILGetFname("par_livetimefile", livetimefile))) {
            LOG(ERROR) << "***Error reading livetime file name:" << livetimefile<< "***";
            return status;
        }


    }
   
    //OUTFILE PREFIX: out (spectrum name: out.spec; light curve name: lc.spec)
    if (PIL_OK != (status = PILGetFname("par_outfile", outfile))) {
        LOG(ERROR) << "***Error reading output LC/SPEC file:" << outfile << "***";
        return status;
    }

    if (PIL_OK != (status = PILGetFname("par_outevtfile", outevtfile))) {
        LOG(ERROR) << "***Error reading output weight added evt file name:" << outevtfile << "***";
        return status;
    }
    
    
    //EXPOSURE ARRAY FILE
    if (PIL_OK != (status = PILGetFname("par_expar", outexparrayfile))) {
        LOG(ERROR) << "***Error reading input/output exposure array file name:" << outexparrayfile<< "***";
        return status;
    }


/*    
    //GTI FILE
    if (PIL_OK != (status = PILGetFname("par_gtifile", gtifile))) {
        LOG(ERROR) << "***Error reading gtifile***";
        return status;
    }

    
    //USER TIME RANGE

    do {
        if (PIL_OK != (status = PILGetString("par_timerange", timerange))) {
            LOG(ERROR) << "***Error Reading timerange***";
            return status;
        }
    } while (isBinsValid(timerange));

*/    
    
    if(PIL_OK!=(status=PILGetBool("par_history",&history))){
        LOG(ERROR)<<"***Error reading history parameter***";
        return status;
    }
    if(PIL_OK!=(status=PILGetBool("par_clobber",&clobber))){
        LOG(ERROR)<<"***Error Reading clobber***";
        return status;
    }
    if (PIL_OK != (status = PILGetBool("par_debug", &debug))) {
        LOG(ERROR) << "***Error reading debugmode parameter***";

        return status;
    }

    if (PIL_OK != (status = PILGetBool("par_maskWeight", &maskWeight))) {
        LOG(ERROR) << "***Error reading maskWeight parameter***";

        return status;
    }
    

    PILClose(status);
    return (EXIT_SUCCESS);
}

void cztbindata::clear(){
    strcpy(infile,"");
    //strcpy(compmaskfile,"");
    strcpy(aspectfileQ0,"");
    strcpy(aspectfileQ1,"");
    strcpy(aspectfileQ2,"");
    strcpy(aspectfileQ3,"");
    //strcpy(cameraGeomFile,"");
    //strcpy(effectiveAreafile,"");
    strcpy(pixelqualityfile, "");
    //strcpy(eboundsFile,"");
    strcpy(outfile,"");
    strcpy(outexparrayfile,"");
//    strcpy(timerange,"");
    strcpy(energyrange,"");
//    strcpy(gtifile,"");
    strcpy(quadsToProcess, "");
    binsize=1.0; 
    strcpy(outputtype,"");
}


int cztbindata::read(char *infile, char* coordtype, vector <string> aspectFileNames, 
        char *quadsToProcess, char *pixelqualityfile,char* outputtype, char *energyrange,
        char* outfile,char *outevtfile, char* outexparrayfile, char *livetimefile, int badpixThreshold, float binsize, float theta1,
        float theta2, int clobber, int history, int debug,int maskWeight){
    strcpy(this->infile,infile);
    //strcpy(this->compmaskfile,compmaskfile);
    strcpy(this->coordtype, coordtype);
    if (strcasecmp(coordtype, "camera") == 0) {
        coordflag = CAMERA;
    } else if (strcasecmp(coordtype, "celestial") == 0) {
        coordflag = CELESTIAL;
    }
    if(coordflag==CELESTIAL && aspectFileNames.size()==4){
        strcpy(this->aspectfileQ0,(aspectFileNames[0]).c_str());
        strcpy(this->aspectfileQ1,(aspectFileNames[1]).c_str());
        strcpy(this->aspectfileQ2,(aspectFileNames[2]).c_str());
        strcpy(this->aspectfileQ3,(aspectFileNames[3]).c_str());
    } else if(coordflag==CELESTIAL && aspectFileNames.size()<4){
        LOG(ERROR) << "Please provide 4 aspect files (one for each quadrant)";
        return EXIT_FAILURE;
    }
    this->theta1=theta1; //Camera/celestial coordinate based on coordinate flag
    this->theta2=theta2;
    strcpy(this->quadsToProcess, quadsToProcess);
    //strcpy(this->eboundsfile, eboundsfile);
    //strcpy(this->cameraGeomFile, cameraGeomFile);
    //strcpy(this->effectiveAreafile, effectiveAreafile);
    strcpy(this->livetimefile,livetimefile);
    strcpy(this->pixelqualityfile,pixelqualityfile);
    strcpy(this->outputtype,outputtype);
    if (strcasecmp(outputtype, "lc") == 0) {
        outtypeflag = LC;
    } else if (strcasecmp(outputtype, "spec") == 0) {
        outtypeflag = SPEC;
    } else if (strcasecmp(outputtype, "both") == 0) {
        outtypeflag = SPECLC;
    }
//    strcpy(this->gtifile, gtifile);
//    strcpy(this->timerange,timerange);
    strcpy(this->outfile, outfile);
    strcpy(this->outexparrayfile, outexparrayfile);
    if(outtypeflag==LC || outtypeflag==SPECLC){
        strcpy(this->energyrange, energyrange);
        this->binsize=binsize;
    }
    this->debug=debug;
    this->clobber=clobber;
    this->history=history;
	this->maskWeight=maskWeight;
    return (EXIT_SUCCESS);
}

void cztbindata::display(){
    LOG(INFO)<<"----------------------------------------------------------------------------";
    LOG(INFO)<<"                          CZTBINDATA PARAMETERS                            ";
    LOG(INFO)<<"----------------------------------------------------------------------------";
    LOG(INFO) << "Input Event (energy added) file  :" << infile;
    //LOG(INFO) << "Input Mask file                  :" << compmaskfile;
    LOG(INFO) << "Coordinate type                  :" << coordtype;
    if(coordflag==CELESTIAL){
        LOG(INFO) << "Input Aspect file Q0             :" << aspectfileQ0;
        LOG(INFO) << "Input Aspect file Q1             :" << aspectfileQ1;
        LOG(INFO) << "Input Aspect file Q2             :" << aspectfileQ2;
        LOG(INFO) << "Input Aspect file Q3             :" << aspectfileQ3;
    }
    LOG(INFO) << "RA/thetax (degrees)              :" << theta1;
    LOG(INFO) << "Dec/thetay (degrees)             :" << theta2;
    LOG(INFO) << "Quadrants to be processed        :" << quadsToProcess;
    LOG(INFO) << "Pixel Quality File               :" << pixelqualityfile;
    //LOG(INFO) << "Ebounds File                     :" << eboundsfile;
    //LOG(INFO) << "Camera Geom File                 :" << cameraGeomFile;
    //LOG(INFO) << "Effective Area File              :" << effectiveAreafile;
//    LOG(INFO) << "GTIfile                          :" << gtifile;
    LOG(INFO) << "Output file                      :" << outfile;
    LOG(INFO) << "Output Type                      :" << outputtype;
//    LOG(INFO) << "Time Range                       :" << timerange;
    if(outtypeflag==LC || outtypeflag==SPECLC) {
    LOG(INFO) << "Energy Range (for lc generation) :" << energyrange;
    LOG(INFO) << "Bin Size (for lc generation)     :" << binsize;
    LOG(INFO) << "Livetime file                    :" << livetimefile;
    
    }

	if(maskWeight==YES)
    LOG(INFO)<<"Maskweight                         :  YES";
	else
    LOG(INFO)<<"Maskweight                         :  NO";
	
    if(debug==YES)
    LOG(INFO)<<"Debug                              :  YES";
    else
    LOG(INFO)<<"Debug                              :  NO";
    if(clobber==YES)
    LOG(INFO)<<"Clobber                            :  YES";
    else
    LOG(INFO)<<"Clobber                            :  NO";
    if(history==YES)
    LOG(INFO)<<"History                            :  YES";
    else
    LOG(INFO)<<"History                            :  NO";
   
    LOG(INFO)<<"----------------------------------------------------------------------------";
}

int cztbindata::cztbindataProcess(){
    fitsfile *fptrIN;
    int status=0; //status variable
    int i=0, j=0; //indexVariable
    vector <string> outfilenames;
    vector<string> inputFilenames;
    vector<string> nonExistentFiles;
    char outfileLC[PIL_LINESIZE];
    char outfileSPEC[PIL_LINESIZE];
    vector <string> aspectFilenames;
    AspectFileHandler aspect;
    float nX = 0.0, nY = 0.0, nZ = 0.0, nXt = 0.0, nYt = 0.0, nZt = 0.0; //pointing and twist vectors
    float RA = 0.0, DEC = 0.0, TWIST = 0.0; //RA,DEC (pointing direction) & TWIST
    float txrad=0.0, tyrad=0.0, thetaxd=0.0, thetayd=0.0; //Camera coordinates in both radian and degrees.
    string compMaskFile;
    string camGeomFile;
    string eboundsFile;
    string effAreaFile;
    TanMaskGeometry tanmask;
    vector <vector <unsigned char> > fullfinermask;
    ExposureTable exptable;
    ExposuremapFileHandler expmapfh;
    EventFileHandler evtfile; //to read energy added event file (output of cztpha2energy)


/*
    GTIhandler gtifh;
    vector <GTIrecord> gti1; //from gti file
    vector <GTIrecord> gti2; //from user input
    vector <GTIrecord> gtiout; //from user input
*/
    int ntbins=0; //number of time bins provide by user other than GTI
    int nebins=0; //number of energy bins provided by user for light curve generation
//    vector <double> userTstart, userTstop; //tstart and tstop values provided by user (other than GTI)
    vector <float> userEstart, userEstop; //estart and estop values provided by user (for lc)
    vector <double> tstart, tstop;
//    vector <double> gtiTstart, gtiTstop;
    vector <float> specEstart, specEstop;
    float theta1rad=0.0;
    float theta2rad=0.0;
    vector <int> quadVec;
    int itime=0;
    fitsfile *fin;            //pointer to input event file
    bool readExpFlag=false;    //true: expfile read false:unable to read expfile
    int iquad=0;              //index quadrant

    char gtitype[20];
    int tfilter,efilter;
    
    //Generating full path for CALDB files

/**
 * Replace with CIF Querying
 **/
    /*
    compMaskFilePath = caldb_full_path_generator((string) compmaskfile);
    camGeomFilePath = caldb_full_path_generator((string) cameraGeomFile);
    eboundFilePath = caldb_full_path_generator((string) eboundsfile);
    effAreaFilePath = caldb_full_path_generator((string) effectiveAreafile);*/
    
    //Checking existence of input files
    inputFilenames.clear();
    inputFilenames.push_back((string)infile);
 //   inputFilenames.push_back(compMaskFile);
    if(coordflag==CELESTIAL){
        inputFilenames.push_back((string) aspectfileQ0);
        inputFilenames.push_back((string) aspectfileQ1);
        inputFilenames.push_back((string) aspectfileQ2);
        inputFilenames.push_back((string) aspectfileQ3);
    }
 //   inputFilenames.push_back(eboundFile);
 //   inputFilenames.push_back(camGeomFile);
 //   inputFilenames.push_back(effAreaFile);
    inputFilenames.push_back((string) pixelqualityfile);
//    inputFilenames.push_back((string) gtifile);
    
    files_exist(inputFilenames, &nonExistentFiles);
    if(nonExistentFiles.size()>0){
        LOG(ERROR) << "Following input files do not exist:";
        for(i=0; i<nonExistentFiles.size(); i++){
            LOG(ERROR) << i+1 << ". " << nonExistentFiles[i];
        }
        return EXIT_FAILURE;
    }

    //Get energy range for lc generation
    if(outtypeflag==LC || outtypeflag==SPECLC){
        nebins=get_nbins(energyrange);
        if (getVecBins((string) energyrange, &userEstart, &userEstop, nebins)) {
            LOG(ERROR) << "Error extracting Energy bins from energy range provided by user.";
            return EXIT_FAILURE;
        }
    }


//**********************************************************************************

    //Read the GTITYPE from the header of primary

    fitsfile *fevt;

    fits_open_file(&fevt, infile, READONLY, &status);
    if(status){
        fits_report_error(stderr,status);
        LOG(ERROR)<<"***Error in opening event file";
        return (EXIT_FAILURE);
    }
    double head_tstart,head_tstop;    
    int  effareaExtnum,eboundsExtnum,geometryExtnum,compmaskExtnum;
    fits_read_key(fevt, TDOUBLE, "TSTART", &head_tstart, NULL, &status);
    report_error(status, (string) "Error in reading keyword TSTART");

    fits_read_key(fevt, TDOUBLE, "TSTOP", &head_tstop, NULL, &status);
    report_error(status, (string) "Error in reading keyword TSTOP");

		
    if(QueryCaldb("ASTROSAT","CZTI","-","EBOUNDS",head_tstart,head_tstop,eboundsFile,eboundsExtnum))
    {
        LOG(ERROR) << "Not able to get CALDB EBOUNDS file";
        return (EXIT_FAILURE);
    }
 
    if(QueryCaldb("ASTROSAT","CZTI","QUADRANT0","EFF_AREA",head_tstart,head_tstop,effAreaFile,effareaExtnum))
    {
        LOG(ERROR) << "Not able to get CALDB EFF_AREA file";
        return (EXIT_FAILURE);
    }

    if(QueryCaldb("ASTROSAT","CZTI","-","GEOMETRY",head_tstart,head_tstop,camGeomFile,geometryExtnum))
    {
        LOG(ERROR) << "Not able to get CALDB GEOMETRY file";
        return (EXIT_FAILURE);
    }
    
    if(QueryCaldb("ASTROSAT","CZTI","-","MASK_OVERSAMPLED",head_tstart,head_tstop,compMaskFile,compmaskExtnum))
    {
        LOG(ERROR) << "Not able to get CALDB MASK_OVERSAMPLED file";
        return (EXIT_FAILURE);
    }


    fits_read_key(fevt,TSTRING,"GTITYPE",gtitype,NULL,&status);
    if(status){
        fits_report_error(stderr,status);
        LOG(ERROR)<<"***Error in reading GTITYPE in event file header";
        return (EXIT_FAILURE);
    }
  

    fits_close_file(fevt,&status);
    if(status){
        fits_report_error(stderr,status);
        LOG(ERROR)<<"***Error in closing event file";
        return (EXIT_FAILURE);
    }

  
/*    
    //Extracting time bins and energy bins provided by user
    ntbins = get_nbins(timerange);
    if(getVecBins((string)timerange, &userTstart, &userTstop, ntbins)){
        LOG(ERROR) << "Error extracting time bins from time range provided by user.";
        return EXIT_FAILURE;
    }
 */   

/*    
    //Reading time bins specified in gti file and finding
    //intersetion of gti time with user time input.
    try{
        gtifh.read_gti_file((string)gtifile);
        gti1 = gtifh.get_GTI();
        //setting gti2
        gtifh.set_GTI(userTstart, userTstop);
        gti2=gtifh.get_GTI();
        //Finding the intersecting time range
        find_intersecting_range(gti1, gti2, &gtiout);
    } catch(ErrorHandler errHandler){
        logError(errHandler);
        return EXIT_FAILURE;
    }
    
    tstart.resize(gtiout.size(), 0.0);
    tstop.resize(gtiout.size(), 0.0);
    for(itime=0; itime<gtiout.size(); itime++){
        tstart[itime]=gtiout[itime].tstart;
        tstop[itime] = gtiout[itime].tstop;
    }
*/

 
    //Getting quadsToProcess
    if(get_quadsToProcessVec((string)quadsToProcess, &quadVec)){
        LOG(ERROR) << "Error getting quadrants to be processed.";
        return EXIT_FAILURE;
    }

    if(strcasecmp(gtitype,"COMMON")==0)
    {
    //Generating exposure array for all quadrants
    quadVec.clear();
    for(i=0;i<4;i++) quadVec.push_back(i);
    }

    //Calculating and assigning camera coordinates of the source for which light
    //curve and spectrum needs to be generated.
    if(coordflag==CAMERA){
        thetaxd=theta1;
        thetayd=theta2;
        txrad=theta1*TORAD;
        tyrad=theta2*TORAD;
    } else if (coordflag == CELESTIAL) {
        //GETTING RA,DEC, TWIST FOR POINTING DIRECTION
        //Reading multiple aspect files to calculate average RA, DEC & TWIST values
        //for all 4 quadrants
        aspectFilenames.push_back(aspectfileQ0);
        aspectFilenames.push_back(aspectfileQ1);
        aspectFilenames.push_back(aspectfileQ2);
        aspectFilenames.push_back(aspectfileQ3);

        if (aspect.read_multiple_aspect_files(aspectFilenames)) {
            LOG(ERROR) << "Error in reading multiple aspect files simultaneously.";
            return EXIT_FAILURE;
        }

        if (aspect.get_avg_pointing_and_twist_vector(nX, nY, nZ, nXt, nYt, nZt, status)) {
            LOG(ERROR) << "Error in calculating average pointing and twist vector.";
            return EXIT_FAILURE;
        }

        if (calculate_RA_DEC_Twist(nX, nY, nZ, nXt, nYt, nZt, RA, DEC, TWIST, status)) {
            LOG(ERROR) << "Error in calculating RA, DEC & TWIST values.";
            return EXIT_FAILURE;
        }
        //RA, DEC & TWIST VALUES STORED

        //Finding camera coordinates for user defined RA & DEC values.
        theta1rad=theta1*TORAD;
        theta2rad=theta2*TORAD;
        if (to_thetaX_thetaY(RA, DEC, TWIST, theta1rad, theta2rad, txrad, tyrad)) {
            LOG(ERROR) << "Error in converting celestial coordinates RA,DEC: " <<
                    theta1 << ", " << theta2 << " to camera coordinates.";
            return EXIT_FAILURE;
        }
        thetaxd = txrad * TODEG;
        thetayd = tyrad * TODEG;
        //txrad, tyrad, thetax and thetay evaluated.
    }


if(maskWeight==true)
{
    //Writing exposure array file 
    if(FileExists(outexparrayfile)){
        if(expmapfh.read_exposure_array_file((string)outexparrayfile)){
            LOG(WARNING)<< "Warning: Unable to read exposure array file. Creating new with same name.";
            unlink(outexparrayfile);
        }
        else {
            readExpFlag=true;
        }
        
    }

    if(readExpFlag==true){
        exptable=expmapfh.get_exposure_table();
        if(exptable.thetaxd!=thetaxd || exptable.thetayd!=thetayd){
            LOG(WARNING) << "Exposure file provided by user has been generated for thetax: " << exptable.thetaxd
                    << " thetay: " << exptable.thetayd;
            readExpFlag=false;
            outfilenames.clear();
            outfilenames.push_back((string) outexparrayfile);
            if (deleteFile(outfilenames, clobber)) {
                LOG(ERROR) << "Error in deleting output exposure array file.";
                return EXIT_FAILURE;
            }
        }
    }
    if(readExpFlag==false) {

        exptable.reset();

        //COMPRESSED MASK READING
        //Extracting compressed mask file
        LOG(INFO) << "Reading compressed mask file to create 18200x18200 full uncompressed mask.";
        if (tanmask.read_compressed_mask(compMaskFile)) {
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

        //Creating exposure array
        if (create_exposure_array(thetaxd, thetayd, camGeomFile, fullfinermask, exptable,
                1, 10, eboundsFile)) {
            LOG(ERROR) << "Error in creating exposure array";
            return EXIT_FAILURE;
        }
        //Exposure file generated
        
        /* Modified to pass the event file name.
         *  To read exposure fractions and use them in renormalizing weights
         * */
        //Renormalizing weights
        if (calculate_renormalized_weights(exptable, pixelqualityfile,
                badpixThreshold, quadVec, effAreaFile, 1,infile)) {
            LOG(ERROR) << "Error in calculating renormalized weights.";
            return EXIT_FAILURE;
        }
        //Weights renormalized

        expmapfh.set_exposuretable(exptable);
        if (expmapfh.write_exposure_array_file((string) outexparrayfile, "ExparrayTemplate")) {
            LOG(ERROR) << "Error in writing exposure array file.";
            return EXIT_FAILURE;
        }
    }

}
else
{

	LOG(INFO)<<"******MASK WEIGHTS ARE NOT APPLIED WHILE GENERATING SPECTRUM/LC *******";
		
	vector <unsigned short> PIsArray;
	
	for (i=0;i<512;i++)PIsArray.push_back(i);
	exptable.thetaxd=thetaxd;
	exptable.thetayd=thetayd;
	exptable.PIs=PIsArray;
	exptable.nchannels=512;

}
	//Reading energy added event file
  
    int qid;
    SpectrumLc spec, lc;
    SpectrumFileHandler specfh;
    LightCurveFileHandler lcfh;

	spec.badpixthresh=badpixThreshold;
	spec.maskweighted=(int)maskWeight;
    spec.badpixfile=pixelqualityfile;

	lc.badpixthresh=badpixThreshold;
	lc.maskweighted=(int)maskWeight;
    lc.badpixfile=pixelqualityfile;

    int qstart=quadVec[0],qstop=quadVec[quadVec.size()-1];

    //Add flags to decide whether to filter based on time and energy
    // tfilter/efilter: =0: No filtering =1: Apply filter 
    try{
        for(qid=qstart;qid<=qstop;qid++)
        {
            spec.clear();
            lc.clear();
	   
            spec.quadrantid=qid;
            lc.quadrantid=qid;

            if(strcasecmp(gtitype,"COMMON")==0) // Generate for all quadrants added
            {
				spec.allquad=1;
				lc.allquad=1;
                quadVec.clear();
                for(i=0;i<4;i++) quadVec.push_back(i);
           
                sprintf(outfileLC,"%s.lc",outfile); 
                sprintf(outfileSPEC,"%s.pha",outfile);        
                              
	        }
            else if (strcasecmp(gtitype,"QUAD")==0) //Generate for single quadrant
            {
				spec.allquad=0;
				lc.allquad=0;
                quadVec.clear();
                quadVec.push_back(qid);
                sprintf(outfileLC,"%s_Q%d.lc",outfile,qid);
                sprintf(outfileSPEC,"%s_Q%d.pha",outfile,qid);
            }
            else
            {
                LOG(ERROR)<<"Unknown GTITYPE in event file header";
                return(EXIT_FAILURE);
            }
                

            //Checking existence of outfiles and deletes them if clobber is yes
            outfilenames.clear();
            outfilenames.push_back((string)outfileLC);
            outfilenames.push_back((string)outfileSPEC);
            outfilenames.push_back((string)outevtfile);
            if (deleteFile(outfilenames, clobber)) {
            LOG(ERROR) << "Error in deleting output files.";
                return EXIT_FAILURE;
            }

			 
            if(outtypeflag==SPEC){
                spec.bin_spectrum((string) infile, tstart, tstop, specEstart, specEstop, quadVec, exptable,0,0,maskWeight);//tfilter=efilter=0
				specfh.set_spectrum(spec);
                if(specfh.write_spectrum_file((string)outfileSPEC, "spectrumTemplate",(string)infile)){
                    LOG(ERROR) << "Error writing spectrum file.";
                    return EXIT_FAILURE;
                }
            }
            if(outtypeflag==LC){
                lc.bin_lc((string) infile, tstart, tstop, userEstart, userEstop, quadVec, exptable,binsize,0,efilter,livetimefile,maskWeight);//tfilter=0
                cout<<"CAME OUT OF LC\n";
                lcfh.set_lc(lc);
                if(lcfh.write_lc_file((string)outfileLC, "lcTemplate",(string)infile)){
                    LOG(ERROR) << "Error writing light curve file.";
                    return EXIT_FAILURE;
                }
            }

            if(outtypeflag==SPECLC) {
                spec.bin_spectrum((string) infile, tstart, tstop, specEstart, specEstop, quadVec, exptable,0,0,maskWeight); //tfilter=efilter=0
                lc.bin_lc((string) infile, tstart, tstop, userEstart, userEstop, quadVec, exptable,binsize,0,0,livetimefile,maskWeight);//tfiletr=0
                specfh.set_spectrum(spec);
                if (specfh.write_spectrum_file((string)outfileSPEC, "spectrumTemplate",(string)infile)) {
                    LOG(ERROR) << "Error writing spectrum file.";
                    return EXIT_FAILURE;
                }
                lcfh.set_lc(lc);
                if (lcfh.write_lc_file((string)outfileLC, "lcTemplate",(string)infile)) {
                    LOG(ERROR) << "Error writing light curve file.";
                    return EXIT_FAILURE;
                }
            }

            //If common, job is done already
            if(strcasecmp(gtitype,"COMMON")==0)
                break;            
            LOG(INFO)<<"Quadrant "<<qid<<" processing is over";
            }
    } catch(ErrorHandler errHandler){
        logError(errHandler);
        return EXIT_FAILURE;
    }

    //Write the event file with computed mask weights and open fractions

if(maskWeight==true)
{
     //copying input to output file
     fitsfile *foutevt;
    

     fits_open_file(&fevt,infile,READONLY,&status);
     if(status){
         fits_report_error(stderr,status);
         LOG(ERROR)<<"***Error in opening file "<<infile<<"***";
         return (EXIT_FAILURE);
     }
     fits_create_file(&foutevt,outevtfile,&status);
      if(status){
         fits_report_error(stderr,status);
         LOG(ERROR)<<"***Error in creating file "<<outevtfile<<"***";
         return (EXIT_FAILURE);
     }
     LOG(INFO) << "Output evt file created" ;
     fits_copy_file(fevt,foutevt,1,1,1,&status);
     if(status){
         fits_report_error(stderr,status);
         LOG(ERROR)<<"***Error in copying input to output file***";
         return (EXIT_FAILURE);
     }
     LOG(INFO)<<"File copied";

    //Generating exposure index table for faster processing
    try{
        exptable.generate_index_table(false);
    } catch (ErrorHandler errHandler){
        throw errHandler;
    }

    fits_close_file(fevt,&status);  
    if(status) { fits_report_error(stderr,status); return (EXIT_FAILURE); }

    char *ttype[]={"OPEN_FRACTION", "WEIGHT"};
    char *tform[]={"E", "E"};

    long nrows;
    int ncols;
    int detid_col,pixid_col,detid,pixid,PI_col,PI;
    int detx,dety;
    int indexPix;
    int locx,locy;

    for(qid=0;qid<4;qid++)
    {
        char quadnam[10];
        sprintf(quadnam,"Q%d",qid);

        try{
        fits_movnam_hdu(foutevt, BINARY_TBL, quadnam, 0, &status);
        
        fits_get_num_cols(foutevt,&ncols,&status);

        fits_get_colnum(foutevt,CASEINSEN,"DETID",&detid_col,&status);

        fits_get_colnum(foutevt,CASEINSEN,"PIXID",&pixid_col,&status);

        fits_get_colnum(foutevt,CASEINSEN,"PI",&PI_col,&status);

        fits_get_num_rows(foutevt, &nrows, &status);

        fits_insert_cols(foutevt,ncols+1,2,ttype,tform,&status);

        }catch(ErrorHandler errHandler){
            logError(errHandler);
            return EXIT_FAILURE;
        }

        for(i=0;i<nrows;i++)
        {
        
            try{
            fits_read_col(foutevt, TINT, detid_col, i+1, 1,1, NULL, &detid,NULL, &status);
            fits_read_col(foutevt, TINT, pixid_col, i+1, 1,1, NULL, &pixid,NULL, &status);
            fits_read_col(foutevt, TINT, PI_col, i+1, 1,1, NULL, &PI,NULL, &status);
            } catch(ErrorHandler errHandler){
            logError(errHandler);
            return EXIT_FAILURE;
            }

            generate_locx_locy(detid, pixid,qid, detx, dety, locx, locy);
            indexPix = exptable.indexTable[locy][locx];

            try{
            fits_write_col(foutevt, TFLOAT, ncols+1, i+1,1, 1, &exptable.openfracArray[indexPix][PI], &status);
            fits_write_col(foutevt, TFLOAT, ncols+2, i+1,1, 1, &exptable.weightsArray[indexPix][PI], &status);
            } catch(ErrorHandler errHandler){
            logError(errHandler);
            return EXIT_FAILURE;
            }


        }
        
        
    }


    fits_close_file(foutevt,&status); 
    if(status) { fits_report_error(stderr,status); return (EXIT_FAILURE); }

 }
    
    return (EXIT_SUCCESS);
}




//int cztbindata::getHistory(vector<string> &vhistory){
//    char *user=getlogin();
//    string str="Module run by "+(string)user;
//    vhistory.push_back(str);
//    vhistory.push_back("Parameter List START for "+(string)modulename);
//    vhistory.push_back("P1 infile="+(string)infile);
//    vhistory.push_back("P2 maskfile="+(string)maskfile);
//    vhistory.push_back("P3 aspectfile="+(string)aspectfile);
//    vhistory.push_back("P4 outfile="+(string)outfile);
//    vhistory.push_back("P5 outputtype="+(string)outputtype);
//    vhistory.push_back("P6 timerange="+(string)timerange);
//    vhistory.push_back("P7 energyrange="+(string)energyrange);
//    vhistory.push_back("P8 bintype="+(string)bintype);
//    vhistory.push_back("P9 gtifile="+(string)gtifile);
//    char temp[25];
//    sprintf(temp,"%d",binsize);
//    vhistory.push_back("P10 binsize="+(string)temp);
//    vhistory.push_back("P11 tcol="+(string)tcol);
//    vhistory.push_back("P12 ecol="+(string)ecol);
//    vhistory.push_back("P13 xcol"+(string)xcol);
//    vhistory.push_back("P14 ycol="+(string)ycol);
//    if(clobber==YES) 
//        vhistory.push_back("P15 clobber=yes");
//    else
//        vhistory.push_back("P15 clobber=no");
//    if(history==YES)
//        vhistory.push_back("P16 history=yes");
//    else
//        vhistory.push_back("P16 history=no");
//    vhistory.push_back("Parameter List END");
//    return (EXIT_SUCCESS);
//}






