arduino-cli compile --fqbn arduino:avr:uno . &&
mv ./build/arduino.avr.uno/code.ino.hex ./build/arduino.avr.uno/..ino.hex &&
arduino-cli upload -p /dev/ttyUSB0 --fqbn arduino:avr:uno .
