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
#include "arduinoMFCC.h"
#ifndef PI
#define PI 3.1415926535897932384626433832795
#endif
#if !defined(__SAM3X8E__)
#error "Ce code est conçu pour être utilisé uniquement avec Arduino Due. Veuillez utiliser un Arduino Due pour le projet Neurospeech."
#endif

#define AUDIO_SAMPLE_RATE_EXACT 8000. // 16000. //44100.0 // ou 8000.0 -> supprimer dans la suite du code

arduinoMFCC::arduinoMFCC(int num_channels, int frame_size, int hop_size, int mfcc_size, float samplerate) {
    _num_channels = num_channels;
    _frame_size = frame_size;
    _hop_size = hop_size;
    _mfcc_size = mfcc_size;
    _samplerate=samplerate;
    _frame = (float*)malloc(_frame_size * sizeof(float));
    _hamming_window = (float*)malloc(_frame_size * sizeof(float));
    _mel_filter_bank = (float*)malloc(_num_channels * _frame_size * sizeof(float));
    _dct_matrix = (float*)malloc(_mfcc_size * _num_channels * sizeof(float));
    _mfcc_coeffs = (float*)malloc(_mfcc_size * sizeof(float)); 
}

void arduinoMFCC::compute() {
    // ... code pour calculer les coefficients arduinoMFCC ...
    apply_hamming_window();
    apply_mel_filter_bank();
    apply_dct();
}
//////////////////////////////////////////////////////////////////////////
// Fonction privée pour créer la fenêtre de Hamming
void arduinoMFCC::create_hamming_window() {
    // ... code pour créer la fenêtre de Hamming ...
	// Fonction privée pour créer la fenêtre de Hamming
    for (int i = 0; i < _frame_size; i++) {
        _hamming_window[i] = 0.54 - 0.46 * cos(2 * PI * i / (_frame_size - 1));
    }
}
////////////////////////////////////////////////////////////////////////////
void arduinoMFCC::create_hamming_window(int _frame_size) {
    // ... code pour créer la fenêtre de Hamming ...
	// Fonction privée pour créer la fenêtre de Hamming
    for (int i = 0; i < _frame_size; i++) {
        _hamming_window[i] = 0.54 - 0.46 * cos(2 * PI * i / (_frame_size - 1));
    }
}
////////////////////////////////////////////////////////////////////////////
// Fonction privée pour créer les filtres de Mel
void arduinoMFCC::create_mel_filter_bank() {
    float mel_low_freq = 0;
    //float mel_high_freq = 2595 * log10(1 + (AUDIO_SAMPLE_RATE_EXACT / 2) / 700);
    float mel_high_freq = 2595 * log10(1 + (_samplerate / 2) / 700);
    float mel_freq_delta = (mel_high_freq - mel_low_freq) / (_num_channels + 1);
    float* mel_f = (float*)malloc((_num_channels + 2) * sizeof(float));

    for (int i = 0; i < _num_channels + 2; i++) {
        mel_f[i] = mel_low_freq + i * mel_freq_delta;
    }

    for (int i = 0; i < _num_channels; i++) {
        float* mel_filter = _mel_filter_bank + i * _frame_size;

        for (int j = 0; j < _frame_size; j++) {
            if (j < mel_f[i]) {
                mel_filter[j] = 0;
            }
            else if (j >= mel_f[i] && j < mel_f[i + 1]) {
                mel_filter[j] = (j - mel_f[i]) / (mel_f[i + 1] - mel_f[i]);
            }
            else if (j >= mel_f[i + 1] && j < mel_f[i + 2]) {
                mel_filter[j] = (mel_f[i + 2] - j) / (mel_f[i + 2] - mel_f[i + 1]);
            }
            else {
                mel_filter[j] = 0;
            }
        }
    }

    free(mel_f);
}
///////////////////////////////////////////////////////////////////////////////////
void arduinoMFCC::create_mel_filter_bank(float _samplerate, int _num_channels,int _frame_size) {
    float mel_low_freq = 0;
    //float mel_high_freq = 2595 * log10(1 + (AUDIO_SAMPLE_RATE_EXACT / 2) / 700);
    float mel_high_freq = 2595 * log10(1 + (_samplerate / 2) / 700);
    float mel_freq_delta = (mel_high_freq - mel_low_freq) / (_num_channels + 1);
    float* mel_f = (float*)malloc((_num_channels + 2) * sizeof(float));

    for (int i = 0; i < _num_channels + 2; i++) {
        mel_f[i] = mel_low_freq + i * mel_freq_delta;
    }

    for (int i = 0; i < _num_channels; i++) {
        float* mel_filter = _mel_filter_bank + i * _frame_size;

        for (int j = 0; j < _frame_size; j++) {
            if (j < mel_f[i]) {
                mel_filter[j] = 0;
            }
            else if (j >= mel_f[i] && j < mel_f[i + 1]) {
                mel_filter[j] = (j - mel_f[i]) / (mel_f[i + 1] - mel_f[i]);
            }
            else if (j >= mel_f[i + 1] && j < mel_f[i + 2]) {
                mel_filter[j] = (mel_f[i + 2] - j) / (mel_f[i + 2] - mel_f[i + 1]);
            }
            else {
                mel_filter[j] = 0;
            }
        }
    }

    free(mel_f);
}

