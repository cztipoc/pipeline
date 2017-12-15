#/bin/bash
if [ $1 ];then
	l1obs=$1
else
	echo "Please provide l1 obs path"
	exit
fi
if [ -d $l1obs/czti/aux/ ] ;
then
	echo removing aux directory which already exist
	rm -rf $l1obs/czti/aux/

fi	
if [ -d $l1obs/czti/modeM0/ ] ;
then
	echo removing modeM0 directory which already exist
	rm -rf $l1obs/czti/modeM0/

fi	
if [ -d $l1obs/czti/modeM9/ ] ;
then
	echo removing modeM9 directory which already exist
	rm -rf $l1obs/czti/modeM9/

fi	
if [ -d $l1obs/czti/modeSS/ ] ;
then
	echo removing modeSS directory which already exist
	rm -rf $l1obs/czti/modeSS/

fi	

if [ -f '/tmp/cztmerge_V2_1.txt' ];then
	echo removing /tmp/cztmerge_V2_1.txt
	/bin/rm /tmp/cztmerge_V2_1.txt
fi
if [ -f '/tmp/cztmerge_V2_2.txt' ];then
	echo removing /tmp/cztmerge_V2_2.txt
	/bin/rm /tmp/cztmerge_V2_2.txt
fi

if [ -f '/tmp/merged_sorted.txt' ];then
	echo removing /tmp/merged_sorted.txt
	/bin/rm /tmp/merged_sorted.txt
fi
if [ -f '/tmp/merged.txt' ];then
	echo removing /tmp/merged.txt
	/bin/rm /tmp/merged.txt
fi

if [ -f '/tmp/cztmergeaux_input.txt' ];then
	echo removing /tmp/cztmergeaux_input.txt
	/bin/rm /tmp/cztmergeaux_input.txt
fi

mkdir -p $l1obs/czti/aux/aux1 $l1obs/czti/aux/aux2 $l1obs/czti/aux/aux3 $l1obs/czti/modeM0 $l1obs/czti/modeM9 $l1obs/czti/modeSS
echo -e "\nMaking merge level1 directory structure ......."


ls $l1obs/czti/orbit/ |grep -v V|grep -v txt|grep -v ignore|sort>>/tmp/cztmerge_V2_1.txt
for file in `find -L $l1obs/czti/orbit/ -name "*.mkf"|grep -v V|grep -v ignore|sort`;do temp=`fkeyprint $file[0] TSTART exact=yes|grep TSTART`;echo $temp|cut -d' ' -f6>>/tmp/cztmerge_V2_2.txt;done;
paste -d " " /tmp/cztmerge_V2_1.txt /tmp/cztmerge_V2_2.txt>>/tmp/merged.txt	
cat /tmp/merged.txt|sort -k2 -n>>/tmp/merged_sorted.txt

#merging TCT
>/tmp/cztmergeaux_input.txt
echo `wc -l /tmp/merged_sorted.txt|cut -d' ' -f1`>>/tmp/cztmergeaux_input.txt
single_file=$(find -L $l1obs/czti/orbit/????[0-9]/aux/ -name "*.tct"|grep -v 'ignore' |head -1)
outfile=$l1obs/czti/aux/$(basename $single_file)
merged_tct=$outfile
echo $outfile>>/tmp/cztmergeaux_input.txt
echo "y">>/tmp/cztmergeaux_input.txt
for orbit in `cat /tmp/merged_sorted.txt |cut -d' ' -f1`;do find -L $l1obs/czti/orbit/$orbit/aux/ -name "*.tct">>/tmp/cztmergeaux_input.txt ;done;
#gvim /tmp/cztmergeaux_input.txt
cztmergeaux</tmp/cztmergeaux_input.txt
echo -e "\ntct merging completed......"
#merging LBT
>/tmp/cztmergeaux_input.txt
echo `wc -l /tmp/merged_sorted.txt|cut -d' ' -f1`>>/tmp/cztmergeaux_input.txt
single_file=$(find -L $l1obs/czti/orbit/????[0-9]/aux/aux2/ -name "*.lbt"|grep -v 'ignore' |head -1)
outfile=$l1obs/czti/aux/aux2/$(basename $single_file)
echo $outfile>>/tmp/cztmergeaux_input.txt
echo "y">>/tmp/cztmergeaux_input.txt
for orbit in `cat /tmp/merged_sorted.txt |cut -d' ' -f1`;do find -L $l1obs/czti/orbit/$orbit/aux/aux2/ -name "*.lbt">>/tmp/cztmergeaux_input.txt ;done;
#gvim /tmp/cztmergeaux_input.txt
cztmergeaux</tmp/cztmergeaux_input.txt
echo -e "\nlbt merging completed......"

