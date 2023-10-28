#include "RedAeroportuaria.h"

// ----------------------------------------------------------------------------------------------------------------- //
//                                              Subclase Aeropuerto                                                  //
// ----------------------------------------------------------------------------------------------------------------- //

RedAeroportuaria::Aeropuerto::Aeropuerto(const std::string &nombre, const std::string &ciudad,
                                         const std::string &pais) // O(1)
{
    this->nombre = nombre;
    this->ciudad = ciudad;
    this->pais = pais;
}

// ----------------------------------------------------------------------------------------------------------------- //

RedAeroportuaria::Aeropuerto::~Aeropuerto() // O(1)
{
}

// ----------------------------------------------------------------------------------------------------------------- //

std::string RedAeroportuaria::Aeropuerto::verNombre() const // O(1)
{
    return nombre;
}

// ----------------------------------------------------------------------------------------------------------------- //

std::string RedAeroportuaria::Aeropuerto::verCiudad() const // O(1)
{
    return ciudad;
}

// ----------------------------------------------------------------------------------------------------------------- //

std::string RedAeroportuaria::Aeropuerto::verPais() const // O(1)
{
    return pais;
}

// ----------------------------------------------------------------------------------------------------------------- //
//                                                 Subclase Vuelo                                                    //
// ----------------------------------------------------------------------------------------------------------------- //

RedAeroportuaria::Vuelo::Vuelo(const bool &cabotaje, const double &distancia) // O(1)
{
    this->cabotaje = cabotaje;
    this->distancia = distancia;
}

// ----------------------------------------------------------------------------------------------------------------- //

RedAeroportuaria::Vuelo::~Vuelo() // O(k), siendo k el número de aerolíneas que posibilitan el vuelo
{
    vuelos.clear();
}

// ----------------------------------------------------------------------------------------------------------------- //

RedAeroportuaria::Vuelo::Vuelo(const Vuelo &otroVuelo)
// O(k log k), siendo k el número de aerolíneas que posibilitan el vuelo, debido a que invoca al operador =.
{
    *this = otroVuelo;
}

// ----------------------------------------------------------------------------------------------------------------- //

RedAeroportuaria::Vuelo::Vuelo() // O(1)
{
    this->cabotaje = false;
    this->distancia = 0;
}

// ----------------------------------------------------------------------------------------------------------------- //

RedAeroportuaria::Vuelo &RedAeroportuaria::Vuelo::operator=(const Vuelo &otroVuelo)
// O(k log k), siendo k el número de aerolíneas que posibilitan el viaje.
{
    vuelos.clear(); // O(k)

    this->cabotaje = otroVuelo.esCabotaje();
    this->distancia = otroVuelo.verDistancia();

    std::list<std::string> aerolineas;
    otroVuelo.verAerolineas(aerolineas);

    for (const std::string &aerolinea : aerolineas)
        agregarAerolinea(aerolinea, otroVuelo.verAsientosTotales(aerolinea)); // O(log k)

    return *this;
}

// ----------------------------------------------------------------------------------------------------------------- //

unsigned int RedAeroportuaria::Vuelo::verCantidadVuelos() const // O(1)
{
    return vuelos.size();
}

// ----------------------------------------------------------------------------------------------------------------- //

void RedAeroportuaria::Vuelo::verAerolineas(std::list<std::string> &aerolineas) const // O(1)
{
    std::string aerolinea;
    for (const auto &vuelo : vuelos)
    {
        aerolinea = vuelo.first;
        aerolineas.push_back(aerolinea);
    }
}

// ----------------------------------------------------------------------------------------------------------------- //

double RedAeroportuaria::Vuelo::verDistancia() const // O(1)
{
    return distancia;
}

// ----------------------------------------------------------------------------------------------------------------- //

bool RedAeroportuaria::Vuelo::esCabotaje() const // O(1)
{
    return cabotaje;
}

// ----------------------------------------------------------------------------------------------------------------- //

bool RedAeroportuaria::Vuelo::existeAerolinea(const std::string &aerolinea) const
// O(log k), siendo k el número de aerolíneas.
{
    // Busco la aerolínea de la que quiero obtener los asientos libres.
    auto iteradorAerolinea = vuelos.find(aerolinea);

    // Retorno si logré obtener un iterador a ella.
    return (iteradorAerolinea != vuelos.end());
}

