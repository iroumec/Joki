#include "Grafo.h"
#include <cassert>

// --------------------------------------------------------------------------------------------- //
/**                                   Implementación - Arco                                      **/
// --------------------------------------------------------------------------------------------- //

template <typename C> Grafo<C>::Arco::Arco()
{
}

// --------------------------------------------------------------------------------------------- //

template <typename C> Grafo<C>::Arco::Arco(int adyacente, const C & costo)
{
    // El atributo vértice se torna el pasado por parámetro.
    vertice = adyacente;

    // El atributo costo toma el valor del costo por parámetro.
    this->costo = costo;
}

// --------------------------------------------------------------------------------------------- //

template <typename C> int Grafo<C>::Arco::devolver_adyacente() const
{
    // Devuelvo el nombre del vértice adyacente.
    return vertice;
}

// --------------------------------------------------------------------------------------------- //

template <typename C> void Grafo<C>::Arco::nuevo_costo(const C & costo)
{
    // El atributo costo toma el valor del parámetro.
    this->costo = costo;
}

// --------------------------------------------------------------------------------------------- //

template <typename C> const C & Grafo<C>::Arco::devolver_costo() const
{
    // Retorno el atributo costo del arco.
    return costo;
}

// --------------------------------------------------------------------------------------------- //
/**                                 Implementación - Grafo                                       **/
// --------------------------------------------------------------------------------------------- //

template <typename C> Grafo<C>::Grafo()
{
}

// --------------------------------------------------------------------------------------------- //

template <typename C> Grafo<C>::Grafo(const Grafo & otroGrafo)
{
    // Copio los valores del grafo actual al que se pasa por parámetro.
    *this = otroGrafo;
}

// --------------------------------------------------------------------------------------------- //

template <typename C> Grafo<C>::~Grafo()
{
    // Vacío el mapa con el que represento al grafo.
    graph.clear();
}

// --------------------------------------------------------------------------------------------- //

template <typename C> Grafo<C> & Grafo<C>::operator = (const Grafo & otroGrafo)
{
    // Obtengo una lista con los vértices del grafo.
    list<int> vertices;
    otroGrafo.devolver_vertices(vertices);
    auto it = vertices.begin();

    list<Arco> adyacentes;

    // Recorro a lista de vértices.
    while (it != vertices.end())
    {
        // Obtengo los adyacentes del vértice.
        otroGrafo.devolver_adyacentes(*it, adyacentes);

        // Inserto los adyacentes a mi grafo, junto con el vértice.
        graph.insert({*it, adyacentes});

        // Vacío la lista para darle lugar a la siguiente iteración.
        adyacentes.clear();

        it++;
    }

    return *this;
}

// --------------------------------------------------------------------------------------------- //

template <typename C> bool Grafo<C>::esta_vacio() const
{
    // Retorno si el mapa está vacío.
    return graph.empty();
}

// --------------------------------------------------------------------------------------------- //

template <typename C> int Grafo<C>::devolver_longitud() const
{
    // Devuelve el número de elementos del mapa.
    return graph.size();
}

// --------------------------------------------------------------------------------------------- //

template <typename C> bool Grafo<C>::existe_vertice(int vertice) const
{
    // Busco el vértice.
    auto it = graph.find(vertice);

    // Si lo encontré (TRUE),  el iterador será diferente al elemento teórico que le sigue al
    // último del mapa.
    return (it != graph.end());
}

// --------------------------------------------------------------------------------------------- //

template <typename C> bool Grafo<C>::existe_arco(int origen, int destino) const
{
    bool found = false;

    // Obtengo un iterador al vértice de origen.
    auto it = graph.find(origen);

    // Si encontré el vértice...
    if (it != graph.end())
    {
        // Obtengo un iterador al comienzo de la lista de arcos.
        auto they = (it->second).begin();

        // Mientras no se acabe la lista y no haya encontrado el arco...
        while ((they != (it->second).end()) && not(found))
        {
            // Si el destino es igual al que busco, lo encontré.
            if (they->devolver_adyacente() == destino)
                found = true;
            they++;
        }
    }

    return found;
}

// --------------------------------------------------------------------------------------------- //

template <typename C> const C & Grafo<C>::costo_arco(int origen, int destino) const
{
    // Obtengo un iterador al vértice de origen.
    std::cout << origen;

    auto it = graph.find(origen);

    // Si no encuentro el vértice de origen...
    assert(it != graph.end());

    // Obtengo un iterador a la lista de adyacentes del vértice
    auto they = (it->second).begin();

    // Itero sobre la lista de arcos, buscando el vértice destino.
    while ((they != (it->second).end()) && they->devolver_adyacente() != destino)
        they++;

    // Si el arco no existe...
    assert(they != (it->second).end());

    // Retorno el costo del arco.
    return they->devolver_costo();
}

