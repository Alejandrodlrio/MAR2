
// Nombre y Apellidos: Alejandro del Rio

// Comentario general sobre la solucion: En este problema necesitamos ir rellenandno la matriz por diagonales.
// explicando como se resuelve el problema

#include <iostream>
#include <fstream>

#include <vector>
#include "Matriz.h"
using namespace std;



bool resuelveCaso()
{
    string s1, s2; cin >> s1 >> s2;
    int n1 = s1.length();
    int n2 = s2.length();

    Matriz<int> m(n1 + 1, n2 + 1, 0);

    int comunes = subsecuencia(m, s1, s2, n1, n2);
    return true;
}

int main()
{
    // ajustes para que cin extraiga directamente de un fichero
#ifndef DOMJUDGE
    std::ifstream in("casos.txt");
    auto cinbuf = std::cin.rdbuf(in.rdbuf());
#endif

    while (
        resuelveCaso())
        ;
    // para dejar todo como estaba al principio
#ifndef DOMJUDGE
    std::cin.rdbuf(cinbuf);
    system("PAUSE");
#endif
    return 0;
}
