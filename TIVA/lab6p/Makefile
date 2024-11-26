
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
# The default rule, which causes the lab6p example to be built.
#
all: ${COMPILER}
all: ${COMPILER}/lab6p.axf

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
# Rules for building the lab6p example.
#
${COMPILER}/lab6p.axf: ${COMPILER}/lab6p.o
${COMPILER}/lab6p.axf: ${COMPILER}/startup_${COMPILER}.o
${COMPILER}/lab6p.axf: ${ROOT}/driverlib/${COMPILER}/libdriver.a
${COMPILER}/lab6p.axf: lab6p.ld
SCATTERgcc_lab6p=lab6p.ld
ENTRY_lab6p=ResetISR
CFLAGSgcc=-DTARGET_IS_TM4C129_RA1

#
# Include the automatically generated dependency files.
#
ifneq (${MAKECMDGOALS},clean)
-include ${wildcard ${COMPILER}/*.d} __dummy__
endif
