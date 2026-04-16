
// Nombre: Alejandro del Rio Caballero

#include <vector>
#include <queue>
#include <iostream>
#include <fstream>
using namespace std;

/*
    N funcionarios y N trabajos
    T[i, j] representa el tiempo que el funcionario i tarda en realizar el trabajo j.
    Objetivo: asignacion optima de trabajos a funcionarios.
    donde la suma total de tiempos sea minima.
*/


/*

    1. Espacio de soluciones

        Representa las posibles asignaciones de trabajos a cada trabajador.
        Cada solución es un vector sol[0..n-1] donde sol[i] = j indica que el trabajador i se le ha asignado el trabajo j. El objetivo es minimizar el número total de horas de trabajo.

    2. Estructura del Árbol
        El árbol no es binario sino que tiene N ramas por nodo, donde N es el número de trabajos disponibles:

        La raíz representa el estado inicial sin ninguna asignación
        Cada nivel k corresponde al trabajador k y su decisión de qué trabajo realizar
        Cada nodo tiene tantos hijos como trabajos aún no asignados queden

    3. Restricciones
    Explícitas:

        !asignados[j] -> cada trabajo solo puede asignarse a un único trabajador
        Todos los trabajadores deben tener exactamente un trabajo asignado

    Implícitas:

        sol[i] = j con i != j para todo par -> no puede haber dos trabajadores con el mismo trabajo
        El orden de exploración es por trabajador (nivel = trabajador), no por trabajo


    4. Tamaño del Árbol
        En cada nivel k quedan N - k trabajos disponibles, por lo que el tamaño del arbol es de N^K

    5. Test de Factibilidad
    Cota optimista -> para cada trabajador pendiente asigna el trabajo de menor tiempo, ignorando conflictos. Nunca sobreestima el mínimo real, por lo que se usa para podar:
        if(X.tiempo_estimado <= mejor_tiempo) // si no puede mejorar, se poda
    Cota pesimista -> para cada trabajador asigna el trabajo de mayor tiempo. Devuelve una solución real y alcanzable (en el peor caso), usada para inicializar y actualizar
*/

struct pack{
    vector<double> pes;
    vector<double> opt;
};

struct Nodo {
    vector<int> sol;
    int k;

    vector<bool> asignados; // trabajos asignados.
    double tiempo_acumulado;

    double tiempo_estimado;
    bool operator<(Nodo const& o) const {
        return o.tiempo_estimado < tiempo_estimado;
    }
};

pack opt_pes(vector<vector<double>> const& T, Nodo const& X){
    pack p;
    size_t N = T.size();
    vector<double> fast(N);
    vector<double> slow(N);
    // Calculamos el minimo y maximo de cada trabajador.
    for(int i = 0; i < N; ++i){
        fast[i] = T[i][0];
        slow[i] = T[i][0];
        for(int j = 1; j < N; ++j){
            if(fast[i] > T[i][j]){
                fast[i] = T[i][j];
               
            }
            if(slow[i] < T[i][j]){
                slow[i] = T[i][j];
            }
        }
    }
    // registramos en un vector de acumulados: 
    p.opt = vector<double>(N + 1, 0);
    p.pes = vector<double>(N + 1, 0);

    for(int i = N - 1; i >= 0; --i){
        p.opt[i] = p.opt[i + 1] + fast[i];
        p.pes[i] = p.pes[i + 1] + slow[i];
    }

    return p;

}

void funcionarios_rp(vector<vector<double>> const& T, vector<int> &mejor_sol, double &mejor_tiempo){


    size_t N = T.size();
    Nodo Y;
    Y.sol = vector<int>(N, -1);
    Y.k = -1; // raiz.
    Y.tiempo_acumulado = 0;
    Y.asignados = vector<bool>(N, false);
    pack p = opt_pes(T, Y);
    Y.tiempo_estimado = p.opt[0];

    mejor_tiempo = p.pes[0];
    priority_queue<Nodo> q;
    q.push(Y);
    while(!q.empty() && q.top().tiempo_estimado <= mejor_tiempo){
        Y = q.top(); q.pop();

        Nodo X(Y);
        X.k++; // representa al trabajador.
        for(int i = 0 ; i < N; ++i){
            if(!X.asignados[i]){
                X.sol[X.k]= i;
                X.asignados[i] = true;
                X.tiempo_acumulado = Y.tiempo_acumulado + T[X.k][i];
                X.tiempo_estimado = X.tiempo_acumulado + p.opt[X.k + 1];

                if(X.tiempo_estimado <= mejor_tiempo){
                    if(X.k == N - 1){
                        mejor_tiempo = X.tiempo_acumulado;
                        mejor_sol = X.sol;
                    }
                    else{
                        q.push(X);
                        mejor_tiempo = min(mejor_tiempo, X.tiempo_acumulado + p.pes[X.k + 1]);
                    }
                }
                
                //desmarcamos
                X.asignados[i] = false;
            }
        }
    }
}

bool resuelveCaso(){
    int n; cin >> n;
    if(n == 0) return false;

    vector<vector<double>> m;

    vector<double> aux(n);
    //recorremos la matriz de trabajo
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; ++j){
            cin >> aux[j];
        }
        m.push_back(aux);
    }

    double mejor_tiempo;
    vector<int> mejor_sol;
    funcionarios_rp(m, mejor_sol, mejor_tiempo);
    cout << mejor_tiempo << endl;

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