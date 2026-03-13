#include <iostream>
#include <fstream>
#include <vector>
using namespace std;
#include "Matriz.h"
#include "EnterosInf.h"


EntInf cambio(Matriz<EntInf> &mat, vector<int> const& monedas, int n, int m){
    // resolvemos el problema mediante programacion dinamica ascendente


    mat[0][0] = 0;
    for(int i = 1; i <= n; ++i){
        mat[i][0] = 0;
        for(int j = 0; j <= m; ++j){
            if(j < monedas[i - 1]){ // la cantidad a pagar es menor que la moneda con la que queremos pagar
                mat[i][j] = mat[i - 1][j];
            }
            else{
                mat[i][j] = min(mat[i-1][j], mat[i][j - monedas[i]] + 1);

            }
        }
    }

    return mat[n][m];

}

void resuelveCaso(){
    int n, total; cin >> n >> total;
    vector<int> monedas(n);

    //Vector de monedas: 
    for(auto &i: monedas) cin >> i;

    // Creamos la matriz de n+1 filas y n+1 columnas, inicializada a infinito
    Matriz<EntInf> m(n+1, total+1, Infinito); 
    EntInf res = cambio(m, monedas, n, total);
    cout << res << "\n";
}
int main() {
    #ifndef DOMJUDGE
    std::ifstream in("casos.txt");
    auto cinbuf = std::cin.rdbuf(in.rdbuf());
    #endif
    
    
    resuelveCaso();
    
    
    


    #ifndef DOMJUDGE
    cin.rdbuf(cinbuf);
    system("PAUSE");
    #endif
    return 0;
}