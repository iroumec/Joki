#ifndef MINHEAP_H_INCLUDE
#define MINHEAP_H_INCLUDE

#include <iostream>
#include <vector>

class MinHeap // La raíz es el más chico
{
    private:
        std::vector<int> heap;

        void heapifyUp(int index); // Función para mantener la propiedad del heap después de insertar un elemento.

        void heapifyDown(int index); // Función para mantener la propiedad del heap después de eliminar un elemento.

    public:
        MinHeap(); // Constructor de la clase.

        ~MinHeap(); // Destructor de la clase.

        void insert(int value); // Insertar un elemento en el heap.

        int extractMin(); // Eliminar y devolver el elemento mínimo del heap.

        int peekMin(); // Ver el elemento mínimo sin eliminarlo.

        bool isEmpty(); // Comprobar si el heap está vacío.
};

#endif // MINHEAP_H_INCLUDE
