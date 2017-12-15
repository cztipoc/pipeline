#!/bin/bash
if [ $1 ]; then		 
	obsid_path=$1
else
	echo "Error: observation path not provided"
	exit -1
fi

orbits_path=$obsid_path/czti/orbit
#making directory structure for combined files
if [ -d $obsid_path/czti/aux/ ] ;
then
	rm -rf $obsid_path/czti/aux/

fi	
if [ -d $obsid_path/czti/modeM0/ ] ;
then
	rm -rf $obsid_path/czti/modeM0/

fi	
if [ -d $obsid_path/czti/modeM9/ ] ;
then
	rm -rf $obsid_path/czti/modeM9/

fi	
if [ -d $obsid_path/czti/modeSS/ ] ;
then
	rm -rf $obsid_path/czti/modeSS/

fi	


mkdir -p $obsid_path/czti/aux/aux1 $obsid_path/czti/aux/aux2 $obsid_path/czti/aux/aux3 $obsid_path/czti/modeM0 $obsid_path/czti/modeM9 $obsid_path/czti/modeSS
echo -e "\nMaking level1 directory structure complete......."

#merging tct files
nfiles=$(find $orbits_path/?????/aux/ -name *.tct|grep -v 'ignore' |wc -l)
files=$(find $orbits_path/?????/aux/ -name *.tct|grep -v 'ignore'|sort)
single_file=$(find $orbits_path/?????/aux/ -name *.tct|grep -v 'ignore' |head -1)
outfile1=$obsid_path/czti/aux/$(basename $single_file)

#creating input txt file for cztmergeaux_V2.cpp
>cztmergeaux_V2_input.txt
echo $nfiles>>cztmergeaux_V2_input.txt
echo $outfile1>>cztmergeaux_V2_input.txt
echo "y">>cztmergeaux_V2_input.txt
echo $files>>cztmergeaux_V2_input.txt
cztmergeaux<cztmergeaux_V2_input.txt

echo -e "\ntct merging completed......"

#merging lbt files
nfiles=$(find $orbits_path/?????/aux/aux2 -name *.lbt|grep -v 'ignore' |wc -l)
files=$(find $orbits_path/?????/aux/aux2 -name *.lbt|grep -v 'ignore'|sort)
single_file=$(find $orbits_path/?????/aux/aux2 -name *.lbt|grep -v 'ignore' |head -1)
outfile2=$obsid_path/czti/aux/aux2/$(basename $single_file)

#creating input txt file for cztmergeaux_V2.cpp
>cztmergeaux_V2_input.txt
echo $nfiles>>cztmergeaux_V2_input.txt
echo $outfile2>>cztmergeaux_V2_input.txt
echo "y">>cztmergeaux_V2_input.txt
echo $files>>cztmergeaux_V2_input.txt
cztmergeaux<cztmergeaux_V2_input.txt
echo -e "\nlbt merging completed......"


#merging and interpolating aux1 orb files
nfiles=$(find $orbits_path/?????/aux/aux1 -name *.orb|grep -v 'ignore' |wc -l)
files=$(find $orbits_path/?????/aux/aux1 -name *.orb|grep -v 'ignore'|sort)
single_file=$(find $orbits_path/?????/aux/aux1 -name *.orb|grep -v 'ignore' |head -1)
outfile3=$obsid_path/czti/aux/aux1/$(basename $single_file)
>cztmergeaux_V2_input.txt
echo $nfiles>>cztmergeaux_V2_input.txt
echo $outfile3>>cztmergeaux_V2_input.txt
echo "y">>cztmergeaux_V2_input.txt
echo $files>>cztmergeaux_V2_input.txt
cztmergeaux<cztmergeaux_V2_input.txt
temp_outfile3=$(dirname $outfile3)"/temp_aux1_orb"
>cztorbcorrect_input.txt
echo $outfile3>>cztorbcorrect_input.txt
echo $temp_outfile3>>cztorbcorrect_input.txt
cztorbcorrect<cztorbcorrect_input.txt
mv $temp_outfile3 $outfile3
echo -e "\naux1 orb merging completed......"



#merging and interpolating aux3 orb files
nfiles=$(find $orbits_path/?????/aux/aux3 -name *.orb|grep -v 'ignore' |wc -l)
files=$(find $orbits_path/?????/aux/aux3 -name *.orb|grep -v 'ignore'|sort)
single_file=$(find $orbits_path/?????/aux/aux3 -name *.orb|grep -v 'ignore' |head -1)
outfile4=$obsid_path/czti/aux/aux3/$(basename $single_file)
>cztmergeaux_V2_input.txt
echo $nfiles>>cztmergeaux_V2_input.txt
echo $outfile4>>cztmergeaux_V2_input.txt
echo "y">>cztmergeaux_V2_input.txt
echo $files>>cztmergeaux_V2_input.txt
cztmergeaux<cztmergeaux_V2_input.txt
temp_outfile4=$(dirname $outfile4)"/temp_aux3_orb"
>cztorbcorrect_input.txt
echo $outfile4>>cztorbcorrect_input.txt
echo $temp_outfile4>>cztorbcorrect_input.txt
cztorbcorrect<cztorbcorrect_input.txt
mv $temp_outfile4 $outfile4
echo -e "\naux3 orb merging completed......"

