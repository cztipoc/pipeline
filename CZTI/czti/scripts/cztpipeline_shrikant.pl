#!/usr/bin/perl

#CZTPIPELINE
#
#This executes tasks of data analysis pipeline of AstroSat CZTI sequentially
#
#Rakesh Khanna
#15/09/2016
#
#Relased from CZTI-POC 

use File::Basename qw(basename);
use File::Copy qw(copy);
use File::Find qw(find);;

require "libcztiperl.pl";
use vars qw(%Task %Parms_str %Default);

%Task = (
             stem             => "1.0",
	     name	      => "cztpipeline",
	     rdate	      => "2016-09-15",
);


print"\n************************************************************\n";
print"\t\tRunning AstroSat CZTI pipeline\n";
print" Task: $Task{name} Version:$Task{stem} Release Date:$Task{rdate}\n";
print"************************************************************\n";

%Parms_str = (
              		  l1dir	=> "",
	                  l2dir	=> "",
			  clobber => "", 
			  history => "",
			  debug => "",
			  eboundsfile => "",
			  gainfile => "",
			  effareafile => "",
			  lldfile => "",
			  badpixfile => "",
			  maskfile => "",
			  camgeomfile => "",
			  respparfile => "",
			  pixrespfile => "",
			  bigendian => "",
			  mkfthresholdfile => "",
			  usergtifile => "",
			  gtitype => "",
			  writedblevt=> "",
			  alphasel => "",	
			  vetosel => "",
			  dpi_timerange => "",
			  dpi_energybins => "",
			  image_inputtype => "",
			  applymaskweight => "",
			  rasrc => "",
			  decsrc => "",
			  bindata_outputtype => "",
			  lc_energyrange => "",
			  applyLLD => "",
			  generate_eventfile => "",
			  bindata_quadsToProcess => "",	
);

%Parms_num = (

			  entrystage => $Default{'NOTVALDPARNUMB'},
			  exitstage => $Default{'NOTVALDPARNUMB'},
			  sc2evt_buffersize  => $Default{'NOTVALDPARNUMB'},
			  bunchdeftime => $Default{'NOTVALDPARNUMB'},
			  bunch_length_thresh => $Default{'NOTVALDPARNUMB'},
			  skipT1 => $Default{'NOTVALDPARNUMB'},
			  skipT2 => $Default{'NOTVALDPARNUMB'},
			  skipT3 => $Default{'NOTVALDPARNUMB'},
			  livetime_binsize => $Default{'NOTVALDPARNUMB'},
			  noisypix_nsigma => $Default{'NOTVALDPARNUMB'},
			  dettbinsize => $Default{'NOTVALDPARNUMB'},
			  pixtbinsize => $Default{'NOTVALDPARNUMB'},
			  detcount_threshold => $Default{'NOTVALDPARNUMB'},
			  pixcount_threshold => $Default{'NOTVALDPARNUMB'},
			  dpi_badpixthreshold => $Default{'NOTVALDPARNUMB'},
			  bindata_badpixthreshold => $Default{'NOTVALDPARNUMB'},
			  lc_timebinsize => $Default{'NOTVALDPARNUMB'},	
		  
);


%Default = (
            NOTVALDPARNUMB  => "-",
            NOTVALDPARBOOL  => "-",
);
$as1_path =  $ENV{'as1czt'};
$CALDB_path=$ENV{'CALDB'};
if ( &GetInputParameters ) {
	printf "Error parsing input parameters";
	exit -1;
}



