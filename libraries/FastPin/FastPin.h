/*
    FastPins, a pin-twiddling library for AVR microcontrollers.
    Copyright (C) 2015 Jure Varlec <jure@varlec.si>

    Certain macros relating to for_pins() functionality were written
    by Laurent Deniau and Christof Warlich.

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

#ifndef FAST_PINS_H
#define FAST_PINS_H

#include <avr/io.h>

/* This header provides MCU-specific implementations of a FastPin
   class which looks like

   template<uint8_t pin>
   struct FastPin
   {
       static void high();
       static void low();
       static void toggle();
       static void set(bool high);
       static bool get();
       static void output();
       static void input();
       static void direction(bool out);
       operator FastAnyPin() const;
       operator uint8_t() const;
   }

   This struct provides control for the specified pin, takes no
   storage and functions compile to single instructions. Pins are
   numbered in the same order as they are in the AVR datasheets,
   unless the Arduino environment is detected. In that case, Arduino
   numbering is used and the number of accessible pins may be reduced.
   If C++11 or later is used, compile-time errors are emitted for
   invalid pin numbers.

   A runtime-dispatch struct called FastAnyPin is provided. It can be
   used to call any pin the same way as FastPin, but its functions are
   not static, it may use memory and will only compile to a single
   instruction if you do not confuse the compiler too much. It can be
   constructed from the pin number or by casting an existing FastPin.

   This header also provides convenience macros for manipulating bit
   values in registers:

   FAST_SET(reg, bit)
   FAST_GET(reg, bit)
   FAST_CLR(reg, bit)

   A macro is provided that iterates over listed pins and calls the
   provided functor on each. It only expands out to 16 pins. To gain
   performance, use a templated functor. Call the macro as

   for_pin(functor, pin1, pin2, ...);

   As FastPin can be implicitly cast to a FastAnyPin, the functor can
   always take the latter as argument, but that will compile into
   function calls. Still faster than Arduino's digitalWrite though ...
*/

#define FAST_SET(reg, bit) ((reg) |= _BV((bit)))
#define FAST_GET(reg, bit) ((reg) & _BV((bit)))
#define FAST_CLR(reg, bit) ((reg) &= ~_BV((bit)))
// Used to return a value in FAST_PINS_IF_PATTERN (see below).
#define FAST_RGET(reg, bit) return FAST_GET(reg, bit)

template <uint8_t pin>
struct FastPin;

// Pin number checks. Assumes the max pin number is defined on invocation.

#if __cplusplus > 199711L
#define FAST_PINS_CHECK(pin) static_assert(pin < FAST_PINS_MAXPIN, \
					   "Pin number too high for this MCU.");
#else
#define FAST_PINS_CHECK(pin)
#endif


/* Pin mapping is done using if or switch statements. A macro
   containing those must be defined for each MCU and called
   FAST_PINS_IF_PATTERN(typ, op), where typ is the type of register
   (PORT, PIN or DDR) and op is the operation (SET, RGET or CLR). the
   following macro can then be used to define the the FastPin struct.

   The implementation for ATtiny24 et al. should make this clearer :)
*/

#define FAST_PINS_DEFINE_STRUCT						\
    struct FastAnyPin							\
    {									\
        FastAnyPin(): pin(255) {}                                       \
        FastAnyPin(uint8_t pin_): pin(pin_) {}				\
        void high() const { FAST_PINS_IF_PATTERN(PORT, SET) }	        \
	void low() const { FAST_PINS_IF_PATTERN(PORT, CLR) }		\
	void toggle() const { FAST_PINS_IF_PATTERN(PIN, SET) }		\
	void set(bool h) const { if (h) high(); else low(); }		\
	bool get() const { FAST_PINS_IF_PATTERN(PIN, RGET) }		\
	void output() const { FAST_PINS_IF_PATTERN(DDR, SET) }		\
	void input() const { FAST_PINS_IF_PATTERN(DDR, CLR) }		\
	void direction(bool out) const { if (out) output(); else input(); } \
	operator uint8_t() const { return pin; }			\
    private:								\
        uint8_t pin;							\
    };									\
									\
    template <uint8_t pin>						\
    struct FastPin							\
    {									\
	FAST_PINS_CHECK(pin)						\
        static void high() { FAST_PINS_IF_PATTERN(PORT, SET) }	        \
	static void low() { FAST_PINS_IF_PATTERN(PORT, CLR) }		\
	static void toggle() { FAST_PINS_IF_PATTERN(PIN, SET) }		\
	static void set(bool h) { if (h) high(); else low(); }		\
	static bool get() { FAST_PINS_IF_PATTERN(PIN, RGET) }		\
	static void output() { FAST_PINS_IF_PATTERN(DDR, SET) }		\
	static void input() { FAST_PINS_IF_PATTERN(DDR, CLR) }		\
	static void direction(bool out) { if (out) output(); else input(); } \
	operator FastAnyPin() const { return FastAnyPin(pin); }		\
	operator uint8_t() const { return pin; }			\
    };

