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
	${OBJECTDIR}/_ext/232673814/caldbHandler.o \
	${OBJECTDIR}/_ext/232673814/coordinateTrasformation.o \
	${OBJECTDIR}/_ext/232673814/cztstring.o \
	${OBJECTDIR}/_ext/232673814/errorHandler.o \
	${OBJECTDIR}/_ext/232673814/l1evtdecode.o \
	${OBJECTDIR}/_ext/232673814/level1handler.o \
	${OBJECTDIR}/_ext/232673814/level2validation.o \
	${OBJECTDIR}/_ext/232673814/utils.o \
	${OBJECTDIR}/_ext/1250952262/cztscience2event.o \
	${OBJECTDIR}/_ext/1250952262/main.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-DNDEBUG
CXXFLAGS=-DNDEBUG

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=../../lib/libcfitsio.a ../../lib/libglog.a ../../lib/libpil.a -lpthread

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ../../bin/cztscience2event

../../bin/cztscience2event: ../../lib/libcfitsio.a

../../bin/cztscience2event: ../../lib/libglog.a

../../bin/cztscience2event: ../../lib/libpil.a

../../bin/cztscience2event: ${OBJECTFILES}
	${MKDIR} -p ../../bin
	${LINK.cc} -o ../../bin/cztscience2event ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/_ext/232673814/caldbHandler.o: ../../src/common/caldbHandler.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/232673814
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../include -I../../src/common -I../../src/cztscience2event -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/232673814/caldbHandler.o ../../src/common/caldbHandler.cpp

${OBJECTDIR}/_ext/232673814/coordinateTrasformation.o: ../../src/common/coordinateTrasformation.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/232673814
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../include -I../../src/common -I../../src/cztscience2event -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/232673814/coordinateTrasformation.o ../../src/common/coordinateTrasformation.cpp

${OBJECTDIR}/_ext/232673814/cztstring.o: ../../src/common/cztstring.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/232673814
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../include -I../../src/common -I../../src/cztscience2event -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/232673814/cztstring.o ../../src/common/cztstring.cpp

${OBJECTDIR}/_ext/232673814/errorHandler.o: ../../src/common/errorHandler.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/232673814
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../include -I../../src/common -I../../src/cztscience2event -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/232673814/errorHandler.o ../../src/common/errorHandler.cpp

${OBJECTDIR}/_ext/232673814/l1evtdecode.o: ../../src/common/l1evtdecode.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/232673814
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../include -I../../src/common -I../../src/cztscience2event -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/232673814/l1evtdecode.o ../../src/common/l1evtdecode.cpp

${OBJECTDIR}/_ext/232673814/level1handler.o: ../../src/common/level1handler.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/232673814
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../include -I../../src/common -I../../src/cztscience2event -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/232673814/level1handler.o ../../src/common/level1handler.cpp

${OBJECTDIR}/_ext/232673814/level2validation.o: ../../src/common/level2validation.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/232673814
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../include -I../../src/common -I../../src/cztscience2event -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/232673814/level2validation.o ../../src/common/level2validation.cpp

${OBJECTDIR}/_ext/232673814/utils.o: ../../src/common/utils.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/232673814
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../include -I../../src/common -I../../src/cztscience2event -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/232673814/utils.o ../../src/common/utils.cpp

${OBJECTDIR}/_ext/1250952262/cztscience2event.o: ../../src/cztscience2event/cztscience2event.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1250952262
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../include -I../../src/common -I../../src/cztscience2event -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1250952262/cztscience2event.o ../../src/cztscience2event/cztscience2event.cpp

${OBJECTDIR}/_ext/1250952262/main.o: ../../src/cztscience2event/main.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1250952262
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../include -I../../src/common -I../../src/cztscience2event -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1250952262/main.o ../../src/cztscience2event/main.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ../../bin/cztscience2event

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