// ----------------------------------------------------------------------------------------------------------------- //

unsigned int RedAeroportuaria::Vuelo::verAsientosTotales(const std::string &aerolinea) const
// O(log k), siendo k el número de aerolíneas.
{
    unsigned int asientos_totales = 0;

    auto it = vuelos.find(aerolinea);

    if (it != vuelos.end())
    {
        asientos_totales = it->second;
    }

    return asientos_totales;
}

// ----------------------------------------------------------------------------------------------------------------- //

void RedAeroportuaria::Vuelo::agregarAerolinea(const std::string &nombreAerolinea, const unsigned int &asientosTotales)
// O(log k), siendo k el número de aerolíneas que posibilitan el vuelo entre los aeropuertos
{
    vuelos.insert({nombreAerolinea, asientosTotales});
}

// ----------------------------------------------------------------------------------------------------------------- //
//                                                 Subclase Reserva                                                  //
// ----------------------------------------------------------------------------------------------------------------- //

RedAeroportuaria::Reserva::Reserva(const std::string &origen, const std::string &destino, const std::string &aerolinea,
                                   const unsigned int &asientosReservados, const unsigned int &asientosTotales) // O(1)
{
    this->aeropuertoOrigen = origen;
    this->aeropuertoDestino = destino;
    this->aerolinea = aerolinea;
    this->asientosReservados = asientosReservados;
    this->asientosTotales = asientosTotales;
}

// ----------------------------------------------------------------------------------------------------------------- //

std::string RedAeroportuaria::Reserva::retornarOrigen() const // O(1)
{
    return aeropuertoOrigen;
}

// ----------------------------------------------------------------------------------------------------------------- //

std::string RedAeroportuaria::Reserva::retornarDestino() const // O(1)
{
    return aeropuertoDestino;
}

// ----------------------------------------------------------------------------------------------------------------- //

std::string RedAeroportuaria::Reserva::retornarAerolinea() const // O(1)
{
    return aerolinea;
}

// ----------------------------------------------------------------------------------------------------------------- //

unsigned int RedAeroportuaria::Reserva::retornarAsientosReservados() const // O(1)
{
    return asientosReservados;
}

// ----------------------------------------------------------------------------------------------------------------- //

unsigned int RedAeroportuaria::Reserva::retornarAsientosTotales() const // O(1)
{
    return asientosTotales;
}

// ----------------------------------------------------------------------------------------------------------------- //

RedAeroportuaria::Reserva::~Reserva() // O(1)
{
}

// ----------------------------------------------------------------------------------------------------------------- //
//                                            Clase RedAeroportuaria                                                 //
// ----------------------------------------------------------------------------------------------------------------- //

RedAeroportuaria::RedAeroportuaria() // O(1)
{
}

// ----------------------------------------------------------------------------------------------------------------- //

RedAeroportuaria::~RedAeroportuaria() // O(1)
{
}

// ----------------------------------------------------------------------------------------------------------------- //

std::string RedAeroportuaria::nombreAeropuerto(const unsigned int &identificador) const // O(1)
{
    return aeropuertos[identificador].verNombre();
}

// ----------------------------------------------------------------------------------------------------------------- //

void RedAeroportuaria::agregarAeropuerto(const std::string &nombre, const std::string &ciudad,
                                         const std::string &pais)
// O(log n), siendo n el número de aeropuertos en la red aeroportuaria
{
    Aeropuerto nuevoAeropuerto(nombre, ciudad, pais);

    redAeroportuaria.agregarVertice(aeropuertos.size());

    aeropuertos.push_back(nuevoAeropuerto);
}

// ----------------------------------------------------------------------------------------------------------------- //

void RedAeroportuaria::agregarRuta(const std::string &origen, const std::string &destino, Vuelo &nuevoVuelo)
// O(n), siendo n el número de aeropuertos
{
    unsigned int claveOrigen, claveDestino;

    // Debido a que nuestro grafo trabaja con enteros, debemos recorrer el std::mapa de aeropuertos
    // en busca de las claves para poder aagregar el arco a nuestro grafo.
    for (unsigned int i = 0; i < aeropuertos.size(); i++)
    {
        if (aeropuertos[i].verNombre() == origen)
            claveOrigen = i;
        if (aeropuertos[i].verNombre() == destino)
            claveDestino = i;
    }

    redAeroportuaria.agregarArco(claveOrigen, claveDestino, nuevoVuelo);
    redAeroportuaria.agregarArco(claveDestino, claveOrigen, nuevoVuelo);
}

