# ESP32 PWM Chicken-Coop Lighting

A group coursework prototype that regulates an LED lighting load from an LDR
reading using ESP32 PWM. The final sketch also reads a DS3231 real-time clock
and stores measurements on a microSD card.

![Wiring diagram](hardware/wiring_diagram.png)

![Prototype](hardware/prototype.jpg)

## Repository structure

- `firmware/pwm_chicken_coop_lighting/`: selected final sketch.
- `experiments/`: the main prototype and an incomplete RTC test retained as
  development evidence.
- `hardware/wiring_diagram.png`: original Fritzing wiring view.
- `hardware/pwm_chicken_coop_lighting.fzz`: editable Fritzing project.
- `hardware/prototype.jpg`: original hardware photograph.

## Main pin mapping

| Function | ESP32 pin |
|---|---:|
| LDR analog input | GPIO 36 |
| L298N IN1 / IN2 / ENA | GPIO 13 / GPIO 12 / GPIO 14 |
| microSD chip select | GPIO 5 |
| DS3231 SDA / SCL | GPIO 25 / GPIO 26 |

The sketch uses the ESP32 Arduino core, `FS`, `SD`, `SPI`, `Wire`, and
Adafruit `RTClib`.

## Attribution and status

This was group coursework by Irdina Rusyda Kamila Arsil and Rafi Kamil Arief.
It is presented as group work and does not claim sole authorship. The source,
Fritzing file, and images were curated and scanned, but the sketch was not
recompiled and the circuit was not re-tested during publication. The RTC test
under `experiments/` is incomplete and is not the recommended entry point.

No repository-wide license is applied; obtain permission from the relevant
authors before reuse.

