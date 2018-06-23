#_______________________________________________________________________________
#
#                             Teensy makefile
#                     based on edam's Arduino makefile
#_______________________________________________________________________________
#                                                                    version 0.1
#
# Copyright (C) 2017-2018 Foster McLane <fkmclane@gmail.com>
# Copyright (C) 2011, 2012, 2013 Tim Marston <tim@ed.am>.
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#
#_______________________________________________________________________________
#
#
# This is a general purpose makefile for use with Arduino hardware and
# software.  It works with the arduino-1.0 and later software releases.  It
# should work GNU/Linux and OS X.  To download the latest version of this
# makefile visit the following website where you can also find documentation on
# it's use.  (The following text can only really be considered a reference.)
#
#   http://ed.am/dev/make/arduino-mk
#
# This makefile can be used as a drop-in replacement for the Arduino IDE's
# build system.  To use it, just copy arduino.mk in to your project directory.
# Or, you could save it somewhere (I keep mine at ~/src/arduino.mk) and create
# a symlink to it in your project directory, named "Makefile".  For example:
#
#   $ ln -s ~/src/arduino.mk Makefile
#
# The Arduino software (version 1.0 or later) is required.  On GNU/Linux you
# can probably install the software from your package manager.  If you are
# using Debian (or a derivative), try `apt-get install arduino`.  Otherwise,
# you can download the Arduino software manually from http://arduino.cc/.  It
# is suggested that you install it at ~/opt/arduino (or /Applications on OS X)
# if you are unsure.
#
# If you downloaded the Arduino software manually and unpacked it somewhere
# other than ~/opt/arduino (or /Applications), you will need to set up the
# ARDUINODIR environment variable to be the path where you unpacked it.  (If
# unset, ARDUINODIR defaults to some sensible places).  You could set this in
# your ~/.profile by adding something like this:
#
#   export ARDUINODIR=~/somewhere/arduino-1.0
#
# For each project, you will also need to set BOARD to the type of Arduino
# you're building for.  Type `make boards` for a list of acceptable values.
# For example:
#
#   $ export BOARD=uno
#   $ make
#
# You may also need to set SERIALDEV if it is not detected correctly.
#
# The presence of a .ino (or .pde) file causes the arduino.mk to automatically
# determine values for SOURCES, TARGET and LIBRARIES.  Any .c, .cc and .cpp
# files in the project directory (or any "util" or "utility" subdirectories)
# are automatically included in the build and are scanned for Arduino libraries
# that have been #included.  Note, there can only be one .ino (or .pde) file in
# a project directory and if you want to be compatible with the Arduino IDE, it
# should be called the same as the directory name.
#
# Alternatively, if you want to manually specify build variables, create a
# Makefile that defines SOURCES and LIBRARIES and then includes arduino.mk.
# (There is no need to define TARGET).  You can also specify the BOARD here, if
# the project has a specific one.  Here is an example Makefile:
#
#   SOURCES := main.cc other.cc
#   LIBRARIES := EEPROM
#   BOARD := teensy35
#   include ~/src/teensy.mk
#
# Here is a complete list of configuration parameters:
#
# ARDUINODIR   The path where the Arduino software is installed on your system.
#
# ARDUINOCONST The Arduino software version, as an integer, used to define the
#              ARDUINO version constant.  This defaults to 100 if undefined.
#
# ARMTOOLSPATH A space-separated list of directories that is searched in order
#              when looking for the avr build tools.  This defaults to PATH,
#              followed by subdirectories in ARDUINODIR.
#
# BOARD        Specify a target board type.  Run `make boards` to see available
#              board types.
#
# SPEED        Specify a target board speed.  Run `make speeds` to see available
#              board types.
#
# USB          Specify a target usb type.
#
# LAYOUT       Specify a target usb keyboard layout type (if applicable).
#
# CPPFLAGS     Specify any additional flags for the compiler.  The usual flags,
#              required to build the project, will be appended to this.
#
# LINKFLAGS    Specify any additional flags for the linker.  The usual flags,
#              required to build the project, will be appended to this.
#
# LIBRARIES    A list of Arduino libraries to build and include.  This is set
#              automatically if a .ino (or .pde) is found.
#
# LIBRARYPATH  A space-separated list of directories that is searched in order
#              when looking for Arduino libraries.  This defaults to "libs",
#              "libraries" (in the project directory), then your sketchbook
#              "libraries" directory, then the Arduino libraries directory.
#
# SERIALBAUD   The rate of serial transfer. Defaults to 9600.
#
# SERIALDEV    The POSIX device name of the serial device that is the Arduino.
#              If unspecified, an attempt is made to guess the name of a
#              connected Arduino's serial device, which may work in some cases.
#
# SOURCES      A list of all source files of whatever language.  The language
#              type is determined by the file extension.  This is set
#              automatically if a .ino (or .pde) is found.
#
# TARGET       The name of the target file.  This is set automatically if a
#              .ino (or .pde) is found, but it is not necessary to set it
#              otherwise.
#
# This makefile also defines the following goals for use on the command line
# when you run make:
#
# all          This is the default if no goal is specified.  It builds the
#              target.
#
# target       Builds the target.
#
# upload       Uploads the target (building it, as necessary) to an attached
#              Arduino.
#
# clean        Deletes files created during the build.
#
# boards       Display a list of available board names, so that you can set the
#              BOARD environment variable appropriately.
#
# monitor      Start a serial monitor session with the Arduino.
#
# size         Displays size information about the built target.
#
# <file>       Builds the specified file, either an object file or the target,
#              from those that that would be built for the project.
#_______________________________________________________________________________
#

