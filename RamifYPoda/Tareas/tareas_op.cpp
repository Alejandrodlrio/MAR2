#include <vector>
#include <iostream>
#include <queue>
using namespace std;

// cosas a explicar: 
/*
    1. Describir espacio de soluciones.
        Representa todas las posibles secuencias de decisiones sobre si ejecutar o no cada tarea dentro de su plazo
        donde sol[i] = true indica que la tarea i se ejecuta a tiempo sin penalizacion y sol[i] = false, nno, por lo que se pagara su penalizacion.

        El objetivo es encontrar un subconjunto de tareas a ejecutar que nos minimice el coste total de las penalizaciones (minimizar la variable coste_acumulado)
    
    2. Estructura del arbol de soluciones.

        El arbol es binario, profundidad N, donde N es el numero de tareas.
        Cada rama del arbol representa una decision (rama izq : si) (rama der: no)


    3. Restricciones implicitas y explicitas.
        Explicitas: 
            Condiciones directamente impuestas por el enunciado, es decir, cada tarea debe ejecutarse antes de que se cumpla su plazo. 
        Ademas la tupla devuelta debe ser binaria y el coste de cada tarea deberá ser >= 0.

        Implicitas: 
            Restricciones estrucurales para la solucion optima.
            Cada tarea deben estar ordenadas por plazo de forma no decreciente.
            Si una tarea no se ejecuta, su tiempo no se acumula, pero si su coste.

            Buscamos minimizar el pago de multas (coste)

    4. Tamaño del arbol.
        El arbol tiene un tamaño de 2^N, pero con la funcion de estimacion, podemos podar bastantes ramas cuyo "coste_estiamdo >= mejor_coste"

    5. Test de factibilidad.    
        (1) -> X.tiempo_acumulado + tareas[i].tiempo < tareas[i].plazo 
            Solo generamos siempre y cuando la tarea a ejecutar está dentro de su plazo.
        (2) -> X.coste_estimado < mejor_coste
            La estimacion que estamos haciendo es optimista ya que asume que todas las tareas restantes sin plazo cumplido pagaran multa
            lo que da una cota inferior del coste final.
*/
struct tarea{
    int tiempo;// tiempo de ejecucion
    int plazo; // limite
    int coste; // multa a pagar  
};

struct Nodo{
    int tiempo_acumulado;
    int coste_acumulado;
    int coste_estimado;
    
    int k; // nivel
    vector<bool> sol; // solucionado
    bool operator<(Nodo const& o) const{
        return o.coste_estimado < coste_estimado;
    }
};


int cota_optimista(Nodo const& X, vector<tarea> const&tareas){
    int coste_estimado, coste_actual;
    coste_actual = X.coste_acumulado;
    coste_estimado = coste_actual;
    for(int i = X.k + 1; i < tareas.size(); ++i){
        // Actuamos segun las restricciones: 
        if(X.tiempo_acumulado + tareas[i].tiempo > tareas[i].plazo){
            //pagamos la multa
            coste_estimado += tareas[i].coste;
        }
    }

    return coste_estimado;
}

int cota_pesimista(Nodo X, vector<tarea> const& tareas){
    int pesimista = X.coste_acumulado;
    int tiempo_pesimista = X.tiempo_acumulado;
    // asumimos que no ejecutamos ninguna tarea y pagamos todas las multas a partir de k + 1 que no cumpla
    for(int i = X.k + 1; i < tareas.size(); ++i){

        if(tiempo_pesimista + tareas[i].tiempo > tareas[i].plazo){
            pesimista += tareas[i].coste;
        }
        else // parte pesimista.
            tiempo_pesimista += tareas[i].tiempo;
    }
    return pesimista;
}

void tareas_rp(vector<tarea> const& tareas, vector<bool> &mejor_sol, int & mejor_coste){
    size_t N = tareas.size();

    // Generamos el nodo raiz.
    Nodo Y;
    Y.k = -1;
    Y.sol = vector<bool>(N, false);
    //Variables a optimizar
    Y.tiempo_acumulado = 0; // no se ha ejecutado aun ninguna tarea. MAXIMIZAR.
    Y.coste_acumulado = 0; // No se ha podido pagar nada aun . MINIMIZAR.
    
    Y.coste_estimado = cota_optimista(Y, tareas);
    mejor_coste = cota_pesimista(Y, tareas);

    priority_queue<Nodo> q;
    
    while(!q.empty() && q.top().coste_estimado < mejor_coste){
        Y = q.top();
        q.pop();

        //inicializamos el ndo: 
        Nodo X(Y);
        ++X.k;
        // SI ejecutamos la tarea (Rama izquierda): 
        if(Y.tiempo_acumulado + tareas[X.k].tiempo <= tareas[X.k].plazo){
            X.sol[X.k] = true;
            X.coste_acumulado = Y.coste_acumulado; // no penalizamos nada 
            X.tiempo_acumulado = Y.tiempo_acumulado + tareas[X.k].tiempo;
            X.coste_estimado = cota_optimista(X, tareas);

            if(X.coste_estimado < mejor_coste){
                if(X.k == N - 1){
                    mejor_sol = X.sol;
                    mejor_coste = X.coste_acumulado;
                }
                else 
                    q.push(X);
            }
        }

        // NO ejecutamos la tarea (Rama derecha): 
        X.sol[X.k] = false;
        X.coste_acumulado = Y.coste_acumulado + tareas[X.k].coste; // si hay penalizacion
        X.tiempo_acumulado = Y.tiempo_acumulado;
        X.coste_estimado = cota_optimista(X, tareas);
        if(X.coste_estimado < mejor_coste){
            if(X.k == N - 1){
                mejor_sol = X.sol;
                mejor_coste = X.coste_acumulado;
            }
            else {

                q.push(X);
                mejor_coste = min(mejor_coste, cota_pesimista(X, tareas));
            }
        }
        
       
    }






}








int main(){



    return 0;
}