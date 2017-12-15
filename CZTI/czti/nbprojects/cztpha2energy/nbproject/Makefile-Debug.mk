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
	${OBJECTDIR}/_ext/232673814/cztstring.o \
	${OBJECTDIR}/_ext/232673814/errorHandler.o \
	${OBJECTDIR}/_ext/232673814/level2validation.o \
	${OBJECTDIR}/_ext/232673814/utils.o \
	${OBJECTDIR}/_ext/1092292547/cztpha2energyv2.o \
	${OBJECTDIR}/_ext/1092292547/main.o


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
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ../../bin/cztpha2energy

../../bin/cztpha2energy: ../../lib/libcfitsio.a

../../bin/cztpha2energy: ../../lib/libglog.a

../../bin/cztpha2energy: ../../lib/libpil.a

../../bin/cztpha2energy: ${OBJECTFILES}
	${MKDIR} -p ../../bin
	${LINK.cc} -o ../../bin/cztpha2energy ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/_ext/232673814/caldbHandler.o: ../../src/common/caldbHandler.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/232673814
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../src/common -I../../include -I../../src/cztpha2energy -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/232673814/caldbHandler.o ../../src/common/caldbHandler.cpp

${OBJECTDIR}/_ext/232673814/cztstring.o: ../../src/common/cztstring.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/232673814
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../src/common -I../../include -I../../src/cztpha2energy -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/232673814/cztstring.o ../../src/common/cztstring.cpp

${OBJECTDIR}/_ext/232673814/errorHandler.o: ../../src/common/errorHandler.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/232673814
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../src/common -I../../include -I../../src/cztpha2energy -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/232673814/errorHandler.o ../../src/common/errorHandler.cpp

${OBJECTDIR}/_ext/232673814/level2validation.o: ../../src/common/level2validation.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/232673814
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../src/common -I../../include -I../../src/cztpha2energy -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/232673814/level2validation.o ../../src/common/level2validation.cpp

${OBJECTDIR}/_ext/232673814/utils.o: ../../src/common/utils.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/232673814
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../src/common -I../../include -I../../src/cztpha2energy -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/232673814/utils.o ../../src/common/utils.cpp

${OBJECTDIR}/_ext/1092292547/cztpha2energyv2.o: ../../src/cztpha2energy/cztpha2energyv2.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1092292547
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../src/common -I../../include -I../../src/cztpha2energy -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1092292547/cztpha2energyv2.o ../../src/cztpha2energy/cztpha2energyv2.cpp

${OBJECTDIR}/_ext/1092292547/main.o: ../../src/cztpha2energy/main.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1092292547
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../src/common -I../../include -I../../src/cztpha2energy -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1092292547/main.o ../../src/cztpha2energy/main.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ../../bin/cztpha2energy

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