// ----------------------------------------------------------------------------------------------------------------- //

void RedAeroportuaria::agregarReserva(const std::string &origen, const std::string &destino,
                                      const std::string &aerolinea, const unsigned int &asientosReservados)
// O(max{log n, log k}), siendo n el número de aeropuertos y k, el número de aerolíneas que posibilitan el vuelo
// entre el viaje y el destino. No incluimos a log m, ya que log n, en el peor de los casos, lo incluirá.
{
    // Busco el aeropuerto de origen
    auto iteradorAeropuertoOrigen = reservas.find(origen);

    if (iteradorAeropuertoOrigen != reservas.end())
    {
        // Busco el aeropuerto de destino
        auto iteradorAeropuertoDestino = (iteradorAeropuertoOrigen->second).find(destino);

        if (iteradorAeropuertoDestino != (iteradorAeropuertoOrigen->second).end())
        {
            // Busco la aerolínea
            auto iteradorAerolinea = (iteradorAeropuertoDestino->second).find(aerolinea); // Busco la aerolínea

            // De no existir la aerolínea, la agrego
            if (iteradorAerolinea == (iteradorAeropuertoDestino->second).end()) // Si no está la aerolínea aún
            {
                (iteradorAeropuertoDestino->second).insert({aerolinea, asientosReservados});
            }
        }
        else // De no existir el aaeropuerto de destino, lo agrego, junto a la aerolínea
        {
            std::map<std::string, unsigned int> nuevaAerolinea;
            nuevaAerolinea.insert({aerolinea, asientosReservados});

            (iteradorAeropuertoOrigen->second).insert({destino, nuevaAerolinea});
        }
    }
    else // Si no está el aeropuerto de origen
    {
        std::map<std::string, unsigned int> nuevaAerolinea;
        nuevaAerolinea.insert({aerolinea, asientosReservados});

        std::map<std::string, std::map<std::string, unsigned int>> nuevoDestino;
        nuevoDestino.insert({destino, nuevaAerolinea});

        reservas.insert({origen, nuevoDestino});
    }

    cantidadReservas++;
}

// ----------------------------------------------------------------------------------------------------------------- //

void RedAeroportuaria::agregarAerolinea(const std::string &nuevaAerolinea) // O(log k), siendo k el número de aerolíneas
{
    aerolineas.insert({nuevaAerolinea});
}

// ----------------------------------------------------------------------------------------------------------------- //

bool RedAeroportuaria::estaVacia() const // O(1)
{
    return (redAeroportuaria.devolverLongitud() == 0);
}

// ----------------------------------------------------------------------------------------------------------------- //

bool RedAeroportuaria::existeAeropuerto(const unsigned int &aeropuerto) const
// O(log n), siendo n el número de aeropuertos.
{
    return redAeroportuaria.existeVertice(aeropuerto);
}

// ----------------------------------------------------------------------------------------------------------------- //
//                                                Listar Aeropuertos                                                 //
// ----------------------------------------------------------------------------------------------------------------- //

void RedAeroportuaria::listarAeropuertos(std::list<Aeropuerto> &listaAeropuertos)
// O(n), siendo n el número de aeropuertos
{
    for (const Aeropuerto &aeropuerto : aeropuertos)
        listaAeropuertos.push_back(aeropuerto);
}

// ----------------------------------------------------------------------------------------------------------------- //

unsigned int RedAeroportuaria::numeroAeropuertos() const // O(1)
{
    return aeropuertos.size();
}

// ----------------------------------------------------------------------------------------------------------------- //

void RedAeroportuaria::listarAerolineas(std::list<std::string> &listaAerolineas)
// O(k), siendo k el número de aerolíneas en el conjunto
{
    for (const auto &aerolinea : aerolineas)
        listaAerolineas.push_back(aerolinea);
}

// ----------------------------------------------------------------------------------------------------------------- //

unsigned int RedAeroportuaria::numeroAerolineas() const // O(1)
{
    return aerolineas.size();
}

