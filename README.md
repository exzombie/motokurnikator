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
* A board specification to allow the ATtiny44 microcontroller to be
  used with the Arduino framework. Thanks to David A. Mellis for
  [this library][3].
* A library to control microcontroller pins with much less overhead
  than Arduino's digitalWrite(). Not that speed matters for this
  project, it serves as a demonstration.
* Build scripts to allow compiling and uploading using the Arduino IDE
  without actually running the IDE. This makes the package easy to use
  both for those who like using the IDE and those who don't.

[2]: http://www.kicad-pcb.org/ "KiCAD website"
[3]: https://github.com/damellis/attiny "ATtiny Arduino library"

## How to build the firmware? ##

Make sure you have Arduino IDE version 1.5 or later. Open the
preferences.txt file and set the custom_cpu field to the actual
variant you use (either 84 or 44).

If you like using the IDE, use the runide.sh script to start it. This
will make it use the package-provided preferences.txt, libraries and
build directory. It will pollute the preferences.txt by dumping all
the IDE settings there, but since you're using the IDE, this doesn't
matter to you.

If you don't like the IDE, use the compile.sh and upload.sh
scripts. The latter should be modified for the type of programmer you
use. At the time of writing, I am using the [Open Programmer][4] and
the script contains a commented line using that. The uncommented line
uses an Arduino with the ArduinoISP sketch as the programmer. Select
the correct port for that in preferences.txt.

The variables and preprocessor macros at the top of the sketch define
which pin belongs to which switch, the motor direction and the amount
of hysteresis. Adjust to match your particular circuit.

[4]: http://openprog.altervista.org/ "Open Programmer"

## What about the circuit? ##

Okay, this is how the whole thing works. The light level is measured
through the photoresistor (R3 on the circuit schematic). The RV1
potentiometer decides the ADC reference voltage. If the light level is
at or above the reference voltage, it is daytime and the door is
opened. If the light level drops below a certain amount (about 10%
below reference by default), it is nighttime and the door is
closed. Those 10% represent a hysteresis which ensures that the door
doesn't try to both open and close at the same time in twilight. There
is an indicator LED (D1) which is dark during nighttime, lit during
daytime, and blinking when the voltage is in the hysteresis
range. This makes it easier to adjust the potentiometer (and possibly
decide to increase the hysteresis).

The "open" and "closed" positions are defined by the two end switches,
which are triggered by the door itself (see the [image][1]). There is
a manual override switch so you can open or close the door
anytime. The door is mounted to the frame via a pair of rails. The
stepper motor turns a threaded rod which moves the door along the
rails.

The circuit schematic is deliberately vague because the actual parts
used don't matter. Have a different voltage regulator on hand? Use
that then. Don't have a 10k pot? Use 5k if you want. And so on. The
only part that matters is the microcontroller, unless you modify the
software quite a bit. There are a few points to take into
consideration though.

It doesn't matter whether your stepper motor is unipolar or bipolar,
just make sure you have the correct driver chip. The circuit is made
to use four wires, but if you have a two wire motor, you only have to
replace the line in the program which defines the motor with the
version for two wires (see [this page][5]). Note that the circuit
schematic doesn't define any power lines to the motor. Depending on
the motor and the driver chip, you may or may not need to power it
through the same regulator as the circuit itself. In our case, the
motor is powered from the 12V battery directly, while the circuit is
powered through the 5V regulator. The GND line is shared, of course.

A photoresistor as it's wired in the schematic can be overly
sensitive. First, ensure that you are measuring the light you actually
want: put the photoresistor in an enclosed space so that it is only
watching the diffuse light scattered by its surroundings. Make sure it
can't be affected by the sky, shadows and such. Next, pick a
reasonable value for R2. It must **not** be so low as to draw too much
current, even when R3 is fully conducting. What constitutes a
reasonable value depends on what kind of photoresistor you have. Play
around. You may want to increase the size of the hysteresis in
software. If you still can't make it work, there's nothing stopping
you from ripping out the photoresistor and putting in something
different, like a phototransistor or a more sophisticated circuit.

[5]: http://www.arduino.cc/en/Reference/Stepper "Arduino stepper library"

## Who put this together? ##

The circut, sketch and the FastPins library are written by Jure Varlec
<jure@varlec.si> and are published under the GPL3 licence. External
contributions are licenced by their authors; they are credited above
and in the code.