sub GetInputParameters {

    use vars qw( %Task  %Parms_str %Parms_num %Parms_bool %Default );
    
    my ($i,$invokestring);
    $invokestring = "";

    if( @ARGV ) {
        foreach $i (0 .. $#ARGV) {
            $ARGV[$i] = "\"".$ARGV[$i]."\"";
        }
        $invokestring = join(' ',@ARGV);
    }
	
	{
        my (%tmp) = (l1dir => "",);
        if( &getStrPar(\%tmp, $invokestring)){ return 1;}
        $Parms_str{l1dir} = $tmp{l1dir};
    }
    
	{
        my (%tmp) = (l2dir => "",);
        if( &getStrPar(\%tmp, $invokestring)){ return 1;}
        $Parms_str{l2dir} = $tmp{l2dir};
    }

    {
        my (%tmp) = (entrystage => $Default{'NOTVALDPARNUMB'},);
        if( &getNumPar(\%tmp, $invokestring)){ return 1;}
        $Parms_num{entrystage} = $tmp{entrystage};
    }


    {
        my (%tmp) = (exitstage => $Default{'NOTVALDPARNUMB'},);
        if( &getNumPar(\%tmp, $invokestring)){ return 1;}
        $Parms_num{exitstage} = $tmp{exitstage};
    }

    {
        my (%tmp) = (clobber => "",);
        if( &getStrPar(\%tmp, $invokestring)){ return 1;}
        $Parms_str{clobber} = $tmp{clobber};
    }


    {
        my (%tmp) = (history => "",);
        if( &getStrPar(\%tmp, $invokestring)){ return 1;}
        $Parms_str{history} = $tmp{history};
    }

    {
        my (%tmp) = (debug => "",);
        if( &getStrPar(\%tmp, $invokestring)){ return 1;}
        $Parms_str{debug} = $tmp{debug};
    }

    {
        my (%tmp) = (eboundsfile => "",);
        if( &getStrPar(\%tmp, $invokestring)){ return 1;}
        $Parms_str{eboundsfile} = $tmp{eboundsfile};
    }


    {
        my (%tmp) = (gainfile => "",);
        if( &getStrPar(\%tmp, $invokestring)){ return 1;}
        $Parms_str{gainfile} = $tmp{gainfile};
    }

    {
        my (%tmp) = (effareafile => "",);
        if( &getStrPar(\%tmp, $invokestring)){ return 1;}
        $Parms_str{effareafile} = $tmp{effareafile};
    }

    {
        my (%tmp) = (lldfile => "",);
        if( &getStrPar(\%tmp, $invokestring)){ return 1;}
        $Parms_str{lldfile} = $tmp{lldfile};
    }

    {
        my (%tmp) = (badpixfile => "",);
        if( &getStrPar(\%tmp, $invokestring)){ return 1;}
        $Parms_str{badpixfile} = $tmp{badpixfile};
    }


    {
        my (%tmp) = (maskfile => "",);
        if( &getStrPar(\%tmp, $invokestring)){ return 1;}
        $Parms_str{maskfile} = $tmp{maskfile};
    }

    {
        my (%tmp) = (camgeomfile => "",);
        if( &getStrPar(\%tmp, $invokestring)){ return 1;}
        $Parms_str{camgeomfile} = $tmp{camgeomfile};
    }

    {
        my (%tmp) = (respparfile => "",);
        if( &getStrPar(\%tmp, $invokestring)){ return 1;}
        $Parms_str{respparfile} = $tmp{respparfile};
    }

    {
        my (%tmp) = (pixrespfile => "",);
        if( &getStrPar(\%tmp, $invokestring)){ return 1;}
        $Parms_str{pixrespfile} = $tmp{pixrespfile};
    }


    {
        my (%tmp) = (sc2evt_buffersize => $Default{'NOTVALDPARNUMB'},);
        if( &getNumPar(\%tmp, $invokestring)){ return 1;}
        $Parms_num{sc2evt_buffersize} = $tmp{sc2evt_buffersize};
    }


    {
        my (%tmp) = (bigendian => "",);
        if( &getStrPar(\%tmp, $invokestring)){ return 1;}
        $Parms_str{bigendian} = $tmp{bigendian};
    }

	{
        my (%tmp) = (bunchdeftime => $Default{'NOTVALDPARNUMB'},);
        if( &getNumPar(\%tmp, $invokestring)){ return 1;}
        $Parms_num{bunchdeftime} = $tmp{bunchdeftime};
    }

    {
        my (%tmp) = (bunch_length_thresh => $Default{'NOTVALDPARNUMB'},);
        if( &getNumPar(\%tmp, $invokestring)){ return 1;}
        $Parms_num{bunch_length_thresh} = $tmp{bunch_length_thresh};
    }

    {
        my (%tmp) = (skipT1 => $Default{'NOTVALDPARNUMB'},);
        if( &getNumPar(\%tmp, $invokestring)){ return 1;}
        $Parms_num{skipT1} = $tmp{skipT1};
    }

    {
        my (%tmp) = (skipT2 => $Default{'NOTVALDPARNUMB'},);
        if( &getNumPar(\%tmp, $invokestring)){ return 1;}
        $Parms_num{skipT2} = $tmp{skipT2};
    }

    {
        my (%tmp) = (skipT3 => $Default{'NOTVALDPARNUMB'},);
        if( &getNumPar(\%tmp, $invokestring)){ return 1;}
        $Parms_num{skipT3} = $tmp{skipT3};
    }

    {
        my (%tmp) = (livetime_binsize => $Default{'NOTVALDPARNUMB'},);
        if( &getNumPar(\%tmp, $invokestring)){ return 1;}
        $Parms_num{livetime_binsize} = $tmp{livetime_binsize};
    }

    {
        my (%tmp) = (mkfthresholdfile => "",);
        if( &getStrPar(\%tmp, $invokestring)){ return 1;}
        $Parms_str{mkfthresholdfile} = $tmp{mkfthresholdfile};
    }


	{
        my (%tmp) = (usergtifile => "",);
        if( &getStrPar(\%tmp, $invokestring)){ return 1;}
        $Parms_str{usergtifile} = $tmp{usergtifile};
    }

	{
        my (%tmp) = (gtitype => "",);
        if( &getStrPar(\%tmp, $invokestring)){ return 1;}
        $Parms_str{gtitype} = $tmp{gtitype};
    }

	{
        my (%tmp) = (noisypix_nsigma => $Default{'NOTVALDPARNUMB'},);
        if( &getNumPar(\%tmp, $invokestring)){ return 1;}
        $Parms_num{noisypix_nsigma} = $tmp{noisypix_nsigma};
    }

    {
        my (%tmp) = (dettbinsize => $Default{'NOTVALDPARNUMB'},);
        if( &getNumPar(\%tmp, $invokestring)){ return 1;}
        $Parms_num{dettbinsize} = $tmp{dettbinsize};
    }

    {
        my (%tmp) = (pixtbinsize => $Default{'NOTVALDPARNUMB'},);
        if( &getNumPar(\%tmp, $invokestring)){ return 1;}
        $Parms_num{pixtbinsize} = $tmp{pixtbinsize};
    }

    {
        my (%tmp) = (detcount_threshold => $Default{'NOTVALDPARNUMB'},);
        if( &getNumPar(\%tmp, $invokestring)){ return 1;}
        $Parms_num{detcount_threshold} = $tmp{detcount_threshold};
    }

    {
        my (%tmp) = (pixcount_threshold => $Default{'NOTVALDPARNUMB'},);
        if( &getNumPar(\%tmp, $invokestring)){ return 1;}
        $Parms_num{pixcount_threshold} = $tmp{pixcount_threshold};
    }


	{
        my (%tmp) = (writedblevt => "",);
        if( &getStrPar(\%tmp, $invokestring)){ return 1;}
        $Parms_str{writedblevt} = $tmp{writedblevt};
    }

     {
        my (%tmp) = (alphasel => "",);
        if( &getStrPar(\%tmp, $invokestring)){ return 1;}
        $Parms_str{alphasel} = $tmp{alphasel};
    }


    {
        my (%tmp) = (vetosel => "",);
        if( &getStrPar(\%tmp, $invokestring)){ return 1;}
        $Parms_str{vetosel} = $tmp{vetosel};
    }

	{
        my (%tmp) = (dpi_badpixthreshold => $Default{'NOTVALDPARNUMB'},);
        if( &getNumPar(\%tmp, $invokestring)){ return 1;}
        $Parms_num{dpi_badpixthreshold} = $tmp{dpi_badpixthreshold};
	}

    {
        my (%tmp) = (dpi_timerange => "",);
        if( &getStrPar(\%tmp, $invokestring)){ return 1;}
        $Parms_str{dpi_timerange} = $tmp{dpi_timerange};
    }

    {
        my (%tmp) = (dpi_energybins => "",);
        if( &getStrPar(\%tmp, $invokestring)){ return 1;}
        $Parms_str{dpi_energybins} = $tmp{dpi_energybins};
    }


    {
        my (%tmp) = (image_inputtype => "",);
        if( &getStrPar(\%tmp, $invokestring)){ return 1;}
        $Parms_str{image_inputtype} = $tmp{image_inputtype};
    }

    {
        my (%tmp) = (applymaskweight => "",);
        if( &getStrPar(\%tmp, $invokestring)){ return 1;}
        $Parms_str{applymaskweight} = $tmp{applymaskweight};
    }


    {
        my (%tmp) = (rasrc => "",);
        if( &getStrPar(\%tmp, $invokestring)){ return 1;}
        $Parms_str{rasrc} = $tmp{rasrc};
    }


    {
        my (%tmp) = (decsrc => "",);
        if( &getStrPar(\%tmp, $invokestring)){ return 1;}
        $Parms_str{decsrc} = $tmp{decsrc};
    }

    {
        my (%tmp) = (bindata_badpixthreshold => $Default{'NOTVALDPARNUMB'},);
        if( &getNumPar(\%tmp, $invokestring)){ return 1;}
        $Parms_num{bindata_badpixthreshold} = $tmp{bindata_badpixthreshold};
    }
   

    {
        my (%tmp) = (bindata_outputtype => "",);
        if( &getStrPar(\%tmp, $invokestring)){ return 1;}
        $Parms_str{bindata_outputtype} = $tmp{bindata_outputtype};
    }


   {
        my (%tmp) = (lc_timebinsize => $Default{'NOTVALDPARNUMB'},);
        if( &getNumPar(\%tmp, $invokestring)){ return 1;}
        $Parms_num{lc_timebinsize} = $tmp{lc_timebinsize};
    }


    {
        my (%tmp) = (lc_energyrange => "",);
        if( &getStrPar(\%tmp, $invokestring)){ return 1;}
        $Parms_str{lc_energyrange} = $tmp{lc_energyrange};
    }

    {
        my (%tmp) = (applyLLD => "",);
        if( &getStrPar(\%tmp, $invokestring)){ return 1;}
        $Parms_str{applyLLD} = $tmp{applyLLD};
    }

    {
        my (%tmp) = (generate_eventfile => "",);
        if( &getStrPar(\%tmp, $invokestring)){ return 1;}
        $Parms_str{generate_eventfile} = $tmp{generate_eventfile};
    }

    {
        my (%tmp) = (bindata_quadsToProcess => "",);
        if( &getStrPar(\%tmp, $invokestring)){ return 1;}
        $Parms_str{bindata_quadsToProcess} = $tmp{bindata_quadsToProcess};
    }

return 0;
	
}


