################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../GUI/Icons/KI.c \
../GUI/Icons/add.c \
../GUI/Icons/alcohols.c \
../GUI/Icons/dairy.c \
../GUI/Icons/disconnect.c \
../GUI/Icons/drinks.c \
../GUI/Icons/fish.c \
../GUI/Icons/frozen.c \
../GUI/Icons/fruitvege.c \
../GUI/Icons/klepsydra.c \
../GUI/Icons/meat.c \
../GUI/Icons/previous.c \
../GUI/Icons/refresh.c \
../GUI/Icons/sauces.c \
../GUI/Icons/settings.c \
../GUI/Icons/settings_white_img.c \
../GUI/Icons/shopcart_white_img.c \
../GUI/Icons/sweets.c 

OBJS += \
./GUI/Icons/KI.o \
./GUI/Icons/add.o \
./GUI/Icons/alcohols.o \
./GUI/Icons/dairy.o \
./GUI/Icons/disconnect.o \
./GUI/Icons/drinks.o \
./GUI/Icons/fish.o \
./GUI/Icons/frozen.o \
./GUI/Icons/fruitvege.o \
./GUI/Icons/klepsydra.o \
./GUI/Icons/meat.o \
./GUI/Icons/previous.o \
./GUI/Icons/refresh.o \
./GUI/Icons/sauces.o \
./GUI/Icons/settings.o \
./GUI/Icons/settings_white_img.o \
./GUI/Icons/shopcart_white_img.o \
./GUI/Icons/sweets.o 

C_DEPS += \
./GUI/Icons/KI.d \
./GUI/Icons/add.d \
./GUI/Icons/alcohols.d \
./GUI/Icons/dairy.d \
./GUI/Icons/disconnect.d \
./GUI/Icons/drinks.d \
./GUI/Icons/fish.d \
./GUI/Icons/frozen.d \
./GUI/Icons/fruitvege.d \
./GUI/Icons/klepsydra.d \
./GUI/Icons/meat.d \
./GUI/Icons/previous.d \
./GUI/Icons/refresh.d \
./GUI/Icons/sauces.d \
./GUI/Icons/settings.d \
./GUI/Icons/settings_white_img.d \
./GUI/Icons/shopcart_white_img.d \
./GUI/Icons/sweets.d 


