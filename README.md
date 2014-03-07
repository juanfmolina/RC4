RC4
===

Sexto laboratorio de la asignatura Sistemas Operativos referente a la seguridad, probado con los vectores del RFC.

El RC4 se ejecuta en dos fases: la primera parte es el algoritmo de planificación de la clave, KSA que toma un arreglo S o S-box para derivar una permutación de {0,1,2,..... ,N-1} utilizando un tamaño k para la variable clave. La segunda parte es la parte de generación de PRGA que produce bytes pseudo aleatorios  utilizando la permutación de KSA. Cada iteración o bucle produce un valor de salida. El texto plano es bit a bit pasado por una Xored con el keyStream para producir el texto encriptad.
En la mayoría de las aplicaciones se utiliza RC4 con una longitud de palabra n = 8 bits de un N = 256. El símbolo l indica la longitud de bytes de la clave secreta.


KSA (K, S)
for i = 0 to N -1
S[i] = i
j = 0

for i = 0 to N - 1
j = (j + S[i] + K[i mod l]) mod N
Swap(S[i], S[j])


PRGA(S)
i = 0
j = 0
Output Generation loop
i = (i + 1) mod N
j = (j + S[i]) mod N
Swap(S[i]; S[j])
Output=S[(S[i] + S[j]) mod N] 

