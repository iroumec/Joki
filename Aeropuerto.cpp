#include "Aeropuerto.h"

// --------------------------------------------------------------------------------------------- //

Aeropuerto::Aeropuerto()
{
}

Aeropuerto::Aeropuerto(const std::string &nombre, const std::string &ciudad, const std::string &pais)
{
    // Cargo los datos del aeropuerto.
    this->nombre = nombre;
    this->ciudad = ciudad;
    this->pais = pais;
}

// --------------------------------------------------------------------------------------------- //

Aeropuerto::~Aeropuerto()
{
}

// --------------------------------------------------------------------------------------------- //

std::string Aeropuerto::verNombre() const
{
    return nombre;
}

// --------------------------------------------------------------------------------------------- //

std::string Aeropuerto::verCiudad() const
{
    return ciudad;
}

// --------------------------------------------------------------------------------------------- //

std::string Aeropuerto::verPais() const
{
    return pais;
}
