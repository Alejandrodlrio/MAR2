// Usuario: Alejandro del Rio Caballero

#include <iostream>
#include <fstream>
#include <vector>
using namespace std;
#include "Matriz.h"
#include "EnterosInf.h"

/*
Problema con 3 soluciones:
    1º Matematica: De cuantas formas diferentes podriamos formar la cuerda de ese tamaño? --> numero de formas
    2º ingeniero: hacer la cometa con el minimo numero de cordeles. --> minimizar numero de cordeles.
    3º Economista: hacer la cometa mas barata con los cordeles que tenemos --> minimizar coste

    OJO: no se repiten los cordeles.

    Explicacion:
        Recurrencia:



*/
using lli = long long int;
struct pack
{
    int longitud;
    int coste;
};

struct sol
{
    lli formas;
    EntInf cuerdas;
    EntInf minCoste;
};

sol cuerdas(Matriz<lli> &numFormas, Matriz<EntInf> &minCuerdas, Matriz<EntInf> &minCost, vector<pack> const &v, int n, int len)
{

    numFormas[0][0] = 1; // hay exactamente 1 forma de sumar 0, es decir, no usar ningun cordel :
    minCuerdas[0][0] = 0;
    minCost[0][0] = 0;
    for (int i = 1; i <= n; ++i)
    {
        numFormas[i][0] = 1;
        minCuerdas[i][0] = 0;
        minCost[i][0] = 0;
        for (int j = 1; j <= len; ++j)
        {
            if (j < v[i - 1].longitud)
            {
                numFormas[i][j] = numFormas[i - 1][j]; // no usamos el cordel i.
                minCuerdas[i][j] = minCuerdas[i - 1][j];
                minCost[i][j] = minCost[i - 1][j];
            }
            else
            {
                numFormas[i][j] = numFormas[i - 1][j] + numFormas[i - 1][j - v[i - 1].longitud];
                minCuerdas[i][j] = min(minCuerdas[i - 1][j], minCuerdas[i - 1][j - v[i - 1].longitud] + 1);
                minCost[i][j] = min(minCost[i - 1][j], minCost[i - 1][j - v[i - 1].longitud] + v[i - 1].coste);
            }
        }
    }

    return {numFormas[n][len], minCuerdas[n][len], minCost[n][len]};
}

bool resuelveCaso()
{
    int n, len;
    cin >> n >> len;
    if (!cin)
        return false;

    vector<pack> v(n);
    for (auto &i : v)
        cin >> i.longitud >> i.coste;

    Matriz<lli> matematico(n + 1, len + 1, 0);
    Matriz<EntInf> ingeniero(n + 1, len + 1, Infinito);
    Matriz<EntInf> economista(n + 1, len + 1, Infinito);

    sol r = cuerdas(matematico, ingeniero, economista, v, n, len);

    if (r.formas > 0)
    {
        cout << "SI " << r.formas << " " << r.cuerdas << " " << r.minCoste;
    }
    else
    {
        cout << "NO";
    }
    cout << "\n";

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