// Implementations

#if defined (__AVR_ATtiny24__) || defined (__AVR_ATtiny44__) || defined (__AVR_ATtiny84__)

// Pin 11 (i.e. PB3) is not exposed by the Arduino board file because it
// coincides with the reset pin, but is provided for use outside of Arduino.
// Also, Arduino reverses the order of pins 8-10.

#ifdef ARDUINO
#define FAST_PINS_MAXPIN 11
#define FAST_PINS_IF_PATTERN(typ, op)		\
    if (pin < 8) {				\
	FAST_ ## op (typ ## A, pin);		\
    } else {					\
	FAST_ ## op (typ ## B, (10-pin));	\
    }
#else
#define FAST_PINS_MAXPIN 12
#define FAST_PINS_IF_PATTERN(typ, op)		\
    if (pin < 8) {				\
	FAST_ ## op (typ ## A, pin);		\
    } else {					\
	FAST_ ## op (typ ## B, (pin-8));	\
    }
#endif

FAST_PINS_DEFINE_STRUCT

#elif defined (__AVR_ATtiny25__) || defined (__AVR_ATtiny45__) || defined (__AVR_ATtiny85__)

// Pin 5 (i.e. PB5) is not exposed by the arduino board file because it
// coincides with the reset pin, but is provided for use outside of arduino.

#ifdef ARDUINO
#define FAST_PINS_MAXPIN 5
#else
#define FAST_PINS_MAXPIN 6
#endif

// For this MCU, the pattern is essentially a no-op.
#define FAST_PINS_IF_PATTERN(typ, op) FAST_ ## op (typ ## B, pin);

FAST_PINS_DEFINE_STRUCT

#elif defined (__AVR_ATmega328P__) || defined (__AVR_ATmega328__)

#ifdef ARDUINO
#define FAST_PINS_MAXPIN 14
#define FAST_PINS_IF_PATTERN(typ, op)		\
    if (pin < 8) {				\
	FAST_ ## op (typ ## D, pin);		\
    } else {					\
	FAST_ ## op (typ ## B, (pin-8));	\
    }
#else
#define FAST_PINS_MAXPIN 24
#define FAST_PINS_IF_PATTERN(typ, op)		\
    if (pin < 8) {				\
	FAST_ ## op (typ ## B, pin);		\
    } else if (pin < 15) {			\
	FAST_ ## op (typ ## C, (pin-8));	\
    } else {					\
	FAST_ ## op (typ ## D, (pin-15));	\
    }
#endif

FAST_PINS_DEFINE_STRUCT

#else
#error "FastPins does not support this MCU."
#endif

#undef FAST_PINS_STRUCT_PATTERN
#undef FAST_PINS_IF_PATTERN


// For loop over pins.

/*
 * The FAST_PINS_NARG macro evaluates to the number of arguments that have been
 * passed to it.
 *
 * Laurent Deniau, "__VA_NARG__," 17 January 2006, <comp.std.c> (29 November 2007).
 */
#define FAST_PINS_NARG(...)  FAST_PINS_NARG_(__VA_ARGS__,FAST_PINS_RSEQ_N())
#define FAST_PINS_NARG_(...) FAST_PINS_ARG_N(__VA_ARGS__)

#define FAST_PINS_ARG_N( \
        _1, _2, _3, _4, _5, _6, _7, _8, _9,_10,  \
        _11,_12,_13,_14,_15,_16,_17,_18,_19,_20, \
        _21,_22,_23,_24,_25,_26,_27,_28,_29,_30, \
        _31,_32,_33,_34,_35,_36,_37,_38,_39,_40, \
        _41,_42,_43,_44,_45,_46,_47,_48,_49,_50, \
        _51,_52,_53,_54,_55,_56,_57,_58,_59,_60, \
        _61,_62,_63,N,...) N

