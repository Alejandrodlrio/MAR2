#include <vector>
#include <iostream>
using namespace std;

// cosas a explicar: 
/*
    1. Describir espacio de soluciones.


        Representa todas las posibles secuencias de decisiones sobre si meter o no cada objeto dentro de la mochila
        donde sol[i] = true indica que el objeto i se ha metido en la. mochila y sol[i] = false, no, por lo que no podremos contar con su valor.

        El objetivo es encontrar un subconjunto de objetos que maximice el beneficio total .
    
    2. Estructura del arbol de soluciones.

        El arbol es binario, profundidad N, donde N es el numero de tareas.
        Cada rama del arbol representa una decision (rama izq : si) (rama der: no)


    3. Restricciones implicitas y explicitas.
        Explicitas (condiciones expuestas por el enunciado): 
            Debemos llenar la mochila sin pasarnos del peso maximo M maximizando la relacion valor / peso
        Implicitas: (Restricciones estrucurales para la solucion optima.)
            La rama izquierda solo existe si peso_acumulado + objetos[k].peso <= M
            Cada objeto debe estar ordenado segun valor/peso decreciente (mayor valor/peso primero).
            Si un objeto no se mete en la bolsa su peso no baja de la mochila ni su valor.

    4. Tamaño del arbol.
        El arbol tiene un tamaño de 2^N, pero con la funcion de estimacion, podemos podar bastantes ramas cuyo "beneficio_estimado < mejor_beneficio"

    5. Test de factibilidad.    
       Una solucion es factible si maximiza su valor y su peso no sobrepasa la variable M de peso maximo de forma que se cumpla: 
       (1) Usamos una cota optimista (nunca subestima el valor real)
        Calcula el máximo beneficio alcanzable desde un nodo dado de forma optimista. Parte del beneficio ya acumulado y recorre los objetos
         restantes metiéndolos enteros mientras quepan. Cuando encuentra uno que no cabe, aplica la relajación continua (lo parte en fracción), 
         lo que garantiza que nunca subestima el valor real alcanzable
        (2) usamos la poda beneficio:estimado <= mejor_benef ya que si esto se cumple podamos directamente.
*/
struct Objeto{
    int peso;
    int valor;
};

struct Nodo {

    int k;      // nivel del arbol. (objeto actual)
    vector<bool> sol;

    int peso_acumulado; 
    int beneficio_acumulado;
    int beneficio_estimado; // variable a estimar, cta optimista del valor maximo alcanzable.

    bool operator<(Nodo const& o) const{
        return  beneficio_estimado < o.beneficio_estimado; //mayor estimado primero
    }
};

int estimacion(Nodo const& X, vector<Objeto> const& objetos, int M){

    int hueco = M - X.peso_acumulado; // hueco disponible en la mochila.
    int estimacion = X.beneficio_acumulado;
    int i = X.k + 1;
    while(i < objetos.size() && objetos[i].peso <= hueco){
        hueco -= objetos[i].peso;
        estimacion += objetos[i].valor;
        ++i;
    }
    // si aun hay objetos, los partimos y alcanzamos el algoritmo voraz para maximizar el beneficio.
    if(i < objetos.size()){
        estimacion += (hueco / objetos[i].peso)*objetos[i].valor;
    }

    return estimacion;

}

void mochila_entera(vector<Objeto> const& objetos, int M, vector<bool> &mejor_sol, int &mejor_benef){

    size_t N = objetos.size();
    Nodo Y; 
    Y.k = -1;
    Y.sol = vector<bool>(N, false);

    Y.peso_acumulado = 0;
    Y.beneficio_acumulado = 0;
    Y.beneficio_estimado = estimacion(Y, objetos, M);

    priority_queue<Nodo> q;
    q.push(Y);
    mejor_benef = -1;

    while(!q.empty() && q.top().beneficio_estimado > mejor_benef){
        Y = q.top(); q.pop();
        Nodo X(Y);
        X.k++;
        // cogemos el objeto 
        if(Y.peso_acumulado + objetos[X.k].peso <= M){
            X.sol[X.k] = true;
            X.beneficio_acumulado = Y.beneficio_acumulado + objetos[X.k].valor;
            X.peso_acumulado = Y.peso_acumulado + objetos[X.k].peso;
            X.beneficio_estimado = Y.beneficio_estimado;
            // no estimamos porque ya tenemos el valro maximo (en un inicio obtenemos una estimacion de que cogemos el objeto para poder maximizar el valor.)
            //if(X.beneficio_estimado > mejor_benef){ 

                if(X.k == N - 1){
                    mejor_sol = X.sol;
                    mejor_benef = X.beneficio_acumulado;    
                }
                else{
                    q.push(X);
                }
            //}
        }
        X.sol[X.k] = false;
        X.beneficio_acumulado = Y.beneficio_acumulado;
        X.peso_acumulado = Y.peso_acumulado;

        X.beneficio_estimado = estimacion(X, objetos, M);
        if(X.beneficio_estimado > mejor_benef){

            if(X.k == N - 1){
                mejor_sol = X.sol;
                mejor_benef = X.beneficio_acumulado;    
            }
            else{
                    q.push(X);
            }
        }
    }

}
