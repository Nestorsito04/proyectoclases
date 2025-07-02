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
