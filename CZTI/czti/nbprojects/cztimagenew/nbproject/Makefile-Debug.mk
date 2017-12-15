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
	${OBJECTDIR}/_ext/232673814/badpixCALDB.o \
	${OBJECTDIR}/_ext/232673814/caldbHandler.o \
	${OBJECTDIR}/_ext/232673814/coordinateTrasformation.o \
	${OBJECTDIR}/_ext/232673814/errorHandler.o \
	${OBJECTDIR}/_ext/232673814/skyimage.o \
	${OBJECTDIR}/_ext/232673814/utils.o \
	${OBJECTDIR}/_ext/1932611319/cztimagenew.o \
	${OBJECTDIR}/_ext/1932611319/main.o


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
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ../../bin/cztimagenew

../../bin/cztimagenew: ${OBJECTFILES}
	${MKDIR} -p ../../bin
	${LINK.cc} -o ../../bin/cztimagenew ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/_ext/232673814/badpixCALDB.o: ../../src/common/badpixCALDB.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/232673814
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../src/cztimagenew -I../../src/common -I../../include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/232673814/badpixCALDB.o ../../src/common/badpixCALDB.cpp

${OBJECTDIR}/_ext/232673814/caldbHandler.o: ../../src/common/caldbHandler.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/232673814
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../src/cztimagenew -I../../src/common -I../../include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/232673814/caldbHandler.o ../../src/common/caldbHandler.cpp

${OBJECTDIR}/_ext/232673814/coordinateTrasformation.o: ../../src/common/coordinateTrasformation.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/232673814
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../src/cztimagenew -I../../src/common -I../../include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/232673814/coordinateTrasformation.o ../../src/common/coordinateTrasformation.cpp

${OBJECTDIR}/_ext/232673814/errorHandler.o: ../../src/common/errorHandler.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/232673814
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../src/cztimagenew -I../../src/common -I../../include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/232673814/errorHandler.o ../../src/common/errorHandler.cpp

${OBJECTDIR}/_ext/232673814/skyimage.o: ../../src/common/skyimage.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/232673814
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../src/cztimagenew -I../../src/common -I../../include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/232673814/skyimage.o ../../src/common/skyimage.cpp

${OBJECTDIR}/_ext/232673814/utils.o: ../../src/common/utils.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/232673814
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../src/cztimagenew -I../../src/common -I../../include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/232673814/utils.o ../../src/common/utils.cpp

${OBJECTDIR}/_ext/1932611319/cztimagenew.o: ../../src/cztimagenew/cztimagenew.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1932611319
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../src/cztimagenew -I../../src/common -I../../include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1932611319/cztimagenew.o ../../src/cztimagenew/cztimagenew.cpp

${OBJECTDIR}/_ext/1932611319/main.o: ../../src/cztimagenew/main.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1932611319
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../src/cztimagenew -I../../src/common -I../../include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1932611319/main.o ../../src/cztimagenew/main.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ../../bin/cztimagenew

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
