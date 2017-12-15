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
	${OBJECTDIR}/_ext/1237661282/cztdatasel.o \
	${OBJECTDIR}/_ext/1237661282/main.o


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
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ../../bin/cztdatasel

../../bin/cztdatasel: ../../lib/libcfitsio.a

../../bin/cztdatasel: ../../lib/libglog.a

../../bin/cztdatasel: ../../lib/libpil.a

../../bin/cztdatasel: ${OBJECTFILES}
	${MKDIR} -p ../../bin
	${LINK.cc} -o ../../bin/cztdatasel ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/_ext/232673814/cztstring.o: ../../src/common/cztstring.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/232673814
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../src/common -I../../include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/232673814/cztstring.o ../../src/common/cztstring.cpp

${OBJECTDIR}/_ext/232673814/utils.o: ../../src/common/utils.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/232673814
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../src/common -I../../include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/232673814/utils.o ../../src/common/utils.cpp

${OBJECTDIR}/_ext/1237661282/cztdatasel.o: ../../src/cztdatasel/cztdatasel.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1237661282
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../src/common -I../../include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1237661282/cztdatasel.o ../../src/cztdatasel/cztdatasel.cpp

${OBJECTDIR}/_ext/1237661282/main.o: ../../src/cztdatasel/main.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1237661282
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../src/common -I../../include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1237661282/main.o ../../src/cztdatasel/main.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ../../bin/cztdatasel

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