#merging and interpolating att files
nfiles=$(find $orbits_path/?????/aux/aux1 -name *.att|grep -v 'ignore' |wc -l)
files=$(find $orbits_path/?????/aux/aux1 -name *.att|grep -v 'ignore'|sort)
single_file=$(find $orbits_path/?????/aux/aux1 -name *.att|grep -v 'ignore' |head -1)
outfile5=$obsid_path/czti/aux/aux1/$(basename $single_file)
>cztmergeaux_V2_input.txt
echo $nfiles>>cztmergeaux_V2_input.txt
echo $outfile5>>cztmergeaux_V2_input.txt
echo "y">>cztmergeaux_V2_input.txt
echo $files>>cztmergeaux_V2_input.txt
cztmergeaux<cztmergeaux_V2_input.txt
temp_outfile5=$(dirname $outfile5)"/temp_aux1_att"
>cztattcorrect_input.txt
echo $outfile5>>cztattcorrect_input.txt
echo $temp_outfile5>>cztattcorrect_input.txt
cztattcorrect<cztattcorrect_input.txt
mv $temp_outfile5 $outfile5
echo -e "\natt merging completed......"


################# cztmergescience processing ################
#processing modeM0
mode='modeM0'
no_fits_files=$(find -L $orbits_path/????? -name *M0*.fits|grep -v 'ignore' |wc -l)
if [ $no_fits_files -eq 0 ];then
	echo "There is no modeM0 files"

fi

if [ $no_fits_files -gt 0 ];then
fits_files=$(find -L $orbits_path/????? -name *M0*.fits|grep -v 'ignore')
merged_tct=$outfile1
mcap_mkf_path=$(find -L $orbits_path/????? -name *.xml|grep -v 'ignore'|head -1)
declare -a myarr  # declare an array
myarr=($(find -L $orbits_path/????? -name *M0*.fits|grep -v 'ignore'|sort))
output_base=`basename ${myarr[0]}`
output_tmp_file=$obsid_path/czti/modeM0/$output_base
output_file=$obsid_path/czti/modeM0/$output_base
>cztmergescience_input.txt
echo $mode>>cztmergescience_input.txt
echo $no_fits_files>>cztmergescience_input.txt
for ((i=0;i<$no_fits_files;i++));
do
	echo ${myarr[i]}>>cztmergescience_input.txt
done
echo $merged_tct>>cztmergescience_input.txt
echo $mcap_mkf_path>>cztmergescience_input.txt
echo $output_file>>cztmergescience_input.txt
echo "y">>cztmergescience_input.txt
echo -e "\nRunning cztmergescience on modeM0...."
cztmergescience<cztmergescience_input.txt 
echo -e "\nmodeM0 cztmergescience processing Completed.........."
fi
echo "Processed single file"

#if [[ -f $output_tmp_file && -f $output_file ]]
#then
	#mv $output_tmp_file $output_file
#fi
#processing modeSS
mode='modeSS'
no_fits_files=$(find -L $orbits_path/????? -name *SS*.fits|grep -v 'ignore' |wc -l)
if [ $no_fits_files -eq 0 ];then
	echo "There is no modeSS files"

fi

if [ $no_fits_files -gt 0 ];then
fits_files=$(find -L $orbits_path/????? -name *SS*.fits|grep -v 'ignore')
merged_tct=$outfile1
mcap_mkf_path=$(find -L $orbits_path/????? -name *.xml|grep -v 'ignore'|head -1)
declare -a myarr  # declare an array
myarr=($(find -L $orbits_path/????? -name *SS*.fits|grep -v 'ignore'|sort))
echo ${myarr[0]}
output_base=`basename ${myarr[0]}`
#echo "Base name:" $output_base
output_file=$obsid_path/czti/modeSS/$output_base
>cztmergescience_input.txt
echo $mode>>cztmergescience_input.txt
echo $no_fits_files>>cztmergescience_input.txt
for ((i=0;i<$no_fits_files;i++));
do
	echo ${myarr[i]}>>cztmergescience_input.txt
done
echo $merged_tct>>cztmergescience_input.txt
echo $mcap_mkf_path>>cztmergescience_input.txt
echo $output_file>>cztmergescience_input.txt
echo "y">>cztmergescience_input.txt
echo -e "\nRunning cztmergescience on modeSS...."
cztmergescience<cztmergescience_input.txt 
echo -e "\nmodeSS cztmergescience processing Completed.........."
fi

#processing modeM9
mode='modeM9'
no_fits_files=$(find -L $orbits_path/????? -name *M9*.fits|grep -v 'ignore' |wc -l)
if [ $no_fits_files -eq 0 ];then
	echo "There is no modeSS files"

fi

if [ $no_fits_files -gt 0 ];then
fits_files=$(find -L $orbits_path/????? -name *M9*.fits|grep -v 'ignore')
merged_tct=$outfile1
mcap_mkf_path=$(find -L $orbits_path/????? -name *.xml|grep -v 'ignore'|head -1)
declare -a myarr  # declare an array
myarr=($(find -L $orbits_path/????? -name *M9*.fits|grep -v 'ignore'|sort))
echo ${myarr[0]}
output_base=`basename ${myarr[0]}`
#echo "Base name:" $output_base
output_file=$obsid_path/czti/modeM9/$output_base
>cztmergescience_input.txt
echo $mode>>cztmergescience_input.txt
echo $no_fits_files>>cztmergescience_input.txt
for ((i=0;i<$no_fits_files;i++));
do
	echo ${myarr[i]}>>cztmergescience_input.txt
done
echo $merged_tct>>cztmergescience_input.txt
echo $mcap_mkf_path>>cztmergescience_input.txt
echo $output_file>>cztmergescience_input.txt
echo "y">>cztmergescience_input.txt
echo -e "\nRunning cztmergescience on modeM9...."
cztmergescience<cztmergescience_input.txt
echo -e "\nmodeM9 cztmergescience processing Completed.........."
fi

rm cztmergeaux_V2_input.txt cztattcorrect_input.txt cztorbcorrect_input.txt cztmergescience_input.txt
echo JOB DONE......!!
