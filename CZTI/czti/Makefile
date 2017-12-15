#this make file is for AstroSat CZTI Pipeline
#!/bin/csh
SRC_PATH=src

all:
	make -C $(SRC_PATH)/cztattcorrect
	make -C $(SRC_PATH)/cztmergeaux
	make -C $(SRC_PATH)/cztorbcorrect
	make -C $(SRC_PATH)/czttctheal
	make -C $(SRC_PATH)/cztmerge
	make -C $(SRC_PATH)/cztscience2event
	make -C $(SRC_PATH)/cztpha2energy
	make -C $(SRC_PATH)/cztbunchclean
	make -C $(SRC_PATH)/cztgaas
	make -C $(SRC_PATH)/cztgtigen
	make -C $(SRC_PATH)/cztmkfgen			
	make -C $(SRC_PATH)/cztdatasel
	make -C $(SRC_PATH)/cztpixclean	
	make -C $(SRC_PATH)/cztevtclean
	make -C $(SRC_PATH)/cztflagbadpix
	make -C $(SRC_PATH)/cztdpigen
	make -C $(SRC_PATH)/cztimage
	make -C $(SRC_PATH)/cztbindata
	make -C $(SRC_PATH)/cztrspgen
#	make -C $(SRC_PATH)/cztpsfshadowgen
#	make -C $(SRC_PATH)/cztgenshadow
#	make -C $(SRC_PATH)/cztexpmap
#	make -C $(SRC_PATH)/cztpipelne	
	make -C $(SRC_PATH)/cztattcorrect
	make -C $(SRC_PATH)/cztorbcorrect
	make -C $(SRC_PATH)/cztmergeaux
	make -C $(SRC_PATH)/czttctheal
	make -C $(SRC_PATH)/cztdataqualitycheck
	@echo "----------------------------------------"
	@echo "Compilation of All modules of CZTI pipeline completed successfully"
	@echo "----------------------------------------"




clean:
	
	make -C $(SRC_PATH)/cztattcorrect clean
	make -C $(SRC_PATH)/cztmergeaux clean
	make -C $(SRC_PATH)/cztorbcorrect clean
	make -C $(SRC_PATH)/czttctheal clean
	make -C $(SRC_PATH)/cztmerge clean
	make -C $(SRC_PATH)/cztscience2event clean
	make -C $(SRC_PATH)/cztpha2energy clean
	make -C $(SRC_PATH)/cztbunchclean clean
	make -C $(SRC_PATH)/cztgaas clean
	make -C $(SRC_PATH)/cztgtigen clean
	make -C $(SRC_PATH)/cztmkfgen clean            
	make -C $(SRC_PATH)/cztdatasel clean
	make -C $(SRC_PATH)/cztpixclean clean
	make -C $(SRC_PATH)/cztevtclean clean
	make -C $(SRC_PATH)/cztflagbadpix clean
	make -C $(SRC_PATH)/cztdpigen clean
	make -C $(SRC_PATH)/cztimage clean
	make -C $(SRC_PATH)/cztbindata clean
	make -C $(SRC_PATH)/cztrspgen clean 
#	make -C $(SRC_PATH)/cztpsfshadowgen clean
#	make -C $(SRC_PATH)/cztgenshadow clean
#	make -C $(SRC_PATH)/cztexpmap clean
	make -C $(SRC_PATH)/cztattcorrect clean
	make -C $(SRC_PATH)/cztorbcorrect clean	
	make -C $(SRC_PATH)/cztmergeaux clean
	make -C $(SRC_PATH)/czttctheal clean
	make -C $(SRC_PATH)/cztdataqualitycheck clean
distclean:
	rm  bin/*
	
		


