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
#include <EEPROM.h>

static const unsigned long maxSpinTimeMeasurementDelayMs = 10000;
static const unsigned int blinkMs = 100;

static const FastPin<3> endSwOpen;
static const FastPin<4> endSwClosed;
static const FastPin<9> manualOpen;
static const FastPin<10> manualClose;
static const FastPin<8> indicator;

static const FastPin<6> motor1;
static const FastPin<7> motor2;

static const byte lightLevelPin = A2;
static const byte dayLevelPin = A0;
static const byte nightLevelPin = A1;

static const byte standbyDelayMs = 100;
static const byte closeDirection = 0x00;
static const byte openDirection = 0xff;

enum class Mode: byte {
    open, closed, opening, closing
};

static Mode mode = Mode::closing;
static Mode lastMode = Mode::closing;

static unsigned long lastMillisBlink = 0;
static unsigned long lastMillisMotor = 0;
static unsigned long lastMillisManual = 0;
static unsigned long maxSpinTimeMs = 1000;

static const int maxSpinTimeMsEEOffset = 1;

static void moveMotor(byte direction, bool noTimeout = false);

void setup()
{
    for_pin([](const FastAnyPin p){ p.input(); p.low(); },
        lightLevelPin, dayLevelPin, nightLevelPin);
    for_pin([](const FastAnyPin p){ p.input(); p.high(); },
        endSwOpen, endSwClosed, manualOpen, manualClose);
    indicator.high();
    indicator.output();
    for_pin([](const FastAnyPin p){ p.output(); p.low(); },
        motor1, motor2);

    bool isFirstRun = EEPROM[0] == 0xff;
    if (isFirstRun) {
      EEPROM[0] = 0;
      EEPROM.put(maxSpinTimeMsEEOffset, maxSpinTimeMs);
    } else {
      EEPROM.get(maxSpinTimeMsEEOffset, maxSpinTimeMs);
    }
}

void loop()
{
    bool daytime = false, nighttime = false;
    bool override = !manualOpen.get() || !manualClose.get();

    // Check whether we are at one of the end positions.
    if (!endSwOpen.get()) {
        changeMode(Mode::open);
    } else if (!endSwClosed.get()) {
        changeMode(Mode::closed);
    }

    // Check whether the manual override has been pushed for long enough
    // to start travel time measurement.
    if ((!manualOpen.get() && !endSwOpen.get()) ||
        (!manualClose.get() && !endSwClosed.get())) {
        if (!lastMillisManual) {
            lastMillisManual = millis();
        } else if (millis() - lastMillisManual > maxSpinTimeMeasurementDelayMs) {
            maxSpinTimeMeasurement();
            lastMillisManual = 0;
            return;
        }
    } else {
        lastMillisManual = 0;
    }

    // Decide whether to open or close the door.
    if (!manualOpen.get() && endSwOpen.get()) {
        changeMode(Mode::opening);
    } else if (!manualClose.get() && endSwClosed.get()) {
        changeMode(Mode::closing);
    } else {
        int rawLightLevel = analogRead(lightLevelPin);
        int rawDayLevel = analogRead(dayLevelPin);
        int rawNightLevel = analogRead(nightLevelPin);

        // Check that pots are set sanely. Makes sure nighttime and daytime
        // cannot happen simultaneously (see below).
        if (rawNightLevel >= rawDayLevel) {
          stopMotor();
          return;
        }

        float dayLightLevel = (float)(rawLightLevel) /
            (float)(rawDayLevel);
        float nightLightLevel = (float)(rawLightLevel) /
            (float)(rawNightLevel);
        nighttime = nightLightLevel < 1.;
        daytime = dayLightLevel > 1.;
        if (nighttime) {
            indicator.low();
        } else if (daytime) {
            indicator.high();
        } else {
            unsigned long ms = millis();
            if (ms - lastMillisBlink > blinkMs) {
                indicator.toggle();
                lastMillisBlink = ms;
            }
        }
    }

    // Do something about the decision taken above.
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

static void moveMotor(byte direction, bool noTimeout)
{
    if (!noTimeout && millis() - lastMillisMotor > maxSpinTimeMs) {
        stopMotor();
        if (direction == openDirection) {
            changeMode(Mode::open);
        } else {
            changeMode(Mode::closed);
        }
        return;
    }

    if (direction == openDirection) {
        motor2.low();
        motor1.high();
    } else {
        motor1.low();
        motor2.high();
    }
}

static void maxSpinTimeMeasurement() {
    byte direction = mode == Mode::open ? closeDirection : openDirection;
    unsigned long spinTime1 = millis();
    moveMotor(direction, true);
    if (direction == closeDirection) {
        while (endSwClosed.get()) {}
    } else {
        while (endSwOpen.get()) {}
    }
    stopMotor();
    spinTime1 = millis() - spinTime1;

    direction = direction == closeDirection ? openDirection : closeDirection;
    unsigned long spinTime2 = millis();
    moveMotor(direction, true);
    if (direction == closeDirection) {
        while (endSwClosed.get()) {}
    } else {
        while (endSwOpen.get()) {}
    }
    stopMotor();
    spinTime2 = millis() - spinTime2;

    // Take the longer time and extend it by 5%.
    maxSpinTimeMs = max(spinTime1, spinTime2);
    maxSpinTimeMs *= 105;
    maxSpinTimeMs /= 100;

    // Do not accept less than 1 second.
    maxSpinTimeMs = max(maxSpinTimeMs, 1000);

    EEPROM.put(maxSpinTimeMsEEOffset, maxSpinTimeMs);
}

