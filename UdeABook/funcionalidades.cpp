#include "funcionalidades.h"
#include "gestorarchivos.h"

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

void ListaEnteros::ampliar() {
    int nueva = (capacidad == 0) ? 4 : capacidad * 2;
    int *nuevo = new int[nueva];

    Memoria::reservar(sizeof(int) * nueva);

    for (int i = 0; i < cantidad; i++) {
        Metricas::iterar();
        nuevo[i] = datos[i];
    }
    if (datos != 0) {
        Memoria::liberar(sizeof(int) * capacidad);
        delete[] datos;
    }
    datos = nuevo;
    capacidad = nueva;
}

ListaEnteros::ListaEnteros() : datos(nullptr), cantidad(0), capacidad(0) {}

ListaEnteros::~ListaEnteros() {
    if (datos != 0) {
        Memoria::liberar(sizeof(int) * capacidad);
        delete[] datos;
    }
}

void ListaEnteros::agregar(int valor) {
    if (cantidad == capacidad) ampliar();//Validacion si requiere ampliar espacio
    datos[cantidad++] = valor;
}

bool ListaEnteros::existe(int valor) const {//Buscar valor mediante un recorrido en ciclo
    for (int i = 0; i < cantidad; i++) {
        Metricas::iterar();
        if (datos[i] == valor) return true;
    }
    return false;
}

void ListaEnteros::eliminarL(int valor) {//Busca el valor a eliminar, desplaza los elementos posteriores a la izq
    for (int i = 0; i < cantidad; i++) {
        Metricas::iterar();
        if (datos[i] == valor) {
            for (int j = i; j < cantidad - 1; j++) {
                Metricas::iterar();
                datos[j] = datos[j + 1];
            }
            cantidad--;
            return;
        }
    }
}

int ListaEnteros::obtener(int posicion) const {//Devuelve el elemento correspondiente, si pos está dentro del rango
    if (posicion < 0 || posicion >= cantidad) return -1;
    return datos[posicion];
}

int ListaEnteros::tamano() const { return cantidad; }//Numero de elementos válidos

//Cada fila y columna representa un usuario,el valor '1' indica que son amisgos. La matriz es simétrica
MatrizAdyacencia::MatrizAdyacencia() : datos(0), n(0) {}

MatrizAdyacencia::~MatrizAdyacencia() {
    if (datos != 0) {
        Memoria::liberar(sizeof(unsigned char) * n * n);
        delete[] datos;
    }
}

void MatrizAdyacencia::crear(int cantidadUsuarios) {/*Actualiza 'n' con la cantidad de usuarios para reserva un
                                                    arreglo de n^2 elementos. */
    if (datos != 0) {
        Memoria::liberar(sizeof(unsigned char) * n * n);
        delete[] datos;
    }
    n = cantidadUsuarios;
    if (n > 0) {
        datos = new unsigned char[n * n];
        Memoria::reservar(sizeof(unsigned char) * n * n);
        for (int i = 0; i < n * n; i++) {
            Metricas::iterar();
            datos[i] = 0;
        }
    }
}

void MatrizAdyacencia::hacerAmigo(int a, int b) {//'a' y 'b' son posiciones de los usuarios en la matriz.
    if (a < 0 || b < 0 || a >= n || b >= n) return;
    datos[a * n + b] = 1;
    datos[b * n + a] = 1;
}

bool MatrizAdyacencia::amistad(int a, int b) const {//Verificar si dos usuarios son amigos
    if (a < 0 || b < 0 || a >= n || b >= n) return false;
    return datos[a * n + b] != 0;
}

int MatrizAdyacencia::cantidadComun(int a, int b) const {//Compara las filas de dos usuarios
    if (a < 0 || b < 0 || a >= n || b >= n) return 0;
    int comunes = 0;
    for (int i = 0; i < n; i++) {
        Metricas::iterar();
        if (datos[a * n + i] && datos[b * n + i]) comunes++;
    }
    return comunes;
}

