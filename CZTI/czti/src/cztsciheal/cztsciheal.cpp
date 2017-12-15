#include<fitsio.h>
#include<math.h>
#include<vector>
#include<stdlib.h>
#include <iostream>
#include <string>
#include<level1handler.h>
#include <limits>
using namespace std;

int status=0,ifile;
char infile[PIL_LINESIZE];
char modename[PIL_LINESIZE];
int nscienceFiles;
int clobber;                      
int history;
vector <string> scienceFiles;
vector <double> time_vec;
vector <long> time_vec_index;
vector <int> orbits_vec;
vector <long> frame_seq_no_vec;
//vector <double> new_key_vec;
vector <string> new_key_vec;
//output vectors
vector <int> orbits_sorted_vec;
vector <long> start_row_vec;
vector <long> end_row_vec;


void heapSort_lexical(long n);
void heapify_lexical(long n, long i);
void heapSort(long n);
void heapify(long n, long i);
void sort_lexical();
int sorted_orbit_time();
int read(int argc,char **argv)
{

	scienceFiles.clear();
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

/*    if (PIL_OK != (status = PILGetFname("par_tctfile", tctfile))) {
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
*/    PILClose(status);

    return (EXIT_SUCCESS);	
	
}


int main(int argc, char** argv) 
{

	google::InitGoogleLogging(argv[0]);
	google::SetStderrLogging(google::INFO);

	checkPFILESenv();
    checkParFile("cztsciheal");

    if(read(argc,argv)){
        LOG(ERROR)<<"***Error reading parameters***";
        return (EXIT_FAILURE);
    }
	//cout<<string(scienceFiles[0]);

//Getting input files orbit nos
/*	for(i=0;i<nscienceFiles;i++){

		//cout<<string(scienceFiles[0]).substr (pos+6,5);
		orbits_vec.push_back(string(scienceFiles[0]).substr (pos+6,5));
		//cout<<orbits_vec[i]<<endl;
	}	
*/
	sorted_orbit_time();
}	
int sorted_orbit_time(){
	//cout<<"####"<<nscienceFiles;
	int i=0;
	long j=0,k=0;
	fitsfile *fin;
	double *TIME;
	int *errorflag;
	long *row_no;
	int status=0;
	long nrows=0;
	long count=0;
	long *frame_seq_no;
	//double new_key=0;
	char new_key[28];
	const char *orbit;
	int orbit1;
	int *orbit_array;
	long *start_row_array;
	long *end_row_array;

	new_key_vec.clear();
	time_vec.clear();
	time_vec_index.clear();
	orbits_vec.clear();
	frame_seq_no_vec.clear();
	orbits_sorted_vec.clear();
	start_row_vec.clear();
	end_row_vec.clear();

	orbit=(const char*)malloc(sizeof(char)*5);
	orbit_array=(int*)malloc(sizeof(int)*nscienceFiles);
	start_row_array=(long*)malloc(sizeof(long)*nscienceFiles);
	end_row_array=(long*)malloc(sizeof(long)*nscienceFiles);
	for(j=0;j<nscienceFiles;j++){	
		std::size_t pos =string(scienceFiles[j]).find("orbit");
		orbit="\0";
		nrows=0;
		orbit=string(scienceFiles[j]).substr (pos+6,5).c_str();
		orbit1=atoi(orbit);
		orbit_array[j]=orbit1;
		//cout<<orbit_array[nscienceFiles];
		fits_open_file(&fin,scienceFiles[j].c_str(),READONLY,&status);
		//cout<<"\nReading file: "<<scienceFiles[j];
		fits_movabs_hdu(fin,2,NULL, &status);
		fits_get_num_rows(fin,&nrows,&status);
		//printf("\n\t%ld",nrows);
		//cout<<"\t"<<nrows<<endl;
		TIME=(double*)malloc(sizeof(double)*nrows);
		errorflag=(int*)malloc(sizeof(int)*nrows);
		frame_seq_no=(long*)malloc(sizeof(long)*nrows);
		fits_read_col(fin,TDOUBLE,1,1,1,nrows,NULL,TIME,NULL,&status);
		if(status) { printf("\nError(%s:%d):Error in reading time column",__FILE__,__LINE__);fits_report_error(stderr,status); return(EXIT_FAILURE);}
		fits_read_col(fin,TINT,5,1,1,nrows,NULL,errorflag,NULL,&status);
		if(status) { printf("\nError(%s:%d):Error in reading decoding status flag column",__FILE__,__LINE__);fits_report_error(stderr,status); return(EXIT_FAILURE);}
		fits_read_col(fin,TLONG,3,1,1,nrows,NULL,frame_seq_no,NULL,&status);
		if(status) { printf("\nError(%s:%d):Error in reading decoding status flag column",__FILE__,__LINE__);fits_report_error(stderr,status); return(EXIT_FAILURE);}
		
		for(k=0;k<nrows;k++){
			if(errorflag[k]==0){
				//new_key=(TIME[k])+frame_seq_no[k];
				sprintf(new_key,"%lf%ld",TIME[k],frame_seq_no[k]);
				//cout<<new_key<<endl;
				time_vec.push_back(TIME[k]);
				frame_seq_no_vec.push_back(frame_seq_no[k]);
				time_vec_index.push_back(count+1);
				orbits_vec.push_back(orbit1);
				new_key_vec.push_back(new_key);
				count++;
			}
		}
		count=0;	
		fits_close_file(fin,&status);
		if(status) { printf("Error(%s:%d):\n",__FILE__,__LINE__);fits_report_error(stderr,status); return(EXIT_FAILURE);}		
	}

	
	//sorting time and accordingly shuffling frame sequence no and time_vec_index
	heapSort_lexical(time_vec.size());
	//sort_lexical();
	for(k=0;k<time_vec.size()-1;k++){
		if(time_vec[k]==time_vec[k+1] && frame_seq_no_vec[k]==frame_seq_no_vec[k+1]){
			time_vec.erase(time_vec.begin()+k);
			frame_seq_no_vec.erase(frame_seq_no_vec.begin()+k);
			time_vec_index.erase(time_vec_index.begin()+k);
			orbits_vec.erase(orbits_vec.begin()+k);
		}
	}	
	
	for(k=0;k<time_vec.size()-1;k++){
		//printf("%lf\t%s\t%ld\n",time_vec[k],string(orbits_vec[k]),time_vec_index[k]);
		//cout<<"\n"<< fixed <<time_vec[k]<<"\t"<<frame_seq_no_vec[k]<<"\t"<<orbits_vec[k]<<"\t"<<time_vec_index[k]<<"\t"<<new_key_vec[k]<<endl;
	}	


//making the chunks of orbit no	
	//int first_orbit=orbits_vec[0];
	long first_row=time_vec_index[0];
	long read_till;
	for(k=0;k<orbits_vec.size()-1;k++){
		if(orbits_vec[k]!=orbits_vec[k+1]){
			read_till=time_vec_index[k];
			orbits_sorted_vec.push_back(orbits_vec[k]);
			start_row_vec.push_back(first_row);
			end_row_vec.push_back(read_till);
			//cout<<"read orbit: "<<orbits_vec[k]<<"\t"<<first_row<<"-"<<read_till<<endl;
			//first_orbit=orbits_vec[k+1];
			first_row=time_vec_index[k+1];
		}

	}

	for(k=0;k<orbits_sorted_vec.size();k++){
			cout<<orbits_sorted_vec[k]<<"\t"<<start_row_vec[k]<<"-"<<end_row_vec[k]<<endl;
	}		










/*	for(i=0;i<nscienceFiles;i++){
		for(k=0;k<orbits_vec.size();k++){
			if(orbits_vec[k]==orbit_array[nscienceFiles]){
				count++;
				if (count==1){
					cout<<orbits_vec[k]<<"\t###"<<orbit_array[nscienceFiles];
					start_row_array[i]=time_vec_index[k];
				}
			}
			end_row_array[i]=time_vec_index[k];
		}
	}
	cout<<"start row: "<<start_row_array[nscienceFiles-1]<<"\t"<<"end row: "<<end_row_array[nscienceFiles-1]<<endl;
*/

//	for(j=0;j<nscienceFiles;j++){
//		printf("\n%d",orbit_array[j]);
//	}	
	
 free(TIME);
 free(errorflag);
 free(frame_seq_no);
 free(start_row_array);
 free(end_row_array);
 free(orbit_array);
}	



