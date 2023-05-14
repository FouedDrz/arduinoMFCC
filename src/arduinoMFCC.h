/*
	MFCC library
	Copyright (C) 2023 Foued DERRAZ

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
#ifndef arduinoMFCC_H
#define arduinoMFCC_H

#include <Arduino.h>
#include <math.h>
#define AUDIO_SAMPLE_RATE_EXACT  8000

class arduinoMFCC {
public:
    // Constructeur
    arduinoMFCC(int num_channels, int frame_size, int hop_size, int mfcc_size, float samplerate);

    // Fonctions publiques
    void compute();
    void apply_hamming_window();
    void apply_mel_filter_bank();
    void apply_dct();
    void create_hamming_window();
    void create_mel_filter_bank();
    void create_dct_matrix();
    float* coeffs;

private:
    // Variables privées
    int _num_channels;
    int _frame_size;
    int _hop_size;
    int _mfcc_size;
    float _samplerate;
    float* _frame;
    float* _hamming_window;
    float* _mel_filter_bank;
    float* _dct_matrix;
    float* _mfcc_coeffs;

    // Fonctions privées

};

#endif
