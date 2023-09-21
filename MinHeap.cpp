#include "MinHeap.h"

MinHeap::MinHeap()
{
}

MinHeap::~MinHeap()
{
    // Vacío el vector.
    heap.clear();
}


void MinHeap::heapifyUp(int index)
{
    // Obtengo el índice del padre del elemento en el índice.
    int parentIndex = (index - 1) / 2;

    // Mientras el índice sea mayor a 0 y el elemento del índice sea menor al del padre...
    while ((index > 0) && (heap[index] < heap[parentIndex]))
    {
        // Intercambio el padre y el elemento en cuestión.
        std::swap(heap[index], heap[parentIndex]);

        // Ahora el índice del elemento es el del padre.
        index = parentIndex;

        // Recalculo el índice del padre.
        parentIndex = (index - 1) / 2;
    }
}

void MinHeap::heapifyDown(int index)
{
    // Obtengo el índice a los hijos a la izquierda y a la derecha.
    int leftChild = 2 * index + 1;
    int rightChild = 2 * index + 2;
    int smallest = index;

    // Si el hijo izquierdo existe (su índice está dentro del tamaño del heap),
    // y el elemento es menor al padre, el menor elemento será el hijo izquierdo.
    if ((leftChild < heap.size()) && (heap[leftChild] < heap[smallest]))
    {
        // El índice del menor elemento del heap se vuelve el hijo.
        smallest = leftChild;
    }

    // Si el hijo derecho existe (su índice está dentro de los rangos del heap),
    // y el elemento es menor al elemento cuyo índice es "smallest", redefino "smallest".
    if (rightChild < heap.size() && heap[rightChild] < heap[smallest])
    {
        smallest = rightChild;
    }

    // Si el menor no es el elemento que tengo de raíz...
    if (smallest != index)
    {
        // El menor se intercambia con la raíz.
        std::swap(heap[index], heap[smallest]);

        // Hago un llamado recursivo para seguir ordenando.
        heapifyDown(smallest);
    }
}

void MinHeap::insert(int value)
{
    // Agrego el elemento al final del vector.
    heap.push_back(value);

    // Defino index como el índice del elemento que agregué.
    int index = heap.size() - 1;

    // Invoco a heapify para ordenar el heap.
    heapifyUp(index);
}

int MinHeap::extractMin()
{
    // Si el heap está vacío, tendré una excepción.
    if (heap.empty())
    {
        throw std::out_of_range("El heap está vacío.");
    }

    // El mínimo valor del heap es la raíz.
    int minValue = heap[0];

    // La raíz del heap se vuelve el último elemento de este.
    heap[0] = heap.back();

    // Como ahora el último elemento pasa a ser la raíz, lo elimino del heap.
    heap.pop_back();

    // Ordeno el heap.
    heapifyDown(0);

    // Retorno el valor mínimo.
    return minValue;
}

int MinHeap::peekMin()
{
    // Si el heap está vacío, tendré una excepción.
    if (heap.empty())
    {
        throw std::out_of_range("El heap está vacío.");
    }

    // Retorno la raíz del heap.
    return heap[0];
}

bool MinHeap::isEmpty()
{
    // Retorno si el vector está vacío.
    return heap.empty();
}
