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
SOURCEFILES_QUOTED_IF_SPACED=IO_Entity/IO_Control.c IO_Entity/IO_Entity_Mgr.c RTC_Control/RTC_Control.c RTC_Profile/RTC_Profile.c RTC_Log/RTC_Log.c RTC_PulseControl/RTC_PulseControl.c Timers/Timers.c usb/usb_descriptors.c usb/usb_device.c usb/usb_device_hid.c usb/usb_events.c usb/usb_hal_16bit.c usb_app/usb_app.c main.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/IO_Entity/IO_Control.o ${OBJECTDIR}/IO_Entity/IO_Entity_Mgr.o ${OBJECTDIR}/RTC_Control/RTC_Control.o ${OBJECTDIR}/RTC_Profile/RTC_Profile.o ${OBJECTDIR}/RTC_Log/RTC_Log.o ${OBJECTDIR}/RTC_PulseControl/RTC_PulseControl.o ${OBJECTDIR}/Timers/Timers.o ${OBJECTDIR}/usb/usb_descriptors.o ${OBJECTDIR}/usb/usb_device.o ${OBJECTDIR}/usb/usb_device_hid.o ${OBJECTDIR}/usb/usb_events.o ${OBJECTDIR}/usb/usb_hal_16bit.o ${OBJECTDIR}/usb_app/usb_app.o ${OBJECTDIR}/main.o
POSSIBLE_DEPFILES=${OBJECTDIR}/IO_Entity/IO_Control.o.d ${OBJECTDIR}/IO_Entity/IO_Entity_Mgr.o.d ${OBJECTDIR}/RTC_Control/RTC_Control.o.d ${OBJECTDIR}/RTC_Profile/RTC_Profile.o.d ${OBJECTDIR}/RTC_Log/RTC_Log.o.d ${OBJECTDIR}/RTC_PulseControl/RTC_PulseControl.o.d ${OBJECTDIR}/Timers/Timers.o.d ${OBJECTDIR}/usb/usb_descriptors.o.d ${OBJECTDIR}/usb/usb_device.o.d ${OBJECTDIR}/usb/usb_device_hid.o.d ${OBJECTDIR}/usb/usb_events.o.d ${OBJECTDIR}/usb/usb_hal_16bit.o.d ${OBJECTDIR}/usb_app/usb_app.o.d ${OBJECTDIR}/main.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/IO_Entity/IO_Control.o ${OBJECTDIR}/IO_Entity/IO_Entity_Mgr.o ${OBJECTDIR}/RTC_Control/RTC_Control.o ${OBJECTDIR}/RTC_Profile/RTC_Profile.o ${OBJECTDIR}/RTC_Log/RTC_Log.o ${OBJECTDIR}/RTC_PulseControl/RTC_PulseControl.o ${OBJECTDIR}/Timers/Timers.o ${OBJECTDIR}/usb/usb_descriptors.o ${OBJECTDIR}/usb/usb_device.o ${OBJECTDIR}/usb/usb_device_hid.o ${OBJECTDIR}/usb/usb_events.o ${OBJECTDIR}/usb/usb_hal_16bit.o ${OBJECTDIR}/usb_app/usb_app.o ${OBJECTDIR}/main.o

