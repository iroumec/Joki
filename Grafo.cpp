#include "Grafo.h"
#include "RedAeroportuaria.h"

// ----------------------------------------------------------------------------------------------------------------- //
//                                                  Subclase Arco                                                    //
// ----------------------------------------------------------------------------------------------------------------- //

template <typename C>
Grafo<C>::Arco::Arco()
// Complejidad temporal: O(1).
// Complejidad espacial: O(1).
{
}

// ----------------------------------------------------------------------------------------------------------------- //

template <typename C>
Grafo<C>::Arco::Arco(const int &adyacente, const C &costo)
// Complejidad temporal: O(C), siendo C la complejidad temporal del operador = del costo del arco.
// Complejidad espacial: O(T), siendo T el costo espacial de almacenar un dato de tipo C.
{
    vertice = adyacente;

    this->costo = costo;
}

// ----------------------------------------------------------------------------------------------------------------- //

template <typename C>
int Grafo<C>::Arco::devolverAdyacente() const
// Complejidad temporal: O(1).
// Complejidad espacial: O(1).
{
    return vertice;
}

// ----------------------------------------------------------------------------------------------------------------- //

template <typename C>
void Grafo<C>::Arco::nuevoCosto(const C &costo)
// Complejidad temporal: O(C), siendo C la complejidad temporal del operador = del costo del arco.
// Complejidad espacial: O(T), siendo T el costo espacial de almacenar un dato de tipo C.
{
    this->costo = costo;
}

// ----------------------------------------------------------------------------------------------------------------- //

template <typename C>
const C &Grafo<C>::Arco::devolverCosto() const
// Complejidad temporal: O(1).
// Complejidad espacial: O(1).
{
    return costo;
}

// ----------------------------------------------------------------------------------------------------------------- //
//                                                  Clase Grafo                                                      //
// ----------------------------------------------------------------------------------------------------------------- //

template <typename C>
Grafo<C>::Grafo()
// Complejidad temporal: O(1).
// Complejidad espacial: O(1).
{
}

// ----------------------------------------------------------------------------------------------------------------- //

template <typename C>
Grafo<C>::Grafo(const Grafo &otroGrafo)
// Complejidad temporal: O(n * m * log n), siendo n el número de vértice del grafo otroGrafo y m, el número promedio
// de adyacentes por vértice. Esto debido a que utiliza el operador =.
// Complejidad temporal: O(n * m * T), donde n es el número de vértices del grafo; m, el número promedio de arcos por
// vértice; y T, el costo espacial de almacenar un dato de tipo C.
{
    *this = otroGrafo;
}

// ----------------------------------------------------------------------------------------------------------------- //

template <typename C>
Grafo<C>::~Grafo()
// Complejidad temporal: O(n + m * C), donde n es el número de vértices; m, el número de arcos; y C, la complejidad
// temporal del destructor de la estructura del costo del arco. Esto debido a que invoca al método ".vaciar()".
// Complejidad espacial: O(1).
{
    grafo.vaciar();
}

// ----------------------------------------------------------------------------------------------------------------- //

template <typename C>
Grafo<C> &Grafo<C>::operator=(const Grafo &otroGrafo)
// Complejidad temporal: O(n * m * log n), donde n es el número de vértices en el grafo y m, el número promedio
// de adyacentes por vértice.
// Complejidad espacial: O(n * m), donde n es la cantidad de vértices del grafo otroGrafo y m, la cantidad promedio
// de adyacentes por vértice.
{
    grafo.clear();

    std::list<int> vertices;
    otroGrafo.devolverVertices(vertices); // O(n)

    std::list<Arco> arcos;

    for (const auto &vertice : vertices) // O(n)
    {
        agregarVertice(vertice); // O(log n)

        otroGrafo.devolverAdyacentes(vertice, arcos); // O(m)

        for (const auto &arco : arcos)                                               // O(m)
            grafo[vertice].insert({arco.devolverAdyacente(), arco.devolverCosto()}); // O(log n)

        arcos.clear(); // O(m)
    }

    return *this;
}

// ----------------------------------------------------------------------------------------------------------------- //

template <typename C>
bool Grafo<C>::estaVacio() const
// Complejidad temporal: O(1).
// Complejidad espacial: O(1).
{
    return grafo.empty();
}

// ----------------------------------------------------------------------------------------------------------------- //

template <typename C>
int Grafo<C>::devolverLongitud() const
// Complejidad temporal: O(1).
// Complejidad espacial: O(1).
{
    return grafo.size();
}

// ----------------------------------------------------------------------------------------------------------------- //

template <typename C>
bool Grafo<C>::existeVertice(const int &vertice) const
// Complejidad temporal: O(log n), siendo n el número de vértices en el grafo.
// Complejidad espacial: O(1).
{
    auto iteradorVertice = grafo.find(vertice);

    return (iteradorVertice != grafo.end());
}

// ----------------------------------------------------------------------------------------------------------------- //

template <typename C>
bool Grafo<C>::existeArco(const int &origen, const int &destino) const
// Complejidad temporal: O(log n + log m), siendo n el número de vértices y m, el número de vértices adyacentes al
// vértice de origen.
// Complejidad espacial: O(1).
{
    bool hallado = false;

    // Busco el vértice de origen
    auto iteradorVerticeOrigen = grafo.find(origen);

    if (iteradorVerticeOrigen != grafo.end())
    {
        // Busco el vértice de destino dentro de los adyacentes al de origen
        auto iteradorVerticeDestino = (iteradorVerticeOrigen->second).find(destino);

        if (iteradorVerticeDestino != (iteradorVerticeOrigen->second).end())
            hallado = true;
    }

    return hallado;
}

