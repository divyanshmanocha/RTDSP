Fs = 8000;
b = m_filter(Fs);
freqz(b, 1, 1024, Fs);

%save filter_coeff.txt b -ASCII -DOUBLE -TABS