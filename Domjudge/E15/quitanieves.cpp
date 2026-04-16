
// Nombre: Alejandro del Rio Caballero

#include <vector>
#include <queue>
#include <iostream>
#include <fstream>
using namespace std;

/*
    N carros para M caminos tal que M > N
    Cada carro tiene una anchura ANCH tal que solo puede limpiar caminos con ANCH_carro < ANCH_camino
    Objetivo: maximizar la suma de calidades de los caminos limpiados. Se ha de tener en cuenta que cada carro va a limpiar a lo sumo un camino. Y que cada camino se va a limpiar a lo sumo con un carro. (uno a uno)
*/


/*

    1. Espacio de soluciones
        Cada solución se representa como un vector sol de tamaño N donde:
            sol[i] = j  -> el carro i limpia el camino j
            sol[i] = -1 -> el carro i no se asigna a ningún camino
        Condiciones:
        - Cada camino puede usarse como máximo una vez
        - No todos los carros tienen por qué asignarse (aunque normalmente interesa)

    2. Estructura del Árbol


    3. Restricciones
    Explícitas:
    - Un carro i solo puede asignarse a un camino j si:
        carros[i].anchura <= caminos[j].anchura
    - Cada camino se usa como máximo una vez

    Implícitas:
    - Solo se exploran nodos cuya cota optimista ≥ mejor solución actual
    - Se descartan ramas que no pueden mejorar la solución (poda)

    4. Tamaño del Árbol
        M ramas, N niveles por tanto M^N
    

    5. Test de Factibilidad

        Un nodo es factible si:
        - No se repiten caminos (controlado con vector<bool> asignados)
        - Se respeta la restricción de anchura:
            carros[i].anchura <= caminos[j].anchura

        Además:
        - Una solución es completa cuando k == N - 1
        - En ese caso se actualiza la mejor solución si mejora la calidad
*/


struct Atributo{
    int anchura;
};

struct Nodo {
    int k;
    vector<int> sol;
    vector<bool> asignados; // vector de caminos asignados.
    int calidad_acumulada;
    int calidad_estimada;

    bool operator<(Nodo const& o) const {
        return o.calidad_estimada > calidad_estimada;
    }
};
struct pack{
    vector<int> opt;
    vector<int> pes;
};
// Cota optimista: para cada carro restante asignamos el mejor camino disponible ignorando conflictos
pack opt_pes(Nodo const& X, vector<vector<int>> const& C, vector<Atributo> const& carros, vector<Atributo> const& caminos, int N, int M){
    pack p;
    vector<int> best(N, -1);
    vector<int> worst(N, -1);
    for (int i = 0; i < N; ++i) {
        int mejor = 0;
        int peor = 0;
        for(int j = 0; j < M; ++j){
            if(carros[i].anchura <= caminos[j].anchura){
                if(mejor < C[i][j]) {
                   mejor = C[i][j];
                }
                if(peor > C[i][j]) {
                    worst[i] = C[i][j];
                }
            }
        }
        best[i] = mejor;
        worst[i] = peor;
    }


    // formamos un vector de acumulados optimista y pesimista: 
    p.opt = vector<int>(N + 1, 0);
    p.pes = vector<int>(N + 1, 0);

    for(int i = N - 1; i >= 0; i--){
        p.opt[i] = p.opt[i + 1] + best[i];
        p.pes[i] = p.pes[i + 1] + worst[i];
    }

    return p;

}
void invernalia(vector<vector<int>> const& C,  vector<Atributo> const& carros, vector<Atributo> const& caminos, vector<int> &mejor_sol, int &mejor_calidad){

    size_t N = C.size();
    size_t M = caminos.size();
    Nodo Y;
    Y.k = -1;
    Y.sol = vector<int>(N, -1);
    Y.asignados = vector<bool>(M, false);
    Y.calidad_acumulada = 0;
    pack p = opt_pes(Y,C, carros, caminos, N, M);
    Y.calidad_estimada = p.opt[0];
    mejor_calidad = p.pes[0];
    priority_queue<Nodo> q;
    q.push(Y);

    while(!q.empty() && q.top().calidad_estimada >= mejor_calidad){
        Y = q.top(); q.pop();
        Nodo X(Y);
        X.k++; // Carro quita-nieve actual.
        //RAMA IZQ: Asignamos el carro X.k a limpiar un camino valido
        for(int i = 0; i < M; ++i){ // observamos los caminos.
            if(!X.asignados[i] && carros[X.k].anchura <= caminos[i].anchura){

                X.asignados[i] = true;
                X.sol[X.k] = i;
                X.calidad_acumulada = Y.calidad_acumulada + C[X.k][i];
                X.calidad_estimada = X.calidad_acumulada + p.opt[X.k + 1];
                if(X.calidad_estimada >= mejor_calidad){
                    if(X.k == N - 1){
                        mejor_calidad = X.calidad_acumulada;
                        mejor_sol = X.sol;
                    }
                    else{
                        q.push(X);
                        mejor_calidad = max(mejor_calidad,  X.calidad_acumulada + p.pes[X.k + 1]);
                    }
                }
                X.asignados[i] = false;
            }
        }
        //RAMA DCHA: el carro X.k. no puede limpiar el camino.
        X.sol[X.k] = -1;
        X.asignados = Y.asignados;
        X.calidad_acumulada = Y.calidad_acumulada;
        X.calidad_estimada = X.calidad_acumulada + p.opt[X.k + 1];
        if(X.calidad_estimada >= mejor_calidad){
            if(X.k == N - 1){
                mejor_calidad = X.calidad_acumulada;
                mejor_sol = X.sol;
            }
            else{
                q.push(X);
                mejor_calidad = max(mejor_calidad, X.calidad_acumulada + p.pes[X.k + 1]);
            }
        }
        
    }
}

void resuelveCaso(){
    int n, m; cin >> n >> m;

    vector<Atributo> carros(n);
    vector<Atributo> caminos(m);
    for(auto &i:  carros) cin >> i.anchura;
    for(auto &i: caminos) cin >> i.anchura;


    // recorremos la matriz de capacidades de nieve.
    vector<vector<int>> nieve;
    vector<int> aux(m);

    for(int i = 0; i < n; ++i){
        for(int j = 0; j < m ; ++j){
            cin >> aux[j];
        }   
        nieve.push_back(aux);
    }

    int mejor_calidad;
    vector<int> mejor_sol;
    invernalia(nieve, carros, caminos, mejor_sol, mejor_calidad);
    cout << mejor_calidad << endl;


}

int main()
{
    // ajustes para que cin extraiga directamente de un fichero
#ifndef DOMJUDGE
    std::ifstream in("casos.txt");
    auto cinbuf = std::cin.rdbuf(in.rdbuf());
#endif
    int n; cin >> n;
    for(int i = 0; i < n; ++i) resuelveCaso();
    // para dejar todo como estaba al principio
#ifndef DOMJUDGE
    std::cin.rdbuf(cinbuf);
    system("PAUSE");
#endif
    return 0;
}