int MatrizAdyacencia::cantidadAmigos(int a) const {//Recorre la fila de 'a', cuenta las posiciones distintas de 0
    if (a < 0 || a >= n) return 0;
    int total = 0;
    for (int i = 0; i < n; i++) {
        Metricas::iterar();
        if (datos[a * n + i]) total++;
    }
    return total;
}

int MatrizAdyacencia::tamano() const { return n; }//Dimensión de la matriz

static char *copiarTexto(const char *texto) {//Copia de una cadena de caracteres
    if (texto == 0) texto = "";
    int n = (int)strlen(texto);//Cantidad de caracteres
    char *copia = new char[n + 1];
    Memoria::reservar(n + 1);
    strcpy(copia, texto);//Copia la cadena completa
    return copia;
}

static void liberarTexto(char *texto) {//Calcula los bytes contabilizados para libera espacio del arreglo
    if (texto != 0) {
        Memoria::liberar((long long)strlen(texto) + 1);
        delete[] texto;
    }
}

Mensaje::Mensaje(int i, int e, int r, const char *f, bool l, const char *t)
    : id(i), emisor(e), receptor(r), fecha(copiarTexto(f)), leido(l), texto(copiarTexto(t)) {}
Mensaje::~Mensaje() { liberarTexto(fecha); liberarTexto(texto); }


int Mensaje::getId() const { return id; }
int Mensaje::getEmisor() const { return emisor; }
int Mensaje::getReceptor() const { return receptor; }
const char *Mensaje::getFecha() const { return fecha; }
const char *Mensaje::getTexto() const { return texto; }
bool Mensaje::estaLeido() const { return leido; }
void Mensaje::marcarLeido() { leido = true; }

Foto::Foto(int i, const char *n, const char *f, long long t, const char *r, int l)
    : id(i), nombre(copiarTexto(n)), fechaCaptura(copiarTexto(f)), tamano(t), ruta(copiarTexto(r)), likes(l) {}

Foto::~Foto() {
    liberarTexto(nombre);
    liberarTexto(fechaCaptura);
    liberarTexto(ruta);
}

int Foto::getId() const { return id; }
const char *Foto::getNombre() const { return nombre; }
const char *Foto::getFecha() const { return fechaCaptura; }
long long Foto::getTamano() const { return tamano; }
const char *Foto::getRuta() const { return ruta; }
int Foto::getLikes() const { return likes; }
void Foto::cambiarLike(bool agregar) {
    if (agregar) likes++;
    else if (likes > 0) likes--; }

Album::Album(int i, int p, const char *n, const char *r, PrivacidadAlbum ap)
    : id(i), propietario(p), nombre(copiarTexto(n)), ruta(copiarTexto(r)), privacidad(ap) {}

Album::~Album() {
    liberarTexto(nombre);
    liberarTexto(ruta);
}

int Album::getId() const { return id; }
int Album::getPropietario() const { return propietario; }
const char *Album::getNombre() const { return nombre; }
const char *Album::getRuta() const { return ruta; }
PrivacidadAlbum Album::getPrivacidadAlbum() const { return privacidad; }
void Album::agregarFoto(Foto *foto) { fotos.agregar(foto); }//Incorpora un puntero a la colección
int Album::cantidadFotos() const { return fotos.tamano(); }
Foto *Album::obtenerFoto(int posicion) const { return fotos.obtener(posicion); }

//Petición de amistad entre dos usuarios
Solicitud::Solicitud(int e, int r) : emisor(e), receptor(r) {}
int Solicitud::getEmisor() const { return emisor; }
int Solicitud::getReceptor() const { return receptor; }

SugerenciaAmistad::SugerenciaAmistad(int u, int c) : usuarioId(u), comunes(c) {}
int SugerenciaAmistad::getUsuarioId() const { return usuarioId; }
int SugerenciaAmistad::getComunes() const { return comunes; }

Usuario::Usuario(int i, const char *n, const char *c, const char *r)
    : id(i), nombre(copiarTexto(n)), clave(copiarTexto(c)), rutaPerfil(copiarTexto(r)) {}

