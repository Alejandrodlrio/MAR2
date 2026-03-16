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


int mochila(Matriz<int> &m, vector<pack> const& v, int i, int j){
    
    //vemos si el problema ya esta resuelto y devolvemos la solucion; 
    if(m[i][j] != -1){
        return m[i][j];
    }
    //Caso base;
    if(i == 0 || j == 0) m[i][j] = 0 ;
    else if(j < v[i - 1].p) m[i][j] = mochila(m, v, i-1, j);
    else{
        m[i][j] = max(mochila(m, v, i-1, j), mochila(m, v, i-1, j - v[i - 1].p) + v[i - 1].v);
    }
    return m[i][j];
}


void resuelveCaso(){
    int n, m; cin >> n >> m;
    
    vector<pack> x(n);
    for(auto &i: x) cin >> i.v >> i.p;

    Matriz<int> matriz(n+1, m+1, -1);

    int r = mochila(matriz, x, n, m);
    cout << "Valor maximo obtenido: " << r << endl;
}



int main(){

    resuelveCaso();
    return 0;

}