################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../littlevgl/lvgl/src/lv_misc/lv_anim.c \
../littlevgl/lvgl/src/lv_misc/lv_area.c \
../littlevgl/lvgl/src/lv_misc/lv_async.c \
../littlevgl/lvgl/src/lv_misc/lv_bidi.c \
../littlevgl/lvgl/src/lv_misc/lv_color.c \
../littlevgl/lvgl/src/lv_misc/lv_fs.c \
../littlevgl/lvgl/src/lv_misc/lv_gc.c \
../littlevgl/lvgl/src/lv_misc/lv_ll.c \
../littlevgl/lvgl/src/lv_misc/lv_log.c \
../littlevgl/lvgl/src/lv_misc/lv_math.c \
../littlevgl/lvgl/src/lv_misc/lv_mem.c \
../littlevgl/lvgl/src/lv_misc/lv_printf.c \
../littlevgl/lvgl/src/lv_misc/lv_task.c \
../littlevgl/lvgl/src/lv_misc/lv_templ.c \
../littlevgl/lvgl/src/lv_misc/lv_txt.c \
../littlevgl/lvgl/src/lv_misc/lv_txt_ap.c \
../littlevgl/lvgl/src/lv_misc/lv_utils.c 

OBJS += \
./littlevgl/lvgl/src/lv_misc/lv_anim.o \
./littlevgl/lvgl/src/lv_misc/lv_area.o \
./littlevgl/lvgl/src/lv_misc/lv_async.o \
./littlevgl/lvgl/src/lv_misc/lv_bidi.o \
./littlevgl/lvgl/src/lv_misc/lv_color.o \
./littlevgl/lvgl/src/lv_misc/lv_fs.o \
./littlevgl/lvgl/src/lv_misc/lv_gc.o \
./littlevgl/lvgl/src/lv_misc/lv_ll.o \
./littlevgl/lvgl/src/lv_misc/lv_log.o \
./littlevgl/lvgl/src/lv_misc/lv_math.o \
./littlevgl/lvgl/src/lv_misc/lv_mem.o \
./littlevgl/lvgl/src/lv_misc/lv_printf.o \
./littlevgl/lvgl/src/lv_misc/lv_task.o \
./littlevgl/lvgl/src/lv_misc/lv_templ.o \
./littlevgl/lvgl/src/lv_misc/lv_txt.o \
./littlevgl/lvgl/src/lv_misc/lv_txt_ap.o \
./littlevgl/lvgl/src/lv_misc/lv_utils.o 

C_DEPS += \
./littlevgl/lvgl/src/lv_misc/lv_anim.d \
./littlevgl/lvgl/src/lv_misc/lv_area.d \
./littlevgl/lvgl/src/lv_misc/lv_async.d \
./littlevgl/lvgl/src/lv_misc/lv_bidi.d \
./littlevgl/lvgl/src/lv_misc/lv_color.d \
./littlevgl/lvgl/src/lv_misc/lv_fs.d \
./littlevgl/lvgl/src/lv_misc/lv_gc.d \
./littlevgl/lvgl/src/lv_misc/lv_ll.d \
./littlevgl/lvgl/src/lv_misc/lv_log.d \
./littlevgl/lvgl/src/lv_misc/lv_math.d \
./littlevgl/lvgl/src/lv_misc/lv_mem.d \
./littlevgl/lvgl/src/lv_misc/lv_printf.d \
./littlevgl/lvgl/src/lv_misc/lv_task.d \
./littlevgl/lvgl/src/lv_misc/lv_templ.d \
./littlevgl/lvgl/src/lv_misc/lv_txt.d \
./littlevgl/lvgl/src/lv_misc/lv_txt_ap.d \
./littlevgl/lvgl/src/lv_misc/lv_utils.d 