Usuario::~Usuario() {
    liberarTexto(nombre);
    liberarTexto(clave);
    liberarTexto(rutaPerfil);
}

int Usuario::getId() const { return id; }
const char *Usuario::getNombre() const { return nombre; }
const char *Usuario::getClave() const { return clave; }
const char *Usuario::getRutaPerfil() const { return rutaPerfil; }

int Usuario::cantidadAmigos() const { return amigos.tamano(); }
void Usuario::agregarAmigo(int idAmigo) { if (!amigos.existe(idAmigo)) amigos.agregar(idAmigo); }/*Solamente se agrega
                                                                                    si todavía no está en la lista.*/
bool Usuario::esAmigo(int idAmigo) const { return amigos.existe(idAmigo); }
void Usuario::quitarAmigo(int idAmigo) { amigos.eliminarL(idAmigo); }
int Usuario::obtenerAmigo(int posicion) const { return amigos.obtener(posicion); }

void Usuario::agregarEnviado(int idMensaje) { enviados.agregar(idMensaje); }
void Usuario::agregarRecibido(int idMensaje) { recibidos.agregar(idMensaje); }
const ListaEnteros &Usuario::getEnviados() const { return enviados; }
const ListaEnteros &Usuario::getRecibidos() const { return recibidos; }

bool Usuario::fotoTieneLike(int idFoto) const { return fotosConLike.existe(idFoto); }/*Identificadores de las fotos
                                                                                    que el usuario ha dado like*/
void Usuario::agregarLike(int idFoto) { if (!fotosConLike.existe(idFoto)) fotosConLike.agregar(idFoto); }
void Usuario::quitarLike(int idFoto) { fotosConLike.eliminarL(idFoto); }
const ListaEnteros &Usuario::getFotosConLike() const { return fotosConLike; }

void Usuario::agregarAlbum(Album *album) { albumes.agregar(album); }
int Usuario::cantidadAlbumes() const { return albumes.tamano(); }
Album *Usuario::obtenerAlbum(int posicion) const { return albumes.obtener(posicion); }

RedSocial::RedSocial() : siguienteMensaje(1) {}
RedSocial::~RedSocial() {}

bool RedSocial::cargarDatos(const char *carpeta) {
    return GestorArchivos::cargar(*this, carpeta);
}

bool RedSocial::guardarDatos(const char *carpeta) const {
    return GestorArchivos::guardar(*this, carpeta);
}

Usuario *RedSocial::buscarIdUsuario(int id) const {
    for (int i = 0; i < usuarios.tamano(); i++) {
        Metricas::iterar();
        Usuario *u = usuarios.obtener(i);
        if (u->getId() == id) return u;
    }
    return 0;
}

Usuario *RedSocial::buscarUsuario(const char *nombre) const {
    for (int i = 0; i < usuarios.tamano(); i++) {
        Metricas::iterar();
        Usuario *u = usuarios.obtener(i);
        if (strcmp(u->getNombre(), nombre) == 0) return u;
    }
    return 0;
}

Usuario *RedSocial::autenticar(const char *nombre, const char *clave) const {
    Usuario *u = buscarUsuario(nombre);
    if (u != 0 && strcmp(u->getClave(), clave) == 0) return u;
    return 0;
}

int RedSocial::cantidadUsuarios() const { return usuarios.tamano(); }

bool RedSocial::verificarSolicitud(int emisor, int receptor) const {
    for (int i = 0; i < solicitudes.tamano(); i++) {
        Metricas::iterar();
        Solicitud *s = solicitudes.obtener(i);
        if ((s->getEmisor() == emisor && s->getReceptor() == receptor) ||
            (s->getEmisor() == receptor && s->getReceptor() == emisor)) return true;
    }
    return false;
}

bool RedSocial::enviarSolicitud(Usuario *actual, const char *destino) {
    Metricas::reiniciar();
    long long memoriaLocal = sizeof(destino) + sizeof(actual);
    Usuario *u = buscarUsuario(destino);

    if (u == 0 || u == actual || actual->esAmigo(u->getId()) || verificarSolicitud(actual->getId(), u->getId())) {
        Metricas::mostrar("Enviar solicitud", memoriaLocal);
        return false;
    }
    solicitudes.agregar(new Solicitud(actual->getId(), u->getId()));
    Metricas::mostrar("Enviar solicitud", memoriaLocal);
    return true;
}

