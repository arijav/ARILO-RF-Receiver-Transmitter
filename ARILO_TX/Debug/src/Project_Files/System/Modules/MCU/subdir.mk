################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Project_Files/System/Modules/MCU/hw_ADC.c \
../src/Project_Files/System/Modules/MCU/hw_interr.c \
../src/Project_Files/System/Modules/MCU/hw_mcu.c \
../src/Project_Files/System/Modules/MCU/hw_ports.c \
../src/Project_Files/System/Modules/MCU/hw_spi.c \
../src/Project_Files/System/Modules/MCU/hw_timers.c \
../src/Project_Files/System/Modules/MCU/hw_usart.c 

OBJS += \
./src/Project_Files/System/Modules/MCU/hw_ADC.o \
./src/Project_Files/System/Modules/MCU/hw_interr.o \
./src/Project_Files/System/Modules/MCU/hw_mcu.o \
./src/Project_Files/System/Modules/MCU/hw_ports.o \
./src/Project_Files/System/Modules/MCU/hw_spi.o \
./src/Project_Files/System/Modules/MCU/hw_timers.o \
./src/Project_Files/System/Modules/MCU/hw_usart.o 

C_DEPS += \
./src/Project_Files/System/Modules/MCU/hw_ADC.d \
./src/Project_Files/System/Modules/MCU/hw_interr.d \
./src/Project_Files/System/Modules/MCU/hw_mcu.d \
./src/Project_Files/System/Modules/MCU/hw_ports.d \
./src/Project_Files/System/Modules/MCU/hw_spi.d \
./src/Project_Files/System/Modules/MCU/hw_timers.d \
./src/Project_Files/System/Modules/MCU/hw_usart.d 


# Each subdirectory must supply rules for building sources it contributes
src/Project_Files/System/Modules/MCU/%.o: ../src/Project_Files/System/Modules/MCU/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F10X_HD -DUSE_STDPERIPH_DRIVER -DHSE_VALUE=8000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f1-stdperiph" -I"/Volumes/JetDrive/Development/Workspace_Eclipse/ARILO_TX/src/Project_Files" -I"/Volumes/JetDrive/Development/Workspace_Eclipse/ARILO_TX/src/Project_Files/Application/State_Machines/CmdInput" -I"/Volumes/JetDrive/Development/Workspace_Eclipse/ARILO_TX/src/Project_Files/Application/State_Machines/Communications" -I"/Volumes/JetDrive/Development/Workspace_Eclipse/ARILO_TX/src/Project_Files/Application/State_Machines/Control_Buzzer" -I"/Volumes/JetDrive/Development/Workspace_Eclipse/ARILO_TX/src/Project_Files/Application/State_Machines/Control_Fan" -I"/Volumes/JetDrive/Development/Workspace_Eclipse/ARILO_TX/src/Project_Files/Application/State_Machines/Control_PPM" -I"/Volumes/JetDrive/Development/Workspace_Eclipse/ARILO_TX/src/Project_Files/Application/State_Machines/Display" -I"/Volumes/JetDrive/Development/Workspace_Eclipse/ARILO_TX/src/Project_Files/Application/State_Machines/Telemetry" -I"/Volumes/JetDrive/Development/Workspace_Eclipse/ARILO_TX/src/Project_Files/Event_Manager" -I"/Volumes/JetDrive/Development/Workspace_Eclipse/ARILO_TX/src/Project_Files/Lib" -I"/Volumes/JetDrive/Development/Workspace_Eclipse/ARILO_TX/src/Project_Files/System/HAL" -I"/Volumes/JetDrive/Development/Workspace_Eclipse/ARILO_TX/src/Project_Files/System/Low_Level_State_Machines/Error_Handler" -I"/Volumes/JetDrive/Development/Workspace_Eclipse/ARILO_TX/src/Project_Files/System/Low_Level_State_Machines/RF_Handler" -I"/Volumes/JetDrive/Development/Workspace_Eclipse/ARILO_TX/src/Project_Files/System/Low_Level_State_Machines/SafeLink_Handler" -I"/Volumes/JetDrive/Development/Workspace_Eclipse/ARILO_TX/src/Project_Files/System/Modules/MCU" -I"/Volumes/JetDrive/Development/Workspace_Eclipse/ARILO_TX/src/Project_Files/System/Modules/RF" -std=gnu99 -O0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

