
#include "cztpipeline.h"

using namespace std;

cztpipeline::cztpipeline(){
    strcpy(modulename,"cztpipeline_v");
    strcat(modulename,VERSION);
    
    //optional parameter setting
    strcpy(alphaval,"0,1");
    strcpy(vetorange,"-");
    strcpy(runmode,"copy");
    strcpy(thresholdfile,"");
    strcpy(quadsToProcess, "0,1,2,3");
    nBkgdCoef=1;
    strcpy(customgti,"");
    strcpy(usergti,"");
    strcpy(gtiextname,"GTI");
    badpixthreshold=0;
    nPackets = 50000;
    strcpy(outputquality,"GOOD");
            
}

int cztpipeline::read(int argc,char **argv){
     int status=0;
    if(PIL_OK!=(status=PILInit(argc,argv))){
        LOG(ERROR)<<"***Error Initializing PIL***";
        return status;
    }
    
    if(PIL_OK!=(status=PILGetFname("indir",indir))){
        LOG(ERROR)<<"***Error Reading input directory:"<<indir<<"***";
        return status;
    }
     
    if(PIL_OK!=(status=PILGetFname("outdir",outdir))){
        LOG(ERROR)<<"***Error Reading output directory:"<<outdir<<"***";
        return status;
    }
     
    if(PIL_OK!=(status=PILGetFname("caldb",caldb))){
        LOG(ERROR)<<"***Error Reading CALDB directory:"<<caldb<<"***";
        return status;
    }
    
    if(strcmp(caldb,"$CALDBnew")==0){
         char *caldbpath=getenv("CALDBnew");
         if(caldbpath==NULL){
             LOG(ERROR)<<"***Environment variable CALDB not set***";
             return (EXIT_FAILURE);
         }
         strcpy(caldb,caldbpath);
    } 
    
    // it chechks whether data is big endian for cztscience2event module
    if(PIL_OK!=(status=PILGetBool("bigendian",&bigendian))){
        LOG(ERROR)<<"***Error Reading bigendian***";
        return status;
    }

    if (PIL_OK != (status = PILGetInt("nPackets", &nPackets))) {
        LOG(ERROR) << "***Error Reading number of packets " << nPackets << "***";
        return status;
    }
        
    if(PIL_OK!=(status=PILGetBool("cztevtcleanflag",&cztevtcleanflag))){
        LOG(ERROR)<<"***Error Reading cztevtcleanflag ***";
        return status;
    }  
    if(cztevtcleanflag){ 
        if(PIL_OK!=(status=PILGetString("alphaval",alphaval))){
            LOG(ERROR)<<"***Error reading alphaval***";
            return status;
        } 

        if(PIL_OK!=(status=PILGetString("vetorange",vetorange))){
            LOG(ERROR)<<"***Error reading vetorange***";
            return status;
        } 
    }
    
    if(PIL_OK!=(status=PILGetBool("cztgtigenflag",&cztgtigenflag))){
        LOG(ERROR)<<"***Error Reading cztgtigenflag ***";
        return status;
    }   
    if(cztgtigenflag){
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
    if(PIL_OK!=(status=PILGetString("runmode",runmode))){
        LOG(ERROR)<<"***Error Reading runmode parameter***";
        return status;
    }

    if(strncasecmp(runmode,"mkf-refined",11)==0)  runmodeflag=MKF_REFINED;
    else if(strncasecmp(runmode,"custom-refined",14)==0)  runmodeflag=CUSTOM_REFINED;
    else if(strncasecmp(runmode,"mkf",3)==0)  runmodeflag=MKF;
    else if(strncasecmp(runmode,"custom",6)==0)  runmodeflag=CUSTOM;
    else if(strncasecmp(runmode,"copy",4)==0)    runmodeflag=COPY;

        else{
            LOG(ERROR)<<"Wrong mode entered Use custom/copy/custom-refined";
            return -1;
        }
        if(runmodeflag==MKF || runmodeflag==MKF_REFINED){
        if(PIL_OK!=(status=PILGetFname("thresholdfile",thresholdfile))){
                LOG(ERROR)<<"***Error reading threshold file:"<<thresholdfile<<"***";
                return status;
            }
        }
        else if(runmodeflag==CUSTOM || runmodeflag==CUSTOM_REFINED){
            if(PIL_OK!=(status=PILGetString("customgti",customgti))){
                LOG(ERROR)<<"***Error reading custom intervals***";
                return status;
            }
        }  

        if(PIL_OK!=(status=PILGetFname("usergti",usergti))){
            LOG(ERROR)<<"***Error reading user gti filename:"<<usergti<<"***";
            return status;
        }

        if(PIL_OK!=(status=PILGetString("gtiextname",gtiextname))){
                LOG(ERROR)<<"***Error reading gti extension name***";
                return status;
        }         
    }
     
    if(PIL_OK!=(status=PILGetString("ebins",ebins))){
        LOG(ERROR)<<"***Error Reading energy bins:"<<ebins<<"***";
        return status;
    }
    //QUADS TO PROCESS
    do {
        if (PIL_OK != (status = PILGetString("quadsToProcess", quadsToProcess))) {
            LOG(ERROR) << "***Error reading quadrant:" << quadsToProcess << "***";
            return status;
        }
    } while (isQuadrantNumbersValid(quadsToProcess));
    if(PIL_OK!=(status=PILGetString("tbins",tbins))){
        LOG(ERROR)<<"***Error Reading time bins***";
        return status;
    }

    if (PIL_OK != (status = PILGetReal4("badpixthreshold", &badpixthreshold))) {
        LOG(ERROR) << "***Error Reading badpixthreshold ***";
        return status;
    }  
     
    if(PIL_OK!=(status=PILGetInt("catalogextnum",&catalogextnum))){
        LOG(ERROR)<<"***Error Reading extension number for catalog file***";
        return status;
    }
    
    if(PIL_OK!=(status=PILGetString("RA_colname",RA_colname))){
        LOG(ERROR)<<"***Error reading RA column name***";
        return status;
    }
    
    if(PIL_OK!=(status=PILGetString("DEC_colname",DEC_colname))){
        LOG(ERROR)<<"***Error reading DEC column name***";
        return status;
    }
    
    if(PIL_OK!=(status=PILGetString("FLUX_colname",FLUX_colname))){
        LOG(ERROR)<<"***Error reading FLUX column name***";
        return status;
    } 
     
    if(PIL_OK!=(status=PILGetString("energyrange",energyrange))){
        LOG(ERROR)<<"***Error reading energy range***";
        return status;
    }
    if(PIL_OK!=(status=PILGetString("timerange",timerange))){
        LOG(ERROR)<<"***Error reading energy range for spectrum***";
        return status;
    }
     
    if(PIL_OK!=(status=PILGetReal4("binsize_lc",&binsize_lc))){
        LOG(ERROR)<<"Error reading binsize for light curve***";
        return status;
    }
    if(PIL_OK!=(status=PILGetReal4("binsize_spec",&binsize_spec))){
        LOG(ERROR)<<"Error reading binsize for spectrum***";
        return status;
    }
    if(PIL_OK!=(status=PILGetInt("oversampfactor",&oversampfactor))){
        LOG(ERROR)<<"Error reading oversampling factor for image***";
        return status;
    }
    
     if(PIL_OK!=(status=PILGetReal4("resolution_limit",&resolution_limit))){
        LOG(ERROR)<<"Error reading oversampling factor for image***";
        return status;
    }
     
     
     if (PIL_OK != (status = PILGetInt("nBkgdCoef", &nBkgdCoef))) {
        LOG(ERROR) << "***Error reading number of background coefficients --Allowed range is 1-6***";
        return status;
    }
    if (nBkgdCoef > 6 || nBkgdCoef < 1) {
        LOG(ERROR) << "***Error reading number of background coefficients --Allowed range is 1-6***";
        return EXIT_FAILURE;
    }
    
     if(PIL_OK!=(status=PILGetReal4("peak_threshold",&peak_thr))){
        LOG(ERROR)<<"Error reading peak threshold value for image***";
        return status;
    }
     if(PIL_OK!=(status=PILGetReal4("source_strength_threshold",&source_strength_thr))){
        LOG(ERROR)<<"Error reading source strength threshold value for image***";
        return status;
    }
     
        
    if(PIL_OK!=(status=PILGetBool("debugmode",&debugmode))){
        LOG(ERROR)<<"***Error Reading debugmode parameter***";
        return status;
    }
    if(PIL_OK!=(status=PILGetBool("history",&history))){
        LOG(ERROR)<<"***Error Reading history parameter"<<history<<"***";
        return status;
    }
    
    if(PIL_OK!=(status=PILGetBool("clobber",&clobber))){
        LOG(ERROR)<<"***Error Reading clobber:"<<clobber<<"***";
        return status;
    }
         
    PILClose(status);
}


