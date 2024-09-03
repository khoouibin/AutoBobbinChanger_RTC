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
SOURCEFILES_QUOTED_IF_SPACED=IO_Entity/IO_Control.c IO_Entity/IO_Entity_Mgr.c RTC_Control/RTC_Control.c Timers/Timers.c USB/usb_descriptors.c USB/usb_device.c USB/usb_function_generic.c USBComm/USBComm.c USBComm/USBComm_Driver.c USBComm/Msg_Prot.c main.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/IO_Entity/IO_Control.o ${OBJECTDIR}/IO_Entity/IO_Entity_Mgr.o ${OBJECTDIR}/RTC_Control/RTC_Control.o ${OBJECTDIR}/Timers/Timers.o ${OBJECTDIR}/USB/usb_descriptors.o ${OBJECTDIR}/USB/usb_device.o ${OBJECTDIR}/USB/usb_function_generic.o ${OBJECTDIR}/USBComm/USBComm.o ${OBJECTDIR}/USBComm/USBComm_Driver.o ${OBJECTDIR}/USBComm/Msg_Prot.o ${OBJECTDIR}/main.o
POSSIBLE_DEPFILES=${OBJECTDIR}/IO_Entity/IO_Control.o.d ${OBJECTDIR}/IO_Entity/IO_Entity_Mgr.o.d ${OBJECTDIR}/RTC_Control/RTC_Control.o.d ${OBJECTDIR}/Timers/Timers.o.d ${OBJECTDIR}/USB/usb_descriptors.o.d ${OBJECTDIR}/USB/usb_device.o.d ${OBJECTDIR}/USB/usb_function_generic.o.d ${OBJECTDIR}/USBComm/USBComm.o.d ${OBJECTDIR}/USBComm/USBComm_Driver.o.d ${OBJECTDIR}/USBComm/Msg_Prot.o.d ${OBJECTDIR}/main.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/IO_Entity/IO_Control.o ${OBJECTDIR}/IO_Entity/IO_Entity_Mgr.o ${OBJECTDIR}/RTC_Control/RTC_Control.o ${OBJECTDIR}/Timers/Timers.o ${OBJECTDIR}/USB/usb_descriptors.o ${OBJECTDIR}/USB/usb_device.o ${OBJECTDIR}/USB/usb_function_generic.o ${OBJECTDIR}/USBComm/USBComm.o ${OBJECTDIR}/USBComm/USBComm_Driver.o ${OBJECTDIR}/USBComm/Msg_Prot.o ${OBJECTDIR}/main.o