bool RedSocial::aceptarSolicitud(Usuario *actual, int posicionListaU) {
    Metricas::reiniciar();
    long long memoriaLocal = sizeof(posicionListaU) + sizeof(actual);
    int contador = 0;

    for (int i = 0; i < solicitudes.tamano(); i++) {
        Metricas::iterar();
        Solicitud *s = solicitudes.obtener(i);
        if (s->getReceptor() == actual->getId()) {
            if (contador == posicionListaU) {
                Usuario *emisor = buscarIdUsuario(s->getEmisor());
                if (emisor != 0) {
                    actual->agregarAmigo(emisor->getId());
                    emisor->agregarAmigo(actual->getId());
                    int posA = 0;
                    int posB = 0;
                    for (int j = 0; j < usuarios.tamano(); j++) {
                        Metricas::iterar();
                        if (usuarios.obtener(j)->getId() == actual->getId()) posA = j;
                        if (usuarios.obtener(j)->getId() == emisor->getId()) posB = j;
                    }
                    matriz.hacerAmigo(posA, posB);
                    solicitudes.eliminar(i);
                    Metricas::mostrar("Aceptar solicitud", memoriaLocal);
                    return true;
                }
            }
            contador++;
        }
    }
    Metricas::mostrar("Aceptar solicitud", memoriaLocal);
    return false;
}

bool RedSocial::rechazarSolicitud(Usuario *actual, int posicionListaU) {
    Metricas::reiniciar();
    long long memoriaLocal = sizeof(posicionListaU) + sizeof(actual);
    int contador = 0;
    for (int i = 0; i < solicitudes.tamano(); i++) {
        Metricas::iterar();
        Solicitud *s = solicitudes.obtener(i);
        if (s->getReceptor() == actual->getId()) {
            if (contador == posicionListaU) {
                solicitudes.eliminar(i);
                Metricas::mostrar("Rechazar solicitud", memoriaLocal);
                return true;
            }
            contador++;
        }
    }
    Metricas::mostrar("Rechazar solicitud", memoriaLocal);
    return false;
}

void RedSocial::administrarAmistades(Usuario *actual) {
    int opcion = 0;

    do {
        cout << "\n_-_-_-_-_-Administrar Amistades-_-_-_-_-_\n";
        cout << "1. Enviar solicitud de amistad\n";
        cout << "2. Aceptar solicitudes de amistad\n";
        cout << "3. Rechazar solicitud de amistad\n";
        cout << "4. Regresar\n";
        cout << "Ingrese una opcion: ";
        cin >> opcion;

        switch (opcion) {
        case 1:{
            char nombre[128];
            cout << "Nombre de usuario: ";
            cin >> nombre;
            if (enviarSolicitud(actual, nombre)) { cout << "Solicitud enviada.\n";}
            else { cout << "No fue posible enviar la solicitud, intente de nuevo.\n";}
            break;
        }

        case 2:{
            Metricas::reiniciar();
            int cantidad = 0;
            cout << "\n_-_-_-Solicitudes Recibidas-_-_-_\n";
            for (int i = 0; i < solicitudes.tamano(); i++) {
                Metricas::iterar();
                Solicitud *s = solicitudes.obtener(i);
                if (s->getReceptor() == actual->getId()) {
                    Usuario *u = buscarIdUsuario(s->getEmisor());
                    cout << cantidad << ". " << (u ? u->getNombre() : "desconocido") << "\n";
                    cantidad++;
                }
            }
            int pos;
            cout << "Ingrese la posicion de la solicitud o '-1' para regresar: ";
            cin >> pos;
            if (pos >= 0) {
                if (aceptarSolicitud(actual, pos)) { cout << "Solicitud aceptada.\n";}
                else { cout << "Posicion invalida.\n";};
            } else {
                Metricas::mostrar("Lista solicitudes", sizeof(pos) + sizeof(cantidad));
            }
            break;
        }

        case 3:{
            Metricas::reiniciar();
            int cantidad = 0;
            cout << "\n_-_-_-Solicitudes Recibidas-_-_-_\n";
            for (int i = 0; i < solicitudes.tamano(); i++) {
                Metricas::iterar();
                Solicitud *s = solicitudes.obtener(i);
                if (s->getReceptor() == actual->getId()) {
                    Usuario *u = buscarIdUsuario(s->getEmisor());
                    cout << cantidad << ". " << (u ? u->getNombre() : "desconocido") << "\n";
                    cantidad++;
                }
            }
            int pos;
            cout << "Ingrese la posicion de la solicitud o '-1' para regresar: ";
            cin >> pos;
            if (pos >= 0) {
                if (rechazarSolicitud(actual, pos)) { cout << "Solicitud rechazada.\n";}
                else { cout << "Posicion invalida.\n";}
            } else {
                Metricas::mostrar("Lista solicitudes", sizeof(pos) + sizeof(cantidad));
            }
            break;
        }

        case 4:
            break;

        default:
            cout << "Opcion invalida. Intente nuevamente.\n";
            break;
        }
    } while (opcion != 4);
}

