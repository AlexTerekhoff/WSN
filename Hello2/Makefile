#
# Makefile для msp430
#
######################################################################################
#   http://sourceforge.net/apps/mediawiki/mspgcc/index.php?title=Example:Makefile    #
######################################################################################
#
# Команда (без параметров)
# $ make
# построит проект полностью.
#
# Команда
# $ make clean
# удалит все файлы за исключением исходников и файла Makefile.
#
# Вы должны задать свои значения параметров для TARGET, MCU и SOURCES в проекте.
#
# TARGET -- это имя исполняемого файла, который будет создан.
#
# В процессе построения проекта также будут созданы файлы $(TARGET).elf
# $(TARGET).hex $(TARGET).txt и $(TARGET).map.
#
# Файл .TXT используется для загрузки через BSL, файл .ELF может быть использован
# для работы через JTAG.
#
######################################################################################
#
# 
TARGET     = hello2
MCU        = msp430f5438
#
# Перечислите здесь все файлы исходников, например, если у вас есть файл foo.c тогда
# укажите его здесь
SOURCES = hello2.c leds.c mclk.c
#
# Подключите все include, находящиеся в include-директориях
INCLUDES = -IInclude
#
LIBS = ../include/hal.lib /usr/local/lib/gcc/msp430/4.6.3/libcrt0.a
# Add or subtract whatever MSPGCC flags you want. There are plenty more
# Добавьте и удалите необходимые флаги MSPGCC. На самом деле их может быть очень много
#######################################################################################
CFLAGS   = -mmcu=$(MCU) -g -Os -Wall -Wunused $(INCLUDES)   
ASFLAGS  = -mmcu=$(MCU) -x assembler-with-cpp -Wa,-gstabs
LDFLAGS  = -mmcu=$(MCU) -Wl,-Map=$(TARGET).map
########################################################################################
CC       = msp430-gcc
LD       = msp430-ld
AR       = msp430-ar
AS       = msp430-gcc
GASP     = msp430-gasp
NM       = msp430-nm
OBJCOPY  = msp430-objcopy
RANLIB   = msp430-ranlib
STRIP    = msp430-strip
SIZE     = msp430-size
READELF  = msp430-readelf
MAKETXT  = srec_cat
CP       = cp -p
RM       = rm -f
MV       = mv
########################################################################################
# файлы, которые имеют зависимости
DEPEND = $(SOURCES:.c=.d)
#
# все объектные файлы
OBJECTS = $(SOURCES:.c=.o)
#
all: $(TARGET).elf $(TARGET).hex $(TARGET).txt 
$(TARGET).elf: $(OBJECTS)
	echo "Линковка $@"
	$(CC) $(OBJECTS) $(LDFLAGS) $(LIBS) -o $@
	echo
	echo ">>>> Размер программы <<<<"
	$(SIZE) $(TARGET).elf
	echo
%.hex: %.elf
	$(OBJCOPY) -O ihex $< $@
%.txt: %.hex
	$(MAKETXT) -O $@ -TITXT $< -I
%.o: %.c
	echo "Компиляция $<"
	$(CC) -c $(CFLAGS) -o $@ $<
# правило для создания ассемблерного листинга (чтобы посмотреть полученный код)
%.lst: %.c
	$(CC) -c $(CFLAGS) -Wa,-anlhd $< > $@
# подключим зависимости, в противном случае мы все сотрем и забудем о них.
ifneq ($(MAKECMDGOALS), clean)
-include $(DEPEND)
endif
# Поскольку некоторые зависимости могут быть нашими, то следует рассмотреть файл
# зависимостей белее внимательно
# (в противном случае используйте -MM вместо -M)
%.d: %.c
	echo "Создадим зависимости $@ из $<"
	$(CC) -M ${CFLAGS} $< >$@
.SILENT:
.PHONY:	clean
clean:
	-$(RM) $(OBJECTS)
	-$(RM) $(TARGET).o $(TARGET).elf $(TARGET).hex $(TARGET).map $(TARGET).txt
#	-$(RM) $(SOURCES:.c=.lst)
	-$(RM) $(DEPEND)
