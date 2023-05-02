#include <Arduino.h>

const int samplingFrequency = 44000;
const int bufferSize = 512;

volatile uint32_t adcBuffer[bufferSize];

void setupADC() {
  PMC->PMC_PCER1 |= PMC_PCER1_PID37; // Active le périphérique ADC
  ADC->ADC_MR = ADC_MR_PRESCAL(255)  // Définit le diviseur de fréquence à 255
              | ADC_MR_STARTUP_SUT64 // Définit le temps de démarrage à 64 périodes d'ADC_CLK
              | ADC_MR_TRACKTIM(15)  // Définit le temps de suivi à 15 périodes d'ADC_CLK
              | ADC_MR_SETTLING_AST3;// Définit le temps de stabilisation à 17 périodes d'ADC_CLK
  ADC->ADC_CHER = ADC_CHER_CH7;      // Active le canal 7 (A0)

  // Configure Timer Counter 0 Channel 0 (TC0) pour samplingFrequency
  PMC->PMC_PCER0 |= PMC_PCER0_PID27; // Active le périphérique TC0
  TC0->TC_CHANNEL[0].TC_CMR = TC_CMR_TCCLKS_TIMER_CLOCK4 | TC_CMR_CPCTRG; 
  // Définit la source d'horloge à TCLK4 (MCK / 128, 84 MHz / 128 = 656.25 kHz)
  // Active le déclenchement de comparaison RC
  // Définit la valeur RC pour une fréquence samplingFrequency Hz
  TC0->TC_CHANNEL[0].TC_RC = 656250 / samplingFrequency - 1;
  // Active l'interruption de comparaison RC
  TC0->TC_CHANNEL[0].TC_IER = TC_IER_CPCS;
  // Active l'interruption TC0_IRQn dans le NVIC
  NVIC_EnableIRQ(TC0_IRQn);

  // Configure le contrôleur DMA
  PMC->PMC_PCER1 |= PMC_PCER1_PID39; // Active le périphérique PDC
  ADC->ADC_PTCR = ADC_PTCR_RXTDIS | ADC_PTCR_TXTDIS; // Désactive le transfert PDC
  ADC->ADC_RPR = (uint32_t)adcBuffer; // Définit le pointeur de réception sur le tampon
  ADC->ADC_RCR = bufferSize; // Définit le compteur de réception à la taille du tampon
  ADC->ADC_RNPR = (uint32_t)adcBuffer; // Définit le prochain pointeur de réception sur le tampon
  ADC->ADC_RNCR = bufferSize; // Définit le prochain compteur de réception à la taille du tampon
  ADC->ADC_PTCR = ADC_PTCR_RXTEN; // Active le transfert PDC
}

void TC0_Handler() {
  // Lit le registre d'état pour effacer le drapeau d'interruption
  TC0->TC_CHANNEL[0].TC_SR; 
  // Démarre une nouvelle conversion ADC
  ADC->ADC_CR = ADC_CR_START;
}

void setup() {
  Serial.begin(115200);
  setupADC();
  // Active le compteur de temps et le déclenche
  TC0->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG;
}

void loop() {
// Vérifie si le transfert DMA est terminé
if (ADC->ADC_ISR & ADC_ISR_ENDRX) {
// Désactive le transfert PDC
ADC->ADC_PTCR = ADC_PTCR_RXTDIS | ADC_PTCR_TXTDIS;
// Imprime les valeurs ADC sur le moniteur série
for (int i = 0; i < bufferSize; i++) {
Serial.println(adcBuffer[i]);
}
// Réactive le transfert PDC
ADC->ADC_PTCR = ADC_PTCR_RXTEN;
// Réinitialise le pointeur de réception et le compteur
ADC->ADC_RPR = (uint32_t)adcBuffer;
ADC->ADC_RCR = bufferSize;
// Réinitialise le prochain pointeur de réception et le compteur
ADC->ADC_RNPR = (uint32_t)adcBuffer;
ADC->ADC_RNCR = bufferSize;
}
}
