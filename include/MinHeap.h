#ifndef MINHEAP_H_INCLUDE
#define MINHEAP_H_INCLUDE

#include <iostream>
#include <vector>

class MinHeap // La ra�z es el m�s chico
{
    private:
        std::vector<int> heap;

        void heapifyUp(int index); // Funci�n para mantener la propiedad del heap despu�s de insertar un elemento.

        void heapifyDown(int index); // Funci�n para mantener la propiedad del heap despu�s de eliminar un elemento.

    public:
        MinHeap(); // Constructor de la clase.

        ~MinHeap(); // Destructor de la clase.

        void insert(int value); // Insertar un elemento en el heap.

        int extractMin(); // Eliminar y devolver el elemento m�nimo del heap.

        int peekMin(); // Ver el elemento m�nimo sin eliminarlo.

        bool isEmpty(); // Comprobar si el heap est� vac�o.
};

#endif // MINHEAP_H_INCLUDE
