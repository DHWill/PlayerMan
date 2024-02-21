################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Manager.cpp \
../src/main.cpp \
../src/utils.cpp 

CPP_DEPS += \
./src/Manager.d \
./src/main.d \
./src/utils.d 

OBJS += \
./src/Manager.o \
./src/main.o \
./src/utils.o 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	aarch64-tdx-linux-g++ -I/opt/tdx-xwayland/6.6.0/sysroots/cortexa72-cortexa53-tdx-linux/usr/include/glib-2.0 -I/opt/tdx-xwayland/6.6.0/sysroots/cortexa72-cortexa53-tdx-linux/usr/include/atk-1.0 -I/opt/tdx-xwayland/6.6.0/sysroots/cortexa72-cortexa53-tdx-linux/usr/include/gdk-pixbuf-2.0/ -I/opt/tdx-xwayland/6.6.0/sysroots/cortexa72-cortexa53-tdx-linux/usr/include/cairo -I/opt/tdx-xwayland/6.6.0/sysroots/cortexa72-cortexa53-tdx-linux/usr/include/pango-1.0 -I/opt/tdx-xwayland/6.6.0/sysroots/cortexa72-cortexa53-tdx-linux/usr/include/gtk-3.0 -I/opt/tdx-xwayland/6.6.0/sysroots/cortexa72-cortexa53-tdx-linux/usr/include/gtk-3.0/gdk/ -I/opt/tdx-xwayland/6.6.0/sysroots/cortexa72-cortexa53-tdx-linux/usr/lib/glib-2.0/include -I/opt/tdx-xwayland/6.6.0/sysroots/cortexa72-cortexa53-tdx-linux/usr/include/gstreamer-1.0/ -I/opt/tdx-xwayland/6.6.0/sysroots/cortexa72-cortexa53-tdx-linux/usr/include/ -I/opt/tdx-xwayland/6.6.0/sysroots/cortexa72-cortexa53-tdx-linux/usr/include/harfbuzz -O0 -g3 -Wall -c -fmessage-length=0 --sysroot=/opt/tdx-xwayland/6.6.0/sysroots/cortexa72-cortexa53-tdx-linux `/opt/tdx-xwayland/6.6.0/sysroots/x86_64-tdxsdk-linux/usr/bin/pkg-config --libs gtk+-3.0` -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src

clean-src:
	-$(RM) ./src/Manager.d ./src/Manager.o ./src/main.d ./src/main.o ./src/utils.d ./src/utils.o

.PHONY: clean-src

