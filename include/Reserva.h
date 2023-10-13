#include <iostream>
#include <map>

#ifndef RESERVA_H_
#define RESERVA_H_

class Reserva
{
public:
    Reserva();

    Reserva(const std::string &origen, const std::string &destino, const std::string &aerolinea, const unsigned int &asientosResevados, const unsigned int &asientosTotales);

    ~Reserva();

    std::string retornarOrigen() const;

    std::string retornarDestino() const;

    std::string retornarAerolinea() const;

    unsigned int retornarAsientosReservados() const;

    unsigned int retornarAsientosTotales() const;

private:
    std::string aeropuertoOrigen, aeropuertoDestino, aerolinea;
    unsigned int asientosReservados, asientosTotales;
};

#endif