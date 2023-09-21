#include <cassert>
#include "Sets.h"

/**
* Constructor de la clase.
*
* Complejidad: O(n), donde n es la cantidad de nodos del conjunto disjunto.
*/
Sets::Sets(int n)
{
    // Reservo el espacio en memoria necesario para el arreglo din�mico.
    p = new int[n+1];

    // Lleno las celdas del arreglo din�mica con un valor discernible.
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
    // Vac�o el arreglo din�mico.
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
* Complejidad: O(max{n, e �(n)}), por invocar a Sets::Find.
*/
bool Sets::Connected (int i, int j)
{
    // Retorna True si i y j est�n en la misma componente.
    // En caso contrario, retorna falso.
    return (Find(i) == Find(j));
}

/**
* Unir dos conjuntos.
*
* Regla del peso para la Union(i, j):
* Si el n�mero de nodos en el �rbol i es menor o igual al n�mero de nodos en j, haremos j
* padre de i. De lo contrario, haremos i padre de j.
*
* Para la implementaci�n, necesitamos saber cu�ntos nodos tiene cada �rbol.
*
* Complejidad: O(log n), donde n es la cantidad de v�rtices.
*/
void Sets::Union(int i, int j)
{
    // i y j deben ser ra�ces de conjuntos para realizar la uni�n.
    assert((p[i] < 0) && (p[j] < 0));

    int temp = p[i] + p[j];

    // Si i tiene menos nodos...
    // (Recordemos que la cantidad de nodos del �rbol se representa con n�meros negativos en la celda del padre).
    if ( p[i] > p[j])
    {
        // Indico que el �rbol en i tiene de padre a j.
        p[i] = j ;
        p [j] = temp;
    }
    // Si j es quien tiene menos nodos...
    else
    {
        // Indico que el �rbol en j tiene de padre a i.
        p[j] = i ;
        p [i] = temp;
    }

    // Dos conjuntos separados se unieron. Ahora el n�mero de conjuntos es menor.
    conjuntos--;
}

/**
* Encuentra el �nico conjunto que tiene un elemento dado.
*
* Utilizamos la t�cnica de comprensi�n de camino (Collapsing Rule).
*
* Complejidad: O(max{n, e �(n)}), donde: e es la cantidad de aristas; n, la cantidad de v�rtices entre
* i y la ra�z; y �, la funci�n inversa de Ackermann.
*
* Referencia: Computer Algorithms (Horowitz, E.; Sahni, S.; Rajasekaran, S.).
* Cap�tulo 2.5: Sets and disjoint set union, p�gina 101.
*/
int Sets::Find(int i)
{
    int r = i, temp;

    // Busco la ra�z.
    while(p[r] >= 0)
        r = p[r];

    // Cuelgo cada nodo en el camino de i a la ra�z de la ra�z.
    // De esta forma, para cada nodo para el cual quiera realizar la b�squeda que haya en ese camino,
    // tendr� una complejidad reducidad, la indica por la funci�n inversa de Ackermann.
    // Es la primera vez, cuando ordeno, que la complejidad est� dada por O(n).
    while(i != r)
    {
        temp = p[i];
        p[i] = r;
        i= temp;
    }

    // Retorna
    return r;
}
