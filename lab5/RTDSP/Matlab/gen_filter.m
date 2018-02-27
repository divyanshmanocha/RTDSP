Fs = 8000;
[b,a] = elliptical (Fs);
freqz(b, a);

%save filter_coeff_ell_b.txt b -ASCII -DOUBLE -TABS
%save filter_coeff_ell_a.txt a -ASCII -DOUBLE -TABS