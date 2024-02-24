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
#include<Arduino.h>
#include<arduinoFFT.h>
#include "arduinoMFCC.h"
#ifndef PI
#define PI 3.1415926535897932384626433832795
#endif
#if !defined(__SAM3X8E__)
#error "Ce code est conçu pour être utilisé uniquement avec Arduino Due. Veuillez utiliser un Arduino Due pour le projet Neurospeech."
#endif

arduinoMFCC::arduinoMFCC(uint8_t  num_channels, uint16_t  frame_size, uint8_t  hop_size, uint8_t  mfcc_size, float samplerate) {
    _num_channels = num_channels;
    _frame_size = frame_size;
    _hop_size = hop_size;
    _mfcc_size = mfcc_size;
    _samplerate=samplerate;
    _frame = (float*)malloc(_frame_size * sizeof(float));
    _hamming_window = (float*)malloc(_frame_size * sizeof(float));
    //_mel_filter_bank = (float*)malloc(_num_channels * _frame_size * sizeof(float));
    float **_mel_filter_bank  = (float **)malloc(_num_channels * sizeof(float *));
        for (uint8_t i = 0; i < _num_channels; i++) {
             _mel_filter_bank[i] = (float *)malloc((_frame_size/2) * sizeof(float));//
             }
    float **_dct_matrix  = (float **)malloc(_mfcc_size * sizeof(float *));
        for (uint8_t i = 0; i < _mfcc_size; i++) {
             _dct_matrix[i] = (float *)malloc(_num_channels * sizeof(float));//
             }             
    //_dct_matrix = (float*)malloc(_mfcc_size * _num_channels * sizeof(float));

    _rmfcc_coeffs = (float*)malloc(_mfcc_size * sizeof(float)); 
    _mfcc_coeffs = (float*)malloc(_num_channels* sizeof(float)); 	
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

arduinoMFCC::~arduinoMFCC() {
    free(_frame);
    free(_hamming_window);
    for (int i = 0; i < _num_channels; i++) {
          free(_mel_filter_bank[i]);
          }
    free(_mel_filter_bank);
    //
    for (int i = 0; i < _mfcc_size; i++) {
          free(_dct_matrix[i]);
          }
    free(_dct_matrix);
    //
    free(_rmfcc_coeffs);
    free(_mfcc_coeffs);

}
/////////////////////////////////////////////////////////////////////////////
void arduinoMFCC::pre_emphasis(uint16_t _frame_size, float *_frame)
{
// pre-emphasis
    for (uint16_t j = 1; j < _frame_size; j++)
          _frame[j] = _frame[j] - 0.95 * _frame[j-1];
     
}       
/////////////////////////////////////////////////////////////////////////
void arduinoMFCC::compute() {
    // ... code pour calculer les coefficients arduinoMFCC ...
    apply_hamming_window();
    apply_mel_filter_bank();
    apply_dct();
}
//////////////////////////////////////////////////////////////////////////
void arduinoMFCC::compute(uint8_t _num_channels,uint16_t _frame_size,float _samplerate, float *_frame, float *_mfcc_coeffs){
    // ... code pour calculer les coefficients arduinoMFCC ...
    create_hamming_window(_frame_size, _hamming_window); 
    apply_hamming_window(_frame,_hamming_window);
    create_mel_filter_bank(_samplerate,_num_channels,_frame_size, _mel_filter_bank); 
    apply_mel_filter_bank_power(_frame_size,_frame);
    apply_mel_filter_bank(_num_channels,_frame_size,_frame,_mel_filter_bank,_mfcc_coeffs);

    }
//////////////////////////////////////////////////////////////////////////
void arduinoMFCC::computebust(uint8_t _num_channels,uint16_t _frame_size,float _samplerate, float *_frame, float *_mfcc_coeffs) {
    // ... code pour calculer les coefficients arduinoMFCC ...
    //pre_emphasis(_frame_size, _frame); 
    create_hamming_window(_frame_size, _hamming_window); 
    apply_hamming_window(_frame,_hamming_window);
    create_mel_filter_bank(_samplerate,_num_channels,_frame_size,_mel_filter_bank); 
    apply_mel_filter_bank_power(_frame_size,_frame);
    apply_mel_filter_bank(_num_channels,_frame_size,_frame,_mel_filter_bank,_mfcc_coeffs);

    }
//////////////////////////////////////////////////////////////////////////
void arduinoMFCC::computebust_dct(uint8_t _mfcc_size,uint8_t _num_channels,uint16_t  _frame_size,float *_rmfcc_coeffs) {
    // ... code pour calculer les coefficients arduinoMFCC ...
    pre_emphasis(_frame_size, _frame); 
    create_hamming_window(_frame_size, _hamming_window); 
    apply_hamming_window(_frame,_hamming_window);
    apply_mel_filter_bank(_num_channels,_frame_size,_frame,_mel_filter_bank,_mfcc_coeffs);
    apply_dct(_mfcc_size,_num_channels,_frame_size,_mel_filter_bank,_mfcc_coeffs,_rmfcc_coeffs);
    }

///////////////////////////////////////////////////////////////////////////
// Fonction publique pour créer la fenêtre de Hamming
void arduinoMFCC::create_hamming_window() {
    // ... code pour créer la fenêtre de Hamming ...
	// Fonction publique pour créer la fenêtre de Hamming
    for (uint16_t  i = 0; i < _frame_size; i++) {
        _hamming_window[i] = 0.54 - 0.46 * cos(2 * PI * i / (_frame_size - 1));
    }
}
////////////////////////////////////////////////////////////////////////////
void arduinoMFCC::create_hamming_window(uint16_t  _frame_size) {
    // ... code pour créer la fenêtre de Hamming ...
	// Fonction publique pour créer la fenêtre de Hamming
    for (uint16_t  i = 0; i < _frame_size; i++) {
        _hamming_window[i] = 0.54 - 0.46 * cos(2 * PI * i / (_frame_size - 1));
    }
}
///////////////////////////////////////////////////////////////////////////
void arduinoMFCC::create_hamming_window(uint16_t  _frame_size, float *_hamming_window) {
    // ... code pour créer la fenêtre de Hamming ...
	// Fonction publique pour créer la fenêtre de Hamming
    for (uint16_t  i = 0; i < _frame_size; i++) {
        _hamming_window[i] = 0.54 - 0.46 * cos(2 * PI * i / (_frame_size - 1));
    }
}

///////////////////////////////////////////////////////////////////////////
// Fonction publique pour créer la fenêtre de Hanning
void arduinoMFCC::create_hanning_window() {
    // ... code pour créer la fenêtre de Hanning ...
	// Fonction publique pour créer la fenêtre de Hanning
    for (uint16_t  i = 0; i < _frame_size; i++) {
        _hanning_window[i] = 0.5 - 0.5 * cos(2 * PI * i / (_frame_size - 1));
    }
}
////////////////////////////////////////////////////////////////////////////
void arduinoMFCC::create_hanning_window(uint16_t  _frame_size) {
    // ... code pour créer la fenêtre de Hanning ...
	// Fonction publique pour créer la fenêtre de Hanning
    for (uint16_t  i = 0; i < _frame_size; i++) {
        _hanning_window[i] = 0.5 - 0.5 * cos(2 * PI * i / (_frame_size - 1));
    }
}
///////////////////////////////////////////////////////////////////////////
void arduinoMFCC::create_hanning_window(uint16_t  _frame_size, float *_hanning_window) {
    // ... code pour créer la fenêtre de Hanning ...
	// Fonction publique pour créer la fenêtre de Hanning
    for (uint16_t  i = 0; i < _frame_size; i++) {
        _hanning_window[i] = 0.5 - 0.5 * cos(2 * PI * i / (_frame_size - 1));
    }
}
///////////////////////////////////////////////////////////////////////////
// Fonction publique pour créer les filtres de Mel
void arduinoMFCC::create_mel_filter_bank()
 {
      float f_low          = 300.;  
    float f_high         = _samplerate; // Nyquist Frequency
    float mel_low_freq   = 2595. * log10f(1 + (f_low / 2.) / 700.);
    float mel_high_freq  = 2595. * log10f(1 + (f_high / 2.) / 700.);
    float* mel_f         = (float*)malloc((_num_channels + 2) * sizeof(float));
    float* hzPoints      = (float*)malloc((_num_channels + 2) * sizeof(float)); // Corresponding Hz scale points
  // Calculate Mel and Hz scale points
    float mel_freq_delta = (mel_high_freq - mel_low_freq) / (_num_channels + 1);
  for (uint8_t i = 0; i < _num_channels + 2; i++) {
    mel_f[i] = mel_low_freq + i * mel_freq_delta;
    hzPoints[i] = 700.0 * (powf(10, mel_f[i] / 2595.0) - 1);
  }
  // Create the filter bank
  for (uint8_t i = 0; i < _num_channels; i++) {
    for (uint16_t j = 0; j < _frame_size/2; j++) {
      float freq = (float)j * (_samplerate / 2) / (_frame_size / 2);
      if (freq  < hzPoints[i])
        _mel_filter_bank[i][j] = 0;
      else if ( freq >= hzPoints[i] && freq < hzPoints[i+1])
        _mel_filter_bank[i][j] = (freq - hzPoints[i]) / (hzPoints[i+1] - hzPoints[i]);
      else if ( freq >= hzPoints[i+1] && freq <= hzPoints[i+2])
        _mel_filter_bank[i][j] = (hzPoints[i+2] - freq) / (hzPoints[i+2] - hzPoints[i+1]);
      else
        _mel_filter_bank[i][j] = 0;
    }
  }
  free(mel_f);
  free(hzPoints);
}
////////////////////////////////////////////////////////////////////////////////////
void arduinoMFCC::create_mel_filter_bank(float _samplerate, uint8_t _num_channels, uint16_t _frame_size, float **_mel_filter_bank) 
{
    float f_low          = 300.;  
    float f_high         = _samplerate; // Nyquist Frequency
    float mel_low_freq   = 2595. * log10f(1 + (f_low / 2.) / 700.);
    float mel_high_freq  = 2595. * log10f(1 + (f_high / 2.) / 700.);
    float* mel_f         = (float*)malloc((_num_channels + 2) * sizeof(float));
    float* hzPoints      = (float*)malloc((_num_channels + 2) * sizeof(float)); // Corresponding Hz scale points
  // Calculate Mel and Hz scale points
    float mel_freq_delta = (mel_high_freq - mel_low_freq) / (_num_channels + 1);
  for (uint8_t i = 0; i < _num_channels + 2; i++) {
    mel_f[i] = mel_low_freq + i * mel_freq_delta;
    hzPoints[i] = 700.0 * (powf(10, mel_f[i] / 2595.0) - 1);
  }
  // Create the filter bank
  for (uint8_t i = 0; i < _num_channels; i++) {
    for (uint16_t j = 0; j < _frame_size/2; j++) {
      float freq = (float)j * (_samplerate / 2) / (_frame_size / 2);
      if (freq  < hzPoints[i])
        _mel_filter_bank[i][j] = 0;
      else if ( freq >= hzPoints[i] && freq < hzPoints[i+1])
        _mel_filter_bank[i][j] = (freq - hzPoints[i]) / (hzPoints[i+1] - hzPoints[i]);
      else if ( freq >= hzPoints[i+1] && freq <= hzPoints[i+2])
        _mel_filter_bank[i][j] = (hzPoints[i+2] - freq) / (hzPoints[i+2] - hzPoints[i+1]);
      else
        _mel_filter_bank[i][j] = 0;
    }
  }
  free(mel_f);
  free(hzPoints);
}
//////////////////////////////////////////////////////////////////////////////////
// Fonction publique pour créer la matrice de transformée de cosinus discrète (DCT)
// Function is OK
void arduinoMFCC::create_dct_matrix() {
    // ... code pour créer la matrice DCT ...
	float sqrt_2_over_n = sqrt(2.0 / _num_channels);
	for (uint8_t  i = 0; i < _mfcc_size; i++) {
    for (uint8_t  j = 0; j < _num_channels; j++) {
        //_dct_matrix[i][j] = sqrt_2_over_n *cos((M_PI * i * (j + 0.5)) / _num_channels);
        _dct_matrix[i][j] =sqrt_2_over_n *cos((PI * i * (j + 0.5)) / _mfcc_size);
        //Serial.print(_dct_matrix[i][j]);
        // Serial.print("\t");

}
  }
}
/////////////////////////////////////////////////////////////////////////////
// Fonction publique pour créer la matrice de transformée de cosinus discrète (DCT)
// Function is OK
void arduinoMFCC::create_dct_matrix(float **_dct_matrix) {
    // ... code pour créer la matrice DCT ...
	float sqrt_2_over_n = sqrt(2.0 / _num_channels);
	for (uint8_t  i = 0; i < _mfcc_size; i++) {
    for (uint8_t  j = 0; j < _num_channels; j++) {
        //_dct_matrix[i][j] = sqrt_2_over_n *cos((M_PI * i * (j + 0.5)) / _num_channels);
        _dct_matrix[i][j] =cos((PI * i * (j + 0.5)) / _mfcc_size);
         //     Serial.print(_dct_matrix[i][j]);
         //     Serial.print("\t");


    }
}

}
/////////////////////////////////////////////////////////////////////////////
// Fonction publique pour appliquer la fenêtre de Hamming au signal audio
void arduinoMFCC::apply_hamming_window() {
    for (uint8_t  n = 0; n < _frame_size; n++) {
        _frame[n] = _frame[n] * _hamming_window[n];
    }
}
/////////////////////////////////////////////////////////////////////////////
void arduinoMFCC::apply_hamming_window(float *_frame) {
    for (uint16_t  n = 0; n < _frame_size; n++) {
        _frame[n] = _frame[n] * _hamming_window[n];
    }
}
/////////////////////////////////////////////////////////////////////////////
void arduinoMFCC::apply_hamming_window(float *_frame,float *_hamming_window) {
    for (uint16_t  n = 0; n < _frame_size; n++) {
        _frame[n] = _frame[n] * _hamming_window[n];
    }
}
/////////////////////////////////////////////////////////////////////////////
// Fonction publique pour appliquer les filtres de Mel au signal audio
void arduinoMFCC::apply_mel_filter_bank() {
    for (uint8_t  i = 0; i < _num_channels; i++) {
        float output = 0.0f;
        for (uint16_t  j = 0; j < _frame_size/2; j++) {
            output += _frame[j] * _mel_filter_bank[i][j];
        }
        _mfcc_coeffs[i] = log10f(output);
    }
}
/////////////////////////////////////////////////////////////////////////////

void arduinoMFCC::apply_mel_filter_bank_power(uint16_t _frame_size,float *_frame) {
  arduinoFFT myFFTframe;
  double *_vframe= (double*)malloc(_frame_size * sizeof(double));
   double *_rframe= (double*)malloc(_frame_size * sizeof(double));
  for(uint16_t i=0;i<_frame_size;i++){
  _rframe[i]=_frame[i];
  _vframe[i]=0.0;
  }
  myFFTframe= arduinoFFT(_rframe, _vframe,  _frame_size, (double)_samplerate);
  myFFTframe.Compute(FFT_FORWARD);
  myFFTframe.ComplexToMagnitude();
   for(uint16_t i=0;i<_frame_size;i++)
  _frame[i]=(float)_rframe[i];
  free(_rframe);
  free(_vframe);
 }


/////////////////////////////////////////////////////////////////////////////
void arduinoMFCC::apply_mel_filter_bank(uint8_t  _num_channels, uint16_t  _frame_size,float *_frame,float ** _mel_filter_bank, float *_mfcc_coeffs) {
    for (uint8_t  i = 0; i < _num_channels; i++) {
        float output = 0.0f;
        for (uint16_t  j = 0; j < _frame_size/2; j++) {
            output += _frame[j] * _mel_filter_bank[i][j];
        }
        _mfcc_coeffs[i] = log10f(output);
     }
}
/////////////////////////////////////////////////////////////////////////////
// Fonction publique pour appliquer la transformée de cosinus discrète (DCT) au signal audio
void arduinoMFCC::apply_dct() {
      for (uint8_t  i = 0; i < _mfcc_size; i++) {
        _rmfcc_coeffs[i] = 0.0;
        for (uint8_t  j = 0; j < _num_channels; j++) {
            _rmfcc_coeffs[i] += _mfcc_coeffs[j] *_dct_matrix[i][j];
        }
    }
}

/////////////////////////////////////////////////////////////////////////////

// Fonction publique pour appliquer la transformée de cosinus discrète (DCT) au signal audio
void arduinoMFCC::apply_dct(uint8_t  _mfcc_size, uint8_t  _num_channels,uint16_t  _frame_size,float **_mel_filter_bank,float *_mfcc_coeffs, float *_rmfcc_coeffs) {
    for (uint8_t  i = 0; i < _mfcc_size; i++) {
        float sum = 0.0;
        for (uint8_t  j = 0; j < _num_channels; j++) {
            _rmfcc_coeffs[i] += _mfcc_coeffs[j] *_dct_matrix[i][j];
        }
        _rmfcc_coeffs[i] =sum;
    }
}