# Each subdirectory must supply rules for building sources it contributes
littlevgl/lvgl/src/lv_misc/%.o: ../littlevgl/lvgl/src/lv_misc/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -DCPU_LPC54628J512ET180=1 -DA_LITTLE_ENDIAN -DCPU_LPC54628J512ET180_cm4 -DCPU_LPC54628 -D__USE_CMSIS -DLV_CONF_INCLUDE_SIMPLE=1 -DLV_EX_CONF_INCLUDE_SIMPLE=1 -DSERIAL_PORT_TYPE_UART=1 -DFSL_RTOS_FREE_RTOS -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -DNDEBUG -D__REDLIB__ -DSDK_OS_FREE_RTOS -I"C:\Users\andre\Documents\MCUXpressoIDE_11.3.0_5222\ELO\OM13098_Smart_Fridge\utilities" -I"C:\Users\andre\Documents\MCUXpressoIDE_11.3.0_5222\ELO\OM13098_Smart_Fridge\freertos\freertos_kernel\include" -I"C:\Users\andre\Documents\MCUXpressoIDE_11.3.0_5222\ELO\OM13098_Smart_Fridge\freertos\freertos_kernel\portable\GCC\ARM_CM4F" -I"C:\Users\andre\Documents\MCUXpressoIDE_11.3.0_5222\ELO\OM13098_Smart_Fridge\CMSIS" -I"C:\Users\andre\Documents\MCUXpressoIDE_11.3.0_5222\ELO\OM13098_Smart_Fridge\drivers" -I"C:\Users\andre\Documents\MCUXpressoIDE_11.3.0_5222\ELO\OM13098_Smart_Fridge\source" -I"C:\Users\andre\Documents\MCUXpressoIDE_11.3.0_5222\ELO\OM13098_Smart_Fridge\touchpanel" -I"C:\Users\andre\Documents\MCUXpressoIDE_11.3.0_5222\ELO\OM13098_Smart_Fridge\littlevgl\lvgl" -I"C:\Users\andre\Documents\MCUXpressoIDE_11.3.0_5222\ELO\OM13098_Smart_Fridge\littlevgl\lvgl\porting" -I"C:\Users\andre\Documents\MCUXpressoIDE_11.3.0_5222\ELO\OM13098_Smart_Fridge\littlevgl\lvgl\src" -I"C:\Users\andre\Documents\MCUXpressoIDE_11.3.0_5222\ELO\OM13098_Smart_Fridge\littlevgl\lvgl\src\lv_core" -I"C:\Users\andre\Documents\MCUXpressoIDE_11.3.0_5222\ELO\OM13098_Smart_Fridge\littlevgl\lvgl\src\lv_draw" -I"C:\Users\andre\Documents\MCUXpressoIDE_11.3.0_5222\ELO\OM13098_Smart_Fridge\littlevgl\lvgl\src\lv_font" -I"C:\Users\andre\Documents\MCUXpressoIDE_11.3.0_5222\ELO\OM13098_Smart_Fridge\littlevgl\lvgl\src\lv_gpu" -I"C:\Users\andre\Documents\MCUXpressoIDE_11.3.0_5222\ELO\OM13098_Smart_Fridge\littlevgl\lvgl\src\lv_hal" -I"C:\Users\andre\Documents\MCUXpressoIDE_11.3.0_5222\ELO\OM13098_Smart_Fridge\littlevgl\lvgl\src\lv_misc" -I"C:\Users\andre\Documents\MCUXpressoIDE_11.3.0_5222\ELO\OM13098_Smart_Fridge\littlevgl\lvgl\src\lv_themes" -I"C:\Users\andre\Documents\MCUXpressoIDE_11.3.0_5222\ELO\OM13098_Smart_Fridge\littlevgl\lvgl\src\lv_widgets" -I"C:\Users\andre\Documents\MCUXpressoIDE_11.3.0_5222\ELO\OM13098_Smart_Fridge\component\uart" -I"C:\Users\andre\Documents\MCUXpressoIDE_11.3.0_5222\ELO\OM13098_Smart_Fridge\component\lists" -I"C:\Users\andre\Documents\MCUXpressoIDE_11.3.0_5222\ELO\OM13098_Smart_Fridge\component\serial_manager" -I"C:\Users\andre\Documents\MCUXpressoIDE_11.3.0_5222\ELO\OM13098_Smart_Fridge\littlevgl" -I"C:\Users\andre\Documents\MCUXpressoIDE_11.3.0_5222\ELO\OM13098_Smart_Fridge\board" -I"C:\Users\andre\Documents\MCUXpressoIDE_11.3.0_5222\ELO\OM13098_Smart_Fridge" -I"C:\Users\andre\Documents\MCUXpressoIDE_11.3.0_5222\ELO\OM13098_Smart_Fridge\wifi_qca\port\boards\lpcxpresso54628\freertos\gt202" -I"C:\Users\andre\Documents\MCUXpressoIDE_11.3.0_5222\ELO\OM13098_Smart_Fridge\wifi_qca\port\boards\lpcxpresso54628\freertos" -I"C:\Users\andre\Documents\MCUXpressoIDE_11.3.0_5222\ELO\OM13098_Smart_Fridge\wifi_qca\port\env\freertos" -I"C:\Users\andre\Documents\MCUXpressoIDE_11.3.0_5222\ELO\OM13098_Smart_Fridge\wifi_qca" -I"C:\Users\andre\Documents\MCUXpressoIDE_11.3.0_5222\ELO\OM13098_Smart_Fridge\wifi_qca\common_src\hcd" -I"C:\Users\andre\Documents\MCUXpressoIDE_11.3.0_5222\ELO\OM13098_Smart_Fridge\wifi_qca\common_src\include" -I"C:\Users\andre\Documents\MCUXpressoIDE_11.3.0_5222\ELO\OM13098_Smart_Fridge\wifi_qca\common_src\stack_common" -I"C:\Users\andre\Documents\MCUXpressoIDE_11.3.0_5222\ELO\OM13098_Smart_Fridge\wifi_qca\custom_src\include" -I"C:\Users\andre\Documents\MCUXpressoIDE_11.3.0_5222\ELO\OM13098_Smart_Fridge\wifi_qca\custom_src\stack_custom" -I"C:\Users\andre\Documents\MCUXpressoIDE_11.3.0_5222\ELO\OM13098_Smart_Fridge\wifi_qca\include\AR6002" -I"C:\Users\andre\Documents\MCUXpressoIDE_11.3.0_5222\ELO\OM13098_Smart_Fridge\wifi_qca\include\AR6002\hw2.0\hw" -I"C:\Users\andre\Documents\MCUXpressoIDE_11.3.0_5222\ELO\OM13098_Smart_Fridge\wifi_qca\include\AR6002\hw4.0\hw" -I"C:\Users\andre\Documents\MCUXpressoIDE_11.3.0_5222\ELO\OM13098_Smart_Fridge\wifi_qca\include" -I"C:\Users\andre\Documents\MCUXpressoIDE_11.3.0_5222\ELO\OM13098_Smart_Fridge\wifi_qca\port" -I"C:\Users\andre\Documents\MCUXpressoIDE_11.3.0_5222\ELO\OM13098_Smart_Fridge\wifi_qca\port\drivers\flexcomm_freertos" -I"C:\Users\andre\Documents\MCUXpressoIDE_11.3.0_5222\ELO\OM13098_Smart_Fridge\wifi_qca\port\boards\lpcxpresso54628\freertos\wifi10click" -I"C:\Users\andre\Documents\MCUXpressoIDE_11.3.0_5222\ELO\OM13098_Smart_Fridge\wifi_qca\common_src\wmi" -I"C:\Users\andre\Documents\MCUXpressoIDE_11.3.0_5222\ELO\OM13098_Smart_Fridge\GUI" -I"C:\Users\andre\Documents\MCUXpressoIDE_11.3.0_5222\ELO\OM13098_Smart_Fridge\GUI\Icons" -I"C:\Users\andre\Documents\MCUXpressoIDE_11.3.0_5222\ELO\OM13098_Smart_Fridge\GUI\fonts" -I"C:\Users\andre\Documents\MCUXpressoIDE_11.3.0_5222\ELO\OM13098_Smart_Fridge\RC522" -I"C:\Users\andre\Documents\MCUXpressoIDE_11.3.0_5222\ELO\OM13098_Smart_Fridge\GUI\screens" -I"C:\Users\andre\Documents\MCUXpressoIDE_11.3.0_5222\ELO\OM13098_Smart_Fridge\component\device" -O2 -fno-common -g -Wall -c  -ffunction-sections  -fdata-sections  -ffreestanding  -fno-builtin -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