/*void heapify(long n, long i)
{
    long largest = i;  // Initialize largest as root
    long l = 2*i + 1;  // left = 2*i + 1
    long r = 2*i + 2;  // right = 2*i + 2
 	double temp=0.0;
	long temp_index=0;
	long temp3=0;
	int temp_orbit;
	double temp_new_key=0;
    // If left child is larger than root
 //   if (l < n && time_vec[l] > time_vec[largest])
   //     largest = l;
 
    if (l < n && new_key_vec[l] > new_key_vec[largest])
       largest = l;


    // If right child is larger than largest so far
   // if (r < n && time_vec[r] > time_vec[largest])
    //    largest = r;
 
    if (r < n && new_key_vec[r] > new_key_vec[largest])
        largest = r;
	
    // If largest is not root
    if (largest != i)
    {
        //swap(time_vec[i], time_vec[largest]);

		
		temp_new_key=new_key_vec[i];
		new_key_vec[i]=new_key_vec[largest];
		new_key_vec[largest]=temp_new_key;

 		temp=time_vec[i];
		time_vec[i]=time_vec[largest];
		time_vec[largest]=temp;

 		temp_index=time_vec_index[i];
		time_vec_index[i]=time_vec_index[largest];
		time_vec_index[largest]=temp_index;
		
		temp3=frame_seq_no_vec[i];
		frame_seq_no_vec[i]=frame_seq_no_vec[largest];
		frame_seq_no_vec[largest]=temp3;

		temp_orbit=orbits_vec[i];
		orbits_vec[i]=orbits_vec[largest];
		orbits_vec[largest]=temp_orbit;		


        // Recursively heapify the affected sub-tree
        heapify(n, largest);
    }
}
 
// main function to do heap sort
void heapSort(long n)
{
	double temp1=0.0;
	long temp1_index=0;
	long temp_frame=0;
	int temp2_orbit;
	double temp1_new_key=0;
    // Build heap (rearrange array)
    for (long i = n / 2 - 1; i >= 0; i--)
        heapify(n, i);
 
    // One by one extract an element from heap
    for (long i=n-1; i>=0; i--)
    {
        // Move current root to end
        //swap(time_vec[0], time_vec[i]);
 		temp1_new_key=new_key_vec[0];
		new_key_vec[0]=new_key_vec[i];
		new_key_vec[i]=temp1_new_key;	
	
		temp1=time_vec[0];
		time_vec[0]=time_vec[i];
		time_vec[i]=temp1;

 		temp1_index=time_vec_index[0];
		time_vec_index[0]=time_vec_index[i];
		time_vec_index[i]=temp1_index;

		temp_frame=frame_seq_no_vec[0];
		frame_seq_no_vec[0]=frame_seq_no_vec[i];
		frame_seq_no_vec[i]=temp_frame;

		temp2_orbit=orbits_vec[0];
		orbits_vec[0]=orbits_vec[i];
		orbits_vec[i]=temp2_orbit;
        // call max heapify on the reduced heap
        heapify(i, 0);
    }
}
*/
void sort_lexical(){
    long new_key_size=new_key_vec.size();
    int done = 0;

    for (long i = 0; i < new_key_size; i++){
          for (long j = i+1; j < new_key_size; j++){

                  if (strcmp(new_key_vec[i].c_str(),new_key_vec[j].c_str()) > 0){
					  new_key_vec[i].swap(new_key_vec[j]);

			}
          }
    }
/*    for (int i = 0; i < new_key_size; i++){
        printf("%d - %s\n", i+1,new_key_vec[i].c_str() );
    }
*/}

