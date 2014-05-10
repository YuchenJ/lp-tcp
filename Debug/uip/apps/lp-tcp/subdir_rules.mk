################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
uip/apps/lp-tcp/driver_uart.obj: ../uip/apps/lp-tcp/driver_uart.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv5/tools/compiler/msp430_4.1.8/bin/cl430" -vmsp --abi=eabi -g --include_path="C:/ti/ccsv5/ccs_base/msp430/include" --include_path="C:/ti/ccsv5/tools/compiler/msp430_4.1.8/include" --include_path="C:/Users/kellogg/workspace_v5_4/lp-tcp/uip/msp430g2553" --include_path="C:/Users/kellogg/workspace_v5_4/lp-tcp/uip/uip" --include_path="C:/Users/kellogg/workspace_v5_4/lp-tcp/uip/apps/lp-tcp" --advice:power=all --define=__MSP430G2553__ --diag_warning=225 --display_error_number --diag_wrap=off --printf_support=minimal --preproc_with_compile --preproc_dependency="uip/apps/lp-tcp/driver_uart.pp" --obj_directory="uip/apps/lp-tcp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

uip/apps/lp-tcp/main.obj: ../uip/apps/lp-tcp/main.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv5/tools/compiler/msp430_4.1.8/bin/cl430" -vmsp --abi=eabi -g --include_path="C:/ti/ccsv5/ccs_base/msp430/include" --include_path="C:/ti/ccsv5/tools/compiler/msp430_4.1.8/include" --include_path="C:/Users/kellogg/workspace_v5_4/lp-tcp/uip/msp430g2553" --include_path="C:/Users/kellogg/workspace_v5_4/lp-tcp/uip/uip" --include_path="C:/Users/kellogg/workspace_v5_4/lp-tcp/uip/apps/lp-tcp" --advice:power=all --define=__MSP430G2553__ --diag_warning=225 --display_error_number --diag_wrap=off --printf_support=minimal --preproc_with_compile --preproc_dependency="uip/apps/lp-tcp/main.pp" --obj_directory="uip/apps/lp-tcp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


