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
Grafo<C>::Arco::Arco(string adyacente, const C &costo)
{
    // El atributo v�rtice se torna el pasado por par�metro.
    vertice = adyacente;

    // El atributo costo toma el valor del costo por par�metro.
    this->costo = costo;
}

// --------------------------------------------------------------------------------------------- //

template <typename C>
string Grafo<C>::Arco::devolver_adyacente() const
{
    // Devuelvo el nombre del v�rtice adyacente.
    return vertice;
}

// --------------------------------------------------------------------------------------------- //

template <typename C>
void Grafo<C>::Arco::nuevo_costo(const C &costo)
{
    // El atributo costo toma el valor del par�metro.
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
    // Vac�o el mapa con el que represento al grafo.
    graph.clear();
}

// --------------------------------------------------------------------------------------------- //

template <typename C>
Grafo<C> &Grafo<C>::operator=(const Grafo &otroGrafo)
{
    // Obtengo una lista con los v�rtices del grafo.
    list<string> vertices;
    otroGrafo.devolver_vertices(vertices);
    auto it = vertices.begin();

    list<Arco> adyacentes;

    // Recorro a lista de v�rtices.
    while (it != vertices.end())
    {
        // Obtengo los adyacentes del v�rtice.
        otroGrafo.devolver_adyacentes(*it, adyacentes);

        // Inserto los adyacentes a mi grafo, junto con el v�rtice.
        graph.insert({*it, adyacentes});

        // Vac�o la lista para darle lugar a la siguiente iteraci�n.
        adyacentes.clear();

        it++;
    }

    return *this;
}

// --------------------------------------------------------------------------------------------- //

template <typename C>
bool Grafo<C>::esta_vacio() const
{
    // Retorno si el mapa est� vac�o.
    return graph.empty();
}

// --------------------------------------------------------------------------------------------- //

template <typename C>
int Grafo<C>::devolver_longitud() const
{
    // Devuelve el n�mero de elementos del mapa.
    return graph.size();
}

// --------------------------------------------------------------------------------------------- //

template <typename C>
bool Grafo<C>::existe_vertice(string vertice) const
{
    // Busco el v�rtice.
    auto it = graph.find(vertice);

    // Si lo encontr� (TRUE),  el iterador ser� diferente al elemento te�rico que le sigue al
    // �ltimo del mapa.
    return (it != graph.end());
}

// --------------------------------------------------------------------------------------------- //

template <typename C>
bool Grafo<C>::existe_arco(string origen, string destino) const
{
    bool found = false;

    // Obtengo un iterador al v�rtice de origen.
    auto it = graph.find(origen);

    // Si encontr� el v�rtice...
    if (it != graph.end())
    {
        // Obtengo un iterador al comienzo de la lista de arcos.
        auto they = (it->second).begin();

        // Mientras no se acabe la lista y no haya encontrado el arco...
        while ((they != (it->second).end()) && not(found))
        {
            // Si el destino es igual al que busco, lo encontr�.
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
    // Obtengo un iterador al v�rtice de origen.
    auto it = graph.find(origen);

    // Si no encuentro el v�rtice de origen...
    assert(it != graph.end());

    // Obtengo un iterador a la lista de adyacentes del v�rtice
    auto they = (it->second).begin();

    // Itero sobre la lista de arcos, buscando el v�rtice destino.
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

    // Lo recorro, agregando los v�rtices a la lista.
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
    // Obtengo un iterador al v�rtice
    auto it = graph.find(origen);

    // Si encuentro el v�rtice de origen...
    if (it != graph.end())
    {
        // Obtengo un iterador a la lista de adyacentes del v�rtice.
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

    // Cargo en el mapa el v�rtice como clave y la lista de adyacentes vac�a como valor.
    graph.insert({vertice, adyacentes});
}

// --------------------------------------------------------------------------------------------- //

template <typename C>
void Grafo<C>::eliminar_vertice(string vertice)
{
    auto it = graph.begin();

    // Elimino los arcos que haya de otros v�rtices al que quiero eliminar.
    while (it != graph.end())
    {
        if ((it->first) != vertice)
            eliminar_arco((it->first), vertice);
        it++;
    }

    // Eliminando el v�rtice, ya elimino los arcos de �l a los dem�s;
    // pero no los de los dem�s a �l. Por eso el bucle de arriba.
    graph.erase(vertice);
}

// --------------------------------------------------------------------------------------------- //

template <typename C>
void Grafo<C>::modificar_costo_arco(string origen, string destino, const C &costo)
{
    // Obtengo un iterador a la posici�n donde se halle la clave del par�metro origen
    auto it = graph.find(origen);

    bool found = false;

    // Si es igual a graph.end(), no encontro la clave
    if (it != graph.end()) // Si la encontro
    {
        // Tomo un iterador al comienzo de la lista
        auto they = (it->second).begin();

        // Recorro la lista hasta encontrar la posici�n en la que est� el adyacentee que busco
        while ((they != (it->second).end()) && (not(found)))
        {
            if (they->devolver_adyacente() == destino)
                found = true;
            else
                they++;
        }

        // Si lo encontr�, remplazo el costo
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

    // Lo agrego a la lista de arcos del v�rtice origen.
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

        // Recorro la lista de arcos hasta encontrar el v�rtice adyacente que busco.
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
    // Vac�o el mapa con los v�rtices.
    graph.clear();
}

template class Grafo<string>;
template class Grafo<char>;
template class Grafo<int>;
template class Grafo<float>;
template class Grafo<double>;
template class Grafo<Vuelo>;
