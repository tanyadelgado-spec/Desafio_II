#ifndef DATOSGENERALES_H
#define DATOSGENERALES_H


//Memoria dinámica para la ejecucion
class Memoria{
public:
    static void reservar(long long cantidad);
    static void liberar(long long cantidad);
    static long long actual();
private:
    static long long bytes;

};

#endif // DATOSGENERALES_H
