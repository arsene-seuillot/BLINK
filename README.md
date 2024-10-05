# BLINK

**BLINK** is a program with two main features.

1. It creates an audio interface between an input and an output, with the ability to process sound in real-time in between. For instance, examples of delay, reverb, and saturation effects have been implemented.

2. The second and primary purpose is to transform a polyphonic input signal into data representing only the frequencies being played. This allows for identifying, at any given moment, the notes being played, and subsequently recreating an artificial signal composed of pure tones corresponding to the input signal.

A significant part of the project involves mastering signal processing tools and algorithms to understand how to handle digital signals for music. We will explore how to visualize the signal in the frequency domain in real-time using spectrograms, and how to manipulate this signal. Additionally, we will cover various methods for programming effects, such as implementing a delay using a circular buffer, as well as different types of filters like gain filters and convolution filters.
