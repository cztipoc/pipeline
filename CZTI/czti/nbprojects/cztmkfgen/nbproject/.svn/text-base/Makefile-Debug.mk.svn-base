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
	${OBJECTDIR}/_ext/232673814/Mvector.o \
	${OBJECTDIR}/_ext/232673814/cztstring.o \
	${OBJECTDIR}/_ext/232673814/errorHandler.o \
	${OBJECTDIR}/_ext/232673814/level1handler.o \
	${OBJECTDIR}/_ext/232673814/mkfRegeneration.o \
	${OBJECTDIR}/_ext/232673814/utils.o \
	${OBJECTDIR}/_ext/881817382/cztmkfgen.o \
	${OBJECTDIR}/_ext/881817382/main.o


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
LDLIBSOPTIONS=-lpthread ../../lib/libcfitsio.a ../../lib/libglog.a ../../lib/libpil.a

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ../../bin/cztmkfgen

../../bin/cztmkfgen: ../../lib/libcfitsio.a

../../bin/cztmkfgen: ../../lib/libglog.a

../../bin/cztmkfgen: ../../lib/libpil.a

../../bin/cztmkfgen: ${OBJECTFILES}
	${MKDIR} -p ../../bin
	${LINK.cc} -o ../../bin/cztmkfgen ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/_ext/232673814/Mvector.o: ../../src/common/Mvector.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/232673814
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../include -I../../src/common -I../../src/cztmkfgen -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/232673814/Mvector.o ../../src/common/Mvector.cpp

${OBJECTDIR}/_ext/232673814/cztstring.o: ../../src/common/cztstring.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/232673814
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../include -I../../src/common -I../../src/cztmkfgen -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/232673814/cztstring.o ../../src/common/cztstring.cpp

${OBJECTDIR}/_ext/232673814/errorHandler.o: ../../src/common/errorHandler.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/232673814
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../include -I../../src/common -I../../src/cztmkfgen -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/232673814/errorHandler.o ../../src/common/errorHandler.cpp

${OBJECTDIR}/_ext/232673814/level1handler.o: ../../src/common/level1handler.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/232673814
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../include -I../../src/common -I../../src/cztmkfgen -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/232673814/level1handler.o ../../src/common/level1handler.cpp

${OBJECTDIR}/_ext/232673814/mkfRegeneration.o: ../../src/common/mkfRegeneration.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/232673814
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../include -I../../src/common -I../../src/cztmkfgen -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/232673814/mkfRegeneration.o ../../src/common/mkfRegeneration.cpp

${OBJECTDIR}/_ext/232673814/utils.o: ../../src/common/utils.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/232673814
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../include -I../../src/common -I../../src/cztmkfgen -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/232673814/utils.o ../../src/common/utils.cpp

${OBJECTDIR}/_ext/881817382/cztmkfgen.o: ../../src/cztmkfgen/cztmkfgen.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/881817382
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../include -I../../src/common -I../../src/cztmkfgen -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/881817382/cztmkfgen.o ../../src/cztmkfgen/cztmkfgen.cpp

${OBJECTDIR}/_ext/881817382/main.o: ../../src/cztmkfgen/main.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/881817382
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../include -I../../src/common -I../../src/cztmkfgen -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/881817382/main.o ../../src/cztmkfgen/main.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ../../bin/cztmkfgen

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
