#include "Grafo.h"
#include "RedAeroportuaria.h"

// ----------------------------------------------------------------------------------------------------------------- //
//                                                  Subclase Arco                                                    //
// ----------------------------------------------------------------------------------------------------------------- //

template <typename C>
Grafo<C>::Arco::Arco() // O(1)
{
}

// ----------------------------------------------------------------------------------------------------------------- //

template <typename C>
Grafo<C>::Arco::Arco(const int &adyacente, const C &costo) // O(1)
{
    vertice = adyacente;

    this->costo = costo;
}

// ----------------------------------------------------------------------------------------------------------------- //

template <typename C>
int Grafo<C>::Arco::devolverAdyacente() const // O(1)
{
    return vertice;
}

// ----------------------------------------------------------------------------------------------------------------- //

template <typename C>
void Grafo<C>::Arco::nuevoCosto(const C &costo) // O(1)
{
    this->costo = costo;
}

// ----------------------------------------------------------------------------------------------------------------- //

template <typename C>
const C &Grafo<C>::Arco::devolverCosto() const // O(1)
{
    return costo;
}

// ----------------------------------------------------------------------------------------------------------------- //
//                                                  Clase Grafo                                                      //
// ----------------------------------------------------------------------------------------------------------------- //

template <typename C>
Grafo<C>::Grafo() // O(1)
{
}

// ----------------------------------------------------------------------------------------------------------------- //

template <typename C>
Grafo<C>::Grafo(const Grafo &otroGrafo) // O(1)
{
    *this = otroGrafo;
}

// ----------------------------------------------------------------------------------------------------------------- //

template <typename C>
Grafo<C>::~Grafo() // O()
{
    for (auto &arco : grafo)
        arco.second.clear();

    grafo.clear();
}

// ----------------------------------------------------------------------------------------------------------------- //

template <typename C>
Grafo<C> &Grafo<C>::operator=(const Grafo &otroGrafo) // O(N^2 + NM + NK)
{
    grafo.clear();

    std::list<int> vertices;
    otroGrafo.devolverVertices(vertices);

    std::list<Arco> arcos;

    for (const auto &vertice : vertices)
    {
        agregarVertice(vertice);

        otroGrafo.devolverAdyacentes(vertice, arcos);

        for (const auto &arco : arcos)
            grafo[vertice].insert({arco.devolverAdyacente(), arco.devolverCosto()});

        arcos.clear();
    }

    return *this;
}

// ----------------------------------------------------------------------------------------------------------------- //

template <typename C>
bool Grafo<C>::estaVacio() const // O(1)
{
    return grafo.empty();
}

// ----------------------------------------------------------------------------------------------------------------- //

template <typename C>
int Grafo<C>::devolverLongitud() const // O(1)
{
    return grafo.size();
}

// ----------------------------------------------------------------------------------------------------------------- //

template <typename C>
bool Grafo<C>::existeVertice(const int &vertice) const // O(log n), siendo n el número de vértices
{
    auto it = grafo.find(vertice);

    return (it != grafo.end());
}

// ----------------------------------------------------------------------------------------------------------------- //

template <typename C>
bool Grafo<C>::existeArco(const int &origen, const int &destino) const // O(log n), siendo n el número de vértices
{
    bool found = false;

    auto it = grafo.find(origen);

    if (it != grafo.end())
    {
        auto they = (it->second).find(destino);

        if (they != (it->second).end())
            found = true;
    }

    return found;
}

// ----------------------------------------------------------------------------------------------------------------- //

template <typename C>
const C &Grafo<C>::costoArco(const int &origen, const int &destino) const
// O(log n), siendo n el número de vértices.
// Si bien se itera sobre sus adyacentes, el número de vértices totales siempre será mayor al número de adyacentes.
{
    // Busco el vértice de origen.
    auto it_origen = grafo.find(origen);

    if (it_origen == grafo.end())
        throw std::invalid_argument("No existe el vértice de origen");

    // Busco el vértice adyacente.
    auto it_adyacente = (it_origen->second).find(destino);

    if (it_adyacente == (it_origen->second).end())
        throw std::invalid_argument("No existe un arco entre el vértice de origen y el de destino");

    return (it_adyacente->second);
}

// ----------------------------------------------------------------------------------------------------------------- //

template <typename C>
void Grafo<C>::devolverVertices(std::list<int> &vertices) const // O(n), siendo n el número de vértices
{
    for (const auto &vertice : grafo)
        vertices.push_back(vertice.first);
}

// ----------------------------------------------------------------------------------------------------------------- //

template <typename C>
void Grafo<C>::devolverAdyacentes(const int &origen, std::list<Arco> &adyacentes) const
// O(max{log n, m}), siendo n el número de vértices y m, la cantidad de adyacentes del vértice origen.
{
    auto it = grafo.find(origen);

    if (it != grafo.end())
    {
        for (const auto &adyacente : (it->second))
        {
            Arco nuevo_arco(adyacente.first, adyacente.second);
            adyacentes.push_back(nuevo_arco);
        }
    }
}

// ----------------------------------------------------------------------------------------------------------------- //

template <typename C>
void Grafo<C>::agregarVertice(const int &vertice) // O(log n), siendo n el número de vértices
{
    std::map<int, C> arcos;

    grafo.insert({vertice, arcos});
}

// ----------------------------------------------------------------------------------------------------------------- //

template <typename C>
void Grafo<C>::eliminarVertice(const int &verticeAEliminar) // O(n log n), siendo n el número de vértices
{
    // Elimino los arcos que haya de otros vértices al que quiero eliminar.
    for (const auto &vertice : grafo)
    {
        if (vertice.first != verticeAEliminar)
            eliminarArco(vertice.first, verticeAEliminar);
    }

    grafo.erase(verticeAEliminar);
}

// ----------------------------------------------------------------------------------------------------------------- //

template <typename C>
void Grafo<C>::modificarCostoArco(const int &origen, const int &destino, const C &costo)
// O(log n), siendo n el número de vértices
{
    auto it = grafo.find(origen);

    if (it != grafo.end())
    {
        auto they = (it->second).find(destino);

        if (they != (it->second).end())
            (they->second) = costo;
    }
}

// ----------------------------------------------------------------------------------------------------------------- //

template <typename C>
void Grafo<C>::agregarArco(const int &origen, const int &destino, const C &costo)
// O(max{log n, log m}), siendo n el número de vértices y m, el número de adyacentes del origen
{
    grafo[origen].insert({destino, costo});
}

// ----------------------------------------------------------------------------------------------------------------- //

template <typename C>
void Grafo<C>::eliminarArco(const int &origen, const int &destino) // O(log n), siendo n el número de vértices
{
    auto it = grafo.find(origen);

    if (it != grafo.end())
    {
        auto they = (it->second).find(destino);

        if (they != (it->second).end())
            (it->second).erase(they);
    }
}

// ----------------------------------------------------------------------------------------------------------------- //

template <typename C>
void Grafo<C>::vaciar() //
{
    grafo.clear();
}

template class Grafo<int>;
template class Grafo<char>;
template class Grafo<std::string>;
template class Grafo<float>;
template class Grafo<double>;
template class Grafo<RedAeroportuaria::Vuelo>;
