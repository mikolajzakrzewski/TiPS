import sounddevice as sd
import numpy as np
from scipy.io.wavfile import read, write

allowed_quantization_values = [8, 16]


def quantize(recording_data, quantization_value):
    max_value = 2 ** (quantization_value - 1) - 1
    normalized_data = np.float64(recording_data / np.max(abs(recording_data)))
    scaled_data = np.round(normalized_data * max_value)
    return scaled_data


def main():
    while True:
        choice = input("Wybierz operację do wykonania:\n1. Nagraj dźwięk\n2. Odtwórz dźwięk\n3. Wyjdź z programu\n")
        if choice == '1':
            while True:
                print("Podaj poziom kwantyzacji (8 / 16):")
                quantization_value = int(input())
                if quantization_value not in allowed_quantization_values:
                    print("Niepoprawny wybór. Wybierz jeszcze raz.")

                else:
                    break

            sample_rate = int(input("Podaj częstotliwość próbkowania [Hz]: "))
            recording_duration = int(input("Podaj czas nagrania [s]: "))

            print("Nagrywanie dźwięku...")
            recording_data = sd.rec(int(recording_duration * sample_rate), samplerate=sample_rate, channels=1)
            sd.wait()
            quantized_data = quantize(recording_data, quantization_value)

            write("recording.wav", sample_rate, quantized_data)
            print("Zakończono nagrywanie.")

        elif choice == '2':
            recording_data = read("recording.wav")
            sample_rate = recording_data[0]
            print("Odtwarzanie dźwięku...")
            recording_data = np.float64(recording_data[1] / np.max(abs(recording_data[1])))
            sd.play(recording_data, samplerate=sample_rate)
            sd.wait()

        elif choice == '3':
            break

        else:
            print("Niepoprawny wybór. Wybierz jeszcze raz.")


main()
