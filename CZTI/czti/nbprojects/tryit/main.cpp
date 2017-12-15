/* 
 * File:   main.cpp
 * Author: tanul
 *
 * Created on April 24, 2015, 4:28 PM
 */

#include <cstdlib>
#include <iostream>
#include "Fitting.h"
#include "glog/logging.h"
#include "caldbHandler.h"
#include "fitsio.h"
#include "level2validation.h"
#include "badpixCALDB.h"
#include "utils.h"
#include "level1handler.h"
#include "coordinateTransformation.h"
#include <algorithm>
#include <vector>
#include "detectorGeometry.h"
#include "cztexpmap.h"
#include <iomanip>
#include "maskGeometry.h"
#include "l1evtdecode.h"
#include "Exposure.h"
#include "errorHandler.h"
#include "gtiHandler.h"
#include "cztiopath.h"
#include "Mvector.h"
#include "mkfRegeneration.h"
#include "fft.h"





using namespace std;

char* getObsID(char *indir);

struct tempV{
    int temp1;
    float temp2;
    string temp3;
};

int main(int argc, char** argv) {
    google::InitGoogleLogging(argv[0]);
    google::SetStderrLogging(google::INFO);    
    int status=0;
    
//    Teldef t;
//    string teldefFileName= caldb_full_path_generator("AS1czt020150211v01.teldef");
//    if(teldefFileName==""){
//        LOG(ERROR)<< "Not able to generate teldef path.";
//        return(EXIT_FAILURE);
//    }
//    string eboundsFilename = caldb_full_path_generator("AS1cztebounds20150310v01.fits");
//    if(eboundsFilename==""){
//        LOG(ERROR)<< "Not able to generate EBOUNDS path.";
//        return(EXIT_FAILURE);
//    }
//    
//    string gainsFilename = caldb_full_path_generator("AS1cztgain20150310v01.fits");
//    
//    string offsetsFilename = caldb_full_path_generator("AS1cztoffset20150310v01.fits");
//    
//    string responseParsFilename = caldb_full_path_generator("AS1cztresp_par20150310v01.fits");
//    
//    string effareaFilename = caldb_full_path_generator("AS1czteffarea20150310v01.fits");
//
//    if(t.read_teldef_file(teldefFileName)) {
//        LOG(INFO) << "teldef file has not been read.";   
//    }
//    
//    Ebounds e;
//    if(e.read_ebounds_file(eboundsFilename)) {
//        LOG(INFO) << "unable to read ebounds file.";
//    }
//    
//    e.display();
//    
//
//    
//    Gain g;
//    if(g.read_gains_file(gainsFilename, "Q2")){
//        LOG(INFO) << "unable to read gains file.";
//        exit(EXIT_FAILURE);
//    }
//    g.display();
//    
//    Offset o;
//    if(o.read_offsets_file(offsetsFilename, "Q2")){
//        LOG(INFO) << "unable to read offsets file.";
//        exit(EXIT_FAILURE);
//    }
//    o.display();
//    
//    vector <float> temp(256,0.0);    
//    vector <vector <float> > rearranged_array(16,temp);
//
////    for (int i=0; i< 16; i++) {
////        rearranged_array.push_back(temp);
////    }
//    //create_2D_array(o, &rearranged_array);
//    
//    vector <unsigned char> detID= o.get_detID();
//    vector <unsigned char> pixID= o.get_PixID();
//    vector <float> o1 = o.get_offsets(1, status);
//    
//    create_2d_vector(detID, pixID, o1, &rearranged_array);
//    LOG(INFO) << "We are here";
//    for (int i=0; i<16; i++){
//        for (int j=0; j<256; j++) {
//            
//            LOG(INFO) << "i: " << i << " j: " << j << " value: " << rearranged_array[i][j];
//        }
//    }
//    
//    for (int i=0; i< 16; i++) {
//        rearranged_array[i].clear();
//    }
//    
//            
//    ResponsePar rp;
//    if(rp.read_response_par_file(responseParsFilename, "Q2")){
//        LOG(INFO) << "unable to read RESPONSE file.";
//        exit(EXIT_FAILURE);
//    }
//    rp.display();
//    
//    EffArea ea;
//    if(ea.read_effarea_file(effareaFilename, "Q2")){
//        LOG(INFO) << "unable to read EFFAREA file.";
//        exit(EXIT_FAILURE);
//    }
//    //ea.display();    
//
//
//    fitsfile *fptr;
//    fits_open_file(&fptr, effareaFilename.c_str(), READONLY, &status);
//    FitsFileInfo f;
//    f.get_fitsinfo(fptr);
//    
//    string hdunames_array[] = {"Primary", "Q0", "Q1", "Q2", "Q3"};
//    int hdutypes_array[]={0,2,2,2,2};
//    int ncols_array[]={0,5,5,5,5};
//    long nrows_array[]={0,4096, 4096, 4096,4096};
//    
//    vector<string> hdunames;
//    vector<int> hdutypes;
//    vector<int> ncols;
//    vector<long> nrows;
//    for(int i=0; i<5; i++){
//        hdunames.push_back(hdunames_array[i]);
//        hdutypes.push_back(hdutypes_array[i]);
//        ncols.push_back(ncols_array[i]);
//        nrows.push_back(nrows_array[i]);
//        
//    }
//    f.display_fitsinfo();   
//    f.validate_fitsfile(5, hdunames, hdutypes, &status, ncols, nrows);
//    
//    int hdutype, cols=0;
//    long rows=0;
//    f.get_hdu_info("Q0", hdutype, cols, rows);
//    LOG(INFO) << hdutype << "   " << cols << "   " << rows;
//
//    fits_close_file(fptr, &status);
//    LOG(INFO) << "We are here::";
//    
//    fitsfile* fptr;
//    fits_open_file(&fptr, "AS1A06_303T26_0000000006cztSS_el1.evt", READONLY, &status);
//    FitsFileInfo fevent;
//    fevent.get_fitsinfo(fptr);
//    fevent.display_fitsinfo();
//    
//
//
//    EventFileHandler eventFile;
//    eventFile.read_event_temp(fptr, 1);
//    fits_close_file(fptr, &status);    
    
/************CHECKING FOR BADPIXEL FILE*******************/    
//    Badpix bad;
//    string badpixFilename = caldb_full_path_generator("AS1cztbadpix20150310v01.fits");
//    vector<vector <unsigned char> > badpixmap;
//    unsigned char qmap2[128][128];
//    if(badpixFilename==""){
//        LOG(ERROR)<< "Not able to generate EBOUNDS path.";
//        return(EXIT_FAILURE);
//    }
//    if(bad.read_badpix_file(badpixFilename, "Q0")){
//        LOG(INFO) << "unable to read gains file.";
//        exit(EXIT_FAILURE);
//    }
//
//    badpixmap = bad.create_badPixMap(status);
//    for (int i=0; i<badpixmap.size(); i++){
//        for (int j=0; j<badpixmap.size(); j++){
//            qmap2[i][j] = badpixmap[i][j];
//        }
//    }
//    bad.read_badpix_file(badpixFilename, "Q1");
//    badpixmap = bad.create_badPixMap(status);
//    for (int i=0; i<badpixmap.size(); i++){
//        for (int j=0; j<badpixmap.size(); j++){
//            qmap2[i][j+64] = badpixmap[i][j];
//        }
//    }
//    bad.read_badpix_file(badpixFilename, "Q2");
//    badpixmap = bad.create_badPixMap(status);
//    for (int i=0; i<badpixmap.size(); i++){
//        for (int j=0; j<badpixmap.size(); j++){
//            qmap2[i+64][j+64] = badpixmap[i][j];
//        }
//    }
//    bad.read_badpix_file(badpixFilename, "Q3");
//    badpixmap = bad.create_badPixMap(status);
//    for (int i=0; i<badpixmap.size(); i++){
//        for (int j=0; j<badpixmap.size(); j++){
//            qmap2[i+64][j] = badpixmap[i][j];
//        }
//    }
    
//    Badpix bad0, bad1, bad2, bad3;
//    string badpixFilename = caldb_full_path_generator("AS1cztbadpix20150310v01.fits");
//    vector< vector <unsigned char> > fullbadpixmap;
//    fullbadpixmap = create_full_badPixMap(badpixFilename, status);
//    if (status) {
//        LOG(ERROR) << "Error in generating full badpixel map for " << badpixFilename;
//    }
//    char qmap[128][128];
//    char qmap2[128*128];
////    bad0.read_badpix_file(badpixFilename, "Q0");
////    bad1.read_badpix_file(badpixFilename, "Q1");
////    bad2.read_badpix_file(badpixFilename, "Q2");
////    bad3.read_badpix_file(badpixFilename, "Q3");
////    
////    fullbadpixmap = create_full_badPixMap(bad0, bad1, bad2, bad3, status);
//    for (int i=0; i<128; i++){
//        for (int j=0; j<128; j++){
//            qmap[i][j]=fullbadpixmap[i][j];
//            qmap2[i*128+j]=fullbadpixmap[i][j];
//        }
//    }
//    fitsfile *fptr;
//    fits_create_file(&fptr, "test.fits", &status);
//    fits_open_file(&fptr, "test.fits", READWRITE, &status);
//    
//    int bitpix = SHORT_IMG;
//    long naxes[2];
//    naxes[0] = 128;
//    naxes[1] = 128;
//    int naxis = 2;
//    long fpixel[2];
//    fpixel[0]=fpixel[1]=1;
//    fits_create_img(fptr, bitpix, naxis, naxes, &status);
//    fits_movabs_hdu(fptr,1,NULL,&status);
//    fits_write_pix(fptr, TBYTE, fpixel, 16384, qmap2, &status);
//    //fits_write_img(fptr, TBYTE, 1, 16384, qmap, &status);
//    fits_close_file(fptr, &status);
    
    
    /*************Checking for Attitude file*/
//    Q quat, inverse_quat;
//    
//    Axis axis, rot_axis;
//    float RQ[3][3];
//    float transRQ[3][3];
//    float axis3_1[3][1];
//    float rotated_axis[3][1];
//
//
//    float q1, q2, q3, q4=0.0;
//    
//    quat.readQ();
//    quat.readAxisAngle();
//    axis.x = quat.x;
//    axis.y = quat.y;
//    axis.z= quat.z;
//    
//    Inverse(quat, inverse_quat);
//    q1=quat.q1;
//    q2=quat.q2;
//    q3=quat.q3;
//    q4=quat.q4;
//    RQ[0][0] = q1 * q1 - q2 * q2 - q3 * q3 + q4*q4;
//    RQ[0][1] = 2 * (q1 * q2 + q3 * q4);
//    RQ[0][2] = 2 * (q1 * q3 - q2 * q4);
//    RQ[1][0] = 2 * (q1 * q2 - q3 * q4);
//    RQ[1][1] = -q1 * q1 + q2 * q2 - q3 * q3 + q4*q4;
//    RQ[1][2] = 2 * (q2 * q3 + q1 * q4);
//    RQ[2][0] = 2 * (q1 * q3 + q2 * q4);
//    RQ[2][1] = 2 * (q2 * q3 - q1 * q4);
//    RQ[2][2] = -q1 * q1 - q2 * q2 + q3 * q3 + q4*q4;
//    
//    //taking transpose
//    for(int i=0; i<3; i++){
//        for(int j=0; j<3; j++){
//            transRQ[i][j]=RQ[j][i];
//        }
//    }
//
//    //transpose taken
//    
//    // taking matrix product of Direction Cosine Matrix and Axis.
//    axis3_1[0][0]=quat.x;
//    axis3_1[1][0]=quat.y;
//    axis3_1[2][0]=quat.z;
//
//    float sum;
//    for (int i = 0; i < 3; i++) {
//        for (int j = 0; j < 1; j++) {
//            sum = 0;
//            for (int k = 0; k < 3; k++) {
//                sum = sum + (transRQ[i][k] * axis3_1[k][j]);
//                rotated_axis[i][j] = sum;
//            }
//        }
//    }
//    //matrix_product(transRQ, axis3_1, rotated_axis, 3,3,1);
//    
//    rotate(axis, quat, rot_axis);
//    cout << "x,y,z " << rot_axis.x << " " << rot_axis.y << " " << rot_axis.z << endl;
//    
//    
//    float dec = (asin(rot_axis.z))*180/M_PI;
//    //float ra = (acos(rot_axis.x/sqrt(pow(rot_axis.x,2) + pow(rot_axis.y,2))))*180/M_PI;
//    float ra;
//    float temp = atan2(rot_axis.y, rot_axis.x);
//    temp = temp*180/M_PI;
//    if (temp < 0 ) { 
//        ra = temp+360;}
//    else {
//        ra = temp;
//    }
//    cout << "ra, dec :" << ra << " , " << dec << endl;
//    
//    float dec_new = (asin(rotated_axis[2][0]))*180/M_PI;
//    //float ra = (acos(rot_axis.x/sqrt(pow(rot_axis.x,2) + pow(rot_axis.y,2))))*180/M_PI;
//    float ra_new;
//    float temp_new = atan2(rotated_axis[1][0], rotated_axis[0][0]);
//    temp_new = temp_new*180/M_PI;
//    if (temp_new < 0 ) { 
//        ra_new = temp_new+360;}
//    else {
//        ra_new = temp_new;
//    }
//    cout << "ra_new, dec_new :" << ra_new << " , " << dec_new << endl;
//
//    
    

    ////////////////////////////
    
    
    //Checking the functioning of getxy in utils.cpp
//    int quadID, detID, pixID=0;
//    unsigned char x;
//    unsigned char y;
//    short int** image;
//    image = new short int*[128];
//    for(int i=0; i<128; i++){
//        image[i] = new short int[128];
//    }
//    
//    for(int i=0; i<128; i++){
//        for( int j=0; j<128; j++) {
//                image[i][j] = 0;
//        }
//    }
//    for (quadID = 0; quadID < 4; quadID++) {
//        for (detID = 0; detID < 16; detID++) {
//            for (pixID = 0; pixID < 256; pixID++) {
//                getxy(quadID, detID, pixID, &x, &y);
//                LOG(INFO) << "QUADID: " << quadID << " DETID: " << detID <<
//                        " PIXID: " << pixID << " X: " << int(x) << " Y: " << int(y);
//               image[y][x] = (short int) quadID;
//                LOG(INFO) << image[y][x];
//
//            }
//        }
//    }
//        
//    fitsfile *fptr;
//    fits_create_file(&fptr, "test.fits", &status);
//    fits_open_file(&fptr, "test.fits", READWRITE, &status);
//
//    int bitpix = SHORT_IMG;
//    long naxes[2];
//    naxes[0] = 128;
//    naxes[1] = 128;
//    long naxis = 2;
//    long fpixel[2];
//    fpixel[0] = fpixel[1] = 1;
//    fits_create_img(fptr, bitpix, naxis, naxes, &status);
//    fits_movabs_hdu(fptr, 1, NULL, &status);
//    //fits_write_pix(fptr, TBYTE, fpixel, 16384, qmap2, &status);
//    fits_write_img(fptr, TSHORT, 1, 16384, image[0], &status);
//    fits_close_file(fptr, &status);

    
    
    /////// CHECKING LEVEL-1 HANDLER FOR MKF FILES.
//    Mkf mkf;
//    fitsfile *fptr;
//    vector <double> time;
//    vector <float> roll_RA;
//    vector <float> roll_DEC;
//    vector <float> pitch_RA;
//    vector <float> pitch_DEC;
//    vector <float> yaw_RA;
//    vector <float> yaw_DEC;
//    vector <float> temp;
// 
//    
//    string mkfFile = "AS1A06_303T26_0000000006czt_level1.mkf";
//    fits_open_file(&fptr, (char*) mkfFile.c_str(), READONLY, &status);
//    mkf.read_mkf_time(fptr);
//    mkf.read_mkf_rpy(fptr);
//    time = mkf.get_vecTime();
//    roll_DEC = mkf.get_vecRollDEC();
//    roll_RA = mkf.get_vecRollRA();
//    
//    for(int i=0; i<1000; i++){
//        LOG(INFO) << "TIME: " << time[i] << " RA: " << roll_RA[i] << " DEC: " << roll_DEC[i];
//    }
    
//    double RAobject, DECobject;
//    double thetaX, thetaY;
//    string aspectFilename="AS1A06_303T26_0000000006cztM0_level2.aspect_Q1";
//    to_RA_DEC(aspectFilename, 1, 1, RAobject, DECobject);
//    to_thetaX_thetaY(aspectFilename, RAobject,DECobject , thetaX, thetaY);
//    LOG(INFO)<< RAobject << "    " << DECobject;
//    
//    LOG(INFO)<< "THETAX, THETAY::" << thetaX << ", " << thetaY;
    
//    CztiCameraGeometry par;
//    ExposureTable expTab;
//    par.define_camera_geometry();
//    par.write_camera_geometry_file("test.geom", "cztiGeometryTemplate");
//    
//    vector <int> detX;
//    calculate_pixexposure(10, 90, 45, "test.geom", expTab);
//
//    
//    string up= "CALDB";
//    string low;
//    low.resize(up.size());
//    transform(up.begin(), up.end(), up.begin(), ::tolower);
//    cout << up << "    " << low << endl;
//    char tame[100];
//    time_t now = time(0);
//    tm* gmtm = gmtime(&now);
//    
//    sprintf(tame, "%d%02d%02d", gmtm->tm_year+1900, gmtm->tm_mon+1, gmtm->tm_mday);
//    cout << tame << endl;
//    
//    CztDetectorGeometry det;
//    vector<float> xdet0, ydet0;
//    float areaGeom;
//    
//    det.init_detectors();
//    xdet0 = det.get_xdet0();
//    ydet0 = det.get_ydet0();
//    areaGeom = det.get_geometric_area();
//    
//    det.display_detector_coordinates();
//    //det.display_pixel_information("area");
//    //det.display_pixel_information("pxloc",1.0,1.0);
//
//    double a=0.2;
//    cout << pow(a,2);
//    cout << log10(10);
//    
//    ExposuremapFileHandler expfile;
//    expfile.write_exposure_map_file("out.exp", "expmapTemplate");
    
    
//__________MKF TESTING__________________
//    Mkf mkf;
//    fitsfile *fptr;
//    vector<double> vecTime;
//    vector<double> uniqueTime;
//    long i=0;
//    long nrows, nrowsUnique;
//
//    int mycount=0;
//    fits_open_file(&fptr, "AS1A01_003T10_0000000010czt_level1.mkf", READONLY, &status);
//    mkf.read_mkf_time(fptr);
//
//    mkf.check_time_repetition();
    
    //____________________________________
    
//__________NEW GAIN OFFSET READING___________
//    GainOffset go;
//    go.read_gainoffset_file("AS1cztgain20150526v01.fits", "Q0");
//    go.display();
//    
//    EventFileHandler evt;
//    fitsfile *fptr;
//    fits_open_file(&fptr, "AS1A01_003T10_0000000010cztSS_level1.evt", READONLY, &status);
//        if (status) {
//            LOG(ERROR) <<"Error in opening TEMP extension file.";
//            fits_report_error(stderr, status);
//            return (EXIT_FAILURE);
//        }
//    evt.read_event_temp(fptr, 0);
//    vector <double> utctime;
//    vector <double> localtime;
//    vector < vector <float> > temps;
//    long nrows=0;
//    long i=0;
//    
//    utctime = evt.get_vecTimeTemp();
//    localtime = evt.get_vecLocalTimeTemp();
//    temps = evt.get_vecTempQuadrant();
//    
//    nrows=localtime.size();
//    
//    cout<< setw(10) << "TIME" << setw(30) << "TEMPERATURE"<< endl;
//    for(i=0; i<nrows; i++){
//        cout << setw(10) << utctime[i];
//        for(j=0; )
//    }

//___________NORMALIZED EFFECTIVE AREA CALCULATION___________
//    long ix, iy=0;
//    EffArea effa;
//    float maxEffarea;
//    vector <vector <float> > normalizedEffArea;
//    effa.read_effarea_file("AS1czteff_area20150617v01.fits","Q0");
//    effa.calculate_normalized_effarea(6,1000);
//    normalizedEffArea = effa.get_normalized_effarea();
//    maxEffarea=effa.get_max_effarea();
//    for(ix=0; ix<64; ix++){
//        for(iy=0; iy<64; iy++){
//            LOG(INFO) << ix << "  " << iy << "   "<< normalizedEffArea[ix][iy] << endl;
//        }
//    }
//    LOG(INFO)<< "max effective area : "<< maxEffarea;
//    
//___________________DPI COMPUTATION___________________________
//    DPI dpi;
//    EventFileHandler evt;
//    string eventFilename="m0.eadded";
//    int quad_no=0;
//    double minTime;
//    double maxTime;
//    float minEnergy;
//    float maxEnergy;
//    vector <double> cztTime;
//    vector <float> cztEnergy;
//    fitsfile *fevt; //event file pointer
//    fitsfile *fdpi;
//    //Opening event file
//    fits_open_file(&fevt, (char*) eventFilename.c_str(), READONLY, &status);
//    if (status) {
//        LOG(ERROR) << "Error in opening input event file: "<< eventFilename;
//        fits_report_error(stderr, status);
//        return (EXIT_FAILURE);
//    }
//    
//    if(evt.read_quad_extension(fevt, quad_no)){
//        LOG(ERROR)<<"Error in reading event data for Quadrant " << quad_no << " from event file " << eventFilename;
//        return EXIT_FAILURE;
//    }
//    fits_close_file(fevt, &status);
//    
//    cztTime = evt.get_vecTimeQuad();
//    cztEnergy = evt.get_vecEnergyQuad();
//    compute_min_and_max(cztTime.begin(), cztTime.end(), minTime, maxTime);
//    compute_min_and_max(cztEnergy.begin(), cztEnergy.end(), minEnergy, maxEnergy);
//    
//    LOG(INFO)<< "MINIMUM TIME:" << setprecision(20) << minTime;
//    LOG(INFO)<< "MAXIMUM TIME:" << setprecision(20) << maxTime;
//    LOG(INFO)<< "MINIMUM ENERGY:" << minEnergy;
//    LOG(INFO)<< "MAXIMUM ENERGY:" << maxEnergy;
//    
//    
//    if(dpi.create_dph2d(eventFilename, 0, 10.0, maxEnergy, minTime, maxTime)){
//        LOG(ERROR)<<"Error in creating DPH.";
//        return EXIT_FAILURE;
//    }
//    
//    string dpiTemp="dpiTemplate";
//    
//    create_empty_fitsfile("out.dpi", dpiTemp);
//    
//    fits_open_file(&fevt, (char*) eventFilename.c_str(), READONLY, &status);
//    fits_open_file(&fdpi, "out.dpi", READWRITE, &status);
//    
//    vector <string> vecKeywords;
//    vecKeywords.push_back("DATE-OBS");
//    vecKeywords.push_back("TIME-OBS");
//    if (copyUserKeyWords(fevt, fdpi, "Primary", "Primary", vecKeywords)) { //copying keywords from input file
//        LOG(ERROR) << "***Error in copying keywords***";
//    }
//    
//    fits_close_file(fdpi, &status);
    
    // _______ DPI COMPUTATION USING DPI CLASS _____________
//    DPI dpi;
//    EventFileHandler evt;
//    string eventFilename = "m0.eadded";
//    string dpiFilename = "m0.dpi";
//    string dpiTemp="dpiTemplate";
//    int quad_no=0;
//    double minTime;
//    double maxTime;
//    float minEnergy;
//    float maxEnergy;
//    vector <double> cztTime;
//    vector <float> cztEnergy;
//    fitsfile *fevt; //event file pointer
//    fitsfile *fdpi;
//    //Opening event file
//    fits_open_file(&fevt, (char*) eventFilename.c_str(), READONLY, &status);
//    if (status) {
//        LOG(ERROR) << "Error in opening input event file: "<< eventFilename;
//        fits_report_error(stderr, status);
//        return (EXIT_FAILURE);
//    }
//    
//    if(evt.read_quad_extension(fevt, quad_no)){
//        LOG(ERROR)<<"Error in reading event data for Quadrant " << quad_no << " from event file " << eventFilename;
//        return EXIT_FAILURE;
//    }
//    fits_close_file(fevt, &status);
//    
//    cztTime = evt.get_vecTimeQuad();
//    cztEnergy = evt.get_vecEnergyQuad();
//    compute_min_and_max(cztTime.begin(), cztTime.end(), minTime, maxTime);
//    compute_min_and_max(cztEnergy.begin(), cztEnergy.end(), minEnergy, maxEnergy);
//    
//    LOG(INFO)<< "MINIMUM TIME:" << setprecision(20) << minTime;
//    LOG(INFO)<< "MAXIMUM TIME:" << setprecision(20) << maxTime;
//    LOG(INFO)<< "MINIMUM ENERGY:" << minEnergy;
//    LOG(INFO)<< "MAXIMUM ENERGY:" << maxEnergy;
//    
//    if(dpi.create_dpi_file(dpiFilename, dpiTemp, eventFilename)){
//        LOG(ERROR)<<"Error in creating DPI file.";
//        return EXIT_FAILURE;
//    }
//
////    //Opening event file
////    fits_open_file(&fevt, (char*) eventFilename.c_str(), READONLY, &status);
////    if (status) {
////        LOG(ERROR) << "Error in opening input event file: " << eventFilename;
////        fits_report_error(stderr, status);
////        return (EXIT_FAILURE);
////    }
//    //Opening dpi file
//    fits_open_file(&fdpi, (char*) dpiFilename.c_str(), READWRITE, &status);
//    if (status) {
//        LOG(ERROR) << "Error in opening output dph file: " << dpiFilename;
//        fits_report_error(stderr, status);
//        return (EXIT_FAILURE);
//    }
//    dpi.create_dph2d(eventFilename, quad_no, minEnergy, maxEnergy, minTime, maxTime);
//    dpi.add_quad_to_fulldpih(quad_no, "DPH");
//    if(dpi.write_dpih_quad(dpiFilename, "DPH", "Q0")){
//        LOG(ERROR)<<"Error in writing DPH file";
//        return (EXIT_FAILURE);
//    }
//
//
//    
//    dpi.create_dph2d(eventFilename, 3, minEnergy, maxEnergy, minTime, maxTime);
//    dpi.add_quad_to_fulldpih(3, "DPH");
//    if (dpi.write_dpih_quad(dpiFilename, "DPH", "Q3")) {
//        LOG(ERROR) << "Error in writing DPH file";
//        return (EXIT_FAILURE);
//    }
//    
//    LOG(INFO)<< "We are here::::::";
//   if(dpi.write_dpih_full(dpiFilename, "DPH", "FULL_DPI")){
//       LOG(ERROR) << "Error in writing FULL DPH";
//       return EXIT_FAILURE;
//   }



//    fits_close_file(fevt, &status);
//    float RA;
//    float DEC;
//    float nX, nY, nZ;
//    nX=0.9860607;
//    nY=-0.03743543;
//    nZ=0.1621199;
//    to_RA_DEC(nX, nY, nZ, RA, DEC);
//    LOG(INFO) << "RA, DEC :: " << RA << ", "<< DEC;
//    to_nX_nY_nZ(RA, DEC, nX, nY, nZ);
//    LOG(INFO) << "nX, nY, nZ :: " << nX << ", "<< nY << ", " << nZ;   
//    
//
//    LOG(INFO) << "Checking the functionality of coordinate conversions.";
//    double thetaX, thetaY;
//    double RAobject, DECobject;
//    cout << "Enter thetaX : ";
//    cin >> thetaX;
//    cout << "Enter thetaY : ";
//    cin >> thetaY;
//    to_RA_DEC("out.aspect_Q0", thetaX, thetaY , RAobject, DECobject);
//    to_thetaX_thetaY("out.aspect_Q0", RAobject, DECobject, thetaX, thetaY);
//    LOG(INFO) << "ThetaX, ThetaY ::" <<  thetaX << ", " << thetaY;
//    
//    return 0;.
    
    /********Open fraction and tantalum mask**********************/
//    TanMaskGeometry msk;
//    TanMaskWeighting mskw(0.05, 0.05);
//    vector <vector <unsigned char> > fullmask;
//    vector <vector <unsigned char> > fullfinermask;
//    vector <vector <unsigned char> > q0mask;
//    vector <vector <unsigned char> > q0Unmask;
//    vector <vector <unsigned char> > q1Unmask;
//    vector <vector <unsigned char> > q2Unmask;
//    vector <vector <unsigned char> > q3Unmask;
//    vector <vector <unsigned char> > q2mask;
//    vector <vector <unsigned char> > detMask;
//    vector <vector <float> > ofDet;
//    vector <vector <float> > ofShiftedDet;
//    vector <vector <float> > ofQ0;
//    
//    
//    if(msk.read_64x64_mask("CZTIMask64x64.fits")){
//        LOG(ERROR) << "Error in reading 64x64 mask file.";
//        return EXIT_FAILURE;
//    }
//    if(msk.read_compressed_mask("AS1cztmask_pattern20150526v01.fits")){
//        LOG(ERROR) << "Error in reading finer mask file.";
//        return EXIT_FAILURE;
//    }
////    msk.get_full64x64_mask();
//    msk.get_full_finer_mask();
////    fullmask = msk.get_fullMask();
//    fullfinermask = msk.get_fullUnMask();
//    q0Unmask = msk.get_q0UnMask();
//    q1Unmask = msk.get_q1UnMask();
//    q2Unmask = msk.get_q2UnMask();
//    q3Unmask = msk.get_q3UnMask();
//    q0mask = msk.get_q0Mask();
//    q2mask = msk.get_q2Mask();
//    //write_img(fullmask, "fullmask.img", BYTE_IMG, TBYTE);
//    write_img(q2mask, "q2.img", BYTE_IMG, TBYTE);
//    write_img(q0Unmask, "q0finer.img", BYTE_IMG, TBYTE);
//    write_img(q1Unmask, "q1finer.img", BYTE_IMG, TBYTE);
//    write_img(q2Unmask, "q2finer.img", BYTE_IMG, TBYTE);
//    write_img(q3Unmask, "q3finer.img", BYTE_IMG, TBYTE);
//    write_img(fullfinermask, "full_finer_mask.img", BYTE_IMG, TBYTE);
//    detMask = get_detector_mask(2, 15, msk,  status);
//    write_img(detMask, "DETMASK.img", BYTE_IMG, TBYTE);
//    mskw.calculate_open_fraction_det(detMask);
//    ofDet = mskw.get_openfraction_det();
//    write_img(ofDet, "ofDET.img", FLOAT_IMG, TFLOAT);
//    
//    detMask = get_shifted_detector_mask(2, 15, msk, 1 , 1, status);
//    write_img(detMask, "SHIFTDETMASK.img", BYTE_IMG, TBYTE);
//    mskw.calculate_open_fraction_det(detMask);
//    ofShiftedDet = mskw.get_openfraction_det();
//    write_img(ofShiftedDet, "ofShiftedDET.img", FLOAT_IMG, TFLOAT);
//    if(status){
//        LOG(ERROR) << "Error in getting detector mask.";
//        return (EXIT_FAILURE);
//    }
//    
//    mskw.compute_open_fraction_quad(0, msk);
//    ofQ0 = mskw.get_openfractionQ0();
//    write_img(ofQ0, "ofq0.img", FLOAT_IMG, TFLOAT);
//    
//
//    exit(1);
    
    /*****************************DETECTOR GEOMETRY***************************************/
//    CztDetectorGeometry det;
//    det.init_detectors();
//    det.display_detector_coordinates();
//    MaskOpenFractionTable mof;
//    vector <vector <float> > fullOpenfraction;
//    
//    TransMask tm;
//    tm.set_fullUnMask(fullfinermask);
//    tm.compute_mask_openfraction(det,20,60);
//    tm.generate_full_oftrans_image();
//    fullOpenfraction = tm.get_full_oftrans();
//    write_img(fullOpenfraction, "fof.img", FLOAT_IMG, TFLOAT);
    
    
    /**************ASPECT FILE*****************************************/
//    AspectFileHandler asp;
//    vector <string> fnames;
//    fnames.push_back("Q0_output.aspect");
//    fnames.push_back("Q1_output.aspect");
//    fnames.push_back("Q2_output.aspect");
//    fnames.push_back("Q3_output.aspect");
//    asp.read_multiple_aspect_files(fnames);
//    DLOG(INFO) << "NUMBER OF ROWS READ: " << asp.find_nrows();
    
    /**********L1evt Handler******************************************/
//    int i=0, j=0;
//    string infile="AS1A01_500T25_0000000004cztM4_level1.fits";
//    L1evtHandler l1evt(infile);
//    L1evtData l1data, l1data2, l1data3, l1data4;
//    unsigned char ** tempArray;
//    
//    EventFileHandler evtfile;
//    evtfile.create_l2event_file("out.fits", "evtTemplate", infile);
//    //Reading Quad 1
//    l1evt.read_l1evt_file_pkts("CZT_QUAD1");
//    int height = l1evt.get_pktArrayRows();
//    l1data.set_pktArray(l1evt.pktArray, height);
//    l1data.segregate_data();
//    l1data.display_l1data_info();
//    l1data.extract_data();
//    evtfile.write_l2event_file("out.fits", l1data);
//    l1data.reset();
//    l1evt.clear_pktArray();
//
//    //Reading Quad 2
//    l1evt.read_l1evt_file_pkts("CZT_QUAD2");
//    height = l1evt.get_pktArrayRows();
//    l1data2.set_pktArray(l1evt.pktArray, height);
//    l1data2.segregate_data();
//    l1data2.display_l1data_info();  
//    l1data2.extract_data();
//    evtfile.write_l2event_file("out.fits", l1data2);
//    l1data2.reset();
//    l1evt.clear_pktArray();
// 
//    //Reading Quad 3
//    l1evt.read_l1evt_file_pkts("CZT_QUAD3");
//    height = l1evt.get_pktArrayRows();
//    l1data3.set_pktArray(l1evt.pktArray, height);
//    l1data3.segregate_data();
//    l1data3.display_l1data_info();  
//    l1data3.extract_data();
//    evtfile.write_l2event_file("out.fits", l1data3);
//    l1data3.reset();
//    l1evt.clear_pktArray();
//    //Reading Quad 3
//    l1evt.read_l1evt_file_pkts("CZT_QUAD4");
//    height = l1evt.get_pktArrayRows();
//    l1data4.set_pktArray(l1evt.pktArray, height);
//    l1data4.segregate_data();
//    l1data4.display_l1data_info();  
//    if(l1data4.extract_data()){
//        LOG(ERROR) << "Error extracting data for q3";
//        return EXIT_FAILURE;
//    }
//    cout << "Size: " << l1data4.quadEventData.alpha.size()<< endl;
//    evtfile.write_l2event_file("out.fits", l1data4);
//    l1data4.reset();
//    l1evt.clear_pktArray();
//    
//    evtfile.create_l2hdr_file("out.hdr", "hdrTemplate", infile);


/****************READING MKF THRESHOLDS****************************/
//    string mkffilename;
//    string mkfthrfilename;
//    int i=0;
//    Mkf mkf;
//    vector <double> tstart, tstop;
//    mkffilename="AS1A01_300T25_0000000011czt_level1.mkf";
//    mkfthrfilename="/home/tanul/Documents/doctanul/projects/CZTI/czti/paramfiles/mkfThresholds.txt";
//    mkf.read_mkf_file(mkffilename);
//    mkf.get_filtered_gti(mkfthrfilename, tstart, tstop);
//    
//    for(i=0; i<tstart.size(); i++){
//        cout << endl << setprecision(15)<< tstart[i] << "  -  " << tstop[i];
//    }
    
/***********BADPIXEL FILE CREATION AND HANDLING*****************/
//    Badpix bp;
//    bp.create_L2badpix_file("out.badpix", BADPIXTEMPLATE);
    
//    float A[10] = {1,1,1,1,1,1,1,1,1,1};
//    float B[10] = {1.2,2,2.1,2.2,3,3.05,3.1,2.0,1.5,1.5};
//    double coef[2];
//    fitPolynomial(A,B, 100, coef, 1);
//    cout << coef[0] << " " << coef[1];
    
/***************EFFECTIVE AREA FILE HANDLING*******************/
//    EffArea effarea;
//    vector <vector <float> > normalized_effarea;
//    string effareaFilename = "AS1czteff_area20150617v01.fits";
//    effarea.read_effarea_file(effareaFilename, "Q0");
//    effarea.calculate_full_normalized_effarea(effareaFilename, 10.0, 10.0);
//    normalized_effarea = effarea.get_fullnormalized_effarea();
//    write_img(normalized_effarea, "normarea.img", FLOAT_IMG, TFLOAT);
//
//    TanMaskGeometry msk;
//    vector < vector <unsigned char> > fullfinermask;
//    ShadowTable shadowtab;
//    
//    LOG(INFO) << "Reading compressed mask file to create 18200x18200 full uncompressed mask.";
//    //Getting full finer mask
//    if (msk.read_compressed_mask("AS1cztmask_pattern20150526v01.fits")) {
//        return EXIT_FAILURE;
//    }
//    if (msk.get_full_finer_mask()) {
//        LOG(ERROR) << "Error in generating full finer mask from individual quadrant masks.";
//        return EXIT_FAILURE;
//    }
//
////    //storing full finer mask
//    fullfinermask = msk.get_fullUnMask();
////        write_img(fullfinermask, "fullfinermask.img", BYTE_IMG, TBYTE);
//    //msk.reset_full_uncomressed_mask();    
//    Shadow shadow("AS1cztcamerageo20150629v01.fits");
//    shadow.display();
//    shadow.check_shadow_par();
//    shadow.compute_shadow(normalized_effarea, fullfinermask);
//    shadowtab = shadow.get_shadow_table();
//    write_img(shadowtab.shadow2D, "shadow.img", FLOAT_IMG, TFLOAT);
    
/******************SHADOW FILE HANDLER******************************/
//    ShadowFileHandler sfh;
//    string shadowFilename="shadow1";
//    string extName="SHADOW";
//    string templateFullPath="";
//    vector<string> extnames;
////    extnames.push_back("sh1");
////    extnames.push_back("sh2");
////    extnames.push_back("sh3");
////    extnames.push_back("sh4");
//    
////    sfh.create_shadow_file(outfilename, SHADOWTEMPLATE, extnames);
//
//    fitsfile *fshadow;
//
//    fits_open_file(&fshadow, (char*) shadowFilename.c_str(), READWRITE, &status);
//    if (status) {
//        LOG(ERROR) << "Error in opening shadow file " << shadowFilename;
//        fits_report_error(stderr, status);
//        return (EXIT_FAILURE);
//    }
//
//    fits_movnam_hdu(fshadow, BINARY_TBL, (char*) extName.c_str(), NULL, &status);
//    if (status) {
//        LOG(ERROR) << "Error in moving to HDU " << extName << " in shadow file " << shadowFilename;
//        fits_report_error(stderr, status);
//        return (EXIT_FAILURE);
//    }
//    int RA;
//    fits_read_key(fshadow,TDOUBLE,"RA", &RA, NULL, &status);
//    LOG(INFO) << "RA " << RA;
//    
//    return 0;
//}
    
/**************STAR CATALOG CLASS************************/
//    StarCatalog scat;
//    Catalog cat;
//    scat.read_catalog_file("BAT_58m_catalog_100924_new.fits.gz",2);
//    scat.find_sources_in_FOV(cat, 0.0, 0.0, 0.0, 6.0*TORAD, 6.0*TORAD);
//    LOG(INFO) << cat.get_nsources() << " " << cat.thetaxr[0];
    //system((char*) command.c_str());

/*******************BADPIX CALDB CLASS ********************/
//    Badpix badpix;
//    BadpixTable btable0, btable1, btablef;
//    vector <BadpixTable> vecbpix;
//
//    badpix.read_badpix_file("AS1cztbadpix20150526v01.fits");
//    btable0 = badpix.get_badpix_table(0);
//    btable1 = badpix.get_badpix_table(1);
//    btable1.set_quadID(0);
//    btable0.generate_badpix_info();
//    //btable0.display();
//    btable1.generate_badpix_info();
//    //btable1.display();
//    vecbpix.push_back(btable0);
//    vecbpix.push_back(btable1);
//    
//    try{
//        btablef = flag_badpix_quadrant(vecbpix);
//    }
//    catch(string s){
//        LOG(ERROR) << s;
//    }
//    
//    for(int i=0; i<btablef.get_nrows(); i++){
//        cout<< (int)btable0.pixFlag[i] << (int)btable1.pixFlag[i]  << (int)btablef.pixFlag[i] << endl;
//    }
    
//    Badpix badpix, badpix2;
//    vector <BadpixTable> bptables;
//    vector <string> badpixfiles;
//    vector < vector <unsigned char> > badpixMap;
//    vector < vector <unsigned char> > badpixMap2;
////    badpixfiles.push_back("AS1cztbadpix20150526v01.fits");
////    badpixfiles.push_back("testbadpix.fits");
////
////    //badpixfiles.push_back("test2.badpix");
////    try{
////        bptables = flag_badpix_files(badpixfiles);
////    }
////    catch(string s){
////        LOG(ERROR) << s;
////        return EXIT_FAILURE;
////    }
////    try{
////        badpix.set_badpix_tables(bptables);
////    }
////        catch(string s){
////        LOG(ERROR) << s;
////        return EXIT_FAILURE;
////    }
////    badpix.create_badPixMap();
////    badpixMap = badpix.get_badpix_map();
////    badpix.create_L2badpix_file("test.badpix", BADPIXTEMPLATE);
////    badpix.write_L2badpix_file("test.badpix");
//    
//    Badpix badpix2;
//    badpix2.read_badpix_file("test.badpix");
//    badpixMap2 = badpix2.get_badpix_map();
//    write_img(badpixMap2, "out.img", BYTE_IMG, TBYTE);
    

/**********************************************************/
//    string camgeofile = caldb_full_path_generator("AS1cztcamerageo20150629v01.fits");
//    TanMaskGeometry msk;
//    vector < vector <unsigned char> > fullfinermask;
//    vector < vector <float> > of1_300;
//    ExposureTable exptable;
//    string eboundsfilename = caldb_full_path_generator("AS1cztebounds20150526v01.fits");
//    string eventFilename = "out.eadded";
//    string maskFilename = caldb_full_path_generator("AS1cztmask_pattern20150526v01.fits");
//    string badpixFilename = caldb_full_path_generator("AS1cztbadpix20150526v01.fits");
//    string effAreafilename = caldb_full_path_generator("AS1czteff_area20150617v01.fits");
//    ExposuremapFileHandler expmap;
//    time_t st, et;
//    vector <int> quadsToProcess;
//    quadsToProcess.push_back(0);
//    quadsToProcess.push_back(1);
//    quadsToProcess.push_back(2);
//    quadsToProcess.push_back(3);
//
//    st = time(NULL);
//    LOG(INFO) << "Reading compressed mask file to create 18200x18200 full uncompressed mask.";
//    //Getting full finer mask
//    if (msk.read_compressed_mask(maskFilename)) {
//        return EXIT_FAILURE;
//    }
//    if (msk.get_full_finer_mask()) {
//        LOG(ERROR) << "Error in generating full finer mask from individual quadrant masks.";
//        return EXIT_FAILURE;
//    }
//
//    fullfinermask = msk.get_fullUnMask();
//    write_img(fullfinermask, "fullmask", BYTE_IMG, TBYTE);
//    msk.reset_full_uncomressed_mask();
//    create_exposure_array(0.0, 0.0, camgeofile, fullfinermask, exptable, 1, 10, eboundsfilename);//,1.0,55.0,10); 
//    //create_exposure_array(0.0, 0.0, camgeofile, fullfinermask, exptable, 1, 10, "",1.0,250.0,20); 
//    calculate_renormalized_weights(exptable, badpixFilename, 4, quadsToProcess, effAreafilename,1);
//    expmap.set_exposuretable(exptable);
//    
//    expmap.write_exposure_array_file("expar2", "ExparrayTemplate");
//    et = time(NULL);
//    
//    LOG(INFO) << "Time taken " << et-st;
/**********************************************************/
    
/********SPECTRUM MAKING***************************/
    
//    ExposuremapFileHandler efile;
//    ExposureTable exptable;
//    
//    efile.read_exposure_array_file("expar");
//
//    exptable = efile.get_exposure_table();
//        vector <int> quadsToProcess;
//        quadsToProcess.push_back(0);
//        quadsToProcess.push_back(1);
//        quadsToProcess.push_back(2);
//        quadsToProcess.push_back(3);
//    vector <double> tstart;
//    vector <double> tstop;
//    vector <float> estart;
//    vector <float> estop;
//    
//    SpectrumLc spec, lc;
//    SpectrumFileHandler sf;
//    LightCurveFileHandler lf;
//    try{
//        spec.bin_spectrum(eventFilename, tstart, tstop, estart, estop, quadsToProcess, exptable);
//        lc.bin_lc(eventFilename,tstart, tstop, estart, estop, quadsToProcess, exptable, 2.0);
//    }
//    catch(ErrorHandler errHandler){
//        logError(errHandler);
//        return EXIT_FAILURE;
//    }
//    
//    sf.set_spectrum(spec);
//    lf.set_lc(lc);
//    
//    if(lf.write_lc_file("out.lc", "lcTemplate")){
//        LOG(ERROR) << "Error in writing lc file.";
//        return EXIT_FAILURE;
//    }
//    if(sf.write_spectrum_file("out.spec", "spectrumTemplate")){
//        LOG(ERROR) << "Error in writing spectrum file.";
//        return EXIT_FAILURE;
//    }
    
    /**********************************************************************/
//    string expArrFilename = "expar";
//    ExposuremapFileHandler expfile;
//    ExposureTable exptab;
//    expfile.read_exposure_array_file(expArrFilename);
//    exptab = expfile.get_exposure_table();
//    int i=0;
//    
//    cout << exptab.openfracArray.size();
//    
//    for(i=0; i<exptab.openfracArray[0].size(); i++){
//        LOG(INFO) << i << " -> " << exptab.openfracArray[0][i];
//    }

    //************************************************

    //Checking working of mode evaluation
//    vector <float> temperatures;
//    map <float, long> modemap;
//    float mode;
////    temperatures.push_back(100.0);
////    temperatures.push_back(100.0);
////    temperatures.push_back(100.0);
////    temperatures.push_back(110.0);
////    temperatures.push_back(110.0);
////    temperatures.push_back(110.0);
////    temperatures.push_back(120.0);
////    temperatures.push_back(130.0);
////    temperatures.push_back(140.0);
//    try{
//        calculate_mode(temperatures, &mode, &modemap);
//    } catch(ErrorHandler errHandler){
//        logError(errHandler);
//        return (EXIT_FAILURE);
//    }
//        LOG(INFO) << mode;
//    vector <GTIrecord> gti1;
//    vector <GTIrecord> gti2;
//    vector <GTIrecord> outgti;
//    
//    gti1.resize(3);
//    gti2.resize(3);
//    
//    gti1[0].tstart=50.0;
//    gti1[1].tstart=14.0;
//    gti1[2].tstart=17.0;
//    gti2[0].tstart=12.0;
//    gti2[1].tstart=15.0;
//    gti2[2].tstart=18.0;
//    gti1[0].tstop = 20.0;
//    gti1[1].tstop = 17.0;
//    gti1[2].tstop = 20.0;
//    gti2[0].tstop = 16.0;
//    gti2[1].tstop = 17.0;
//    gti2[2].tstop = 23.0;
//    try{
//        find_intersecting_range(gti1, gti2, &outgti);
//    }
//    catch(ErrorHandler errHandler){
//        logError(errHandler);
//        return EXIT_FAILURE;
//    }
//    for (int i = 0; i < gti1.size(); i++) {
//        LOG(INFO) << gti1[i].tstart << "-" << gti1[i].tstop;
//    }
//    LOG(INFO) << "------------------------------------------";
//    for (int i = 0; i < gti2.size(); i++) {
//        LOG(INFO) << gti2[i].tstart << "-" << gti2[i].tstop;
//    }
//    LOG(INFO) << "------------------------------------------";
//    for(int i=0; i<outgti.size(); i++){
//        LOG(INFO) << outgti[i].tstart << "-" << outgti[i].tstop;
//    }
//    string bins="1-2, 3-456, 6";
//    vector <double> startvec;
//    vector <double> stopvec;
//    getVecBins(bins, &startvec, &stopvec, 3);
//    
//    for(int i=0; i<3; i++){
//        LOG(INFO) << startvec[i] << " - " << stopvec[i];
//    }
    
//    vector <string> files;
//    vector <string> dirs;
//    
//    FileDirectoryHandler fdh;
//    string l1basepath = "/home/tanul/Documents/doctanul/projects/CZTI_data/ISAC_data/downloaded_on_03Sept2015_From_ISSDC/20150529_A06_300T25_0000000025_level1";
//    
//    try{
//    fdh.set_l1basepath(l1basepath);
//    fdh.generate_l1_filepaths();
//    } catch(ErrorHandler errHandler){
//        logError(errHandler);
//        return EXIT_FAILURE;
//    }
//    for(std::map<string,string>::iterator it=fdh.l1Paths.begin(); it!=fdh.l1Paths.end(); ++it){
//        LOG(INFO) << it->first << "=>" << it->second;
//    }
//    for(std::map<string,sciencel1Files>::iterator it=fdh.model1FilePaths.begin(); it!=fdh.model1FilePaths.end(); ++it) {
//        if (fdh.modeAvailable[it->first] == true) {
//            LOG(INFO) << "Mode " << it->first;
//            LOG(INFO) << "-------------------------";
//            LOG(INFO) << "Level-1 PREFIX:" << fdh.model1FilePaths[it->first].l1prefix;
//            LOG(INFO) << "Level-1 event file:" << fdh.model1FilePaths[it->first].scienceFilename;
//            LOG(INFO) << "Level-1 GTI file  :" << fdh.model1FilePaths[it->first].gtiFilename;
//            LOG(INFO) << "Level-1 BTI file  :" << fdh.model1FilePaths[it->first].btiFilename;
//        }
//    }
    
//    EventFileHandler evt;
//    vector <double> eventtime;
//    vector <double> cztseccnt;
//    vector <unsigned short> cztntick;
//    vector <unsigned char> detid;
//    vector <unsigned char> pixid;
//    vector <unsigned char> detx;
//    vector <unsigned char> dety;
//    
//    fitsfile *fptr;
//    fits_open_file(&fptr,"AS1P01_003T01_9000000002cztM0_level2.evt", READONLY, &status);
//    if(status){
//        LOG(ERROR) << "Error opening file.";
//        return EXIT_FAILURE;
//    }
//    try{
//    evt.read_quad_extension(fptr, 0, "TIME", 1, 1000);
//    evt.read_quad_extension(fptr, 0, "CZTSECCNT", 1, 1000);
//    evt.read_quad_extension(fptr, 0, "CZTNTICK", 1, 1000);
//    evt.read_quad_extension(fptr, 0, "DETID", 1, 1000);
//    evt.read_quad_extension(fptr, 0, "PIXID", 1, 1000);
//    evt.read_quad_extension(fptr, 0, "DETX", 1, 1000);
//    evt.read_quad_extension(fptr, 0, "DETY", 1, 1000);
//    } catch(ErrorHandler errHandler){
//        logError(errHandler);
//        return EXIT_FAILURE;
//    }
//    eventtime = evt.get_vecTimeQuad();
//    cztntick = evt.get_vecCztntickQuad();
//    cztseccnt = evt.get_vecLocalTimeQuad();
//    detid = evt.get_vecDetIDQuad();
//    pixid = evt.get_vecPixIDQuad();
//    detx = evt.get_vecDetXQuad();
//    dety = evt.get_vecDetYQuad();
    
//    for(long i=0; i<eventtime.size(); i++){
//        LOG(INFO) << eventtime[i] << " " << cztntick[i] << " " << detid[i] << " " << pixid[i] << " " << detx[i] << " " << dety[i];
//    }
    
//    Tct tct;
//    vector <double> spsTime;
//    vector <double> instrumentTime;
//    double *coef;
//    coef = new double [3];
//    string tctFilename="AS1P01_140T01_9000000018czt_level1.tct";
//    tct.read_tct_file(tctFilename);
//    spsTime = tct.getSpsTime();
//    instrumentTime = tct.getInstrumentTime();
//    fitPolynomial(instrumentTime.data(), spsTime.data(), spsTime.size(), coef, 2);
//    LOG(INFO) << "Coefficient of fitting";
//    LOG(INFO) << setprecision(20) << "c0: " << coef[0];
//    LOG(INFO) << setprecision(20) << "c1: " << coef[1];
//    LOG(INFO) << setprecision(20) << "c2: " << coef[2];
    //tct.clean_tct_data();
//    double differenceInst, differenceUTC;
//    for(int i=0; i<instrumentTime.size()-1; i++){
//        differenceInst = instrumentTime[i+1] - instrumentTime[i];
//        differenceUTC = spsTime[i+1] - spsTime[i];
//        LOG(INFO) << i << " " << i+1 << " " << differenceInst << " " << differenceUTC;
//        if((difference-16.0)>0.1 || (16.0-difference)>0.1){
//        LOG(INFO) << setprecision(20) << instrumentTime[i+1] << " " << instrumentTime[i] << " "  << difference;
//        difference = instrumentTime[i + 2] - instrumentTime[i+1];
//        LOG(INFO) << setprecision(20) << instrumentTime[i+2] << " " << instrumentTime[i+1] << " " <<  difference;
//        LOG(INFO) << "-----------------------------";
////        }
//    }
//    //***CLEANING MKF FILE
    //create_empty_fitsfile("out.mkf", "correctedMKFTemplate");
//    Attitude attHandler;
//    Orbit orbHandler;
//    LBT lbtHandler;
//    vector <orbStruct> orbvec1;
//    vector <orbStruct> orbvec2;
//    vector <attStruct> attvec;
//    vector <lbtStruct> lbtvec;
//    double deltaAtt;
//    double deltaOrb;
//    double deltaLbt;
//    string attFilename = "AS1P01_140T01_9000000018czt_level1.att";
//    string orbFilename = "AS1P01_140T01_9000000018czt_level1.orb";
//    string lbtFilename = "/home/sac/tanul/input/20151011_P01_140T01_9000000018_level1_V8.0/czti/aux/aux2/AS1P01_140T01_9000000018czt_level1.lbt";
//    try{
//        attHandler.read_attitude_file(attFilename);
//        orbHandler.read_orbit_file(orbFilename);
//        lbtHandler.read_lbt_file(lbtFilename);
//    } catch(ErrorHandler errHandler){
//        logError(errHandler);
//        return EXIT_FAILURE;
//    }
////    attvec = attHandler.get_att();
//    orbHandler.display_header_keywords();
//    orbvec1 = orbHandler.get_orb();
//    orbvec2 = orbHandler.get_orb();
//    if (adjacent_find(orbvec1.begin(), orbvec1.end(), whether_ascending) == orbvec1.end()) {
//        LOG(INFO) << "Time is increasing";
//    }
//    //sort(orbvec2.begin(), orbvec2.end(), whether_descending);
//    sort(orbvec2.begin(), orbvec2.end());
//    for(long i=0; i<10; i++){
//    LOG(INFO) << setprecision(20) << orbvec1[i].time << " " << orbvec2[i].time;
//    }
//    lbtvec = lbtHandler.get_lbt();
//    for(int i=1; i<attvec.size(); i++){
//        deltaAtt = attvec[i].time - attvec[i-1].time;
//        if(abs(deltaAtt-0.128)>0.1){
//            LOG(INFO) << deltaAtt;
//        }
//    }
//    for(int i=1; i<orbvec.size(); i++){
//        deltaOrb = orbvec[i].time - orbvec[i-1].time;
//        if(abs(deltaOrb-0.128)>0.1){
//            LOG(INFO) << deltaOrb;
//        }
//    }
//    for(int i=1; i<lbtvec.size(); i++){
//        deltaLbt = lbtvec[i].time - lbtvec[i-1].time;
//        LOG(INFO) << deltaLbt << " " << lbtvec[i].q1CZTCounter;
////        if(abs(deltaOrb-0.128)>0.1){
////            LOG(INFO) << deltaOrb;
////        }
//    }
    
//   Q quat;
//   quat.readQ();
//   double rollRA=0.0, pitchRA=0.0, yawRA=0.0;
//   double rollDEC=0.0, pitchDEC=0.0, yawDEC=0.0;
//   
//   get_rpy_RA_DEC(quat, &rollRA, &rollDEC, &pitchRA, &pitchDEC,
//                    &yawRA, &yawDEC);
//   
//   LOG(INFO) << "Roll RA DEC: " << rollRA << " " << rollDEC;
//   LOG(INFO) << "Pitch RA DEC: " << pitchRA << " " << pitchDEC;
//   LOG(INFO) << "Yaw RA DEC: " << yawRA << " " << yawDEC;

//    MVector a(1.0,1.0,1.0);
//    MVector b(1.0,0.0,0.0);
//    LOG(INFO) << a.get_magnitude();
//    LOG(INFO) << b.get_magnitude();
//    LOG(INFO) << dot_product(a,b);
//    LOG(INFO) << get_angle_bw_vectors(a,b)*TORAD << "deg";
    
//    string orbFilename = "AS1P01_140T01_9000000018czt_level1.orb";
//    string lbtFilename = "/home/sac/tanul/input/20151011_P01_140T01_9000000018_level1_V8.0/czti/aux/aux2/AS1P01_140T01_9000000018czt_level1.lbt";
//    try{
//        rmkf.regenerate_mkf_values(attFilename, lbtFilename, orbFilename);
//        rmkf.write_regenerated_mkffile("out.rmkf", "correctedMKFTemplate");
//    } catch(ErrorHandler errHandler){
//        logError(errHandler);
//        return EXIT_FAILURE;
//    }
//    Mkf mkf;
//    Attitude att;
//    Orbit orb;
//    LBT lbt;
//    fitsfile *fmkf;
//    string mkfFile="";
//    string lbtFilename = "AS1P01_140T01_9000000018czt_level1.lbt";
//    string attFilename="AS1P01_140T01_9000000018czt_level1.att";
//    string orbFilename = "AS1P01_140T01_9000000018czt_level1.orb";
//    try{
//        att.read_attitude_file(attFilename);
//        orb.read_orbit_file(orbFilename);
//        lbt.read_lbt_file(lbtFilename);
//        orb.display_header_keywords();
//        att.display_header_keywords();
//        lbt.display_header_keywords();
//    } catch(ErrorHandler errHandler){
//        logError(errHandler);
//    }
//        try{
//            mkf.regenerate_mkf_values(attFilename, lbtFilename, orbFilename);
//            mkf.write_regenerated_mkffile("out.rmkf", "correctedMKFTemplate");
//        } catch(ErrorHandler errHandler){
//            logError(errHandler);
//            return EXIT_FAILURE;
//        }
    //    cout << "Enter  mkf file name" << endl;
//    cin >> mkfFile;
//    fits_open_file(&fmkf, (char*) mkfFile.c_str(), READONLY, &status);
//    
//    if(mkf.read_mkffile(mkfFile)) {
//        LOG(INFO) << "Mkf file " << mkfFile << " has not been read.";
//        return(EXIT_FAILURE);
//    }
//
//    if(mkf.read_mkf_time(fmkf)) {
//        LOG(INFO) << "Mkf time not read."; 
//        return (EXIT_FAILURE);
//}
    
//    vector <double> data;
//    data.push_back(0.0);
//    data.push_back(1.0);
//    data.push_back(0.0);
//    data.push_back(2.0);    
//    data.push_back(0.0);
//    data.push_back(1.0);
//    data.push_back(0.0);
//    data.push_back(-1.0);
//    LOG(INFO) << "Data: ";
//    print_vector(data);
//    four1(&data, 1);
//    LOG(INFO) << "Fourier transform: ";
//    print_vector(data, 5);
//    DPI dpi;
//    TanMaskGeometry mask;
//    string dpiFilename;
//    string maskFilename;
//    vector < vector <unsigned char> > q0mask;
//    vector <unsigned char> lq0mask;
//    vector <long> ldphq0;
//    vector < vector <float> > dpiq0;
//    vector <float> ldph;
//    vector <float> lmask;
//    vector < vector <long> > dphq0;
//    vector <float> dpiq01d;
//    vector <long> dphq01d;
//    vector <float> correlationMatrix;
//    vector <vector <float> > co2d;
//    vector <vector <float> > tempmask;
//    vector <vector <float> > tempq0;
//    vector <vector <float> > tempimage;
//    vector <float> tempco2d;
//    int oversamplingFactor=16;
//    dpiFilename = "/home/sac/test/crab/sim_shadow.fits";
//    maskFilename = "/home/sac/tanul/CALDB/astrosat/czti/bcf/CZTIMask64x64.fits";
//    if(mask.read_64x64_mask(maskFilename)){
//        LOG(ERROR) << "Error in reading mask file.";
//        return EXIT_FAILURE;
//    }
//    try{
//        dphq0 = dpi.get_dph_image(dpiFilename, "Q0");
//    } catch(ErrorHandler errHandler){
//        logError(errHandler);
//    }
//    q0mask = mask.get_q0Mask();
//   
//    linearize_2Dvector(q0mask, &lq0mask);
//    linearize_2Dvector(dphq0, &ldphq0);
//    for(int i=0; i<lq0mask.size(); i++){
//        ldph.push_back((float)ldphq0[i]);
//        lmask.push_back((float) lq0mask[i]);
//    }
//    oversample(&ldph, 64, 64, oversamplingFactor);
//    oversample(&lmask, 64, 64, oversamplingFactor);
//    co2d = generate_correlation_matrix2D(ldph, lmask, &correlationMatrix, oversamplingFactor);
//
////    int size = sqrt(correlationMatrix.size());
////    LOG(INFO) << "xsize of correlation matrix: " << size;
////    tempco2d.resize(size, 0.0);
////    tempmask.resize(size, tempco2d);
////    tempq0.resize(size, tempco2d);
////    for(int i=0; i<size; i++){
////        for(int j=0; j<size; j++){
////            tempmask[i][j] = lmask[i*size+j];
////            tempq0[i][j] = ldph[i*size+j];
////        }
////    }
//    
//    tempco2d.resize(64, 0.0);
//    tempimage.resize(64, tempco2d);
//    for (int i=0; i<64; i++){
//        for (int j=0; j< 64;  j++){
//            tempimage[i][j]=correlationMatrix[i*64+j];
//        }
//    }
//    write_img(tempimage, "tempImg.img" , FLOAT_IMG, TFLOAT);
//    write_img(co2d, "co2d2.img", FLOAT_IMG, TFLOAT);
////    write_img(tempmask, "outmask", FLOAT_IMG, TFLOAT);
////    write_img(tempq0, "q0dph", FLOAT_IMG, TFLOAT);
//    
//    ImageFileHandler imgfh;
//    try{
//    imgfh.create_image_file("out.img", IMAGETEMPLATE);
//    } catch (ErrorHandler errHandler){
//        logError(errHandler);
//        return EXIT_FAILURE;
//    }
//
    Badpix badpix;
    string badpixfilename = "out.badpix";
    vector <vector <unsigned char> > badpixmap;
    badpix.read_badpix_file(badpixfilename);
    badpixmap = badpix.get_badpix_map();
    write_img(badpixmap, "badpixmap.img", BYTE_IMG, TBYTE);
    return 0;   
}




