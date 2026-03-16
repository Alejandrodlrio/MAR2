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
vector<int> reconstruccion(Matriz<int> const&m, vector<pack> const& v, int n, int t){

    vector<int> x;
    //hay solucion
    if(m[n][t] != -1){

        int i = n;
        int j = t;
    
        //"Mientras aun quede espacio"
        while(j > 0 && i > 0){
            if(m[i][j] != m[i - 1][j]){
                x.push_back(i - 1);
                j = j - v[i - 1].p;
            }
            --i;
        }
    }


    return x;
}


//Devuelve el valor maximo con x objetos.
int mochila(Matriz<int> matriz, vector<pack> const& v, int n, int t){

    matriz[0][0] = 0;

    for(int i = 1; i <= n; ++i){
        matriz[i][0] = 0;
        for(int j = 1; j <= t; ++j){
            if(v[i - 1].p > j) {
                //copiamos lo obtenido hasta ahora
                matriz[i][j] = matriz[i - 1][j];
            }
            else{
                matriz[i][j] = max(matriz[i - 1][j], matriz[i - 1][j - v[i - 1].p] + v[i-1].v);
            }
        }

    }
    return matriz[n][t];

}

// mochila_optimizada: 
// int mochila_optimizada(vector<int> matriz, vector<pack> const&v, int n , int t){
//     for(int i = 1; i <= t; ++)
// }
void resuelveCaso(){
    int n, m; cin >> n >> m;
    
    vector<pack> x(n);
    for(auto &i: x) cin >> i.v >> i.p;

    Matriz<int> matriz(n+1, m+1, -1);

    int r = mochila(matriz, x, n, m);
    cout << "Valor maximo obtenido: " << r << endl;
    vector<int> v = reconstruccion(matriz, x, n, m);
    cout << "reconstruccion de la solucion: " << v.size();
}



int main(){

    resuelveCaso();
    return 0;

}