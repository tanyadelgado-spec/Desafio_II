#ifndef FUNCIONALIDADES_H
#define FUNCIONALIDADES_H

#include <iostream>
#include <cstring>

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

//Lista dinámica para numeros enteros
class ListaEnteros {

private:
    int *datos;
    int cantidad; //Cantidad valores almacenados
    int capacidad;//Capacidad de almacenamiento
    void ampliar();//Ampliar la capacidad
public:
    ListaEnteros();
    ~ListaEnteros();

    void agregar(int valor);//Insertar valor
    bool existe(int valor) const; //Verifica si un valor existe
    void eliminarL(int valor);//Elimina un valor de la lista
    int obtener(int posicion) const;//Consultar el entero en una posición
    int tamano() const;//Cantidad de elementos.
};

//Representa las amistades con matriz de adyacencia (si dos usuarios están conectados)
class MatrizAdyacencia {
private:
    unsigned char *datos;//Apuntador a la memoria donde se almacena la representación de la matriz
    int n;//Dimensiones de la matriz
public:
    MatrizAdyacencia();
    ~MatrizAdyacencia();

    void crear(int cantidadUsuarios);//Cantidad de usuarios a representar
    void hacerAmigo(int a, int b);//Registra relación de amistad entre los usuarios
    bool amistad(int a, int b) const;//Consultar si los usuarios son amigos
    int cantidadComun(int a, int b) const;//Cantidad de amigos en comun
    int cantidadAmigos(int a) const;//Cantidad de amigos de un usuario
    int tamano() const;//Tamaño actual de la matriz
};

//Registro de mensajes enviados
class Mensaje {
private:
    int id;//identificador del mensaje
    int emisor;//identificador de usuario que envió mensaje
    int receptor;//identificador de usuario que recibe mensaje
    char *fecha;
    bool leido;//Verificacion de mensaje leido o no
    char *texto;//Puntero a cadena que contiene el mensaje
public:
    Mensaje(int id, int emisor, int receptor, const char *fecha, bool leido, const char *texto);
    ~Mensaje();
    int getId() const;//Devuelve el identificador del mensaje
    int getEmisor() const;
    int getReceptor() const;
    const char *getFecha() const;
    const char *getTexto() const;
    bool estaLeido() const;//'True' para estado de lectura: leido
    void marcarLeido();//Actualiza el estado de lectura del mensaje
};

//Representa de las fotos almacenadas
class Foto {
private:
    int id;//Identificador de la foto
    char *nombre;//Nombre de la foto
    char *fechaCaptura;//Fecha en la que se tomó la foto
    long long tamano;//Tamaño de la foto
    char *ruta;//Ubicación del archivo foto
    int likes;//Cantidad de likes registrados
public:
    Foto(int id, const char *nombre, const char *fechaCaptura, long long tamano, const char *ruta, int likes);//Constructor
    ~Foto();
    int getId() const;
    const char *getNombre() const;
    const char *getFecha() const;
    long long getTamano() const;
    const char *getRuta() const;
    int getLikes() const;
    void cambiarLike(bool agregar);//Modificar contador de likes
};

//Tipo enumerado para representar las opciones de privacidad de los alnumes
enum PrivacidadAlbum {
    PUBLICO = 0,
    PRIVADO = 1,
    AMIGOS = 2
};

//Representa los albumes almacenados
class Album {
private:
    int id;//Identificador del album
    int propietario;//Identificador del usuario propietario del album
    char *nombre;//Nombre del album
    char *ruta;//Ruta de almacenamiento
    PrivacidadAlbum privacidad;//Configuración de privacidad del album
    ListaDinamica<Foto> fotos;
public:
    Album(int id, int propietario, const char *nombre, const char *ruta, PrivacidadAlbum privacidad);
    ~Album();
    int getId() const;
    int getPropietario() const;
    const char *getNombre() const;
    const char *getRuta() const;
    PrivacidadAlbum getPrivacidadAlbum() const;
    void agregarFoto(Foto *foto);//Agregar foto al album.
    int cantidadFotos() const;//Cantidad de fotos en el album
    Foto *obtenerFoto(int posicion) const;
};

//Solicitudes de amistad entre dos usuarios
class Solicitud {
private:
    int emisor;//Identificador del usuario que envía la solicitud
    int receptor;//Identificador del usuario al que se envia la solicitud
public:
    Solicitud(int emisor, int receptor);
    int getEmisor() const;
    int getReceptor() const;
};

