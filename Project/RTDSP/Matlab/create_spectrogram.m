%%
% Creates the spectrogram for the audio files

[song, fs] = audioread('../audio/best_case/car.wav');
song = song(1:fs*10);
figure
spectrogram(song, 256, [], [], fs, 'yaxis');

%%
[song, fs] = audioread('../audio/original/car1.wav');
song = song(1:fs*10);
figure
spectrogram(song, 256, [], [], fs, 'yaxis');

%%
[song, fs] = audioread('../audio/original/clean.wav');
song = song(1:fs*10);
figure
spectrogram(song, 256, [], [], fs, 'yaxis');