// ----------------------------------------------------------------------------------------------------------------- //
//                                                  Listar Reservas                                                  //
// ----------------------------------------------------------------------------------------------------------------- //

void RedAeroportuaria::listarReservas(std::list<Reserva> &listaReservas)
// O(n * m * k), siendo n el número de aeropuertos; siendo m el número de adyacentes del aeropuerto;
// y k, siendo k el número de aerolíneas que posibilitan el viaje entre el aeropuerto y el adyacente.
{
    Vuelo vuelo;
    std::list<Grafo<Vuelo>::Arco> arcos;
    std::list<std::string> aerolineas;
    unsigned int aeropuerto, asientosReservados;
    std::string aeropuertoOrigen, aeropuertoDestino;

    // Itero por cada aeropuerto
    for (aeropuerto = 0; aeropuerto < numeroAeropuertos(); aeropuerto++)
    {
        // Obtengo los aeropuertos adyacentes al aeropuerto de origen.
        redAeroportuaria.devolverAdyacentes(aeropuerto, arcos);

        aeropuertoOrigen = aeropuertos[aeropuerto].verNombre();

        // Itero sobre los adyacentes
        for (const auto &arco : arcos)
        {
            vuelo = arco.devolverCosto();

            // Obtengo las aerolíneas que posibilitan el viaje entre el aeropuerto de origen y el aeropuerto de destino
            vuelo.verAerolineas(aerolineas);

            aeropuertoDestino = nombreAeropuerto(arco.devolverAdyacente());

            // Itero sobre las aerolíneas
            for (std::string aerolinea : aerolineas)
            {
                if (existeReserva(aeropuerto, arco.devolverAdyacente(), aerolinea))
                {
                    asientosReservados = devolverAsientosReservados(aeropuerto, arco.devolverAdyacente(), aerolinea);

                    if (asientosReservados > 0)
                    {
                        Reserva nuevaReserva(aeropuertoOrigen, aeropuertoDestino, aerolinea, asientosReservados, vuelo.verAsientosTotales(aerolinea));

                        listaReservas.push_back(nuevaReserva);
                    }
                }
            }

            // Vacío la lista de aerolíneas para darle paso a la próxima iteración
            aerolineas.clear();
        }

        // Vacío la lista de arcos para dar paso a la siguiente iteración
        arcos.clear();
    }
}

// ----------------------------------------------------------------------------------------------------------------- //

bool RedAeroportuaria::existeReserva(const unsigned int &aeropuertoOrigen, const unsigned int &aeropuertoDestino,
                                     const std::string &aerolinea) const
// O(max{log n, log k}), siendo n el número de aeropuertos y k, el número de aerolíneas que posibilitan el vuelo
// entre el viaje y el destino. No incluimos a log m, siendo m el número de adyacentes del aeropuerto de origen,
// ya que, en el peor de los casos, m = n.
{
    bool reservaHallada = false;

    // Busco el aeropuerto de origen
    auto iteradorAeropuertoOrigen = reservas.find(nombreAeropuerto(aeropuertoOrigen));

    if (iteradorAeropuertoOrigen != reservas.end())
    {
        // Busco el aeropuerto de destino
        auto iteradorAeropuertoDestino = (iteradorAeropuertoOrigen->second).find(nombreAeropuerto(aeropuertoDestino));

        if (iteradorAeropuertoDestino != (iteradorAeropuertoOrigen->second).end())
        {
            // Busco el aeropuerto de aerolínea
            auto iteradorAerolinea = (iteradorAeropuertoDestino->second).find(aerolinea);

            if (iteradorAerolinea != (iteradorAeropuertoDestino->second).end())
                reservaHallada = true;
        }
    }

    return reservaHallada;
}

// ----------------------------------------------------------------------------------------------------------------- //

unsigned int RedAeroportuaria::numeroReservas() const // O(1)
{
    return cantidadReservas;
}

// ----------------------------------------------------------------------------------------------------------------- //
//                                       Verificar si existe un vuelo directo                                        //
// ----------------------------------------------------------------------------------------------------------------- //

bool RedAeroportuaria::existeVueloDirecto(const unsigned int &aeropuertoOrigen, const unsigned int &aeropuertoDestino,
                                          const std::string &aerolinea) const
