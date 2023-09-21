#include "MinHeap.h"

MinHeap::MinHeap()
{
}

MinHeap::~MinHeap()
{
    // Vac�o el vector.
    heap.clear();
}


void MinHeap::heapifyUp(int index)
{
    // Obtengo el �ndice del padre del elemento en el �ndice.
    int parentIndex = (index - 1) / 2;

    // Mientras el �ndice sea mayor a 0 y el elemento del �ndice sea menor al del padre...
    while ((index > 0) && (heap[index] < heap[parentIndex]))
    {
        // Intercambio el padre y el elemento en cuesti�n.
        std::swap(heap[index], heap[parentIndex]);

        // Ahora el �ndice del elemento es el del padre.
        index = parentIndex;

        // Recalculo el �ndice del padre.
        parentIndex = (index - 1) / 2;
    }
}

void MinHeap::heapifyDown(int index)
{
    // Obtengo el �ndice a los hijos a la izquierda y a la derecha.
    int leftChild = 2 * index + 1;
    int rightChild = 2 * index + 2;
    int smallest = index;

    // Si el hijo izquierdo existe (su �ndice est� dentro del tama�o del heap),
    // y el elemento es menor al padre, el menor elemento ser� el hijo izquierdo.
    if ((leftChild < heap.size()) && (heap[leftChild] < heap[smallest]))
    {
        // El �ndice del menor elemento del heap se vuelve el hijo.
        smallest = leftChild;
    }

    // Si el hijo derecho existe (su �ndice est� dentro de los rangos del heap),
    // y el elemento es menor al elemento cuyo �ndice es "smallest", redefino "smallest".
    if (rightChild < heap.size() && heap[rightChild] < heap[smallest])
    {
        smallest = rightChild;
    }

    // Si el menor no es el elemento que tengo de ra�z...
    if (smallest != index)
    {
        // El menor se intercambia con la ra�z.
        std::swap(heap[index], heap[smallest]);

        // Hago un llamado recursivo para seguir ordenando.
        heapifyDown(smallest);
    }
}

void MinHeap::insert(int value)
{
    // Agrego el elemento al final del vector.
    heap.push_back(value);

    // Defino index como el �ndice del elemento que agregu�.
    int index = heap.size() - 1;

    // Invoco a heapify para ordenar el heap.
    heapifyUp(index);
}

int MinHeap::extractMin()
{
    // Si el heap est� vac�o, tendr� una excepci�n.
    if (heap.empty())
    {
        throw std::out_of_range("El heap est� vac�o.");
    }

    // El m�nimo valor del heap es la ra�z.
    int minValue = heap[0];

    // La ra�z del heap se vuelve el �ltimo elemento de este.
    heap[0] = heap.back();

    // Como ahora el �ltimo elemento pasa a ser la ra�z, lo elimino del heap.
    heap.pop_back();

    // Ordeno el heap.
    heapifyDown(0);

    // Retorno el valor m�nimo.
    return minValue;
}

int MinHeap::peekMin()
{
    // Si el heap est� vac�o, tendr� una excepci�n.
    if (heap.empty())
    {
        throw std::out_of_range("El heap est� vac�o.");
    }

    // Retorno la ra�z del heap.
    return heap[0];
}

bool MinHeap::isEmpty()
{
    // Retorno si el vector est� vac�o.
    return heap.empty();
}