void heapify_lexical(long n, long i)
{
    long largest = i;  // Initialize largest as root
    long l = 2*i + 1;  // left = 2*i + 1
    long r = 2*i + 2;  // right = 2*i + 2
 	double temp=0.0;
	long temp_index=0;
	long temp3=0;
	int temp_orbit;
	string temp_new_key;
    // If left child is larger than root
 //   if (l < n && time_vec[l] > time_vec[largest])
   //     largest = l;
 
    if (l < n && strcmp(new_key_vec[l].c_str(),new_key_vec[largest].c_str())>0)
       largest = l;


    // If right child is larger than largest so far
   // if (r < n && time_vec[r] > time_vec[largest])
    //    largest = r;
 
    if (r < n && strcmp(new_key_vec[r].c_str(),new_key_vec[largest].c_str())>0)
        largest = r;
	
    // If largest is not root
    if (largest != i)
    {
        //swap(time_vec[i], time_vec[largest]);

		
		//temp_new_key=new_key_vec[i];
		//new_key_vec[i]=new_key_vec[largest];
		//new_key_vec[largest]=temp_new_key;
		new_key_vec[i].swap(new_key_vec[largest]);
		
 		temp=time_vec[i];
		time_vec[i]=time_vec[largest];
		time_vec[largest]=temp;

 		temp_index=time_vec_index[i];
		time_vec_index[i]=time_vec_index[largest];
		time_vec_index[largest]=temp_index;
		
		temp3=frame_seq_no_vec[i];
		frame_seq_no_vec[i]=frame_seq_no_vec[largest];
		frame_seq_no_vec[largest]=temp3;

		temp_orbit=orbits_vec[i];
		orbits_vec[i]=orbits_vec[largest];
		orbits_vec[largest]=temp_orbit;		


        // Recursively heapify the affected sub-tree
        heapify_lexical(n, largest);
    }
}
 
// main function to do heap sort
void heapSort_lexical(long n)
{
	double temp1=0.0;
	long temp1_index=0;
	long temp_frame=0;
	int temp2_orbit;
	string temp1_new_key;
    // Build heap (rearrange array)
    for (long i = n / 2 - 1; i >= 0; i--)
        heapify_lexical(n, i);
 
    // One by one extract an element from heap
    for (long i=n-1; i>=0; i--)
    {
        // Move current root to end
        //swap(time_vec[0], time_vec[i]);
 		//temp1_new_key=new_key_vec[0];
		//new_key_vec[0]=new_key_vec[i];
		//new_key_vec[i]=temp1_new_key;	
	
		new_key_vec[0].swap(new_key_vec[i]);

		temp1=time_vec[0];
		time_vec[0]=time_vec[i];
		time_vec[i]=temp1;

 		temp1_index=time_vec_index[0];
		time_vec_index[0]=time_vec_index[i];
		time_vec_index[i]=temp1_index;

		temp_frame=frame_seq_no_vec[0];
		frame_seq_no_vec[0]=frame_seq_no_vec[i];
		frame_seq_no_vec[i]=temp_frame;

		temp2_orbit=orbits_vec[0];
		orbits_vec[0]=orbits_vec[i];
		orbits_vec[i]=temp2_orbit;
        // call max heapify on the reduced heap
        heapify_lexical(i, 0);
    }
}



