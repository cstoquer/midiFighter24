# Midi Fighter 24

Firmware and tools for Arduino based 24 buttons midi controller

see it in action [here](https://www.youtube.com/watch?v=FosVgqp6nJg)

# Setup Arduino to send Midi via USB

Install DualMocoLUFA on the Arduino 8U2's chip
(look inside the `dualMocoLUFA` folder in this repository to find code and instructions)

# Schematics

## In a nutshell

 - the 24 Arcade buttons are read by 3 `74HC165` shift registers connected on Arduino's pin 2(LOAD), 3(CLOCK), and 4, 5, 6 (shift registers DATA) 
 - 2 x 7 led digit display are controlled by 2 `74HC595` shift registers connected on Arduino's pin 9(LATCH), 8(CLOCK) and 10(DATA). These shift registers are set in series.
 - two menu button on the side are connected to Arduino's pin 11 and 12 (with a pullup resistor)

## Buttons module
Push buttons are connected to the arduino by using `74HC165` shift registers.

Here the schematic of one module for 8 buttons. I used 2 5x1 headers for the connection to the buttons. In the schematic, this is the labels `A`, `B`, `C`, `D` and `E`, `F`, `G`, `H`. The last pin of each headers are connected to `VCC`. Pull-up resistors are 10KΩ.
![button1](https://cloud.githubusercontent.com/assets/2462139/19406542/e7c0f1be-92c2-11e6-920d-a341865de7c5.jpg)

A group of 4 buttons are connected to one header like this:
![button_connect](https://cloud.githubusercontent.com/assets/2462139/19406545/f8fe480a-92c2-11e6-8eab-4497147f179a.jpg)

This module is duplicated for each group of push 8 buttons.
For 24 buttons, 3 of these modules are connected together like this:
![buttons2](https://cloud.githubusercontent.com/assets/2462139/19406543/f2f55520-92c2-11e6-8f8c-428234ba1f08.JPG)

And here are some photos of how I implemented this module using perf boards.
![button module 1](https://cloud.githubusercontent.com/assets/2462139/19406548/01a3aaf4-92c3-11e6-976f-0342468acd63.JPG)
![button module under](https://cloud.githubusercontent.com/assets/2462139/19406550/06a73f70-92c3-11e6-94d2-ddd80137cba9.JPG)

## Display module
This module is optional, and is for displaying information using 2 seven segments LED.
It is using `74HC595` shift registers to control the 16 LED using 3 pins of the Arduino. 16 LED consume quite a lot of power for the Arduino, so in the code, I take care of turning ED off after a delay.

If I had to put more LED, I would use a `MAX7219` LED matrix controller is this chip use way less power.

This is the schematic for one `74HC595`. Resistors are 4.7KΩ.
![display1](https://cloud.githubusercontent.com/assets/2462139/19406554/15fb5ef2-92c3-11e6-9b94-cd431faec68f.jpg)
Here is how you connect the seven segments display
![7segmets](https://cloud.githubusercontent.com/assets/2462139/19406556/1ef6ce88-92c3-11e6-875f-9897b7435bdc.JPG)
2 of these modules are daisy chained together like so:
![display2](https://cloud.githubusercontent.com/assets/2462139/19406555/1a168bd8-92c3-11e6-89ca-1d1284efc49e.JPG)

Some photos of how I implemented this module using perf boards:
![display module 5](https://cloud.githubusercontent.com/assets/2462139/19406573/68eb7b9c-92c3-11e6-96cb-42bfdfc0f404.JPG)
![display module 4](https://cloud.githubusercontent.com/assets/2462139/19406574/6d1817a2-92c3-11e6-92b1-232b7bd2afd0.JPG)
![display module 2](https://cloud.githubusercontent.com/assets/2462139/19406575/753c9b4c-92c3-11e6-90c1-774cb24555da.JPG)


## Connection to the Arduino
Finally, here is how the two modules are connected to the Arduino
![arduino](https://cloud.githubusercontent.com/assets/2462139/19406562/34e74b1e-92c3-11e6-8588-e99ed15ccaef.JPG)

Some more photos:
![both modules](https://cloud.githubusercontent.com/assets/2462139/19406576/7f4d8f9c-92c3-11e6-8f71-dd0cbaf506c4.JPG)

![inside](https://cloud.githubusercontent.com/assets/2462139/19406563/3fc1a296-92c3-11e6-9dc8-861a3707acb7.JPG)

![inside empty](https://cloud.githubusercontent.com/assets/2462139/19406567/4eb5b3b4-92c3-11e6-9a1c-7389d86cff76.JPG)


# More photos

![front](https://cloud.githubusercontent.com/assets/2462139/13602789/2c77cb92-e57c-11e5-8c30-129de1032203.JPG)

![left](https://cloud.githubusercontent.com/assets/2462139/13602793/3979264c-e57c-11e5-9047-d57a520f9afd.JPG)

![back](https://cloud.githubusercontent.com/assets/2462139/13602795/3fc23138-e57c-11e5-8a2c-f87e0d586652.JPG)

![right](https://cloud.githubusercontent.com/assets/2462139/13602798/446ac42a-e57c-11e5-96e5-483d790f61e0.JPG)

![shiftregisters](https://cloud.githubusercontent.com/assets/2462139/13602802/4a83c0d2-e57c-11e5-9add-57483b9590d8.JPG)