# Source Files
SOURCEFILES=IO_Entity/IO_Control.c IO_Entity/IO_Entity_Mgr.c RTC_Control/RTC_Control.c Timers/Timers.c USB/usb_descriptors.c USB/usb_device.c USB/usb_function_generic.c USBComm/USBComm.c USBComm/USBComm_Driver.c USBComm/Msg_Prot.c main.c



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
MP_LINKER_FILE_OPTION=,--script=p33EP512MU814.gld
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/IO_Entity/IO_Control.o: IO_Entity/IO_Control.c  .generated_files/flags/default/4f35b9591a45272e3d67e0032bf5b4e2e4192adf .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/IO_Entity" 
	@${RM} ${OBJECTDIR}/IO_Entity/IO_Control.o.d 
	@${RM} ${OBJECTDIR}/IO_Entity/IO_Control.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  IO_Entity/IO_Control.c  -o ${OBJECTDIR}/IO_Entity/IO_Control.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/IO_Entity/IO_Control.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"Common" -I"IO_Entity" -I"Timers" -I"USB" -I"USBComm" -I"RTC_Control" -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/IO_Entity/IO_Entity_Mgr.o: IO_Entity/IO_Entity_Mgr.c  .generated_files/flags/default/ab339c2131ac913d06d07b8558f6c84b07e253e .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/IO_Entity" 
	@${RM} ${OBJECTDIR}/IO_Entity/IO_Entity_Mgr.o.d 
	@${RM} ${OBJECTDIR}/IO_Entity/IO_Entity_Mgr.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  IO_Entity/IO_Entity_Mgr.c  -o ${OBJECTDIR}/IO_Entity/IO_Entity_Mgr.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/IO_Entity/IO_Entity_Mgr.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"Common" -I"IO_Entity" -I"Timers" -I"USB" -I"USBComm" -I"RTC_Control" -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/RTC_Control/RTC_Control.o: RTC_Control/RTC_Control.c  .generated_files/flags/default/e11c9eee441edcf4e64d237544d4b60e1cd115a .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/RTC_Control" 
	@${RM} ${OBJECTDIR}/RTC_Control/RTC_Control.o.d 
	@${RM} ${OBJECTDIR}/RTC_Control/RTC_Control.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  RTC_Control/RTC_Control.c  -o ${OBJECTDIR}/RTC_Control/RTC_Control.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/RTC_Control/RTC_Control.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"Common" -I"IO_Entity" -I"Timers" -I"USB" -I"USBComm" -I"RTC_Control" -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Timers/Timers.o: Timers/Timers.c  .generated_files/flags/default/218618967e110e4011f278c1cbc0d44ae65029dd .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/Timers" 
	@${RM} ${OBJECTDIR}/Timers/Timers.o.d 
	@${RM} ${OBJECTDIR}/Timers/Timers.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Timers/Timers.c  -o ${OBJECTDIR}/Timers/Timers.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Timers/Timers.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"Common" -I"IO_Entity" -I"Timers" -I"USB" -I"USBComm" -I"RTC_Control" -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/USB/usb_descriptors.o: USB/usb_descriptors.c  .generated_files/flags/default/47a52216c7488201b87ee5d21e72dd7b765493c0 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/USB" 
	@${RM} ${OBJECTDIR}/USB/usb_descriptors.o.d 
	@${RM} ${OBJECTDIR}/USB/usb_descriptors.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  USB/usb_descriptors.c  -o ${OBJECTDIR}/USB/usb_descriptors.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/USB/usb_descriptors.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"Common" -I"IO_Entity" -I"Timers" -I"USB" -I"USBComm" -I"RTC_Control" -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/USB/usb_device.o: USB/usb_device.c  .generated_files/flags/default/4bf0a5b61d37bc8742758bd0aeb15a21ea4c9a52 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/USB" 
	@${RM} ${OBJECTDIR}/USB/usb_device.o.d 
	@${RM} ${OBJECTDIR}/USB/usb_device.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  USB/usb_device.c  -o ${OBJECTDIR}/USB/usb_device.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/USB/usb_device.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"Common" -I"IO_Entity" -I"Timers" -I"USB" -I"USBComm" -I"RTC_Control" -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/USB/usb_function_generic.o: USB/usb_function_generic.c  .generated_files/flags/default/57e5879db8471e9bcfac21444b3f9c7c17fb90bc .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/USB" 
	@${RM} ${OBJECTDIR}/USB/usb_function_generic.o.d 
	@${RM} ${OBJECTDIR}/USB/usb_function_generic.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  USB/usb_function_generic.c  -o ${OBJECTDIR}/USB/usb_function_generic.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/USB/usb_function_generic.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"Common" -I"IO_Entity" -I"Timers" -I"USB" -I"USBComm" -I"RTC_Control" -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/USBComm/USBComm.o: USBComm/USBComm.c  .generated_files/flags/default/f8da9867412a602fb9c7b20b929470d7c11d7677 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/USBComm" 
	@${RM} ${OBJECTDIR}/USBComm/USBComm.o.d 
	@${RM} ${OBJECTDIR}/USBComm/USBComm.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  USBComm/USBComm.c  -o ${OBJECTDIR}/USBComm/USBComm.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/USBComm/USBComm.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"Common" -I"IO_Entity" -I"Timers" -I"USB" -I"USBComm" -I"RTC_Control" -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/USBComm/USBComm_Driver.o: USBComm/USBComm_Driver.c  .generated_files/flags/default/2ffeaaf35fa36fd20fa6279e7a769dedb1a2d4d1 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/USBComm" 
	@${RM} ${OBJECTDIR}/USBComm/USBComm_Driver.o.d 
	@${RM} ${OBJECTDIR}/USBComm/USBComm_Driver.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  USBComm/USBComm_Driver.c  -o ${OBJECTDIR}/USBComm/USBComm_Driver.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/USBComm/USBComm_Driver.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"Common" -I"IO_Entity" -I"Timers" -I"USB" -I"USBComm" -I"RTC_Control" -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/USBComm/Msg_Prot.o: USBComm/Msg_Prot.c  .generated_files/flags/default/a381759dd3979b53d969e183179e95d92880974e .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/USBComm" 
	@${RM} ${OBJECTDIR}/USBComm/Msg_Prot.o.d 
	@${RM} ${OBJECTDIR}/USBComm/Msg_Prot.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  USBComm/Msg_Prot.c  -o ${OBJECTDIR}/USBComm/Msg_Prot.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/USBComm/Msg_Prot.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"Common" -I"IO_Entity" -I"Timers" -I"USB" -I"USBComm" -I"RTC_Control" -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/main.o: main.c  .generated_files/flags/default/ebfa995f90ef096def12595cc465109312bfc6c2 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  main.c  -o ${OBJECTDIR}/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/main.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"Common" -I"IO_Entity" -I"Timers" -I"USB" -I"USBComm" -I"RTC_Control" -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
