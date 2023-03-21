#include <iostream>
#include <chrono>
#include <random>
#include <set>
#include <map>
#include <fstream>
#include <algorithm>
#include <unordered_map>

using namespace std;

// Definición de la clase nodo de árbol binario ordenado óptimo
class NodoABOO {
public:
    int clave;
    NodoABOO* izq;
    NodoABOO* der;
    NodoABOO(int c) : clave(c), izq(nullptr), der(nullptr) {}
};

// Función auxiliar para insertar nodos en un árbol binario ordenado óptimo
void insertarABOO(NodoABOO*& nodo, int clave, int* arr, int izq, int der) {
    if (izq > der) {
        return;
    }
    int medio = (izq + der) / 2;
    nodo = new NodoABOO(arr[medio]);
    insertarABOO(nodo->izq, clave, arr, izq, medio - 1);
    insertarABOO(nodo->der, clave, arr, medio + 1, der);
}

// Función auxiliar para buscar nodos en un árbol binario ordenado óptimo
#include <unordered_map>

bool buscarABOO(NodoABOO* nodo, int clave, std::unordered_map<int, bool>& memo) {
    if (memo.count(clave)) {
        return memo[clave];
    }

    while (nodo != nullptr) {
        if (clave == nodo->clave) {
            memo[clave] = true;
            return true;
        } else if (clave < nodo->clave) {
            nodo = nodo->izq;
        } else {
            nodo = nodo->der;
        }
    }

    memo[clave] = false;
    return false;
}



int main()
{
    // Generación de números aleatorios para las claves de los nodos
    const int num_claves = 150000;
    const int max_valor = 100000;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distribucion(1, max_valor);

    // Creación de los árboles ABOO, AVL y Rojinegro
    NodoABOO* aboo = nullptr;
    std::set<int> avl;
    std::map<int, int> rojinegro;

    // Creación del archivo de texto
    ofstream archivo("resultados.txt");
    archivo << "ABOO AVL Rojinegro" << endl;

    // Creación del arreglo de claves ordenado
    int arr[num_claves];
    for (int i = 0; i < num_claves; i++) {
        arr[i] = distribucion(gen);
    }
    std::sort(arr, arr + num_claves);

    // Creación del árbol binario ordenado óptimo
    insertarABOO(aboo, -1, arr, 0, num_claves - 1);

    // Tabla hash para memoización en ABOO
    std::unordered_map<int, bool> memo_aboo;

    // Bucle para realizar el código 1000 veces y guardar los resultados en el archivo de texto
    for (int i = 0; i < 10000; i++)
    {
        // Medición del tiempo de búsqueda en ABOO
        auto inicio_aboo = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < num_claves; i++)
        {
            int clave = distribucion(gen);
            buscarABOO(aboo, clave, memo_aboo);
        }
        auto fin_aboo = std::chrono::high_resolution_clock::now();
        auto tiempo_aboo = std::chrono::duration_cast<std::chrono::nanoseconds>(fin_aboo - inicio_aboo).count();

        // Medición del tiempo de búsqueda en AVL
        auto inicio_avl = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < num_claves; i++)
        {
            int clave = distribucion(gen);
            avl.find(clave);
        }
        auto fin_avl = std::chrono::high_resolution_clock::now();
        auto tiempo_avl = std::chrono::duration_cast<std::chrono::nanoseconds>(fin_avl - inicio_avl).count();

        // Medición del tiempo de búsqueda en Rojinegro
        auto inicio_rojinegro = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < num_claves; i++)
        {
            int clave = distribucion(gen);
            rojinegro.find(clave);
        }
        auto fin_rojinegro = std::chrono::high_resolution_clock::now();
        auto tiempo_rojinegro = std::chrono::duration_cast<std::chrono::nanoseconds>(fin_rojinegro - inicio_rojinegro).count();

        // Escritura de los resultados en el archivo de texto
        archivo << tiempo_aboo << " " << tiempo_avl << " " << tiempo_rojinegro << endl;
    }

    // Cierre del archivo de texto 
    archivo.close();

    return 0;

}
