#ifndef REDAEROPORTUARIA_H_
#define REDAEROPORTUARIA_H_

#include "Grafo.h"
#include <vector>
#include <set>

class RedAeroportuaria
{
public:
    class Aeropuerto
    {
    public:
        Aeropuerto(const std::string &nombre, const std::string &ciudad, const std::string &pais);

        ~Aeropuerto();

        std::string verPais() const;

        std::string verNombre() const;

        std::string verCiudad() const;

    private:
        std::string nombre, ciudad, pais;
    };

    class Vuelo
    {
    public:
        Vuelo();

        ~Vuelo();

        Vuelo(const Vuelo &otroVuelo);

        Vuelo &operator=(const Vuelo &otroVuelo);

        Vuelo(const bool &cabotaje, const double &distancia);

        void modificarAsientosReservados(const std::string &aerolinea, const unsigned int &reservas);

        void agregarAerolinea(const std::string &nombreAerolinea, const unsigned int &asientosTotales);

        bool esCabotaje() const;

        double verDistancia() const;

        unsigned int verCantidadVuelos() const;

        bool existeAerolinea(const std::string &aerolinea) const;

        void verAerolineas(std::list<std::string> &aerolineas) const;

        unsigned int verAsientosTotales(const std::string &aerolinea) const;

    private:
        bool cabotaje;

        double distancia;

        std::map<std::string, unsigned int> vuelos;
    };

    class Reserva
    {
    public:
        Reserva(const std::string &origen, const std::string &destino, const std::string &aerolinea,
                const unsigned int &asientosReservados, const unsigned int &asientosTotales);

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

    RedAeroportuaria();
    ~RedAeroportuaria();

    std::string nombreAeropuerto(const unsigned int &identificador) const;

    void agregarAeropuerto(const std::string &nombre, const std::string &ciudad, const std::string &pais);

    void agregarRuta(const std::string &origen, const std::string &destino, Vuelo &nuevoVuelo);

    void agregarReserva(const std::string &origen, const std::string &destino, const std::string &aerolinea, const unsigned int &asientosReservados);

    void agregarAerolinea(const std::string &aerolinea);

    bool estaVacia() const;

    bool existeAeropuerto(const unsigned int &aeropuerto) const;

    void generarListadoAeropuertos();

    void listarAeropuertos(std::list<Aeropuerto> &listaAeropuertos);

    void listarReservas(std::list<Reserva> &listaReservas);

    void listarAerolineas(std::list<std::string> &listaAerolineas);

    void verCircuitoMinimo(const std::string &nombre);

    void obtenerCircuitoMinimo(const unsigned int &verticeInicio, const unsigned int &indice, std::vector<unsigned int> &circuito, std::vector<unsigned int> &circuitoMinimo, unsigned int &costoActual, unsigned int &costoMinimo);

    bool perteneceCircuito(const unsigned int &verticeActual, const unsigned int &indice, const std::vector<unsigned int> &circuito);

    void imprimirCircuitoMinimo();

    unsigned int numeroAeropuertos() const;

    unsigned int numeroAerolineas() const;

    unsigned int numeroReservas() const;

    bool existeReserva(const unsigned int &aeropuertoOrigen, const unsigned int &aeropuertoDestino, const std::string & aerolinea) const;

    unsigned int devolverAsientosReservados(const unsigned int &aeropuertoOrigen, const unsigned int &aeropuertoDestino, const std::string &aerolinea) const;

    unsigned int devolverAsientosDisponibles(const unsigned int &aeropuertoOrigen, const unsigned int &aeropuertoDestino, const std::string &aerolinea) const;

    bool existeVueloDirecto(const unsigned int &aeropuertoOrigen, const unsigned int &aeropuertoDestino, const std::string &aerolinea) const;

    void vuelosMismaAerolinea(const unsigned int &origen, const unsigned int &destino, std::map<std::string, std::list<std::pair<std::list<std::string>, double>>> &vuelos);

    double devolverDistancia(const unsigned int &aeropuertoOrigen, const unsigned int &aeropuertoDestino) const;

    void obtenerCircuitoMinimo(const unsigned int &nombre, std::vector<unsigned int> &circuito, double &distancia);

private:
    void buscarCaminos(const unsigned int &origen, const unsigned int &destino, const std::string &aerolinea, double &distancia, std::vector<bool> &visitado, std::pair<std::list<std::string>, double> &camino, std::list<std::pair<std::list<std::string>, double>> &listadoCaminos);
    void calcularCircuitoMinimo(const unsigned int &verticeInicio, const unsigned int &indice, std::vector<unsigned int> &circuito, std::vector<unsigned int> &circuitoMinimo, double &costoActual, double &costoMinimo);

private:
    const int INF = 1e9;

    unsigned int cantidadReservas = 0;

    std::map<std::string, std::map<std::string, std::map<std::string, unsigned int>>> reservas; // Estructura para almacenar las reservas

    std::set<std::string> aerolineas; // Conjunto con todas las aerolíneas de la red de viaje.

    std::vector<Aeropuerto> aeropuertos; // Declaro una estructura que me permita conectar la información del aeropuerto con un �ndice.

    Grafo<Vuelo> redAeroportuaria;
};

#endif
