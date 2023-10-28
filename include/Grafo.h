#ifndef GRAFO_H_INCLUDED
#define GRAFO_H_INCLUDED

#include <iostream>
#include <list>
#include <map>

template <typename C>
class Grafo
{
public:
	class Arco
	{
	public:
		Arco();

		Arco(const int &adyacente, const C &costo);

		int devolverAdyacente() const;

		const C &devolverCosto() const;

		void nuevoCosto(const C &costo);

	private:
		int vertice;

		C costo;
	};

public:
	Grafo();
	Grafo(const Grafo &otroGrafo);

	~Grafo();

	Grafo &operator=(const Grafo &otroGrafo);

	bool estaVacio() const;

	int devolverLongitud() const;

    void agregarVertice(const int &vertice);

	void eliminarVertice(const int &vertice);

	bool existeVertice(const int &vertice) const;

    void devolverVertices(std::list<int> &vertices) const;

    void eliminarArco(const int &origen, const int &destino);

	bool existeArco(const int &origen, const int &destino) const;

	const C &costoArco(const int &origen, const int &destino) const;

    void agregarArco(const int &origen, const int &destino, const C &costo);

	void devolverAdyacentes(const int &origen, std::list<Arco> &adyacentes) const;

	void modificarCostoArco(const int &origen, const int &destino, const C &costo);

	void vaciar();

private:
	std::map<int, std::map<int, C>> grafo;
};

// ----------------------------------------------------------------------------------------------------------------- //
//                                                  Operador <<                                                      //
// ----------------------------------------------------------------------------------------------------------------- //

template <typename C>
std::ostream &operator<<(std::ostream &salida, const Grafo<C> &grafo)
{
	// Recorremos todos los vertices
	std::list<int> vertices;
	grafo.devolverVertices(vertices);
	typename std::list<int>::iterator v = vertices.begin();
	while (v != vertices.end())
	{
		salida << "    " << *v << "\n";
		// Recorremos todos los adyacentes de cada vertice
		std::list<typename Grafo<C>::Arco> adyacentes;
		grafo.devolverAdyacentes(*v, adyacentes);
		typename std::list<typename Grafo<C>::Arco>::iterator ady = adyacentes.begin();
		while (ady != adyacentes.end())
		{
			salida << "    " << *v << "-> " << ady->devolverAdyacente() << "\n";
			ady++;
		}
		v++;
	}
	return salida;
}

#endif
