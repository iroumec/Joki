#include "Grafo.h"
#include <cassert>

// --------------------------------------------------------------------------------------------- //
/**                                   ImplementaciÛn - Arco                                      **/
// --------------------------------------------------------------------------------------------- //

template <typename C>
Grafo<C>::Arco::Arco()
{
}

// --------------------------------------------------------------------------------------------- //

template <typename C>
Grafo<C>::Arco::Arco(string adyacente, const C &costo)
{
    // El atributo vÈrtice se torna el pasado por par·metro.
    vertice = adyacente;

    // El atributo costo toma el valor del costo por par·metro.
    this->costo = costo;
}

// --------------------------------------------------------------------------------------------- //

template <typename C>
string Grafo<C>::Arco::devolver_adyacente() const
{
    // Devuelvo el nombre del vÈrtice adyacente.
    return vertice;
}

// --------------------------------------------------------------------------------------------- //

template <typename C>
void Grafo<C>::Arco::nuevo_costo(const C &costo)
{
    // El atributo costo toma el valor del par·metro.
    this->costo = costo;
}

// --------------------------------------------------------------------------------------------- //

template <typename C>
const C &Grafo<C>::Arco::devolver_costo() const
{
    // Retorno el atributo costo del arco.
    return costo;
}

// --------------------------------------------------------------------------------------------- //
/**                                 ImplementaciÛn - Grafo                                       **/
// --------------------------------------------------------------------------------------------- //

template <typename C>
Grafo<C>::Grafo()
{
}

// --------------------------------------------------------------------------------------------- //

template <typename C>
Grafo<C>::Grafo(const Grafo &otroGrafo)
{
    // Copio los valores del grafo actual al que se pasa por par·metro.
    *this = otroGrafo;
}

// --------------------------------------------------------------------------------------------- //

template <typename C>
Grafo<C>::~Grafo()
{
    // VacÌo el mapa con el que represento al grafo.
    graph.clear();
}

// --------------------------------------------------------------------------------------------- //

template <typename C>
Grafo<C> &Grafo<C>::operator=(const Grafo &otroGrafo)
{
    // Obtengo una lista con los vÈrtices del grafo.
    list<string> vertices;
    otroGrafo.devolver_vertices(vertices);
    auto it = vertices.begin();

    list<Arco> adyacentes;

    // Recorro a lista de vÈrtices.
    while (it != vertices.end())
    {
        // Obtengo los adyacentes del vÈrtice.
        otroGrafo.devolver_adyacentes(*it, adyacentes);

        // Inserto los adyacentes a mi grafo, junto con el vÈrtice.
        graph.insert({*it, adyacentes});

        // VacÌo la lista para darle lugar a la siguiente iteraciÛn.
        adyacentes.clear();

        it++;
    }

    return *this;
}

// --------------------------------------------------------------------------------------------- //

template <typename C>
bool Grafo<C>::esta_vacio() const
{
    // Retorno si el mapa est· vacÌo.
    return graph.empty();
}

// --------------------------------------------------------------------------------------------- //

template <typename C>
int Grafo<C>::devolver_longitud() const
{
    // Devuelve el nùmero de elementos del mapa.
    return graph.size();
}

// --------------------------------------------------------------------------------------------- //

template <typename C>
bool Grafo<C>::existe_vertice(string vertice) const
{
    // Busco el vùrtice.
    auto it = graph.find(vertice);

    // Si lo encontrù (TRUE),  el iterador serù diferente al elemento teùrico que le sigue al
    // ùltimo del mapa.
    return (it != graph.end());
}

// --------------------------------------------------------------------------------------------- //

template <typename C>
bool Grafo<C>::existe_arco(string origen, string destino) const
{
    bool found = false;

    // Obtengo un iterador al vùrtice de origen.
    auto it = graph.find(origen);

    // Si encontrù el vùrtice...
    if (it != graph.end())
    {
        // Obtengo un iterador al comienzo de la lista de arcos.
        auto they = (it->second).begin();

        // Mientras no se acabe la lista y no haya encontrado el arco...
        while ((they != (it->second).end()) && not(found))
        {
            // Si el destino es igual al que busco, lo encontrù.
            if (they->devolver_adyacente() == destino)
                found = true;
            they++;
        }
    }

    return found;
}

// --------------------------------------------------------------------------------------------- //

