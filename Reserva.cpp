#include "Reserva.h"

Reserva::Reserva()
{
}

Reserva::Reserva(const std::string &origen, const std::string &destino, const std::string &aerolinea, const unsigned int &asientosResevados, const unsigned int &asientosTotales)
{
    this->aeropuertoOrigen = origen;
    this->aeropuertoDestino = destino;
    this->aerolinea = aerolinea;
    this->asientosReservados = asientosReservados;
    this->asientosTotales = asientosTotales;
}

std::string Reserva::retornarOrigen() const
{
    return aeropuertoOrigen;
}

std::string Reserva::retornarDestino() const
{
    return aeropuertoDestino;
}

std::string Reserva::retornarAerolinea() const
{
    return aerolinea;
}

unsigned int retornarAsientosReservados() const
{
    return asientosReservados;
}

unsigned int retornarAsientosTotales() const
{
    return asientosTotales;
}

Reserva::~Reserva()
{
}