else
${OBJECTDIR}/IO_Entity/IO_Control.o: IO_Entity/IO_Control.c  .generated_files/flags/default/753b099e84bf2efbd117393133e8d9da79ec9120 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/IO_Entity" 
	@${RM} ${OBJECTDIR}/IO_Entity/IO_Control.o.d 
	@${RM} ${OBJECTDIR}/IO_Entity/IO_Control.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  IO_Entity/IO_Control.c  -o ${OBJECTDIR}/IO_Entity/IO_Control.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/IO_Entity/IO_Control.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"Common" -I"IO_Entity" -I"Timers" -I"USB" -I"USBComm" -I"RTC_Control" -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/IO_Entity/IO_Entity_Mgr.o: IO_Entity/IO_Entity_Mgr.c  .generated_files/flags/default/a3c79c87a3bdb7d6cab9c6c88b4f9d3aa78ac945 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/IO_Entity" 
	@${RM} ${OBJECTDIR}/IO_Entity/IO_Entity_Mgr.o.d 
	@${RM} ${OBJECTDIR}/IO_Entity/IO_Entity_Mgr.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  IO_Entity/IO_Entity_Mgr.c  -o ${OBJECTDIR}/IO_Entity/IO_Entity_Mgr.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/IO_Entity/IO_Entity_Mgr.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"Common" -I"IO_Entity" -I"Timers" -I"USB" -I"USBComm" -I"RTC_Control" -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/RTC_Control/RTC_Control.o: RTC_Control/RTC_Control.c  .generated_files/flags/default/c82089614ac780bf6315b936b30fce144210a8df .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/RTC_Control" 
	@${RM} ${OBJECTDIR}/RTC_Control/RTC_Control.o.d 
	@${RM} ${OBJECTDIR}/RTC_Control/RTC_Control.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  RTC_Control/RTC_Control.c  -o ${OBJECTDIR}/RTC_Control/RTC_Control.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/RTC_Control/RTC_Control.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"Common" -I"IO_Entity" -I"Timers" -I"USB" -I"USBComm" -I"RTC_Control" -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Timers/Timers.o: Timers/Timers.c  .generated_files/flags/default/98b6ebaff6f3e434f0847253b96f1da77e309d13 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/Timers" 
	@${RM} ${OBJECTDIR}/Timers/Timers.o.d 
	@${RM} ${OBJECTDIR}/Timers/Timers.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Timers/Timers.c  -o ${OBJECTDIR}/Timers/Timers.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Timers/Timers.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"Common" -I"IO_Entity" -I"Timers" -I"USB" -I"USBComm" -I"RTC_Control" -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/USB/usb_descriptors.o: USB/usb_descriptors.c  .generated_files/flags/default/44761651a91305152d3865bdab6dc7d42cc9ebf .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/USB" 
	@${RM} ${OBJECTDIR}/USB/usb_descriptors.o.d 
	@${RM} ${OBJECTDIR}/USB/usb_descriptors.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  USB/usb_descriptors.c  -o ${OBJECTDIR}/USB/usb_descriptors.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/USB/usb_descriptors.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"Common" -I"IO_Entity" -I"Timers" -I"USB" -I"USBComm" -I"RTC_Control" -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/USB/usb_device.o: USB/usb_device.c  .generated_files/flags/default/ff7177c7528cc493ffc96f50c07bfc8e0debbd6e .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/USB" 
	@${RM} ${OBJECTDIR}/USB/usb_device.o.d 
	@${RM} ${OBJECTDIR}/USB/usb_device.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  USB/usb_device.c  -o ${OBJECTDIR}/USB/usb_device.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/USB/usb_device.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"Common" -I"IO_Entity" -I"Timers" -I"USB" -I"USBComm" -I"RTC_Control" -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/USB/usb_function_generic.o: USB/usb_function_generic.c  .generated_files/flags/default/62c1ae892235a9df186aaf8a9e200c34b4ec88f3 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/USB" 
	@${RM} ${OBJECTDIR}/USB/usb_function_generic.o.d 
	@${RM} ${OBJECTDIR}/USB/usb_function_generic.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  USB/usb_function_generic.c  -o ${OBJECTDIR}/USB/usb_function_generic.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/USB/usb_function_generic.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"Common" -I"IO_Entity" -I"Timers" -I"USB" -I"USBComm" -I"RTC_Control" -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/USBComm/USBComm.o: USBComm/USBComm.c  .generated_files/flags/default/9a6c73395551469943c548e65795b7030b25cc6b .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/USBComm" 
	@${RM} ${OBJECTDIR}/USBComm/USBComm.o.d 
	@${RM} ${OBJECTDIR}/USBComm/USBComm.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  USBComm/USBComm.c  -o ${OBJECTDIR}/USBComm/USBComm.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/USBComm/USBComm.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"Common" -I"IO_Entity" -I"Timers" -I"USB" -I"USBComm" -I"RTC_Control" -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/USBComm/USBComm_Driver.o: USBComm/USBComm_Driver.c  .generated_files/flags/default/23fc334b34c071b0fcd9544ee9ee4886039f0638 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/USBComm" 
	@${RM} ${OBJECTDIR}/USBComm/USBComm_Driver.o.d 
	@${RM} ${OBJECTDIR}/USBComm/USBComm_Driver.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  USBComm/USBComm_Driver.c  -o ${OBJECTDIR}/USBComm/USBComm_Driver.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/USBComm/USBComm_Driver.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"Common" -I"IO_Entity" -I"Timers" -I"USB" -I"USBComm" -I"RTC_Control" -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/USBComm/Msg_Prot.o: USBComm/Msg_Prot.c  .generated_files/flags/default/ea152fb984a69a28134febeaa5614bb0f0a78c36 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/USBComm" 
	@${RM} ${OBJECTDIR}/USBComm/Msg_Prot.o.d 
	@${RM} ${OBJECTDIR}/USBComm/Msg_Prot.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  USBComm/Msg_Prot.c  -o ${OBJECTDIR}/USBComm/Msg_Prot.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/USBComm/Msg_Prot.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"Common" -I"IO_Entity" -I"Timers" -I"USB" -I"USBComm" -I"RTC_Control" -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/main.o: main.c  .generated_files/flags/default/b5d3df8f6906e65f940d9dabc2286bf480d83c22 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  main.c  -o ${OBJECTDIR}/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/main.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"Common" -I"IO_Entity" -I"Timers" -I"USB" -I"USBComm" -I"RTC_Control" -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
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
${DISTDIR}/ABC_RTC.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o ${DISTDIR}/ABC_RTC.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -D__DEBUG=__DEBUG   -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"Common" -I"IO_Entity" -I"Timers" -I"USB" -I"USBComm" -I"RTC_Control"  -mreserve=data@0x1000:0x101B -mreserve=data@0x101C:0x101D -mreserve=data@0x101E:0x101F -mreserve=data@0x1020:0x1021 -mreserve=data@0x1022:0x1023 -mreserve=data@0x1024:0x1027 -mreserve=data@0x1028:0x104F   -Wl,--local-stack,,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D__DEBUG=__DEBUG,,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,${DISTDIR}/memoryfile.xml$(MP_EXTRA_LD_POST)  -mdfp="${DFP_DIR}/xc16" 
	
else
${DISTDIR}/ABC_RTC.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o ${DISTDIR}/ABC_RTC.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"Common" -I"IO_Entity" -I"Timers" -I"USB" -I"USBComm" -I"RTC_Control" -Wl,--local-stack,,--defsym=__MPLAB_BUILD=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,${DISTDIR}/memoryfile.xml$(MP_EXTRA_LD_POST)  -mdfp="${DFP_DIR}/xc16" 
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