sub getStrPar{
    my ($strin, $invokestring) = @_;
    my ($par);
 
    
    for $par ( sort(keys %$strin) ) {
        if ( !$$strin{$par} && $$strin{$par} !~ "0" ) {
            my ($String) = "";
            chop($String = qx(pquery2 cztpipeline  $par $invokestring));
	if ( !$String && $String !~ "0" ) {
                &PrntChty(3, "$Task{stem}: Error: error running: 'pquery2 cztpipeline $par $invokestring'\n");
                return 1;
            }
            $$strin{$par} = $String;
        }
    }
    return 0;

} # getStrPar

sub getNumPar{
    my ($strin, $invokestring) = @_;
    my ($par);

    for $par ( sort(keys %$strin) ) {
        if ( $$strin{$par} eq $Default{'NOTVALDPARNUMB'} ) {
            my ($Num) = "";
            chop($Num = qx(pquery2 cztpipeline $par $invokestring));
            if ( !$Num && $Num !~ 0 ) {
                &PrntChty(3, "$Task{stem}: Error: error running: 'pquery2 cztpipeline $par $invokestring'\n");
                return 1;
            }
            $$strin{$par} = $Num;
        }
    }
    return 0;

} # getNumPar

sub getBoolPar{
    my ($strin, $invokestring) = @_;
    my ($par);

    for $par ( sort(keys %$strin) ) {
        if ( $$strin{$par} eq $Default{'NOTVALDPARBOOL'} ) {
            my ($Bool) = "";
            chop($Bool = qx(pquery2 cztpipeline $par $invokestring));
            if ( !$Bool ) {
                &PrntChty(3, "$Task{stem}: Error: error running: 'pquery2 cztpipeline $par $invokestring'\n");
                return 1;
            }
            $$strin{$par} = $Bool;
        }
    }
    return 0;

} # getBoolPar
#### Create Directories