void cztpipeline::display(){
    LOG(INFO)<<"----------------------------------------------------------------------------";
    LOG(INFO)<<"                          CZTPIPELINE PARAMETERS                            ";
    LOG(INFO)<<"----------------------------------------------------------------------------";
    LOG(INFO)<<"Input Directory                     : "<<indir;
    LOG(INFO)<<"Output Directory                    : "<<outdir;         
    LOG(INFO)<<"CALDB Directory                     : "<<caldb;
    LOG(INFO)<<"BigEndian (CZTSCIENCE2EVENT)        : "<<bigendian;
    LOG(INFO)<<"No. of pacets to read in buffer     : "<<nPackets;
    LOG(INFO)<<"cztevtcleanflag                     : "<<cztevtcleanflag;
    LOG(INFO)<<"alphaval (CZTEVTCLEAN)              : "<<alphaval;
    LOG(INFO)<<"vetorange (CZTEVTCLEAN)             : "<<vetorange;
    LOG(INFO)<<"cztgtigenflag                       : "<<cztgtigenflag;
    LOG(INFO)<<"runmode                             : "<<runmode;
    LOG(INFO)<<"thresholdfile                       : "<<thresholdfile;
    LOG(INFO)<<"customgti                           : "<<customgti;
    LOG(INFO)<<"Output user gti file                : "<<usergti;
    LOG(INFO)<<"GTI extension name                  : "<<gtiextname;
    LOG(INFO)<<"Quadrants                           : "<< quadsToProcess;
    LOG(INFO)<<"Time Bins for DPI                   : "<<tbins;
    LOG(INFO)<<"Energy Bins for DPI                 : "<<ebins;
    LOG(INFO)<<"Bad pixel threshold for refinement  : "<<badpixthreshold;
    LOG(INFO)<<"Catalog extension number to use     : "<<catalogextnum;
    LOG(INFO)<<"RA column name in catalog           : "<<RA_colname;
    LOG(INFO)<<"DEC column name in catalo           : "<<DEC_colname;
    LOG(INFO)<<"FLUX column name                    : "<<FLUX_colname;
    LOG(INFO)<<"Time Range for Light Curve/Spectrum : "<<timerange;
    LOG(INFO)<<"Energy range for Light Curve/Spectrum : "<<energyrange;
    LOG(INFO)<<"Binsize for Light Curve             : "<<binsize_lc;
    LOG(INFO)<<"Binsize for Spectrum                : "<<binsize_spec;
    LOG(INFO)<<"Oversampling factor for Image       : "<<oversampfactor;
    LOG(INFO)<<"Peak threshold                      : "<<peak_thr;
    LOG(INFO)<<"Clobber                             : "<<clobber;
    LOG(INFO)<<"History                             : "<<history;
    LOG(INFO)<<"----------------------------------------------------------------------------";
}