# Source Files
SOURCEFILES=IO_Entity/IO_Control.c IO_Entity/IO_Entity_Mgr.c RTC_Control/RTC_Control.c RTC_Profile/RTC_Profile.c RTC_Log/RTC_Log.c RTC_PulseControl/RTC_PulseControl.c Timers/Timers.c usb/usb_descriptors.c usb/usb_device.c usb/usb_device_hid.c usb/usb_events.c usb/usb_hal_16bit.c usb_app/usb_app.c main.c



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
${OBJECTDIR}/IO_Entity/IO_Control.o: IO_Entity/IO_Control.c  .generated_files/flags/default/5aab21ed3c35461f70a7a9c2be65ef540d5776fc .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/IO_Entity" 
	@${RM} ${OBJECTDIR}/IO_Entity/IO_Control.o.d 
	@${RM} ${OBJECTDIR}/IO_Entity/IO_Control.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  IO_Entity/IO_Control.c  -o ${OBJECTDIR}/IO_Entity/IO_Control.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/IO_Entity/IO_Control.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"Common" -I"IO_Entity" -I"Timers" -I"RTC_Control" -I"usb_app" -I"usb" -I"RTC_Profile" -I"RTC_Log" -I"RTC_PulseControl" -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/IO_Entity/IO_Entity_Mgr.o: IO_Entity/IO_Entity_Mgr.c  .generated_files/flags/default/a862a172925dc9c115146ec3f2683c90d76fd298 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/IO_Entity" 
	@${RM} ${OBJECTDIR}/IO_Entity/IO_Entity_Mgr.o.d 
	@${RM} ${OBJECTDIR}/IO_Entity/IO_Entity_Mgr.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  IO_Entity/IO_Entity_Mgr.c  -o ${OBJECTDIR}/IO_Entity/IO_Entity_Mgr.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/IO_Entity/IO_Entity_Mgr.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"Common" -I"IO_Entity" -I"Timers" -I"RTC_Control" -I"usb_app" -I"usb" -I"RTC_Profile" -I"RTC_Log" -I"RTC_PulseControl" -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/RTC_Control/RTC_Control.o: RTC_Control/RTC_Control.c  .generated_files/flags/default/87586a878337c066415f237ec431d68f3b665a47 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/RTC_Control" 
	@${RM} ${OBJECTDIR}/RTC_Control/RTC_Control.o.d 
	@${RM} ${OBJECTDIR}/RTC_Control/RTC_Control.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  RTC_Control/RTC_Control.c  -o ${OBJECTDIR}/RTC_Control/RTC_Control.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/RTC_Control/RTC_Control.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"Common" -I"IO_Entity" -I"Timers" -I"RTC_Control" -I"usb_app" -I"usb" -I"RTC_Profile" -I"RTC_Log" -I"RTC_PulseControl" -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/RTC_Profile/RTC_Profile.o: RTC_Profile/RTC_Profile.c  .generated_files/flags/default/a07991144748cff68d11892e10b3a1056eec8385 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/RTC_Profile" 
	@${RM} ${OBJECTDIR}/RTC_Profile/RTC_Profile.o.d 
	@${RM} ${OBJECTDIR}/RTC_Profile/RTC_Profile.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  RTC_Profile/RTC_Profile.c  -o ${OBJECTDIR}/RTC_Profile/RTC_Profile.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/RTC_Profile/RTC_Profile.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"Common" -I"IO_Entity" -I"Timers" -I"RTC_Control" -I"usb_app" -I"usb" -I"RTC_Profile" -I"RTC_Log" -I"RTC_PulseControl" -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/RTC_Log/RTC_Log.o: RTC_Log/RTC_Log.c  .generated_files/flags/default/9c7aa3a191b8b79cf08b42b82366b166907a88d1 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/RTC_Log" 
	@${RM} ${OBJECTDIR}/RTC_Log/RTC_Log.o.d 
	@${RM} ${OBJECTDIR}/RTC_Log/RTC_Log.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  RTC_Log/RTC_Log.c  -o ${OBJECTDIR}/RTC_Log/RTC_Log.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/RTC_Log/RTC_Log.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"Common" -I"IO_Entity" -I"Timers" -I"RTC_Control" -I"usb_app" -I"usb" -I"RTC_Profile" -I"RTC_Log" -I"RTC_PulseControl" -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/RTC_PulseControl/RTC_PulseControl.o: RTC_PulseControl/RTC_PulseControl.c  .generated_files/flags/default/167804a646185d6ee6134767d2d2274c04bea4af .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/RTC_PulseControl" 
	@${RM} ${OBJECTDIR}/RTC_PulseControl/RTC_PulseControl.o.d 
	@${RM} ${OBJECTDIR}/RTC_PulseControl/RTC_PulseControl.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  RTC_PulseControl/RTC_PulseControl.c  -o ${OBJECTDIR}/RTC_PulseControl/RTC_PulseControl.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/RTC_PulseControl/RTC_PulseControl.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"Common" -I"IO_Entity" -I"Timers" -I"RTC_Control" -I"usb_app" -I"usb" -I"RTC_Profile" -I"RTC_Log" -I"RTC_PulseControl" -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Timers/Timers.o: Timers/Timers.c  .generated_files/flags/default/2ee3c4c29d63e9aa3b134c728e0cdb805b33f5d3 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/Timers" 
	@${RM} ${OBJECTDIR}/Timers/Timers.o.d 
	@${RM} ${OBJECTDIR}/Timers/Timers.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Timers/Timers.c  -o ${OBJECTDIR}/Timers/Timers.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Timers/Timers.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"Common" -I"IO_Entity" -I"Timers" -I"RTC_Control" -I"usb_app" -I"usb" -I"RTC_Profile" -I"RTC_Log" -I"RTC_PulseControl" -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/usb/usb_descriptors.o: usb/usb_descriptors.c  .generated_files/flags/default/8d8b562112186ae197cb7491ed0d2e2cfa00928b .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/usb" 
	@${RM} ${OBJECTDIR}/usb/usb_descriptors.o.d 
	@${RM} ${OBJECTDIR}/usb/usb_descriptors.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  usb/usb_descriptors.c  -o ${OBJECTDIR}/usb/usb_descriptors.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/usb/usb_descriptors.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"Common" -I"IO_Entity" -I"Timers" -I"RTC_Control" -I"usb_app" -I"usb" -I"RTC_Profile" -I"RTC_Log" -I"RTC_PulseControl" -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/usb/usb_device.o: usb/usb_device.c  .generated_files/flags/default/40424235fbe39454f0e5c009bf175ca6aa82d811 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/usb" 
	@${RM} ${OBJECTDIR}/usb/usb_device.o.d 
	@${RM} ${OBJECTDIR}/usb/usb_device.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  usb/usb_device.c  -o ${OBJECTDIR}/usb/usb_device.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/usb/usb_device.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"Common" -I"IO_Entity" -I"Timers" -I"RTC_Control" -I"usb_app" -I"usb" -I"RTC_Profile" -I"RTC_Log" -I"RTC_PulseControl" -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/usb/usb_device_hid.o: usb/usb_device_hid.c  .generated_files/flags/default/864147cfbff52e2f8220ed9b6d6b9993495ee146 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/usb" 
	@${RM} ${OBJECTDIR}/usb/usb_device_hid.o.d 
	@${RM} ${OBJECTDIR}/usb/usb_device_hid.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  usb/usb_device_hid.c  -o ${OBJECTDIR}/usb/usb_device_hid.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/usb/usb_device_hid.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"Common" -I"IO_Entity" -I"Timers" -I"RTC_Control" -I"usb_app" -I"usb" -I"RTC_Profile" -I"RTC_Log" -I"RTC_PulseControl" -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/usb/usb_events.o: usb/usb_events.c  .generated_files/flags/default/dcdff213a85facaaf2c782d397a7cd3e59e7239e .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/usb" 
	@${RM} ${OBJECTDIR}/usb/usb_events.o.d 
	@${RM} ${OBJECTDIR}/usb/usb_events.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  usb/usb_events.c  -o ${OBJECTDIR}/usb/usb_events.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/usb/usb_events.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"Common" -I"IO_Entity" -I"Timers" -I"RTC_Control" -I"usb_app" -I"usb" -I"RTC_Profile" -I"RTC_Log" -I"RTC_PulseControl" -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/usb/usb_hal_16bit.o: usb/usb_hal_16bit.c  .generated_files/flags/default/74fc007623c7b83c55224174925b710a845fc764 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/usb" 
	@${RM} ${OBJECTDIR}/usb/usb_hal_16bit.o.d 
	@${RM} ${OBJECTDIR}/usb/usb_hal_16bit.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  usb/usb_hal_16bit.c  -o ${OBJECTDIR}/usb/usb_hal_16bit.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/usb/usb_hal_16bit.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"Common" -I"IO_Entity" -I"Timers" -I"RTC_Control" -I"usb_app" -I"usb" -I"RTC_Profile" -I"RTC_Log" -I"RTC_PulseControl" -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/usb_app/usb_app.o: usb_app/usb_app.c  .generated_files/flags/default/5d90d8005de68e252d4e420c06bf53c33debf1b6 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/usb_app" 
	@${RM} ${OBJECTDIR}/usb_app/usb_app.o.d 
	@${RM} ${OBJECTDIR}/usb_app/usb_app.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  usb_app/usb_app.c  -o ${OBJECTDIR}/usb_app/usb_app.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/usb_app/usb_app.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"Common" -I"IO_Entity" -I"Timers" -I"RTC_Control" -I"usb_app" -I"usb" -I"RTC_Profile" -I"RTC_Log" -I"RTC_PulseControl" -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/main.o: main.c  .generated_files/flags/default/27f4f4a11b3941a67381293ef90f88e6566dda70 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  main.c  -o ${OBJECTDIR}/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/main.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"Common" -I"IO_Entity" -I"Timers" -I"RTC_Control" -I"usb_app" -I"usb" -I"RTC_Profile" -I"RTC_Log" -I"RTC_PulseControl" -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
