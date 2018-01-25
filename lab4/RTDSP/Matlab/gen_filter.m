Fs = 8000;
b = Untitled;
freqz(b, 1, 1024, Fs);

save filter_coeff_2.txt b -ASCII -DOUBLE -TABS