# Nazwa pliku z funkcj¹ main() - BEZ ROZSZERZENIA!
TARG = main

 

# Lista plików, których zmiana powoduje przebudowanie projektu
 
SRC_FOLDER = ../../03_SRC
LIB_FOLDER = ../../04_LIB
OBJ_FOLDER = ../../01_BIN/02_OBJ
OUT_FOLDER = ../../01_BIN/01_HEX

INC_FOLDER = ../../03_SRC/
RKLIB_SRC_FOLDER = ../../04_LIB/RKLib/07_SW/03_SRC
RKLIB_OBJ_FOLDER = ../../04_LIB/RKLib/07_SW/01_BIN/02_OBJ
RKLIB_OUT_FOLDER = ../../04_LIB/RKLib/07_SW/01_BIN/01_HEX

CONFIG = $(SRC_FOLDER)/appl/config.h

TARGET = $(OUT_FOLDER)/$(TARG)
#TARGET = $(TARG)

# Lista plików Ÿród³owych w jêzyku C
SRCF = appl/main.c
	#SRCF += fbus/n61sms.c

	#SRCF += drv/sw_SPI_drv.c
	#SRCF += drv/Timers.c
SRCF += drv/encoder.c
SRCF += drv/push_button.c
SRCF += drv/lcd.c
SRCF += drv/AC_line.c
SRCF += inverter/pwm.c
SRCF += inverter/sin_table.c
SRCF += inverter/sin_out.c
SRCF += appl/scheduler.c
SRCF += menu/menu_gen.c
SRCF += menu/menu_1.c
SRCF += NVM/nvm.c
SRCF += drv/DS18B20/ds18x20.c
SRCF += drv/OneWire/onewire.c
SRCF += drv/Delay/delay.c
SRCF += Temperature/Temperature.c

	#SRCF += drv/uart.c
	#SRC += Interrupts.c
	#SRC += libs/lcd/lcd.c
	#SRC += PWM_timer1.c
    #SRC += sms.c


LIBRARIES = $(LIB_FOLDER)/RKLib/07_SW/01_BIN/01_HEX/rklib.a

SRC = $(SRCF:%=$(SRC_FOLDER)/%)
 
# Lista plików Ÿród³owych w asemblerze (rozszerzenie S - DU¯E S !)
ASRC = 

# typ mikrokontrolera
#MCU = at90s8515
#MCU = atmega163
#MCU = atmega128  
#MCU = atmega162
#MCU = at90s2313
MCU = atmega32

# Nazwa_plytki
BOARD = Digit

# Format pliku wyjœciowego (srec, ihex)
FORMAT = ihex

# Poziom optymalizacji (0, 1, 2, 3, s) 
# (Uwaga: 3 nie zawsze jest najlepszym wyborem)
OPT = 0

# Katalog z bibliotekami u¿ytkownika
#USRLIB	= $(LIB_FOLDER)/RKLib/07_SW/03_SRC

USRLIB	= ../../04_LIB/RKLib/07_SW/03_SRC

# Lista plików Ÿród³owych bibliotek w jêzyku C
SRCLIB =
	#include $(USRLIB)/adc/sources
	#include $(USRLIB)/adc_pwm/sources
include $(USRLIB)/conv/sources
	#include $(USRLIB)/debug/sources
#include $(USRLIB)/delay/sources
#include $(USRLIB)/ds18x20/sources
	#include $(USRLIB)/i2c/sources
	#include $(USRLIB)/i2c_ee/sources
	#include $(USRLIB)/kbd/sources
	#include $(USRLIB)/lcd/sources
	#include $(USRLIB)/led7seg/sources
#include $(USRLIB)/onewire/sources
	#include $(USRLIB)/pcf8573/sources
	#include $(USRLIB)/ps2kbd/sources
	#include $(USRLIB)/ps2mouse/sources
	#include $(USRLIB)/remote/sources
	#include $(USRLIB)/rtc/sources
	#include $(USRLIB)/spi/sources
	#include $(USRLIB)/twi/sources
	#include $(USRLIB)/uart/sources
	#include $(USRLIB)/uart0/sources
	#include $(USRLIB)/uartsw/sources
	#include $(USRLIB)/usbdrv/sources

# Dodatkowe biblioteki
#
# Minimalna wersja printf
#LDFLAGS += -Wl,-u,vfprintf -lprintf_min
#
# Zmiennoprzecinkowa wersja printf (wymaga biblioteki matematycznej)
#LDFLAGS += -Wl,-u,vfprintf -lprintf_flt
#
# Biblioteka matematyczna
#LDFLAGS += -lm


## Library Directories
LIBDIRS = -L  "default" 

## Libraries
#LIBS += -lmoja 

LDFLAGS += $(LIBDIRS) $(LIBS)

include avr_make
