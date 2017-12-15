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
	${OBJECTDIR}/_ext/232673814/cztstring.o \
	${OBJECTDIR}/_ext/232673814/utils.o \
	${OBJECTDIR}/_ext/182293000/cztflagbadpix.o \
	${OBJECTDIR}/_ext/182293000/main.o


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
LDLIBSOPTIONS=-lpthread ../../lib/libcfitsio.a ../../lib/libglog.a ../../lib/libpil.a

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ../../bin/cztflagbadpix

../../bin/cztflagbadpix: ../../lib/libcfitsio.a

../../bin/cztflagbadpix: ../../lib/libglog.a

../../bin/cztflagbadpix: ../../lib/libpil.a

../../bin/cztflagbadpix: ${OBJECTFILES}
	${MKDIR} -p ../../bin
	${LINK.cc} -o ../../bin/cztflagbadpix ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/_ext/232673814/badpixCALDB.o: ../../src/common/badpixCALDB.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/232673814
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../src/common -I../../src/cztflagbadpix -I../../include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/232673814/badpixCALDB.o ../../src/common/badpixCALDB.cpp

${OBJECTDIR}/_ext/232673814/cztstring.o: ../../src/common/cztstring.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/232673814
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../src/common -I../../src/cztflagbadpix -I../../include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/232673814/cztstring.o ../../src/common/cztstring.cpp

${OBJECTDIR}/_ext/232673814/utils.o: ../../src/common/utils.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/232673814
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../src/common -I../../src/cztflagbadpix -I../../include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/232673814/utils.o ../../src/common/utils.cpp

${OBJECTDIR}/_ext/182293000/cztflagbadpix.o: ../../src/cztflagbadpix/cztflagbadpix.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/182293000
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../src/common -I../../src/cztflagbadpix -I../../include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/182293000/cztflagbadpix.o ../../src/cztflagbadpix/cztflagbadpix.cpp

${OBJECTDIR}/_ext/182293000/main.o: ../../src/cztflagbadpix/main.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/182293000
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../src/common -I../../src/cztflagbadpix -I../../include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/182293000/main.o ../../src/cztflagbadpix/main.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ../../bin/cztflagbadpix

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