int cztpipeline::cztpipelineProcess(){
    int i,j,errorcode=0;
    //check whether input directory exists
    if(!(DirExists(indir))){
        LOG(ERROR)<<"***Input Directory does not exist: "<<indir<<"***";
        return (EXIT_FAILURE);
    }
    if(!(DirExists(caldb))){
        LOG(ERROR)<<"***CALDB directory does not exist: "<<caldb<<"***";
        return (EXIT_FAILURE);
    }
    
    if(objpath.setAllDirPaths(indir,outdir)){
        return (EXIT_FAILURE);
    }
    objpath.display();
    
    if(DirExists(objpath.outdir) && clobber==true){
        LOG(ERROR)<<"Deleting existing directory......";
        string dname = objpath.outdir;
        if (dname.rfind('2') == dname.length()-1){         //delete only in case existing directory ends with 2
            if(deleteDir(objpath.outdir)){
                LOG(ERROR)<<"***Error in deleting directory "<<objpath.outdir<<"***";
                return (EXIT_FAILURE);
            }
        }
        else{
            LOG(ERROR)<<"***Existing directory does not ends with 2***Hence cannot remove";
            return (EXIT_FAILURE);
        }
        
    }
    else if(DirExists(objpath.outdir) && clobber==false){
        LOG(ERROR)<<"***Output directory already exists***";
        LOG(ERROR)<<"***Use clobber=yes for overwriting the output directory***";
        return (EXIT_FAILURE);
    }
    else if(!DirExists(outdir)){
        LOG(ERROR)<<"***'"<<outdir<<"' directory does not exists***";
        LOG(ERROR)<<"Kindly provide existing location for output";
        return (EXIT_FAILURE);
    }
   
    //Creating Data Sets for modules to be run 
    char *obsid=getObsID();
    if(obsid==NULL){
        LOG(ERROR)<<"***Could not find observation id***";
        LOG(ERROR)<<"***Invalid name for level1 directory***";
        return (EXIT_FAILURE);
    }
    LOG(INFO)<<"Observation ID : "<<obsid;
       
    //creating output directory structure
    if(objpath.createOutDir()){
        return (EXIT_FAILURE);
    }
    
    
    vector<DataSet> vdataset;          //one directory can have multiple science data files that will create multiple data sets
    DataSet d;
   
    joinStrings(metafileprefix_in,4,SATID,obsid,INSTRUMENTID,"_level1");
    joinStrings(metafileprefix_out,4,SATID,obsid,INSTRUMENTID,"_level2");
    if(setAuxFiles()){
        LOG(ERROR)<<"***Error in accessing auxiliary files***";
        return (EXIT_FAILURE);
    }
    if(copyAuxFiles()){
        LOG(ERROR)<<"***Error in copying auxiliary files to output***";
        return (EXIT_FAILURE);
    }
    
    //code to create datasets as the number of science data files
    //chain will be run for each of the science data file
    char **scifile; 
    int n,l,flag;
    char temp_prefix[50];
    string cmdstr;
    //for non event data modes
    LOG(INFO)<<"Copying non event data modes to level2 directory";
    for(int i=EVENTDATA_MODE_END+2;i<NUMOFMODES;i++){    //Mode 8 is undefined //Mode9 onwards is not to be processed 
         if(objpath.modepresent[i]==true){
             cmdstr="cp -r "+objpath.modedir_in[i]+" "+objpath.cztidir_out;
             if(system(cmdstr.c_str())) { 
                 LOG(INFO)<<"***"<<cmdstr<<"***";
                 LOG(ERROR)<<"***Error in copying non event mode directories***";
                 return (EXIT_FAILURE);
             }
         }
    }
    //Event Data Processing modes
    char tempExtFilePath[PATHSIZE];
    for(int i=0;i<=(EVENTDATA_MODE_END+1);i++){
        if(objpath.modepresent[i]==true){
             flag=getL1Files((char *)objpath.modedir_in[i].c_str(),&scifile,&n);

             //LOG(ERROR)<<"n:"<<n<<"";
             for(int j=0;j<n;j++){
                 l=strlen(scifile[j]);
                 strncpy(temp_prefix,scifile[j],l-6);
                 temp_prefix[l-6]='\0';
                 strcat(temp_prefix,"2");
                 temp_prefix[l-5]='\0';
                if (i == 8) {
                    joinStrings(tempExtFilePath, 4, objpath.modedir_out[i].c_str(), FILEPATHSEPARATOR, temp_prefix, ".fits");
                }


                 joinStrings(d.scifile,3,objpath.modedir_in[i].c_str(),FILEPATHSEPARATOR,scifile[j]); 
                 LOG(INFO) << d.scifile;
                 joinStrings(d.l1btifile,4,objpath.modedir_in[i].c_str(),FILEPATHSEPARATOR,metafileprefix_in,".bti");
                 joinStrings(d.l1cfgfile,4,objpath.modedir_in[i].c_str(),FILEPATHSEPARATOR,metafileprefix_in,".cfg");
                 joinStrings(d.l1gtifile,4,objpath.modedir_in[i].c_str(),FILEPATHSEPARATOR,metafileprefix_in,".gti");
                 joinStrings(d.l1logfile,4,objpath.modedir_in[i].c_str(),FILEPATHSEPARATOR,metafileprefix_in,".log");
                 joinStrings(d.l1mkffile,4,objpath.modedir_in[i].c_str(),FILEPATHSEPARATOR,metafileprefix_in,".mkf");
                 //for output files
                 
                 joinStrings(d.DPHfile,4,objpath.modedir_out[i].c_str(),FILEPATHSEPARATOR,temp_prefix,".dph");
                 joinStrings(d.DPIfile,4,objpath.modedir_out[i].c_str(),FILEPATHSEPARATOR,temp_prefix,".dpi");
                 joinStrings(d.specLCfile,3,objpath.modedir_out[i].c_str(),FILEPATHSEPARATOR,temp_prefix);
                 joinStrings(d.outexparrFile,4,objpath.modedir_out[i].c_str(),FILEPATHSEPARATOR,temp_prefix,".exparr");
                 joinStrings(d.avgaspectfile,4,objpath.modedir_out[i].c_str(),FILEPATHSEPARATOR,temp_prefix,".aspect");
                 joinStrings(d.badpixelfile,4,objpath.modedir_out[i].c_str(),FILEPATHSEPARATOR,temp_prefix,".badpix");
                 joinStrings(d.eventfile,4,objpath.modedir_out[i].c_str(),FILEPATHSEPARATOR,temp_prefix,".events");
                 
                 joinStrings(d.nobkgdfile,4,objpath.modedir_out[i].c_str(),FILEPATHSEPARATOR,temp_prefix,".nobkgd");
                 
                 joinStrings(d.imagefile,4,objpath.modedir_out[i].c_str(),FILEPATHSEPARATOR,temp_prefix,".img");
                 joinStrings(d.shadowfile,4,objpath.modedir_out[i].c_str(),FILEPATHSEPARATOR,temp_prefix,".shadow");
                 joinStrings(d.specfile,4,objpath.modedir_out[i].c_str(),FILEPATHSEPARATOR,temp_prefix,".spec");
                 joinStrings(d.tempeventfile,4,objpath.modedir_out[i].c_str(),FILEPATHSEPARATOR,temp_prefix,".fits");
                 
                 joinStrings(d.hdrINFOfile,4,objpath.modedir_out[i].c_str(),FILEPATHSEPARATOR,temp_prefix,".hdr");
                 
                 joinStrings(d.l2gtifile,4,objpath.modedir_out[i].c_str(),FILEPATHSEPARATOR,temp_prefix,".gti");
                 
                 joinStrings(d.l2btifile,4,objpath.modedir_out[i].c_str(),FILEPATHSEPARATOR,temp_prefix,".bti");
                 joinStrings(d.sourcelist,4,objpath.modedir_out[i].c_str(),FILEPATHSEPARATOR,temp_prefix,".srclist");    //ASCII
                 d.fail_in_module = "NONE";
                 //d.display();
                 vdataset.push_back(d);
             }
             if(flag==0) { for(int k=0;k<n;k++) delete[] scifile[k];  delete[] scifile; }
        }
    }
    LOG(INFO)<<"Number of datasets found:"<<vdataset.size();
    bool processingstatus[vdataset.size()];     //will hold processing status for each data set
    initArray<bool>(processingstatus,vdataset.size(),false);
    //setting CALDB file
    CALDBdatafile objcaldb;
    objcaldb.getCaldbFiles(caldb);
    objcaldb.display();
    flag=0;

    LOG(INFO) << "Generating level-2 MKF file....";
    cztmkfgen par1;
    flag = par1.read(attfile, lbtfile, orbfile, l2mkffile, clobber, history);
    if (flag) {
        LOG(ERROR) << "***Error in generating level-2 MKF file.***";
        return (EXIT_FAILURE);
    }
    flag = par1.cztmkfgenProcess();
    if (flag) {
        LOG(ERROR) << "***Error in generating level-2 MKF file.***";
        return (EXIT_FAILURE);
    }
    par1.display();
     //running pipeline for each of the dataset
    LOG(INFO)<< vdataset.size();
    for(i=0;i<(vdataset.size());i++){
	//system("rm -rf /tmp/*");
        LOG(INFO)<<"Running chain for dataset "<<i+1;
        LOG(INFO)<<"Files Used for input/output";
        vdataset[i].display();

        //Data Extraction Module
        LOG(INFO)<<"CZTSCIENCE2EVENT ......";
        cztscience2event par1;
        flag=par1.read(vdataset[i].scifile,tctfile,vdataset[i].tempeventfile,vdataset[i].hdrINFOfile, vdataset[i].l2gtifile,
                vdataset[i].l2btifile, nPackets, bigendian,clobber,history);
        par1.display();
        flag=par1.cztscience2eventProcess();
        if(flag){
            LOG(ERROR)<<"***Error in CZTSCIENCE2EVENT for dataset "<<i+1<<"***";
            vdataset[i].fail_in_module = "CZTSCIENCE2EVENT"; 
            continue;
        }
    }
    LOG(INFO) << "vdataset size ::::: " << vdataset.size();
   for(i=0; i<vdataset.size()-1;i++){ 
        //pha to pi/energy conversion module
        LOG(INFO)<<"CZTPHA2ENERGY ........";
        LOG(INFO)<<"TEMP EXT FILE PATH" << tempExtFilePath;
        Cztpha2energy par2;
        par2.read(vdataset[i].tempeventfile,objcaldb.eboundsFile, objcaldb.goFile,
                vdataset[i].eventfile, tempExtFilePath, clobber,history);
        par2.display();
        flag=par2.cztpha2energy_process();
        if(flag)   {
            LOG(ERROR)<<"***Error in CZTPHA2ENERGY for dataset "<<i+1<<"***";
            vdataset[i].fail_in_module = "CZTPHA2ENERGY";
            continue;
        }

        //GTI generation module
        if (cztgtigenflag) {
            LOG(INFO) << "CZTGTIGEN .........";
            cztgtigen gtigenpar;
            gtigenpar.read(runmode,l2mkffile , thresholdfile, customgti,
                    vdataset[i].l2gtifile, gtiextname, usergti, clobber, history);
            gtigenpar.display();
            flag = gtigenpar.cztgtigenProcess();
            if (flag) {
                LOG(ERROR) << "***Error in CZTGTIGEN for dataset " << i + 1 << "***";
                vdataset[i].fail_in_module = "CZTGTIGEN";
                continue;
            }
        } 
        
        //selecting data based on GTI
        LOG(INFO) << "CZTDATASEL .........";
        cztdatasel dataselpar;
        dataselpar.read(vdataset[i].eventfile, vdataset[i].l2gtifile, gtiextname, vdataset[i].eventfile, clobber, history);
        dataselpar.display();
        flag = dataselpar.cztdataselProcess();
        if (flag) {
            LOG(ERROR) << "***Error in CZTDATSEL for dataset " << i + 1 << "***";
            vdataset[i].fail_in_module = "CZTDATASEL";
            continue;
        }
        //Average Aspect generation module
        LOG(INFO)<<"CZTGAAS ........";
        Cztgaas par3;
        par3.read(vdataset[i].eventfile,objcaldb.teldeffileQ0,objcaldb.teldeffileQ1,objcaldb.teldeffileQ2,
                objcaldb.teldeffileQ3, vdataset[i].avgaspectfile, l2mkffile,clobber,history);
        par3.display();
        flag=par3.cztgaas_process();
        if(flag)   {
            LOG(ERROR)<<"***Error in CZTGAAS for dataset "<<i+1<<"***";
            vdataset[i].fail_in_module = "CZTGAAS";
            continue;
        }
//       
//        //Event Cleaning module
        if(cztevtcleanflag){
            LOG(INFO)<<"CZTEVTCLEAN ........";
            cztevtclean par4;
            par4.read(vdataset[i].eventfile,vdataset[i].eventfile,alphaval,
                    vetorange,clobber,history);
            par4.display();
            flag=par4.cztevtcleanProcess();
            if(flag){
                LOG(ERROR)<<"***Error in CZTEVTCLEAN for dataset "<<i+1<<"***";
                vdataset[i].fail_in_module = "CZTEVTCLEAN";
                continue;
            }
        }
//          
        LOG(INFO) << "CZTFLAGBADPIX........";
        cztflagbadpix cztflagbadpixpar;
        vector <string> badpixfiles;
        string caldbbadpixfilepath = (string) caldb + "/bcf/" + (string) objcaldb.badpixFile;
        badpixfiles.push_back(caldbbadpixfilepath);
        cztflagbadpixpar.read(1, badpixfiles, vdataset[i].badpixelfile, debugmode, history, clobber);
        flag=cztflagbadpixpar.cztflagbadpix_process();
        if(flag){
            LOG(ERROR) << "*** Error in CZTFLAGBADPIX for dataset " << i+1 << "***";
            vdataset[i].fail_in_module = "CZTFLAGBADPIX";
            continue;
        }
//        
//        //DPI Generation Module
        LOG(INFO)<<"CZTDPIGEN .........";
        Cztdpigen dpigenpar;
        dpigenpar.read(vdataset[i].eventfile,objcaldb.effAreaFile, objcaldb.badpixFile,
                vdataset[i].DPHfile, vdataset[i].DPIfile,quadsToProcess, badpixthreshold,
                ebins,tbins, caldb,clobber,history);
        dpigenpar.display();
        flag=dpigenpar.cztdpigenProcess();
        if(flag){
            LOG(ERROR)<<"***Error in CZTDPIGEN for dataset "<<i+1<<"***";
            vdataset[i].fail_in_module = "CZTDPIGEN";
            continue;
        }
               
        LOG(INFO)<<"CZTBINDATA..............";
        cztbindata bindatapar;
        vector <string> aspectFiles;
        //for light curve & spectrum both
        LOG(INFO) << "Generating light curve and spectrum....";
        bindatapar.read(vdataset[i].eventfile, objcaldb.compMaskFile, "camera", aspectFiles, quadsToProcess,
                vdataset[i].badpixelfile, objcaldb.eboundsFile, objcaldb.cameraGeomFile, objcaldb.effAreaFile,
                "both", vdataset[i].l2gtifile, timerange, energyrange, vdataset[i].specLCfile, vdataset[i].outexparrFile,
                1, binsize_lc, 0.0, 0.0, clobber, history, debugmode);
        bindatapar.display();
        flag=bindatapar.cztbindataProcess();
        if(flag){
             LOG(ERROR)<<"***Error in CZTBINDATA for dataset "<<i+1<<"***";
             vdataset[i].fail_in_module = "CZTBINDATA";
             continue;
        }
    
        string aspectfileQ0 = (string)vdataset[i].avgaspectfile + "_Q0";
        string aspectfileQ1 = (string)vdataset[i].avgaspectfile + "_Q1";
        string aspectfileQ2 = (string)vdataset[i].avgaspectfile + "_Q2";
        string aspectfileQ3 = (string)vdataset[i].avgaspectfile + "_Q3";
        
        LOG(INFO)<<"CZTPSFSHADOWGEN.........";
        cztpsfshadowgen psfshadowpar;
        psfshadowpar.read((char*)aspectfileQ0.c_str(),(char*)aspectfileQ1.c_str(),
                (char*)aspectfileQ2.c_str(), (char*)aspectfileQ3.c_str(),
                objcaldb.xrayCatalog,catalogextnum,objcaldb.compMaskFile, 
                objcaldb.cameraGeomFile, objcaldb.effAreaFile, vdataset[i].sourcelist,
                vdataset[i].shadowfile, 6.0, 6.0, 1, 10, 10.0, 10.0, 100.0, 10, 
                clobber,history);
        psfshadowpar.display();
        flag=psfshadowpar.cztpsfshadowgenProcess();
        if(flag){
                LOG(ERROR)<<"***Error in CZTPSFSHADOWGEN for dataset "<<i+1<<"***";
                vdataset[i].fail_in_module = "CZTPSFSHADOWGEN";
                continue;
         }  
//        
//        
        LOG(INFO)<<"CZTIMAGE.................";
        cztimage imagepar;
        imagepar.read(vdataset[i].DPIfile,objcaldb.maskfile64x64,objcaldb.compMaskFile, 
                vdataset[i].shadowfile, (char*) aspectfileQ0.c_str(), objcaldb.xrayCatalog, catalogextnum, 
                RA_colname, DEC_colname, FLUX_colname, 
                resolution_limit, peak_thr, oversampfactor, d.sourcelist,
                  source_strength_thr,vdataset[i].imagefile, quadsToProcess, nBkgdCoef, debugmode,clobber,history);
        imagepar.display();
        flag=imagepar.cztimageProcess();
        if(flag){
             LOG(ERROR)<<"***Error in CZTIMAGE for dataset "<<i+1<<"***";
             vdataset[i].fail_in_module = "CZTIMAGE";
             continue;
        }
        
        processingstatus[i]=true;
      }   
    LOG(INFO)<<"---------------------------------------------------------------------------------";
    LOG(INFO)<<"                               SUMMARY REPORT                                    ";
    LOG(INFO)<<"---------------------------------------------------------------------------------";
    bool resultstatus=false;
    for(int i=0;i<vdataset.size();i++){
        if(processingstatus[i]==true){
            LOG(INFO)<< vdataset[i].scifile << "-- Successfully generated"; 
            
        }
        else
            LOG(INFO)<<vdataset[i].scifile << "-- Failed in module "<< vdataset[i].fail_in_module;     
            resultstatus=resultstatus | processingstatus[i];
    }
    if(resultstatus==false){
        return (EXIT_FAILURE);
    }
        LOG(INFO);
        return (EXIT_SUCCESS);
    }

    void DataSet::display(){
        LOG(INFO)<<"-------------Files--------------------------";
        LOG(INFO)<<""<<scifile;
        LOG(INFO)<<""<<l1btifile;
        LOG(INFO)<<""<<l1mkffile;
        LOG(INFO)<<""<<l1gtifile;
        LOG(INFO)<<""<<l1logfile;
        LOG(INFO)<<""<<l1cfgfile;
        LOG(INFO)<<""<<eventfile;
        LOG(INFO)<<""<<tempeventfile;
        LOG(INFO)<<""<<avgaspectfile;
        LOG(INFO)<<""<<badpixelfile;
        LOG(INFO)<<""<<DPIfile;
        LOG(INFO)<<""<<LCfile;
        LOG(INFO)<<""<<specfile;
        LOG(INFO)<<""<<shadowfile;
        LOG(INFO)<<""<<imagefile;
        LOG(INFO)<<"---------------------------------------------";
    }

    int modefilter(const struct dirent *dptr){
        int retval=0;
    if(strncasecmp(dptr->d_name,MODEDIRPREFIX,4)==0){
        retval=1;
    }
    return retval;
}

