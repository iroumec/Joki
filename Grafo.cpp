#include "Grafo.h"
#include <cassert>

// --------------------------------------------------------------------------------------------- //
/**                                   Implementación - Arco                                      **/
// --------------------------------------------------------------------------------------------- //

template <typename C>
Grafo<C>::Arco::Arco()
{
}

// --------------------------------------------------------------------------------------------- //

template <typename C>
Grafo<C>::Arco::Arco(int adyacente, const C &costo)
{
    // El atributo vértice se torna el pasado por parámetro.
    vertice = adyacente;

    // El atributo costo toma el valor del costo por parámetro.
    this->costo = costo;
}

// --------------------------------------------------------------------------------------------- //

template <typename C>
int Grafo<C>::Arco::devolver_adyacente() const
{
    // Devuelvo el nombre del vértice adyacente.
    return vertice;
}

// --------------------------------------------------------------------------------------------- //

template <typename C>
void Grafo<C>::Arco::nuevo_costo(const C &costo)
{
    this->costo = costo;
}

// --------------------------------------------------------------------------------------------- //

template <typename C>
const C &Grafo<C>::Arco::devolver_costo() const
{
    return costo;
}

// --------------------------------------------------------------------------------------------- //
/**                                 Implementación - Grafo                                       **/
// --------------------------------------------------------------------------------------------- //

template <typename C>
Grafo<C>::Grafo()
{
}

// --------------------------------------------------------------------------------------------- //

template <typename C>
Grafo<C>::Grafo(const Grafo &otroGrafo)
{
    // Copio los valores del grafo actual al que se pasa por parámetro.
    *this = otroGrafo;
}

// --------------------------------------------------------------------------------------------- //

template <typename C>
Grafo<C>::~Grafo()
{
    graph.clear();
}

// --------------------------------------------------------------------------------------------- //

template <typename C>
Grafo<C> &Grafo<C>::operator=(const Grafo &otroGrafo)
{
    // Vacío el grafo por si tenía algo antes.
    graph.clear();

    // Obtengo una lista con los vértices del grafo.
    list<int> vertices;
    otroGrafo.devolver_vertices(vertices);

    list<Arco> arcos;

    // Por cada vértice en la lista de vértices...
    for (const auto & vertice : vertices)
    {
        agregar_vertice(vertice);

        // Obtengo los arcos del vértice.
        otroGrafo.devolver_adyacentes(vertice, arcos);

        for (const auto & arco : arcos)
            graph[vertice].insert({arco.devolver_adyacente(), arco.devolver_costo()});

        // Vacío la lista para darle lugar a la siguiente iteración.
        arcos.clear();
    }

    return *this;
}

// --------------------------------------------------------------------------------------------- //

template <typename C>
bool Grafo<C>::esta_vacio() const
{
    return graph.empty();
}

// --------------------------------------------------------------------------------------------- //

template <typename C>
int Grafo<C>::devolver_longitud() const
{
    return graph.size();
}

// --------------------------------------------------------------------------------------------- //

template <typename C>
bool Grafo<C>::existe_vertice(int vertice) const
{
    // Busco el vértice.
    auto it = graph.find(vertice);

    // Retorno si lo encontré.
    return (it != graph.end());
}

// --------------------------------------------------------------------------------------------- //

template <typename C>
bool Grafo<C>::existe_arco(int origen, int destino) const
{
    bool found = false;

    // Busco el vértice de origen.
    auto it = graph.find(origen);

    // Si lo encuentro...
    if (it != graph.end())
    {
        // Busco el vértice de destino...
        auto they = (it->second).find(destino);

        // Si lo encuentro...
        if (they != (it->second).end())
            found = true;
    }

    return found;
}

// --------------------------------------------------------------------------------------------- //

template <typename C>
const C &Grafo<C>::costo_arco(int origen, int destino) const
{
    // Busco el vértice de origen.
    auto it = graph.find(origen);

    // Error si no está el origen.
    assert(it != graph.end());

    // Busco el vértice adyacente.
    auto they = (it->second).find(destino);

    // Error si no está el destino.
    assert(they != (it->second).end());

    // Retorno el costo del arco.
    return (they->second);
}

// --------------------------------------------------------------------------------------------- //

template <typename C>
void Grafo<C>::devolver_vertices(list<int> &vertices) const
{
    // Por cada vértice en el grafo...
    for (const auto & vertice : graph)
        // Agrego el nombre
        vertices.push_back(vertice.first);
}

// --------------------------------------------------------------------------------------------- //

template <typename C>
void Grafo<C>::devolver_adyacentes(int origen, list<Arco> &adyacentes) const
{
    // Obtengo un iterador al vértice
    auto it = graph.find(origen);

    // Si encuentro el vértice de origen...
    if (it != graph.end())
    {
        // Por cada adyacente
        for (const auto & adyacente : (it->second))
        {
            Arco nuevo_arco(adyacente.first, adyacente.second);
            adyacentes.push_back(nuevo_arco);
        }
    }
}

// --------------------------------------------------------------------------------------------- //

template <typename C>
void Grafo<C>::agregar_vertice(int vertice)
{
    map<int, C> arcos;

    // Cargo en el mapa el vértice como clave y la lista de adyacentes vacía como valor.
    graph.insert({vertice, arcos});
}

// --------------------------------------------------------------------------------------------- //

template <typename C>
void Grafo<C>::eliminar_vertice(int vertice_a_eliminar)
{
    // Elimino los arcos que haya de otros vértices al que quiero eliminar.
    for (const auto & vertice : graph)
    {
        if (vertice.first != vertice_a_eliminar)
            eliminar_arco(vertice.first, vertice_a_eliminar);
    }

    // Eliminando el vértice, ya elimino los arcos de él a los demás;
    // pero no los de los demás a él. Por eso el bucle de arriba.
    graph.erase(vertice_a_eliminar);
}

// --------------------------------------------------------------------------------------------- //

template <typename C>
void Grafo<C>::modificar_costo_arco(int origen, int destino, const C &costo)
{
    // Obtengo un iterador a la posición donde se halle la clave del parámetro origen
    auto it = graph.find(origen);

    // Si encontré el vértice origen...
    if (it != graph.end())
    {
        auto they = (it->second).find(destino);

        // Si encontré el destino...
        if (they != (it->second).end())
            (they->second) = costo;
    }
}

// --------------------------------------------------------------------------------------------- //

template <typename C>
void Grafo<C>::agregar_arco(int origen, int destino, const C &costo)
{
    // Agrego el par al mapa correspondiente al vértice de origen
    graph[origen].insert({destino, costo});
}

// --------------------------------------------------------------------------------------------- //

template <typename C>
void Grafo<C>::eliminar_arco(int origen, int destino)
{
    // Busco un iterador al elemento que quiero eliminar
    auto it = graph.find(origen);

    // En caso de haberlo encontrado...
    if (it != graph.end())
    {
        // Busco el vértice adyacente en el mapa
        auto they = (it->second).find(destino);

        // Si lo encontré, lo elimino.
        if (they != (it->second).end())
            (it->second).erase(they);
    }
}

// --------------------------------------------------------------------------------------------- //

template <typename C>
void Grafo<C>::vaciar()
{
    graph.clear();
}

template class Grafo<int>;
template class Grafo<char>;
template class Grafo<string>;
template class Grafo<float>;
template class Grafo<double>;
template class Grafo<Vuelo>;