chomp($l1dir = $Parms_str{l1dir});


$L1DIR = basename($l1dir);
chomp($L1DIR);

$l1dir = $l1dir."/czti/";
 
	unless($L1DIR =~ m/_level1/)
	{
	print "please enter the path till 20XXXXXX_XXX_XXXXXX_90000XXXXX_level1\n";
	exit -1;
	}

chomp($l2path = $Parms_str{l2dir});


$l2dir = $l2path . "/" . $L1DIR ;
$l2dir =~ s/_level1/_level2/g;


$logfile = $l2dir . "/" . $L1DIR . ".log";
$lgfile  =~ s/level1/level2/;


	mkdir($l2dir, 0755);

	$l2dir = $l2dir . "/czti/";

	mkdir($l2dir, 0755);
 	
	opendir (DIR, $l1dir);
        my @files = readdir(DIR);
	closedir(DIR);
       
	foreach $file (@files)
        {
		if($file =~ m/mkf$/)
		{
		$l1mkffile = $file;
		$l2mkffile = $l1mkffile;
		$l2mkffile =~ s/_level1/_level2/g;
		$mkffile = $l2dir.$l2mkffile;
		#print "MKF FILE = $mkffile\n";
		copy $l1dir.$file , $l2dir;
		rename $l2dir.$l1mkffile,$l2dir.$l2mkffile;
		}

                if($file =~ m/mode/)
                {
                $l2modedir = $l2dir.$file;
                #print "$l2modedir\n";
                mkdir($l2modedir, 0755);
                }
        }

$l2auxdir = $l2dir . "/aux/";

	mkdir($l2auxdir, 0755);

$l2aux1dir = $l2auxdir . "/aux1/";

       mkdir($l2aux1dir, 0755);

$l2aux2dir = $l2auxdir . "/aux2/";

        mkdir($l2aux2dir, 0755);

$l2aux3dir = $l2auxdir . "/aux3/";

        mkdir($l2aux3dir, 0755);


$l1auxdir = $l1dir . "aux/";

$l1aux1dir = $l1auxdir . "/aux1/";

$l1aux2dir = $l1auxdir . "/aux2/";

$l1aux3dir = $l1auxdir . "/aux3/";

# Get TCT file
$tct_file=`find -L $l1dir -name *.tct|grep -v orbit|grep -v ignore`;
$num_files=`find -L $l1dir -name *.tct|grep -v orbit|grep -v ignore|wc -l`;
	if($num_files==1)
        {
			$l1tctfile=$tct_file
		}
	else{
		print "0 or >1 tct files present\n";
        exit -1;
        }
chomp($l1tctfile),chomp($l2auxdir);		
copy ($l1tctfile ,$l2auxdir);
# Get Attitude file
$att_file=`find -L $l1dir -name *.att|grep -v orbit|grep -v ignore`;
$num_files=`find -L $l1dir -name *.att|grep -v orbit|grep -v ignore|wc -l`;
	if($num_files==1)
        {
			$l1attfile=$att_file
		}
	else{
		print "0 or >1 att files present\n";
        exit -1;
        }
chomp($l1attfile) ,chomp($l2aux1dir);		
copy($l1attfile,$l2aux1dir);
# Get Orbit file 
$orb_file=`find -L $l1dir -name *.orb|grep -v orbit|grep -v ignore|grep aux1`;
$num_files=`find -L $l1dir -name *.orb|grep -v orbit|grep -v ignore|grep aux1|wc -l`;
	if($num_files==1)
        {
			$l1orbfile=$orb_file
		}
	else{
		print "0 or >1 aux1 orb files present\n";
        exit -1;
        }
chomp($l1orbfile) ,chomp($l2aux3dir);		
copy($l1orbfile,$l2aux1dir);
copy($l1orbfile,$l2aux3dir);

