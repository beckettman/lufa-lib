#
#             LUFA Library
#     Copyright (C) Dean Camera, 2012.
#
#  dean [at] fourwalledcubicle [dot] com
#           www.lufa-lib.org
#

# Makefile for the LUFA library itself. This can be used to generate the library documentation.


# Check to see if the LUFA_PATH variable has not been set (the makefile is not being included from a project makefile)
ifeq ($(origin LUFA_PATH), undefined)
   LUFA_ROOT_PATH = .
   ARCH = {AVR8,UC3,XMEGA}
else
   LUFA_ROOT_PATH = $(LUFA_PATH)/LUFA
endif

# Check to see if the chip architecture has not been defined in the user makefile, set a default architecture if not
ifeq ($(origin ARCH), undefined)
   ARCH = AVR8
endif

# Define module source file lists
LUFA_SRC_USB           = $(LUFA_ROOT_PATH)/Drivers/USB/Core/$(ARCH)/Device_$(ARCH).c          \
                         $(LUFA_ROOT_PATH)/Drivers/USB/Core/$(ARCH)/Endpoint_$(ARCH).c        \
                         $(LUFA_ROOT_PATH)/Drivers/USB/Core/$(ARCH)/Host_$(ARCH).c            \
                         $(LUFA_ROOT_PATH)/Drivers/USB/Core/$(ARCH)/Pipe_$(ARCH).c            \
                         $(LUFA_ROOT_PATH)/Drivers/USB/Core/$(ARCH)/USBController_$(ARCH).c   \
                         $(LUFA_ROOT_PATH)/Drivers/USB/Core/$(ARCH)/USBInterrupt_$(ARCH).c    \
                         $(LUFA_ROOT_PATH)/Drivers/USB/Core/$(ARCH)/EndpointStream_$(ARCH).c  \
                         $(LUFA_ROOT_PATH)/Drivers/USB/Core/$(ARCH)/PipeStream_$(ARCH).c      \
                         $(LUFA_ROOT_PATH)/Drivers/USB/Core/ConfigDescriptor.c                \
                         $(LUFA_ROOT_PATH)/Drivers/USB/Core/DeviceStandardReq.c               \
                         $(LUFA_ROOT_PATH)/Drivers/USB/Core/Events.c                          \
                         $(LUFA_ROOT_PATH)/Drivers/USB/Core/HostStandardReq.c                 \
                         $(LUFA_ROOT_PATH)/Drivers/USB/Core/USBTask.c                         \
                         $(LUFA_ROOT_PATH)/Drivers/USB/Class/Common/HIDParser.c
LUFA_SRC_USBCLASS      = $(LUFA_ROOT_PATH)/Drivers/USB/Class/Device/AudioClassDevice.c        \
                         $(LUFA_ROOT_PATH)/Drivers/USB/Class/Device/CDCClassDevice.c          \
                         $(LUFA_ROOT_PATH)/Drivers/USB/Class/Device/HIDClassDevice.c          \
                         $(LUFA_ROOT_PATH)/Drivers/USB/Class/Device/MassStorageClassDevice.c  \
                         $(LUFA_ROOT_PATH)/Drivers/USB/Class/Device/MIDIClassDevice.c         \
                         $(LUFA_ROOT_PATH)/Drivers/USB/Class/Device/RNDISClassDevice.c        \
                         $(LUFA_ROOT_PATH)/Drivers/USB/Class/Host/AndroidAccessoryClassHost.c \
                         $(LUFA_ROOT_PATH)/Drivers/USB/Class/Host/AudioClassHost.c            \
                         $(LUFA_ROOT_PATH)/Drivers/USB/Class/Host/CDCClassHost.c              \
                         $(LUFA_ROOT_PATH)/Drivers/USB/Class/Host/HIDClassHost.c              \
                         $(LUFA_ROOT_PATH)/Drivers/USB/Class/Host/MassStorageClassHost.c      \
                         $(LUFA_ROOT_PATH)/Drivers/USB/Class/Host/MIDIClassHost.c             \
                         $(LUFA_ROOT_PATH)/Drivers/USB/Class/Host/PrinterClassHost.c          \
                         $(LUFA_ROOT_PATH)/Drivers/USB/Class/Host/RNDISClassHost.c            \
                         $(LUFA_ROOT_PATH)/Drivers/USB/Class/Host/StillImageClassHost.c
LUFA_SRC_TEMPERATURE   = $(LUFA_ROOT_PATH)/Drivers/Board/Temperature.c
LUFA_SRC_SERIAL        = $(LUFA_ROOT_PATH)/Drivers/Peripheral/$(ARCH)/Serial_$(ARCH).c
LUFA_SRC_TWI           = $(LUFA_ROOT_PATH)/Drivers/Peripheral/$(ARCH)/TWI_$(ARCH).c


# Check to see if the LUFA_PATH variable has not been set (the makefile is not being included from a project makefile)
ifeq ($(origin LUFA_PATH), undefined)
   LUFA_VERSION_NUM    = $(shell grep LUFA_VERSION_STRING Version.h | cut -d'"' -f2)

   LUFA_SRC_ALL_FILES  = $(LUFA_SRC_USB)            \
                         $(LUFA_SRC_USBCLASS)       \
                         $(LUFA_SRC_TEMPERATURE)    \
                         $(LUFA_SRC_SERIAL)         \
                         $(LUFA_SRC_TWI)

   EXCLUDE_FROM_EXPORT = Documentation DoxygenPages CodeTemplates *.conf *.tar *.o *.lss *.lst *.hex *.elf *.bin

   all:

   clean:
	rm -f $(LUFA_SRC_ALL_FILES:%.c=%.o)
	rm -f $(LUFA_SRC_ALL_FILES:%.c=%.lst)

   clean_list:

   doxygen:
	@echo Generating LUFA Library Documentation...

	@if ( ( cat Doxygen.conf ; echo "PROJECT_NUMBER=$(LUFA_VERSION_NUM)" ) | doxygen - 2>&1 | grep -v "warning: ignoring unsupported tag" ;); then \
	  exit 1; \
	fi;

	@echo Documentation Generation Complete.

   clean_doxygen:
	rm -rf Documentation

   checksource:
   
   export_tar:
	@echo Exporting LUFA library to a TAR archive...
	@tar -cf LUFA_$(LUFA_VERSION_NUM).tar --directory=. $(EXCLUDE_FROM_EXPORT:%=--exclude=%) *
	@tar -cf LUFA_$(LUFA_VERSION_NUM)_Code_Templates.tar CodeTemplates
	@echo Export LUFA_$(LUFA_VERSION_NUM).tar complete.

   version:
	@echo "LUFA $(LUFA_VERSION_NUM)"

   .PHONY: all clean clean_list doxygen clean_doxygen checksource export_tar version
endif
