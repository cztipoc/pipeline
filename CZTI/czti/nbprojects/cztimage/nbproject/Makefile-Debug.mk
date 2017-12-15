#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux-x86
CND_DLIB_EXT=so
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/232673814/ExpMap.o \
	${OBJECTDIR}/_ext/232673814/Exposure.o \
	${OBJECTDIR}/_ext/232673814/Mvector.o \
	${OBJECTDIR}/_ext/232673814/alglibinternal.o \
	${OBJECTDIR}/_ext/232673814/alglibmisc.o \
	${OBJECTDIR}/_ext/232673814/ap.o \
	${OBJECTDIR}/_ext/232673814/badpixCALDB.o \
	${OBJECTDIR}/_ext/232673814/caldbHandler.o \
	${OBJECTDIR}/_ext/232673814/coordinateTrasformation.o \
	${OBJECTDIR}/_ext/232673814/cztstring.o \
	${OBJECTDIR}/_ext/232673814/detectorGeometry.o \
	${OBJECTDIR}/_ext/232673814/errorHandler.o \
	${OBJECTDIR}/_ext/232673814/jpeg_handling.o \
	${OBJECTDIR}/_ext/232673814/level1handler.o \
	${OBJECTDIR}/_ext/232673814/level2validation.o \
	${OBJECTDIR}/_ext/232673814/linalg.o \
	${OBJECTDIR}/_ext/232673814/maskGeometry.o \
	${OBJECTDIR}/_ext/232673814/mkfRegeneration.o \
	${OBJECTDIR}/_ext/232673814/utils.o \
	${OBJECTDIR}/_ext/232673814/validations.o \
	${OBJECTDIR}/_ext/1630482953/cztimage.o \
	${OBJECTDIR}/_ext/1630482953/cztimage_v2.o \
	${OBJECTDIR}/_ext/1630482953/main.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-lm ../../lib/libcfitsio.a ../../lib/libglog.a ../../lib/libpil.a -lpthread ../../lib/libjpeg.a

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ../../bin/cztimage

../../bin/cztimage: ../../lib/libcfitsio.a

../../bin/cztimage: ../../lib/libglog.a

../../bin/cztimage: ../../lib/libpil.a

../../bin/cztimage: ../../lib/libjpeg.a

../../bin/cztimage: ${OBJECTFILES}
	${MKDIR} -p ../../bin
	${LINK.cc} -o ../../bin/cztimage ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/_ext/232673814/ExpMap.o: ../../src/common/ExpMap.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/232673814
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../src/common -I../../include -I../../src/cztimage -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/232673814/ExpMap.o ../../src/common/ExpMap.cpp

${OBJECTDIR}/_ext/232673814/Exposure.o: ../../src/common/Exposure.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/232673814
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../src/common -I../../include -I../../src/cztimage -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/232673814/Exposure.o ../../src/common/Exposure.cpp

${OBJECTDIR}/_ext/232673814/Mvector.o: ../../src/common/Mvector.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/232673814
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../src/common -I../../include -I../../src/cztimage -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/232673814/Mvector.o ../../src/common/Mvector.cpp

${OBJECTDIR}/_ext/232673814/alglibinternal.o: ../../src/common/alglibinternal.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/232673814
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../src/common -I../../include -I../../src/cztimage -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/232673814/alglibinternal.o ../../src/common/alglibinternal.cpp

${OBJECTDIR}/_ext/232673814/alglibmisc.o: ../../src/common/alglibmisc.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/232673814
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../src/common -I../../include -I../../src/cztimage -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/232673814/alglibmisc.o ../../src/common/alglibmisc.cpp

${OBJECTDIR}/_ext/232673814/ap.o: ../../src/common/ap.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/232673814
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../src/common -I../../include -I../../src/cztimage -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/232673814/ap.o ../../src/common/ap.cpp

${OBJECTDIR}/_ext/232673814/badpixCALDB.o: ../../src/common/badpixCALDB.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/232673814
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../src/common -I../../include -I../../src/cztimage -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/232673814/badpixCALDB.o ../../src/common/badpixCALDB.cpp

