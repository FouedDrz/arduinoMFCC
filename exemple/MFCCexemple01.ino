#include <Arduino.h>
#include "arduinoMFCC.h"
// Définition des paramètres MFCC
const int num_channels = 12;
const int frame_size = 256;
const int hop_size = 128;
const int mfcc_size = 6;
const float sample_rate=8000.;
// Déclaration de l'objet MFCC
arduinoMFCC mymfcc(num_channels, frame_size, hop_size, mfcc_size, sample_rate);
//
//exemple de vecteur data
float frame[256]={
0.00006,0.00067,0.00104,0.00095,0.00146,0.00217,0.00299,0.00339
,0.00345,0.00342,0.00357,0.00293,0.00388,0.00427,0.00507,0.00443
,0.00409,0.00415,0.00333,0.00314,0.00317,0.00317,0.00372,0.00372
,0.00269,0.00339,0.00372,0.00375,0.00336,0.00259,0.00259,0.00323
,0.00281,0.00311,0.00366,0.00360,0.00430,0.00470,0.00406,0.00284
,0.00214,0.00214,0.00104,0.00092,0.00101,0.00134,0.00208,0.00339
,0.00375,0.00418,0.00519,0.00473,0.00513,0.00415,0.00388,0.00311
,0.00250,0.00198,0.00183,0.00253,0.00259,0.00317,0.00336,0.00351
,0.00336,0.00247,0.00238,0.00204,0.00281,0.00348,0.00397,0.00339
,0.00299,0.00372,0.00330,0.00339,0.00369,0.00369,0.00381,0.00436
,0.00381,0.00409,0.00394,0.00391,0.00446,0.00494,0.00507,0.00577
,0.00586,0.00552,0.00644,0.00659,0.00681,0.00549,0.00531,0.00522
,0.00467,0.00421,0.00461,0.00507,0.00546,0.00592,0.00589,0.00620
,0.00580,0.00555,0.00568,0.00623,0.00711,0.00748,0.00812,0.00797
,0.00833,0.00827,0.00772,0.00635,0.00558,0.00537,0.00580,0.00668
,0.00735,0.00827,0.00821,0.00766,0.00653,0.00528,0.00485,0.00485
,0.00467,0.00446,0.00488,0.00525,0.00555,0.00604,0.00653,0.00726
,0.00665,0.00684,0.00754,0.00748,0.00809,0.00882,0.00845,0.00784
,0.00797,0.00739,0.00729,0.00693,0.00717,0.00714,0.00787,0.00809
,0.00867,0.00751,0.00754,0.00705,0.00598,0.00562,0.00476,0.00436
,0.00403,0.00458,0.00433,0.00574,0.00665,0.00760,0.00803,0.00757
,0.00677,0.00638,0.00674,0.00604,0.00629,0.00607,0.00632,0.00644
,0.00668,0.00668,0.00629,0.00577,0.00528,0.00574,0.00534,0.00510
,0.00595,0.00580,0.00562,0.00577,0.00565,0.00543,0.00540,0.00653
,0.00623,0.00684,0.00687,0.00726,0.00616,0.00525,0.00436,0.00363
,0.00433,0.00452,0.00381,0.00443,0.00568,0.00583,0.00598,0.00641
,0.00580,0.00494,0.00391,0.00391,0.00385,0.00485,0.00482,0.00549
,0.00571,0.00586,0.00607,0.00647,0.00647,0.00613,0.00644,0.00604
,0.00650,0.00626,0.00732,0.00830,0.00772,0.00781,0.00775,0.00757
,0.00647,0.00641,0.00607,0.00626,0.00659,0.00604,0.00613,0.00668
,0.00641,0.00601,0.00592,0.00620,0.00586,0.00565,0.00504,0.00534
,0.00507,0.00519,0.00540,0.00494,0.00549,0.00552,0.00531,0.00522};
// Déclaration de l'objet Audio
float **mel_filter   = (float **)malloc(num_channels * sizeof(float *));
float **dct_matrix   = (float **)malloc(mfcc_size* sizeof(float *));

float *hwindows      = (float *)malloc(frame_size* sizeof(float *));
float *mfcc_coeffs   =(float *)malloc(num_channels * sizeof(float *));
float *rmfcc_coeffs  =(float *)malloc(mfcc_size * sizeof(float *));

void setup() {
  Serial.begin(9600);
  while(!Serial);

  // Initialisation de mémoire
        for (int i = 0; i < num_channels; i++) {
             mel_filter[i] = (float *)malloc((frame_size/2) * sizeof(float));
             }
        for (int i = 0; i < mfcc_size; i++) {
              dct_matrix[i] = (float *)malloc(num_channels* sizeof(float));
             }             
       
mymfcc.create_hamming_window(256, hwindows);
mymfcc.create_mel_filter_bank(sample_rate,num_channels,frame_size, mel_filter); 
//
mymfcc.pre_emphasis(frame_size, frame);    
mymfcc.apply_hamming_window(frame, hwindows);
mymfcc.apply_mel_filter_bank_power(frame_size,frame);
mymfcc.apply_mel_filter_bank(num_channels,frame_size,frame,mel_filter,mfcc_coeffs);
}

void loop() {

/*
 for(int i=0 ;i<mfcc_size;i++)
  {  Serial.println( rmfcc_coeffs[i]);
    //for(int j=0 ;j<frame_size/2 ;j++)
//Serial.println(mel_filter[i][j]);
  }
*/
   for(int i=0 ;i<num_channels;i++)
  {  Serial.println(mfcc_coeffs[i]);
    //for(int j=0 ;j<frame_size/2 ;j++)
//Serial.println(mel_filter[i][j]);
  }

  
}
