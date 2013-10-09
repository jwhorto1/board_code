#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/wifly_pass_thru_demo_exp16_p24.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/wifly_pass_thru_demo_exp16_p24.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/1360930230/delay_ms.o ${OBJECTDIR}/_ext/1360930230/indicators.o ${OBJECTDIR}/_ext/1360930230/lcd.o ${OBJECTDIR}/_ext/1360930230/switches.o ${OBJECTDIR}/_ext/1360930230/console.o ${OBJECTDIR}/_ext/1360930230/wifly_drv.o ${OBJECTDIR}/_ext/1360930230/adc.o ${OBJECTDIR}/_ext/1360930230/buttons.o ${OBJECTDIR}/_ext/1472/main.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/1360930230/delay_ms.o.d ${OBJECTDIR}/_ext/1360930230/indicators.o.d ${OBJECTDIR}/_ext/1360930230/lcd.o.d ${OBJECTDIR}/_ext/1360930230/switches.o.d ${OBJECTDIR}/_ext/1360930230/console.o.d ${OBJECTDIR}/_ext/1360930230/wifly_drv.o.d ${OBJECTDIR}/_ext/1360930230/adc.o.d ${OBJECTDIR}/_ext/1360930230/buttons.o.d ${OBJECTDIR}/_ext/1472/main.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/1360930230/delay_ms.o ${OBJECTDIR}/_ext/1360930230/indicators.o ${OBJECTDIR}/_ext/1360930230/lcd.o ${OBJECTDIR}/_ext/1360930230/switches.o ${OBJECTDIR}/_ext/1360930230/console.o ${OBJECTDIR}/_ext/1360930230/wifly_drv.o ${OBJECTDIR}/_ext/1360930230/adc.o ${OBJECTDIR}/_ext/1360930230/buttons.o ${OBJECTDIR}/_ext/1472/main.o


CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/wifly_pass_thru_demo_exp16_p24.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=24FJ128GA010
MP_LINKER_FILE_OPTION=,--script=p24FJ128GA010.gld
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1360930230/delay_ms.o: ../lib/delay_ms.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360930230 
	@${RM} ${OBJECTDIR}/_ext/1360930230/delay_ms.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../lib/delay_ms.c  -o ${OBJECTDIR}/_ext/1360930230/delay_ms.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1360930230/delay_ms.o.d"    -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -DAPP_RUN -O0 -I"..\include" -I"C:\push API\firmware" -I"C:\push API\firmware\include" -I"C:\push API\firmware\lib" -I"C:\Program Files (x86)\Microchip\xc16\v1.11" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360930230/delay_ms.o.d" $(SILENT) 
	
${OBJECTDIR}/_ext/1360930230/indicators.o: ../lib/indicators.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360930230 
	@${RM} ${OBJECTDIR}/_ext/1360930230/indicators.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../lib/indicators.c  -o ${OBJECTDIR}/_ext/1360930230/indicators.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1360930230/indicators.o.d"    -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -DAPP_RUN -O0 -I"..\include" -I"C:\push API\firmware" -I"C:\push API\firmware\include" -I"C:\push API\firmware\lib" -I"C:\Program Files (x86)\Microchip\xc16\v1.11" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360930230/indicators.o.d" $(SILENT) 
	
${OBJECTDIR}/_ext/1360930230/lcd.o: ../lib/lcd.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360930230 
	@${RM} ${OBJECTDIR}/_ext/1360930230/lcd.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../lib/lcd.c  -o ${OBJECTDIR}/_ext/1360930230/lcd.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1360930230/lcd.o.d"    -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -DAPP_RUN -O0 -I"..\include" -I"C:\push API\firmware" -I"C:\push API\firmware\include" -I"C:\push API\firmware\lib" -I"C:\Program Files (x86)\Microchip\xc16\v1.11" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360930230/lcd.o.d" $(SILENT) 
	
${OBJECTDIR}/_ext/1360930230/switches.o: ../lib/switches.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360930230 
	@${RM} ${OBJECTDIR}/_ext/1360930230/switches.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../lib/switches.c  -o ${OBJECTDIR}/_ext/1360930230/switches.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1360930230/switches.o.d"    -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -DAPP_RUN -O0 -I"..\include" -I"C:\push API\firmware" -I"C:\push API\firmware\include" -I"C:\push API\firmware\lib" -I"C:\Program Files (x86)\Microchip\xc16\v1.11" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360930230/switches.o.d" $(SILENT) 
	
