# arduino MFCC (Mel-Frequency Cepstral Coefficients)

La méthode MFCC est une technique largement utilisée pour extraire des caractéristiques pertinentes du signal audio. Elle est couramment utilisée dans la reconnaissance vocale, la classification des genres musicaux et diverses autres applications liées au traitement du signal audio.

## Principe de base

Le processus d'extraction des coefficients MFCC consiste en plusieurs étapes :

1. **Prétraitement** : Le signal audio est prétraité en appliquant un pré-emphase pour augmenter les hautes fréquences. Cela permet de réduire les problèmes liés à l'atténuation des hautes fréquences dans les systèmes de transmission. La formule de pré-emphase est la suivante :

y[n] = x[n] - α * x[n-1]

où `x(t)` est le signal d'entrée, `y(t)` est le signal de sortie et `α` est le coefficient de pré-emphase (généralement compris entre 0,95 et 0,97).

2. **Segmentation** : Le signal est divisé en trames de taille fixe (généralement 20 à 40 ms) avec un certain chevauchement entre les trames adjacentes. Cette étape est nécessaire pour rendre compte de la nature non stationnaire des signaux audio.

3. **Fenêtrage** : Une fonction de fenêtre (par exemple, Hamming ou Hanning) est appliquée à chaque trame pour minimiser les discontinuités aux bords de la trame. La fenêtre est définie comme suit :

w[n] = 0,5 * (1 - cos(2 * π * n / (N - 1)))

où `w(n)` est la valeur de la fenêtre à l'échantillon `n` et `N` est la taille de la trame.

4. **Transformée de Fourier à court terme (STFT)** : La transformée de Fourier à court terme est appliquée à chaque trame pour obtenir le spectre de puissance. Cette étape permet de passer du domaine temporel au domaine fréquentiel.

5. **Filtres Mel** : Le spectre de puissance est filtré à l'aide d'un banc de filtres en triangle espacés selon l'échelle de fréquence Mel. L'échelle Mel est une échelle de fréquence perceptuelle qui tient compte de la manière dont l'oreille humaine perçoit les fréquences. La relation entre la fréquence Mel `m` et la fréquence linéaire `f` est la suivante :

m = 2595 * log10(1 + f / 700)

6. **Logarithme** : Le logarithme de l'énergie de chaque filtre Mel est calculé. Cette étape permet de compresser les données en réduisant l'échelle dynamique.

7. **Transformée en cosinus discrète (DCT)** : Enfin, la transformée en cosinus discrète est appliquée au spectre log-Mel pour obtenir les coefficients MFCC. Seuls les premiers `k` coefficients sont généralement conservés, où `k`



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
