#include "Aeropuerto.h"

// --------------------------------------------------------------------------------------------- //

Aeropuerto::Aeropuerto()
{

}

Aeropuerto::Aeropuerto(const unsigned int & id, const std::string & nombre, const std::string & ciudad, const std::string & pais)
{
    // Cargo los datos del aeropuerto.
    this->id = id;
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

// --------------------------------------------------------------------------------------------- //

unsigned int Aeropuerto::verId() const
{
    return id;
}
