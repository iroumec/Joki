#include <cassert>
#include "Sets.h"

/**
* Constructor de la clase.
*
* Complejidad: O(n), donde n es la cantidad de nodos del conjunto disjunto.
*/
Sets::Sets(int n)
{
    // Reservo el espacio en memoria necesario para el arreglo dinámico.
    p = new int[n+1];

    // Lleno las celdas del arreglo dinámica con un valor discernible.
    for (unsigned int i = 0; i <= n; i++)
        p[i] = -1;

    conjuntos = n;
}

/**
* Destructor de la clase.
*
* Complejidad: O(1).
*/
Sets::~Sets()
{
    // Vacío el arreglo dinámico.
    delete[] p;
}

/**
* Consultar por la cantidad de conjuntos disjuntos.
*
* Complejidad: O(1), retorno un atributo.
*/
int Sets::Count()
{
    // Retorno la cantidad de conjuntos.
    return conjuntos;
}

/**
* Consulto si dos nodos pertenecen al mismo conjunto disjunto.
*
* Complejidad: O(max{n, e á(n)}), por invocar a Sets::Find.
*/
bool Sets::Connected (int i, int j)
{
    // Retorna True si i y j están en la misma componente.
    // En caso contrario, retorna falso.
    return (Find(i) == Find(j));
}

/**
* Unir dos conjuntos.
*
* Regla del peso para la Union(i, j):
* Si el número de nodos en el árbol i es menor o igual al número de nodos en j, haremos j
* padre de i. De lo contrario, haremos i padre de j.
*
* Para la implementación, necesitamos saber cuántos nodos tiene cada árbol.
*
* Complejidad: O(log n), donde n es la cantidad de vértices.
*/
void Sets::Union(int i, int j)
{
    // i y j deben ser raíces de conjuntos para realizar la unión.
    assert((p[i] < 0) && (p[j] < 0));

    int temp = p[i] + p[j];

    // Si i tiene menos nodos...
    // (Recordemos que la cantidad de nodos del árbol se representa con números negativos en la celda del padre).
    if ( p[i] > p[j])
    {
        // Indico que el árbol en i tiene de padre a j.
        p[i] = j ;
        p [j] = temp;
    }
    // Si j es quien tiene menos nodos...
    else
    {
        // Indico que el árbol en j tiene de padre a i.
        p[j] = i ;
        p [i] = temp;
    }

    // Dos conjuntos separados se unieron. Ahora el número de conjuntos es menor.
    conjuntos--;
}

/**
* Encuentra el único conjunto que tiene un elemento dado.
*
* Utilizamos la técnica de comprensión de camino (Collapsing Rule).
*
* Complejidad: O(max{n, e á(n)}), donde: e es la cantidad de aristas; n, la cantidad de vértices entre
* i y la raíz; y á, la función inversa de Ackermann.
*
* Referencia: Computer Algorithms (Horowitz, E.; Sahni, S.; Rajasekaran, S.).
* Capítulo 2.5: Sets and disjoint set union, página 101.
*/
int Sets::Find(int i)
{
    int r = i, temp;

    // Busco la raíz.
    while(p[r] >= 0)
        r = p[r];

    // Cuelgo cada nodo en el camino de i a la raíz de la raíz.
    // De esta forma, para cada nodo para el cual quiera realizar la búsqueda que haya en ese camino,
    // tendrá una complejidad reducidad, la indica por la función inversa de Ackermann.
    // Es la primera vez, cuando ordeno, que la complejidad está dada por O(n).
    while(i != r)
    {
        temp = p[i];
        p[i] = r;
        i= temp;
    }

    // Retorna
    return r;
}
