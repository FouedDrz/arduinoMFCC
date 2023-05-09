# arduino MFCC (Mel-Frequency Cepstral Coefficients)

La méthode MFCC est une technique largement utilisée pour extraire des caractéristiques pertinentes du signal audio. Elle est couramment utilisée dans la reconnaissance vocale, la classification des genres musicaux et diverses autres applications liées au traitement du signal audio.

## Principe de base

Le processus d'extraction des coefficients MFCC consiste en plusieurs étapes :

1. **Prétraitement** : Le signal audio est prétraité en appliquant un pré-emphase pour augmenter les hautes fréquences.
2. **Segmentation** : Le signal est divisé en trames de taille fixe (généralement 20 à 40 ms) avec un certain chevauchement entre les trames adjacentes.
3. **Fenêtrage** : Une fonction de fenêtre (par exemple, une fenêtre de Hamming) est appliquée à chaque trame pour minimiser les discontinuités aux bords de la trame.
4. **Transformée de Fourier** : La transformée de Fourier à court terme (STFT) est appliquée à chaque trame pour obtenir le spectre de puissance.
5. **Filtres de Mel** : Le spectre de puissance est passé à travers un ensemble de filtres de Mel pour capturer les caractéristiques perceptuelles du signal audio.
6. **Logarithme** : On prend le logarithme de l'énergie dans chaque bande de Mel pour imiter la perception humaine de l'intensité.
7. **Transformée en cosinus discrète** : Enfin, la transformée en cosinus discrète (DCT) est appliquée aux coefficients de Mel pour obtenir les coefficients cepstraux.

Les premiers coefficients MFCC (généralement 12 à 20) sont utilisés comme caractéristiques pour représenter le signal audio.

## Implémentations

Il existe plusieurs bibliothèques et outils pour calculer les coefficients MFCC à partir de signaux audio. Voici quelques exemples :

- [Librosa](https://librosa.org/doc/latest/feature.html#mel-frequency-cepstral-coefficients-mfccs) (Python)
- [SpeechRecognition](https://pypi.org/project/SpeechRecognition/) (Python)
- [Kaldi](https://kaldi-asr.org/) (C++)

## Applications

Les MFCC sont utilisés dans diverses applications liées au traitement du signal audio, notamment :

- Reconnaissance vocale
- Classification des genres musicaux
- Reconnaissance des instruments de musique
- Analyse des émotions dans la parole

## Références

- [Davis, S. B., & Mermelstein, P. (1980). Comparison of parametric representations for monosyllabic word recognition in continuously spoken sentences. IEEE Transactions on Acoustics, Speech, and Signal Processing, 28(4), 357-366.](https://doi.org/10.1109/TASSP.1980.1163420)
- [Slaney, M. (1998). Auditory Toolbox Version 2. Technical Report.](http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.130.8506&rep=rep1&type=pdf)
