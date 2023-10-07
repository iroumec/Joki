#include <iostream>
#include <map>
using namespace std;

#ifndef RESERVA_H_
#define RESERVA_H_

class Reserva
{
public:
    Reserva();
    ~Reserva();

    void agregarReserva(const string & aerolinea, const unsigned int & asientos_reservados);

    unsigned int retornarAsientosReservados(const string & aerolinea) const;

private:
    map<string, unsigned int> reservas; // Mapa con aerolíneas y sus asientos reservados
};

#endif
