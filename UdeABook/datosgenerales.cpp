#include "datosgenerales.h"

//Inicializacion de atributos estáticos
long long Memoria::bytes = 0;
long long Metricas::iteraciones = 0;
long long Metricas::externas = 0;

void Memoria::reservar(long long cantidad) { bytes += cantidad; }//+cantidad recibida a bytes existentes
void Memoria::liberar(long long cantidad) {//Restar bytes liberados
    bytes -= cantidad;
    if (bytes < 0) {
        bytes = 0;
    }
}
long long Memoria::actual() { return bytes; }

void Metricas::reiniciar() { iteraciones = 0; externas = 0; } //Reiniciar para no tomar datos de iteraciones anteriores
void Metricas::iterar(long long cantidad) { iteraciones += cantidad; }
void Metricas::externa(long long cantidad) { externas += cantidad; }
long long Metricas::getIteraciones() { return iteraciones; }
long long Metricas::getExternas() { return externas; }
//Imprimir metricas
void Metricas::mostrar(const char *nombre, long long memoriaLocal) {
    cout << "\n-_-_-_-_-_-_-_- Metricas -_-_-_-_-_-_-_- " << nombre << endl;
    cout << "Iteraciones (directas e indirectas): " << iteraciones << endl;//Iteraciones contabilizadas
    cout << "Componentes externos invocados: " << externas << "\n";//Llamadas externas registradas
    cout << "Memoria (aproximada): " << Memoria::actual() + memoriaLocal << " bytes\n";//Memoria contabilizada globalmente + Memoria local declarada
}