//Recomendacion de amistad
class SugerenciaAmistad {
private:
    int usuarioId;//Identificador del usuario que se esta recomendando
    int comunes;//Cantidad de amigos que el usuario actual tiene en común con el usuario sugerido
public:
    SugerenciaAmistad(int usuarioId, int comunes);
    int getUsuarioId() const;
    int getComunes() const;
};

//Informacion de usuarios
class Usuario {
private:
    int id;//Identificador del usuario
    char *nombre;//Nombre del usuario
    char *clave;//Clave asociada a la cuenta
    char *rutaPerfil;//Ruta asociada al perfil
    ListaEnteros amigos;//Identificadores de los usuarios que son amigos de este usuario
    ListaEnteros enviados;//Identificadores de los mensajes enviados por el usuario
    ListaEnteros recibidos;//Identificadores de los mensajes recibidos por el usuario
    ListaEnteros fotosConLike;//Identificadores de las fotos a las que ha dado like
    ListaDinamica<Album> albumes;//Albumes del usuario
public:
    Usuario(int id, const char *nombre, const char *clave, const char *rutaPerfil);
    ~Usuario();

    int getId() const;
    const char *getNombre() const;
    const char *getClave() const;
    const char *getRutaPerfil() const;
    int cantidadAmigos() const;//Cantidad de amigos registrados

    void agregarAmigo(int idAmigo);//Agrega el identificador de un usuario a la lista de amigos
    bool esAmigo(int idAmigo) const;//Comprueba si un identificador esta en la lista de amigos
    void quitarAmigo(int idAmigo);//Eliminr amigo de la lista
    int obtenerAmigo(int posicion) const;//Identificador del amigo que se encuentra en una posición determinada
    void agregarEnviado(int idMensaje);//Agrega el identificador de un mensaje a lista de mensajes enviados
    void agregarRecibido(int idMensaje);//Agrega el identificador de un mensaje a lista de mensajes recibidos
    const ListaEnteros &getEnviados() const;//Devuelve una referencia constante a la lista de mensajes enviados
    const ListaEnteros &getRecibidos() const;

    bool fotoTieneLike(int idFoto) const;//Verificar si el usuario ha dado like a una foto
    void agregarLike(int idFoto);//Agrega like a una foto
    void quitarLike(int idFoto);//Elimina el registro de like asociado a una foto
    const ListaEnteros &getFotosConLike() const;//Consultar la lista de fotos a las que el usuario ha dado like

    void agregarAlbum(Album *album);//Agregar álbum a la coleccion
    int cantidadAlbumes() const;//Cantidad de albumes del usuario
    Album *obtenerAlbum(int posicion) const;//Acceder a un album específico de la lista del usuario
};

class GestorArchivos;

//Coordinar el funcionamiento general de la red social
class RedSocial {
    friend class GestorArchivos;//Los métodos de GestorArchivos pueden acceder a los miembros privados de RedSocial
private:
    ListaDinamica<Usuario> usuarios;
    ListaDinamica<Mensaje> mensajes;
    ListaDinamica<Solicitud> solicitudes;
    MatrizAdyacencia matriz;
    int siguienteMensaje;//Identificador para el siguiente mensaje que se cree

    Usuario *buscarIdUsuario(int id) const; //Busca un usuario a partir de su identificador
    void mostrarPerfilAmigo(Usuario *actual, Usuario *amigo);
    bool verificarSolicitud(int emisor, int receptor) const;//Si existe una solicitud entre dos usuarios
    void ordenarMensajesFecha(Mensaje **lista, int cantidad) const;

public:
    RedSocial();
    ~RedSocial();

    //Metodos de GestorArchivos
    bool cargarDatos(const char *carpeta);
    bool guardarDatos(const char *carpeta) const;

    //Localizar cuentas registradas y comprobar credenciales
    Usuario *buscarUsuario(const char *nombre) const;
    Usuario *autenticar(const char *nombre, const char *clave) const;

    //Interación en la red social
    void mostrarMejoresAmigos(Usuario *actual);
    void administrarAmistades(Usuario *actual);
    void visitarPerfil(Usuario *actual);
    void sugerenciasAmistad(Usuario *actual, int t);
    void administrarMensajes(Usuario *actual);
    bool enviarMensaje(Usuario *actual, const char *destino, const char *texto);

    bool enviarSolicitud(Usuario *actual, const char *destino);
    bool aceptarSolicitud(Usuario *actual, int posicionListaU);
    bool rechazarSolicitud(Usuario *actual, int posicionListaU);

    int cantidadUsuarios() const;//Cantidad de usuarios registrados (Lista Usuarios)
};

#endif // FUNCIONALIDADES_H
