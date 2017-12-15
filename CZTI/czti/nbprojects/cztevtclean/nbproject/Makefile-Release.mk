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
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/232673814/cztstring.o \
	${OBJECTDIR}/_ext/232673814/errorHandler.o \
	${OBJECTDIR}/_ext/232673814/utils.o \
	${OBJECTDIR}/_ext/805604648/cztevtclean.o \
	${OBJECTDIR}/_ext/805604648/main.o


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
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/cztevtclean

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/cztevtclean: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/cztevtclean ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/_ext/232673814/cztstring.o: ../../src/common/cztstring.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/232673814
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/232673814/cztstring.o ../../src/common/cztstring.cpp

${OBJECTDIR}/_ext/232673814/errorHandler.o: ../../src/common/errorHandler.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/232673814
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/232673814/errorHandler.o ../../src/common/errorHandler.cpp

${OBJECTDIR}/_ext/232673814/utils.o: ../../src/common/utils.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/232673814
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/232673814/utils.o ../../src/common/utils.cpp

${OBJECTDIR}/_ext/805604648/cztevtclean.o: ../../src/cztevtclean/cztevtclean.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/805604648
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/805604648/cztevtclean.o ../../src/cztevtclean/cztevtclean.cpp

${OBJECTDIR}/_ext/805604648/main.o: ../../src/cztevtclean/main.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/805604648
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/805604648/main.o ../../src/cztevtclean/main.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/cztevtclean

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
