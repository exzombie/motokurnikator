# motokurnikator #

Automated chicken coop door!

## What is this? ##

motokurnikator is a simple system that makes sure your chickens are
safe at night. It features a light sensor which determines when it is
too dark for chickens to see and closes the door to their coop. It is
simple to build, both in terms of electronics and the door itself. See
[the image][1] for an idea.

[1]: door_mechanism.jpg "Mechanized chichen coop door"

## What is in this package? ##

* [An image][1] that should give you an idea how to build the
  door. Adapt to whatever materials you have on hand.
* A circuit diagram in [KiCAD][2] format. No PCB design though, the
  circuit is simple enough to build on a perfboard.
* An Arduino sketch to drive the whole thing using an ATtiny44 or
  ATtiny84 microcontroller.
* A library to control microcontroller pins with much less overhead
  than Arduino's digitalWrite(). Not that speed matters for this
  project, it serves as a demonstration.

[2]: http://www.kicad-pcb.org/ "KiCAD website"

## How to build the firmware? ##

Make sure you have the latest version of the Arduino IDE. Install
[ATTinyCore][3]; it is available in the IDE's board manager. Select the
Attiny84/44/24 board and use the internal clock (8 or 1 MHz; don't forget to
"Burn bootloader" after any clock change) and clockwise pin mapping.

The variables and preprocessor macros at the top of the sketch define which pin
belongs to which switch, potentiometer and motor pin. They also define the
motor direction and timeouts. Adjust to match your particular circuit. The
timeout setting should be set to just a bit longer than is necessary for the
door to fully open or close; it prevents the motor from draining your battery
(if you have one) in case something breaks.

[3]: https://github.com/SpenceKonde/ATTinyCore "ATtiny Arduino library"

## What about the circuit? ##

Okay, this is how the whole thing works. The light level is measured through
the photoresistor (R3 on the circuit schematic). The RV1 and RV2 potentiometers
decide choose the light level at which the door will open or close; one of the
potentiometers is for opening and the other for closing because the morning and
evening light levels will usually be different. If the light level is at or
above the chosen daytime level, it is daytime and the door is opened. If the
light level drops below the chosen nighttime level, it is nighttime and the
door is closed.

The "open" and "closed" positions of the door are defined by the two end
switches, which are triggered by the door itself (see the [image][1]). There is
a manual override switch so you can open or close the door anytime. The door is
mounted to the frame via a pair of rails. The stepper motor turns a threaded
rod which moves the door along the rails. This approach was chosen because a
stepper motor was on hand, but has turned out to be unreliable. Support for
steppers was removed and it is recommended to use a normal DC motor through an
H-bridge to drive a winch, the door being raised and lowered by a cable.

The circuit schematic is deliberately vague because the actual parts
used don't matter. Have a different voltage regulator on hand? Use
that then. Don't have a 10k pot? Use 5k if you want. And so on. The
only part that matters is the microcontroller, unless you modify the
software quite a bit. There are a few points to take into
consideration though.

A photoresistor as it's wired in the schematic can be overly sensitive. First,
ensure that you are measuring the light you actually want: put the
photoresistor in an enclosed space so that it is only watching the diffuse
light scattered by its surroundings. Make sure it can't be affected by the sky,
direct sunlight, shadows and such. Next, pick a reasonable value for R2. It
must **not** be so low as to draw too much current, even when R3 (the
photoresistor) is fully conducting. What constitutes a reasonable value depends
on what kind of photoresistor you have. Play around. It helps to use a
voltmeter to see how the choice of R2 affects the voltage drop across R3 and to
choose the right levels with RV1 and RV2. If you still can't make it work,
there's nothing stopping you from ripping out the photoresistor and putting in
something different, like a phototransistor or a more sophisticated circuit.

## Who put this together? ##

The circut, sketch and the FastPins library are written by Jure Varlec
<jure@varlec.si> and are published under the GPL3 licence. External
contributions are licenced by their authors; they are credited above
and in the code.