#define FAST_PINS_RSEQ_N() \
        63,62,61,60,                   \
        59,58,57,56,55,54,53,52,51,50, \
        49,48,47,46,45,44,43,42,41,40, \
        39,38,37,36,35,34,33,32,31,30, \
        29,28,27,26,25,24,23,22,21,20, \
        19,18,17,16,15,14,13,12,11,10, \
        9,8,7,6,5,4,3,2,1,0

// Christof Warlich, 24 may 2008
#define PASTE(a, b) a##b
#define FAST_PINS_ARGV(N, ...)        PASTE(FAST_PINS_ARGV, N)(__VA_ARGS__)
#define FAST_PINS_ARGV1(_00,...) \
        _00
#define FAST_PINS_ARGV2(_00,_01,...) \
        _00,_01
#define FAST_PINS_ARGV3(_00,_01,_02,...) \
        _00,_01,_02
#define FAST_PINS_ARGV4(_00,_01,_02,_03,...) \
        _00,_01,_02,_03
#define FAST_PINS_ARGV5(_00,_01,_02,_03,_04,...) \
        _00,_01,_02,_03,_04
#define FAST_PINS_ARGV6(_00,_01,_02,_03,_04,_05,...) \
        _00,_01,_02,_03,_04,_05
#define FAST_PINS_ARGV7(_00,_01,_02,_03,_04,_05,_06,...) \
        _00,_01,_02,_03,_04,_05,_06
#define FAST_PINS_ARGV8(_00,_01,_02,_03,_04,_05,_06,_07,...) \
        _00,_01,_02,_03,_04,_05,_06,_07
#define FAST_PINS_ARGV9(_00,_01,_02,_03,_04,_05,_06,_07,_08,...) \
        _00,_01,_02,_03,_04,_05,_06,_07,_08
#define FAST_PINS_ARGV10(_00,_01,_02,_03,_04,_05,_06,_07,_08,_09,...) \
        _00,_01,_02,_03,_04,_05,_06,_07,_08,_09
#define FAST_PINS_ARGV11(_00,_01,_02,_03,_04,_05,_06,_07,_08,_09,_0a,...) \
        _00,_01,_02,_03,_04,_05,_06,_07,_08,_09,_0a
#define FAST_PINS_ARGV12(_00,_01,_02,_03,_04,_05,_06,_07,_08,_09,_0a,_0b,...) \
        _00,_01,_02,_03,_04,_05,_06,_07,_08,_09,_0a,_0b
#define FAST_PINS_ARGV13(_00,_01,_02,_03,_04,_05,_06,_07,_08,_09,_0a,_0b,_0c,...) \
        _00,_01,_02,_03,_04,_05,_06,_07,_08,_09,_0a,_0b,_0c
#define FAST_PINS_ARGV14(_00,_01,_02,_03,_04,_05,_06,_07,_08,_09,_0a,_0b,_0c,_0d,...) \
        _00,_01,_02,_03,_04,_05,_06,_07,_08,_09,_0a,_0b,_0c,_0d
#define FAST_PINS_ARGV15(_00,_01,_02,_03,_04,_05,_06,_07,_08,_09,_0a,_0b,_0c,_0d,_0e,...) \
        _00,_01,_02,_03,_04,_05,_06,_07,_08,_09,_0a,_0b,_0c,_0d,_0e
#define FAST_PINS_ARGV16(_00,_01,_02,_03,_04,_05,_06,_07,_08,_09,_0a,_0b,_0c,_0d,_0e,_0f,...) \
        _00,_01,_02,_03,_04,_05,_06,_07,_08,_09,_0a,_0b,_0c,_0d,_0e,_0f

/*
 * Apply function-like macro F to each item in argument list.
 */
#define FAST_PINS_MAP16(F,_00,_01,_02,_03,_04,_05,_06,_07,_08,_09,_0a,_0b,_0c,_0d,_0e,_0f,...) \
        F(_00),F(_01),F(_02),F(_03),F(_04),F(_05),F(_06),F(_07), \
        F(_08),F(_09),F(_0a),F(_0b),F(_0c),F(_0d),F(_0e),F(_0f)

#define FAST_PINS_MAP_(F,...)        FAST_PINS_MAP16(F,__VA_ARGS__)
#define FAST_PINS_MAP(F,...)        FAST_PINS_ARGV(FAST_PINS_NARG(__VA_ARGS__),FAST_PINS_MAP_(F,__VA_ARGS__,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0))

#define for_pin(...) FAST_PINS_MAP(__VA_ARGS__)

#endif