BOARD=teensy35
SPEED=120

CWD := $(shell basename $(shell pwd))

# default arduino software directory, check software exists
ifndef ARDUINODIR
ARDUINODIR := $(firstword $(wildcard ~/opt/arduino /usr/share/arduino \
	/Applications/Arduino.app/Contents/Java \
	$(HOME)/Applications/Arduino.app/Contents/Java))
endif
ifeq "$(wildcard $(ARDUINODIR)/hardware/teensy/avr/boards.txt)" ""
$(error ARDUINODIR is not set correctly; teensyduino software not found)
endif

# default arduino version
ARDUINOCONST ?= 100

# default path for avr tools
ARMTOOLSPATH ?= $(subst :, , $(PATH)) $(ARDUINODIR)/hardware/tools \
	$(ARDUINODIR)/hardware/tools/arm/bin

# default path to find libraries
LIBRARYPATH ?= libraries libs $(SKETCHBOOKDIR)/libraries $(ARDUINODIR)/hardware/teensy/avr/libraries $(ARDUINODIR)/libraries

ifeq "$(SERIALBAUD)" ""
    SERIALBAUD := 9600
endif

# default serial device to a poor guess (something that might be an teensy)
SERIALDEVGUESS := 0
ifndef SERIALDEV
SERIALDEV := $(firstword $(wildcard \
	/dev/ttyACM? /dev/ttyUSB? /dev/tty.usbserial* /dev/tty.usbmodem*))
SERIALDEVGUESS := 1
endif

# no board?
ifndef BOARD
ifneq "$(MAKECMDGOALS)" "boards"
ifneq "$(MAKECMDGOALS)" "clean"
$(error BOARD is unset.  Type 'make boards' to see possible values)
endif
endif
endif

ifndef SPEED
$(error SPEED is unset.  Type 'make speeds' to see possible values)
endif

USB ?= serial
LAYOUT ?= US_ENGLISH

# obtain board parameters from the teensy boards.txt file
BOARDSFILE := $(ARDUINODIR)/hardware/teensy/avr/boards.txt
readboardsparam = $(shell sed -ne "s/^$(BOARD)\.$(1)=\(.*\)/\1/p" $(BOARDSFILE))
BOARD_BUILD_CORE := $(call readboardsparam,build.core)
BOARD_BUILD_FCPU := $(call readboardsparam,menu.speed.$(SPEED).build.fcpu)
BOARD_BUILD_USBTYPE := $(call readboardsparam,menu.usb.$(USB).build.usbtype)
BOARD_BUILD_FLAGS_COMMON := $(call readboardsparam,build.flags.common)
BOARD_BUILD_FLAGS_CPU := $(call readboardsparam,build.flags.cpu)
BOARD_BUILD_FLAGS_DEFS := $(call readboardsparam,build.flags.defs)
BOARD_BUILD_FLAGS_CPP := $(call readboardsparam,build.flags.cpp)
BOARD_BUILD_FLAGS_LD := $(call readboardsparam,build.flags.ld)
BOARD_BUILD_FLAGS_LIBS := $(call readboardsparam,build.flags.libs)

# obtain preferences from the IDE's preferences.txt
PREFERENCESFILE := $(firstword $(wildcard \
	$(HOME)/.arduino15/preferences.txt $(HOME)/Library/Arduino/preferences.txt))
