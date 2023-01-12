# house2_luna_program
Small Arduino program to change address of TF-luna

## Update:
For an updated version look at:
https://gitlab.com/jhaand/house2 and the program `./tools/luna_config`
You can run it with the command: `pio run -t upload -t monitor` 

## Description

For my current project I will use 5 TF-luna's next to each other. It was
quite hard to find an example that showed how to reprogram a Benewake
TF-Luna using I2C. So you can give each TF-Luna a different address and
put them all on the same I2C bus. This small program allows you to
diagnose a single TF-luna via I2C, reprogram the address and do a Hard
reset if that's necessary. 

The TF-luna connects to a NodeMCU with and ESP32S. I use PlatformIO to
program the ESP32. 

It uses the TFLI2C library by Bud Ryerson. (@budryerson)
You can find it here: https://github.com/budryerson/TFLuna-I2C