${OBJECTDIR}/_ext/1360930230/console.o: ../lib/console.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360930230 
	@${RM} ${OBJECTDIR}/_ext/1360930230/console.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../lib/console.c  -o ${OBJECTDIR}/_ext/1360930230/console.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1360930230/console.o.d"    -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -DAPP_RUN -O0 -I"..\include" -I"C:\push API\firmware" -I"C:\push API\firmware\include" -I"C:\push API\firmware\lib" -I"C:\Program Files (x86)\Microchip\xc16\v1.11" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360930230/console.o.d" $(SILENT) 
	
${OBJECTDIR}/_ext/1360930230/wifly_drv.o: ../lib/wifly_drv.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360930230 
	@${RM} ${OBJECTDIR}/_ext/1360930230/wifly_drv.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../lib/wifly_drv.c  -o ${OBJECTDIR}/_ext/1360930230/wifly_drv.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1360930230/wifly_drv.o.d"    -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -DAPP_RUN -O0 -I"..\include" -I"C:\push API\firmware" -I"C:\push API\firmware\include" -I"C:\push API\firmware\lib" -I"C:\Program Files (x86)\Microchip\xc16\v1.11" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360930230/wifly_drv.o.d" $(SILENT) 
	
${OBJECTDIR}/_ext/1360930230/adc.o: ../lib/adc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360930230 
	@${RM} ${OBJECTDIR}/_ext/1360930230/adc.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../lib/adc.c  -o ${OBJECTDIR}/_ext/1360930230/adc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1360930230/adc.o.d"    -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -DAPP_RUN -O0 -I"..\include" -I"C:\push API\firmware" -I"C:\push API\firmware\include" -I"C:\push API\firmware\lib" -I"C:\Program Files (x86)\Microchip\xc16\v1.11" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360930230/adc.o.d" $(SILENT) 
	
${OBJECTDIR}/_ext/1360930230/buttons.o: ../lib/buttons.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360930230 
	@${RM} ${OBJECTDIR}/_ext/1360930230/buttons.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../lib/buttons.c  -o ${OBJECTDIR}/_ext/1360930230/buttons.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1360930230/buttons.o.d"    -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -DAPP_RUN -O0 -I"..\include" -I"C:\push API\firmware" -I"C:\push API\firmware\include" -I"C:\push API\firmware\lib" -I"C:\Program Files (x86)\Microchip\xc16\v1.11" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360930230/buttons.o.d" $(SILENT) 
	
${OBJECTDIR}/_ext/1472/main.o: ../main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../main.c  -o ${OBJECTDIR}/_ext/1472/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/main.o.d"    -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -DAPP_RUN -O0 -I"..\include" -I"C:\push API\firmware" -I"C:\push API\firmware\include" -I"C:\push API\firmware\lib" -I"C:\Program Files (x86)\Microchip\xc16\v1.11" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/main.o.d" $(SILENT) 
	
else
${OBJECTDIR}/_ext/1360930230/delay_ms.o: ../lib/delay_ms.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360930230 
	@${RM} ${OBJECTDIR}/_ext/1360930230/delay_ms.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../lib/delay_ms.c  -o ${OBJECTDIR}/_ext/1360930230/delay_ms.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1360930230/delay_ms.o.d"    -g -omf=elf -DAPP_RUN -O0 -I"..\include" -I"C:\push API\firmware" -I"C:\push API\firmware\include" -I"C:\push API\firmware\lib" -I"C:\Program Files (x86)\Microchip\xc16\v1.11" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360930230/delay_ms.o.d" $(SILENT) 
	
${OBJECTDIR}/_ext/1360930230/indicators.o: ../lib/indicators.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360930230 
	@${RM} ${OBJECTDIR}/_ext/1360930230/indicators.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../lib/indicators.c  -o ${OBJECTDIR}/_ext/1360930230/indicators.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1360930230/indicators.o.d"    -g -omf=elf -DAPP_RUN -O0 -I"..\include" -I"C:\push API\firmware" -I"C:\push API\firmware\include" -I"C:\push API\firmware\lib" -I"C:\Program Files (x86)\Microchip\xc16\v1.11" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360930230/indicators.o.d" $(SILENT) 
	
${OBJECTDIR}/_ext/1360930230/lcd.o: ../lib/lcd.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360930230 
	@${RM} ${OBJECTDIR}/_ext/1360930230/lcd.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../lib/lcd.c  -o ${OBJECTDIR}/_ext/1360930230/lcd.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1360930230/lcd.o.d"    -g -omf=elf -DAPP_RUN -O0 -I"..\include" -I"C:\push API\firmware" -I"C:\push API\firmware\include" -I"C:\push API\firmware\lib" -I"C:\Program Files (x86)\Microchip\xc16\v1.11" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360930230/lcd.o.d" $(SILENT) 
	
