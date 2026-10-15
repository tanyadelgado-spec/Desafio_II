#ifndef GESTORARCHIVOS_H
#define GESTORARCHIVOS_H

#include <cstring>
#include <cstdio>
#include <fstream>

#include "interacciones.h"
#include "datosgenerales.h"

//Recuperar datos almacenados y escribirlos en archivos
class GestorArchivos{
public:
    static bool cargar(RedSocial &red, const char *carpeta);
    static bool guardar(const RedSocial &red, const char *carpeta);
};

#endif // GESTORARCHIVOS_H
