#ifndef REDAEROPORTUARIA_H_
#define REDAEROPORTUARIA_H_

#include "Grafo.h"
#include "Reserva.h"
#include <vector>
#include <set>

class RedAeroportuaria
{
public:
    class Aeropuerto
    {
    public:
        Aeropuerto();

        Aeropuerto(const std::string &nombre, const std::string &ciudad, const std::string &pais);

        ~Aeropuerto();

        std::string verNombre() const; // Consultar el nombre del aeropuerto.

        std::string verCiudad() const; // Consultar la ciudad del aeropuerto.

        std::string verPais() const;

    private:
        std::string nombre, ciudad, pais;
    };

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
    }

    RedAeroportuaria();
    ~RedAeroportuaria();

    std::string nombreAeropuerto(const unsigned int &identificador);

    void agregarAeropuerto(const string &nombre, const string &ciudad, const string &pais);

    void agregarRuta(const std::string &origen, const std::string &destino, Vuelo &nuevoVuelo);

    void agregarReserva(const string &origen, const string &destino, const string &aerolinea, const unsigned int &asientosReservados);

    void agregarAerolinea(const string &aerolinea, const unsigned int &asientosTotales);

    bool estaVacia() const;

    unsigned int retornarNumeroReservas() const;

    void generarListadoAeropuertos();

    void listarAeropuertos(list<Aeropuerto> &listaAeropuertos);

    void listarReservas(list<Reserva> &listaReservas);

    void verCircuitoMinimo(const string &nombre);

    void obtenerCircuitoMinimo(const unsigned int &verticeInicio, const unsigned int &indice, vector<unsigned int> &circuito, vector<unsigned int> &circuitoMinimo, unsigned int &costoActual, unsigned int &costoMinimo);

    bool perteneceCircuito(const unsigned int &verticeActual, const unsigned int &indice, const vector<unsigned int> &circuito);

    void imprimirCircuitoMinimo();

    unsigned int numeroAeropuertos() const;

    bool existeVueloDirecto(const unsigned int &aeropuertoOrigen, const unsigned int &aeropuertoDestino, const std::string &aerolinea);

private:
    const int INF = 1e9;

    unsigned int numeroReservas = 0;

    map<string, map<string, map<string, unsigned int>>> reservas; // Mapa de mapas con las reservas

    set<string> aerolineas; // Conjunto con todas las aerolíneas de la red de viaje.

    vector<Aeropuerto> aeropuertos; // Declaro una estructura que me permita conectar la información del aeropuerto con un �ndice.

    Grafo<Vuelo> redAeroportuaria;
};

#endif // REDAEROPORTUARIA_H_