# Each subdirectory must supply rules for building sources it contributes
GUI/Icons/%.o: ../GUI/Icons/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -DCPU_LPC54628J512ET180=1 -DA_LITTLE_ENDIAN -DCPU_LPC54628J512ET180_cm4 -DCPU_LPC54628 -D__USE_CMSIS -DLV_CONF_INCLUDE_SIMPLE=1 -DLV_EX_CONF_INCLUDE_SIMPLE=1 -DSERIAL_PORT_TYPE_UART=1 -DFSL_RTOS_FREE_RTOS -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -DNDEBUG -D__REDLIB__ -DSDK_OS_FREE_RTOS -I"C:\Users\andre\Documents\MCUXpressoIDE_11.3.0_5222\ELO\OM13098_Smart_Fridge\utilities" -I"C:\Users\andre\Documents\MCUXpressoIDE_11.3.0_5222\ELO\OM13098_Smart_Fridge\freertos\freertos_kernel\include" -I"C:\Users\andre\Documents\MCUXpressoIDE_11.3.0_5222\ELO\OM13098_Smart_Fridge\freertos\freertos_kernel\portable\GCC\ARM_CM4F" -I"C:\Users\andre\Documents\MCUXpressoIDE_11.3.0_5222\ELO\OM13098_Smart_Fridge\CMSIS" -I"C:\Users\andre\Documents\MCUXpressoIDE_11.3.0_5222\ELO\OM13098_Smart_Fridge\drivers" -I"C:\Users\andre\Documents\MCUXpressoIDE_11.3.0_5222\ELO\OM13098_Smart_Fridge\source" -I"C:\Users\andre\Documents\MCUXpressoIDE_11.3.0_5222\ELO\OM13098_Smart_Fridge\touchpanel" -I"C:\Users\andre\Documents\MCUXpressoIDE_11.3.0_5222\ELO\OM13098_Smart_Fridge\littlevgl\lvgl" -I"C:\Users\andre\Documents\MCUXpressoIDE_11.3.0_5222\ELO\OM13098_Smart_Fridge\littlevgl\lvgl\porting" -I"C:\Users\andre\Documents\MCUXpressoIDE_11.3.0_5222\ELO\OM13098_Smart_Fridge\littlevgl\lvgl\src" -I"C:\Users\andre\Documents\MCUXpressoIDE_11.3.0_5222\ELO\OM13098_Smart_Fridge\littlevgl\lvgl\src\lv_core" -I"C:\Users\andre\Documents\MCUXpressoIDE_11.3.0_5222\ELO\OM13098_Smart_Fridge\littlevgl\lvgl\src\lv_draw" -I"C:\Users\andre\Documents\MCUXpressoIDE_11.3.0_5222\ELO\OM13098_Smart_Fridge\littlevgl\lvgl\src\lv_font" -I"C:\Users\andre\Documents\MCUXpressoIDE_11.3.0_5222\ELO\OM13098_Smart_Fridge\littlevgl\lvgl\src\lv_gpu" -I"C:\Users\andre\Documents\MCUXpressoIDE_11.3.0_5222\ELO\OM13098_Smart_Fridge\littlevgl\lvgl\src\lv_hal" -I"C:\Users\andre\Documents\MCUXpressoIDE_11.3.0_5222\ELO\OM13098_Smart_Fridge\littlevgl\lvgl\src\lv_misc" -I"C:\Users\andre\Documents\MCUXpressoIDE_11.3.0_5222\ELO\OM13098_Smart_Fridge\littlevgl\lvgl\src\lv_themes" -I"C:\Users\andre\Documents\MCUXpressoIDE_11.3.0_5222\ELO\OM13098_Smart_Fridge\littlevgl\lvgl\src\lv_widgets" -I"C:\Users\andre\Documents\MCUXpressoIDE_11.3.0_5222\ELO\OM13098_Smart_Fridge\component\uart" -I"C:\Users\andre\Documents\MCUXpressoIDE_11.3.0_5222\ELO\OM13098_Smart_Fridge\component\lists" -I"C:\Users\andre\Documents\MCUXpressoIDE_11.3.0_5222\ELO\OM13098_Smart_Fridge\component\serial_manager" -I"C:\Users\andre\Documents\MCUXpressoIDE_11.3.0_5222\ELO\OM13098_Smart_Fridge\littlevgl" -I"C:\Users\andre\Documents\MCUXpressoIDE_11.3.0_5222\ELO\OM13098_Smart_Fridge\board" -I"C:\Users\andre\Documents\MCUXpressoIDE_11.3.0_5222\ELO\OM13098_Smart_Fridge" -I"C:\Users\andre\Documents\MCUXpressoIDE_11.3.0_5222\ELO\OM13098_Smart_Fridge\wifi_qca\port\boards\lpcxpresso54628\freertos\gt202" -I"C:\Users\andre\Documents\MCUXpressoIDE_11.3.0_5222\ELO\OM13098_Smart_Fridge\wifi_qca\port\boards\lpcxpresso54628\freertos" -I"C:\Users\andre\Documents\MCUXpressoIDE_11.3.0_5222\ELO\OM13098_Smart_Fridge\wifi_qca\port\env\freertos" -I"C:\Users\andre\Documents\MCUXpressoIDE_11.3.0_5222\ELO\OM13098_Smart_Fridge\wifi_qca" -I"C:\Users\andre\Documents\MCUXpressoIDE_11.3.0_5222\ELO\OM13098_Smart_Fridge\wifi_qca\common_src\hcd" -I"C:\Users\andre\Documents\MCUXpressoIDE_11.3.0_5222\ELO\OM13098_Smart_Fridge\wifi_qca\common_src\include" -I"C:\Users\andre\Documents\MCUXpressoIDE_11.3.0_5222\ELO\OM13098_Smart_Fridge\wifi_qca\common_src\stack_common" -I"C:\Users\andre\Documents\MCUXpressoIDE_11.3.0_5222\ELO\OM13098_Smart_Fridge\wifi_qca\custom_src\include" -I"C:\Users\andre\Documents\MCUXpressoIDE_11.3.0_5222\ELO\OM13098_Smart_Fridge\wifi_qca\custom_src\stack_custom" -I"C:\Users\andre\Documents\MCUXpressoIDE_11.3.0_5222\ELO\OM13098_Smart_Fridge\wifi_qca\include\AR6002" -I"C:\Users\andre\Documents\MCUXpressoIDE_11.3.0_5222\ELO\OM13098_Smart_Fridge\wifi_qca\include\AR6002\hw2.0\hw" -I"C:\Users\andre\Documents\MCUXpressoIDE_11.3.0_5222\ELO\OM13098_Smart_Fridge\wifi_qca\include\AR6002\hw4.0\hw" -I"C:\Users\andre\Documents\MCUXpressoIDE_11.3.0_5222\ELO\OM13098_Smart_Fridge\wifi_qca\include" -I"C:\Users\andre\Documents\MCUXpressoIDE_11.3.0_5222\ELO\OM13098_Smart_Fridge\wifi_qca\port" -I"C:\Users\andre\Documents\MCUXpressoIDE_11.3.0_5222\ELO\OM13098_Smart_Fridge\wifi_qca\port\drivers\flexcomm_freertos" -I"C:\Users\andre\Documents\MCUXpressoIDE_11.3.0_5222\ELO\OM13098_Smart_Fridge\wifi_qca\port\boards\lpcxpresso54628\freertos\wifi10click" -I"C:\Users\andre\Documents\MCUXpressoIDE_11.3.0_5222\ELO\OM13098_Smart_Fridge\wifi_qca\common_src\wmi" -I"C:\Users\andre\Documents\MCUXpressoIDE_11.3.0_5222\ELO\OM13098_Smart_Fridge\GUI" -I"C:\Users\andre\Documents\MCUXpressoIDE_11.3.0_5222\ELO\OM13098_Smart_Fridge\GUI\Icons" -I"C:\Users\andre\Documents\MCUXpressoIDE_11.3.0_5222\ELO\OM13098_Smart_Fridge\GUI\fonts" -I"C:\Users\andre\Documents\MCUXpressoIDE_11.3.0_5222\ELO\OM13098_Smart_Fridge\RC522" -I"C:\Users\andre\Documents\MCUXpressoIDE_11.3.0_5222\ELO\OM13098_Smart_Fridge\GUI\screens" -I"C:\Users\andre\Documents\MCUXpressoIDE_11.3.0_5222\ELO\OM13098_Smart_Fridge\component\device" -O2 -fno-common -g -Wall -c  -ffunction-sections  -fdata-sections  -ffreestanding  -fno-builtin -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


