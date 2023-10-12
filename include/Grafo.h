// --------------------------------------------------------------------------------------------- //
/**                                 Directivas del Procesador                                    **/
// --------------------------------------------------------------------------------------------- //

#ifndef GRAFO_H_INCLUDED
#define GRAFO_H_INCLUDED

#include <iostream>
#include <list>
#include <map>
#include "Vuelo.h"

#include <cassert>

using namespace std;

// --------------------------------------------------------------------------------------------- //
/**                                 Declaración de la Clase                                      **/
// --------------------------------------------------------------------------------------------- //

template <typename C>
class Grafo
{
public:
	class Arco
	{
	public:
		Arco(); // Constructor de la clase.

		Arco(const int & adyacente, const C &costo); // Destructor de la clase.

		int devolverAdyacente() const; // Devolver el identificador del vértice adyacente.

		const C &devolverCosto() const; // Devolver el costo del arco entre ambos vértices.

		void nuevoCosto(const C &costo); // Modifico el costo del arco.

	private:
		int vertice;

		C costo;

	}; /// class Arco

public:
	Grafo();					   // Constructor de la clase.
	Grafo(const Grafo &otroGrafo); // Constructor de copia.

	~Grafo(); // Destructor de la clase.

	Grafo &operator=(const Grafo &otroGrafo); // Operador =.

	bool estaVacio() const; // Consultar si el grafo está vacío.

	int devolverLongitud() const; // Consultar por la cantidad de vértices del grafo.

	bool existeVertice(const int &vertice) const; // Consultar si existe un vértice en el grafo.

	bool existeArco(const int &origen, const int &destino) const; // Consultar si existe un arco entre dos vértices.

	// PRE CONDICION: existeArco(origen, destino)
	const C &costoArco(const int &origen, const int &destino) const; // Consultar por el costo del arco entre dos vértices.

	void devolverVertices(list<int> &vertices) const;

	void devolverAdyacentes(const int &origen, list<Arco> &adyacentes) const;

	void agregarVertice(const int &vertice);

	// POST CONDICION: Para todo vértice v != vertice: !existeArco(v, vertice) && !existeArco(vertice, v)
	void eliminarVertice(const int &vertice);

	// PRE CONDICION: existeArco(origen, destino)
	void modificarCostoArco(const int &origen, const int &destino, const C &costo);

	// PRE CONDICION: existeVertice(origen) && existeVertice(destino)
	// POST CONDICION: existeArco(origen, destino)
	void agregarArco(const int &origen, const int &destino, const C &costo);

	// POST CONDICION: !existeArco(origen, destino)
	void eliminarArco(const int &origen, const int &destino);

	void vaciar();

private:
	map<int, map<int, C>> graph;

}; // class Grafo

// --------------------------------------------------------------------------------------------- //
///                                   Funciones Libres                                          ///
// --------------------------------------------------------------------------------------------- //

// --------------------------------------------------------------------------------------------- //
/*                                      Operador <<                                              */
// --------------------------------------------------------------------------------------------- //

template <typename C>
ostream &operator<<(ostream &salida, const Grafo<C> &grafo)
{
	// Recorremos todos los vertices
	list<int> vertices;
	grafo.devolverVertices(vertices);
	typename list<int>::iterator v = vertices.begin();
	while (v != vertices.end())
	{
		salida << "    " << *v << "\n";
		// Recorremos todos los adyacentes de cada vertice
		list<typename Grafo<C>::Arco> adyacentes;
		grafo.devolverAdyacentes(*v, adyacentes);
		typename list<typename Grafo<C>::Arco>::iterator ady = adyacentes.begin();
		while (ady != adyacentes.end())
		{
			salida << "    " << *v << "-> " << ady->devolverAdyacente() << "\n";
			ady++;
		}
		v++;
	}
	return salida;
}

#endif /* GRAFO_H_INCLUDED */
