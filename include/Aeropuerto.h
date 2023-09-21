#ifndef AEROPUERTO_H_
#define AEROPUERTO_H_

#include <string>

class Aeropuerto
{

public:

    Aeropuerto();

    Aeropuerto(const unsigned int & id, const std::string & nombre, const std::string & ciudad, const std::string & pais);

    ~Aeropuerto();

    unsigned int verId() const;

    std::string verNombre() const; // Consultar el nombre del aeropuerto.

    std::string verCiudad() const; // Consultar la ciudad del aeropuerto.

    std::string verPais() const; // Consultar el país del aeropuerto.

private:

    std::string nombre, ciudad, pais;
    unsigned int id;

};

#endif // AEROPUERTO_H