int fitsfilter(const struct dirent *dptr){
    int retval=0;
    if(fnmatch("*.fits",dptr->d_name,FNM_CASEFOLD)==0){
        retval=1;
    }
    return retval;
}

char* cztpipeline::getObsID(){
    char *level1dir=basename(indir);
    if(level1dir==NULL)  return NULL;
    int len=strlen(level1dir);
    char *temp=new char[len+1];
    strcpy(temp,level1dir);
    char *t1=strchr(temp,'_');
    if(t1==NULL)  return NULL;
    char *t2=strrchr(t1,'_');
    if(t2==NULL)  return NULL;
    int obsidlen=strlen(t1)-1-strlen(t2);
    if(obsidlen<=0)  return NULL;
    t1++;
    char *obsid=new char[obsidlen+1];
    if(obsid==NULL)  return NULL;
    strncpy(obsid,t1,obsidlen);
    obsid[obsidlen]='\0';
    delete[] temp;
    return obsid;
}

//extracts filenames with '.fits' extension
int getL1Files(char *modedir,char ***fileadd,int *numberoffiles){
    int i=0;
    if(!(DirExists(modedir))) { LOG(ERROR)<<"***"<<modedir<<" does not exists***"; return -1;}  //checking whether dir exists
   
    struct dirent **namelist;
    i=scandir((const char *)modedir,&namelist,fitsfilter,alphasort);   //n is the number of mode directories present inside czti
    *numberoffiles=i;                                                                              //will filter files with fits extension only                              
    
    if(i<=0){
        LOG(ERROR)<<"***Level 1 Data Files not found in "<<modedir<<" ***";
        return -1;
    }
    char **file=allocateMemory<char>(*numberoffiles,PATHSIZE);
    while(i--){
       strcpy(file[i],namelist[i]->d_name);     
    }
    /*
    for(i=0;i<*numberoffiles;i++){
          cout<<file[i];
    }*/
    free(namelist);
    *fileadd=&file[0];
    return 0;
}