// ----------------------------------------------------------------------------------------------------------------- //

template <typename C>
const C &Grafo<C>::costoArco(const int &origen, const int &destino) const
// Complejidad temporal: O(log n + log m), siendo n el número de vértices; y m, el número de adyacentes al vértice de origen.
// Complejidad espacial: O(1).
{
    // Busco el vértice de origen
    auto iteradorVerticeOrigen = grafo.find(origen);

    if (iteradorVerticeOrigen == grafo.end())
        throw std::invalid_argument("No existe el vértice de origen");

    // Busco el vértice destino dentro de los adyacentes al de origen
    auto iteradorVerticeDestino = (iteradorVerticeOrigen->second).find(destino);

    if (iteradorVerticeDestino == (iteradorVerticeOrigen->second).end())
        throw std::invalid_argument("No existe un arco entre el vértice de origen y el de destino");

    return (iteradorVerticeDestino->second);
}

// ----------------------------------------------------------------------------------------------------------------- //

template <typename C>
void Grafo<C>::devolverVertices(std::list<int> &vertices) const
// Complejidad temporal: O(n), siendo n el número de vértices en el grafo.
// Complejidad espacial: O(1).
{
    for (const auto &vertice : grafo)
        vertices.push_back(vertice.first);
}

// ----------------------------------------------------------------------------------------------------------------- //

template <typename C>
void Grafo<C>::devolverAdyacentes(const int &origen, std::list<Arco> &adyacentes) const
// Complejidad temporal: O(log n + m), siendo n el número de vértices y m, la cantidad de adyacentes del vértice origen.
// Complejidad espacial: O(1). No consideramos a lista a devolver como parte de la complejidad espacial.
{
    auto iteradorAeropuertoOrigen = grafo.find(origen); // O(log n)

    if (iteradorAeropuertoOrigen != grafo.end())
    {
        for (const auto &adyacente : (iteradorAeropuertoOrigen->second)) // O(m)
        {
            Arco nuevo_arco(adyacente.first, adyacente.second);
            adyacentes.push_back(nuevo_arco);
        }
    }
}

// ----------------------------------------------------------------------------------------------------------------- //

template <typename C>
void Grafo<C>::agregarVertice(const int &vertice)
// Complejidad temporal: O(log n), siendo n el número de vértices del grafo.
// Complejidad espacial: O(1).
{
    std::map<int, C> arcos;

    grafo.insert({vertice, arcos}); // O(log n)
}

// ----------------------------------------------------------------------------------------------------------------- //

template <typename C>
void Grafo<C>::eliminarVertice(const int &verticeAEliminar)
// Complejidad temporal: O(n log n + n log m), siendo n el número de vértices y m, el número de adyacentes promedio
// al vértice de origen.
// Complejidad espacial: O(1).
{
    // Elimino los arcos que haya de otros vértices al que quiero eliminar.
    for (const auto &vertice : grafo) // O(n)
    {
        if (vertice.first != verticeAEliminar)
            eliminarArco(vertice.first, verticeAEliminar); // O(log n + log m)
    }

    grafo.erase(verticeAEliminar); // O(log n)
}

// ----------------------------------------------------------------------------------------------------------------- //

template <typename C>
void Grafo<C>::modificarCostoArco(const int &origen, const int &destino, const C &costo)
// Complejidad temporal: O(log n + log m), siendo n el número de vértices
// Complejidad espacial: O(1).
{
    auto iteradorAeropuertoOrigen = grafo.find(origen);

    if (iteradorAeropuertoOrigen != grafo.end())
    {
        auto iteradorAeropuertoDestino = (iteradorAeropuertoOrigen->second).find(destino);

        if (iteradorAeropuertoDestino != (iteradorAeropuertoOrigen->second).end())
            (iteradorAeropuertoDestino->second) = costo;
    }
}

// ----------------------------------------------------------------------------------------------------------------- //

template <typename C>
void Grafo<C>::agregarArco(const int &origen, const int &destino, const C &costo)
// Complejidad temporal: O(log n + log m), siendo n el número de vértices del grafo y m, el número de vértices
// adyacentes al vértice de origen.
// Complejidad espacial: O(1).
{
    grafo[origen].insert({destino, costo});
}

// ----------------------------------------------------------------------------------------------------------------- //

template <typename C>
void Grafo<C>::eliminarArco(const int &origen, const int &destino)
// Complejidad temporal: O(log n + log m), siendo n el número de vértices del grafo y m, el número de adyacentes al
// vértice de origen.
// Complejidad espacial: O(1).
{
    auto iteradorVerticeOrigen = grafo.find(origen); // O(log n)

    if (iteradorVerticeOrigen != grafo.end())
    {
        auto iteradorVerticeDestino = (iteradorVerticeOrigen->second).find(destino); // O(log m)

        if (iteradorVerticeDestino != (iteradorVerticeOrigen->second).end())
            (iteradorVerticeOrigen->second).erase(iteradorVerticeDestino); // O(log m)
    }
}

// ----------------------------------------------------------------------------------------------------------------- //

template <typename C>
void Grafo<C>::vaciar()
// Complejidad temporal: O(n + m * C), donde n es el número de vértices; m, el número de arcos; y C, la complejidad
// temporal del destructor de la estructura del costo del arco.
// Complejidad espacial: O(1).
{
    for (auto &arco : grafo) // O(m)
        arco.second.clear();

    grafo.clear(); // O(n)
}

template class Grafo<int>;
template class Grafo<char>;
template class Grafo<std::string>;
template class Grafo<float>;
template class Grafo<double>;
template class Grafo<RedAeroportuaria::Vuelo>;
