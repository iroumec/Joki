#include "Reserva.h"

Reserva::Reserva()
{
}

Reserva::~Reserva() // O(n)
{
    reservas.clear();
}

void Reserva::agregarReserva(const string & aerolinea, const unsigned int & asientos_reservados) // O(log n)
{
    reservas.insert({aerolinea, asientos_reservados});
}

unsigned int Reserva::retornarAsientosReservados(const string & aerolinea) const // O(log n)
{
    auto it = reservas.find(aerolinea);

    if (it == reservas.end())
        throw invalid_argument("Arguemnto inválido: la aerolínea no se encuentra en el mapa");

    return (it->second);
}

