
// Nombre y Apellidos: Alejandro del Rio

// Comentario general sobre la solucion: En este problema necesitamos ir rellenandno la matriz por diagonales.
// explicando como se resuelve el problema

#include <iostream>
#include <fstream>

#include <vector>
#include "Matriz.h"
using namespace std;

int aibo(Matriz<int> &m, string const &s, int i, int j)
{   
    if (i > j)
            return 0;
    if (i == j)
        m[i][i] = 1;
    if(m[i][j] == -1){
        if (s[i] == s[j])
            m[i][j] = aibo(m, s, i + 1, j - 1) + 2;
        else{
            m[i][j] = max(aibo(m, s, i + 1, j), aibo(m, s, i, j - 1));
        }
    }
    

    return m[i][j];
}

string aibo_recons(Matriz<int> const& m, string const& s, int i , int j ){
    if(i > j) return {};
    if (i == j) return {s[i]};
    else if(s[i] == s[j]){
        return s[i] + aibo_recons(m, s, i + 1, j - 1) + s[j];
    }
    else if(m[i + 1][j] >= m[i][j - 1]){
        return s[i] + aibo_recons(m, s, i + 1, j) + s[i];
    }
    else {
        return s[j] + aibo_recons(m, s, i , j - 1) + s[j];
    }
}

bool resuelveCaso()
{
    string s; cin >> s;
    if(!cin) return false;
    int n = s.length();
    Matriz<int> m(n, n, -1);

    // llamada a la funcion:
    int nInserciones = n - aibo(m, s, 0, n - 1);
    string reconstruccion = aibo_recons(m, s, 0, n - 1);
    cout << nInserciones << " " << reconstruccion << endl;
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