template <typename C>
const C &Grafo<C>::costo_arco(string origen, string destino) const
{
    // Obtengo un iterador al vùrtice de origen.
    auto it = graph.find(origen);

    // Si no encuentro el vùrtice de origen...
    assert(it != graph.end());

    // Obtengo un iterador a la lista de adyacentes del vùrtice
    auto they = (it->second).begin();

    // Itero sobre la lista de arcos, buscando el vùrtice destino.
    while ((they != (it->second).end()) && they->devolver_adyacente() != destino)
        they++;

    // Si el arco no existe...
    assert(they != (it->second).end());

    // Retorno el costo del arco.
    return they->devolver_costo();
}

// --------------------------------------------------------------------------------------------- //

template <typename C>
void Grafo<C>::devolver_vertices(list<string> &vertices) const
{
    // Obtengo un iterador al comienzo del mapa.
    auto it = graph.begin();

    // Lo recorro, agregando los vùrtices a la lista.
    while (it != graph.end())
    {
        vertices.push_back(it->first);
        it++;
    }
}

// --------------------------------------------------------------------------------------------- //

template <typename C>
void Grafo<C>::devolver_adyacentes(string origen, list<Arco> &adyacentes) const
{
    // Obtengo un iterador al vùrtice
    auto it = graph.find(origen);

    // Si encuentro el vùrtice de origen...
    if (it != graph.end())
    {
        // Obtengo un iterador a la lista de adyacentes del vùrtice.
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

template <typename C>
void Grafo<C>::agregar_vertice(string vertice)
{
    list<Arco> adyacentes;

    // Cargo en el mapa el vùrtice como clave y la lista de adyacentes vacùa como valor.
    graph.insert({vertice, adyacentes});
}

// --------------------------------------------------------------------------------------------- //

template <typename C>
void Grafo<C>::eliminar_vertice(string vertice)
{
    auto it = graph.begin();

    // Elimino los arcos que haya de otros vùrtices al que quiero eliminar.
    while (it != graph.end())
    {
        if ((it->first) != vertice)
            eliminar_arco((it->first), vertice);
        it++;
    }

    // Eliminando el vùrtice, ya elimino los arcos de ùl a los demùs;
    // pero no los de los demùs a ùl. Por eso el bucle de arriba.
    graph.erase(vertice);
}

// --------------------------------------------------------------------------------------------- //

template <typename C>
void Grafo<C>::modificar_costo_arco(string origen, string destino, const C &costo)
{
    // Obtengo un iterador a la posiciÛn donde se halle la clave del par·metro origen
    auto it = graph.find(origen);

    bool found = false;

    // Si es igual a graph.end(), no encontro la clave
    if (it != graph.end()) // Si la encontro
    {
        // Tomo un iterador al comienzo de la lista
        auto they = (it->second).begin();

        // Recorro la lista hasta encontrar la posiciÛn en la que est· el adyacentee que busco
        while ((they != (it->second).end()) && (not(found)))
        {
            if (they->devolver_adyacente() == destino)
                found = true;
            else
                they++;
        }

        // Si lo encontrÈ, remplazo el costo
        if (found)
            they->nuevo_costo(costo);
    }
}

// --------------------------------------------------------------------------------------------- //

template <typename C>
void Grafo<C>::agregar_arco(string origen, string destino, const C &costo)
{
    // Inicializo un objeto de la clase arco.
    Arco edge(destino, costo);

    // Lo agrego a la lista de arcos del vùrtice origen.
    graph[origen].push_back(edge);
}

// --------------------------------------------------------------------------------------------- //

template <typename C>
void Grafo<C>::eliminar_arco(string origen, string destino)
{
    // Busco un iterador al elemento que quiero eliminar
    auto it = graph.find(origen);

    bool found = false;

    // En caso de haberlo encontrado...
    if (it != graph.end())
    {
        // Obtengo un iterador al comienzo de la lista de arcos...
        auto they = (it->second).begin();

        // Recorro la lista de arcos hasta encontrar el vùrtice adyacente que busco.
        while ((they != (it->second).end()) && (not(found)))
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

template <typename C>
void Grafo<C>::vaciar()
{
    // Vacùo el mapa con los vùrtices.
    graph.clear();
}

template class Grafo<string>;
template class Grafo<char>;
template class Grafo<int>;
template class Grafo<float>;
template class Grafo<double>;
template class Grafo<Vuelo>;
