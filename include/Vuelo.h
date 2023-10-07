#ifndef VUELO_H_
#define VUELO_H_

#include <map>
#include <list>
#include <string>

class Vuelo
{
public:

    // ------------------------------------------------------------------------------------- //
    ///                               Métodos Constructores                                 ///
    // --------------------------------------------------------------------------------------//

    Vuelo();

    Vuelo(const bool & cabotaje, const double & distancia);

    Vuelo(const Vuelo & otroVuelo);

    ~Vuelo();

    Vuelo & operator = (const Vuelo & otroVuelo);

    // ------------------------------------------------------------------------------------- //
    ///                               Métodos Modificadores                                 ///
    // --------------------------------------------------------------------------------------//

    void agregarAerolinea(const std::string & nombreAerolinea, const unsigned int & asientosTotales);

    void modificarAsientosReservados(const std::string & aerolinea, const unsigned int & reservas);

    // ------------------------------------------------------------------------------------- //
    ///                                Métodos Observadores                                 ///
    // --------------------------------------------------------------------------------------//

    double verDistancia() const;

    unsigned int verCantidadVuelos() const;

    bool esCabotaje() const;

    bool existeAerolinea(const std::string & aerolinea) const;

    void verAerolineas(std::list<std::string> & aerolineas) const;

    unsigned int verAsientosTotales(const std::string & aerolinea) const;

private:

    bool cabotaje; // Almaceno si el vuelo es de cabotaje o no (nacional).

    double distancia; // Almaceno la distancia del vuelo.

    std::map<std::string, unsigned int> vuelos; // Almaceno las aerolíneas por las cuales es posible el vuelo (y sus reservas).
};


#endif // VUELO_H_