else
${OBJECTDIR}/IO_Entity/IO_Control.o: IO_Entity/IO_Control.c  .generated_files/flags/default/b6fea8d767d131adc3026d0880f610192414b719 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/IO_Entity" 
	@${RM} ${OBJECTDIR}/IO_Entity/IO_Control.o.d 
	@${RM} ${OBJECTDIR}/IO_Entity/IO_Control.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  IO_Entity/IO_Control.c  -o ${OBJECTDIR}/IO_Entity/IO_Control.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/IO_Entity/IO_Control.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"Common" -I"IO_Entity" -I"Timers" -I"RTC_Control" -I"usb_app" -I"usb" -I"RTC_Profile" -I"RTC_Log" -I"RTC_PulseControl" -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/IO_Entity/IO_Entity_Mgr.o: IO_Entity/IO_Entity_Mgr.c  .generated_files/flags/default/8f66fc527a1f1c9b33c2619f2aea2e0f37bebb87 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/IO_Entity" 
	@${RM} ${OBJECTDIR}/IO_Entity/IO_Entity_Mgr.o.d 
	@${RM} ${OBJECTDIR}/IO_Entity/IO_Entity_Mgr.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  IO_Entity/IO_Entity_Mgr.c  -o ${OBJECTDIR}/IO_Entity/IO_Entity_Mgr.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/IO_Entity/IO_Entity_Mgr.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"Common" -I"IO_Entity" -I"Timers" -I"RTC_Control" -I"usb_app" -I"usb" -I"RTC_Profile" -I"RTC_Log" -I"RTC_PulseControl" -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/RTC_Control/RTC_Control.o: RTC_Control/RTC_Control.c  .generated_files/flags/default/1e4e1aef81b30a5793b7201fe78a849d5f3cd47a .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/RTC_Control" 
	@${RM} ${OBJECTDIR}/RTC_Control/RTC_Control.o.d 
	@${RM} ${OBJECTDIR}/RTC_Control/RTC_Control.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  RTC_Control/RTC_Control.c  -o ${OBJECTDIR}/RTC_Control/RTC_Control.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/RTC_Control/RTC_Control.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"Common" -I"IO_Entity" -I"Timers" -I"RTC_Control" -I"usb_app" -I"usb" -I"RTC_Profile" -I"RTC_Log" -I"RTC_PulseControl" -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/RTC_Profile/RTC_Profile.o: RTC_Profile/RTC_Profile.c  .generated_files/flags/default/2614760fb98584a1cde49828c314db1fdeda0a9d .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/RTC_Profile" 
	@${RM} ${OBJECTDIR}/RTC_Profile/RTC_Profile.o.d 
	@${RM} ${OBJECTDIR}/RTC_Profile/RTC_Profile.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  RTC_Profile/RTC_Profile.c  -o ${OBJECTDIR}/RTC_Profile/RTC_Profile.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/RTC_Profile/RTC_Profile.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"Common" -I"IO_Entity" -I"Timers" -I"RTC_Control" -I"usb_app" -I"usb" -I"RTC_Profile" -I"RTC_Log" -I"RTC_PulseControl" -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/RTC_Log/RTC_Log.o: RTC_Log/RTC_Log.c  .generated_files/flags/default/992f9f4a3dcae7ca1eb2c82a4d781e1b73a2e6cc .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/RTC_Log" 
	@${RM} ${OBJECTDIR}/RTC_Log/RTC_Log.o.d 
	@${RM} ${OBJECTDIR}/RTC_Log/RTC_Log.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  RTC_Log/RTC_Log.c  -o ${OBJECTDIR}/RTC_Log/RTC_Log.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/RTC_Log/RTC_Log.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"Common" -I"IO_Entity" -I"Timers" -I"RTC_Control" -I"usb_app" -I"usb" -I"RTC_Profile" -I"RTC_Log" -I"RTC_PulseControl" -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/RTC_PulseControl/RTC_PulseControl.o: RTC_PulseControl/RTC_PulseControl.c  .generated_files/flags/default/34871f5a1395835b664d89e433d312c4b6bea379 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/RTC_PulseControl" 
	@${RM} ${OBJECTDIR}/RTC_PulseControl/RTC_PulseControl.o.d 
	@${RM} ${OBJECTDIR}/RTC_PulseControl/RTC_PulseControl.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  RTC_PulseControl/RTC_PulseControl.c  -o ${OBJECTDIR}/RTC_PulseControl/RTC_PulseControl.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/RTC_PulseControl/RTC_PulseControl.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"Common" -I"IO_Entity" -I"Timers" -I"RTC_Control" -I"usb_app" -I"usb" -I"RTC_Profile" -I"RTC_Log" -I"RTC_PulseControl" -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Timers/Timers.o: Timers/Timers.c  .generated_files/flags/default/583f993a827f62853e32ed351ed01dea7a7d4a40 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/Timers" 
	@${RM} ${OBJECTDIR}/Timers/Timers.o.d 
	@${RM} ${OBJECTDIR}/Timers/Timers.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Timers/Timers.c  -o ${OBJECTDIR}/Timers/Timers.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Timers/Timers.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"Common" -I"IO_Entity" -I"Timers" -I"RTC_Control" -I"usb_app" -I"usb" -I"RTC_Profile" -I"RTC_Log" -I"RTC_PulseControl" -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/usb/usb_descriptors.o: usb/usb_descriptors.c  .generated_files/flags/default/a884d01613f1c14acd50afb8597c3daebe8d0957 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/usb" 
	@${RM} ${OBJECTDIR}/usb/usb_descriptors.o.d 
	@${RM} ${OBJECTDIR}/usb/usb_descriptors.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  usb/usb_descriptors.c  -o ${OBJECTDIR}/usb/usb_descriptors.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/usb/usb_descriptors.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"Common" -I"IO_Entity" -I"Timers" -I"RTC_Control" -I"usb_app" -I"usb" -I"RTC_Profile" -I"RTC_Log" -I"RTC_PulseControl" -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/usb/usb_device.o: usb/usb_device.c  .generated_files/flags/default/a31f652f031a7e1c765489aa62d441f167bc4df6 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/usb" 
	@${RM} ${OBJECTDIR}/usb/usb_device.o.d 
	@${RM} ${OBJECTDIR}/usb/usb_device.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  usb/usb_device.c  -o ${OBJECTDIR}/usb/usb_device.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/usb/usb_device.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"Common" -I"IO_Entity" -I"Timers" -I"RTC_Control" -I"usb_app" -I"usb" -I"RTC_Profile" -I"RTC_Log" -I"RTC_PulseControl" -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/usb/usb_device_hid.o: usb/usb_device_hid.c  .generated_files/flags/default/f11d5b001d25eecb03da338f14802ee992863ff3 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/usb" 
	@${RM} ${OBJECTDIR}/usb/usb_device_hid.o.d 
	@${RM} ${OBJECTDIR}/usb/usb_device_hid.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  usb/usb_device_hid.c  -o ${OBJECTDIR}/usb/usb_device_hid.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/usb/usb_device_hid.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"Common" -I"IO_Entity" -I"Timers" -I"RTC_Control" -I"usb_app" -I"usb" -I"RTC_Profile" -I"RTC_Log" -I"RTC_PulseControl" -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/usb/usb_events.o: usb/usb_events.c  .generated_files/flags/default/c2e65f0bda220bb0553e127b875eabd7ded82736 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/usb" 
	@${RM} ${OBJECTDIR}/usb/usb_events.o.d 
	@${RM} ${OBJECTDIR}/usb/usb_events.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  usb/usb_events.c  -o ${OBJECTDIR}/usb/usb_events.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/usb/usb_events.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"Common" -I"IO_Entity" -I"Timers" -I"RTC_Control" -I"usb_app" -I"usb" -I"RTC_Profile" -I"RTC_Log" -I"RTC_PulseControl" -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/usb/usb_hal_16bit.o: usb/usb_hal_16bit.c  .generated_files/flags/default/18b33d66bd5471f4e25efb52c09777ccf70fdf92 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/usb" 
	@${RM} ${OBJECTDIR}/usb/usb_hal_16bit.o.d 
	@${RM} ${OBJECTDIR}/usb/usb_hal_16bit.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  usb/usb_hal_16bit.c  -o ${OBJECTDIR}/usb/usb_hal_16bit.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/usb/usb_hal_16bit.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"Common" -I"IO_Entity" -I"Timers" -I"RTC_Control" -I"usb_app" -I"usb" -I"RTC_Profile" -I"RTC_Log" -I"RTC_PulseControl" -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/usb_app/usb_app.o: usb_app/usb_app.c  .generated_files/flags/default/b351c2c87271a393a0a6f88a9dfbf0f6f7715a22 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/usb_app" 
	@${RM} ${OBJECTDIR}/usb_app/usb_app.o.d 
	@${RM} ${OBJECTDIR}/usb_app/usb_app.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  usb_app/usb_app.c  -o ${OBJECTDIR}/usb_app/usb_app.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/usb_app/usb_app.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"Common" -I"IO_Entity" -I"Timers" -I"RTC_Control" -I"usb_app" -I"usb" -I"RTC_Profile" -I"RTC_Log" -I"RTC_PulseControl" -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/main.o: main.c  .generated_files/flags/default/519597a440dc0f9759657c73c69b8c2122d2bc75 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  main.c  -o ${OBJECTDIR}/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/main.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"Common" -I"IO_Entity" -I"Timers" -I"RTC_Control" -I"usb_app" -I"usb" -I"RTC_Profile" -I"RTC_Log" -I"RTC_PulseControl" -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
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
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o ${DISTDIR}/ABC_RTC.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -D__DEBUG=__DEBUG   -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"Common" -I"IO_Entity" -I"Timers" -I"RTC_Control" -I"usb_app" -I"usb" -I"RTC_Profile" -I"RTC_Log" -I"RTC_PulseControl"  -mreserve=data@0x1000:0x101B -mreserve=data@0x101C:0x101D -mreserve=data@0x101E:0x101F -mreserve=data@0x1020:0x1021 -mreserve=data@0x1022:0x1023 -mreserve=data@0x1024:0x1027 -mreserve=data@0x1028:0x104F   -Wl,--local-stack,,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D__DEBUG=__DEBUG,,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,${DISTDIR}/memoryfile.xml$(MP_EXTRA_LD_POST)  -mdfp="${DFP_DIR}/xc16" 
	
else
${DISTDIR}/ABC_RTC.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   p33EP512MU814_ABC.gld
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o ${DISTDIR}/ABC_RTC.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"Common" -I"IO_Entity" -I"Timers" -I"RTC_Control" -I"usb_app" -I"usb" -I"RTC_Profile" -I"RTC_Log" -I"RTC_PulseControl" -Wl,--local-stack,,--defsym=__MPLAB_BUILD=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,${DISTDIR}/memoryfile.xml$(MP_EXTRA_LD_POST)  -mdfp="${DFP_DIR}/xc16" 
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
