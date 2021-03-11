################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include wifi_qca/port/env/freertos/subdir.mk
-include wifi_qca/port/drivers/flexcomm_freertos/subdir.mk
-include wifi_qca/port/boards/lpcxpresso54628/freertos/subdir.mk
-include wifi_qca/custom_src/stack_custom/subdir.mk
-include wifi_qca/custom_src/qapi/subdir.mk
-include wifi_qca/custom_src/hw_interface/subdir.mk
-include wifi_qca/custom_src/driver/subdir.mk
-include wifi_qca/custom_src/api_interface/subdir.mk
-include wifi_qca/common_src/wmi/subdir.mk
-include wifi_qca/common_src/util/subdir.mk
-include wifi_qca/common_src/storerecall/subdir.mk
-include wifi_qca/common_src/stack_common/subdir.mk
-include wifi_qca/common_src/reorder/subdir.mk
-include wifi_qca/common_src/qapi/subdir.mk
-include wifi_qca/common_src/hw_interface/subdir.mk
-include wifi_qca/common_src/htc/subdir.mk
-include wifi_qca/common_src/hcd/subdir.mk
-include wifi_qca/common_src/driver/subdir.mk
-include wifi_qca/common_src/bmi/subdir.mk
-include wifi_qca/common_src/api_interface/subdir.mk
-include utilities/subdir.mk
-include touchpanel/subdir.mk
-include startup/subdir.mk
-include source/subdir.mk
-include littlevgl/lvgl/src/lv_widgets/subdir.mk
-include littlevgl/lvgl/src/lv_themes/subdir.mk
-include littlevgl/lvgl/src/lv_misc/subdir.mk
-include littlevgl/lvgl/src/lv_hal/subdir.mk
-include littlevgl/lvgl/src/lv_gpu/subdir.mk
-include littlevgl/lvgl/src/lv_font/subdir.mk
-include littlevgl/lvgl/src/lv_draw/subdir.mk
-include littlevgl/lvgl/src/lv_core/subdir.mk
-include littlevgl/lv_examples/src/lv_demo_widgets/subdir.mk
-include freertos/freertos_kernel/portable/MemMang/subdir.mk
-include freertos/freertos_kernel/portable/GCC/ARM_CM4F/subdir.mk
-include freertos/freertos_kernel/subdir.mk
-include drivers/subdir.mk
-include device/subdir.mk
-include component/uart/subdir.mk
-include component/serial_manager/subdir.mk
-include component/lists/subdir.mk
-include board/subdir.mk
-include RC522/subdir.mk
-include GUI/screens/subdir.mk
-include GUI/fonts/subdir.mk
-include GUI/config/subdir.mk
-include GUI/Icons/subdir.mk
-include GUI/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: OM13098_Smart_Fridge.axf

# Tool invocations
OM13098_Smart_Fridge.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -L"C:\Users\andre\Documents\MCUXpressoIDE_11.3.0_5222\release\OM13098_Smart_Fridge\libs" -Xlinker -Map="OM13098_Smart_Fridge.map" -Xlinker --gc-sections -Xlinker -print-memory-usage -Xlinker --sort-section=alignment -Xlinker --cref -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -T OM13098_Smart_Fridge_Debug.ld -o "OM13098_Smart_Fridge.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) OM13098_Smart_Fridge.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "OM13098_Smart_Fridge.axf"; # arm-none-eabi-objcopy -v -O binary "OM13098_Smart_Fridge.axf" "OM13098_Smart_Fridge.bin" ; # checksum -p LPC54628J512 -d "OM13098_Smart_Fridge.bin";
	-@echo ' '

.PHONY: all clean dependents post-build

-include ../makefile.targets
