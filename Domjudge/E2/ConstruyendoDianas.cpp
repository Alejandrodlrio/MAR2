
// Nombre y Apellidos: Alejandro del Rio y Ana Sanchez Pou

// Comentario general sobre la solucin,
// explicando c�mo se resuelve el problema

#include <iostream>
#include <fstream>

#include <vector>
#include "EnterosInf.h"
#include "Matriz.h"
using namespace std;

void dianas(Matriz<EntInf>& m, vector<int> const& sect, int v, int s) {
    // Caso base:  j = 0 , i = 0
    m[0][0] = 0;
    for (int i = 1; i <= s; ++i) {
        // Caso base:  j = 0 , 1 <= i < s
        m[i][0] = 0;
        for (int j = 1; j <= v; ++j) {
            // nos quedamos con el sector anterior: 
            
            if (j < sect[i - 1]) {
                m[i][j] = m[i - 1][j];
            }
            else
                m[i][j] = min(m[i - 1][j], m[i][j - sect[i - 1]] + 1);
        }
    }

}

vector<int> reconstruccion(Matriz<EntInf> const& m, vector<int> const& sect, int v, int s) {
    int j = v;
    int i = s;
    vector<int> rec;
    while (j != 0) {
        if (m[s][v] != Infinito) {
            if (m[i][j] != m[i - 1][j])
            {
                rec.push_back(sect[i - 1]);
                j -= sect[i - 1];
            }
            else {
                --i;
            }
        }
    }
    return rec;
}


bool resuelveCaso() {
    int v, s; cin >> v >> s;
    if (!cin) return false;
    vector<int> sectores(s);
    for (auto& i : sectores) {
        cin >> i;
    }

    

    Matriz<EntInf> m(s + 1, v + 1, Infinito);

    // llamamos a la funcion de construccion de dianas: 
    dianas(m, sectores, v, s);
    
    if (m[s][v] != Infinito)
    {
        vector<int> r = reconstruccion(m, sectores, v, s);
        cout << m[s][v] << ": ";
        for (auto i : r) {
            cout << i << " ";
        }
    }
    else
        cout << "Imposible";
    cout << "\n";
    return true;
}

int main() {
    // ajustes para que cin extraiga directamente de un fichero
#ifndef DOMJUDGE
    std::ifstream in("casos.txt");
    auto cinbuf = std::cin.rdbuf(in.rdbuf());
#endif

    while (
        resuelveCaso()
        );
    // para dejar todo como estaba al principio
#ifndef DOMJUDGE
    std::cin.rdbuf(cinbuf);
    system("PAUSE");
#endif
    return 0;
}
