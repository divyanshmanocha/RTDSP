Fs = 8000;
[b,a] = elliptical (32, Fs);

fileID = fopen('coeff.txt','w');
fprintf(fileID, 'double a[] = {');
fprintf(fileID, '%.15e, ', a);
fprintf(fileID, '};\ndouble b[] = {');
fprintf(fileID, '%.15e, ', b);
fprintf(fileID, '};\n');
fclose(fileID);