// --------------------------------------------------------------------------------------------- //

template <typename C> void Grafo<C>::devolver_vertices(list<int> & vertices) const
{
    // Obtengo un iterador al comienzo del mapa.
    auto it = graph.begin();

    // Lo recorro, agregando los vértices a la lista.
    while (it != graph.end())
    {
        vertices.push_back(it->first);
        it++;
    }
}

// --------------------------------------------------------------------------------------------- //

template <typename C> void Grafo<C>::devolver_adyacentes(int origen, list<Arco> & adyacentes) const
{
    // Obtengo un iterador al vértice
    auto it = graph.find(origen);

    // Si encuentro el vértice de origen...
    if (it != graph.end())
    {
        // Obtengo un iterador a la lista de adyacentes del vértice.
        auto they = (it->second).begin();

        // La recorro y guardo los arcos,
        while (they != (it->second).end())
        {
            adyacentes.push_back(*they);
            they++;
        }
    }
}

// --------------------------------------------------------------------------------------------- //

template <typename C> void Grafo<C>::agregar_vertice(int vertice)
{
    list<Arco> adyacentes;

    // Cargo en el mapa el vértice como clave y la lista de adyacentes vacía como valor.
    graph.insert({vertice, adyacentes});
}

// --------------------------------------------------------------------------------------------- //

template <typename C> void Grafo<C>::eliminar_vertice(int vertice)
{
    auto it = graph.begin();

    // Elimino los arcos que haya de otros vértices al que quiero eliminar.
    while (it != graph.end())
    {
        if ((it->first) != vertice)
            eliminar_arco((it->first), vertice);
        it++;
    }

    // Eliminando el vértice, ya elimino los arcos de él a los demás;
    // pero no los de los demás a él. Por eso el bucle de arriba.
    graph.erase(vertice);
}

// --------------------------------------------------------------------------------------------- //

template <typename C> void Grafo<C>::modificar_costo_arco(int origen, int destino, const C & costo)
{
    // Obtengo un iterador a la posición donde se halle la clave del parámetro origen
    auto it = graph.find(origen); // Si no agregaba un "typename" al principio, no funcionaba

    bool found = false;

    // Si es igual a graph.end(), no encontro la clave
    if (it != graph.end()) // Si la encontro
    {
        // Tomo un iterador al comienzo de la lista
        auto they = (it->second).begin();

        // Recorro la lista hasta encontrar la posición en la que está el adyacentee que busco
        while ((they != (it->second).end()) && (not (found)))
        {
            if (they->devolver_adyacente() == destino)
                found = true;
            else
              they++;
        }

        // Si lo encontré, remplazo el costo
        if (found)
            they->nuevo_costo(costo);
    }
}

// --------------------------------------------------------------------------------------------- //

template <typename C> void Grafo<C>::agregar_arco(int origen, int destino, const C & costo)
{
    // Inicializo un objeto de la clase arco.
    Arco edge(destino, costo);

    // Lo agrego a la lista de arcos del vértice origen.
    graph[origen].push_back(edge);
}

// --------------------------------------------------------------------------------------------- //

template <typename C> void Grafo<C>::eliminar_arco(int origen, int destino)
{
    // Busco un iterador al elemento que quiero eliminar
    auto it = graph.find(origen);

    bool found = false;

    // En caso de haberlo encontrado...
    if (it != graph.end())
    {
        // Obtengo un iterador al comienzo de la lista de arcos...
        auto they = (it->second).begin();

        // Recorro la lista de arcos hasta encontrar el vértice adyacente que busco.
        while ((they != (it->second).end()) && (not (found)))
        {
            // Si es igual, lo encontre.
            if (they->devolver_adyacente() == destino)
                found = true;
            else
              they++;
        }

        // De haberlo hallado, lo elimino.
        if (found)
            (it->second).erase(they);
    }
}

// --------------------------------------------------------------------------------------------- //

template <typename C> void Grafo<C>::vaciar()
{
    // Vacío el mapa con los vértices.
    graph.clear();
}

template class Grafo<string>;
template class Grafo<char>;
template class Grafo<int>;
template class Grafo<float>;
template class Grafo<double>;
template class Grafo<Vuelo>;
