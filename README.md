# Smartlock Door and Automatically Curtain for Futuristic Room

Final project for the **Embedded Systems Practicum** (Interface Laboratory, Department of Physics, Faculty of Mathematics and Natural Sciences, Universitas Indonesia, 2024).

An **Arduino Uno**-based system that combines a smart door lock using a fingerprint sensor with an automatic curtain that moves according to light intensity.

## Group Members

- Susan
- Revan

## Objectives

- To apply embedded systems in everyday life, particularly in building a door with a fingerprint sensor and automatic curtains that open and close using a light sensor.
- To fulfill the assessment requirements of the final project for the Embedded Systems Practicum course.

## Features

- **Smart door lock**: a matching fingerprint triggers the servo to open the door, which then closes automatically.
- **Indoor push button**: a push button allows the door to be opened from inside the room, and it also closes automatically.
- **Automatic curtain**: a photodiode detects sunlight; the stepper motor opens the curtain in bright conditions and closes it in dark conditions.

## Applied Module Concepts

| Module | Application |
|--------|-------------|
| **USART** | Serial communication between the Arduino (master) and the FPM10A fingerprint sensor (slave) |
| **Interrupt** | Push button used to open the door from inside |
| **ADC** | Reading the photodiode's analog signal (10-bit, 0–1023) to control the curtain |

## Components

| Component | Quantity |
|-----------|----------|
| Arduino Uno | 1 |
| Fingerprint sensor (FPM10A) | 1 |
| SG90 servo | 1 |
| Stepper motor | 1 |
| Stepper motor driver (L293D IC) | 1 |
| Photodiode | 2 |
| Push button | 1 |

## Pin Configuration

### Fingerprint Sensor

| Fingerprint Sensor | Arduino |
|--------------------|---------|
| VCC | 5V (can also operate at 3.3V) |
| TX | Digital pin 2 (RX, software serial) |
| RX | Digital pin 3 (TX, software serial) |
| GND | GND |

### Other Devices

| Device | Arduino Pin |
|--------|-------------|
| Servo (signal) | 8 |
| Push button | 6 |
| Photodiode | A0 |
| Stepper IN1 | 12 |
| Stepper IN2 | 11 |
| Stepper IN3 | 10 |
| Stepper IN4 | 9 |

## Working Principle

1. The Arduino Uno controls the entire process and continuously monitors the fingerprint sensor, the push button, and the photodiode.
2. When a fingerprint is detected and matches the data stored in the sensor, the servo rotates from 0° to 90° to open the door, waits briefly, and then returns to 0° to close it.
3. When the push button is pressed, the servo performs the same open-and-close sequence.
4. The photodiode is read through the ADC. An ADC value below 500 is interpreted as daytime (the curtain opens, 400 steps), while a value of 500 or above is interpreted as nighttime (the curtain closes, 0 steps).
5. A crystal oscillator keeps the Arduino clock stable, while pull-up/pull-down resistors keep the input logic levels stable.

## Required Libraries

- `Adafruit_Fingerprint`
- `Servo`
- `Wire`
- `Stepper`

Install them via **Arduino IDE → Sketch → Include Library → Manage Libraries**.

## Usage

1. Assemble the components according to the pin configuration above.
2. Install the required libraries in the Arduino IDE.
3. Enroll fingerprints first using the `enroll` example sketch from the Adafruit Fingerprint library (IDs 1–3).
4. Upload the main program to the Arduino Uno.
5. Open the Serial Monitor at a baud rate of **9600** to view the fingerprint status and the photodiode ADC values.
6. Place an enrolled finger on the sensor to open the door, or press the push button from inside.

## Program Structure

| Function | Description |
|----------|-------------|
| `setup()` | Initializes pins, serial communication, the stepper motor (20 RPM), and the fingerprint sensor (baud rate 57600) |
| `loop()` | Calls `getFingerprintID()`, reads the push button, reads the photodiode, and drives the stepper motor |
| `getFingerprintID()` | Captures the fingerprint image, converts it, searches for a match, and moves the servo if a match is found |

## Results and Notes

- The photodiode converts light intensity into an analog voltage, which is read as an ADC value by the Arduino.
- The push button acts as an interrupt to open the door from inside.
- The fingerprint sensor applies the USART concept: a fingerprint matching the database triggers the servo to move.
- The time per step of the stepper motor is **5.86 ms** (20 RPM, 512 steps per revolution).
- The number of stepper steps is adjusted to the size of the installed window.

## References

1. Mazidi, M. A., Naimi, S., & Naimi, S. (2011). *The AVR Microcontroller and Embedded Systems using Assembly and C*. Prentice Hall.
2. [Arduino ADC in Arduino Uno, ElectronicWings](https://www.electronicwings.com/arduino/adc-in-arduino)
3. [Arduino USART in Arduino Uno, ElectronicWings](https://www.electronicwings.com/arduino/usart-in-arduino-uno)
4. [Prasetyo, E. A. Pengertian dan Prinsip Kerja Motor Servo, Arduino Indonesia](https://www.arduinoindonesia.id/2022/10/pengertian-dan-prinsip-kerja-motor-servo.html)
5. Riski, M. D. (2019). *Rancang Alat Lampu Otomatis di Cargo Compartment Pesawat Berbasis Arduino Menggunakan Push Button Switch*. [Politeknik Penerbangan Surabaya](https://ejournal.poltekbangsby.ac.id/index.php/SNITP/article/view/414)
6. Sasmoko, D. *Arduino dan Sensor Pada Project Arduino DIY*.
7. [Universitas Muhammadiyah Yogyakarta. Fingerprint Sensor Module with Arduino (FPM10A)](https://repository.umy.ac.id/bitstream/handle/123456789/22709/FPM10A.pdf?sequence=13&isAllowed=y)

