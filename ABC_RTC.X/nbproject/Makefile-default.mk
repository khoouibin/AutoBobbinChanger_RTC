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
SOURCEFILES_QUOTED_IF_SPACED=IO_Entity/IO_Control.c IO_Entity/IO_Entity_Mgr.c RTC_Control/RTC_Control.c RTC_Profile/RTC_Profile.c RTC_Log/RTC_Log.c Timers/Timers.c usb/usb_descriptors.c usb/usb_device.c usb/usb_device_hid.c usb/usb_events.c usb/usb_hal_16bit.c usb_app/usb_app.c main.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/IO_Entity/IO_Control.o ${OBJECTDIR}/IO_Entity/IO_Entity_Mgr.o ${OBJECTDIR}/RTC_Control/RTC_Control.o ${OBJECTDIR}/RTC_Profile/RTC_Profile.o ${OBJECTDIR}/RTC_Log/RTC_Log.o ${OBJECTDIR}/Timers/Timers.o ${OBJECTDIR}/usb/usb_descriptors.o ${OBJECTDIR}/usb/usb_device.o ${OBJECTDIR}/usb/usb_device_hid.o ${OBJECTDIR}/usb/usb_events.o ${OBJECTDIR}/usb/usb_hal_16bit.o ${OBJECTDIR}/usb_app/usb_app.o ${OBJECTDIR}/main.o
POSSIBLE_DEPFILES=${OBJECTDIR}/IO_Entity/IO_Control.o.d ${OBJECTDIR}/IO_Entity/IO_Entity_Mgr.o.d ${OBJECTDIR}/RTC_Control/RTC_Control.o.d ${OBJECTDIR}/RTC_Profile/RTC_Profile.o.d ${OBJECTDIR}/RTC_Log/RTC_Log.o.d ${OBJECTDIR}/Timers/Timers.o.d ${OBJECTDIR}/usb/usb_descriptors.o.d ${OBJECTDIR}/usb/usb_device.o.d ${OBJECTDIR}/usb/usb_device_hid.o.d ${OBJECTDIR}/usb/usb_events.o.d ${OBJECTDIR}/usb/usb_hal_16bit.o.d ${OBJECTDIR}/usb_app/usb_app.o.d ${OBJECTDIR}/main.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/IO_Entity/IO_Control.o ${OBJECTDIR}/IO_Entity/IO_Entity_Mgr.o ${OBJECTDIR}/RTC_Control/RTC_Control.o ${OBJECTDIR}/RTC_Profile/RTC_Profile.o ${OBJECTDIR}/RTC_Log/RTC_Log.o ${OBJECTDIR}/Timers/Timers.o ${OBJECTDIR}/usb/usb_descriptors.o ${OBJECTDIR}/usb/usb_device.o ${OBJECTDIR}/usb/usb_device_hid.o ${OBJECTDIR}/usb/usb_events.o ${OBJECTDIR}/usb/usb_hal_16bit.o ${OBJECTDIR}/usb_app/usb_app.o ${OBJECTDIR}/main.o