int DirPath::setAllDirPaths(char indir[],char outpath[]){
     //Generating output directory name with absolute path
    strcpy(this->indir,indir);
    strcpy(this->outpath,outpath);
    
    char tempdir[PATHSIZE];
    int len=strlen(indir);
    if(indir[len-1]=='/') indir[len-1]='\0';
    strcpy(tempdir,indir);
    char *temp=strrchr(tempdir,FILEPATHSEPARATOR_C);
    if(temp!=NULL) temp++;  //if '/' is there then only move the temp  
    else{
        temp=new char[len];
        strcpy(temp,tempdir);
    }              //to remove file path separator from name
    len=strlen(temp);
    temp[len-1]='2';                 //suffix for level2 directory
    temp[len]='\0';
    joinStrings(outdir,3,outpath,FILEPATHSEPARATOR,temp);
    
    joinStrings(cztidir_in,3,indir,FILEPATHSEPARATOR,CZTIDIRNAME);
    if(!(DirExists(cztidir_in))){ 
        LOG(ERROR)<<"***"<<cztidir_in<<" directory does not exists***"; 
        return (EXIT_FAILURE);  //checking whether dir exists
    }
    joinStrings(cztidir_out,3,outdir,FILEPATHSEPARATOR,CZTIDIRNAME);
  
    joinStrings(auxdir_in,3,cztidir_in,FILEPATHSEPARATOR,"aux");
    if(!(DirExists(auxdir_in))){
        LOG(ERROR)<<"***"<<auxdir_in<<" directory not found***";
        return(EXIT_FAILURE);
    }
    joinStrings(auxdir_out,3,cztidir_out,FILEPATHSEPARATOR,"aux");
   
    joinStrings(aux1dir_in,3,auxdir_in,FILEPATHSEPARATOR,"aux1");
    if(!(DirExists(aux1dir_in))){
        LOG(ERROR)<<"***"<<aux1dir_in<<" directory not found***";
        return(EXIT_FAILURE);
    }
    joinStrings(aux1dir_out,3,auxdir_out,FILEPATHSEPARATOR,"aux1");
   
    joinStrings(aux2dir_in,3,auxdir_in,FILEPATHSEPARATOR,"aux2");
    if(!(DirExists(aux2dir_in))){
        LOG(ERROR)<<"***"<<aux2dir_in<<" directory not found***";
        return(EXIT_FAILURE);
    }
    joinStrings(aux2dir_out,3,auxdir_out,FILEPATHSEPARATOR,"aux2");
    
    joinStrings(aux3dir_in,3,auxdir_in,FILEPATHSEPARATOR,"aux3");
    if(!(DirExists(aux3dir_in))){
        LOG(ERROR)<<"***"<<aux3dir_in<<" directory not found***";
        return(EXIT_FAILURE);
    }
    joinStrings(aux3dir_out,3,auxdir_out,FILEPATHSEPARATOR,"aux3");
   
    char mm[3];             
    for(int i=0;i<NUMOFMODES;i++) modepresent[i]=false;
    
    //finding mode directories
    int n_modedir,modeid;
    char dirname[PATHSIZE];    
       
    struct dirent **namelist;
    n_modedir=scandir((const char *)cztidir_in,&namelist,modefilter,alphasort);   //n is the number of mode directories present inside czti
    if(n_modedir<0){
        LOG(ERROR)<<"***Mode Directories not found in "<<cztidir_in<<" directory***";
        return (EXIT_FAILURE);
    }
    while(n_modedir>0){
        string temp;
        temp.assign(namelist[n_modedir-1]->d_name);
        temp=temp.substr(temp.size()-2,2);   //for mode id
        //Temprarily added
        modeid = get_modeID(temp);
        //Temporarily added
        //modeid=atoi(temp.c_str());
        modepresent[modeid]=true;
        joinStrings(dirname,3,cztidir_in,FILEPATHSEPARATOR,namelist[n_modedir-1]->d_name);
        LOG(INFO) << modeid << " " << dirname;
        //for input directories
        modedir_in[modeid].assign(dirname);
        //for output directories
        joinStrings((char *)dirname,3,cztidir_out,FILEPATHSEPARATOR,namelist[n_modedir-1]->d_name);
        modedir_out[modeid].assign(dirname);
        n_modedir--;  
    }
    return (EXIT_SUCCESS);
}

 int DirPath::createOutDir(){
   
    LOG(INFO)<<"Creating output directories......"; 
     
    if(mkdir(outdir,S_IRWXU|S_IRWXG|S_IROTH|S_IXOTH)!=0){
        LOG(ERROR)<<"***Error in creating output directory "<<outdir<<"***";
        return -1;
    }
    
    //creating output czti directory
    if(mkdir(cztidir_out,S_IRWXU|S_IRWXG|S_IROTH|S_IXOTH)!=0){
        LOG(ERROR)<<"***Error in creating czti directory***";
        return -1;
    }
   
    //aux directory
    if(mkdir(auxdir_out,S_IRWXU|S_IRWXG|S_IROTH|S_IXOTH)!=0){
        LOG(ERROR)<<"***Error in creating aux directory***";
        return -1;
    }
    
    //aux1 directory
    if(mkdir(aux1dir_out,S_IRWXU|S_IRWXG|S_IROTH|S_IXOTH)!=0){
        LOG(ERROR)<<"***Error in creating aux1 directory***";
        return -1;
    }
    
    //aux2 directory
    if(mkdir(aux2dir_out,S_IRWXU|S_IRWXG|S_IROTH|S_IXOTH)!=0){
        LOG(ERROR)<<"***Error in creating aux2 directory***";
        return -1;
    }
    
    //aux3 directory
    if(mkdir(aux3dir_out,S_IRWXU|S_IRWXG|S_IROTH|S_IXOTH)!=0){
        LOG(ERROR)<<"***Error in creating aux3 directory***";
        return -1;
    }
    
    //creating mode directories
    for(int i=0;i<NUMOFMODES;i++){
        if(modepresent[i]){
            if(mkdir(modedir_out[i].c_str(),S_IRWXU|S_IRWXG|S_IROTH|S_IXOTH)!=0){
                LOG(ERROR)<<"***Error in creating "<<modedir_out[i]<<" directory***";
                return -1;
             }
        }
    }
    return (EXIT_SUCCESS);
 }