#merging and interpolating att
>/tmp/cztmergeaux_input.txt
echo `wc -l /tmp/merged_sorted.txt|cut -d' ' -f1`>>/tmp/cztmergeaux_input.txt
single_file=$(find -L $l1obs/czti/orbit/????[0-9]/aux/aux1/ -name "*.att"|grep -v 'ignore' |head -1)
outfile=$l1obs/czti/aux/aux1/$(basename $single_file)
echo $outfile>>/tmp/cztmergeaux_input.txt
echo "y">>/tmp/cztmergeaux_input.txt
for orbit in `cat /tmp/merged_sorted.txt |cut -d' ' -f1`;do find -L $l1obs/czti/orbit/$orbit/aux/aux1/ -name "*.att">>/tmp/cztmergeaux_input.txt ;done;
#gvim /tmp/cztmergeaux_input.txt
cztmergeaux</tmp/cztmergeaux_input.txt
echo -e "\natt merging completed......"


temp_outfile=$(dirname $outfile)"/temp_aux1_att"
>/tmp/cztmergeaux_input.txt
echo $outfile>>/tmp/cztmergeaux_input.txt
echo $temp_outfile>>/tmp/cztmergeaux_input.txt
echo "y">>/tmp/cztmergeaux_input.txt
cztattcorrect</tmp/cztmergeaux_input.txt
mv $temp_outfile $outfile
echo -e "\natt interpolation completed......"


#merging and interpolating aux1 orb 
>/tmp/cztmergeaux_input.txt
echo `wc -l /tmp/merged_sorted.txt|cut -d' ' -f1`>>/tmp/cztmergeaux_input.txt
single_file=$(find -L $l1obs/czti/orbit/????[0-9]/aux/aux1/ -name "*.orb"|grep -v 'ignore' |head -1)
outfile=$l1obs/czti/aux/aux1/$(basename $single_file)
echo $outfile>>/tmp/cztmergeaux_input.txt
echo "y">>/tmp/cztmergeaux_input.txt
for orbit in `cat /tmp/merged_sorted.txt |cut -d' ' -f1`;do find -L $l1obs/czti/orbit/$orbit/aux/aux1/ -name "*.orb">>/tmp/cztmergeaux_input.txt ;done;
#gvim /tmp/cztmergeaux_input.txt
cztmergeaux</tmp/cztmergeaux_input.txt
echo -e "\naux1 orb merging completed......"


temp_outfile=$(dirname $outfile)"/temp_aux1_orb"
>/tmp/cztmergeaux_input.txt
echo $outfile>>/tmp/cztmergeaux_input.txt
echo $temp_outfile>>/tmp/cztmergeaux_input.txt
echo "y">>/tmp/cztmergeaux_input.txt
cztorbcorrect</tmp/cztmergeaux_input.txt
mv $temp_outfile $outfile
echo -e "\naux1 orb interpolation completed......"