${OBJECTDIR}/_ext/232673814/caldbHandler.o: ../../src/common/caldbHandler.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/232673814
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../src/common -I../../include -I../../src/cztimage -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/232673814/caldbHandler.o ../../src/common/caldbHandler.cpp

${OBJECTDIR}/_ext/232673814/coordinateTrasformation.o: ../../src/common/coordinateTrasformation.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/232673814
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../src/common -I../../include -I../../src/cztimage -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/232673814/coordinateTrasformation.o ../../src/common/coordinateTrasformation.cpp

${OBJECTDIR}/_ext/232673814/cztstring.o: ../../src/common/cztstring.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/232673814
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../src/common -I../../include -I../../src/cztimage -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/232673814/cztstring.o ../../src/common/cztstring.cpp

${OBJECTDIR}/_ext/232673814/detectorGeometry.o: ../../src/common/detectorGeometry.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/232673814
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../src/common -I../../include -I../../src/cztimage -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/232673814/detectorGeometry.o ../../src/common/detectorGeometry.cpp

${OBJECTDIR}/_ext/232673814/errorHandler.o: ../../src/common/errorHandler.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/232673814
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../src/common -I../../include -I../../src/cztimage -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/232673814/errorHandler.o ../../src/common/errorHandler.cpp

${OBJECTDIR}/_ext/232673814/jpeg_handling.o: ../../src/common/jpeg_handling.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/232673814
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../src/common -I../../include -I../../src/cztimage -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/232673814/jpeg_handling.o ../../src/common/jpeg_handling.cpp

${OBJECTDIR}/_ext/232673814/level1handler.o: ../../src/common/level1handler.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/232673814
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../src/common -I../../include -I../../src/cztimage -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/232673814/level1handler.o ../../src/common/level1handler.cpp

${OBJECTDIR}/_ext/232673814/level2validation.o: ../../src/common/level2validation.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/232673814
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../src/common -I../../include -I../../src/cztimage -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/232673814/level2validation.o ../../src/common/level2validation.cpp

${OBJECTDIR}/_ext/232673814/linalg.o: ../../src/common/linalg.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/232673814
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../src/common -I../../include -I../../src/cztimage -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/232673814/linalg.o ../../src/common/linalg.cpp

${OBJECTDIR}/_ext/232673814/maskGeometry.o: ../../src/common/maskGeometry.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/232673814
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../src/common -I../../include -I../../src/cztimage -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/232673814/maskGeometry.o ../../src/common/maskGeometry.cpp

${OBJECTDIR}/_ext/232673814/mkfRegeneration.o: ../../src/common/mkfRegeneration.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/232673814
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../src/common -I../../include -I../../src/cztimage -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/232673814/mkfRegeneration.o ../../src/common/mkfRegeneration.cpp

${OBJECTDIR}/_ext/232673814/utils.o: ../../src/common/utils.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/232673814
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../src/common -I../../include -I../../src/cztimage -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/232673814/utils.o ../../src/common/utils.cpp

${OBJECTDIR}/_ext/232673814/validations.o: ../../src/common/validations.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/232673814
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../src/common -I../../include -I../../src/cztimage -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/232673814/validations.o ../../src/common/validations.cpp

${OBJECTDIR}/_ext/1630482953/cztimage.o: ../../src/cztimage/cztimage.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1630482953
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../src/common -I../../include -I../../src/cztimage -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1630482953/cztimage.o ../../src/cztimage/cztimage.cpp

${OBJECTDIR}/_ext/1630482953/cztimage_v2.o: ../../src/cztimage/cztimage_v2.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1630482953
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../src/common -I../../include -I../../src/cztimage -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1630482953/cztimage_v2.o ../../src/cztimage/cztimage_v2.cpp

${OBJECTDIR}/_ext/1630482953/main.o: ../../src/cztimage/main.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1630482953
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../src/common -I../../include -I../../src/cztimage -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1630482953/main.o ../../src/cztimage/main.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ../../bin/cztimage

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
