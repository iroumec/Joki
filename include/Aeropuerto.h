#ifndef AEROPUERTO_H_
#define AEROPUERTO_H_

#include <string>

class Aeropuerto
{

public:
    Aeropuerto();

    Aeropuerto(const std::string &nombre, const std::string &ciudad, const std::string &pais);

    ~Aeropuerto();

    std::string verNombre() const; // Consultar el nombre del aeropuerto.

    std::string verCiudad() const; // Consultar la ciudad del aeropuerto.

    std::string verPais() const; // Consultar el pa�s del aeropuerto.

private:
    std::string nombre, ciudad, pais;
};

#endif // AEROPUERTO_H
