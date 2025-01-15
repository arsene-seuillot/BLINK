import numpy as np
import matplotlib.pyplot as plt

def calculer_nombre_segments(signal, window_size, hop_size):
    """
    Calcule le nombre de segments nécessaires pour le sonagramme.
    """
    return (len(signal) - window_size) // hop_size + 1


def appliquer_fenetre_hanning(segment):
    """
    Applique une fenêtre de Hanning à un segment donné.
    """
    window = np.hanning(len(segment))
    return segment * window


def calculer_fft(segment, sr):
    """
    Calcule la FFT d'un segment et renvoie la FFT et les fréquences associées.
    """
    Y = np.fft.fft(segment)
    freqs = np.fft.fftfreq(len(segment), 1 / sr)
    return Y, freqs


def calculer_sonagramme(signal, sr, window_size, hop_size):
    """
    Calcule le sonagramme d'un signal en utilisant la FFT.
    """
    # Calcul du nombre de segments
    num_segments = calculer_nombre_segments(signal, window_size, hop_size)
    
    # Initialiser une liste pour stocker la puissance spectrale
    sonagramme = []
    
    for i in range(num_segments):
        # Découper le segment
        start = i * hop_size
        end = start + window_size
        segment = signal[start:end]
        
        # Appliquer une fenêtre de Hanning
        segment = appliquer_fenetre_hanning(segment)
        
        # Calculer la FFT
        Y, freqs = calculer_fft(segment, sr)
        
        # Calcul de la puissance spectrale
        power_spectrum = np.abs(Y[:window_size // 2]) ** 2  # Conserver seulement la moitié positive des fréquences
        
        # Ajouter au spectrogramme
        sonagramme.append(power_spectrum)
    
    # Convertir la liste en tableau numpy
    return np.array(sonagramme)  

def afficher_sonagramme(spectrogram, sr, signal_length, window_size, hop_size):
    """
    Affiche un sonagramme en utilisant Matplotlib.
    """
    spectrogram = np.array(spectrogram).T # Transposer pour avoir temps en colonnes, fréquences en lignes
    # Dimensions pour l'affichage
    temps_total = signal_length / sr  # Durée totale en secondes
    frequence_max = sr / 2  # Fréquence maximale (fréquence de Nyquist)
    
    # Tracer le sonagramme
    plt.figure(figsize=(10, 6))
    plt.imshow(10 * np.log10(spectrogram + 1e-10), aspect='auto', origin='lower',
               extent=[0, temps_total, 0, frequence_max])
    plt.colorbar(format='%+2.0f dB')
    plt.title('Sonagramme')
    plt.xlabel('Temps (s)')
    plt.ylabel('Fréquence (Hz)')
    plt.yscale('log')  # Echelle logarithmique pour l'axe Y
    plt.ylim([10, frequence_max])  # Ajuster les limites des fréquences
    
    plt.show()
