
if(($start_step==1 and $end_step==2) or ($start_step==1 and $end_step==3)  or ($start_step==1 and $end_step==1))
{

		print "*** Processing Stage 1 \n";

		#### CZTSCIENCE2EVENT


		$lt = localtime; print $log "[$lt] executing cztscience2event..........\n\n";
		$modefiles=`find -L $l1dir -name *.fits`;
