Fs = 8000;
[b,a] = elliptical (Fs);
% [y, x] = freqz(b, a, 2000,8000);
% plot(x, abs(y));

fileID = fopen('coeff.txt','w');
fprintf(fileID, 'double a[] = {');
fprintf(fileID, '%.15e, ', a);
fprintf(fileID, '};\ndouble b[] = {');
fprintf(fileID, '%.15e, ', b);
fprintf(fileID, '};\n');
fclose(fileID);