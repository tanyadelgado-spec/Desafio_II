#include <funcionalidades.h>

int main() {
    srand((unsigned int)time(0));
    Metricas::externa();

    RedSocial red;
    const char carpetaDatos[] = "datos_completos";

    if (!red.cargarDatos(carpetaDatos)) {
        cout << "No se pudieron cargar los datos permanentes.\n";
        cout << "Verifique que exista la carpeta con los archivos del proyecto.\n";
        return 1;
    }

    cout << "\n-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-\n";
    cout << "-_-_-_-_-_-_-_-_-_-_-Ude@Book-_-_-_-_-_-_-_-_-_-_-\n";
    cout << "-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-\n";

    char usuario[128];
    char clave[128];
    Usuario *actual = 0;
    int intentos = 0;

    do {
        cout << "\nUsuario: ";
        cin.getline(usuario, 128);
        cout << "Clave: ";
        cin.getline(clave, 128);

        actual = red.autenticar(usuario, clave);
        intentos++;
        if (actual == 0) cout << "Datos incorrectos (intentos: " << intentos << ").\n";

    } while (actual == 0 && intentos < 3);

    if (actual == 0) {
        cout << "Se alcanzo el limite de intentos.\n";
        return 0;
    }

    cout << "\n// Bienvenido a Ude@Book // " << actual->getNombre() << "\n";

    red.mostrarMejoresAmigos(actual);

    char opcion;
    do {
        cout << "\n-_-_-_-_-_-_-Menu-_-_-_-_-_-_-\n";
        cout << "1. Administrar amistades\n";
        cout << "2. Visitar perfil de un amigo\n";
        cout << "3. Ver sugerencias de amistad\n";
        cout << "4. Administrar mensajes\n";
        cout << "[S]. Cerrar sesion\n";
        cout << "Ingrese una opcion: ";
        cin >> opcion;
        cin.ignore();

        switch (opcion) {
        case '1':
            red.administrarAmistades(actual);
            red.guardarDatos(carpetaDatos);
            break;

        case '2':
            red.visitarPerfil(actual);
            red.guardarDatos(carpetaDatos);
            break;

        case '3':
            red.sugerenciasAmistad(actual, 10);
            red.guardarDatos(carpetaDatos);
            break;

        case '4':
            red.administrarMensajes(actual);
            red.guardarDatos(carpetaDatos);
            break;

        case 'S':
        case 's':
            break;

        default:
            cout << "Opcion invalida. Intente nuevamente.\n";
            break;
        }
    } while (opcion != 'S' && opcion != 's');

    if (red.guardarDatos(carpetaDatos)) {
        cout << "\nDatos actualizados en el almacenamiento permanente.\n";
    } else {
        cout << "\nAlerta: no fue posible guardar los datos.\n";
    }

    cout << "Hasta pronto, " << actual->getNombre() << ".\n";
    return 0;
}