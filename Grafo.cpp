#include "Grafo.h"
#include "RedAeroportuaria.h"

// ----------------------------------------------------------------------------------------------------------------- //
//                                                  Subclase Arco                                                    //
// ----------------------------------------------------------------------------------------------------------------- //

template <typename C>
Grafo<C>::Arco::Arco()
// Complejidad temporal: O(1).
{
}

// ----------------------------------------------------------------------------------------------------------------- //

template <typename C>
Grafo<C>::Arco::Arco(const int &adyacente, const C &costo)
// Complejidad temporal: O(C), siendo C la complejidad temporal del operador = del costo del arco.
{
    vertice = adyacente;

    this->costo = costo;
}

// ----------------------------------------------------------------------------------------------------------------- //

template <typename C>
int Grafo<C>::Arco::devolverAdyacente() const
// Complejidad temporal: O(1).
{
    return vertice;
}

// ----------------------------------------------------------------------------------------------------------------- //

template <typename C>
void Grafo<C>::Arco::nuevoCosto(const C &costo)
// Complejidad temporal: O(C), siendo C la complejidad temporal del operador = del costo del arco.
{
    this->costo = costo;
}

// ----------------------------------------------------------------------------------------------------------------- //

template <typename C>
const C &Grafo<C>::Arco::devolverCosto() const
// Complejidad temporal: O(1).
{
    return costo;
}

// ----------------------------------------------------------------------------------------------------------------- //
//                                                  Clase Grafo                                                      //
// ----------------------------------------------------------------------------------------------------------------- //

template <typename C>
Grafo<C>::Grafo()
// Complejidad temporal: O(1).
{
}

// ----------------------------------------------------------------------------------------------------------------- //

template <typename C>
Grafo<C>::Grafo(const Grafo &otroGrafo)
// Complejidad temporal: O(n^2 * log n), siendo n el número de vértices en el grafo. Esto debido a que utiliza el operador =.
{
    *this = otroGrafo;
}

// ----------------------------------------------------------------------------------------------------------------- //

template <typename C>
Grafo<C>::~Grafo()
// Complejidad temporal: O(n^2 * C), donde n es el número de vértices y C, la complejidad temporal del destructor
// de la estructura del costo del arco. Esto debido a que invoca al método ".vaciar()".
{
    vaciar();
}

// ----------------------------------------------------------------------------------------------------------------- //