// Sea n el número de vértices en el grafo y k, el número de aerolíneas que posibilitan el viaje entre el origen y el destino,
// Complejidad espacial: O(k), debido a que se tiene un mapa con las aerolíneas y sus asientos totales.
// Complejidad temporal: O(max{log n, k log k}), debido a los métodos que se invocan.
{
    bool vueloDirecto = false;

    if (redAeroportuaria.existeArco(aeropuertoOrigen, aeropuertoDestino)) // O(log n), siendo n el número de aeropuertos.
    {
        Vuelo vuelo = redAeroportuaria.costoArco(aeropuertoOrigen, aeropuertoDestino); // O(max{log n, k log k}), siendo k el número de aerolíneas que posibilitan el viaje.

        if (vuelo.existeAerolinea(aerolinea))
        {
            vueloDirecto = true;
        }
    }

    return vueloDirecto;
}

// ----------------------------------------------------------------------------------------------------------------- //

unsigned int RedAeroportuaria::devolverAsientosReservados(const unsigned int &aeropuertoOrigen,
                                                          const unsigned int &aeropuertoDestino, const std::string &aerolinea) const
// O(max{log n, log k}), siendo n el número de aeropuertos y k, el número de aerolíneas que posibilitan el vuelo
// entre el viaje y el destino. No incluimos a log m, siendo m el número de adyacentes del aeropuerto de origen,
// ya que, en el peor de los casos, m = n.
{
    unsigned int asientosReservados;
    std::string nombreAeropuertoOrigen, nombreAeropuertoDestino;

    nombreAeropuertoOrigen = aeropuertos[aeropuertoOrigen].verNombre();
    nombreAeropuertoDestino = aeropuertos[aeropuertoDestino].verNombre();

    // Busco el aeropuerto de origen
    auto iteradorAeropuertoOrigen = reservas.find(nombreAeropuertoOrigen); // O(log n)

    if (iteradorAeropuertoOrigen != reservas.end())
    {
        // Busco el aeropuerto de destino
        // O(log m), siendo m el número de adyacentes al vértice de origen.
        auto iteradorAeropuertoDestino = (iteradorAeropuertoOrigen->second).find(nombreAeropuertoDestino);

        if (iteradorAeropuertoDestino != (iteradorAeropuertoOrigen->second).end())
        {
            // Busco la aerolínea
            // O(log k), siendo k el número de reservas
            auto iteradorAerolinea = (iteradorAeropuertoDestino->second).find(aerolinea);

            if (iteradorAerolinea != (iteradorAeropuertoDestino->second).end())
            {
                asientosReservados = (iteradorAerolinea->second);
            }
            else
            {
                throw std::invalid_argument("No se halló la aerolínea dentro de las reservas");
            }
        }
        else
        {
            throw std::invalid_argument("No se halló el destino dentro de las reservas");
        }
    }
    else
    {
        throw std::invalid_argument("No se halló el origen dentro de las reservas");
    }

    return asientosReservados;
}

// ----------------------------------------------------------------------------------------------------------------- //

unsigned int RedAeroportuaria::devolverAsientosDisponibles(const unsigned int &aeropuertoOrigen,
                                                           const unsigned int &aeropuertoDestino, const std::string &aerolinea) const
// O(max{log n, k log k}), siendo n el número de aeropuertos; m, el número de aeropuertos adyacentes al de origen;
// y k, el número de aerolíneas
{
    unsigned int asientosDisponibles, asientosTotales, asientosReservados;

    Vuelo vuelo;

    if (redAeroportuaria.existeArco(aeropuertoOrigen, aeropuertoDestino))               // O(log n), siendo n el número de vértices.
    {
        Vuelo vuelo = redAeroportuaria.costoArco(aeropuertoOrigen, aeropuertoDestino);  // O(k log k), siendo k el número de aerolíneas que posibilitan el viaje.
        asientosTotales = vuelo.verAsientosTotales(aerolinea);                          // O(1)

        if (existeReserva(aeropuertoOrigen, aeropuertoDestino, aerolinea))              // O(max{log n, log k})
        {
            asientosReservados = devolverAsientosReservados(aeropuertoOrigen, aeropuertoDestino, aerolinea); // O(max{log n, log k})
        }
        else
            asientosReservados = 0;
    }
    else
        throw std::invalid_argument("No existe un arco entre el vértice de origen y el de destino");

    asientosDisponibles = asientosTotales - asientosReservados;
    return asientosDisponibles;
}

