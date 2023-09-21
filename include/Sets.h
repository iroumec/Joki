#ifndef SETS_H_INCLUDED
#define SETS_H_INCLUDED

class Sets
{
    private:
        int *p;
        int conjuntos; // Almacena la cantidad de conjuntos

    public:
        Sets(int n); // Constructor de la clase

        ~Sets(); // Destructor de la clase

        int Count(); // Retorna la cantidad de conjuntos

        bool Connected(int i, int j); // Consulta si dos nodos están conectados

        void Union(int i, int j); // Une dos conjuntos

        int Find(int j); // Encuentra el padre de un nodo
};

#endif // SETS_H_
