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

    Vuelo(const bool & cabotaje, const double & distancia);

    Vuelo(const Vuelo & otroVuelo);

    Vuelo();

    ~Vuelo();

    Vuelo & operator = (const Vuelo & otroVuelo);

    // ------------------------------------------------------------------------------------- //
    ///                               Métodos Modificadores                                 ///
    // --------------------------------------------------------------------------------------//

    void agregarAerolinea(const std::string & nombreAerolinea, const unsigned int & asientosTotales,
                          const unsigned int & asientosReservados = 0);

    void modificarAsientosReservados(const std::string & aerolinea, const unsigned int & reservas);

    // ------------------------------------------------------------------------------------- //
    ///                                Métodos Observadores                                 ///
    // --------------------------------------------------------------------------------------//

    double verDistancia() const;

    unsigned int verCantidadVuelos() const;

    bool esCabotaje() const;

    bool existeAerolinea(const std::string & aerolinea) const;

    void verAerolineas(std::list<std::string> & aerolineas) const;

    unsigned int verAsientosLibres(const std::string & aerolinea) const;

    unsigned int verAsientosTotales(const std::string & aerolinea) const;

    unsigned int verAsientosReservados(const std::string & aerolinea) const;

private:

    struct asientos // Estructura para almacenar los asientos totales y los reservados.
    {
        unsigned int totales, reservados;
    };

    bool cabotaje; // Almaceno si el vuelo es de cabotaje o no (nacional).

    double distancia; // Almaceno la distancia del vuelo.

    std::map<std::string, struct asientos> vuelos; // Almaceno las aerolíneas por las cuales es posible el vuelo (y sus reservas).
};


#endif // VUELO_H_
