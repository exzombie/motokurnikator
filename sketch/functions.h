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

// This header is a workaround for Arduino IDE's inability to cope
// with function definitions other than setup() and loop().

inline static void changeMode(Mode m)
{
    mode = m;
}

inline static void moveMotor(int8_t direction)
{
    motor.step(direction * stepsPerRot);
}

template <class Pin>
inline static void lowPin(Pin p)
{
    p.low();
}

inline static void stopMotor()
{
    for_pin(lowPin, motor0, motor1, motor2, motor3);
}