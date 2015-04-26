/*
    motokurnikator, an automated chicken coop door controller.
    Copyright (C) 2015 Jure Varlec <jure@varlec.si>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <FastPin.h>
#include <Stepper.h>

static const FastPin<2> endSwOpen;
static const FastPin<3> endSwClosed;
static const FastPin<9> manualOpen;
static const FastPin<10> manualClose;
static const FastPin<8> indicator;

static const byte stepsPerRot = 48;
static const byte stepperRpm = 500;
static const FastPin<7> motor0;
static const FastPin<5> motor1;
static const FastPin<6> motor2;
static const FastPin<4> motor3;
static Stepper motor(stepsPerRot, motor0, motor1, motor2, motor3);

#define lightLevelPin A1
#define standbyDelayMs 100
#define bootDelayMs 100
#define closeDirection (-1)
#define openDirection (1)
#define lightLevelHysteresis 100
#define lightLevelMargin 10

enum class Mode: byte {
    open, closed, opening, closing
};

static Mode mode = Mode::closing;

static unsigned long lastMillis = 0;

#include "functions.h"

void setup()
{
    pinMode(lightLevelPin, INPUT);
    digitalWrite(lightLevelPin, LOW);
    analogReference(EXTERNAL);
    for_pin([](const FastAnyPin p){ p.input(); p.high(); },
	    endSwOpen, endSwClosed, manualOpen, manualClose);
    indicator.high();
    indicator.output();
    motor.setSpeed(stepperRpm);
    delay(bootDelayMs); // Wait after changing analog reference.
}

void loop()
{
    int lightLevel;
    bool daytime, nighttime;
    bool override = !manualOpen.get() || !manualClose.get();

    if (!endSwOpen.get()) {
	changeMode(Mode::open);
    } else if (!endSwClosed.get()) {
	changeMode(Mode::closed);
    }

    if (!manualOpen.get() && mode != Mode::open) {
	changeMode(Mode::opening);
    } else if (!manualClose.get() && mode != Mode::closed) {
	changeMode(Mode::closing);
    } else {
        lightLevel = analogRead(lightLevelPin);
	nighttime = lightLevel < (1024 - lightLevelHysteresis);
	daytime = lightLevel > (1024 - lightLevelMargin);
	if (nighttime) {
	    indicator.low();
	} else if (daytime) {
	    indicator.high();
	} else {
	    unsigned long ms = millis();
	    if (ms - lastMillis > 100) {
	        indicator.toggle();
	        lastMillis = ms;
	    }
	}
    }

    switch (mode) {
    case Mode::open:
        if (nighttime && !override) {
	    changeMode(Mode::closing);
	    moveMotor(closeDirection);
        } else {
	    stopMotor();
	    delay(standbyDelayMs);
        }
	break;
    case Mode::closed:
        if (daytime && !override) {
	    changeMode(Mode::opening);
	    moveMotor(openDirection);
        } else {
	    stopMotor();
	    delay(standbyDelayMs);
        }
	break;
    case Mode::opening:
        moveMotor(openDirection);
        if (nighttime && !override)
	    changeMode(Mode::closing);
        break;
    case Mode::closing:
        moveMotor(closeDirection);
        if (daytime && !override)
	    changeMode(Mode::opening);
        break;
    }
}
