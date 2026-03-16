#include <iostream>
// #include "EnterosInf.h"
#include "Matriz.h"
#include <vector>
using namespace std;

/*
    Descripcion del problema: 
        n objetos con peso p y valor v
        mochila con peso m
        objetivo: Maximizar la cantidad de x*v donde x es {0, 1}
            con la restriccion de que sum v*p <= m
*/
struct pack{
    int v;
    int p;
};


//reconstruccion de la solucion: 
vector<bool> reconstruccion(Matriz<int> const&m, vector<pack> const& v, int n, int t){

    vector<bool> x(n, false);
    //hay solucion
    if(m[n][t] != -1){

        int i = n;
        int j = t;
    
        //"Mientras aun quede espacio"
        while(j > 0 && i > 0) {
            if(m[i][j] != m[i - 1][j]){
                x[i - 1] = true;
                j = j - v[i - 1].p;
            }
            --i;
        }
    }


    return x;
}


//Devuelve el valor maximo con x objetos.
int mochila(Matriz<int> &matriz, vector<pack> const& v, int n, int t){


    for(int i = 0; i <= n; ++i){
        matriz[i][0] = 0;
        for(int j = 1; j <= t; ++j){
            if(i == 0){ matriz[i][j] = 0;}
            else{
                if(v[i - 1].p > j) {
                    //copiamos lo obtenido hasta ahora
                    matriz[i][j] = matriz[i - 1][j];
                }
                else{
                    matriz[i][j] = max(matriz[i - 1][j], matriz[i - 1][j - v[i - 1].p] + v[i-1].v);
                }
            }
        }

    }
    return matriz[n][t];

}

// mochila con optimizacion en espacio: O(n) en lugar de O(nxt)
// int mochila_opt(vector<pack> const& v, int n, int t){
//     vector<int> matriz(t + 1, 0);
//     for(int i = 1; i <= n; ++i){
//         for(int j = t; j >= v[i - 1].p; j--){
//             matriz[j] = max(matriz[j], matriz[j - v[i - 1].p] + v[i - 1].v);
//         }
//     }

//     return matriz[t];
// }

void resuelveCaso(){
    int n, m; cin >> n >> m;
    
    vector<pack> x(n);
    for(auto &i: x) cin >> i.v >> i.p;

    Matriz<int> matriz(n+1, m+1, -1);

    int r = mochila(matriz, x, n, m);
    // int r = mochila_opt(x,n, m);
    cout << "Valor maximo obtenido: " << r << endl;
    vector<bool> v = reconstruccion(matriz, x, n, m);
    cout << "reconstruccion de la solucion: " ; 
    for(auto i: v) cout << i << " ";
}



int main(){

    resuelveCaso();
    return 0;

}