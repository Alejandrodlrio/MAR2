#include <iostream>
#include "../Archivos de apoyo/Matriz.h"
using namespace std;

//palindromo: longitud del palindromo mas largo que podemos obtener desde i hasta j
// 

//programacion dinamica descendente:
int patindromo(Matriz<int> & m, string s, int i, int j)
{
    if(m[i][j] == -1){
        if(i > j) m[i][j] = 0;
        else if(i == j) m[i][j] = 1;
        else if(s[i] == s[j]) m[i][j] = patindromo(m, s, i + 1, j - 1) + 2; 
        else{
            m[i][j] = max(patindromo(m, s, i + 1, j), patindromo(m, s, i, j - 1));
        }
    }
    return m[i][j];
}

void resuelveCaso(){


    string s; cin >> s;
    int n = s.length();


    Matriz<int> m(n, n, -1);
    //llamada a la funcion patindromo que devuelve el palindromo de mayor longitud.
    int pal = patindromo(m, s, 0, n-1);
    cout << pal << "\n";
}

int main(){
    resuelveCaso();
    return 0;
}