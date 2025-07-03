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
