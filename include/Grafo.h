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

template <typename C> class Grafo
{
public:
	class Arco
	{
	public:

		Arco(); // Constructor de la clase.

		Arco(int adyacente, const C & costo); // Destructor de la clase.

		int devolver_adyacente() const; // Devolver el identificador del vértice adyacente.

		const C & devolver_costo() const; // Devolver el costo del arco entre ambos vértices.

		void nuevo_costo(const C & costo); // Modifico el costo del arco.

	private:

		int vertice;

		C costo;

	}; /// class Arco

public:

	Grafo(); // Constructor de la clase.
	Grafo(const Grafo & otroGrafo); // Constructor de copia.

	~Grafo(); // Destructor de la clase.

	Grafo & operator = (const Grafo & otroGrafo); // Operador =.

	bool esta_vacio() const; // Consultar si el grafo está vacío.

	int devolver_longitud() const; // Consultar por la cantidad de vértices del grafo.

	bool existe_vertice(int vertice) const; // Consultar si existe un vértice en el grafo.

	bool existe_arco(int origen, int destino) const; // Consultar si existe un arco entre dos vértices.

	// PRE CONDICION: existe_arco(origen, destino)
	const C & costo_arco(int origen, int destino) const; // Consultar por el costo del arco entre dos vértices.

	void devolver_vertices(list<int> & vertices) const;

	void devolver_adyacentes(int origen, list<Arco> & adyacentes) const;

	void agregar_vertice(int vertice);

	// POST CONDICION: Para todo vértice v != vertice: !existeArco(v, vertice) && !existeArco(vertice, v)
	void eliminar_vertice(int vertice);

	// PRE CONDICION: existeArco(origen, destino)
	void modificar_costo_arco(int origen, int destino, const C & costo);

	// PRE CONDICION: existeVertice(origen) && existeVertice(destino)
	// POST CONDICION: existeArco(origen, destino)
	void agregar_arco(int origen, int destino, const C & costo);

	// POST CONDICION: !existeArco(origen, destino)
	void eliminar_arco(int origen, int destino);

	void vaciar();

private:

    map<int, list<Arco>> graph;

}; // class Grafo

// --------------------------------------------------------------------------------------------- //
///                                   Funciones Libres                                          ///
// --------------------------------------------------------------------------------------------- //

// --------------------------------------------------------------------------------------------- //
/*                                      Operador <<                                              */
// --------------------------------------------------------------------------------------------- //

template <typename C>
ostream & operator << (ostream & salida, const Grafo<C> & grafo)
{
	// Recorremos todos los vertices
	list<int> vertices;
	grafo.devolver_vertices(vertices);
	typename list<int>::iterator v = vertices.begin();
	while (v != vertices.end()) {
		salida << "    " << *v << "\n";
		// Recorremos todos los adyacentes de cada vertice
		list<typename Grafo<C>::Arco> adyacentes;
		grafo.devolver_adyacentes(*v, adyacentes);
		typename list<typename Grafo<C>::Arco>::iterator ady = adyacentes.begin();
		while (ady != adyacentes.end()) {
			salida << "    " << *v << "-> " << ady->devolver_adyacente() << " (" << ady->devolver_costo() << ")\n";
			ady++;
		}
		v++;
	}
	return salida;
}

#endif /* GRAFO_H_INCLUDED */