static int prioridadComunes(int comunes) {
    if (comunes <= 10) return 10;
    if (comunes <= 20) return 25;
    return 35;
}

static int elegirPonderado(int *prioridades, int cantidad) {
    if (cantidad <= 0) return -1;
    int total = 0;
    for (int i = 0; i < cantidad; i++) {
        Metricas::iterar();
        total += prioridades[i];
    }
    Metricas::externa();
    int aleatorio = rand() % total;
    int acumulado = 0;
    for (int i = 0; i < cantidad; i++) {
        Metricas::iterar();
        acumulado += prioridades[i];
        if (aleatorio < acumulado) return i;
    }
    return cantidad - 1;
}

void RedSocial::mostrarMejoresAmigos(Usuario *actual) {
    Metricas::reiniciar();
    long long memoriaLocal = sizeof(actual);
    int cantidad = actual->cantidadAmigos();

    if (cantidad == 0) {
        cout << "No tiene lista de amigos para mostrar.\n";
        Metricas::mostrar("Mostrar fotos de amigos", memoriaLocal);
        return;
    }

    int maximo = cantidad < 3 ? cantidad : 3;
    int *ids = new int[cantidad];
    int *prioridades = new int[cantidad];
    int *primerGrupo = new int[maximo];
    Memoria::reservar(sizeof(int) * cantidad * 2 + sizeof(int) * maximo);

    //Lista de amigos y prioridad segun los amigos en comun
    int posActual = -1;
    for (int i = 0; i < usuarios.tamano(); i++) {
        Metricas::iterar();
        if (usuarios.obtener(i)->getId() == actual->getId()) posActual = i;
    }

    int cantidadCandidatos = 0;
    for (int i = 0; i < cantidad; i++) {
        Metricas::iterar();
        int idAmigo = actual->obtenerAmigo(i);
        ids[cantidadCandidatos] = idAmigo;

        int posAmigo = -1;
        for (int j = 0; j < usuarios.tamano(); j++) {
            Metricas::iterar();
            if (usuarios.obtener(j)->getId() == idAmigo) posAmigo = j;
        }
        int comunes = (posActual >= 0 && posAmigo >= 0) ? matriz.cantidadComun(posActual, posAmigo) : 0;
        prioridades[cantidadCandidatos] = prioridadComunes(comunes);
        cantidadCandidatos++;
    }

    //Ponderado, sin repeticiones
    for (int s = 0; s < maximo; s++) {
        int disponibles = cantidadCandidatos - s;
        int *idsTemp = new int[disponibles];
        int *prioridadesTemp = new int[disponibles];
        Memoria::reservar(sizeof(int) * disponibles * 2);
        int p = 0;

        for (int i = 0; i < cantidadCandidatos; i++) {
            Metricas::iterar();
            bool usado = false;
            for (int j = 0; j < s; j++) {
                Metricas::iterar();
                if (primerGrupo[j] == ids[i]) usado = true;
            }
            if (!usado) {
                idsTemp[p] = ids[i];
                prioridadesTemp[p] = prioridades[i];
                p++;
            }
        }

        int elegido = elegirPonderado(prioridadesTemp, p);
        primerGrupo[s] = idsTemp[elegido];
        delete[] idsTemp;
        delete[] prioridadesTemp;
        Memoria::liberar(sizeof(int) * disponibles * 2);
    }

    cout << "\n_-_-_-_-_-_-Amigos Destacados-_-_-_-_-_-_\n";
    for (int i = 0; i < maximo; i++) {
        Metricas::iterar();
        Usuario *u = buscarIdUsuario(primerGrupo[i]);
        if (u != 0) {
            cout << "Amigo " << (i + 1) << ": " << u->getNombre() << "\n";
            cout << "Foto de perfil: " << u->getRutaPerfil() << "\n";
        }
    }

    //Otro grupo de 3 distintos (min. 6 amigos)
    if (cantidad >= 6 && maximo == 3) {
        int op;
        cout << "\n1. Actualizar fotos\n2. Continuar\n";
        cout << "Ingrese una opción: ";
        cin >> op;
        if (op == 1) {
            int *segundoGrupo = new int[3];
            Memoria::reservar(sizeof(int) * 3);

            for (int s = 0; s < 3; s++) {
                int disponibles = 0;
                for (int i = 0; i < cantidadCandidatos; i++) {
                    Metricas::iterar();
                    bool prohibido = false;
                    for (int j = 0; j < 3; j++) {
                        Metricas::iterar();
                        if (primerGrupo[j] == ids[i]) prohibido = true;
                    }
                    for (int j = 0; j < s; j++) {
                        Metricas::iterar();
                        if (segundoGrupo[j] == ids[i]) prohibido = true;
                    }
                    if (!prohibido) disponibles++;
                }

                int *idsTemp = new int[disponibles];
                int *prioridadesTemp = new int[disponibles];
                Memoria::reservar(sizeof(int) * disponibles * 2);
                int p = 0;
                for (int i = 0; i < cantidadCandidatos; i++) {
                    Metricas::iterar();
                    bool prohibido = false;
                    for (int j = 0; j < 3; j++) {
                        Metricas::iterar();
                        if (primerGrupo[j] == ids[i]) prohibido = true;
                    }
                    for (int j = 0; j < s; j++) {
                        Metricas::iterar();
                        if (segundoGrupo[j] == ids[i]) prohibido = true;
                    }
                    if (!prohibido) {
                        idsTemp[p] = ids[i];
                        prioridadesTemp[p] = prioridades[i];
                        p++;
                    }
                }
                int elegido = elegirPonderado(prioridadesTemp, p);
                segundoGrupo[s] = idsTemp[elegido];
                delete[] idsTemp;
                delete[] prioridadesTemp;
                Memoria::liberar(sizeof(int) * disponibles * 2);
            }

            cout << "\n_-_-_-Nuevo Grupo de Amigos Destacados-_-_-_\n";
            for (int i = 0; i < 3; i++) {
                Metricas::iterar();
                Usuario *u = buscarIdUsuario(segundoGrupo[i]);
                if (u != 0) {
                    cout << "Amigo " << (i + 1) << ": " << u->getNombre() << "\n";
                    cout << "Foto de perfil: " << u->getRutaPerfil() << "\n";
                }
            }
            delete[] segundoGrupo;
            Memoria::liberar(sizeof(int) * 3);
        }
    } else if (cantidad < 6) {
        cout << "\nNo es posible actualizar la lista con amigos diferentes, ya que tiene menos de 6 amigos.\n";
    }

    delete[] ids;
    delete[] prioridades;
    delete[] primerGrupo;
    Memoria::liberar(sizeof(int) * cantidad * 2 + sizeof(int) * maximo);
    Metricas::mostrar("Mostrar fotos de amigos", memoriaLocal);
}