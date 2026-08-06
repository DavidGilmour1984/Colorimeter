# Six-Wavelength LED Absorbance Colorimeter

A teaching colorimeter that measures the absorbance of a solution at six visible
wavelengths. Six coloured LEDs are shone one at a time through a cuvette, and a
GL5516 light-dependent resistor (LDR) on the far side measures how much light
gets through. Comparing each reading against a blank gives absorbance, which is
plotted as a spectrum and logged to a table in the browser.

The system has two parts: an ATmega328P board running the firmware
(`Colorimeter.ino`), and a Web Serial control page (`colorimeter.html`) that
talks to it over USB.

## Hardware

| Block | Detail |
|---|---|
| MCU | ATmega328P (Arduino Uno core), 16 MHz, 5 V |
| USB–serial | CH340C |
| Detector | GL5516 CdS photoresistor (LDR) in a voltage divider on ADC2 (pin A2) |
| Emitters | 6 LEDs on digital pins 2–7, each through a 1 kΩ series resistor |
| ADC | 10-bit, default 5 V reference → 0–1023 counts |
| Serial | 9600 baud |

### LED / wavelength map

| Pin | Wavelength | Colour |
|---|---|---|
| 2 | 665 nm | Red |
| 3 | 630 nm | Orange |
| 4 | 600 nm | Yellow |
| 5 | 550 nm | Green |
| 6 | 470 nm | Blue |
| 7 | 400 nm | Violet |

### The GL5516 detector

The GL5516 is a cadmium-sulphide photoresistor. Its resistance falls as incident
light increases — bright light gives a low resistance (a few kΩ), darkness gives
a high resistance (up to the low MΩ range). It is wired as the upper (or lower)
leg of a divider against a 1 kΩ fixed resistor; the junction feeds ADC2. As more
light passes through the cuvette, the divider voltage shifts and the ADC count
changes accordingly.

Two practical points follow from using a CdS cell:

- **It is slow.** A GL5516 takes tens of milliseconds to settle after a change in
  light level, which is why each LED is held on for 2.5 s before the reading is
  taken.
- **Its response is not flat across colour.** CdS cells peak in sensitivity
  around green (~540 nm) and fall off towards red and violet. This is corrected
  for by the per-channel calibration step below — each wavelength is referenced
  to its own blank reading, so the differing sensitivity cancels out.

## How a measurement works

1. **Blank (I₀).** During calibration each LED is lit in turn with a blank
   cuvette (solvent only) in the holder, and the LDR reading is stored as I₀ for
   that channel — the "full transmission" reference.
2. **Sample (I).** With the sample cuvette in place, the same LED is lit and the
   LDR reading I is taken.
3. **Absorbance.** The firmware computes

   ```
   A = -log10(I / I0)
   ```

   A blank-equivalent sample gives I ≈ I₀ and A ≈ 0. A darker (more absorbing)
   sample gives a smaller I and a larger positive A.

Because I₀ is measured per wavelength, the LDR's uneven colour response, the
differing LED brightnesses, and any fixed offset from ambient light all divide
out of the ratio.

## Firmware commands

All commands are newline-terminated and sent at 9600 baud.

| Command | Action | Output |
|---|---|---|
| `CALIBRATE` | Lights each LED against the blank and stores I₀ per channel | `CAL_START`, `CAL_STEP <nm>` per step, then `CAL_DONE` |
| `READ_<nm>` | Measures one wavelength | `<nm>,<absorbance>` then `DONE` |
| `SWEEP` | Measures all six in ROYGBIV order | one CSV line of six absorbance values |

Each LED is held on for 2.5 s before its reading to let the LDR settle.

## Web Serial control page

`colorimeter.html` runs in a Chromium-based browser (Chrome or Edge) that
supports the Web Serial API.

- **Connect** — opens the serial port at 9600 baud; the status chip turns green.
- **Calibrate** — prompts to confirm the blank is inserted, then runs `CALIBRATE`.
- **Wavelength buttons** — send `READ_<nm>` for a single colour.
- **Sweep** — steps through all six wavelengths, plotting the spectrum point by
  point.
- **Export** — saves the absorbance table as a CSV named after the trial.
- **Clear Data** — empties the table and chart.
- **Instructions** — on-screen operating steps for the user.

The centre panel draws the spectrum against a rainbow background whose colour
bands line up with the true wavelength positions on the x-axis (400–665 nm).

## Operating procedure

1. Plug in over USB and click **Connect**.
2. Insert the blank cuvette, close the lid, and click **Calibrate**. Wait for
   calibration to finish.
3. Swap in the sample cuvette and close the lid.
4. Click a wavelength button for a single reading, or **Sweep** for the full
   spectrum.
5. Name the trial and click **Export** to save the data.

## Notes

- Keep the lid closed during readings; stray room light adds to the LDR signal.
- Recalibrate whenever the solvent, cuvette type, or ambient lighting changes.
- Readings near 0 or 1023 counts mean the LDR is near the ends of its range —
  adjust LED brightness or the divider resistor if a channel consistently sits
  there.
- The `.ino` is provided as `.txt` for easy copying; rename to `.ino` before
  flashing.