#merging and interpolating aux3 orb 
>/tmp/cztmergeaux_input.txt
echo `wc -l /tmp/merged_sorted.txt|cut -d' ' -f1`>>/tmp/cztmergeaux_input.txt
single_file=$(find -L $l1obs/czti/orbit/????[0-9]/aux/aux3/ -name "*.orb"|grep -v 'ignore' |head -1)
outfile=$l1obs/czti/aux/aux3/$(basename $single_file)
echo $outfile>>/tmp/cztmergeaux_input.txt
echo "y">>/tmp/cztmergeaux_input.txt
for orbit in `cat /tmp/merged_sorted.txt |cut -d' ' -f1`;do find -L $l1obs/czti/orbit/$orbit/aux/aux3/ -name "*.orb">>/tmp/cztmergeaux_input.txt ;done;
#gvim /tmp/cztmergeaux_input.txt
cztmergeaux</tmp/cztmergeaux_input.txt
echo -e "\naux3 orb merging completed......"


temp_outfile=$(dirname $outfile)"/temp_aux1_orb"
>/tmp/cztmergeaux_input.txt
echo $outfile>>/tmp/cztmergeaux_input.txt
echo $temp_outfile>>/tmp/cztmergeaux_input.txt
echo "y">>/tmp/cztmergeaux_input.txt
cztorbcorrect</tmp/cztmergeaux_input.txt
mv $temp_outfile $outfile
echo -e "\naux3 orb interpolation completed......"

################# cztmergescience processing ################
#processing modeM0
>/tmp/cztmergeaux_input.txt
echo 'modeM0'>>/tmp/cztmergeaux_input.txt
#nfiles=`wc -l /tmp/merged_sorted.txt|cut -d' ' -f1`
nfiles=`find -L $l1obs/czti/orbit/ -name "*M0_level1.fits"|grep -v V|grep -v ignore|wc -l`
echo $nfiles>>/tmp/cztmergeaux_input.txt
for orbit in `cat /tmp/merged_sorted.txt |cut -d' ' -f1`;do find -L $l1obs/czti/orbit/$orbit/modeM0/ -name "*.fits">>/tmp/cztmergeaux_input.txt ;done;
echo $merged_tct>>/tmp/cztmergeaux_input.txt
mcap_mkf_path=$(find -L $l1obs/czti/orbit/????[0-9]/ -name "*.xml"|grep -v V|grep -v 'ignore'|head -1)
echo $mcap_mkf_path
echo $mcap_mkf_path>>/tmp/cztmergeaux_input.txt
single_fits_file=$(find -L $l1obs/czti/orbit/????[0-9]/modeM0/ -name "*.fits"|grep -v V|grep -v 'ignore' |head -1)
temp_base=`basename $single_fits_file`
output_file=$l1obs/czti/modeM0/$temp_base
echo $output_file>>/tmp/cztmergeaux_input.txt
echo "y">>/tmp/cztmergeaux_input.txt
echo -e "\nRunning cztmergescience on modeM0...."

#gvim /tmp/cztmergeaux_input.txt
cztmergescience</tmp/cztmergeaux_input.txt
echo -e "\nmodeM0 cztmergescience processing Completed.........."