//////////////////////////////////////////////////////////////////////////////////
// Fonction privée pour créer la matrice de transformée de cosinus discrète (DCT)
void arduinoMFCC::create_dct_matrix() {
    // ... code pour créer la matrice DCT ...
	float sqrt_2_over_n = sqrt(2.0 / _num_channels);
	for (int i = 0; i < _mfcc_size; i++) {
    for (int j = 0; j < _num_channels; j++) {
        _dct_matrix[i * _num_channels + j] = sqrt_2_over_n * cos((i + 0.5) * PI / _num_channels * (j + 0.5));
    }
}

}
/////////////////////////////////////////////////////////////////////////////
// Fonction privée pour appliquer la fenêtre de Hamming au signal audio
void arduinoMFCC::apply_hamming_window() {
    for (int n = 0; n < _frame_size; n++) {
        _frame[n] = _frame[n] * _hamming_window[n];
    }
}
/////////////////////////////////////////////////////////////////////////////
void arduinoMFCC::apply_hamming_window(float *_frame) {
    for (int n = 0; n < _frame_size; n++) {
        _frame[n] = _frame[n] * _hamming_window[n];
    }
}
/////////////////////////////////////////////////////////////////////////////
// Fonction privée pour appliquer les filtres de Mel au signal audio
void arduinoMFCC::apply_mel_filter_bank() {
    for (int i = 0; i < _num_channels; i++) {
        float output = 0.0f;
        for (int j = 0; j < _frame_size; j++) {
            output += _frame[j] * _mel_filter_bank[i*_frame_size + j];
        }
        _mfcc_coeffs[i] = log10f(output);
    }
}
/////////////////////////////////////////////////////////////////////////////
void arduinoMFCC::apply_mel_filter_bank(int _num_channels, int _frame_size,float *_frame, float *_mfcc_coeffs) {
    for (int i = 0; i < _num_channels; i++) {
        float output = 0.0f;
        for (int j = 0; j < _frame_size; j++) {
            output += _frame[j] * _mel_filter_bank[i*_frame_size + j];
        }
        _mfcc_coeffs[i] = log10f(output);
    }
}

/////////////////////////////////////////////////////////////////////////////

// Fonction privée pour appliquer la transformée de cosinus discrète (DCT) au signal audio
void arduinoMFCC::apply_dct() {
    for (int i = 0; i < _mfcc_size; i++) {
        _mfcc_coeffs[i] = 0.0;
        for (int j = 0; j < _num_channels; j++) {
            _mfcc_coeffs[i] += _mel_filter_bank[j * _frame_size + i] * log10(_mel_filter_bank[j * _frame_size + i]);
        }
    }

    for (int i = 0; i < _mfcc_size; i++) {
        float sum = 0.0;
        for (int j = 0; j < _num_channels; j++) {
            sum += _mfcc_coeffs[j] * cos((M_PI * i * (j + 0.5)) / _mfcc_size);
        }
        _mfcc_coeffs[i] = sum;
    }
}
