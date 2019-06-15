# rnd-for-mp3

install dependency
```
sudo apt-get install gcc-avr avr-libc avrdude
```
RPI GPIO 
```
RESET = 17
SCK   = 11
MOSI  = 10
MISO  = 9
```
unexport gpio if busy
```
echo 17 > /sys/class/gpio/unexport
```
