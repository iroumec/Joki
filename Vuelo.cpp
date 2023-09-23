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
        agregarAerolinea(aerolinea, otroVuelo.verAsientosTotales(aerolinea), otroVuelo.verAsientosReservados(aerolinea));

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

unsigned int Vuelo::verAsientosLibres(const std::string &aerolinea) const
{
    // Inicializo los asientos disponibles en 0
    unsigned int asientos_disponibles = 0;

    // Busco la aerol�nea de la que quiero obtener los asientos libres.
    auto it = vuelos.find(aerolinea);

    // Si encontr� la aerol�nea, calculo los asientos disponibles.
    if (it != vuelos.end())
    {
        asientos_disponibles = (it->second).totales - (it->second).reservados;
    }

    // Retorno los asientos disponibles.
    return asientos_disponibles;
}

// --------------------------------------------------------------------------------------------- //

bool Vuelo::existeAerolinea(const std::string &aerolinea) const
{
    // Busco la aerolínea de la que quiero obtener los asientos libres.
    auto it = vuelos.find(aerolinea);

    // Retorno si logré obtener un iterador a ella.
    return (it != vuelos.end());
}

// --------------------------------------------------------------------------------------------- //

unsigned int Vuelo::verAsientosTotales(const std::string &aerolinea) const
{
    // Inicializo los asientos disponibles en 0
    unsigned int asientos_totales = 0;

    // Busco la aerol�nea de la que quiero obtener los asientos libres.
    auto it = vuelos.find(aerolinea);

    // Si encontr� la aerol�nea, calculo los asientos disponibles.
    if (it != vuelos.end())
    {
        asientos_totales = (it->second).totales;
    }

    // Retorno los asientos disponibles.
    return asientos_totales;
}

// --------------------------------------------------------------------------------------------- //

unsigned int Vuelo::verAsientosReservados(const std::string &aerolinea) const
{
    // Inicializo los asientos disponibles en 0
    unsigned int asientos_reservados = 0;

    // Busco la aerol�nea de la que quiero obtener los asientos libres.
    auto it = vuelos.find(aerolinea);

    // Si encontr� la aerol�nea, obtengo los asientos reservados.
    if (it != vuelos.end())
        asientos_reservados = (it->second).reservados;

    // Retorno los asientos disponibles.
    return asientos_reservados;
}

// --------------------------------------------------------------------------------------------- //

void Vuelo::modificarAsientosReservados(const std::string &aerolinea, const unsigned int &reservas)
{
    // Busca la struct reservas de la aerol�nea cuyos asientos se quieren modificar.
    auto it = vuelos.find(aerolinea);

    // Si la encuentra, cambia el n�mero de asientos reservados.
    if (it != vuelos.end())
    {
        (it->second).reservados = reservas;
    }

    // Si no la encuentra, nada se va a modificar.
}

// --------------------------------------------------------------------------------------------- //

void Vuelo::agregarAerolinea(const std::string &nombreAerolinea, const unsigned int &asientosTotales,
                             const unsigned int &asientosReservados)
{
    struct asientos asientosNuevoVuelo;

    asientosNuevoVuelo.totales = asientosTotales;
    asientosNuevoVuelo.reservados = asientosReservados;

    vuelos.insert({nombreAerolinea, asientosNuevoVuelo});
}