# Get LBT file
$lbt_file=`find -L $l1dir -name *.lbt|grep -v orbit|grep -v ignore`;
$num_files=`find -L $l1dir -name *.lbt|grep -v orbit|grep -v ignore|wc -l`;
	if($num_files==1)
        {
			$l1lbtfile=$lbt_file
		}
	else{
		print "0 or >1 lbt files present\n";
        exit -1;
        }
chomp($l1lbtfile) ,chomp($l2aux2dir);
copy($l1lbtfile ,$l2aux2dir);

open(my $log, '>', $logfile) or die "Could not open file '$logfile' $!";

$start_step = $Parms_num{entrystage};
$end_step = $Parms_num{exitstage};
@modefiles=`find -L $l1dir -name *.fits|grep -v orbit|grep -v ignore|sort`;

if(($start_step==1 and $end_step==2) or ($start_step==1 and $end_step==3)  or ($start_step==1 and $end_step==1))
{
	
	foreach $file (@modefiles){
		unless($file =~ m/modeM0/ or $file =~ m/modeSS/){
			next;
		}
		#print "*** Processing Stage 1 \n";

	#### CZTSCIENCE2EVENT
	
		$lt = localtime; print $log "[$lt] executing cztscience2event..........\n\n";

		#create output file
		$outfile = $file;
		$outfile =~ s/level1/level2/g;

		#create header file
		$hdrfile = $outfile;
		$hdrfile =~ s/fits/hdr/g;

		#create bunch file
		$bunchfile = $outfile;
		$bunchfile =~ s/.fits/_bunch.fits/g;

		chomp($file,$outfile,$hdrfile,$bunchfile);
		$cztscience2event = "cztscience2event infile=$file TCTfile=$l1tctfile outfile=$outfile hdrInfoFile=$hdrfile bunchfile=$bunchfile nPackets=$Parms_num{sc2evt_buffersize} BigEndian=$Parms_str{bigendian} clobber=$Parms_str{clobber} history=$Parms_str{history} debug=$Parms_str{debug}";
		print "executing cztscience2event for $file\n";
		$lt = localtime; print $log "[$lt] executing cztscience2event for $file\n\n";
		$lt = localtime; print $log "[$lt] $cztscience2event\n\n";
		if(system($cztscience2event)){$lt = localtime; print $log "[$lt] cztscience2event failed\n\n";print "cztscience2event failed\n";exit -1;}
	}	

##### CZTBUNCHCLEAN
$lt = localtime; print $log "[$lt] executing cztbunchclean..........\n\n";
# Get level2 event file
$evtfile=`find -L $l2dir -name *level2.fits|grep -v orbit|grep -v ignore|grep modeM0`;
$num_files=`find -L $l2dir -name *level2.fits|grep -v orbit|grep -v ignore|grep modeM0|wc -l`;
	if($num_files==1)
        {
			$l2evtfile=$evtfile
		}
	else{
		print "0 or >1 level2 event files present\n";
		$lt = localtime; print $log "[$lt] zero or more than one level 2 evt file present. EXITING....\n\n";
        exit -1;
        }
# get modeM0 bunch file
        $bunchfileM0 = $l2evtfile;
        $bunchfileM0 =~ s/.fits/_bunch.fits/g;	
	unless (!(-f $bunchfileM0))
	{
	print "error getting modeM0 bunch file\n";
        $lt = localtime; print $log "[$lt] error getting modeM0 bunch file. EXITING....\n\n";
	exit -1;
	}

# create bunch clean file
	$bunch_clean_fits = $l2evtfile;
	$bunch_clean_fits =~ s/.fits/_bc.fits/g;

# create bunch clean live time file
	$bunch_clean_livetime = $l2evtfile;
	$bunch_clean_livetime =~ s/.fits/_bc_livetime.fits/g;

	chomp($l2evtfile,$bunchfileM0,$bunch_clean_fits,$bunch_clean_livetime);
	$cztbunchclean = "cztbunchclean par_infile=$l2evtfile par_bunchfile=$bunchfileM0 par_outfile=$bunch_clean_fits par_livetimefile=$bunch_clean_livetime par_bunchdeftime=$Parms_num{bunchdeftime} par_bunch_length_thresh=$Parms_num{bunch_length_thresh} par_skipT1=$Parms_num{skipT1} par_skipT2=$Parms_num{skipT2} par_skipT3=$Parms_num{skipT3} par_livetime_binsize=$Parms_num{livetime_binsize} clobber=$Parms_str{clobber} history=$Parms_str{history}";

        $lt = localtime; print $log "[$lt] $cztbunchclean\n\n";

	if(system($cztbunchclean)) {$lt = localtime; print $log "[$lt] cztbunchclean failed\n\n";print "cztbunchclean failed\n";exit -1;}


#### CZTPHA2ENERGY
$lt = localtime; print $log "[$lt] executing cztpha2energy..........\n\n";
# create bunch clean event file
	$bunch_clean_evt = $bunch_clean_fits;
	$bunch_clean_evt =~ s/.fits/.evt/g;
# Get level2 event file
$SSevtfile=`find -L $l2dir -name *level2.fits|grep -v orbit|grep -v ignore|grep modeSS`;
$num_files=`find -L $l2dir -name *level2.fits|grep -v orbit|grep -v ignore|grep modeSS|wc -l`;
	if($num_files==1)
        {
			$mSSl2fits=$SSevtfile
		}
	else{
		print "0 or >1 level2 modeSS event files present\n";
		$lt = localtime; print $log "[$lt] zero or more than one level 2 modeSS evt file present. EXITING....\n\n";
        exit -1;
        }

	chomp($bunch_clean_fits,$bunch_clean_evt,$mSSl2fits);
	$cztpha2energy = "cztpha2energy par_infile=$bunch_clean_fits par_outfile=$bunch_clean_evt par_tempextfile=$mSSl2fits par_clobber=$Parms_str{clobber} par_history=$Parms_str{history}";

	$lt = localtime; print $log "[$lt] $cztpha2energy\n\n";

	if(system($cztpha2energy)){$lt = localtime; print $log "[$lt] cztpha2energy failed\n\n";print "cztpha2energy failed\n";exit -1;}
}#End of if for stage 1	