ifneq "$(PREFERENCESFILE)" ""
readpreferencesparam = $(shell sed -ne "s/^$(1)=\(.*\)/\1/p" $(PREFERENCESFILE))
SKETCHBOOKDIR := $(call readpreferencesparam,sketchbook.path)
endif

# invalid board?
ifeq "$(BOARD_BUILD_CORE)" ""
ifneq "$(MAKECMDGOALS)" "boards"
ifneq "$(MAKECMDGOALS)" "clean"
$(error BOARD is invalid.  Type 'make boards' to see possible values)
endif
endif
endif

# auto mode?
INOFILE := $(wildcard *.ino *.pde)
ifdef INOFILE
ifneq "$(words $(INOFILE))" "1"
$(error There is more than one .pde or .ino file in this directory!)
endif

# automatically determine sources and targeet
TARGET := $(basename $(INOFILE))
SOURCES := $(INOFILE) \
	$(wildcard *.c *.cc *.cpp *.C) \
	$(wildcard $(addprefix util/, *.c *.cc *.cpp *.C)) \
	$(wildcard $(addprefix utility/, *.c *.cc *.cpp *.C))

# automatically determine included libraries
LIBRARIES := $(filter $(notdir $(wildcard $(addsuffix /*, $(LIBRARYPATH)))), \
	$(shell sed -ne "s/^ *\# *include *[<\"]\(.*\)\.h[>\"]/\1/p" $(SOURCES)))

endif

# software
findsoftware = $(firstword $(wildcard $(addsuffix /$(1), $(ARMTOOLSPATH))))
CC := $(call findsoftware,arm-none-eabi-gcc)
CXX := $(call findsoftware,arm-none-eabi-g++)
LD := $(call findsoftware,arm-none-eabi-ld)
AR := $(call findsoftware,arm-none-eabi-gcc-ar)
OBJCOPY := $(call findsoftware,arm-none-eabi-objcopy)
OBJDUMP := $(call findsoftware,arm-none-eabi-objdump)
ARMSIZE := $(call findsoftware,arm-none-eabi-size)
TEENSYPOST := $(call findsoftware,teensy_post_compile)

# directories
ARDUINOCOREDIR := $(ARDUINODIR)/hardware/teensy/avr/cores/${BOARD_BUILD_CORE}
LIBRARYDIRS := $(foreach lib, $(LIBRARIES), \
	$(firstword $(wildcard $(addsuffix /$(lib), $(LIBRARYPATH)))))
LIBRARYDIRS += $(addsuffix /utility, $(LIBRARYDIRS))
LIBRARYDIRS += $(addsuffix /src, $(LIBRARYDIRS))
LIBRARYDIRS += $(addsuffix /src/utility, $(LIBRARYDIRS))

# files
TARGET := $(if $(TARGET),$(TARGET),a.out)
OBJECTS := $(addsuffix .o, $(basename $(SOURCES)))
DEPFILES := $(patsubst %, .dep/%.dep, $(SOURCES))
ARDUINOLIB := .lib/arduino.a
ARDUINOLIBOBJS := $(foreach dir, $(ARDUINOCOREDIR) $(LIBRARYDIRS), \
	$(patsubst %, .lib/%.o, $(wildcard $(addprefix $(dir)/, *.c *.cpp))))

# flags
CPPFLAGS += -Os -Wall -ffunction-sections -fdata-sections -nostdlib
CPPFLAGS += -DARDUINO=$(ARDUINOCONST) -DF_CPU=$(BOARD_BUILD_FCPU) -D$(BOARD_BUILD_USBTYPE) -DLAYOUT_$(LAYOUT)
CPPFLAGS += $(BOARD_BUILD_FLAGS_CPU)
CPPFLAGS += $(BOARD_BUILD_FLAGS_DEFS)
CPPFLAGS += $(BOARD_BUILD_FLAGS_CPP)
CPPFLAGS += -I. -Iutil -Iutility -I $(ARDUINOCOREDIR)
CPPFLAGS += $(addprefix -I , $(LIBRARYDIRS))
CPPDEPFLAGS = -MMD -MP -MF .dep/$<.dep
CPPINOFLAGS := -x c++ -include $(ARDUINOCOREDIR)/Arduino.h
LINKFLAGS += -Os
LINKFLAGS += $(shell echo '$(BOARD_BUILD_FLAGS_LD)' | sed -e "s@{extra\.time\.local}@`date +%s`@" | sed -e "s@{build\.core\.path}@$(ARDUINOCOREDIR)@")
LINKFLAGS += $(BOARD_BUILD_FLAGS_CPU)
LINKFLAGS += $(BOARD_BUILD_FLAGS_LIBS)

# figure out which arg to use with stty (for OS X, GNU and busybox stty)
STTYFARG := $(shell stty --help 2>&1 | \
	grep -q 'illegal option' && echo -f || echo -F)

# include dependencies
ifneq "$(MAKECMDGOALS)" "clean"
-include $(DEPFILES)
endif

# default rule
.DEFAULT_GOAL := all

#_______________________________________________________________________________
#                                                                          RULES

.PHONY:	all target upload clean boards monitor size

all: target

target: $(TARGET).hex

upload: target
	@echo "Uploading to board..."
	$(TEENSYPOST) -file=$(TARGET) -path=$(PWD) -tools=$(ARDUINODIR)/hardware/tools/

clean:
	rm -f $(OBJECTS)
	rm -f $(TARGET).elf $(TARGET).hex $(ARDUINOLIB) *~
	rm -rf .lib .dep

boards:
	@echo "Available values for BOARD:"
	@sed -nEe '/^#/d; /^[^.]+\.name=/p' $(BOARDSFILE) | \
		sed -Ee 's/([^.]+)\.name=(.*)/\1            \2/' \
			-e 's/(.{12}) *(.*)/\1 \2/'

speeds:
	@echo "Available values for SPEED for '$(BOARD)':"
	@sed -nEe "s/^$(BOARD)\.menu\.speed\.([^.=]*)=(.*)/\1            \2/p" $(BOARDSFILE) \
		-e 's/(.{12}) *(.*)/\1 \2/'
	@for cpu in $(BOARD_MENU_SPEEDS); do echo $$cpu; done

monitor:
	stty raw $(SERIALBAUD) igncr hupcl -echo $(STTYFARG) $(SERIALDEV)
	@echo "Connected. Press Ctrl+D to close the monitor."
	@sh -c 'trap "kill %1" INT; cat -v $(SERIALDEV) & cat >$(SERIALDEV); kill %1'
	stty sane $(STTYFARG) $(SERIALDEV)

size: $(TARGET).elf
	echo && $(ARMSIZE) $(TARGET).elf

# building the target

$(TARGET).eep: $(TARGET).elf
	$(OBJCOPY) -O ihex -j .eeprom --set-section-flags=.eeprom=alloc,load --no-change-warnings --change-section-lma .eeprom=0 $< $@

$(TARGET).hex: $(TARGET).elf
	$(OBJCOPY) -O ihex -R .eeprom $< $@

$(TARGET).sym: $(TARGET).elf
	$(OBJDUMP) -t -C $< >$@

.INTERMEDIATE: $(TARGET).elf

$(TARGET).elf: $(ARDUINOLIB) $(OBJECTS)
	$(CC) $(LINKFLAGS) $(OBJECTS) $(ARDUINOLIB) -lm -o $@

%.o: %.c
	mkdir -p .dep/$(dir $<)
	$(COMPILE.c) $(CPPDEPFLAGS) -o $@ $<

%.o: %.cpp
	mkdir -p .dep/$(dir $<)
	$(COMPILE.cpp) $(CPPDEPFLAGS) -o $@ $<

%.o: %.cc
	mkdir -p .dep/$(dir $<)
	$(COMPILE.cpp) $(CPPDEPFLAGS) -o $@ $<

%.o: %.C
	mkdir -p .dep/$(dir $<)
	$(COMPILE.cpp) $(CPPDEPFLAGS) -o $@ $<

%.o: %.ino
	mkdir -p .dep/$(dir $<)
	$(COMPILE.cpp) $(CPPDEPFLAGS) -o $@ $(CPPINOFLAGS) $<

%.o: %.pde
	mkdir -p .dep/$(dir $<)
	$(COMPILE.cpp) $(CPPDEPFLAGS) -o $@ $(CPPINOFLAGS) $<

# building the arduino library

$(ARDUINOLIB): $(ARDUINOLIBOBJS)
	$(AR) rcs $@ $?

.lib/%.c.o: %.c
	mkdir -p $(dir $@)
	$(COMPILE.c) -o $@ $<

.lib/%.cpp.o: %.cpp
	mkdir -p $(dir $@)
	$(COMPILE.cpp) -o $@ $<

.lib/%.cc.o: %.cc
	mkdir -p $(dir $@)
	$(COMPILE.cpp) -o $@ $<

.lib/%.C.o: %.C
	mkdir -p $(dir $@)
	$(COMPILE.cpp) -o $@ $<

# Local Variables:
# mode: makefile
# tab-width: 4
# End:
