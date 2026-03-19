
// Nombre y Apellidos: Alejandro del Rio

// Comentario general sobre la solucion: En este problema vamos atendiendo la diagonal porque es donde vamos sumando 1.
// explicando como se resuelve el problema

#include <iostream>
#include <fstream>

#include <vector>
#include "Matriz.h"
using namespace std;

string reconstruccion(Matriz<int> const& m, string const& s1, string const& s2, int n1, int n2){

    int i = n1;
    int j = n2;
    string s = "";
    while(i > 0 && j > 0){
        if(s1[i-1] == s2[j-1]){
            s = s1[i - 1] + s;
            i--; j--;
        }
        else if(m[i-1][j] >= m[i][j-1]){
            i--;
        }
        else{
            j--;
        }
    }
    

    return s;
}
// Programacion dinamica ascendente:
string subsecuencia(Matriz<int> & m, string const& s1, string const& s2, int n1, int n2){

    
    for(int i = 1; i <= n1; ++i){
        for(int j = 1; j <= n2; ++j){
            if(s1[i - 1] == s2[j - 1]){
                m[i][j] = m[i - 1][j - 1] + 1;
            }
            else {
                m[i][j] = max( m[i - 1][j], m[i][j - 1]);
            }
        }
    }

    // return m[n1][n2];
    
    
    return reconstruccion(m, s1, s2, n1, n2);
    
}



bool resuelveCaso()
{
    string s1, s2; cin >> s1 >> s2;
    if(!cin) return false;
    int n1 = s1.size();
    int n2 = s2.size();

    Matriz<int> m(n1 + 1, n2 + 1, 0); 

    // int comunes = subsecuencia(m, s1, s2, n1, n2);
    string r = subsecuencia(m, s1, s2, n1, n2);
    cout << r << '\n';
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
