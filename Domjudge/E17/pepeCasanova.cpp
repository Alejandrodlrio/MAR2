// Nombre: Alejandro del Rio Caballero

#include <vector>
#include <queue>
#include <iostream>
#include <fstream>
#include <algorithm>
using namespace std;


/*
    Quirere formar su propia cinta con vinilos. 
    Cuanto mayor es la puntuacion del vinilo, mas le gusta.
    Objetivo: maximizar la suma de puntuaciones 
    1. No puede repetir canciones. 
    2. las canciones escogidas deben caber enteras
    no es admisible que se corte a la mitad al final de una cara de la cinta. (debe ajustarse al tamaño exacto)
*/


/*


1. Espacio de Soluciones
    Representa todas las posibles asignaciones de canciones a las dos caras de la cinta. Cada solución es un vector sol[0..n-1] donde sol[i] = true indica que la canción i se graba en alguna de las dos caras.
    El objetivo es maximizar la suma de puntuaciones sin superar la capacidad maxD de cada cara.

2. Estructura del Árbol
El árbol tiene 3 ramas por nodo:

    Rama 1: meter la canción en la cara 1 (si cabe)
    Rama 2: meter la canción en la cara 2 (si cabe)
    Rama 3: no meter la canción (siempre factible)


3. Restricciones
    Explícitas:

    tiempo1 + canciones[k].duracion <= maxD -> la canción debe caber entera en la cara 1
    tiempo2 + canciones[k].duracion <= maxD -> la canción debe caber entera en la cara 2
    No se puede repetir una canción en ambas caras

    Implícitas:

    Las canciones están ordenadas por puntuación decreciente para que la poda sea más efectiva
    Las ramas 1 y 2 solo existen si la canción cabe; la rama 3 siempre existe


4. Tamaño del Árbol
    El árbol tiene 3^N nodos en el peor caso. 

5. Test de Factibilidad
    Cota optimista --> Mete las canciones restantes ordenadas por ratio puntuacion/duracion decreciente, eligiendo siempre la cara con menos tiempo acumulado. Si una canción no cabe entera, mete la fracción que quepa.
    La cota no sobreestima el valor asi que sirve para poder el arbol de exploracion.

    Cota pesimista -> mete las canciones restantes de forma voraz respetando las restricciones (sin fraccionar).
*/



struct Cancion{
    int duracion;
    int puntuacion;
};

struct Nodo{
    int k;
    vector<bool> sol;
    int puntuacion_acumulada;
    int tiempo1;
    int tiempo2;
    double puntuacion_estimada;

    bool operator<(Nodo const& o) const {
        return o.puntuacion_estimada > puntuacion_estimada;
    }
};

//Como la mochila pero viendo en cual de las dos caras cabe mejor, es decir, cota optimista como la mochila pero simulando dos mochilas.
int optimista(Nodo const& X, vector<Cancion> const& canciones, int maxD){
    double est = X.puntuacion_acumulada;

    int t1 = X.tiempo1;
    int t2 = X.tiempo2;

    for(int i = X.k + 1; i < (int)canciones.size(); i++){

        double ratio = canciones[i].puntuacion / canciones[i].duracion;

        // intentamos meter donde quepa más completo
        if(t1 <= t2){
            if(t1 + canciones[i].duracion <= maxD){
                t1 += canciones[i].duracion;
                est += canciones[i].puntuacion;
            } else {
                double hueco = maxD - t1;
                est += ratio * hueco;
                t1 = maxD;
            }
        }
        else {
            if(t2 + canciones[i].duracion <= maxD){
                t2 += canciones[i].duracion;
                est += canciones[i].puntuacion;
            } else {
                double hueco = maxD - t2;
                est += ratio * hueco;
                t2 = maxD;
            }
        }
    }

    return est;
}

// si no cabe en niguna de las cintas, se quita la cancion.
int pesimista(Nodo const& X, vector<Cancion> const& canciones, int maxD){
    int est = X.puntuacion_acumulada;
    int t1 = X.tiempo1;
    int t2 = X.tiempo2;
    for(int i = X.k + 1; i < (int)canciones.size(); i++){
        if(t1 + canciones[i].duracion <= maxD){
            t1 += canciones[i].duracion;
            est += canciones[i].puntuacion;
        }
        else if(t2 + canciones[i].duracion <= maxD){
            t2 += canciones[i].duracion;
            est += canciones[i].puntuacion;
        }
    }
    return est;
}

