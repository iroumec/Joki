#include "Vuelo.h"

// --------------------------------------------------------------------------------------------- //

Vuelo::Vuelo(const bool &cabotaje, const double &distancia)
{
    this->cabotaje = cabotaje;
    this->distancia = distancia;
}

// --------------------------------------------------------------------------------------------- //

Vuelo::~Vuelo()
{
    // Vac�o el mapa de aerol�neas y sus respectivas reservas.
    vuelos.clear();
}

// --------------------------------------------------------------------------------------------- //

Vuelo::Vuelo(const Vuelo &otroVuelo)
{
    *this = otroVuelo;
}

// --------------------------------------------------------------------------------------------- //

Vuelo::Vuelo()
{
    this->cabotaje = false;
    this->distancia = 0;
}

// --------------------------------------------------------------------------------------------- //

Vuelo &Vuelo::operator=(const Vuelo &otroVuelo)
{
    // Limpio la estructura.
    vuelos.clear();

    this->cabotaje = otroVuelo.esCabotaje();
    this->distancia = otroVuelo.verDistancia();

    std::list<std::string> aerolineas;
    otroVuelo.verAerolineas(aerolineas);

    for (const std::string & aerolinea : aerolineas)
        agregarAerolinea(aerolinea, otroVuelo.verAsientosTotales(aerolinea));

    return *this;
}

// --------------------------------------------------------------------------------------------- //

unsigned int Vuelo::verCantidadVuelos() const
{
    return vuelos.size();
}

// --------------------------------------------------------------------------------------------- //

void Vuelo::verAerolineas(std::list<std::string> &aerolineas) const
{
    auto it = vuelos.begin();

    // Recorro el mapa, agregando el nombre de la aerol�nea a la lista a retornar.
    while (it != vuelos.end())
    {
        aerolineas.push_back((it->first));

        it++;
    }
}

// --------------------------------------------------------------------------------------------- //

double Vuelo::verDistancia() const
{
    return distancia;
}

// --------------------------------------------------------------------------------------------- //

bool Vuelo::esCabotaje() const
{
    return cabotaje;
}

// --------------------------------------------------------------------------------------------- //

bool Vuelo::existeAerolinea(const std::string &aerolinea) const // O(log n)
{
    // Busco la aerolínea de la que quiero obtener los asientos libres.
    auto it = vuelos.find(aerolinea);

    // Retorno si logré obtener un iterador a ella.
    return (it != vuelos.end());
}

// --------------------------------------------------------------------------------------------- //

unsigned int Vuelo::verAsientosTotales(const std::string &aerolinea) const // O(log n)
{
    // Inicializo los asientos disponibles en 0
    unsigned int asientos_totales = 0;

    // Busco la aerol�nea de la que quiero obtener los asientos libres.
    auto it = vuelos.find(aerolinea);

    // Si encontr� la aerol�nea, calculo los asientos disponibles.
    if (it != vuelos.end())
    {
        asientos_totales = it->second;
    }

    // Retorno los asientos disponibles.
    return asientos_totales;
}

// --------------------------------------------------------------------------------------------- //

void Vuelo::agregarAerolinea(const std::string &nombreAerolinea, const unsigned int &asientosTotales) // O(log n)
{
    vuelos.insert({nombreAerolinea, asientosTotales});
}
