
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
# The default rule, which causes the Labo6_3 example to be built.
#
all: ${COMPILER}
all: ${COMPILER}/Labo6_3.axf

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
# Rules for building the Labo6_3 example.
#
${COMPILER}/Labo6_3.axf: ${COMPILER}/Labo6_3.o
${COMPILER}/Labo6_3.axf: ${COMPILER}/startup_${COMPILER}.o
${COMPILER}/Labo6_3.axf: ${ROOT}/driverlib/${COMPILER}/libdriver.a
${COMPILER}/Labo6_3.axf: Labo6_3.ld
SCATTERgcc_Labo6_3=Labo6_3.ld
ENTRY_Labo6_3=ResetISR
CFLAGSgcc=-DTARGET_IS_TM4C129_RA1

#
# Include the automatically generated dependency files.
#
ifneq (${MAKECMDGOALS},clean)
-include ${wildcard ${COMPILER}/*.d} __dummy__
endif