#**** Stage 2 ****#(cztgtigen,cztgaas,cztdatasel,cztpixclean,cztflagbadpix,cztevtclean)

if(($start_step==1 and $end_step==3) or ($start_step==1 and $end_step==2) or ($start_step==2 and $end_step==3) or ($start_step==2 and $end_step==2))
{

print "*** Processing Stage 2 \n";

#### CZTGTIGEN
$lt = localtime; print $log "[$lt] executing cztgtigen..........\n\n";

# create GTI file
        $l2gtifile = $l2evtfile;
        $l2gtifile =~ s/.fits/.gti/g;
	
# create aliases
	chomp($bunch_clean_evt,$mkffile,$Parms_str{mkfthresholdfile},$l2gtifile);
	$cztgtigen = "cztgtigen  eventfile=$bunch_clean_evt mkffile=$mkffile thresholdfile=$as1_path/$Parms_str{mkfthresholdfile} outfile=$l2gtifile usergtifile=$Parms_str{usergtifile}  clobber=$Parms_str{clobber} history=$Parms_str{history}";

	$lt = localtime; print $log "[$lt] $cztgtigen\n\n";

	if(system($cztgtigen)) {$lt = localtime; print $log "[$lt] cztgtigen failed\n\n";print "cztgtigen failed\n";exit -1;}

#### CZTGAAS
$lt = localtime; print $log "[$lt] executing cztgaas..........\n\n";
# create aspect file
        $aspectfile = $l2evtfile;
        $aspectfile =~ s/.fits/.aspect/g;

# alias teldef file
	chomp($l2evtfile,$mkffile,$aspectfile);
	$cztgaas = "cztgaas par_evtfile=$l2evtfile par_mkffile=$mkffile par_outAspectfile=$aspectfile par_clobber=$Parms_str{clobber} par_history=$Parms_str{history}";

	$lt = localtime; print $log "[$lt] $cztgaas\n\n";

	if(system($cztgaas)){$lt = localtime; print $log "[$lt] cztgaas failed\n\n";print "cztgaas failed\n";exit -1;}

#### CZTDATASEL

$lt = localtime; print $log "[$lt] executing cztdatasel..........\n\n";


		if($Parms_str{gtitype} eq "QUAD" || $Parms_str{gtitype} eq "quad")
		{
			$dataseloutfile = $bunch_clean_evt;
			$dataseloutfile =~ s/_bc.evt/_quad_bc_ds.evt/g;
		}
		elsif(($Parms_str{gtitype} eq "COMMON" || $Parms_str{gtitype} eq "common")) 
		{
      		  $dataseloutfile = $bunch_clean_evt;
       		  $dataseloutfile =~ s/_bc.evt/_common_bc_ds.evt/g;
		}
		else
		{
		print "Wrong GTI Type\n";
		exit -1;
		}

	chomp($bunch_clean_evt,$l2gtifile,$dataseloutfile);
	$cztdatasel = "cztdatasel infile=$bunch_clean_evt gtifile=$l2gtifile gtitype=$Parms_str{gtitype} outfile=$dataseloutfile clobber=$Parms_str{clobber} history=$Parms_str{history}";


	$lt = localtime; print $log "[$lt] $cztdatasel\n\n";
	
	if(system($cztdatasel)) {$lt = localtime; print $log "[$lt] cztdatasel failed\n\n";print "cztdatasel failed\n";exit -1;}
	#### CZTPIXCLEAN
	$lt = localtime; print $log "[$lt] executing cztpixclean..........\n\n";

	# get badpixel fie from CLADB
	$caldb_badpix_file="$CALDB_path/data/as1/czti/bcf/AS1cztbadpix20160317v01.fits";

	        if($Parms_str{gtitype} eq "COMMON" || $Parms_str{gtitype} eq "common")
        {
    		$cztpixclean_pc_evt = $dataseloutfile;
    		$cztpixclean_pc_evt =~ s/common_bc_ds.evt/common_bc_ds_pc.evt/g;
    		$cztpixclean_dbl_evt = $l2evtfile;
    		$cztpixclean_dbl_evt =~ s/.fits/_common.dblevt/g;
    		$cztpixclean_livetime = $dataseloutfile;
    		$cztpixclean_livetime =~ s/common_bc_ds.evt/common_livetime.fits/g;
	 		$cztpixclean_badpix = $l2evtfile;
    		$cztpixclean_badpix =~ s/.fits/_common_badpix.fits/g;
   	    	$cztpixclean_badpix = $l2evtfile;
    	    $cztpixclean_badpix =~ s/.fits/_common_badpix.fits/g;


        }
        elsif(($Parms_str{gtitype} eq "QUAD" || $Parms_str{gtitype} eq "quad"))
        {
            $cztpixclean_pc_evt = $dataseloutfile;
            $cztpixclean_pc_evt =~ s/quad_bc_ds.evt/quad_bc_ds_pc.evt/g;
            $cztpixclean_dbl_evt = $l2evtfile;
            $cztpixclean_dbl_evt =~ s/.fits/_quad.dblevt/g;
            $cztpixclean_livetime = $dataseloutfile;
            $cztpixclean_livetime =~ s/quad_bc_ds.evt/quad_livetime.fits/g;
            $cztpixclean_badpix = $l2evtfile;
            $cztpixclean_badpix =~ s/.fits/_quad_badpix.fits/g;
	        $cztpixclean_badpix = $l2evtfile;
    	    $cztpixclean_badpix =~ s/.fits/_quad_badpix.fits/g;
        }
        else
        {
        print "Wrong GTI Type\n";
        exit -1;
        }


	$writedblevtFlag = "Y";

	$cztpixclean = "cztpixclean par_infile=$dataseloutfile par_inlivetimefile=$bunch_clean_livetime par_writedblevt=$Parms_str{writedblevt} par_outfile1=$cztpixclean_pc_evt par_outfile2=$cztpixclean_dbl_evt par_outlivetimefile=$cztpixclean_livetime  par_badpixfile=$cztpixclean_badpix par_nsigma=$Parms_num{noisypix_nsigma} par_det_tbinsize=$Parms_num{dettbinsize} par_pix_tbinsize=$Parms_num{pixtbinsize} par_det_count_thresh=$Parms_num{detcount_threshold} par_pix_count_thresh=$Parms_num{pixcount_threshold}";



	$lt = localtime; print $log "[$lt] $cztpixclean\n\n";

	if(system($cztpixclean)){$lt = localtime; print $log "[$lt] cztpixclean failed\n\n";print "cztpixclean failed\n";exit -1;}
# create output cztflagbad pix file
	
	$out_cztflagbadpix=$cztpixclean_badpix."_";

	$cztflagbadpix = "cztflagbadpix nbadpixFiles=1 badpixfile=$cztpixclean_badpix outfile=$out_cztflagbadpix clobber=$Parms_str{clobber} history=$Parms_str{history} debug=$Parms_str{debug}";

	$lt = localtime; print $log "[$lt] $cztflagbadpix\n\n";

	if(system($cztflagbadpix)){$lt = localtime; print $log "[$lt] cztflagbadpix failed\n\n";print "cztflagbadpix failed\n";exit -1;}
	
		if(-f $out_cztflagbadpix)
		{
			rename $out_cztflagbadpix,$cztpixclean_badpix;
			unlink $out_cztflagbadpix;
		}


#### common CZTEVTCLEAN, without alpha and veto
$lt = localtime; print $log "[$lt] executing cztevtclean..........\n\n";

        if($Parms_str{gtitype} eq "COMMON" || $Parms_str{gtitype} eq "common")
		{
    	$cztevtclean_clean_evt = $cztpixclean_pc_evt;
	    $cztevtclean_clean_evt =~ s/common_bc_ds_pc.evt/common_clean.evt/g;
		}
		elsif($Parms_str{gtitype} eq "QUAD" || $Parms_str{gtitype} eq "quad")
		{
    	$cztevtclean_clean_evt = $cztpixclean_pc_evt;
	    $cztevtclean_clean_evt =~ s/quad_bc_ds_pc.evt/quad_clean.evt/g;
		}
		else
		{
	    print "Wrong GTI Type\n";
		exit -1;
		}


	$cztevtclean = "cztevtclean infile=$cztpixclean_pc_evt outfile=$cztevtclean_clean_evt alphaval=$Parms_str{alphasel} vetorange=$Parms_str{vetosel} isdoubleEvent=N clobber=$Parms_str{clobber} history=$Parms_str{history}";

	$lt = localtime; print $log "[$lt] $cztevtclean\n\n";

	if(system($cztevtclean)){$lt = localtime; print $log "[$lt] cztevtclean failed\n\n";print "cztevtclean failed\n";exit -1;}



}# End of if for stage 2

