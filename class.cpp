#include <iostream>
#include <iostream>
#include <algorithm>

using namespace std;

class Publicacion {
protected:
    string titulo;
    int ano;
public:
    Publicacion(string t, int a) : titulo(move(t)), ano(a) {}
    virtual void mostrar() const = 0;
    virtual string tipo() const = 0;
    virtual ~Publicacion() {}
    string getTitulo() const { return titulo; }
    int getAno() const { return ano; }
};

class Libro : public Publicacion {
    int paginas;
public:
    Libro(string t, int a, int p) : Publicacion(move(t), a), paginas(p) {}
    void mostrar() const override {
        cout << "[Libro] \"" << titulo << "\" - Ano: " << ano << ", Paginas: " << paginas << '\n';
    }
    string tipo() const override { return "Libro"; }
};

class Revista : public Publicacion {
    int edicion;
public:
    Revista(string t, int a, int e) : Publicacion(move(t), a), edicion(e) {}
    void mostrar() const override {
        cout << "[Revista] \"" << titulo << "\" - Ano: " << ano << ", Edicion: " << edicion << '\n';
    }
    string tipo() const override { return "Revista"; }
};

class Periodico : public Publicacion {
    string fecha;
public:
    Periodico(string t, int a, string f) : Publicacion(move(t), a), fecha(move(f)) {}
    void mostrar() const override {
        cout << "[Periodico] \"" << titulo << "\" - Ano: " << ano << ", Fecha: " << fecha << '\n';
    }
    string tipo() const override { return "Periodico"; }
};

bool validarAno(int ano) {
    return ano >= 1500 && ano <= 2025;
}

void limpiarBuffer() {
    while (cin.get() != '\n');
}

string leerTexto(const string& texto) {
    string input;
    do {
        cout << texto;
        getline(cin, input);
    } while (input.empty());
    return input;
}

int leerEntero(const string& texto, int min = 1) {
    int valor;
    do {
        cout << texto;
        cin >> valor;
        limpiarBuffer();
    } while (valor < min);
    return valor;
}

void agregarPublicacion(Publicacion**& catalogo, int& tamano) {
    int opcion;
    cout << "\nTipo de publicacion:\n1. Libro\n2. Revista\n3. Periodico\n> ";
    cin >> opcion;
    limpiarBuffer();

    string titulo = leerTexto("Titulo: ");
    int ano = leerEntero("Ano de publicacion: ", 1500);
    if (!validarAno(ano)) {
        cout << "Ano fuera de rango.\n";
        return;
    }

    Publicacion* nueva = nullptr;
    if (opcion == 1) {
        int paginas = leerEntero("Numero de paginas: ");
        nueva = new Libro(titulo, ano, paginas);
    } else if (opcion == 2) {
        int edicion = leerEntero("Numero de edicion: ");
        nueva = new Revista(titulo, ano, edicion);
    } else if (opcion == 3) {
        string fecha = leerTexto("Fecha de publicacion: ");
        nueva = new Periodico(titulo, ano, fecha);
    } else {
        cout << "Opcion invalida.\n";
        return;
    }

    Publicacion** nuevoCatalogo = new Publicacion*[tamano + 1];
    for (int i = 0; i < tamano; ++i)
        nuevoCatalogo[i] = catalogo[i];
    nuevoCatalogo[tamano] = nueva;

    delete[] catalogo;
    catalogo = nuevoCatalogo;
    ++tamano;
}

void mostrarCatalogo(Publicacion** catalogo, int tamano) {
    if (tamano == 0) {
        cout << "No hay publicaciones registradas.\n";
        return;
    }
    for (int i = 0; i < tamano; ++i) {
        cout << i + 1 << ". ";
        catalogo[i]->mostrar();
    }
}

void buscarTitulo(Publicacion** catalogo, int tamano) {
    string clave = leerTexto("Buscar titulo: ");
    transform(clave.begin(), clave.end(), clave.begin(), ::tolower);
    bool encontrado = false;
    for (int i = 0; i < tamano; ++i) {
        string t = catalogo[i]->getTitulo();
        string t_lower = t;
        transform(t_lower.begin(), t_lower.end(), t_lower.begin(), ::tolower);
        if (t_lower.find(clave) != string::npos) {
            catalogo[i]->mostrar();
            encontrado = true;
        }
    }
    if (!encontrado) cout << "No se encontraron coincidencias.\n";
}

void eliminarPublicacion(Publicacion**& catalogo, int& tamano) {
    mostrarCatalogo(catalogo, tamano);
    if (tamano == 0) return;
    int indice = leerEntero("Seleccione numero a eliminar: ") - 1;
    if (indice < 0 || indice >= tamano) {
        cout << "Indice invalido.\n";
        return;
    }

    delete catalogo[indice];
    for (int i = indice; i < tamano - 1; ++i)
        catalogo[i] = catalogo[i + 1];

    Publicacion** nuevoCatalogo = new Publicacion*[tamano - 1];
    for (int i = 0; i < tamano - 1; ++i)
        nuevoCatalogo[i] = catalogo[i];

    delete[] catalogo;
    catalogo = nuevoCatalogo;
    --tamano;

    cout << "Publicacion eliminada.\n";
}

void mostrarEstadisticas(Publicacion** catalogo, int tamano) {
    int libros = 0, revistas = 0, periodicos = 0;
    int minAno = 9999, maxAno = 0;

    for (int i = 0; i < tamano; ++i) {
        string tipo = catalogo[i]->tipo();
        if (tipo == "Libro") ++libros;
        else if (tipo == "Revista") ++revistas;
        else if (tipo == "Periodico") ++periodicos;

        int a = catalogo[i]->getAno();
        if (a < minAno) minAno = a;
        if (a > maxAno) maxAno = a;
    }

    cout << "\nEstadisticas:\n";
    cout << "Total: " << tamano << "\n";
    cout << "Libros: " << libros << ", Revistas: " << revistas << ", Periodicos: " << periodicos << '\n';
    if (tamano > 0)
        cout << "Ano mas antiguo: " << minAno << ", mas reciente: " << maxAno << '\n';
}

void liberarMemoria(Publicacion**& catalogo, int& tamano) {
    for (int i = 0; i < tamano; ++i)
        delete catalogo[i];
    delete[] catalogo;
    catalogo = nullptr;
    tamano = 0;
}

int main() {
    Publicacion** catalogo = nullptr;
    int tamano = 0;
    int opcion;

    do {
        cout << "MENU PRINCIPAL";
        cout << "\n1. Agregar publicacion\n2. Mostrar publicaciones\n3. Buscar por titulo\n4. Eliminar publicacion\n5. Estadisticas\n6. Salir\n> ";
        cin >> opcion;
        limpiarBuffer();

        switch (opcion) {
            case 1: agregarPublicacion(catalogo, tamano); break;
            case 2: mostrarCatalogo(catalogo, tamano); break;
            case 3: buscarTitulo(catalogo, tamano); break;
            case 4: eliminarPublicacion(catalogo, tamano); break;
            case 5: mostrarEstadisticas(catalogo, tamano); break;
            case 6: liberarMemoria(catalogo, tamano); cout << "Saliendo...\n"; break;
            default: cout << "Opcion invalida.\n"; break;
        }
    } while (opcion != 6);

    return 0;
}
