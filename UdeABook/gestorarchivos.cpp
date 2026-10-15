#include "gestorarchivos.h"

//Construir la ruta completa de un archivo con el nombre de la carpeta y del archivo.
static void rutaArchivo(const char *carpeta, const char *nombre, char *salida, int capacidad) {
    int n = sprintf(salida, "%s/%s", carpeta, nombre);/*'sprintf'. Concatena la carpeta una barra '/' y el nombre del
                                                        archivo, guarda el resultado en 'salida'.
                                                        Devuelve numero entero 'n'(cantidad de caracteres que se escribieron)*/
    if (n < 0 || n >= capacidad) salida[0] = '\0';
}

//Dividir una línea de texto en campos separados por '|' (usuarios.txt)
static int separarDatos(char *linea, char **campos, int maxCampos) {
    int cantidad = 0;
    char *p = strtok(linea, "|");//'strtok()'. Busca los fragmentos delimitados por |.
    while (p != 0 && cantidad < maxCampos) {
        campos[cantidad++] = p;
        p = strtok(0, "|");
    }
    return cantidad;//Devuelve cantidad campos encontrados
}

//Convertir num leído en un valor del tipo PrivacidadAlbum
static PrivacidadAlbum convertirPrivacidad(int ap) {
    if (ap == PRIVADO) return PRIVADO;
    if (ap == AMIGOS) return AMIGOS;
    return PUBLICO;
}