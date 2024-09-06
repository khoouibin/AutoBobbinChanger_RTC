#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=mkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/ABC_RTC.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/ABC_RTC.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
else
COMPARISON_BUILD=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=IO_Entity/IO_Control.c IO_Entity/IO_Entity_Mgr.c RTC_Control/RTC_Control.c Timers/Timers.c usb/usb_descriptors.c usb/usb_device.c usb/usb_device_hid.c usb/usb_events.c usb/usb_hal_16bit.c usb_app/usb_app.c main.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/IO_Entity/IO_Control.o ${OBJECTDIR}/IO_Entity/IO_Entity_Mgr.o ${OBJECTDIR}/RTC_Control/RTC_Control.o ${OBJECTDIR}/Timers/Timers.o ${OBJECTDIR}/usb/usb_descriptors.o ${OBJECTDIR}/usb/usb_device.o ${OBJECTDIR}/usb/usb_device_hid.o ${OBJECTDIR}/usb/usb_events.o ${OBJECTDIR}/usb/usb_hal_16bit.o ${OBJECTDIR}/usb_app/usb_app.o ${OBJECTDIR}/main.o
POSSIBLE_DEPFILES=${OBJECTDIR}/IO_Entity/IO_Control.o.d ${OBJECTDIR}/IO_Entity/IO_Entity_Mgr.o.d ${OBJECTDIR}/RTC_Control/RTC_Control.o.d ${OBJECTDIR}/Timers/Timers.o.d ${OBJECTDIR}/usb/usb_descriptors.o.d ${OBJECTDIR}/usb/usb_device.o.d ${OBJECTDIR}/usb/usb_device_hid.o.d ${OBJECTDIR}/usb/usb_events.o.d ${OBJECTDIR}/usb/usb_hal_16bit.o.d ${OBJECTDIR}/usb_app/usb_app.o.d ${OBJECTDIR}/main.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/IO_Entity/IO_Control.o ${OBJECTDIR}/IO_Entity/IO_Entity_Mgr.o ${OBJECTDIR}/RTC_Control/RTC_Control.o ${OBJECTDIR}/Timers/Timers.o ${OBJECTDIR}/usb/usb_descriptors.o ${OBJECTDIR}/usb/usb_device.o ${OBJECTDIR}/usb/usb_device_hid.o ${OBJECTDIR}/usb/usb_events.o ${OBJECTDIR}/usb/usb_hal_16bit.o ${OBJECTDIR}/usb_app/usb_app.o ${OBJECTDIR}/main.o