void DirPath::display(){
    LOG(INFO)<<"-------------------DIRECTORY PATHS-----------------------";
    LOG(INFO)<<"INDIR       : "<<indir;
    LOG(INFO)<<"OUTPATH     : "<<outpath;
    LOG(INFO)<<"OUTDIR      : "<<outdir;
    LOG(INFO)<<"CZTI INDIR  : "<<cztidir_in;
    LOG(INFO)<<"CZTI_OUTDIR : "<<cztidir_out;
    LOG(INFO)<<"HK INDIR    : "<<hkdir_in;
    LOG(INFO)<<"HK OUTDIR   : "<<hkdir_out;
    LOG(INFO)<<"AUX INDIR   : "<<auxdir_in;
    LOG(INFO)<<"AUX OUTDIR  : "<<auxdir_out;
    LOG(INFO)<<"AUX1 INDIR  : "<<aux1dir_in;
    LOG(INFO)<<"AUX1 OUTDIR : "<<aux1dir_out;
    LOG(INFO)<<"AUX2 INDIR  : "<<aux2dir_in;
    LOG(INFO)<<"AUX2 OUTDIR : "<<aux2dir_out;
    LOG(INFO)<<"AUX3 INDIR  : "<<aux3dir_in;
    LOG(INFO)<<"AUX3 OUTDIR : "<<aux3dir_out;
    LOG(INFO)<<"Input Mode Directories";
    for(int i=0;i<NUMOFMODES;i++){
        if(modepresent[i]){
            LOG(INFO)<<"       "<<modedir_in[i].c_str();
        }
    }
    LOG(INFO)<<"Output Mode Directories";
    for(int i=0;i<NUMOFMODES;i++){
        if(modepresent[i]){
            LOG(INFO)<<"       "<<modedir_out[i].c_str();
        }
    }
    LOG(INFO)<<"---------------------------------------------------------";
 }

