#include "Grafo.h"
#include <cassert>

// --------------------------------------------------------------------------------------------- //
/**                                   Implementaci�n - Arco                                      **/
// --------------------------------------------------------------------------------------------- //

template <typename C>
Grafo<C>::Arco::Arco()
{
}

// --------------------------------------------------------------------------------------------- //

template <typename C>
Grafo<C>::Arco::Arco(int adyacente, const C &costo)
{
    // El atributo v�rtice se torna el pasado por par�metro.
    vertice = adyacente;

    // El atributo costo toma el valor del costo por par�metro.
    this->costo = costo;
}

// --------------------------------------------------------------------------------------------- //

template <typename C>
int Grafo<C>::Arco::devolver_adyacente() const
{
    // Devuelvo el nombre del v�rtice adyacente.
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
/**                                 Implementaci�n - Grafo                                       **/
// --------------------------------------------------------------------------------------------- //

template <typename C>
Grafo<C>::Grafo()
{
}

// --------------------------------------------------------------------------------------------- //

template <typename C>
Grafo<C>::Grafo(const Grafo &otroGrafo)
{
    // Copio los valores del grafo actual al que se pasa por par�metro.
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
    // Vac�o el grafo por si ten�a algo antes.
    graph.clear();

    // Obtengo una lista con los v�rtices del grafo.
    list<int> vertices;
    otroGrafo.devolver_vertices(vertices);

    list<Arco> arcos;

    // Por cada v�rtice en la lista de v�rtices...
    for (const auto & vertice : vertices)
    {
        agregar_vertice(vertice);

        // Obtengo los arcos del v�rtice.
        otroGrafo.devolver_adyacentes(vertice, arcos);

        for (const auto & arco : arcos)
            graph[vertice].insert({arco.devolver_adyacente(), arco.devolver_costo()});

        // Vac�o la lista para darle lugar a la siguiente iteraci�n.
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
    // Busco el v�rtice.
    auto it = graph.find(vertice);

    // Retorno si lo encontr�.
    return (it != graph.end());
}

// --------------------------------------------------------------------------------------------- //

template <typename C>
bool Grafo<C>::existe_arco(int origen, int destino) const
{
    bool found = false;

    // Busco el v�rtice de origen.
    auto it = graph.find(origen);

    // Si lo encuentro...
    if (it != graph.end())
    {
        // Busco el v�rtice de destino...
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
    // Busco el v�rtice de origen.
    auto it = graph.find(origen);

    // Error si no est� el origen.
    assert(it != graph.end());

    // Busco el v�rtice adyacente.
    auto they = (it->second).find(destino);

    // Error si no est� el destino.
    assert(they != (it->second).end());

    // Retorno el costo del arco.
    return (they->second);
}

// --------------------------------------------------------------------------------------------- //

template <typename C>
void Grafo<C>::devolver_vertices(list<int> &vertices) const
{
    // Por cada v�rtice en el grafo...
    for (const auto & vertice : graph)
        // Agrego el nombre
        vertices.push_back(vertice.first);
}

// --------------------------------------------------------------------------------------------- //

template <typename C>
void Grafo<C>::devolver_adyacentes(int origen, list<Arco> &adyacentes) const
{
    // Obtengo un iterador al v�rtice
    auto it = graph.find(origen);

    // Si encuentro el v�rtice de origen...
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

    // Cargo en el mapa el v�rtice como clave y la lista de adyacentes vac�a como valor.
    graph.insert({vertice, arcos});
}

// --------------------------------------------------------------------------------------------- //

template <typename C>
void Grafo<C>::eliminar_vertice(int vertice_a_eliminar)
{
    // Elimino los arcos que haya de otros v�rtices al que quiero eliminar.
    for (const auto & vertice : graph)
    {
        if (vertice.first != vertice_a_eliminar)
            eliminar_arco(vertice.first, vertice_a_eliminar);
    }

    // Eliminando el v�rtice, ya elimino los arcos de �l a los dem�s;
    // pero no los de los dem�s a �l. Por eso el bucle de arriba.
    graph.erase(vertice_a_eliminar);
}

// --------------------------------------------------------------------------------------------- //

template <typename C>
void Grafo<C>::modificar_costo_arco(int origen, int destino, const C &costo)
{
    // Obtengo un iterador a la posici�n donde se halle la clave del par�metro origen
    auto it = graph.find(origen);

    // Si encontr� el v�rtice origen...
    if (it != graph.end())
    {
        auto they = (it->second).find(destino);

        // Si encontr� el destino...
        if (they != (it->second).end())
            (they->second) = costo;
    }
}

// --------------------------------------------------------------------------------------------- //

template <typename C>
void Grafo<C>::agregar_arco(int origen, int destino, const C &costo)
{
    // Agrego el par al mapa correspondiente al v�rtice de origen
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
        // Busco el v�rtice adyacente en el mapa
        auto they = (it->second).find(destino);

        // Si lo encontr�, lo elimino.
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
