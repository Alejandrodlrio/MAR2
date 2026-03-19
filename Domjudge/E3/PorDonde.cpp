
// Nombre y Apellidos: Alejandro del Rio

// Comentario general sobre la solucion: 
// explicando como se resuelve el problema

#include <iostream>
#include <fstream>

#include <vector>
#include "Matriz.h"
using namespace std;


struct pack {
    int res;
    int c;
};



pack caminoMaximo(Matriz<int> const& tablero, Matriz<int> &dp, int n){

    // Caso base: 
    dp = tablero;
    int mejor = 0;
    //empezamos en la n - 2
    for(int i = n - 2; i >= 0; --i){
        for(int j = 0; j < n; ++j){
            mejor = dp[i + 1][j];
            if(j < n-1 && dp[i+1][j+1] > mejor) mejor = dp[i+1][j+1];
            if(j > 0 && dp[i+1][j-1] > mejor) mejor = dp[i+1][j-1];
            dp[i][j] += mejor;
        }
    }

    //buscamos el maximo en la primera fila.
    int maxi = dp[0][0];
    int mejorColumna = 0;
    for(int j = 1; j < n; ++j){
        if(maxi < dp[0][j]){
            maxi = dp[0][j];
            mejorColumna = j;
        }
    }


    int row = mejorColumna;
    for(int i = 0; i < n - 1; ++i){
        int siguiente = row;
        if(row > 0 && dp[i+1][row-1] > dp[i+1][siguiente]){
            siguiente = row - 1;
        }
        if(row < n - 1 && dp[i+1][row+1] > dp[i+1][siguiente]){
            siguiente = row + 1;
        }
        row = siguiente;
    }



    return {maxi, row + 1};
}

bool resuelveCaso()
{
    int n; cin >> n;
    if(!cin) return false;
    Matriz<int> tablero(n, n);
    Matriz<int> dp(n , n, 0);
    for(int i = 0; i < n; ++i){
        for(int j = 0; j < n; ++j){
            cin >> tablero[i][j];
        }
    }

    // llamamos a la funcion: 
    pack p = caminoMaximo(tablero, dp, n);

    cout << p.res << " " << p.c << "\n";


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