// ----------------------------------------------------------------------------------------------------------------- //

double RedAeroportuaria::devolverDistancia(const unsigned int &aeropuertoOrigen, const unsigned int &aeropuertoDestino) const
// O(log n), siendo n el número de aeropuertos.
{
    unsigned int distancia;

    if (redAeroportuaria.existeArco(aeropuertoOrigen, aeropuertoDestino)) // O(log n)
    {
        Vuelo vuelo = redAeroportuaria.costoArco(aeropuertoOrigen, aeropuertoDestino); // O(log n)

        distancia = vuelo.verDistancia(); // O(1)
    }
    else
        throw std::invalid_argument("No existe un arco entre el vértice de origen y el de destino");

    return distancia;
}

// ----------------------------------------------------------------------------------------------------------------- //
//                          Vuelos desde un Origen a un Destino sin cambiar de Aerolínea                             //
// ----------------------------------------------------------------------------------------------------------------- //

void RedAeroportuaria::vuelosMismaAerolinea(const unsigned int &origen, const unsigned int &destino,
                                            std::map<std::string, std::list<std::pair<std::list<std::string>, double>>> &vuelos)
// Complejidad temporal: O(max{k*n*log n, (k^2)*n*log k), siendo n el número de aeropuertos y k, el número de aerolíneas.
// Esto debido a que, por cada aerolínea, invoca a "buscarCaminos", cuya complejidad es la indicada.
// Complejidad espacial: O(max{n, k}), ya que se invoca a "buscarCaminos", cuya complejidad espacial es la indicada.
{
    double distancia = 0;
    std::vector<bool> visitado(aeropuertos.size(), false);

    // Por cada aerolínea en el conjunto de aerolíneas...
    for (const auto &aerolinea : aerolineas)
    {
        std::pair<std::list<std::string>, double> camino;
        std::list<std::pair<std::list<std::string>, double>> listadoCaminos;

        buscarCaminos(origen, destino, aerolinea, distancia, visitado, camino, listadoCaminos); // O(max{n*log n, n*k log k})

        vuelos.insert({aerolinea, listadoCaminos});
    }
}

// ----------------------------------------------------------------------------------------------------------------- //

void RedAeroportuaria::buscarCaminos(const unsigned int &origen, const unsigned int &destino,
                                     const std::string &aerolinea, double &distancia,
                                     std::vector<bool> &visitado, std::pair<std::list<std::string>,
                                     double> &camino, std::list<std::pair<std::list<std::string>, double>> &listadoCaminos)
// Complejidad temporal: O(max{n*log n, n*k log k}), siendo n el número de aeropuertos y k, el número de aerolíneas. Esto
// debido a que recorrerá cada aeropuerto y, en cada recorrido, invocará a "existeReserva" y a "asientosReservados", cuya
// complejidad es la indicada.
// Complejidad espacial: O(max{n, k}), ya que utiliza dos estructuras lineales que: vuelo (su mapa interno), que en el
// peor de los casos almacenará tantos elementos como aerolíneas; y arcos, que en el peor de los casos almacenará tantos
// elementos como aeropuertos haya.
{
    Vuelo vuelo;
    unsigned int asientosReservados;

    visitado[origen] = true;
    camino.first.push_back(nombreAeropuerto(origen));

    if (origen == destino)
    {
        camino.second = distancia;
        listadoCaminos.push_back(camino);
    }
    else
    {
        std::list<Grafo<Vuelo>::Arco> arcos;
        redAeroportuaria.devolverAdyacentes(origen, arcos);                                  // O(n)

        for (const auto &arco : arcos)                                                       // O(n)
        {
            vuelo = arco.devolverCosto();                                                    // O(k log k)

            if (existeReserva(origen, destino, aerolinea))                                   // O(max{log n, log k})
            {
                asientosReservados = devolverAsientosReservados(origen, destino, aerolinea); // O(max{log n, log k})
            }
            else
                asientosReservados = 0;

            if (!(visitado[arco.devolverAdyacente()]))
                if ((vuelo.verAsientosTotales(aerolinea) - asientosReservados) > 0)
                {
                    distancia += vuelo.verDistancia();
                    buscarCaminos(arco.devolverAdyacente(), destino, aerolinea, distancia, visitado, camino, listadoCaminos);
                    distancia -= vuelo.verDistancia();
                }
        }
    }
    camino.first.pop_back();
    visitado[origen] = false;
}

