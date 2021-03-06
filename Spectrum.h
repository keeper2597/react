/*
 * React: https://github.com/evilgeniuslabs/react
 * Copyright (C) 2015 Jason Coon, Evil Genius Labs
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

// The scale sets how much sound is needed in each frequency range to
// show all 32 bars.  Higher numbers are more sensitive.
float scale = 256.0;

const int bandCount = 16;

float fftLevels[bandCount];

float level;

int shown;

int huesPerPixel = 255 / NUM_LEDS;

uint32_t spectrumBar() {
    scale = 256.0;

    gPalette = RainbowColors_p;

    if (fft.available()) {
        // read from the FFT

        // I'm skipping the first two bins, as they seem to be unusable
        // they start out at zero, but then climb and don't come back down
        // even after sound input stops
        level = fft.read(2, 127);
    }

    fill_solid(leds, NUM_LEDS, CRGB::Black);

    int val = level * scale;
    val -= 5;
    if (val >= NUM_LEDS) val = NUM_LEDS - 1;

    if (val >= shown) {
        shown = val;
    }
    else {
        if (shown > 0) shown = shown - 1;
        val = shown;
    }

    for (int i = 0; i < shown; i++) {
        leds[i] = ColorFromPalette(gPalette, i * huesPerPixel + gHue, 255, NOBLEND);
    }

    return 0;
}

const int ledsPerBand = NUM_LEDS / bandCount;

uint32_t spectrumDots() {
    scale = 256.0;

    gPalette = RainbowColors_p;

    if (fft.available()) {
        fftLevels[0] = fft.read(2);
        fftLevels[1] = fft.read(3);
        fftLevels[2] = fft.read(4);
        fftLevels[3] = fft.read(5, 6);
        fftLevels[4] = fft.read(7, 8);
        fftLevels[5] = fft.read(9, 10);
        fftLevels[6] = fft.read(11, 14);
        fftLevels[7] = fft.read(15, 19);
        fftLevels[8] = fft.read(20, 25);
        fftLevels[9] = fft.read(26, 32);
        fftLevels[10] = fft.read(33, 41);
        fftLevels[11] = fft.read(42, 52);
        fftLevels[12] = fft.read(53, 65);
        fftLevels[13] = fft.read(66, 82);
        fftLevels[14] = fft.read(83, 103);
        fftLevels[15] = fft.read(104, 127);
    }

    fill_solid(leds, NUM_LEDS, CRGB::Black);

    for (int i = 0; i < NUM_LEDS; i++) {
        int val = fftLevels[i / ledsPerBand] * scale;

        if (val >= 256) val = 255;

        leds[i] = ColorFromPalette(gPalette, i * 2 + gHue, val, NOBLEND);
    }

    return 0;
}