# Source Files
SOURCEFILES=IO_Entity/IO_Control.c IO_Entity/IO_Entity_Mgr.c RTC_Control/RTC_Control.c Timers/Timers.c usb/usb_descriptors.c usb/usb_device.c usb/usb_device_hid.c usb/usb_events.c usb/usb_hal_16bit.c usb_app/usb_app.c main.c



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
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk ${DISTDIR}/ABC_RTC.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=33EP512MU814
MP_LINKER_FILE_OPTION=,--script="p33EP512MU814_ABC.gld"
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/IO_Entity/IO_Control.o: IO_Entity/IO_Control.c  .generated_files/flags/default/1426c90cd8c490a081e44f78b062efa26ab22555 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/IO_Entity" 
	@${RM} ${OBJECTDIR}/IO_Entity/IO_Control.o.d 
	@${RM} ${OBJECTDIR}/IO_Entity/IO_Control.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  IO_Entity/IO_Control.c  -o ${OBJECTDIR}/IO_Entity/IO_Control.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/IO_Entity/IO_Control.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"Common" -I"IO_Entity" -I"Timers" -I"RTC_Control" -I"usb_app" -I"usb" -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/IO_Entity/IO_Entity_Mgr.o: IO_Entity/IO_Entity_Mgr.c  .generated_files/flags/default/af2a5bec5144641e09496e9b2401470465d99932 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/IO_Entity" 
	@${RM} ${OBJECTDIR}/IO_Entity/IO_Entity_Mgr.o.d 
	@${RM} ${OBJECTDIR}/IO_Entity/IO_Entity_Mgr.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  IO_Entity/IO_Entity_Mgr.c  -o ${OBJECTDIR}/IO_Entity/IO_Entity_Mgr.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/IO_Entity/IO_Entity_Mgr.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"Common" -I"IO_Entity" -I"Timers" -I"RTC_Control" -I"usb_app" -I"usb" -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/RTC_Control/RTC_Control.o: RTC_Control/RTC_Control.c  .generated_files/flags/default/6daaa6278e525c8ff4586817bca67994f32d1693 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/RTC_Control" 
	@${RM} ${OBJECTDIR}/RTC_Control/RTC_Control.o.d 
	@${RM} ${OBJECTDIR}/RTC_Control/RTC_Control.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  RTC_Control/RTC_Control.c  -o ${OBJECTDIR}/RTC_Control/RTC_Control.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/RTC_Control/RTC_Control.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"Common" -I"IO_Entity" -I"Timers" -I"RTC_Control" -I"usb_app" -I"usb" -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Timers/Timers.o: Timers/Timers.c  .generated_files/flags/default/d98ef619e86186b6b60dc88200765a3094cf49a8 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/Timers" 
	@${RM} ${OBJECTDIR}/Timers/Timers.o.d 
	@${RM} ${OBJECTDIR}/Timers/Timers.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Timers/Timers.c  -o ${OBJECTDIR}/Timers/Timers.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Timers/Timers.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"Common" -I"IO_Entity" -I"Timers" -I"RTC_Control" -I"usb_app" -I"usb" -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/usb/usb_descriptors.o: usb/usb_descriptors.c  .generated_files/flags/default/53f8c12fdb0630760458401c064333cd4e709f7b .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/usb" 
	@${RM} ${OBJECTDIR}/usb/usb_descriptors.o.d 
	@${RM} ${OBJECTDIR}/usb/usb_descriptors.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  usb/usb_descriptors.c  -o ${OBJECTDIR}/usb/usb_descriptors.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/usb/usb_descriptors.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"Common" -I"IO_Entity" -I"Timers" -I"RTC_Control" -I"usb_app" -I"usb" -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/usb/usb_device.o: usb/usb_device.c  .generated_files/flags/default/db1b19c2bdb7ec4f73a35e67ca56ec841ee87d3c .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/usb" 
	@${RM} ${OBJECTDIR}/usb/usb_device.o.d 
	@${RM} ${OBJECTDIR}/usb/usb_device.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  usb/usb_device.c  -o ${OBJECTDIR}/usb/usb_device.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/usb/usb_device.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"Common" -I"IO_Entity" -I"Timers" -I"RTC_Control" -I"usb_app" -I"usb" -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/usb/usb_device_hid.o: usb/usb_device_hid.c  .generated_files/flags/default/d3a99d94974705e80d2ce13c2d66e8d33c29f8bc .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/usb" 
	@${RM} ${OBJECTDIR}/usb/usb_device_hid.o.d 
	@${RM} ${OBJECTDIR}/usb/usb_device_hid.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  usb/usb_device_hid.c  -o ${OBJECTDIR}/usb/usb_device_hid.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/usb/usb_device_hid.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"Common" -I"IO_Entity" -I"Timers" -I"RTC_Control" -I"usb_app" -I"usb" -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/usb/usb_events.o: usb/usb_events.c  .generated_files/flags/default/1c56ac97ebb890bbf8f2f1feeb8dcda86afd96b8 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/usb" 
	@${RM} ${OBJECTDIR}/usb/usb_events.o.d 
	@${RM} ${OBJECTDIR}/usb/usb_events.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  usb/usb_events.c  -o ${OBJECTDIR}/usb/usb_events.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/usb/usb_events.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"Common" -I"IO_Entity" -I"Timers" -I"RTC_Control" -I"usb_app" -I"usb" -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/usb/usb_hal_16bit.o: usb/usb_hal_16bit.c  .generated_files/flags/default/82e015e56587c320e785afcebeaa03eb30542846 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/usb" 
	@${RM} ${OBJECTDIR}/usb/usb_hal_16bit.o.d 
	@${RM} ${OBJECTDIR}/usb/usb_hal_16bit.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  usb/usb_hal_16bit.c  -o ${OBJECTDIR}/usb/usb_hal_16bit.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/usb/usb_hal_16bit.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"Common" -I"IO_Entity" -I"Timers" -I"RTC_Control" -I"usb_app" -I"usb" -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/usb_app/usb_app.o: usb_app/usb_app.c  .generated_files/flags/default/9672ddf59a1ef737989d261f23d1baf11fb9fcce .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/usb_app" 
	@${RM} ${OBJECTDIR}/usb_app/usb_app.o.d 
	@${RM} ${OBJECTDIR}/usb_app/usb_app.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  usb_app/usb_app.c  -o ${OBJECTDIR}/usb_app/usb_app.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/usb_app/usb_app.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"Common" -I"IO_Entity" -I"Timers" -I"RTC_Control" -I"usb_app" -I"usb" -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/main.o: main.c  .generated_files/flags/default/3803b991c39a95672ac16d3d1c6c4ab14f0147ce .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  main.c  -o ${OBJECTDIR}/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/main.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"Common" -I"IO_Entity" -I"Timers" -I"RTC_Control" -I"usb_app" -I"usb" -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
