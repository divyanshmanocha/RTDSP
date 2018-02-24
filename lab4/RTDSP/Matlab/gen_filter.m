Fs = 8000;
b = m_filter_256;
freqz(b, 1, 1024, Fs);

%save filter_coeff_256.txt b -ASCII -DOUBLE -TABS