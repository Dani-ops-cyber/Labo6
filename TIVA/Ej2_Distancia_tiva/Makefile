#******************************************************************************

#******************************************************************************

#
# Defines the part type that this project uses.
#
PART=TM4C1294NCPDT

#
# The base directory for TivaWare.
#
ROOT=../../../..

#
# Include the common make definitions.
#
include ${ROOT}/makedefs

#
# Where to find header files that do not live in the source directory.
#
IPATH=../../../..

#
# The default rule, which causes the auxp example to be built.
#
all: ${COMPILER}
all: ${COMPILER}/auxp.axf

#
# The rule to clean out all the build products.
#
clean:
	@rm -rf ${COMPILER} ${wildcard *~}

#
# The rule to create the target directory.
#
${COMPILER}:
	@mkdir -p ${COMPILER}

#
# Rules for building the auxp example.
#
${COMPILER}/auxp.axf: ${COMPILER}/auxp.o
${COMPILER}/auxp.axf: ${COMPILER}/startup_${COMPILER}.o
${COMPILER}/auxp.axf: ${ROOT}/driverlib/${COMPILER}/libdriver.a
${COMPILER}/auxp.axf: auxp.ld
SCATTERgcc_auxp=auxp.ld
ENTRY_auxp=ResetISR
CFLAGSgcc=-DTARGET_IS_TM4C129_RA1

#
# Include the automatically generated dependency files.
#
ifneq (${MAKECMDGOALS},clean)
-include ${wildcard ${COMPILER}/*.d} __dummy__
endif
