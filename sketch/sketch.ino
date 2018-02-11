/*
    motokurnikator, an automated chicken coop door controller.
    Copyright (C) 2015-2018 Jure Varlec <jure@varlec.si>

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

static const unsigned long maxSpinTimeMs = 5000;

static const FastPin<2> endSwOpen;
static const FastPin<3> endSwClosed;
static const FastPin<9> manualOpen;
static const FastPin<10> manualClose;
static const FastPin<8> indicator;

static const FastPin<5> motor1;
static const FastPin<4> motor2;

#define lightLevelPin A1
#define referenceLevelPin A0
#define standbyDelayMs 100
#define closeDirection (-1)
#define openDirection (1)
#define lightLevelHysteresisPercent 10
#define lightLevelMarginPercent 1

enum class Mode: byte {
    open, closed, opening, closing
};

static Mode mode = Mode::closing;
static Mode lastMode = Mode::closing;

static unsigned long lastMillisBlink = 0;
static unsigned long lastMillisMotor = 0;

void setup()
{
    pinMode(lightLevelPin, INPUT);
    pinMode(referenceLevelPin, INPUT);
    digitalWrite(lightLevelPin, LOW);
    digitalWrite(referenceLevelPin, LOW);
    for_pin([](const FastAnyPin p){ p.input(); p.high(); },
        endSwOpen, endSwClosed, manualOpen, manualClose);
    indicator.high();
    indicator.output();
    for_pin([](const FastAnyPin p){ p.output(); p.low(); },
        motor1, motor2);
}

void loop()
{
    bool daytime = false, nighttime = false;
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
        float lightLevel = (float)(analogRead(lightLevelPin)) /
            (float)(analogRead(referenceLevelPin));
        nighttime = lightLevel < (1. - lightLevelHysteresisPercent / 100.);
        daytime = lightLevel > (1. - lightLevelMarginPercent / 100.);
        if (nighttime) {
            indicator.low();
        } else if (daytime) {
            indicator.high();
        } else {
            unsigned long ms = millis();
            if (ms - lastMillisBlink > 100) {
                indicator.toggle();
                lastMillisBlink = ms;
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

inline static void changeMode(Mode m)
{
    mode = m;
    if (m != lastMode) {
      if (m == Mode::opening || m == Mode::closing) {
        lastMillisMotor = millis();
      }
      lastMode = m;
    }
}

template <class Pin> inline static void lowPin(Pin p)
{
    p.low();
}

inline static void stopMotor()
{
    for_pin(lowPin, motor1, motor2);
}

inline static void moveMotor(int8_t direction)
{
    if (millis() - lastMillisMotor > maxSpinTimeMs) {
        stopMotor();
        if (direction == openDirection) {
            changeMode(Mode::open);
        } else {
          changeMode(Mode::closed);
        }
    }

    if (direction > 0) {
      motor2.low();
      motor1.high();
    } else {
      motor1.low();
      motor2.high();
    }
}
