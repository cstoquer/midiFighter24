# Midi Fighter 24

Firmware and tools for Arduino based 24 buttons midi controller

see it in action [here](https://www.youtube.com/watch?v=FosVgqp6nJg)

## Setup Arduino to send Midi via USB

Install DualMocoLUFA on the Arduino 8U2's chip
(look inside the `dualMocoLUFA` folder in this repository to find code and instructions)

## Hardware

no schematic yet, sorry.

### In a nutshell

 - the 24 Arcade buttons are read by 3 `74HC165` shift registers connected on Arduino's pin 2(LOAD), 3(CLOCK), and 4, 5, 6 (shift registers DATA) 
 - 2 x 7 led digit display are controlled by 2 `74HC595` shift registers connected on Arduino's pin 9(LATCH), 8(CLOCK) and 10(DATA). These shift registers are set in series.
 - two menu button on the side are connected to Arduino's pin 11 and 12 (with a pullup resistor)

### Photos

![front](https://cloud.githubusercontent.com/assets/2462139/13602789/2c77cb92-e57c-11e5-8c30-129de1032203.JPG)

![left](https://cloud.githubusercontent.com/assets/2462139/13602793/3979264c-e57c-11e5-9047-d57a520f9afd.JPG)

![back](https://cloud.githubusercontent.com/assets/2462139/13602795/3fc23138-e57c-11e5-8a2c-f87e0d586652.JPG)

![right](https://cloud.githubusercontent.com/assets/2462139/13602798/446ac42a-e57c-11e5-96e5-483d790f61e0.JPG)

![shiftregisters](https://cloud.githubusercontent.com/assets/2462139/13602802/4a83c0d2-e57c-11e5-9add-57483b9590d8.JPG)