${OBJECTDIR}/_ext/1360930230/switches.o: ../lib/switches.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360930230 
	@${RM} ${OBJECTDIR}/_ext/1360930230/switches.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../lib/switches.c  -o ${OBJECTDIR}/_ext/1360930230/switches.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1360930230/switches.o.d"    -g -omf=elf -DAPP_RUN -O0 -I"..\include" -I"C:\push API\firmware" -I"C:\push API\firmware\include" -I"C:\push API\firmware\lib" -I"C:\Program Files (x86)\Microchip\xc16\v1.11" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360930230/switches.o.d" $(SILENT) 
	
${OBJECTDIR}/_ext/1360930230/console.o: ../lib/console.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360930230 
	@${RM} ${OBJECTDIR}/_ext/1360930230/console.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../lib/console.c  -o ${OBJECTDIR}/_ext/1360930230/console.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1360930230/console.o.d"    -g -omf=elf -DAPP_RUN -O0 -I"..\include" -I"C:\push API\firmware" -I"C:\push API\firmware\include" -I"C:\push API\firmware\lib" -I"C:\Program Files (x86)\Microchip\xc16\v1.11" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360930230/console.o.d" $(SILENT) 
	
${OBJECTDIR}/_ext/1360930230/wifly_drv.o: ../lib/wifly_drv.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360930230 
	@${RM} ${OBJECTDIR}/_ext/1360930230/wifly_drv.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../lib/wifly_drv.c  -o ${OBJECTDIR}/_ext/1360930230/wifly_drv.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1360930230/wifly_drv.o.d"    -g -omf=elf -DAPP_RUN -O0 -I"..\include" -I"C:\push API\firmware" -I"C:\push API\firmware\include" -I"C:\push API\firmware\lib" -I"C:\Program Files (x86)\Microchip\xc16\v1.11" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360930230/wifly_drv.o.d" $(SILENT) 
	
${OBJECTDIR}/_ext/1360930230/adc.o: ../lib/adc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360930230 
	@${RM} ${OBJECTDIR}/_ext/1360930230/adc.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../lib/adc.c  -o ${OBJECTDIR}/_ext/1360930230/adc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1360930230/adc.o.d"    -g -omf=elf -DAPP_RUN -O0 -I"..\include" -I"C:\push API\firmware" -I"C:\push API\firmware\include" -I"C:\push API\firmware\lib" -I"C:\Program Files (x86)\Microchip\xc16\v1.11" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360930230/adc.o.d" $(SILENT) 
	
${OBJECTDIR}/_ext/1360930230/buttons.o: ../lib/buttons.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360930230 
	@${RM} ${OBJECTDIR}/_ext/1360930230/buttons.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../lib/buttons.c  -o ${OBJECTDIR}/_ext/1360930230/buttons.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1360930230/buttons.o.d"    -g -omf=elf -DAPP_RUN -O0 -I"..\include" -I"C:\push API\firmware" -I"C:\push API\firmware\include" -I"C:\push API\firmware\lib" -I"C:\Program Files (x86)\Microchip\xc16\v1.11" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360930230/buttons.o.d" $(SILENT) 
	
${OBJECTDIR}/_ext/1472/main.o: ../main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../main.c  -o ${OBJECTDIR}/_ext/1472/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/main.o.d"    -g -omf=elf -DAPP_RUN -O0 -I"..\include" -I"C:\push API\firmware" -I"C:\push API\firmware\include" -I"C:\push API\firmware\lib" -I"C:\Program Files (x86)\Microchip\xc16\v1.11" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/main.o.d" $(SILENT) 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemblePreproc
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/wifly_pass_thru_demo_exp16_p24.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/wifly_pass_thru_demo_exp16_p24.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)    -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -DAPP_RUN -Wl,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__ICD2RAM=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_ICD3=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,--report-mem$(MP_EXTRA_LD_POST) 
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/wifly_pass_thru_demo_exp16_p24.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/wifly_pass_thru_demo_exp16_p24.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)    -omf=elf -DAPP_RUN -Wl,--defsym=__MPLAB_BUILD=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,--report-mem$(MP_EXTRA_LD_POST) 
	${MP_CC_DIR}\\xc16-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/wifly_pass_thru_demo_exp16_p24.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} -a  -omf=elf 
	
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