void  CALDBdatafile::getCaldbFiles(char *CaldbDirPath){
    
    joinStrings(goFile,1,"AS1cztgain20150526v01.fits");
    joinStrings(badpixFile,1,"AS1cztbadpix20150526v01.fits");
    joinStrings(eboundsFile,1,"AS1cztebounds20150526v01.fits");
    joinStrings(maskfile64x64,1,"CZTIMask64x64.fits");
    joinStrings(compMaskFile,1,"AS1cztmask_pattern20150526v01.fits");
    joinStrings(cameraGeomFile,1,"AS1cztcamerageo20150629v01.fits");
    joinStrings(effAreaFile, 1, "AS1czteff_area20150617v01.fits");
    joinStrings(teldeffileQ0,1,"AS1czt020151002v01.teldef");
    joinStrings(teldeffileQ1,1,"AS1czt120151002v01.teldef");
    joinStrings(teldeffileQ2,1,"AS1czt220151002v01.teldef");
    joinStrings(teldeffileQ3,1,"AS1czt320151002v01.teldef");
    joinStrings(xrayCatalog,1,"BAT_58m_catalog_100924_new.fits.gz");
}

int CALDBdatafile::checkExistence(){
    if(!FileExists(goFile)){
        LOG(ERROR)<<"***"<<goFile<<" not found***";
        return (EXIT_FAILURE);
    }

    if(!FileExists(badpixFile)){
        LOG(ERROR)<<"***"<<badpixFile<<" not found***";
        return (EXIT_FAILURE);
    }
    if(!FileExists(eboundsFile)){
        LOG(ERROR)<<"***"<<eboundsFile<<" not found***";
        return (EXIT_FAILURE);
    }
    if (!FileExists(maskfile64x64)) {
        LOG(ERROR) << "***" << maskfile64x64 << " not found***";
        return (EXIT_FAILURE);
    }
    if (!FileExists(compMaskFile)) {
        LOG(ERROR) << "***" << compMaskFile << " not found***";
        return (EXIT_FAILURE);
    }
    if (!FileExists(teldeffileQ0)) {
        LOG(ERROR) << "***" << teldeffileQ0 << " not found***";
        return (EXIT_FAILURE);
    }
    if (!FileExists(teldeffileQ1)) {
        LOG(ERROR) << "***" << teldeffileQ1 << " not found***";
        return (EXIT_FAILURE);
    }
    if (!FileExists(teldeffileQ2)) {
        LOG(ERROR) << "***" << teldeffileQ2 << " not found***";
        return (EXIT_FAILURE);
    }
    if (!FileExists(teldeffileQ3)) {
        LOG(ERROR) << "***" << teldeffileQ3 << " not found***";
        return (EXIT_FAILURE);
    }
    if(!FileExists(xrayCatalog)){
        LOG(ERROR)<<"***"<<xrayCatalog<<" not found***";
        return (EXIT_FAILURE);
    }
    return (EXIT_SUCCESS);
}

void CALDBdatafile::display(){
    LOG(INFO)<<"---------------------CALDB files used--------------------------------------";
    LOG(INFO)<<"Gain Offset File                : " << goFile;
    LOG(INFO)<<"Badpixel File                   : " << badpixFile;
    LOG(INFO)<<"Ebounds File                    : " << eboundsFile;
    LOG(INFO)<<"Compressed Mask File            : " << compMaskFile;
    LOG(INFO)<<"Mask File (64x64)               : "<<maskfile64x64;
    LOG(INFO)<<"Effective Area File             : "<<effAreaFile;
    LOG(INFO)<<"Telescope Definition file (Q0)  : "<<teldeffileQ0;
    LOG(INFO)<<"Telescope Definition file (Q1)  : "<<teldeffileQ1;
    LOG(INFO)<<"Telescope Definition file (Q2)  : "<<teldeffileQ2;
    LOG(INFO)<<"Telescope Definition file (Q3)  : "<<teldeffileQ3;
    LOG(INFO)<<"Catalog file                    : "<<xrayCatalog;
    LOG(INFO)<<"----------------------------------------------------------------------------";
}