#processing modeSS
>/tmp/cztmergeaux_input.txt
echo 'modeSS'>>/tmp/cztmergeaux_input.txt
#nfiles=`wc -l /tmp/merged_sorted.txt|cut -d' ' -f1`
nfiles=`find -L $l1obs/czti/orbit/ -name "*SS_level1.fits"|grep -v V|grep -v ignore|wc -l`
echo $nfiles>>/tmp/cztmergeaux_input.txt
for orbit in `cat /tmp/merged_sorted.txt |cut -d' ' -f1`;do find -L $l1obs/czti/orbit/$orbit/modeSS/ -name "*.fits">>/tmp/cztmergeaux_input.txt ;done;
echo $merged_tct>>/tmp/cztmergeaux_input.txt
mcap_mkf_path=$(find -L $l1obs/czti/orbit/????[0-9]/ -name "*.xml"|grep -v V|grep -v 'ignore'|head -1)
echo $mcap_mkf_path
echo $mcap_mkf_path>>/tmp/cztmergeaux_input.txt
single_fits_file=$(find -L $l1obs/czti/orbit/????[0-9]/modeSS/ -name "*.fits"|grep -v V|grep -v 'ignore' |head -1)
echo "#### find -L $l1obs/czti/orbit/????[0-9]/modeM0/ -name *.fits|grep -v V|grep -v 'ignore' |head -1"
temp_base=`basename $single_fits_file`
output_file=$l1obs/czti/modeSS/$temp_base
echo $output_file>>/tmp/cztmergeaux_input.txt
echo "y">>/tmp/cztmergeaux_input.txt
echo -e "\nRunning cztmergescience on modeSS...."
#gvim /tmp/cztmergeaux_input.txt
cztmergescience</tmp/cztmergeaux_input.txt
echo -e "\nmodeSS cztmergescience processing Completed.........."


	#processing modeM9
	>/tmp/cztmergeaux_input.txt
	echo 'modeM9'>>/tmp/cztmergeaux_input.txt
	#nfiles=`find -L $l1obs/czti/orbit/ -name "*.fits"|grep modeM9|grep -v V|wc -l`
	
	
	
	nfiles=`find -L $l1obs/czti/orbit/ -name "*M9_level1.fits"|grep -v V|grep -v ignore|wc -l`
	#added by ajay, Feb 10 2017. 
	#Checks if modeM9 files exists or not
	if [ ! $nfiles -eq 0 ];
	then 	


	
		echo $nfiles>>/tmp/cztmergeaux_input.txt
		for orbit in `cat /tmp/merged_sorted.txt |cut -d' ' -f1`;do find -L $l1obs/czti/orbit/$orbit/modeM9/ -name "*.fits">>/tmp/cztmergeaux_input.txt ;done;
		echo $merged_tct>>/tmp/cztmergeaux_input.txt
		mcap_mkf_path=$(find -L $l1obs/czti/orbit/????[0-9]/ -name "*.xml"|grep -v V|grep -v 'ignore'|head -1)
		echo $mcap_mkf_path
		echo $mcap_mkf_path>>/tmp/cztmergeaux_input.txt
		single_fits_file=$(find -L $l1obs/czti/orbit/????[0-9]/modeM9/ -name "*.fits"|grep -v V|grep -v 'ignore' |head -1)
		temp_base=`basename $single_fits_file`
		output_file=$l1obs/czti/modeM9/$temp_base
		echo $output_file>>/tmp/cztmergeaux_input.txt
		echo "y">>/tmp/cztmergeaux_input.txt
		echo -e "\nRunning cztmergescience on modeM9...."
		cztmergescience</tmp/cztmergeaux_input.txt
	
		echo -e "\nmodeM9 cztmergescience processing Completed.........."
	else
		echo -e  "modeM9 is not present\n"
	fi


if [ -f '/tmp/cztmerge_V2_1.txt' ];then
	echo removing /tmp/cztmerge_V2_1.txt
	/bin/rm /tmp/cztmerge_V2_1.txt
fi
if [ -f '/tmp/cztmerge_V2_2.txt' ];then
	echo removing /tmp/cztmerge_V2_2.txt
	/bin/rm /tmp/cztmerge_V2_2.txt
fi

if [ -f '/tmp/merged_sorted.txt' ];then
	echo removing /tmp/merged_sorted.txt
	/bin/rm /tmp/merged_sorted.txt
fi
if [ -f '/tmp/merged.txt' ];then
	echo removing /tmp/merged.txt
	/bin/rm /tmp/merged.txt
fi

if [ -f '/tmp/cztmergeaux_input.txt' ];then
	echo removing /tmp/cztmergeaux_input.txt
	/bin/rm /tmp/cztmergeaux_input.txt
fi