else
${OBJECTDIR}/IO_Entity/IO_Control.o: IO_Entity/IO_Control.c  .generated_files/flags/default/b77133c9ff5ab03fd5e96bc230386221598293c .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/IO_Entity" 
	@${RM} ${OBJECTDIR}/IO_Entity/IO_Control.o.d 
	@${RM} ${OBJECTDIR}/IO_Entity/IO_Control.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  IO_Entity/IO_Control.c  -o ${OBJECTDIR}/IO_Entity/IO_Control.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/IO_Entity/IO_Control.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"Common" -I"IO_Entity" -I"Timers" -I"RTC_Control" -I"usb_app" -I"usb" -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/IO_Entity/IO_Entity_Mgr.o: IO_Entity/IO_Entity_Mgr.c  .generated_files/flags/default/58eaac308fd817e9984dd7ba03f9378d1acb6f22 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/IO_Entity" 
	@${RM} ${OBJECTDIR}/IO_Entity/IO_Entity_Mgr.o.d 
	@${RM} ${OBJECTDIR}/IO_Entity/IO_Entity_Mgr.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  IO_Entity/IO_Entity_Mgr.c  -o ${OBJECTDIR}/IO_Entity/IO_Entity_Mgr.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/IO_Entity/IO_Entity_Mgr.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"Common" -I"IO_Entity" -I"Timers" -I"RTC_Control" -I"usb_app" -I"usb" -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/RTC_Control/RTC_Control.o: RTC_Control/RTC_Control.c  .generated_files/flags/default/960b51be5c92f09753a32c029c5783f7d7a50f9 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/RTC_Control" 
	@${RM} ${OBJECTDIR}/RTC_Control/RTC_Control.o.d 
	@${RM} ${OBJECTDIR}/RTC_Control/RTC_Control.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  RTC_Control/RTC_Control.c  -o ${OBJECTDIR}/RTC_Control/RTC_Control.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/RTC_Control/RTC_Control.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"Common" -I"IO_Entity" -I"Timers" -I"RTC_Control" -I"usb_app" -I"usb" -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Timers/Timers.o: Timers/Timers.c  .generated_files/flags/default/f122248fd1ab96d7a4d083608ed2f7e4d36057b .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/Timers" 
	@${RM} ${OBJECTDIR}/Timers/Timers.o.d 
	@${RM} ${OBJECTDIR}/Timers/Timers.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Timers/Timers.c  -o ${OBJECTDIR}/Timers/Timers.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Timers/Timers.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"Common" -I"IO_Entity" -I"Timers" -I"RTC_Control" -I"usb_app" -I"usb" -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/usb/usb_descriptors.o: usb/usb_descriptors.c  .generated_files/flags/default/e504c67e484794a897de2f97a177915e39218e9 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/usb" 
	@${RM} ${OBJECTDIR}/usb/usb_descriptors.o.d 
	@${RM} ${OBJECTDIR}/usb/usb_descriptors.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  usb/usb_descriptors.c  -o ${OBJECTDIR}/usb/usb_descriptors.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/usb/usb_descriptors.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"Common" -I"IO_Entity" -I"Timers" -I"RTC_Control" -I"usb_app" -I"usb" -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/usb/usb_device.o: usb/usb_device.c  .generated_files/flags/default/123e46b642fb5d0dfcbcccb6dbfc445539ef9b1d .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/usb" 
	@${RM} ${OBJECTDIR}/usb/usb_device.o.d 
	@${RM} ${OBJECTDIR}/usb/usb_device.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  usb/usb_device.c  -o ${OBJECTDIR}/usb/usb_device.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/usb/usb_device.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"Common" -I"IO_Entity" -I"Timers" -I"RTC_Control" -I"usb_app" -I"usb" -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/usb/usb_device_hid.o: usb/usb_device_hid.c  .generated_files/flags/default/39c10af29c94df38fc27c8b31170f9c2ca0bc479 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/usb" 
	@${RM} ${OBJECTDIR}/usb/usb_device_hid.o.d 
	@${RM} ${OBJECTDIR}/usb/usb_device_hid.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  usb/usb_device_hid.c  -o ${OBJECTDIR}/usb/usb_device_hid.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/usb/usb_device_hid.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"Common" -I"IO_Entity" -I"Timers" -I"RTC_Control" -I"usb_app" -I"usb" -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/usb/usb_events.o: usb/usb_events.c  .generated_files/flags/default/f97ef7d9fa8b5a611bee408c0ed3479bf4f3b9f3 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/usb" 
	@${RM} ${OBJECTDIR}/usb/usb_events.o.d 
	@${RM} ${OBJECTDIR}/usb/usb_events.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  usb/usb_events.c  -o ${OBJECTDIR}/usb/usb_events.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/usb/usb_events.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"Common" -I"IO_Entity" -I"Timers" -I"RTC_Control" -I"usb_app" -I"usb" -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/usb/usb_hal_16bit.o: usb/usb_hal_16bit.c  .generated_files/flags/default/8ca29e97318833d1dd61b9252cd84773d1138630 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/usb" 
	@${RM} ${OBJECTDIR}/usb/usb_hal_16bit.o.d 
	@${RM} ${OBJECTDIR}/usb/usb_hal_16bit.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  usb/usb_hal_16bit.c  -o ${OBJECTDIR}/usb/usb_hal_16bit.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/usb/usb_hal_16bit.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"Common" -I"IO_Entity" -I"Timers" -I"RTC_Control" -I"usb_app" -I"usb" -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/usb_app/usb_app.o: usb_app/usb_app.c  .generated_files/flags/default/cbd59b10b589ab03eca19430aa4eb411146e644e .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/usb_app" 
	@${RM} ${OBJECTDIR}/usb_app/usb_app.o.d 
	@${RM} ${OBJECTDIR}/usb_app/usb_app.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  usb_app/usb_app.c  -o ${OBJECTDIR}/usb_app/usb_app.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/usb_app/usb_app.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"Common" -I"IO_Entity" -I"Timers" -I"RTC_Control" -I"usb_app" -I"usb" -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/main.o: main.c  .generated_files/flags/default/60cce9b6f5ef4a95b3a45ad4e84328a307575d85 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  main.c  -o ${OBJECTDIR}/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/main.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"Common" -I"IO_Entity" -I"Timers" -I"RTC_Control" -I"usb_app" -I"usb" -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
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
${DISTDIR}/ABC_RTC.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    p33EP512MU814_ABC.gld
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o ${DISTDIR}/ABC_RTC.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -D__DEBUG=__DEBUG   -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"Common" -I"IO_Entity" -I"Timers" -I"RTC_Control" -I"usb_app" -I"usb"  -mreserve=data@0x1000:0x101B -mreserve=data@0x101C:0x101D -mreserve=data@0x101E:0x101F -mreserve=data@0x1020:0x1021 -mreserve=data@0x1022:0x1023 -mreserve=data@0x1024:0x1027 -mreserve=data@0x1028:0x104F   -Wl,--local-stack,,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D__DEBUG=__DEBUG,,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,${DISTDIR}/memoryfile.xml$(MP_EXTRA_LD_POST)  -mdfp="${DFP_DIR}/xc16" 
	
else
${DISTDIR}/ABC_RTC.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   p33EP512MU814_ABC.gld
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o ${DISTDIR}/ABC_RTC.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"Common" -I"IO_Entity" -I"Timers" -I"RTC_Control" -I"usb_app" -I"usb" -Wl,--local-stack,,--defsym=__MPLAB_BUILD=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,${DISTDIR}/memoryfile.xml$(MP_EXTRA_LD_POST)  -mdfp="${DFP_DIR}/xc16" 
	${MP_CC_DIR}/xc16-bin2hex ${DISTDIR}/ABC_RTC.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} -a  -omf=elf   -mdfp="${DFP_DIR}/xc16" 
	
	@echo Normalizing hex file
	@"/opt/microchip/mplabx/v6.15/mplab_platform/platform/../mplab_ide/modules/../../bin/hexmate" --edf="/opt/microchip/mplabx/v6.15/mplab_platform/platform/../mplab_ide/modules/../../dat/en_msgs.txt" ${DISTDIR}/ABC_RTC.X.${IMAGE_TYPE}.hex -o${DISTDIR}/ABC_RTC.X.${IMAGE_TYPE}.hex

endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${OBJECTDIR}
	${RM} -r ${DISTDIR}

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(wildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
