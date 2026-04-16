#include <iostream>
#include <vector>
#include <cmath>
#include <queue>
using namespace std;

/*

1. Espacio de Soluciones
     las posibles asignaciones de objetos a envases. Cada solución es un vector sol[0..n-1] donde sol[i] = j indica que el objeto i se almacena en el envase j. El objetivo es minimizar el número de envases necesarios para almacenar todos los objetos sin superar la capacidad E de cada envase.

2. Estructura del Árbol
    El árbol no es binario sino que tiene N ramas por nodo, donde N es el número de envases disponibles en ese momento:

    La raíz representa el primer objeto asignado al primer envase
    Cada nivel k corresponde a la decisión de en qué envase meter el objeto k
    Cada nodo tiene tantos hijos como envases posibles:

    Ramas [1..envases]: meter el objeto en un envase ya existente (si cabe)
    Rama nueva: abrir un nuevo envase

3. Restricciones: 
    Explícitas:

    capacidad[j] - objetos[k].volumen >= 0 → el objeto debe caber en el envase
    Cada envase tiene capacidad máxima E

    Implícitas:

    Los objetos deben estar ordenados por volumen decreciente para que la estimación voraz sea válida
    Solo se abre un nuevo envase si el objeto no cabe en ninguno de los existentes


4.  Tamaño del Árbol
     Al tener N ramas y K niveles N^K


5. Test de Factibilidad
    Se usan dos cotas:
        Cota optimista -> ceil(volumen_total / E): número mínimo de envases asumiendo que se pueden empaquetar perfectamente. Nunca sobreestima el número real de envases necesarios.
        Cota pesimista -> asigna los objetos restantes a los envases existentes de forma voraz (primer envase donde quepan). Devuelve una solución real y alcanzable, por lo que se usa para actualizar mejor_envases en cada paso.
    La poda se aplica con:
        q.top().envases <= mejor_envases
    Si la cota optimista de un nodo ya iguala o supera la mejor solución real encontrada, se descarta esa rama.

*/


struct Objeto{
    int volumen;
};

struct Nodo {
    int k;
    vector<int> sol;

    int envases; // numero de envases estimados.
    vector<int> capacidad; // N envases con capacidad E.

    bool operator<(Nodo const& o) const{
        return o.envases < envases; // min heap: minimizamos el numero de envases
    }

};

int pesimista(Nodo const& X, vector<Objeto> const& objetos, int E){
    int envasesTotales = X.envases; // minimo numero de envases.
    vector<int> capacidad_aux = X.capacidad; // lo que restan de los envases
    for(int i = X.k + 1; i < objetos.size(); ++i){ 
        int j = 1;
        while(objetos[i].volumen > capacidad_aux[j]){++j;}
        //hemos encontrado uno que cabe sin rebosar.
        capacidad_aux[j] -= objetos[i].volumen;
        envasesTotales = max(envasesTotales, j);
    }
    return envasesTotales;
}

// solucion optima: necesitaremos tantos envases seguro..
int optimista(Nodo const& X, vector<Objeto> const& objetos, int E){
    int total = 0;
    for(int i = 0; i < objetos.size(); ++i){
        total += objetos[i].volumen;
    }
    return ceil(total / E);
}
void envases_rp(vector<Objeto> const& objetos, int E, vector<int> mejor_sol, int mejor_envases){

    size_t N = objetos.size();
    Nodo Y;
    Y.sol = vector<int>(N, -1);
    //El primer objeto se almacena en el primer envase: 
    Y.k = 0;
    Y.sol[Y.k] = 1;
    
    Y.envases = optimista(Y, objetos, E);

    Y.capacidad = vector<int>(N, E);
    Y.capacidad[Y.k] -= objetos[Y.k].volumen; 

    mejor_envases = pesimista(Y, objetos, E);
    priority_queue<Nodo> q;
    q.push(Y);
    bool sol_encontrada = false;
    while(!q.empty() && q.top().envases <= mejor_envases){
        Y = q.top(); q.pop();
        Nodo X(Y);

        X.k++; // siguiente objeto.
        
        // recorremos los envases hasta llenarlos completamente.
        for(int i = 1; i < X.envases && !sol_encontrada; ++i){
            // Vemos si caben los objetos en los envases: 

            if(Y.capacidad[i] - objetos[X.k].volumen >= 0){
                // Reutilizamos el envase anterior.
                Y.capacidad[i] -= objetos[X.k].volumen;
                Y.sol[X.k] = i;

                //hemos llegado al final
                if(X.k == N - 1){
                    mejor_sol = X.sol;
                    sol_encontrada = (X.envases == mejor_envases);
                }
                else{
                    q.push(X);
                    mejor_envases = min(mejor_envases, pesimista(X, objetos, E));
                }

                // desmarcamos la capacidad y la restablecemos a la inicial.
                X.capacidad[i] = Y.capacidad[i]; 
            }
        }
        if(!sol_encontrada){
            // probamos a meter en un nuevo envase.
            int nuevo = Y.envases + 1;
            X.sol[X.k] = nuevo;

            X.capacidad[nuevo] -= objetos[X.k].volumen;
            if(X.envases <= mejor_envases){
                if(X.k == N - 1){
                    mejor_sol = X.sol;
                    sol_encontrada = (X.envases == mejor_envases);
                }
                else{
                    q.push(X);
                    mejor_envases = min(mejor_envases, pesimista(X, objetos, E));
                }
            }
        }
    }
}


void main(){}