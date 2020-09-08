arduino-cli compile --fqbn arduino:avr:uno . &&
avrdude -v -V -patmega328p -carduino -P/dev/ttyUSB0 -D -Uflash:w:./build/arduino.avr.uno/code.ino.hex:i