#**** Stage 3 ****#(cztdpigen,cztimage,cztbindata,cztrspgen)

if(($start_step==1 and $end_step==3) or ($start_step==2 and $end_step==3) or ($start_step==3 and $end_step==3))
{

print "*** Processing Stage 3 \n";

#### CZTDPIGEN
	$lt = localtime; print $log "[$lt] executing cztdpigen..........\n\n";
	$cleanevtfiles=`find -L $l2dir -name *_clean.evt|grep -v orbit|grep -v ignore|grep modeM0`;
	$num_files=`find -L $l2dir -name *_clean.evt|grep -v orbit|grep -v ignore|grep modeM0|wc -l`;
	if($num_files==1)
        {
			$l2evtfile=$cleanevtfiles
		}
	else{
		print "0 or >1 level2 _clean.evt files present\n";
		$lt = localtime; print $log "[$lt] zero or more than one level 2 _clean.evt present. EXITING....\n\n";
        exit -1;
        }
	        
		
		foreach $file (@cleanevtfiles)
        {
        if($file =~ m/_clean.evt/)
        {
	# create input event file
				$l2evtfile=$cleanevtfiles;
	# creatbad pix file
                $badpix_file = $l2evtfile;
                $badpix_file =~ s/_clean.evt/_badpix.fits/g;

	# create output dph file
                $outdph = $l2evtfile;
                $outdph =~ s/.evt/.dph/g;

	# create output dpi file
                $outdpi = $l2evtfile;
                $outdpi =~ s/.evt/.dpi/g;


	$cztdpigen = "cztdpigen par_infile=$l2evtfile par_badpixFile=$badpix_file par_badpixThreshold=$Parms_num{dpi_badpixthreshold} par_outDPHfile=$outdph par_outDPIfile=$outdpi par_quadsToProcess=$Parms_str{bindata_quadsToProcess} par_timerange=$Parms_str{dpi_timerange} par_ebins=$Parms_str{dpi_energybins}   par_clobber=$Parms_str{clobber} par_history=$Parms_str{history}";

	$lt = localtime; print $log "[$lt] $cztdpigen\n\n";

	if(system($cztdpigen)){$lt = localtime; print $log "[$lt] cztdpigen failed\n\n";print "cztdpigen failed\n";exit -1;}

	#### CZTIMAGE
	$lt = localtime; print $log "[$lt] executing cztimage..........\n\n";

	# assign variables
	$aspectfileQ0 = $aspectfile."_Q0";
	$aspectfileQ1 = $aspectfile."_Q1";
	$aspectfileQ2 = $aspectfile."_Q2";
	$aspectfileQ3 = $aspectfile."_Q3";

	# create output image file
                $outimg = $l2evtfile;
                $outimg =~ s/.evt/.img/g;

	$cztimage = "cztimage par_intype=$Parms_str{image_inputtype} par_infile=$outdpi par_aspectfileQ0=$aspectfileQ0 par_aspectfileQ1=$aspectfileQ1 par_aspectfileQ2=$aspectfileQ2 par_aspectfileQ3=$aspectfileQ3  par_outImgFile=$outimg  par_quadsToProcess=$Parms_str{bindata_quadsToProcess}  par_clobber=$Parms_str{clobber} par_history=$Parms_str{history} par_debugmode=$Parms_str{debug}";

	$lt = localtime; print $log "[$lt] $cztimage\n\n";
	
	if(system($cztimage)){$lt = localtime; print $log "[$lt] cztimage failed\n\n";print "cztimage failed\n";exit -1;}
	

#### CZTBINDATA
	$lt = localtime; print $log "[$lt] executing cztbindata..........\n\n";
	# create output livetimefile
                $livetimefile = $l2evtfile;
                $livetimefile =~ s/_clean.evt/_livetime.fits/g;

	# create output spectrum file and light curve
                $outspeclc = $l2evtfile;
                $outspeclc =~ s/.evt//g;

	# create output weight file
                $evtfilewt = $l2evtfile;
                $evtfilewt =~ s/.evt/_weight.evt/g;

	# create output exposure map file
                $expmap = $l2evtfile;
                $expmap =~ s/.evt/.expmap/g;
	$l2attfile =`find -L $l2dir -name *.att|grep -v orbit|grep -v ignore`;
	$num_files=`find -L $l2dir -name *.att|grep -v orbit|grep -v ignore|wc -l`;
	if($num_files==1)
        {
			$L2ATTFILE=$l2attfile;
		}
	else{
		print "0 or >1 level2 _clean.evt files present\n";
		$lt = localtime; print $log "[$lt] zero or more than one level 2 _clean.evt present. EXITING....\n\n";
        exit -1;
        }
	
		$cztbindata = "cztbindata inevtfile=$l2evtfile  attfile=$L2ATTFILE badpixfile=$badpix_file quadsToProcess=$Parms_str{bindata_quadsToProcess} badpixThreshold=$Parms_num{bindata_badpixthreshold} livetimefile=$livetimefile outputtype=$Parms_str{bindata_outputtype} energyrange=$Parms_str{lc_energyrange} generate_eventfile=$Parms_str{generate_eventfile} timebinsize=$Parms_num{lc_timebinsize} outfile=$outspeclc outevtfile=$evtfilewt applyLLD=$Parms_str{applyLLD}  maskWeight=$Parms_str{applymaskweight} rasrc=$Parms_str{rasrc} decsrc=$Parms_str{decsrc} clobber=$Parms_str{clobber} history=$Parms_str{history} debug=$Parms_str{debug}";

	$lt = localtime; print $log "[$lt] $cztbindata\n\n";

	if(system($cztbindata)){$lt = localtime; print $log "[$lt] cztbindata failed\n\n";print "cztbindata failed\n";exit -1;}
	}
	}
}	

