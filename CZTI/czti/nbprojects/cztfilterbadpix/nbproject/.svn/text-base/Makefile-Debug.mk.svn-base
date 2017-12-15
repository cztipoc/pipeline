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
	${OBJECTDIR}/_ext/232673814/cztstring.o \
	${OBJECTDIR}/_ext/232673814/utils.o \
	${OBJECTDIR}/_ext/887112836/cztfilterbadpix.o \
	${OBJECTDIR}/_ext/887112836/main.o


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
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ../../bin/cztfilterbadpix

../../bin/cztfilterbadpix: ../../lib/libcfitsio.a

../../bin/cztfilterbadpix: ../../lib/libglog.a

../../bin/cztfilterbadpix: ../../lib/libpil.a

../../bin/cztfilterbadpix: ${OBJECTFILES}
	${MKDIR} -p ../../bin
	${LINK.cc} -o ../../bin/cztfilterbadpix ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/_ext/232673814/cztstring.o: ../../src/common/cztstring.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/232673814
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../include -I../../src/common -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/232673814/cztstring.o ../../src/common/cztstring.cpp

${OBJECTDIR}/_ext/232673814/utils.o: ../../src/common/utils.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/232673814
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../include -I../../src/common -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/232673814/utils.o ../../src/common/utils.cpp

${OBJECTDIR}/_ext/887112836/cztfilterbadpix.o: ../../src/cztfilterbadpix/cztfilterbadpix.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/887112836
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../include -I../../src/common -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/887112836/cztfilterbadpix.o ../../src/cztfilterbadpix/cztfilterbadpix.cpp

${OBJECTDIR}/_ext/887112836/main.o: ../../src/cztfilterbadpix/main.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/887112836
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../include -I../../src/common -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/887112836/main.o ../../src/cztfilterbadpix/main.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ../../bin/cztfilterbadpix

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