# Source Files
SOURCEFILES=IO_Entity/IO_Control.c IO_Entity/IO_Entity_Mgr.c RTC_Control/RTC_Control.c RTC_Profile/RTC_Profile.c RTC_Log/RTC_Log.c Timers/Timers.c usb/usb_descriptors.c usb/usb_device.c usb/usb_device_hid.c usb/usb_events.c usb/usb_hal_16bit.c usb_app/usb_app.c main.c



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
${OBJECTDIR}/IO_Entity/IO_Control.o: IO_Entity/IO_Control.c  .generated_files/flags/default/d668cf9f0f537d4bf77e93b8eaf9b08fd93d7b98 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/IO_Entity" 
	@${RM} ${OBJECTDIR}/IO_Entity/IO_Control.o.d 
	@${RM} ${OBJECTDIR}/IO_Entity/IO_Control.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  IO_Entity/IO_Control.c  -o ${OBJECTDIR}/IO_Entity/IO_Control.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/IO_Entity/IO_Control.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"Common" -I"IO_Entity" -I"Timers" -I"RTC_Control" -I"usb_app" -I"usb" -I"RTC_Profile" -I"RTC_Log" -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/IO_Entity/IO_Entity_Mgr.o: IO_Entity/IO_Entity_Mgr.c  .generated_files/flags/default/fef9f9313114fbf215a03071861150b463c05d57 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/IO_Entity" 
	@${RM} ${OBJECTDIR}/IO_Entity/IO_Entity_Mgr.o.d 
	@${RM} ${OBJECTDIR}/IO_Entity/IO_Entity_Mgr.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  IO_Entity/IO_Entity_Mgr.c  -o ${OBJECTDIR}/IO_Entity/IO_Entity_Mgr.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/IO_Entity/IO_Entity_Mgr.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"Common" -I"IO_Entity" -I"Timers" -I"RTC_Control" -I"usb_app" -I"usb" -I"RTC_Profile" -I"RTC_Log" -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/RTC_Control/RTC_Control.o: RTC_Control/RTC_Control.c  .generated_files/flags/default/8961a1d50e532ced0c8b5bf04b3792bdd2c55d59 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/RTC_Control" 
	@${RM} ${OBJECTDIR}/RTC_Control/RTC_Control.o.d 
	@${RM} ${OBJECTDIR}/RTC_Control/RTC_Control.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  RTC_Control/RTC_Control.c  -o ${OBJECTDIR}/RTC_Control/RTC_Control.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/RTC_Control/RTC_Control.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"Common" -I"IO_Entity" -I"Timers" -I"RTC_Control" -I"usb_app" -I"usb" -I"RTC_Profile" -I"RTC_Log" -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/RTC_Profile/RTC_Profile.o: RTC_Profile/RTC_Profile.c  .generated_files/flags/default/dd0aad1481de115eab783bef4da3868ae27be47f .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/RTC_Profile" 
	@${RM} ${OBJECTDIR}/RTC_Profile/RTC_Profile.o.d 
	@${RM} ${OBJECTDIR}/RTC_Profile/RTC_Profile.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  RTC_Profile/RTC_Profile.c  -o ${OBJECTDIR}/RTC_Profile/RTC_Profile.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/RTC_Profile/RTC_Profile.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"Common" -I"IO_Entity" -I"Timers" -I"RTC_Control" -I"usb_app" -I"usb" -I"RTC_Profile" -I"RTC_Log" -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/RTC_Log/RTC_Log.o: RTC_Log/RTC_Log.c  .generated_files/flags/default/10f4dd5427e13dae5dc6226b0948ab428fe4d10c .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/RTC_Log" 
	@${RM} ${OBJECTDIR}/RTC_Log/RTC_Log.o.d 
	@${RM} ${OBJECTDIR}/RTC_Log/RTC_Log.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  RTC_Log/RTC_Log.c  -o ${OBJECTDIR}/RTC_Log/RTC_Log.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/RTC_Log/RTC_Log.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"Common" -I"IO_Entity" -I"Timers" -I"RTC_Control" -I"usb_app" -I"usb" -I"RTC_Profile" -I"RTC_Log" -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Timers/Timers.o: Timers/Timers.c  .generated_files/flags/default/a892e1e5f41ededf95c99859effe14b44c9825c0 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/Timers" 
	@${RM} ${OBJECTDIR}/Timers/Timers.o.d 
	@${RM} ${OBJECTDIR}/Timers/Timers.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Timers/Timers.c  -o ${OBJECTDIR}/Timers/Timers.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Timers/Timers.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"Common" -I"IO_Entity" -I"Timers" -I"RTC_Control" -I"usb_app" -I"usb" -I"RTC_Profile" -I"RTC_Log" -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/usb/usb_descriptors.o: usb/usb_descriptors.c  .generated_files/flags/default/1edc3e01915abef355356b3a7f15e6e90a256c7a .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/usb" 
	@${RM} ${OBJECTDIR}/usb/usb_descriptors.o.d 
	@${RM} ${OBJECTDIR}/usb/usb_descriptors.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  usb/usb_descriptors.c  -o ${OBJECTDIR}/usb/usb_descriptors.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/usb/usb_descriptors.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"Common" -I"IO_Entity" -I"Timers" -I"RTC_Control" -I"usb_app" -I"usb" -I"RTC_Profile" -I"RTC_Log" -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/usb/usb_device.o: usb/usb_device.c  .generated_files/flags/default/75ad5d8e5e75221c550baf6fb10cf6d016c40374 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/usb" 
	@${RM} ${OBJECTDIR}/usb/usb_device.o.d 
	@${RM} ${OBJECTDIR}/usb/usb_device.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  usb/usb_device.c  -o ${OBJECTDIR}/usb/usb_device.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/usb/usb_device.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"Common" -I"IO_Entity" -I"Timers" -I"RTC_Control" -I"usb_app" -I"usb" -I"RTC_Profile" -I"RTC_Log" -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/usb/usb_device_hid.o: usb/usb_device_hid.c  .generated_files/flags/default/f3978993e58fbe08c50221fbc518df01fd485b41 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/usb" 
	@${RM} ${OBJECTDIR}/usb/usb_device_hid.o.d 
	@${RM} ${OBJECTDIR}/usb/usb_device_hid.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  usb/usb_device_hid.c  -o ${OBJECTDIR}/usb/usb_device_hid.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/usb/usb_device_hid.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"Common" -I"IO_Entity" -I"Timers" -I"RTC_Control" -I"usb_app" -I"usb" -I"RTC_Profile" -I"RTC_Log" -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/usb/usb_events.o: usb/usb_events.c  .generated_files/flags/default/a5ea626e082c354915ad2c56e29e3c8e8440fa67 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/usb" 
	@${RM} ${OBJECTDIR}/usb/usb_events.o.d 
	@${RM} ${OBJECTDIR}/usb/usb_events.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  usb/usb_events.c  -o ${OBJECTDIR}/usb/usb_events.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/usb/usb_events.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"Common" -I"IO_Entity" -I"Timers" -I"RTC_Control" -I"usb_app" -I"usb" -I"RTC_Profile" -I"RTC_Log" -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/usb/usb_hal_16bit.o: usb/usb_hal_16bit.c  .generated_files/flags/default/913acf58c18db88d182d73c1530daea0537df8b8 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/usb" 
	@${RM} ${OBJECTDIR}/usb/usb_hal_16bit.o.d 
	@${RM} ${OBJECTDIR}/usb/usb_hal_16bit.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  usb/usb_hal_16bit.c  -o ${OBJECTDIR}/usb/usb_hal_16bit.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/usb/usb_hal_16bit.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"Common" -I"IO_Entity" -I"Timers" -I"RTC_Control" -I"usb_app" -I"usb" -I"RTC_Profile" -I"RTC_Log" -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/usb_app/usb_app.o: usb_app/usb_app.c  .generated_files/flags/default/efb4541b9df95c661bbee4d179215327d0d468f8 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/usb_app" 
	@${RM} ${OBJECTDIR}/usb_app/usb_app.o.d 
	@${RM} ${OBJECTDIR}/usb_app/usb_app.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  usb_app/usb_app.c  -o ${OBJECTDIR}/usb_app/usb_app.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/usb_app/usb_app.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"Common" -I"IO_Entity" -I"Timers" -I"RTC_Control" -I"usb_app" -I"usb" -I"RTC_Profile" -I"RTC_Log" -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/main.o: main.c  .generated_files/flags/default/977096e45ed29020ad510463b2c899128af306a7 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  main.c  -o ${OBJECTDIR}/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/main.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"Common" -I"IO_Entity" -I"Timers" -I"RTC_Control" -I"usb_app" -I"usb" -I"RTC_Profile" -I"RTC_Log" -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
