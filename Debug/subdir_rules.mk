################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
EK_TM4C129EXL.obj: ../EK_TM4C129EXL.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_15.12.4.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --opt_for_speed=5 --include_path="C:/Users/jalex/workspace_v6_2/MobileRobotTIVA" --include_path="C:/ti/tirtos_tivac_2_16_01_14/products/tidrivers_tivac_2_16_01_13/packages/ti/mw/wifi/cc3x00" --include_path="C:/ti/tirtos_tivac_2_16_01_14/products/tidrivers_tivac_2_16_01_13/packages/ti/mw/wifi/cc3x00/oslib" --include_path="C:/ti/tirtos_tivac_2_16_01_14/products/tidrivers_tivac_2_16_01_13/packages/ti/mw/wifi/cc3x00/simplelink/include" --include_path="C:/ti/tirtos_tivac_2_16_01_14/products/TivaWare_C_Series-2.1.1.71b" --include_path="C:/ti/tirtos_tivac_2_16_01_14/products/bios_6_45_02_31/packages/ti/sysbios/posix" --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-arm_15.12.4.LTS/include" -g --gcc --define=ccs="ccs" --define=TI_DRIVERS_UART_DMA --define=PART_TM4C129ENCPDT --define=ccs --define=TIVAWARE --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --preproc_with_compile --preproc_dependency="EK_TM4C129EXL.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

empty.obj: ../empty.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_15.12.4.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --opt_for_speed=5 --include_path="C:/Users/jalex/workspace_v6_2/MobileRobotTIVA" --include_path="C:/ti/tirtos_tivac_2_16_01_14/products/tidrivers_tivac_2_16_01_13/packages/ti/mw/wifi/cc3x00" --include_path="C:/ti/tirtos_tivac_2_16_01_14/products/tidrivers_tivac_2_16_01_13/packages/ti/mw/wifi/cc3x00/oslib" --include_path="C:/ti/tirtos_tivac_2_16_01_14/products/tidrivers_tivac_2_16_01_13/packages/ti/mw/wifi/cc3x00/simplelink/include" --include_path="C:/ti/tirtos_tivac_2_16_01_14/products/TivaWare_C_Series-2.1.1.71b" --include_path="C:/ti/tirtos_tivac_2_16_01_14/products/bios_6_45_02_31/packages/ti/sysbios/posix" --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-arm_15.12.4.LTS/include" -g --gcc --define=ccs="ccs" --define=TI_DRIVERS_UART_DMA --define=PART_TM4C129ENCPDT --define=ccs --define=TIVAWARE --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --preproc_with_compile --preproc_dependency="empty.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

build-2079561537: ../empty.cfg
	@echo 'Building file: $<'
	@echo 'Invoking: XDCtools'
	"C:/ti/tirex-content/xdctools_3_32_00_06_core/xs" --xdcpath="C:/ti/tirtos_tivac_2_16_01_14/packages;C:/ti/tirtos_tivac_2_16_01_14/products/tidrivers_tivac_2_16_01_13/packages;C:/ti/tirtos_tivac_2_16_01_14/products/bios_6_45_02_31/packages;C:/ti/tirtos_tivac_2_16_01_14/products/ndk_2_25_00_09/packages;C:/ti/tirtos_tivac_2_16_01_14/products/uia_2_00_05_50/packages;C:/ti/tirtos_tivac_2_16_01_14/products/ns_1_11_00_10/packages;C:/ti/ccsv6/ccs_base;" xdc.tools.configuro -o configPkg -t ti.targets.arm.elf.M4F -p ti.platforms.tiva:TM4C129ENCPDT -r release -c "C:/ti/ccsv6/tools/compiler/ti-cgt-arm_15.12.4.LTS" --compileOptions "-mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --opt_for_speed=5 --include_path=\"C:/Users/jalex/workspace_v6_2/MobileRobotTIVA\" --include_path=\"C:/ti/tirtos_tivac_2_16_01_14/products/tidrivers_tivac_2_16_01_13/packages/ti/mw/wifi/cc3x00\" --include_path=\"C:/ti/tirtos_tivac_2_16_01_14/products/tidrivers_tivac_2_16_01_13/packages/ti/mw/wifi/cc3x00/oslib\" --include_path=\"C:/ti/tirtos_tivac_2_16_01_14/products/tidrivers_tivac_2_16_01_13/packages/ti/mw/wifi/cc3x00/simplelink/include\" --include_path=\"C:/ti/tirtos_tivac_2_16_01_14/products/TivaWare_C_Series-2.1.1.71b\" --include_path=\"C:/ti/tirtos_tivac_2_16_01_14/products/bios_6_45_02_31/packages/ti/sysbios/posix\" --include_path=\"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_15.12.4.LTS/include\" -g --gcc --define=ccs=\"ccs\" --define=TI_DRIVERS_UART_DMA --define=PART_TM4C129ENCPDT --define=ccs --define=TIVAWARE --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi  " "$<"
	@echo 'Finished building: $<'
	@echo ' '

configPkg/linker.cmd: build-2079561537
configPkg/compiler.opt: build-2079561537
configPkg/: build-2079561537

sockets.obj: ../sockets.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_15.12.4.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --opt_for_speed=5 --include_path="C:/Users/jalex/workspace_v6_2/MobileRobotTIVA" --include_path="C:/ti/tirtos_tivac_2_16_01_14/products/tidrivers_tivac_2_16_01_13/packages/ti/mw/wifi/cc3x00" --include_path="C:/ti/tirtos_tivac_2_16_01_14/products/tidrivers_tivac_2_16_01_13/packages/ti/mw/wifi/cc3x00/oslib" --include_path="C:/ti/tirtos_tivac_2_16_01_14/products/tidrivers_tivac_2_16_01_13/packages/ti/mw/wifi/cc3x00/simplelink/include" --include_path="C:/ti/tirtos_tivac_2_16_01_14/products/TivaWare_C_Series-2.1.1.71b" --include_path="C:/ti/tirtos_tivac_2_16_01_14/products/bios_6_45_02_31/packages/ti/sysbios/posix" --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-arm_15.12.4.LTS/include" -g --gcc --define=ccs="ccs" --define=TI_DRIVERS_UART_DMA --define=PART_TM4C129ENCPDT --define=ccs --define=TIVAWARE --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --preproc_with_compile --preproc_dependency="sockets.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


