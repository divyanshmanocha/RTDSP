Fs = 8000;
b = m_filter;
freqz(b, 1, 1024, Fs);

save filter_coeff_correct.txt b -ASCII -DOUBLE -TABS