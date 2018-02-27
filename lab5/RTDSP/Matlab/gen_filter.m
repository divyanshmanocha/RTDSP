Fs = 8000;
[b,a] = elliptical (Fs);
freqz(b, a);

fileID = fopen('coeff2.txt','w');
fprintf(fileID, 'double a[] = {');
fprintf(fileID, '%.15e, ', a);
fprintf(fileID, '};\ndouble b[] = {');
fprintf(fileID, '%.15e, ', b);
fprintf(fileID, '};');
fclose(fileID);