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

static int convertirInt(const char *texto) {
    int numero = 0;
    int i = 0;

    while (texto[i] >= '0' && texto[i] <= '9') {
        numero = numero * 10 + (texto[i] - '0');
        i++;
    }
    return numero;
}

static long long convertirLong(const char *texto) {
    long long numero = 0;
    int i = 0;

    while (texto[i] >= '0' && texto[i] <= '9') {
        numero = numero * 10 + (texto[i] - '0');
        i++;
    }
    return numero;
}

bool GestorArchivos::cargar(RedSocial &red, const char *carpeta) {
    char ruta[512];
    char linea[4096];
    char *c[10];

    rutaArchivo(carpeta, "usuarios.txt", ruta, 512);
    ifstream usuarios(ruta);
    if (!usuarios) return false;

    while (usuarios.getline(linea, sizeof(linea))) {
        if (linea[0] == '\0' || linea[0] == '#') continue;
        int n = separarDatos(linea, c, 10);
        if (n >= 4) {
            red.usuarios.agregar(new Usuario(convertirInt(c[0]), c[1], c[2], c[3]));
        }
    }
    usuarios.close();

    red.matriz.crear(red.usuarios.tamano());

    // Amigos
    rutaArchivo(carpeta, "amistades.txt", ruta, 512);
    ifstream amistades(ruta);
    if (amistades) {
        while (amistades.getline(linea, sizeof(linea))) {
            if (linea[0] == '\0' || linea[0] == '#') continue;
            int n = separarDatos(linea, c, 10);
            if (n >= 2) {
                int a = convertirInt(c[0]);
                int b = convertirInt(c[1]);
                Usuario *ua = red.buscarIdUsuario(a);
                Usuario *ub = red.buscarIdUsuario(b);
                if (ua && ub) {
                    ua->agregarAmigo(b);
                    ub->agregarAmigo(a);
                    int pa = -1, pb = -1;
                    for (int i = 0; i < red.usuarios.tamano(); i++) {
                        Metricas::iterar();
                        if (red.usuarios.obtener(i)->getId() == a) pa = i;
                        if (red.usuarios.obtener(i)->getId() == b) pb = i;
                    }
                    red.matriz.hacerAmigo(pa, pb);
                }
            }
        }
        amistades.close();
    }

    // Albumes
    rutaArchivo(carpeta, "albumes.txt", ruta, 512);
    ifstream albumes(ruta);
    if (albumes) {
        while (albumes.getline(linea, sizeof(linea))) {
            if (linea[0] == '\0' || linea[0] == '#') continue;
            int n = separarDatos(linea, c, 10);
            if (n >= 5) {
                Usuario *u = red.buscarIdUsuario(convertirInt(c[1]));
                if (u) {
                    Album *a = new Album(convertirInt(c[0]), convertirInt(c[1]), c[2], c[3],
                                         convertirPrivacidad(convertirInt(c[4])));
                    u->agregarAlbum(a);
                }
            }
        }
        albumes.close();
    }

    // Fotos
    rutaArchivo(carpeta, "fotos.txt", ruta, 512);
    ifstream fotos(ruta);
    if (fotos) {
        while (fotos.getline(linea, sizeof(linea))) {
            if (linea[0] == '\0' || linea[0] == '#') continue;
            int n = separarDatos(linea, c, 10);
            if (n >= 7) {
                int idAlbum = convertirInt(c[1]);
                for (int i = 0; i < red.usuarios.tamano(); i++) {
                    Metricas::iterar();
                    Usuario *u = red.usuarios.obtener(i);
                    for (int j = 0; j < u->cantidadAlbumes(); j++) {
                        Metricas::iterar();
                        Album *a = u->obtenerAlbum(j);
                        if (a->getId() == idAlbum) {
                            a->agregarFoto(new Foto(convertirInt(c[0]), c[2], c[3], convertirLong(c[4]), c[5],
                                                    convertirInt(c[6])));
                        }
                    }
                }
            }
        }
        fotos.close();
    }

    // Mensajes
    rutaArchivo(carpeta, "mensajes.txt", ruta, 512);
    ifstream mensajes(ruta);
    if (mensajes) {
        while (mensajes.getline(linea, sizeof(linea))) {
            if (linea[0] == '\0' || linea[0] == '#') continue;
            int n = separarDatos(linea, c, 10);
            if (n >= 6) {
                int id = convertirInt(c[0]);
                int emisor = convertirInt(c[1]);
                int receptor = convertirInt(c[2]);
                bool leido = convertirInt(c[4]) != 0;
                Mensaje *m = new Mensaje(id, emisor, receptor, c[3], leido, c[5]);
                red.mensajes.agregar(m);
                Usuario *ue = red.buscarIdUsuario(emisor);
                Usuario *ur = red.buscarIdUsuario(receptor);
                if (ue) ue->agregarEnviado(id);
                if (ur) ur->agregarRecibido(id);
                if (id >= red.siguienteMensaje) red.siguienteMensaje = id + 1;
            }
        }
        mensajes.close();
    }

    //Likes dados por cusuario (conteo total en 'fotos.txt')
    rutaArchivo(carpeta, "likes.txt", ruta, 512);
    ifstream likes(ruta);
    if (likes) {
        while (likes.getline(linea, sizeof(linea))) {
            if (linea[0] == '\0' || linea[0] == '#') continue;
            int n = separarDatos(linea, c, 10);
            if (n >= 2) {
                Usuario *u = red.buscarIdUsuario(convertirInt(c[0]));
                if (u) u->agregarLike(convertirInt(c[1]));
            }
        }
        likes.close();
    }

    //Solicitudes pendientes
    rutaArchivo(carpeta, "solicitudes.txt", ruta, 512);
    ifstream solicitudes(ruta);
    if (solicitudes) {
        while (solicitudes.getline(linea, sizeof(linea))) {
            if (linea[0] == '\0' || linea[0] == '#') continue;
            int n = separarDatos(linea, c, 10);
            if (n >= 2) red.solicitudes.agregar(new Solicitud(convertirInt(c[0]), convertirInt(c[1])));
        }
        solicitudes.close();
    }
    return true;
}