template <typename C>
Grafo<C> &Grafo<C>::operator=(const Grafo &otroGrafo)
// Complejidad temporal: O(n^2 * log n), donde n es el número de vértices en el grafo.
// En el peor de los casos, el número de adyacentes de un vértice será tan grande como el número de vértices.
{
    grafo.clear();

    std::list<int> vertices;
    otroGrafo.devolverVertices(vertices); // O(n)

    std::list<Arco> arcos;

    for (const auto &vertice : vertices) // O(n)
    {
        agregarVertice(vertice); // O(log n)

        otroGrafo.devolverAdyacentes(vertice, arcos); // O(m), siendo m la cantidad de adyacentes al vértice.

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
{
    return grafo.empty();
}

// ----------------------------------------------------------------------------------------------------------------- //

template <typename C>
int Grafo<C>::devolverLongitud() const
// Complejidad temporal: O(1).
{
    return grafo.size();
}

// ----------------------------------------------------------------------------------------------------------------- //

template <typename C>
bool Grafo<C>::existeVertice(const int &vertice) const
// Complejidad temporal: O(log n), siendo n el número de vértices en el grafo.
{
    auto iteradorVertice = grafo.find(vertice); // O(log n)

    return (iteradorVertice != grafo.end());
}

// ----------------------------------------------------------------------------------------------------------------- //

template <typename C>
bool Grafo<C>::existeArco(const int &origen, const int &destino) const
// Complejidad temporal: O(log n), siendo n el número de vértices.
// En el peor de los casos, el número de adyacentes de un vértice será tan grande como el número de vértices.
{
    bool hallado = false;

    // Busco el vértice de origen
    auto iteradorVerticeOrigen = grafo.find(origen);

    if (iteradorVerticeOrigen != grafo.end())
    {
        // Busco el vértice de destino dentro de los adyacentes al de origen
        auto iteradorVerticeDestino = (iteradorVerticeOrigen->second).find(destino);
        // O(log m), siendo m el número de adyacentes al vértice de origen.

        if (iteradorVerticeDestino != (iteradorVerticeOrigen->second).end())
            hallado = true;
    }

    return hallado;
}

// ----------------------------------------------------------------------------------------------------------------- //

template <typename C>
const C &Grafo<C>::costoArco(const int &origen, const int &destino) const
// Complejidad temporal: O(log n), siendo n el número de vértices.
// En el peor de los casos, el número de adyacentes de un vértice será tan grande como el número de vértices.
{
    // Busco el vértice de origen
    auto iteradorVerticeOrigen = grafo.find(origen); // O(log n)

    if (iteradorVerticeOrigen == grafo.end())
        throw std::invalid_argument("No existe el vértice de origen");

    // Busco el vértice destino dentro de los adyacentes al de origen
    auto iteradorVerticeDestino = (iteradorVerticeOrigen->second).find(destino);
    // O(log m), siendo m el número de adyacentes al vértice de origen.

    if (iteradorVerticeDestino == (iteradorVerticeOrigen->second).end())
        throw std::invalid_argument("No existe un arco entre el vértice de origen y el de destino");

    return (iteradorVerticeDestino->second);
}

// ----------------------------------------------------------------------------------------------------------------- //

template <typename C>
void Grafo<C>::devolverVertices(std::list<int> &vertices) const
// Complejidad temporal: O(n), siendo n el número de vértices en el grafo.
{
    for (const auto &vertice : grafo)
        vertices.push_back(vertice.first);
}

// ----------------------------------------------------------------------------------------------------------------- //

template <typename C>
void Grafo<C>::devolverAdyacentes(const int &origen, std::list<Arco> &adyacentes) const
// Complejidad temporal: O(n), siendo n el número de vértices.
// En el peor de los casos, el número de adyacentes de un vértice será tan grande como el número de vértices.
{
    auto iteradorAeropuertoOrigen = grafo.find(origen); // O(log n)

    if (iteradorAeropuertoOrigen != grafo.end())
    {
        for (const auto &adyacente : (iteradorAeropuertoOrigen->second))
        // O(m), siendo m la cantidad de adyacentes al vértice de origen.
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
{
    std::map<int, C> arcos;

    grafo.insert({vertice, arcos}); // O(log n)
}

// ----------------------------------------------------------------------------------------------------------------- //

template <typename C>
void Grafo<C>::eliminarVertice(const int &verticeAEliminar)
// Complejidad temporal: O(n log n), siendo n el número de vértices.
// En el peor de los casos, el número de adyacentes de un vértice será tan grande como el número de vértices.
{
    // Elimino los arcos que haya de otros vértices al que quiero eliminar.
    for (const auto &vertice : grafo) // O(n)
    {
        if (vertice.first != verticeAEliminar)
            eliminarArco(vertice.first, verticeAEliminar); // O(log n)
    }

    grafo.erase(verticeAEliminar); // O(log n)
}

// ----------------------------------------------------------------------------------------------------------------- //

template <typename C>
void Grafo<C>::modificarCostoArco(const int &origen, const int &destino, const C &costo)
// Complejidad temporal: O(max{log n, C}), siendo n el número de vértices del grafo y C, el costo del operador igual
// de la estructura que se almacene como costo en el grafo.
// En el peor de los casos, el número de adyacentes de un vértice será tan grande como el número de vértices.
{
    auto iteradorAeropuertoOrigen = grafo.find(origen); // O(log n)

    if (iteradorAeropuertoOrigen != grafo.end())
    {
        auto iteradorAeropuertoDestino = (iteradorAeropuertoOrigen->second).find(destino);
        // O(log m), siendo m el número de adyacentes al aeropuerto de origen.

        if (iteradorAeropuertoDestino != (iteradorAeropuertoOrigen->second).end())
            (iteradorAeropuertoDestino->second) = costo;
    }
}

// ----------------------------------------------------------------------------------------------------------------- //

template <typename C>
void Grafo<C>::agregarArco(const int &origen, const int &destino, const C &costo)
// Complejidad temporal: O(log n), siendo n el número de vértices del grafo. En el peor de los casos, el número
// de adyacentes de un vértice será tan grande como el número de vértices.
{
    grafo[origen].insert({destino, costo});
}

// ----------------------------------------------------------------------------------------------------------------- //

template <typename C>
void Grafo<C>::eliminarArco(const int &origen, const int &destino)
// Complejidad temporal: O(log n), siendo n el número de vértices del grafo. En el peor de los casos, el número
// de adyacentes de un vértice será tan grande como el número de vértices.
{
    auto iteradorVerticeOrigen = grafo.find(origen); // O(log n)

    if (iteradorVerticeOrigen != grafo.end())
    {
        auto iteradorVerticeDestino = (iteradorVerticeOrigen->second).find(destino);
        // O(log m), siendo m el número de adyacentes a origen

        if (iteradorVerticeDestino != (iteradorVerticeOrigen->second).end())
            (iteradorVerticeOrigen->second).erase(iteradorVerticeDestino);
    }
}

// ----------------------------------------------------------------------------------------------------------------- //

template <typename C>
void Grafo<C>::vaciar()
// Complejidad temporal: O(n^2 * C), donde n es el número de vértices; y C, la complejidad temporal del destructor
// de la estructura del costo del arco. Esto debido a que, en el peor de los casos, se tendrán tantos adyacentes como
// vértices.
{
    for (auto &arco : grafo) // O(n), itera sobre cada vértice
        arco.second.clear(); // O(n), ya que en el peor de los casos un vértice tendrá n-1 adyacentes

    grafo.clear(); // O(n)
}

template class Grafo<int>;
template class Grafo<char>;
template class Grafo<std::string>;
template class Grafo<float>;
template class Grafo<double>;
template class Grafo<RedAeroportuaria::Vuelo>;
