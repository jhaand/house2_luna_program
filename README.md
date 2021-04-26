# house2_luna_program
Small Arduino program to change address of TF-luna

For my current project I will use 5 TF-luna's next to each other. It was
quite hard to find an example that showed how to program an TF-Luna
using I2C with a changed address. This small program allows you to
diagnose a single TF-luna via I2C, reprogram the address and do a Hard
reset if that's necessary. 

The TF-luna connects to a NodeMCU with and ESP32S. I use PlatformIO to
program the ESP32. 

It uses the TFLI2C library by Bud Ryerson. 
You can find it here: https://github.com/budryerson/TFLuna-I2C


