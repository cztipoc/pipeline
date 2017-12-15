export AS1
INCLUDE1 = -I. -I../common/ -I../../../include/  -I $(SRCDIR)/cztscience2event -I $(SRCDIR)/cztpha2energy -I $(SRCDIR)/cztgtigen -I $(SRCDIR)/cztevtclean -I $(SRCDIR)/cztfindbadpix -I $(SRCDIR)/cztfilterbadpix -I $(SRCDIR)/cztdatasel -I $(SRCDIR)/cztgaas -I $(SRCDIR)/cztdpigen -I $(SRCDIR)/cztbindatanew -I $(SRCDIR)/cztpsfshadowgen -I $(SRCDIR)/cztimage -I $(SRCDIR)/cztexpmap -I $(SRCDIR)/cztflagbadpix
CDIR=../common
SRCDIR=..


EXE=cztpipeline	

SOURCES=main.cpp cztpipeline.cpp $(SRCDIR)/cztscience2event/cztscience2event.cpp $(SRCDIR)/cztpha2energy/cztpha2energyv2.cpp $(SRCDIR)/cztgtigen/cztgtigen.cpp $(SRCDIR)/cztevtclean/cztevtclean.cpp $(SRCDIR)/cztflagbadpix/cztflagbadpix.cpp $(SRCDIR)/cztdatasel/cztdatasel.cpp $(SRCDIR)/cztgaas/cztgaas_v2.cpp $(SRCDIR)/cztdpigen/cztdpigen_v2.cpp $(SRCDIR)/cztbindatanew/cztbindatanew.cpp $(SRCDIR)/cztpsfshadowgen/cztpsfshadowgen.cpp $(SRCDIR)/cztimage/cztimage.cpp $(CDIR)/utils.cpp $(CDIR)/cztstring.cpp $(CDIR)/ExpMap.cpp $(CDIR)/coordinateTrasformation.cpp $(CDIR)/level1handler.cpp $(CDIR)/level2validation.cpp $(CDIR)/validations.cpp $(CDIR)/caldbHandler.cpp $(CDIR)/jpeg_handling.cpp $(CDIR)/gtiHandler.cpp \
		$(CDIR)/linalg.cpp $(CDIR)/alglibinternal.cpp $(CDIR)/alglibmisc.cpp \
		$(CDIR)/ap.cpp $(CDIR)/badpixCALDB.cpp $(CDIR)/maskGeometry.cpp $(CDIR)/Exposure.cpp $(CDIR)/detectorGeometry.cpp $(CDIR)/l1evtdecode.cpp $(CDIR)/errorHandler.cpp

include $(as1czt)/makefile.common
