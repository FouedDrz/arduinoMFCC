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
    arduinoMFCC(uint8_t  num_channels, uint16_t  frame_size, uint8_t  hop_size, uint8_t  mfcc_size, float samplerate);
    ~arduinoMFCC() ;

    // Fonctions publiques
    void compute();
    void compute(uint8_t ,uint16_t ,float , float *, float *);
    void computebust(uint8_t ,uint16_t ,float , float *, float *);
    void computebust_dct(uint8_t ,uint8_t ,uint16_t ,float *);
    void apply_hamming_window();
    void apply_hamming_window(float *);
    void apply_hamming_window(float *,float *);
    //
    void apply_hanning_window();
    void apply_hanning_window(float *);
    void apply_hanning_window(float *,float *);
    //
    void pre_emphasis(uint16_t , float *);
    void apply_mel_filter_bank();    
    void apply_mel_filter_bank(uint8_t  , uint16_t  ,float *, float **, float *);
    void create_mel_filter_bank();
    void create_mel_filter_bank(float , uint8_t  ,uint16_t , float**);
    void apply_mel_filter_bank_power(uint16_t ,float *);
    void apply_dct();
    void apply_dct(uint8_t  , uint8_t  ,uint16_t  ,float **,float *, float *);
    void create_hamming_window();
    void create_hamming_window(uint16_t  );
    void create_hamming_window(uint16_t , float *);
    void create_hanning_window();
    void create_hanning_window(uint16_t  );
    void create_hanning_window(uint16_t , float *);
    void create_dct_matrix();
    void create_dct_matrix(float **);
    // 
    float _samplerate;
    float* _frame;
    uint16_t  _frame_size;
    float* _mfcc_coeffs;
    float *_rmfcc_coeffs;
    uint8_t  _num_channels;
    float* _hamming_window;
    float* _hanning_window;
    float** _mel_filter_bank;
    uint8_t  _hop_size;
    uint8_t  _mfcc_size;
    float** _dct_matrix;
private:
    // Variables privées
        float* coeffs;

    // Fonctions privées

};

#endif