else
${OBJECTDIR}/IO_Entity/IO_Control.o: IO_Entity/IO_Control.c  .generated_files/flags/default/ebc15d7bdf73776a8fd4609889330792ae17a277 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/IO_Entity" 
	@${RM} ${OBJECTDIR}/IO_Entity/IO_Control.o.d 
	@${RM} ${OBJECTDIR}/IO_Entity/IO_Control.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  IO_Entity/IO_Control.c  -o ${OBJECTDIR}/IO_Entity/IO_Control.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/IO_Entity/IO_Control.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"Common" -I"IO_Entity" -I"Timers" -I"RTC_Control" -I"usb_app" -I"usb" -I"RTC_Profile" -I"RTC_Log" -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/IO_Entity/IO_Entity_Mgr.o: IO_Entity/IO_Entity_Mgr.c  .generated_files/flags/default/455bc80af2b79b58103d7c613b6384356d0b47ed .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/IO_Entity" 
	@${RM} ${OBJECTDIR}/IO_Entity/IO_Entity_Mgr.o.d 
	@${RM} ${OBJECTDIR}/IO_Entity/IO_Entity_Mgr.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  IO_Entity/IO_Entity_Mgr.c  -o ${OBJECTDIR}/IO_Entity/IO_Entity_Mgr.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/IO_Entity/IO_Entity_Mgr.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"Common" -I"IO_Entity" -I"Timers" -I"RTC_Control" -I"usb_app" -I"usb" -I"RTC_Profile" -I"RTC_Log" -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/RTC_Control/RTC_Control.o: RTC_Control/RTC_Control.c  .generated_files/flags/default/192e8d28e81baa82c0d8fdbb8a71011521942212 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/RTC_Control" 
	@${RM} ${OBJECTDIR}/RTC_Control/RTC_Control.o.d 
	@${RM} ${OBJECTDIR}/RTC_Control/RTC_Control.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  RTC_Control/RTC_Control.c  -o ${OBJECTDIR}/RTC_Control/RTC_Control.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/RTC_Control/RTC_Control.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"Common" -I"IO_Entity" -I"Timers" -I"RTC_Control" -I"usb_app" -I"usb" -I"RTC_Profile" -I"RTC_Log" -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/RTC_Profile/RTC_Profile.o: RTC_Profile/RTC_Profile.c  .generated_files/flags/default/f6cb39b5309aefc45e87474dee010f516115766 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/RTC_Profile" 
	@${RM} ${OBJECTDIR}/RTC_Profile/RTC_Profile.o.d 
	@${RM} ${OBJECTDIR}/RTC_Profile/RTC_Profile.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  RTC_Profile/RTC_Profile.c  -o ${OBJECTDIR}/RTC_Profile/RTC_Profile.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/RTC_Profile/RTC_Profile.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"Common" -I"IO_Entity" -I"Timers" -I"RTC_Control" -I"usb_app" -I"usb" -I"RTC_Profile" -I"RTC_Log" -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/RTC_Log/RTC_Log.o: RTC_Log/RTC_Log.c  .generated_files/flags/default/ae89236b319fdb7896402a2130938f1cce4a673 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/RTC_Log" 
	@${RM} ${OBJECTDIR}/RTC_Log/RTC_Log.o.d 
	@${RM} ${OBJECTDIR}/RTC_Log/RTC_Log.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  RTC_Log/RTC_Log.c  -o ${OBJECTDIR}/RTC_Log/RTC_Log.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/RTC_Log/RTC_Log.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"Common" -I"IO_Entity" -I"Timers" -I"RTC_Control" -I"usb_app" -I"usb" -I"RTC_Profile" -I"RTC_Log" -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Timers/Timers.o: Timers/Timers.c  .generated_files/flags/default/e7374c372b7674fc2e094e8abd63d80940dc9880 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/Timers" 
	@${RM} ${OBJECTDIR}/Timers/Timers.o.d 
	@${RM} ${OBJECTDIR}/Timers/Timers.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Timers/Timers.c  -o ${OBJECTDIR}/Timers/Timers.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Timers/Timers.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"Common" -I"IO_Entity" -I"Timers" -I"RTC_Control" -I"usb_app" -I"usb" -I"RTC_Profile" -I"RTC_Log" -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/usb/usb_descriptors.o: usb/usb_descriptors.c  .generated_files/flags/default/3f49b6412be046d47194d99a95f6df9160f0eae .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/usb" 
	@${RM} ${OBJECTDIR}/usb/usb_descriptors.o.d 
	@${RM} ${OBJECTDIR}/usb/usb_descriptors.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  usb/usb_descriptors.c  -o ${OBJECTDIR}/usb/usb_descriptors.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/usb/usb_descriptors.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"Common" -I"IO_Entity" -I"Timers" -I"RTC_Control" -I"usb_app" -I"usb" -I"RTC_Profile" -I"RTC_Log" -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/usb/usb_device.o: usb/usb_device.c  .generated_files/flags/default/6dc146417f9c185a531cba444e0b4c59f93aa21b .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/usb" 
	@${RM} ${OBJECTDIR}/usb/usb_device.o.d 
	@${RM} ${OBJECTDIR}/usb/usb_device.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  usb/usb_device.c  -o ${OBJECTDIR}/usb/usb_device.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/usb/usb_device.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"Common" -I"IO_Entity" -I"Timers" -I"RTC_Control" -I"usb_app" -I"usb" -I"RTC_Profile" -I"RTC_Log" -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/usb/usb_device_hid.o: usb/usb_device_hid.c  .generated_files/flags/default/ec117523a6d525f8906ddeff3374de906c4fd32b .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/usb" 
	@${RM} ${OBJECTDIR}/usb/usb_device_hid.o.d 
	@${RM} ${OBJECTDIR}/usb/usb_device_hid.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  usb/usb_device_hid.c  -o ${OBJECTDIR}/usb/usb_device_hid.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/usb/usb_device_hid.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"Common" -I"IO_Entity" -I"Timers" -I"RTC_Control" -I"usb_app" -I"usb" -I"RTC_Profile" -I"RTC_Log" -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/usb/usb_events.o: usb/usb_events.c  .generated_files/flags/default/43c6fd6ee5ff06709a1aeee8624dbdc2ab218400 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/usb" 
	@${RM} ${OBJECTDIR}/usb/usb_events.o.d 
	@${RM} ${OBJECTDIR}/usb/usb_events.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  usb/usb_events.c  -o ${OBJECTDIR}/usb/usb_events.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/usb/usb_events.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"Common" -I"IO_Entity" -I"Timers" -I"RTC_Control" -I"usb_app" -I"usb" -I"RTC_Profile" -I"RTC_Log" -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/usb/usb_hal_16bit.o: usb/usb_hal_16bit.c  .generated_files/flags/default/d589c1f0f9bcfb9a076f3fc4d4678a75f093afc5 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/usb" 
	@${RM} ${OBJECTDIR}/usb/usb_hal_16bit.o.d 
	@${RM} ${OBJECTDIR}/usb/usb_hal_16bit.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  usb/usb_hal_16bit.c  -o ${OBJECTDIR}/usb/usb_hal_16bit.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/usb/usb_hal_16bit.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"Common" -I"IO_Entity" -I"Timers" -I"RTC_Control" -I"usb_app" -I"usb" -I"RTC_Profile" -I"RTC_Log" -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/usb_app/usb_app.o: usb_app/usb_app.c  .generated_files/flags/default/f9888be08a7592cbc394dbc72c4f03a692aa7083 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/usb_app" 
	@${RM} ${OBJECTDIR}/usb_app/usb_app.o.d 
	@${RM} ${OBJECTDIR}/usb_app/usb_app.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  usb_app/usb_app.c  -o ${OBJECTDIR}/usb_app/usb_app.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/usb_app/usb_app.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"Common" -I"IO_Entity" -I"Timers" -I"RTC_Control" -I"usb_app" -I"usb" -I"RTC_Profile" -I"RTC_Log" -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/main.o: main.c  .generated_files/flags/default/7107e60c2b57c88eba40c464a8b410e83c13655c .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  main.c  -o ${OBJECTDIR}/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/main.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"Common" -I"IO_Entity" -I"Timers" -I"RTC_Control" -I"usb_app" -I"usb" -I"RTC_Profile" -I"RTC_Log" -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
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
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o ${DISTDIR}/ABC_RTC.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -D__DEBUG=__DEBUG   -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"Common" -I"IO_Entity" -I"Timers" -I"RTC_Control" -I"usb_app" -I"usb" -I"RTC_Profile" -I"RTC_Log"  -mreserve=data@0x1000:0x101B -mreserve=data@0x101C:0x101D -mreserve=data@0x101E:0x101F -mreserve=data@0x1020:0x1021 -mreserve=data@0x1022:0x1023 -mreserve=data@0x1024:0x1027 -mreserve=data@0x1028:0x104F   -Wl,--local-stack,,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D__DEBUG=__DEBUG,,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,${DISTDIR}/memoryfile.xml$(MP_EXTRA_LD_POST)  -mdfp="${DFP_DIR}/xc16" 
	
else
${DISTDIR}/ABC_RTC.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   p33EP512MU814_ABC.gld
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o ${DISTDIR}/ABC_RTC.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"Common" -I"IO_Entity" -I"Timers" -I"RTC_Control" -I"usb_app" -I"usb" -I"RTC_Profile" -I"RTC_Log" -Wl,--local-stack,,--defsym=__MPLAB_BUILD=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,${DISTDIR}/memoryfile.xml$(MP_EXTRA_LD_POST)  -mdfp="${DFP_DIR}/xc16" 
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