int cztpipeline::setAuxFiles(){
    
//    joinStrings(hkfile,4,objpath.hkdir_in,FILEPATHSEPARATOR,metafileprefix_in,".hk");
//    if(!FileExists(hkfile)){
//        LOG(ERROR)<<"***Could not find file "<<hkfile<<"***";
//        return (EXIT_FAILURE);
//    }
//    
    joinStrings(orbfile,4,objpath.aux1dir_in,FILEPATHSEPARATOR,metafileprefix_in,".orb");
    if(!FileExists(orbfile)){
        LOG(ERROR)<<"***Could not find file "<<orbfile<<"***";
        return (EXIT_FAILURE);
    }
    
    joinStrings(l2mkffile,4,objpath.cztidir_out,FILEPATHSEPARATOR,metafileprefix_out,".mkf");
    
    joinStrings(attfile,4,objpath.aux1dir_in,FILEPATHSEPARATOR,metafileprefix_in,".att");
    if(!FileExists(attfile)){
        LOG(ERROR)<<"***Could not find file "<<attfile<<"***";
        return (EXIT_FAILURE);
    }
    joinStrings(mkffile,6,indir, FILEPATHSEPARATOR, "czti/", FILEPATHSEPARATOR, metafileprefix_in, ".mkf" );
    if(!FileExists(mkffile)){
        LOG(ERROR)<<"***Could not find file "<<attfile<<"***";
        return (EXIT_FAILURE);
    }    
    
    
    joinStrings(tctfile,4,objpath.auxdir_in,FILEPATHSEPARATOR,metafileprefix_in,".tct");
    if(!FileExists(tctfile)){
        LOG(ERROR)<<"***Could not find file "<<tctfile<<"***";
        return (EXIT_FAILURE);
    }
    
    joinStrings(lbtfile,4,objpath.aux2dir_in,FILEPATHSEPARATOR,metafileprefix_in,".lbt");
    if(!FileExists(lbtfile)){
        LOG(ERROR)<<"***Could not find file "<<lbtfile<<"***";
        return (EXIT_FAILURE);
    }
    return (EXIT_SUCCESS);
}

int cztpipeline::copyAuxFiles(){
    string cmdstr;
    //for HK file
//    cmdstr="cp -r "+(string)hkfile+" "+(string)objpath.hkdir_out;
//    if(system(cmdstr.c_str())){
//        LOG(ERROR)<<"***Error copying "<<hkfile<<" to "<<objpath.hkdir_out<<"***";
//        return (EXIT_FAILURE);
//    }
    //for orbit file
    cmdstr="cp -r "+(string)orbfile+" "+(string)objpath.aux3dir_out;
    if(system(cmdstr.c_str())){
        LOG(ERROR)<<"***Error copying "<<orbfile<<" to "<<objpath.aux3dir_out<<"***";
        return (EXIT_FAILURE);
    }
    //for attitude file
    cmdstr="cp -r "+(string)attfile+" "+(string)objpath.aux1dir_out;
    if(system(cmdstr.c_str())){
        LOG(ERROR)<<"***Error copying "<<attfile<<" to "<<objpath.aux1dir_out<<"***";
        return (EXIT_FAILURE);
    }
    cmdstr="cp -r "+(string)tctfile+" "+(string)objpath.auxdir_out;
    if(system(cmdstr.c_str())){
        LOG(ERROR)<<"***Error copying "<<tctfile<<" to "<<objpath.auxdir_out<<"***";
        return (EXIT_FAILURE);
    }
    cmdstr="cp -r "+(string)mkffile+" "+(string)objpath.cztidir_out;
    if(system(cmdstr.c_str())){
        LOG(ERROR)<<"***Error copying "<<mkffile<<" to "<<objpath.cztidir_out<<"***";
        return (EXIT_FAILURE);
    }
        cmdstr="cp -r "+(string)lbtfile+" "+(string)objpath.aux1dir_out;
    if(system(cmdstr.c_str())){
        LOG(ERROR)<<"***Error copying "<<lbtfile<<" to "<<objpath.aux2dir_out<<"***";
        return (EXIT_FAILURE);
    }
    return (EXIT_SUCCESS);
}

int get_modeID(string temp){
    int modeid=0;
    if (temp=="MO"){
        modeid=0;
    }
    else if(temp=="M1"){
        modeid=1;
    }
    else if(temp=="M2"){
        modeid=2;
    }
    else if(temp=="M3"){
        modeid=3;
    }
    else if(temp=="M4"){
        modeid=4;
    }
    else if(temp=="M5"){
        modeid=5;
    }
    else if(temp=="M6"){
        modeid=6;
    }
    else if(temp=="M7"){
        modeid=7;
    }
    else if(temp=="SS"){
        modeid=8;
    }
    else if(temp=="M9"){
        modeid=9;
    }
    else if(temp=="MA"){
        modeid=10;
    }
    else if(temp=="MB"){
        modeid=10;
    }
    else if(temp=="MC"){
        modeid=11;
    }
    else if(temp=="MD"){
        modeid=12;
    }
    return modeid;
}

int extractTars(string tar_name, string &fileoutname) {
//    string tarname = tar_name;
//    string tar_date_data = tarname.substr(11, 8);
//    string tar_OBS_id_data = tarname.substr(19, 21);
//    string lastname="";
//    
//    fileoutname = tar_date_data + "_" + tar_OBS_id_data + "_level1";
//    LOG(INFO) << "Tar name: " << tar_date_data << endl;
//    LOG(INFO) << "Tar OBS_id: " << tar_OBS_id_data << endl;
//    LOG(INFO) << "Final directory name: " << fileoutname << endl;
//    string strtemp(tarname);
//    int pos = strtemp.find_last_of("/");
//    string tempFilename;
//    string cmd;
//    if (pos > 0 && pos < strtemp.length()) {
//        tempFilename = strtemp.substr(0, pos);
//        cmd = "tar -xvf  " + tarname + " --directory =" + tempFilename;
//    } else {
//        cmd = "tar -xvf  " + tarname;
//    }
//    
//    cout << cmd << endl;
//    system(cmd.c_str());
//
//
//
//    return (EXIT_SUCCESS);
    string tarname = tar_name;


    //    cout<<"Tar name "<<tar_date_data<<endl;
    //     cout<<"Tar OBS_id "<<tar_OBS_id_data<<endl;
    //    cout<<"Final directory name "<<fileoutname<<endl;
    string strtemp(tarname);
    int pos = strtemp.find_last_of("/");
    string tempFilename = "";
    string cmd;
    string tar_date_data = tarname.substr(pos + 1 + 11, 8);
    string tar_OBS_id_data = tarname.substr(pos + 1 + 19, 21);

    if (pos > 0 && pos < strtemp.length()) {
        tempFilename = strtemp.substr(0, pos);
        cmd = "tar -xvf  " + tarname + " --directory=" + tempFilename;
        fileoutname = tempFilename + "/" + tar_date_data + "_" + tar_OBS_id_data + "_level1";
        //  cout<<"P P "<<fileoutname<<endl;
    } else {

        cmd = "tar -xvf  " + tarname;
        fileoutname = tar_date_data + "_" + tar_OBS_id_data + "_level1";
    }

    cout << cmd << endl;
    //   cout<<fileoutname<<endl;
    system(cmd.c_str());



    return (EXIT_SUCCESS);    
}