// ----------------------------------------------------------------------------------------------------------------- //
//                                         Circuito Mínimo de Aeropuertos                                            //
// ----------------------------------------------------------------------------------------------------------------- //

void RedAeroportuaria::obtenerCircuitoMinimo(const unsigned int &origen, std::vector<unsigned int> &circuito, double &distancia)
// Complejidad temporal: O(n! * mk log k), siendo n el número de aeropuertos en la red Aeroporturia. Esto debido a que invoca a "calcularCircuitoMinimo".
// Complejidad espacial: O(n), ya que utiliza tres estructuras lineales para llevar un registro de qué vértices pertenecen al circuito,
// qué vertices hay en el circuito actual y cuáles, en el circuito mínimo.
{
    unsigned int indice = 0;
    double costoActual = 0, costoMinimo = INF;
    std::vector<unsigned int> circuitoActual(redAeroportuaria.devolverLongitud(), 0), circuitoMinimo(redAeroportuaria.devolverLongitud(), 0);
    std::vector<bool> perteneceCircuito(redAeroportuaria.devolverLongitud(), false);

    calcularCircuitoMinimo(origen, indice, circuitoActual, circuitoMinimo, perteneceCircuito, costoActual, costoMinimo); // O(n!)

    if (costoMinimo != INF)
    {
        distancia = costoMinimo;
        circuito = circuitoMinimo;
    }

    circuitoActual.clear();
    circuitoMinimo.clear();
}

// ----------------------------------------------------------------------------------------------------------------- //

void RedAeroportuaria::calcularCircuitoMinimo(const unsigned int &verticeInicio, const unsigned int &indice,
                                              std::vector<unsigned int> &circuito, std::vector<unsigned int> &circuitoMinimo,
                                              std::vector<bool> &perteneceCircuito, double &costoActual, double &costoMinimo)
// Complejidad temporal: O(n! * mk log k), siendo n el número de aeropuertos en la red Aeroportuaria.
// Complejidad espacial: O(n), ya que en cada iteración obtengo una lista con los adyacentes al vértice, que a lo sumo
// tendrá tantos elementos como aeropuertos - 1.
{
    circuito[indice] = verticeInicio + 1;
    perteneceCircuito[verticeInicio] = true;

    if (indice == redAeroportuaria.devolverLongitud() - 1) // Me pasé de la última posición del circuito
    {
        if (redAeroportuaria.existeArco(verticeInicio, circuito[0] - 1))                // O(log n)
        {
            Vuelo vuelo = redAeroportuaria.costoArco(verticeInicio, circuito[0] - 1);   // O(log n)
            costoActual += vuelo.verDistancia();
            if (costoActual < costoMinimo)
            {
                circuitoMinimo = circuito;                                              // O(n), el circuito mínimo se vuelve el guardado hasta el momento
                costoMinimo = costoActual;
            }
            costoActual -= vuelo.verDistancia();
        }
    }
    else
    {
        std::list<Grafo<Vuelo>::Arco> adyacentesVerticeInicio;
        redAeroportuaria.devolverAdyacentes(verticeInicio, adyacentesVerticeInicio);    // O(m), siendo m la cantidad de adyacentes del vértice de origen

        for (const auto &arco : adyacentesVerticeInicio)                                // O(m)
        {
            if (!perteneceCircuito[arco.devolverAdyacente()])
            {
                Vuelo vuelo = arco.devolverCosto();                                     // O(k log k), por el operador = de vuelo
                if (costoActual + vuelo.verDistancia() < costoMinimo)                   // O(1)
                {
                    costoActual += vuelo.verDistancia();
                    calcularCircuitoMinimo(arco.devolverAdyacente(), (indice + 1), circuito, circuitoMinimo, perteneceCircuito, costoActual, costoMinimo);
                    costoActual -= vuelo.verDistancia();
                }
            }
        }
    }

    circuito[indice] = 0;
    perteneceCircuito[verticeInicio] = false;
}
