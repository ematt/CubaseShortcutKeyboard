# CubaseShortcutKeyboard

## Introduction ##

This is a simple device designed to be a shortcut for common action in Cubase. It can:
- control the system volume
- mute/unmute the sound
- play/stop
- start recording
- toggle transport cycle
- mark the current selected track as solo

![picture of the device](https://raw.githubusercontent.com/ematt/CubaseShortcutKeyboard/master/pictures/IMG_20181126_214617348.jpg)

## How it works ##

Using the v-usb library, it emulates a keyboard and a multimedia device.
The rotary encoder controls the volume and it can mute the sound when the encoder's switch is pressed.
Also it has 6 aditional slots for buttons that can be linked to a specific shortcut used by Cubase.
By default it can play/stop, start recording, Toggle transport cycle and mark the selected track  as solo.

## Directories in this repository ##

*CubaseRemote* - Atmel studio project

*eagle_Cubase USB Remote v1.0* - schematics and eagle file

*hex* - generated hex files

*pictures* - pictures of the electronics board

## Requirements ##

**ATmega8** or any other similar AVR microcontroller. If using a different model, some minor fine-tuning of the firmware might be neded. 

**Other electronic components**. See the circuit schematic.

The required software environment:

Atmel Studio 6

## How to build this project ##

1. Use Atmel studio to edit and build the project.
2. Flash with your favorite programmer
3. Profit

## TODO ##

1. Make an ergonomic case