void playlist(vector<Cancion> const& canciones, int maxD, vector<bool> &sol_mejor, int &puntuacion_mejor){
    size_t N = canciones.size();
    Nodo Y;
    Y.sol = vector<bool>(N, false);
    Y.k = -1;
    Y.puntuacion_acumulada = 0;
    Y.tiempo1 = 0;
    Y.tiempo2 = 0;
    Y.puntuacion_estimada = optimista(Y, canciones, maxD);

    puntuacion_mejor = pesimista(Y, canciones, maxD);
    sol_mejor = Y.sol;

    priority_queue<Nodo> q;
    q.push(Y);

    while(!q.empty() && q.top().puntuacion_estimada >= puntuacion_mejor){
        Y = q.top(); q.pop();
        Nodo X(Y);
        X.k++;

        // Rama 1: cancion en cara 1
        if(Y.tiempo1 + canciones[X.k].duracion <= maxD){
            X.sol[X.k] = true;
            X.tiempo1 = Y.tiempo1 + canciones[X.k].duracion;
            X.tiempo2 = Y.tiempo2;
            X.puntuacion_acumulada = Y.puntuacion_acumulada + canciones[X.k].puntuacion;
            X.puntuacion_estimada = optimista(X, canciones, maxD);

            if(X.puntuacion_estimada > puntuacion_mejor){
                if(X.k == N - 1){
                    puntuacion_mejor = X.puntuacion_acumulada;
                    sol_mejor = X.sol;
                }
                else{
                    q.push(X);
                    puntuacion_mejor = max(puntuacion_mejor, pesimista(X, canciones, maxD));
                }
            }
        }

        // Rama 2: cancion en cara 2
        if(Y.tiempo2 + canciones[X.k].duracion <= maxD){
            X.sol[X.k] = true;
            X.tiempo1 = Y.tiempo1;                                         // restauramos tiempo1.
            X.tiempo2 = Y.tiempo2 + canciones[X.k].duracion;
            X.puntuacion_acumulada = Y.puntuacion_acumulada + canciones[X.k].puntuacion;
            X.puntuacion_estimada = optimista(X, canciones, maxD);

            if(X.puntuacion_estimada > puntuacion_mejor){
                if(X.k == N - 1){
                    puntuacion_mejor = X.puntuacion_acumulada;
                    sol_mejor = X.sol;
                }
                else{
                    q.push(X);
                    puntuacion_mejor = max(puntuacion_mejor, pesimista(X, canciones, maxD));
                }
            }
        }

        // Rama 3: no cogemos la cancion (siempre factible -> actualizamos pesimista)
        X.sol[X.k] = false;
        X.tiempo1 = Y.tiempo1;
        X.tiempo2 = Y.tiempo2;
        X.puntuacion_acumulada = Y.puntuacion_acumulada;
        X.puntuacion_estimada = optimista(X, canciones, maxD);

        puntuacion_mejor = max(puntuacion_mejor, pesimista(X, canciones, maxD));

        if(X.puntuacion_estimada >= puntuacion_mejor){
            if(X.k == N - 1){
                if(X.puntuacion_acumulada > puntuacion_mejor){
                    puntuacion_mejor = X.puntuacion_acumulada;
                    sol_mejor = X.sol;
                }
            }
            else{
                q.push(X);
            }
        }
    }
}

bool resuelveCaso(){
    int n; cin >> n;
    if(n == 0) return false;

    int d; cin >> d;
    vector<Cancion> canciones(n);
    for(auto &c : canciones) cin >> c.duracion >> c.puntuacion;

    // Ordenamos por puntuacion decreciente para mejorar la poda
    sort(canciones.begin(), canciones.end(), [](Cancion const& a, Cancion const& b){
        return (double(a.puntuacion) / a.duracion) > (double(b.puntuacion) / b.duracion);
    });

    int puntuacion_mejor;
    vector<bool> sol_mejor;
    playlist(canciones, d, sol_mejor, puntuacion_mejor);

    cout << puntuacion_mejor << endl;
    return true;
}





int main()
{
    // ajustes para que cin extraiga directamente de un fichero
#ifndef DOMJUDGE
    std::ifstream in("casos.txt");
    auto cinbuf = std::cin.rdbuf(in.rdbuf());
#endif
    while(resuelveCaso())
    ;
    // para dejar todo como estaba al principio
#ifndef DOMJUDGE
    std::cin.rdbuf(cinbuf);
    system("PAUSE");
#endif
    return 0;
}