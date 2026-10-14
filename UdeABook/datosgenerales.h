#ifndef DATOSGENERALES_H
#define DATOSGENERALES_H

#include <iostream>

using namespace std;

//Memoria dinámica para la ejecucion
class Memoria{
public:
    static void reservar(long long cantidad);
    static void liberar(long long cantidad);
    static long long actual();
private:
    static long long bytes;

};

//Mediciones de funcionamiento
class Metricas {

private:
    static long long iteraciones;
    static long long externas;
public:
    static void reiniciar();
    static void iterar(long long cantidad = 1);
    static void externa(long long cantidad = 1);
    static long long getIteraciones();
    static long long getExternas();
    static void mostrar(const char *nombre, long long memoriaLocal);
};

//Estructura de datos dinámica propia-Clase plantilla
template <class T>
class ListaDinamica {
private:
    T **datos;//Puntero a punteros de tipo T
    int cantidad;//Elementos actuales almacenados
    int capacidad;//Posiciones reservadas

    //Ampliar capacidad de memoria cuando el arreglo esta lleno
    void ampliar() {
        int nuevaCapacidad = (capacidad == 0) ? 4 : capacidad * 2;
        //Nueva reserva de memoria
        T **nuevo = new T*[nuevaCapacidad];
        Memoria::reservar(sizeof(T*) * nuevaCapacidad);

        //Copiar las direcciones de los objetos del arreglo anterior al nuevo
        for (int i = 0; i < cantidad; i++) {
            Metricas::iterar();
            nuevo[i] = datos[i];
        }

        //liberar memoria del arreglo existente
        if (datos != 0) {
            Memoria::liberar(sizeof(T*) * capacidad);
            delete[] datos;
        }
        //Se actualizan los atributos
        datos = nuevo;
        capacidad = nuevaCapacidad;
    }

public:
    ListaDinamica() : datos(nullptr), cantidad(0), capacidad(0) {}//Constructor

    ~ListaDinamica() {//Destructor
        for (int i = 0; i < cantidad; i++) {
            Metricas::iterar();
            delete datos[i];//Destruye el objeto al que apunta i
        }
        if (datos != 0) {
            Memoria::liberar(sizeof(T*) * capacidad);
            delete[] datos;//Destruye el arreglo que almacena los punteros
        }
    }

    //Agrega un objeto a la lista
    void agregar(T *elemento) {
        if (cantidad == capacidad) {//Comprobacion de capacidad
            ampliar();
        }
        datos[cantidad] = elemento;
        cantidad++;
    }

    //Cantidad de elementos almacenados
    int tamano() const {
        return cantidad;
    }

    //Obtener puntero ubicado en una posición.
    T *obtener(int posicion) const {
        if (posicion < 0 || posicion >= cantidad) {
            return 0;
        }
        return datos[posicion];
    }

    //Elimina un objeto de su posición
    void eliminar(int posicion) {
        if (posicion < 0 || posicion >= cantidad){
            return;
        }
        delete datos[posicion];//Libera del objeto
        for (int i = posicion; i < cantidad - 1; i++) {//Desplazamiento hacia izquierda posterior a la eliminacion
            Metricas::iterar();
            datos[i] = datos[i + 1];
        }
        cantidad--;//Actualizacion cantidad de elementos
    }

    //Retirar dato de la lista, sin destruir el objeto
    void nonEliminar(int posicion) {
        if (posicion < 0 || posicion >= cantidad){
            return;
        }
        for (int i = posicion; i < cantidad - 1; i++) {
            Metricas::iterar();
            datos[i] = datos[i + 1];
        }
        cantidad--;
    }
};

#endif // DATOSGENERALES_H
