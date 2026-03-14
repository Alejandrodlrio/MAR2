#include <iostream>
#include <vector>
#include "Matriz.h"
#include "EnterosInf.h"
using namespace std;

vector<int> reconstruccion(Matriz<EntInf> const& m, vector<int> const& monedas, int n, int total) {
	vector<int> z;
	if (m[n][total] != Infinito) { // hay solucion.
		
		int i = n;
		int j = total;
		while (j > 0) { // aun queda por pagar.
			if (m[i][j] != m[i - 1][j] && monedas[i - 1] <= j) {
				z.push_back(monedas[i - 1]);
				j = j - monedas[i - 1];
			}
			else {
				i--;
			}
		}
	}
	
	return z;
}
EntInf cambio(Matriz<EntInf>& m, vector<int> const& monedas, int n, int total) {
	
	m[0][0] = 0;
	for (int i = 1; i <= n; ++i) {
		m[i][0] = 0;
		for (int j = 1; j <= total; ++j) {
			if (j < monedas[i - 1]) {
				m[i][j] = m[i - 1][j];
			}
			else {
				m[i][j] = min(m[i - 1][j], m[i][j - monedas[i - 1]] + 1);
			}
		}
	}

	return m[n][total];

}

void resuelveCaso()
{
	int total, n; cin >> total >> n;
	
	vector<int> monedas(n);
	for (auto& i : monedas) cin >> i;

	Matriz<EntInf> m(n + 1, total + 1, Infinito);

	//llamamos a la funcion: 
	EntInf r = cambio(m, monedas, n, total);
	vector<int> z = reconstruccion(m, monedas, n, total);
	cout << "Numero de monedas: " << r << "\n";
	cout << "Monedas utilizadas: ";
	if (z.size() > 0) {
		for (int i = 0; i < z.size(); ++i)
			cout << z[i] << " ";
	}
	else {
		cout << "0";
	}

}

int main() {


	resuelveCaso();

	return 0;
}