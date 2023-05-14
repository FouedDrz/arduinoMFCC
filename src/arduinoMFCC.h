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
    void compute(int ,int ,int ,float *);
    void apply_hamming_window();
    void apply_hamming_window(float *);
    void apply_hamming_window(float *,float *);
    void apply_mel_filter_bank();
    void apply_mel_filter_bank(int , int ,float *, float *, float *);
    void create_mel_filter_bank();
    void create_mel_filter_bank(float , int ,int, float* );
    void apply_dct();
    void apply_dct(int , int ,int ,float *,float *, float *);
    void create_hamming_window();
    void create_hamming_window(int );
    void create_dct_matrix();
  
    float* coeffs;
    float _samplerate;
    float* _frame;
    int _frame_size;
    float* _mfcc_coeffs;
    float *_rmfcc_coeffs;
    int _num_channels;
    float* _hamming_window;
    float* _mel_filter_bank;

private:
    // Variables privées
    int _hop_size;
    int _mfcc_size;

    float* _dct_matrix;


    // Fonctions privées

};

#endif
