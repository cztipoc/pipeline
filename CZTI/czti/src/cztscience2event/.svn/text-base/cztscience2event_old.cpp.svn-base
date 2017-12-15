

#include "cztscience2event.h"

using namespace std;

cztscience2event::cztscience2event() {
    strcpy(modulename, "cztscience2event_v");
    strcat(modulename, VERSION);
}

int cztscience2event::read(int argc, char **argv) {
    int status = 0;


    if (PIL_OK != (status = PILInit(argc, argv))) {
        LOG(ERROR) << "***Error Initializing PIL***";
        return status;
    }

    if (PIL_OK != (status = PILGetFname("infile", infile))) {
        LOG(ERROR) << "***Error Reading Science data file:" << infile << "***";
        return status;
    }

    if (PIL_OK != (status = PILGetFname("TCTfile", TCTfile))) {
        LOG(ERROR) << "***Error Reading TCT file:" << TCTfile << "***";
        return status;
    }

    if (PIL_OK != (status = PILGetFname("outfile", outfile))) {
        LOG(ERROR) << "***Error Reading eventfile:" << outfile << "***";
        return status;
    }

    if (PIL_OK != (status = PILGetFname("hdrInfoFile", hdrInfoFile))) {
        LOG(ERROR) << "***Error Reading eventfile:" << hdrInfoFile << "***";
        return status;
    }

    if (PIL_OK != (status = PILGetFname("GTIfile", GTIfile))) {
        LOG(ERROR) << "***Error Reading output GTI file:" << GTIfile << "***";
        return status;
    }

    if (PIL_OK != (status = PILGetFname("BTIfile", BTIfile))) {
        LOG(ERROR) << "***Error Reading output BTI file:" << BTIfile << "***";
        return status;
    }


    if (PIL_OK != (status = PILGetBool("BigEndian", &BigEndian))) {
        LOG(ERROR) << "***Error Reading BigEndian parameter" << BigEndian << "***";
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

int cztscience2event::read(char *infile, char *TCTfile, char *outfile, char* hdrInfoFile,
        char *gtifile, char *btifile, int bigendian, int clobber, int history) {

    if (strcpy(this->infile, infile) == NULL) {
        LOG(ERROR) << "***Error while copying input filename***";
        return (EXIT_FAILURE);
    }

    if (strcpy(this->TCTfile, TCTfile) == NULL) {
        LOG(ERROR) << "***Error while copying TCT filename***";
        return (EXIT_FAILURE);
    }

    if (strcpy(this->outfile, outfile) == NULL) {
        LOG(ERROR) << "***Error while copying output filename***";
        return (EXIT_FAILURE);
    }

    if (strcpy(this->hdrInfoFile, hdrInfoFile) == NULL) {
        LOG(ERROR) << "***Error while copying header information filename***";
        return (EXIT_FAILURE);
    }

    if (strcpy(this->GTIfile, gtifile) == NULL) {
        LOG(ERROR) << "***Error while copying output BTI filename***";
        return (EXIT_FAILURE);
    }

    if (strcpy(this->BTIfile, btifile) == NULL) {
        LOG(ERROR) << "***Error while copying output BTI filename***";
        return (EXIT_FAILURE);
    }
    this->clobber = clobber;
    this->history = history;
    this->BigEndian = bigendian;
    return (EXIT_SUCCESS);

}

void cztscience2event::display() {
    LOG(INFO) << "----------------------------------------------------------------------------";
    LOG(INFO) << "                           SCIENCEDATA2EVENT PARAMETERS                     ";
    LOG(INFO) << "----------------------------------------------------------------------------";
    LOG(INFO) << "Modulename              : " << modulename;
    LOG(INFO) << "Science data file       : " << infile; //input science data file 
    LOG(INFO) << "TCT file                : " << TCTfile;
    LOG(INFO) << "Output Event file       : " << outfile; //output event file
    LOG(INFO) << "Header Inforamtion file : " << hdrInfoFile;
    LOG(INFO) << "Output GTI file         : " << GTIfile;
    LOG(INFO) << "Output BTI file         : " << BTIfile;
    LOG(INFO) << "BigEndian               : " << BigEndian;
    LOG(INFO) << "Clobber                 : " << clobber;
    LOG(INFO) << "History                 : " << history;
    LOG(INFO) << "---------------------------------------------------------------------------";

}

int FramesInDataFile;
int invalidForEventData;

int cztscience2event::cztscience2eventProcess() {
    int flag = 0, i = 0, j = 0;
    int hdunum = 0;
    long nrows = 0;
    double coef[2];
    fitsfile *finptr; // file pointer to input Level-1 science data file.
    fitsfile *foutptr; // file pointer to output Level-2 science event file.
    fitsfile *fouthdr; // file pointer to ouput Level-2 header file
    int status = 0; // status variable to keep track of program status.
    unsigned short hdrRowNo = 0;


    if (clobber == YES) {
        if (FileExists(outfile)) {
            LOG(INFO) << outfile << "  :FileExists ";
            if (unlink(outfile) != 0) {
                LOG(ERROR)<<"***Error in deleting "<<outfile<<"***"; return (EXIT_FAILURE);
            }
        }
        if (FileExists(GTIfile)) {
            if (unlink(GTIfile) != 0) {
                LOG(ERROR)<<"***Error in deleting "<<GTIfile<<"***"; return (EXIT_FAILURE);
            }
        }
        if (FileExists(hdrInfoFile)) {
            if (unlink(hdrInfoFile) != 0) {
                LOG(ERROR)<<"***Error in deleting "<<GTIfile<<"***"; return (EXIT_FAILURE);
            }
        }
        if (FileExists(BTIfile)) {
            if (unlink(BTIfile) != 0) {
                LOG(ERROR)<<"***Error in deleting "<<BTIfile<<"***"; return (EXIT_FAILURE);
            }
        }
    } else {
        if (FileExists(outfile) || FileExists(GTIfile) || FileExists(BTIfile) || FileExists(hdrInfoFile)) {
            LOG(ERROR) << "***Output file already exists***";
            LOG(ERROR) << "Use clobber=yes for overwriting the file";
            return (EXIT_FAILURE);
        }
    }

    /************************************************************************************
     * Extracting information from level-1 science data file.
     
     ************************************************************************************/
    fits_open_file(&finptr, infile, READONLY, &status);
    if (status) {
        LOG(ERROR) << "***Error in opening level1 science data file " << infile << "***";
        fits_report_error(stderr, status);
        return (EXIT_FAILURE);
    }

    flag = createLevel2DataFile(); //creating empty event file
    if (flag) {
        LOG(ERROR) << "Error in creating level2 event data file***";
        return (EXIT_FAILURE);
    }
    LOG(INFO) << "Level 2 data file structure created.";


    fits_open_file(&foutptr, outfile, READWRITE, &status); // opening empty event file
    if (status) {
        LOG(ERROR) << "***Error in opening level2 event data file***";
        return (EXIT_FAILURE);
    }


    flag = createLevel2HeaderFile(); // creating level-2 empty header file
    if (flag) {
        LOG(ERROR) << "*** Error in creating level 2 event header file***";
        return (EXIT_FAILURE);
    }

    LOG(INFO) << "Level 2 header file structure created.";

    fits_open_file(&fouthdr, hdrInfoFile, READWRITE, &status); //opening empty header file
    if (status) {
        LOG(ERROR) << "***Error in opening level 2 header file ***";
        return (EXIT_FAILURE);
    }
    fits_movabs_hdu(fouthdr, 2, NULL, &status);

    for (hdunum = 2; hdunum <= 5; hdunum++) {
        int hdutype = 0;
        fits_movabs_hdu(finptr, hdunum, &hdutype, &status);
        if (status) {
            LOG(ERROR) << "***Error moving to hdu number" << hdunum << " HDU in Science data file***";
            fits_report_error(stderr, status);
            return (EXIT_FAILURE);
        }
        if (hdutype != BINARY_TBL) {
            LOG(ERROR) << "*** HDU" << hdunum << " in science data file " << infile << " is not binary table***";
            return (EXIT_FAILURE);
        }

        unsigned char **datapackets;
        datapackets = allocateMemory<unsigned char>(PACKETBUFFER, PACKETSIZE);
        if (datapackets == NULL) {
            LOG(ERROR) << "***Out of Memory Error: cztscinece2event_process() ***";
            return (EXIT_FAILURE);
        }
        fits_get_num_rows(finptr, &nrows, &status);
        if (status) {
            LOG(ERROR) << "***Error reading number of rows from file " << infile << "***";
            fits_report_error(stderr, status);
            return (EXIT_FAILURE);
        }

        if (nrows == 0) {
            LOG(INFO) << "HDU " << hdunum << " has no records";
            continue;
        }


        LOG(INFO) << "Number of rows in HDU " << hdunum << " :" << nrows;
        
        Frame f;
        long framenumber_curr = 0;
        int lastpacketwithzeropacketno = 1;
        int validframes = 0, invalidframes = 0;
        long startRow = 1, endRow = 0; //to store start row and end row number while reading in buffer.
        FramesInDataFile = 0;
        invalidForEventData = 0;
        Frame::count = 0;
        int missingFrames = 0;
        vector<int> missingFrameList;
        int nextframe = 0;


        do {
            /* If number of rows in one HDU of level-1 science file is less than PACKETBUFFER
             * then then all the records in that table are read. Otherwise data is read in buffers
             * of 10000 lines at a time.
             */
            endRow = (nrows <= (startRow + PACKETBUFFER)) ? nrows : startRow + PACKETBUFFER - 1;
            LOG(INFO) << "startrow:" << startRow << " endrow:" << endRow;
            flag = 0;
            flag = getPackets(finptr, datapackets, startRow, endRow); //getting packets from level-1 science data file and storing it in datapackets.
            if (flag) {
                LOG(ERROR) << "***Error in reading packets from input file***";
                return (EXIT_FAILURE);
            }

            /* Reading Packets one row at a time.
             * STEP1: each packet gets decoded to obtain following values:
             * STEP2: data is extracted framewise, such that all data corresponding
             *        to a particular frame number is stored in that frame.
             */
            for (j = startRow, i = 0; j <= endRow; j++, i++) {
                DLOG(INFO) << "Decoding Packet Number:" << j;
                p.decode(datapackets[i], BigEndian); //to decode ith datapacket
                
                if (Frame::count > 0 && p.isFirstpacket) {
                    if (f.isValid) { 
                        if (writeData(f, foutptr, (string) TCTfile, coef)) {
                            LOG(ERROR) << "***Error in writing frame no " << f.frameno << " to output event file***";
                            return (EXIT_FAILURE);
                        }
                        validframes++;

                        //cout<<"Frame Number:"<<Frame::count<<"\tSize:"<<f.size;
                    } else {
                        LOG(ERROR) << "\t Invalid frames :" << invalidframes;
                        invalidframes++;
                    }
                }
                if (p.isFirstpacket) { //First packet of a frame contains frame header of 16 words
                    Frame::count++; //setting frame data
                    lastpacketwithzeropacketno = j;
                    f.setValues(p);
                    framenumber_curr = p.framecounter;
                    f.startpkt = j;
                    hdrRowNo++;
                    writeHdrInfo(fouthdr, (string) TCTfile, coef, hdrRowNo);
                    
                }
                if (Frame::count == 0) continue; //if first packet is not frame then go for next packet
                f.size = f.size + p.wcnt;
                //copying packet data to its frame
                for (int k = 0; k < p.wcnt; k++, f.frameindex++) {
                    if (f.frameindex > MAXFRAMEDATASIZE) {
                        LOG(ERROR) << "***FRAME NUMBER IS:" << f.frameindex << "***";
                        LOG(ERROR) << "This is greater than maximum number of packets allowed in a frame";
                        return (EXIT_FAILURE);
                    }
                    f.framedata[f.frameindex] = p.hbtdata[k];   //storing packet data into its corresponding frame
                }

                //-------frame completed   

                // CHECKING MISSING FRAME LIST
                nextframe = nextframe + 1;
                if (nextframe != f.frameno) {
                    if (f.frameno - nextframe > 0) {
                        // LOG(INFO) << f.frameno - nextframe << " frames missing before frame no " << f.frameno;
                        missingFrames = missingFrames + (f.frameno - nextframe);
                        for (int m = nextframe; m < f.frameno; m++)
                            missingFrameList.push_back(m);
                    }
                    nextframe = f.frameno;
                }
                // END CHECKING MISSING FRAME LIST
            }
            startRow = endRow + 1;

        } while (endRow < nrows);
        
        
        if (f.isValid && Frame::count > (invalidframes + validframes)) {
            flag = writeData(f, foutptr, (string) TCTfile, coef); //forlast frame
            if (flag) {
                LOG(ERROR) << "***Error in writing frame no " << f.frameno << " to output event file***";
                return (EXIT_FAILURE);
            }
            validframes++;
        } else
            invalidframes++;

        LOG(INFO) << "Total Frame Count----------------" << Frame::count;
        LOG(INFO) << "Valid Frames---------------------" << validframes;
        LOG(INFO) << "Invalid Frames-------------------" << invalidframes;
        LOG(INFO) << "Frames In Data File--------------" << FramesInDataFile;
        LOG(INFO) << "Frames Invalid for Event Data----" << invalidForEventData;
    }
    fits_close_file(fouthdr, &status);

    /**
     * Code for copying keywords from level1 input file to level2 output file
     * in particular HDUs
     */
    status = 0;
    LOG(INFO) << "Writing required keywords from Level-1 input file to output file.......";
    fits_movabs_hdu(finptr, 1, NULL, &status);
    if (status) {
        fits_report_error(stderr, status);
        return (EXIT_FAILURE);
    }

    fits_movabs_hdu(foutptr, 1, NULL, &status);
    if (status) {
        fits_report_error(stderr, status);
        return (EXIT_FAILURE);
    }

    if (copyUserKeyWords(finptr, foutptr)) {
        LOG(ERROR) << "***Error in copying keywords from input to output file***";
        return (EXIT_FAILURE);
    } //for primary HDU
    fits_write_chksum(foutptr, &status);
    fits_update_key(foutptr, TSTRING, "CREATOR", modulename, "Module that created this file", &status);
    fits_update_key(foutptr, TSTRING, "ORIGIN", (char *) ORIGIN, NULL, &status);
    fits_update_key(foutptr, TSTRING, "INSTRUME", (char *) INSTRUME, NULL, &status);

    //writing history to primary HDU of output file
    if (history == YES) {
        vector<string> vhistory;
        getHistory(vhistory);
        for (int j = 0; j < vhistory.size(); j++) {
            fits_write_history(foutptr, vhistory[j].c_str(), &status);
            if (status) {
                fits_report_error(stderr, status);
                return (EXIT_FAILURE);
            }
        }
    }

    fits_movabs_hdu(finptr, 2, NULL, &status);
    if (status) {
        fits_report_error(stderr, status);
        LOG(ERROR) << "***Error moving to 2nd HDU: " << infile << "***";
        return (EXIT_FAILURE);
    }
    int numtemp;
    char str[5];
    double tstart, tstop;
    fits_get_num_hdus(foutptr, &numtemp, &status);

    //code for writing keywords in output file extensions
    //and write TSTART and TSTOP for each extension
    for (int i = 2; i <= 7; i++) { //for extensions 2 to 7 in output file
        sprintf(str, "%d", i);
        fits_movabs_hdu(foutptr, i, NULL, &status);
        if (status) {
            fits_report_error(stderr, status);
            return (EXIT_FAILURE);
        }

        //finding TSTART and TSTOP
        fits_get_num_rows(foutptr, &nrows, &status);
        if (status) {
            fits_report_error(stderr, status);
            return (EXIT_FAILURE);
        }

        if (nrows > 0) {
            fits_read_col(foutptr, TDOUBLE, 1, 1, 1, 1, NULL, &tstart, NULL, &status);
            if (status) {
                fits_report_error(stderr, status);
                return (EXIT_FAILURE);
            }

            fits_read_col(foutptr, TDOUBLE, 1, nrows, 1, 1, NULL, &tstop, NULL, &status);
            if (status) {
                fits_report_error(stderr, status);
                return (EXIT_FAILURE);
            }

            fits_update_key(foutptr, TDOUBLE, "TSTART", &tstart, NULL, &status);
            if (status) {
                fits_report_error(stderr, status);
                return (EXIT_FAILURE);
            }

            fits_update_key(foutptr, TDOUBLE, "TSTOP", &tstop, NULL, &status);
            if (status) {
                fits_report_error(stderr, status);
                return (EXIT_FAILURE);
            }

        }
        if (copyUserKeyWords(finptr, foutptr)) {
            LOG(ERROR) << "***Error in copying keywords from input to output file***";
            return (EXIT_FAILURE);
        }

        fits_write_chksum(foutptr, &status);
        fits_update_key(foutptr, TSTRING, "CREATOR", modulename, "Module that created this file", &status);
        fits_update_key(foutptr, TSTRING, "ORIGIN", (char *) ORIGIN, NULL, &status);

        if (history == YES) {
            vector<string> vhistory;
            getHistory(vhistory);
            for (int j = 0; j < vhistory.size(); j++) {
                fits_write_history(foutptr, vhistory[j].c_str(), &status);
                if (status) {
                    fits_report_error(stderr, status);
                    return (EXIT_FAILURE);
                }
            }
        }
    }
    fits_close_file(foutptr, &status);
    if (status) {
        fits_report_error(stderr, status);
        return (EXIT_FAILURE);
    }
    //-----------------------output event file creation complete----------------
    fits_close_file(finptr, &status);
    if (status) {
        fits_report_error(stderr, status);
        return (EXIT_FAILURE);
    }
    createGTIBTIfile();

    return (EXIT_SUCCESS);
}

int cztscience2event::createLevel2HeaderFile() {
    LOG(INFO) << ">>> Creating level2 header file...";
    fitsfile *fhdr;
    int status = 0;

    
    string templateFileName = "";
    templateFileName = template_full_path_generator("hdrTemplate");
    
    if(templateFileName==""){
        LOG(ERROR)<< "Not able to generate Event file template path.";
        LOG(ERROR)<< "Probably Environment Variables are not declared properly.";
        return(EXIT_FAILURE);
    }    
    LOG(INFO) << "Template used: " << templateFileName;
    fits_create_template(&fhdr, hdrInfoFile, (char*) templateFileName.c_str(), &status);
    if (status) {
        LOG(ERROR) << "Error in creating header file.";
        fits_report_error(stderr, status);
        return (EXIT_FAILURE);
    }
    

    LOG(INFO) << "Header Information file: " << hdrInfoFile << "created";

    fits_close_file(fhdr, &status);
    if (status) {
        LOG(ERROR) << "***Error in closing level 2 header file: " << hdrInfoFile << " ***";
        fits_report_error(stderr, status);
        return (EXIT_FAILURE);
    }
    LOG(INFO) << "Level2 header file created <<<";
    return (EXIT_SUCCESS);
}

int cztscience2event::createLevel2DataFile() {
    LOG(INFO) << ">>> Creating Level2 event file...";
    fitsfile *fptr;
    int status = 0;
    string templateFilename = template_full_path_generator("evtTemplate");
    if(templateFilename==""){
        LOG(ERROR)<< "Not able to generate Event file template path.";
        return(EXIT_FAILURE);
    }    
    LOG(INFO) << "Template used: " << templateFilename;
    fits_create_template(&fptr, outfile, (char*) templateFilename.c_str(), &status);
    if (status) {
        LOG(ERROR) << "***Error creating event file: " << outfile << "***";
        fits_report_error(stderr, status);
        return (EXIT_FAILURE);
    }

    fits_close_file(fptr, &status);
    if (status) {
        LOG(ERROR) << "***Error in closing level 2 event file: " << outfile << " ***";
        fits_report_error(stderr, status);
        return (EXIT_FAILURE);
    }
    LOG(INFO) << "Level2 event file created <<<";
    return (EXIT_SUCCESS);
}

int cztscience2event::createGTIBTIfile() {
    fitsfile *fptr;
    fitsfile *fptr_in;
    int status = 0;
    fits_open_file(&fptr_in, infile, READONLY, &status);
    if (status) {
        LOG(ERROR) << "Error in opening file " << infile << "***";
        fits_report_error(stderr, status);
        return (EXIT_FAILURE);
    }

    long naxes[2];
    naxes[0] = 0;
    naxes[1] = 0;
    //for GTI file
    fits_create_file(&fptr, GTIfile, &status);
    if (status) {
        LOG(ERROR) << "***Error creating file " << GTIfile << "***";
        fits_report_error(stderr, status);
        return (EXIT_FAILURE);
    }

    fits_create_img(fptr, 8, 0, naxes, &status);
    if (status) {
        fits_report_error(stderr, status);
        return (EXIT_FAILURE);
    }
    copyUserKeyWords(fptr_in, fptr);

    fits_movnam_hdu(fptr_in, BINARY_TBL, (char *) "GTI", 0, &status);
    if (status) {
        fits_report_error(stderr, status);
        return (EXIT_FAILURE);
    }

    fits_copy_hdu(fptr_in, fptr, 0, &status);
    if (status) {
        fits_report_error(stderr, status);
        return (EXIT_FAILURE);
    }

    for (int i = 1; i <= 2; i++) {
        fits_movabs_hdu(fptr, i, NULL, &status);
        if (status) {
            fits_report_error(stderr, status);
            return (EXIT_FAILURE);
        }
        fits_write_date(fptr, &status);
        fits_write_chksum(fptr, &status);
        fits_update_key(fptr, TSTRING, "CREATOR", modulename, "Module that created this file", &status);
        fits_update_key(fptr, TSTRING, "ORIGIN", (char *) ORIGIN, NULL, &status);
        if (history == YES) {
            vector<string> vhistory;
            getHistory(vhistory);
            for (int j = 0; j < vhistory.size(); j++) {
                fits_write_history(fptr, vhistory[j].c_str(), &status);
                if (status) {
                    fits_report_error(stderr, status);
                    return (EXIT_FAILURE);
                }
            }
        }
    }

    fits_close_file(fptr, &status);
    if (status) {
        fits_report_error(stderr, status);
        return (EXIT_FAILURE);
    }

    //for BTI file
    fits_create_file(&fptr, BTIfile, &status); //BTI file creation
    if (status) {
        LOG(ERROR) << "***Error creating file " << BTIfile << "***";
        fits_report_error(stderr, status);
        return (EXIT_FAILURE);
    }

    fits_create_img(fptr, 8, 0, naxes, &status);
    if (status) {
        fits_report_error(stderr, status);
        return (EXIT_FAILURE);
    }

    fits_movabs_hdu(fptr_in, 1, NULL, &status); //moving to first HDU of input science data file to copy required keywords
    if (status) {
        fits_report_error(stderr, status);
        return (EXIT_FAILURE);
    }

    copyUserKeyWords(fptr_in, fptr);

    fits_movnam_hdu(fptr_in, BINARY_TBL, (char *) "BTI", 0, &status);
    if (status) {
        fits_report_error(stderr, status);
        return (EXIT_FAILURE);
    }

    fits_copy_hdu(fptr_in, fptr, 0, &status);
    if (status) {
        fits_report_error(stderr, status);
        return (EXIT_FAILURE);
    }

    for (int i = 1; i <= 2; i++) {
        fits_movabs_hdu(fptr, i, NULL, &status);
        if (status) {
            fits_report_error(stderr, status);
            return (EXIT_FAILURE);
        }
        fits_write_date(fptr, &status);
        fits_write_chksum(fptr, &status);
        fits_update_key(fptr, TSTRING, "CREATOR", modulename, "Module that created this file", &status);
        fits_update_key(fptr, TSTRING, "ORIGIN", (char *) ORIGIN, NULL, &status);
        if (history == YES) {
            vector<string> vhistory;
            getHistory(vhistory);
            for (int j = 0; j < vhistory.size(); j++) {
                fits_write_history(fptr, vhistory[j].c_str(), &status);
                if (status) {
                    fits_report_error(stderr, status);
                    return (EXIT_FAILURE);
                }
            }
        }
    }

    fits_close_file(fptr, &status);
    if (status) {
        fits_report_error(stderr, status);
        return (EXIT_FAILURE);
    }
    fits_close_file(fptr_in, &status);
    if (status) {
        fits_report_error(stderr, status);
        return (EXIT_FAILURE);
    }
    return (EXIT_SUCCESS);
}

int getPackets(fitsfile *fptr, unsigned char **pktarray,
        long startRowNumber, long endRowNumber) {
    //cout<<"Size of packet array:"<<m<<" x "<<n<<"";
    int status = 0;
    int colnum = DATA_ARRAY_COLNUM;
    int firstelem = 1;
    int nelements = PACKETSIZE * (endRowNumber - startRowNumber + 1);
    unsigned char *array = new unsigned char[nelements];
    if (array == NULL) {
        LOG(ERROR) << "Out of Memory Error: getPackets()-cztscience2event.cpp";
        return (EXIT_FAILURE);
    }
    fits_read_col(fptr, TBYTE, colnum, startRowNumber, firstelem, nelements, NULL,
            array, NULL, &status);
    if (status) {
        fits_report_error(stderr, status);
        return (EXIT_FAILURE);
    }

    for (int i = 0; i < nelements / PACKETSIZE; i++) {
        for (int j = 0; j < PACKETSIZE; j++)
            pktarray[i][j] = array[i * PACKETSIZE + j];
    }
    delete[] array;
    return (EXIT_SUCCESS);
}

void getFrameInfo(vector<Frame> &vframe) {
    LOG(INFO) << "---------------------------------------------";
    LOG(INFO) << "" << setw(8) << "FrameNo" << setw(10) << "Datasize" << setw(7) << "ModeID" << setw(7) << "dataID";
    for (int i = 0; i < vframe.size(); i++) {
        LOG(INFO) << "" << setw(8) << vframe[i].frameno;
        LOG(INFO) << setw(10) << vframe[i].size;
        LOG(INFO) << setw(7) << (int) vframe[i].modeID;
        LOG(INFO) << setw(7) << (int) vframe[i].dataID;
        LOG(INFO) << setw(5) << (int) vframe[i].isValid;
        LOG(INFO) << setw(12) << vframe[i].laxpctime;
        LOG(INFO) << setw(12) << vframe[i].secondcount;
    }
    LOG(INFO) << "---------------------------------------------";
}

int writeData(Frame &frame,fitsfile *fptr, string TCTfilename, double *coef){
    DLOG(INFO) << "Writing frame " << frame.frameno << " to event file;";
    int i,j=0; //counter variables
    double *time, *localtime;
    unsigned short *pha, *event_time, *veto;
    unsigned char *detid, *pixid, *alpha, *detx, *dety;
    int nevents, feb_veto_size, eventsize, vetospecstart = -1;
    int temp, status = 0;
    long nrows = 0;
    Tct TCT;
    if(TCT.read_tct_file(TCTfilename)){
        LOG(ERROR) << "*** Error reading TCT file***";
    }
    if(TCT.is_reset()){
        LOG(ERROR) << "*** Error in level-1 TCT file ***";
    }
    
    unsigned short *vetospec = new unsigned short[VETOSPEC_SIZE]; //Array for holding veto spectrum
    for (i = 0; i < VETOSPEC_SIZE; i++) {
        vetospec[i] = 0;
    }
    bool vetoSpecRange;
    double frametime=0.0; //veto spectrum contains 232 words
                                                               //counter variables
    //-----------------Decoding and writing Event Data-----------------------------
    switch (frame.modeID) {
        case 0:
        case 4: feb_veto_size = 240; //Detector data has 240 words for FEB header and Veto Spectrum
            eventsize = 3; //Each event consumes 3 words
            vetospecstart = 8;
            break;
        case 1:
        case 5: feb_veto_size = 8;
            eventsize = 3;
            vetospecstart = -1;
            break;
        case 2:
        case 6: eventsize = 2;
            feb_veto_size = 244;
            vetospecstart = 8;
            break;
        case 3:
        case 7: eventsize = 2;
            feb_veto_size = 8;
            vetospecstart = -1;
            break;
        default:
            LOG(INFO) << "" << frame.frameno << ". Mode ID " << (int) frame.modeID << " is not valid for event data";
            invalidForEventData++;
            return (EXIT_SUCCESS);
    }

    if ((frame.dataID >= 4 && frame.dataID <= 7) || (frame.dataID >= 12 && frame.dataID <= 16)) {
        LOG(INFO) << "" << frame.frameno << ". Data ID " << (int) frame.dataID << " is not valid for event data";
        invalidForEventData++;
        return (EXIT_SUCCESS);
    }
    FramesInDataFile++;
    if (frame.dataID >= 0 && frame.dataID <= 3) {

        //allocating memory to arrays for holding event data
        nevents = (frame.size - feb_veto_size) / eventsize;
        time = new double[nevents];
        event_time = new unsigned short[nevents];
        localtime = new double[nevents];
        pha = new unsigned short[nevents];
        detid = new unsigned char[nevents];
        pixid = new unsigned char[nevents];
        alpha = new unsigned char[nevents];
        veto = new unsigned short[nevents];
        detx = new unsigned char[nevents];
        dety = new unsigned char[nevents];

        //decoding events for normal and reduced mode
        for (i = feb_veto_size, j = 0; j < nevents; i += eventsize, j++) { //Each event is of three words in mode 0 & 4
            //for three word event report
            if (eventsize == 3) {
                localtime[j] = ((double) frame.framedata[i] / 50000.0)+(double) frame.secondcount; //conversion of fractional time into microseconds by multipyitn it by 20 microsec
                DLOG(INFO) << setprecision(12) << "Packet time: " << frame.framedata[i] << " secondCount: " << frame.secondcount << " LAXPC time:" << frame.laxpctime;
                event_time[j] = (unsigned short) frame.framedata[i];
                pha[j] = (unsigned short) (((frame.framedata[i + 1] >> 4) & 0xfff)>>2);
                detid[j] = (unsigned char) (frame.framedata[i + 1] & 0xf);
                pixid[j] = (unsigned char) ((frame.framedata[i + 2] >> 8) & 0xff);
                veto[j] = (unsigned short) ((frame.framedata[i + 2] >> 1) & 0x7f);
                alpha[j] = (unsigned char) (frame.framedata[i + 2] & 0x1);
            }
            //For two word event report 
            if (eventsize == 2) {
                temp = (frame.framedata[i] >> 7) & 0x1ff; //extracted 9 bits for time
                event_time[j] = (unsigned short) ((frame.framedata[i] &0xff80) >> 7);
                localtime[j] = ((double) temp / 50000.0)+(double) frame.secondcount;
                pha[j] = (unsigned short) ((((int) (frame.framedata[i] & 0x7f) << 2) | (int) ((frame.framedata[i + 1] >> 14) & 0x3)) << 1);
                detid[j] = (unsigned char) (frame.framedata[i + 1] & 0xf);
                pixid[j] = (unsigned char) ((frame.framedata[i + 1] >> 6) & 0xff);
                veto[j] = (unsigned short) ((frame.framedata[i + 1] >> 5) & 0x1);
                alpha[j] = (unsigned char) ((frame.framedata[i + 1] >> 4) & 0x1);
            }

        }
        //Computing detx and dety    
        //detx for column, dety for row
        //detx and dety defines the x,y coordinate of pixel in one quadrant

        for(i=0; i<nevents; i++){
            generate_detx_dety(detid[i], pixid[i], frame.quadID, detx[i], dety[i]);
        }
        

        DLOG(INFO)<<"Interpolating TCT time.";
        //finding UTC time
        for (i = 0; i < nevents; i++) {
            //time[i] = coef[0]+(pow(localtime[i], 1) * coef[1]);
//            if(TCT.interpolate_time(localtime[i], time[i], NO)){
//                LOG(ERROR)<<"Error in interpolating INSTRUMENT TIME: " << localtime[i];
//                return (EXIT_FAILURE);
//            }
            time[i]=0.0;
        }
        //writing events to data file
        status = 0;
        char str[100];
        fits_movabs_hdu(fptr, frame.dataID + 2, NULL, &status);
        if (status) {
            fits_report_error(stderr, status);
            return (EXIT_FAILURE);
        }

        fits_get_num_rows(fptr, &nrows, &status);
        if (status) {
            fits_report_error(stderr, status);
            return (EXIT_FAILURE);
        }

        fits_write_col(fptr, TDOUBLE, 1, nrows + 1, 1, nevents, time, &status);
        if (status) {
            fits_report_error(stderr, status);
            return (EXIT_FAILURE);
        }

        fits_write_col(fptr, TDOUBLE, 2, nrows + 1, 1, nevents, localtime, &status);
        if (status) {
            fits_report_error(stderr, status);
            return (EXIT_FAILURE);
        }


        fits_write_col(fptr, TUSHORT, 3, nrows + 1, 1, nevents, pha, &status);
        if (status) {
            fits_report_error(stderr, status);
            return (EXIT_FAILURE);
        }

        fits_write_col(fptr, TBYTE, 4, nrows + 1, 1, nevents, detid, &status);
        if (status) {
            fits_report_error(stderr, status);
            return (EXIT_FAILURE);
        }

        fits_write_col(fptr, TBYTE, 5, nrows + 1, 1, nevents, pixid, &status);
        if (status) {
            fits_report_error(stderr, status);
            return (EXIT_FAILURE);
        }

        fits_write_col(fptr, TUSHORT, 6, nrows + 1, 1, nevents, veto, &status);
        if (status) {
            fits_report_error(stderr, status);
            return (EXIT_FAILURE);
        }

        fits_write_col(fptr, TBYTE, 7, nrows + 1, 1, nevents, alpha, &status);
        if (status) {
            fits_report_error(stderr, status);
            return (EXIT_FAILURE);
        }

        fits_write_col(fptr, TBYTE, 8, nrows + 1, 1, nevents, detx, &status);
        if (status) {
            fits_report_error(stderr, status);
            return (EXIT_FAILURE);
        }

        fits_write_col(fptr, TBYTE, 9, nrows + 1, 1, nevents, dety, &status);
        if (status) {
            fits_report_error(stderr, status);
            return (EXIT_FAILURE);
        }

        fits_write_col(fptr, TUSHORT, 10, nrows + 1, 1, nevents, event_time, &status);
        if (status) {
            fits_report_error(stderr, status);
            return (EXIT_FAILURE);
        }

        delete[] time, pha, detx, dety, detid, pixid, veto, alpha;
    }
    //------------------------Decoding and writing vetospectrum----------------------------
    status = 0;
    fits_movabs_hdu(fptr, 6, NULL, &status); //Vetospectrum will be in 6th extension of events file
    if (status) {
        fits_report_error(stderr, status);
        return (EXIT_FAILURE);
    }

    if (vetospecstart == 8) {
        vetoSpecRange=(bool)((frame.framedata[0]>>11) & (0x1));
        if (vetoSpecRange == 0) {
            for (i = vetospecstart, j = 24; j < VETOSPEC_SIZE; i++, j++) {
                vetospec[j] = frame.framedata[i];
            }
        } else if(vetoSpecRange==1){
            for (i = vetospecstart, j = 0; j < VETOSPEC_SIZE-24; i++, j++) {
                vetospec[j] = frame.framedata[i];
            }
        }
        fits_get_num_rows(fptr, &nrows, &status);

        frametime = 0.0;
        for (int j = 0; j <= 1; j++){
            //frametime = frametime + pow((double) frame.secondcount, j) * coef[j];
            TCT.interpolate_time((double)frame.secondcount, frametime, NO);
        }  //converted to UTC
        fits_write_col(fptr, TDOUBLE, 1, nrows + 1, 1, 1, &frametime, &status);
        if (status) {
            fits_report_error(stderr, status);
            return (EXIT_FAILURE);
        }

        fits_write_col(fptr, TINT, 2, nrows + 1, 1, 1, &frame.secondcount, &status);
        if (status) {
            fits_report_error(stderr, status);
            return (EXIT_FAILURE);
        }

        fits_write_col(fptr, TUSHORT, 3, nrows + 1, 1, VETOSPEC_SIZE, vetospec, &status);
        if (status) {
            fits_report_error(stderr, status);
            return (EXIT_FAILURE);
        }
    }
    delete[] vetospec;

    /*Decoding Data for Secondary Spectral Mode (SSM) and writing to SSM extension of FITS file
     *Temperature data for SSM extension is also extracted and stored in TEMP extension.
     */
    if (frame.dataID >= 8 && frame.dataID <= 11) {
        DLOG(INFO)<<"SSM data at "<<frame.frameno;
        status = 0;
        // SSM Extension
        fits_movabs_hdu(fptr, 7, NULL, &status); //SSM data will be at 7th extension of data file
        if (status) {
            fits_report_error(stderr, status);
            return (EXIT_FAILURE);
        }

        fits_get_num_rows(fptr, &nrows, &status);
        if (status) {
            fits_report_error(stderr, status);
            return (EXIT_FAILURE);
        }

        frametime = 0.0;
        for (j = 0; j <= 1; j++) {
            //frametime = frametime + pow((double) frame.secondcount, j) * coef[j];
            TCT.interpolate_time((double) frame.secondcount, frametime, NO);
        } //converted to UTC
        //Writing time
        
        fits_write_col(fptr, TDOUBLE, 1, nrows + 1, 1, 1, &frametime, &status);
        if (status) {
            fits_report_error(stderr, status);
            return (EXIT_FAILURE);
        }
        fits_write_col(fptr, TINT, 2, nrows + 1, 1, 1, &frame.secondcount, &status);
        if (status) {
            fits_report_error(stderr, status);
            return (EXIT_FAILURE);
        }

        unsigned short *tempdata;
        //-----------Veto spectrum decoding and writing--------------------
        tempdata = new unsigned short[256]; //for 256 words of vetospectrum
        for (i = 0, j = 184; i < 256; j++, i++) tempdata[i] = frame.framedata[j]; //starts from 184th word in frame data
        fits_write_col(fptr, TUSHORT, 3, nrows + 1, 1, 256, tempdata, &status);
        if (status) {
            fits_report_error(stderr, status);
            return (EXIT_FAILURE);
        }

        //--------------CZT spectrum decoding and writing-----------------
        tempdata = new unsigned short[512]; //for 512 words of cztspectrum
        for (i = 0, j = 440; i < 512; j++, i++) tempdata[i] = frame.framedata[j]; //starts from 440th word in frame data
        fits_write_col(fptr, TUSHORT, 4, nrows + 1, 1, 512, tempdata, &status);
        if (status) {
            fits_report_error(stderr, status);
            return (EXIT_FAILURE);
        }

        //-----------CZT spectrum with veto decoding and writing--------------
        tempdata = new unsigned short[512]; //for 512 words of cztspectrum with veto
        for (i = 0, j = 952; i < 512; j++, i++) tempdata[i] = frame.framedata[j]; //starts from 952nd word in frame data 
        fits_write_col(fptr, TUSHORT, 5, nrows + 1, 1, 512, tempdata, &status);
        if (status) {
            fits_report_error(stderr, status);
            return (EXIT_FAILURE);
        }

        //-------------CZT spectrum with alpha and without veto decoding and writing-------
        tempdata = new unsigned short[512]; //for 512 words of cztspectrum with alpha without veto
        for (i = 0, j = 1464; i < 512; j++, i++) tempdata[i] = frame.framedata[j]; //starts from 1464th word in frame data 
        fits_write_col(fptr, TUSHORT, 6, nrows + 1, 1, 512, tempdata, &status);
        if (status) {
            fits_report_error(stderr, status);
            return (EXIT_FAILURE);
        }

        //--------------writing quadrant id for data----------------
        fits_write_col(fptr, TBYTE, 7, nrows + 1, 1, 1, &frame.quadID, &status);
        if (status) {
            fits_report_error(stderr, status);
            return (EXIT_FAILURE);
        }
        delete[] tempdata;
        // SSM EXTENSION END
        
        /************WRITING TEMP EXTENSION OF EVENT FILE*********************/
        fits_movabs_hdu(fptr, 8, NULL, &status); //Temp data will be at 8th extension of data file
        if (status) {
            fits_report_error(stderr, status);
            return (EXIT_FAILURE);
        }
        
        fits_get_num_rows(fptr, &nrows, &status);
        if(status) {
            fits_report_error(stderr, status);
            return (EXIT_FAILURE);
        }
        
        frametime = 0.0;
        for (int j = 0; j <= 1; j++) {
            //frametime = frametime + pow((double) frame.secondcount, j) * coef[j];
            TCT.interpolate_time((double) frame.secondcount, frametime, NO);
        } //converted to UTC
        
        fits_write_col(fptr, TDOUBLE, 1, nrows + 1, 1, 1, &frametime, &status);
        if (status) {
            fits_report_error(stderr, status);
            return (EXIT_FAILURE);
        }
        DLOG(INFO) << frame.secondcount;
        fits_write_col(fptr, TINT, 2, nrows + 1, 1, 1, &frame.secondcount, &status);
        if (status) {
            fits_report_error(stderr, status);
            return (EXIT_FAILURE);
        }        
        
        float *temperatures;
        temperatures = new float [NUM_DET_PER_QUADRANT];
        for(i=0; i<NUM_DET_PER_QUADRANT; i++){
            temperatures[i]=0.0;
        }




        for (i = 0; i < NUM_DET_PER_QUADRANT; i++) {
            temperatures[i] = (float) frame.temperature[i];
        }
        fits_write_col(fptr, TFLOAT, frame.quadID+3, nrows + 1, 1, 16, temperatures, &status);

        if (status) {
            fits_report_error(stderr, status);
            return (EXIT_FAILURE);
        }
        
        fits_write_col(fptr, TBYTE, 7, nrows+1, 1, 1, &frame.quadID, &status);


        
        /******************END TEMP EXTENSION***********************************/

        
        
        
    }
    return (EXIT_SUCCESS);
}


int cztscience2event::getHistory(vector<string> &vhistory) {
    //char *user = getlogin();
    char *user = getenv("USER");
	vhistory.push_back("Module run by " + (string) user);
    vhistory.push_back("Parameter List START for " + (string) modulename);
    vhistory.push_back("P1 infile=" + (string) infile);
    vhistory.push_back("P2 TCTfile=" + (string) TCTfile);
    vhistory.push_back("P3 outfile=" + (string) outfile);
    vhistory.push_back("P4 GTIfile=" + (string) GTIfile);
    vhistory.push_back("P5 BTIfile=" + (string) BTIfile);

    if (BigEndian == YES)
        vhistory.push_back("P6 BigEndian=yes");
    else
        vhistory.push_back("P6 BigEndian=no");

    if (clobber == YES)
        vhistory.push_back("P7 clobber=yes");
    else
        vhistory.push_back("P7 clobber=no");
    if (history == YES)
        vhistory.push_back("P8 history=yes");
    else
        vhistory.push_back("P8 history=no");
    vhistory.push_back("Parameter List END");
    return (EXIT_SUCCESS);
}


int cztscience2event::writeHdrInfo(fitsfile *fhdr, string TCTfilename, double *coef, unsigned short row_no) {
    int status = 0;
    static int i=0; //status variable
    double cztUTCTime=0.0; //storing UTC event time found using TCT file.
    Tct TCT;
    if(TCT.read_tct_file(TCTfilename)){
        LOG(ERROR) << "*** Error reading TCT file***";
        return (EXIT_FAILURE);
    }
    if(TCT.is_reset()){
        LOG(ERROR) << "*** Error in level-1 TCT file ***";
        return (EXIT_FAILURE);
    }
    
    if(TCT.interpolate_time((double) p.secondcount, cztUTCTime, NO)){
        LOG(ERROR) << "Error in TCT time interpolation.";
        return (EXIT_FAILURE);
    }
    //cztUTCTime = coef[0]+(pow(p.secondcount, 1) * coef[1]); //calculating UTC time from second count.

    fits_write_col(fhdr, TDOUBLE, 1, row_no,1,1, &cztUTCTime, &status);
    fits_write_col(fhdr, TBYTE, 2, row_no, 1, 1, &p.dataID, &status);
    fits_write_col(fhdr, TBYTE, 3, row_no, 1, 1, &p.modeID, &status);
    fits_write_col(fhdr, TUSHORT, 4, row_no, 1, 1, &p.wcnt, &status); //total events
    fits_write_col(fhdr, TUSHORT, 5, row_no, 1, 1, &p.framecounter, &status);
    fits_write_col(fhdr, TUSHORT, 6, row_no, 1, 1, &p.statusFlag, &status);
    fits_write_col(fhdr, TUSHORT, 7, row_no, 1, 1, &p.writePktNo, &status);
    fits_write_col(fhdr, TUSHORT, 8, row_no, 1, 1, &p.readPktNo, &status);
    fits_write_col(fhdr, TINT, 9, row_no, 1, 1, &p.command, &status);
    fits_write_col(fhdr, TLONGLONG, 10, row_no, 1, 1, &p.laxpctime, &status);
    fits_write_col(fhdr, TUSHORT, 11, row_no, 1, 1, &p.dcnt, &status);
    fits_write_col(fhdr, TBYTE, 12, row_no, 1, 1, &p.errorcount, &status);
    fits_write_col(fhdr, TBYTE, 13, row_no, 1, 1, &p.errorflag, &status);
    fits_write_col(fhdr, TBYTE, 14, row_no,1,1,&p.bootpageno, &status);
    fits_write_col(fhdr, TBYTE, 15, row_no, 1, 1, &p.febhdr.CZTNo, &status);
    fits_write_col(fhdr, TBYTE, 16, row_no, 1, 1, &p.febhdr.CZTstatus, &status);
    fits_write_col(fhdr, TBYTE, 17, row_no, 1, 1, &p.febhdr.EventReadMode, &status);
    fits_write_col(fhdr, TBYTE, 18, row_no, 1, 1, &p.febhdr.CmdStatus, &status);
    fits_write_col(fhdr, TBYTE, 19, row_no, 1, 1, &p.febhdr.BufferNo, &status);
    fits_write_col(fhdr, TBYTE, 20, row_no, 1, 1, &p.febhdr.BaseAdd, &status);
    fits_write_col(fhdr, TBYTE, 21, row_no, 1, 1, &p.febhdr.VetoSpecRange, &status);
    fits_write_col(fhdr, TBYTE, 22, row_no, 1, 1, &p.febhdr.ChannelNo, &status);
    fits_write_col(fhdr, TUSHORT, 23, row_no, 1, 1, &p.febhdr.ADCoutput, &status);
    fits_write_col(fhdr, TUSHORT, 24, row_no,1,1,&p.febhdr.cmd1sec, &status);
    fits_write_col(fhdr, TUSHORT, 25, row_no, 1, 1, &p.febhdr.AlphaCount, &status);
    fits_write_col(fhdr, TUSHORT, 26, row_no, 1, 1, &p.febhdr.VetoCount, &status);
    fits_write_col(fhdr, TUSHORT, 27, row_no, 1, 1, &p.febhdr.CZTcount_lt_ULD, &status);
    fits_write_col(fhdr, TUSHORT, 28, row_no, 1, 1, &p.febhdr.CZTcount_gt_ULD, &status);
    fits_write_col(fhdr, TUSHORT, 29, row_no, 1, 1, &p.febhdr.CZTdataread, &status);
    fits_write_col(fhdr, TUSHORT, 31, row_no, 1, 1, &p.secondcount, &status);
    
    
    return status;
}

