MCU=attiny13
F_CPU=1200000
CC=avr-gcc
OBJCPY=avr-objcopy
CFLAGS=-std=c99 -Wall -g -Os -mmcu=${MCU} -DF_CPU=${F_CPU} -I.
TARGET=main
SRCS=main.c
DUDECFG= -p ${MCU} -C avrdude_gpio.conf -c pi_3

test:
	avrdude ${DUDECFG} -F -v

package:
	${CC} ${CFLAGS} -o ${TARGET}.bin ${SRCS}
	avr-size -C ${TARGET}.bin
	${OBJCPY} -j .text -j.data -O ihex ${TARGET}.bin ${TARGET}.hex

flash:
	avrdude ${DUDECFG} flash:w:${TARGET}.hex:i -F

clean:
	rm -f *.bin *.hex

