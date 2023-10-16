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

RedAeroportuaria::Vuelo::Vuelo(const Vuelo &otroVuelo) // O(k log k), debido a que invoca al operador =
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

RedAeroportuaria::Vuelo& RedAeroportuaria::Vuelo::operator=(const Vuelo &otroVuelo)
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

bool RedAeroportuaria::Vuelo::existeAerolinea(const std::string &aerolinea) const // O(log n)
{
    // Busco la aerolínea de la que quiero obtener los asientos libres.
    auto it = vuelos.find(aerolinea);

    // Retorno si logré obtener un iterador a ella.
    return (it != vuelos.end());
}

// ----------------------------------------------------------------------------------------------------------------- //

unsigned int RedAeroportuaria::Vuelo::verAsientosTotales(const std::string &aerolinea) const // O(log n)
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

void RedAeroportuaria::agregarReserva(const std::string &origen, const std::string &destino, const std::string &aerolinea, const unsigned int &asientosReservados)
{
    auto it_origen = reservas.find(origen);

    if (it_origen != reservas.end())
    {
        auto it_destino = (it_origen->second).find(destino);

        if (it_destino != (it_origen->second).end())
        {
            auto it_aerolinea = (it_destino->second).find(aerolinea); // Busco la aerolínea

            if (it_aerolinea == (it_destino->second).end()) // Si no está la aerolínea aún
            {
                (it_destino->second).insert({aerolinea, asientosReservados});
            }
        }
        else
        {
            std::map<std::string, unsigned int> nuevaAerolinea;
            nuevaAerolinea.insert({aerolinea, asientosReservados});

            (it_origen->second).insert({destino, nuevaAerolinea});
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

bool RedAeroportuaria::estaVacia() const
{
    return (redAeroportuaria.devolverLongitud() == 0);
}

// ----------------------------------------------------------------------------------------------------------------- //

bool RedAeroportuaria::existeAeropuerto(const unsigned int &aeropuerto) const
{
    return redAeroportuaria.existeVertice(aeropuerto);
}

// ----------------------------------------------------------------------------------------------------------------- //
//                                                Listar Aeropuertos                                                 //
// ----------------------------------------------------------------------------------------------------------------- //

void RedAeroportuaria::listarAeropuertos(std::list<Aeropuerto> &listaAeropuertos)
{
    for (const Aeropuerto &aeropuerto : aeropuertos)
        listaAeropuertos.push_back(aeropuerto);
}

// ----------------------------------------------------------------------------------------------------------------- //

unsigned int RedAeroportuaria::numeroAeropuertos() const
{
    return aeropuertos.size();
}

// ----------------------------------------------------------------------------------------------------------------- //

void RedAeroportuaria::listarAerolineas(std::list<std::string> &listaAerolineas)
{
    for (const auto &aerolinea : aerolineas)
        listaAerolineas.push_back(aerolinea);
}

// ----------------------------------------------------------------------------------------------------------------- //

unsigned int RedAeroportuaria::numeroAerolineas() const
{
    return aerolineas.size();
}

// ----------------------------------------------------------------------------------------------------------------- //
//                                                  Listar Reservas                                                  //
// ----------------------------------------------------------------------------------------------------------------- //

void RedAeroportuaria::listarReservas(std::list<Reserva> &listaReservas)
{
    Vuelo vuelo;
    std::list<Grafo<Vuelo>::Arco> arcos;
    std::list<std::string> aerolineas;
    unsigned int aeropuerto, asientosReservados;
    std::string aeropuertoOrigen, aeropuertoDestino;

    for (aeropuerto = 0; aeropuerto < numeroAeropuertos(); aeropuerto++)
    {
        // Busco los aeropuertos destinos a partir del origen.
        redAeroportuaria.devolverAdyacentes(aeropuerto, arcos);

        aeropuertoOrigen = aeropuertos[aeropuerto].verNombre();

        for (const auto &arco : arcos)
        {
            vuelo = arco.devolverCosto();

            vuelo.verAerolineas(aerolineas);

            aeropuertoDestino = nombreAeropuerto(arco.devolverAdyacente());

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

            // Vacío la lista de aerolíneas para darle paso a la próxima iteración.
            aerolineas.clear();
        }

        // Vacío la lista de arcos para dar paso a la siguiente iteración.
        arcos.clear();
    }
}

// ----------------------------------------------------------------------------------------------------------------- //

bool RedAeroportuaria::existeReserva(const unsigned int &aeropuertoOrigen, const unsigned int &aeropuertoDestino,
                                     const std::string & aerolinea) const
{
    bool reservaHallada = false;

    auto iteradorAeropuertoOrigen = reservas.find(nombreAeropuerto(aeropuertoOrigen));

    if (iteradorAeropuertoOrigen != reservas.end())
    {
        auto iteradorAeropuertoDestino = (iteradorAeropuertoOrigen->second).find(nombreAeropuerto(aeropuertoDestino));

        if (iteradorAeropuertoDestino != (iteradorAeropuertoOrigen->second).end())
        {
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

    if (redAeroportuaria.existeArco(aeropuertoOrigen, aeropuertoDestino)) // O(log n), siendo n el número de aeropuertos
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
// O(log n, log k), siendo n el número de aeropuertos y k, el número de aerolíneas
{
    unsigned int asientosReservados;
    std::string nombreAeropuertoOrigen, nombreAeropuertoDestino;

    nombreAeropuertoOrigen = aeropuertos[aeropuertoOrigen].verNombre();
    nombreAeropuertoDestino = aeropuertos[aeropuertoDestino].verNombre();

    auto it_origen = reservas.find(nombreAeropuertoOrigen);

    if (it_origen != reservas.end())
    {
        auto it_destino = (it_origen->second).find(nombreAeropuertoDestino);

        if (it_destino != (it_origen->second).end())
        {
            auto it_aerolinea = (it_destino->second).find(aerolinea);

            if (it_aerolinea != (it_destino->second).end())
            {
                asientosReservados = (it_aerolinea->second);
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

// -------------------------------------------------------------------------------------------------- //

unsigned int RedAeroportuaria::devolverAsientosDisponibles(const unsigned int &aeropuertoOrigen,
                                                           const unsigned int &aeropuertoDestino, const std::string &aerolinea) const
{
    unsigned int asientosDisponibles, asientosTotales, asientosReservados;

    Vuelo vuelo;

    if (redAeroportuaria.existeArco(aeropuertoOrigen, aeropuertoDestino)) // O(log n), siendo n el número de vértices.
    {
        Vuelo vuelo = redAeroportuaria.costoArco(aeropuertoOrigen, aeropuertoDestino); // O(k log k), siendo k el número de aerolíneas que posibilitan el viaje.
        asientosTotales = vuelo.verAsientosTotales(aerolinea); // O(1)

        if (existeReserva(aeropuertoOrigen, aeropuertoDestino, aerolinea))
        {
            asientosReservados = devolverAsientosReservados(aeropuertoOrigen, aeropuertoDestino, aerolinea);
        }
        else
            asientosReservados = 0;
    }
    else
        throw std::invalid_argument("No existe un arco entre el vértice de origen y el de destino");

    asientosDisponibles = asientosTotales - asientosReservados;
    return asientosDisponibles;
}

// -------------------------------------------------------------------------------------------------- //

double RedAeroportuaria::devolverDistancia(const unsigned int &aeropuertoOrigen, const unsigned int &aeropuertoDestino) const
{
    unsigned int distancia;

    if (redAeroportuaria.existeArco(aeropuertoOrigen, aeropuertoDestino))
    {
        Vuelo vuelo = redAeroportuaria.costoArco(aeropuertoOrigen, aeropuertoDestino);

        distancia = vuelo.verDistancia();
    }
    else
        throw std::invalid_argument("No existe un arco entre el vértice de origen y el de destino");

    return distancia;
}

// -------------------------------------------------------------------------------------------------- //
//                   Vuelos desde un Origen a un Destino sin cambiar de Aerolínea                     //
// -------------------------------------------------------------------------------------------------- //

void RedAeroportuaria::vuelosMismaAerolinea(const unsigned int &origen, const unsigned int &destino, std::map<std::string, std::list<std::pair<std::list<std::string>, double>>> &vuelos)
{
    double distancia = 0;
    std::vector<bool> visitado(aeropuertos.size(), false);

    // Por cada aerolínea en el conjunto de aerolíneas...
    for (const auto &aerolinea : aerolineas)
    {
        std::pair<std::list<std::string>, double> camino;
        std::list<std::pair<std::list<std::string>, double>> listadoCaminos;

        buscarCaminos(origen, destino, aerolinea, distancia, visitado, camino, listadoCaminos);

        vuelos.insert({aerolinea, listadoCaminos});
    }
}

// -------------------------------------------------------------------------------------------------- //

void RedAeroportuaria::buscarCaminos(const unsigned int &origen, const unsigned int &destino,
                                     const std::string &aerolinea, double &distancia,
                                     std::vector<bool> &visitado, std::pair<std::list<std::string>, double> &camino, std::list<std::pair<std::list<std::string>, double>> &listadoCaminos)
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
        redAeroportuaria.devolverAdyacentes(origen, arcos);

        for (const auto &arco : arcos)
        {
            vuelo = arco.devolverCosto();

            if (existeReserva(origen, destino, aerolinea))
            {
                asientosReservados = devolverAsientosReservados(origen, destino, aerolinea);
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

// -------------------------------------------------------------------------------------------------- //
//                                  Circuito Mínimo de Aeropuertos                                    //
// -------------------------------------------------------------------------------------------------- //

void RedAeroportuaria::obtenerCircuitoMinimo(const unsigned int &origen, std::vector<unsigned int> &circuito, double &distancia)
{
    unsigned int indice = 0;
    double costoActual = 0, costoMinimo = INF;
    std::vector<unsigned int> circuitoActual(redAeroportuaria.devolverLongitud(), 0), circuitoMinimo(redAeroportuaria.devolverLongitud(), 0);

    calcularCircuitoMinimo(origen, indice, circuitoActual, circuitoMinimo, costoActual, costoMinimo);

    if (costoMinimo != INF)
    {
        distancia = costoMinimo;
        circuito = circuitoMinimo;
    }

    circuitoActual.clear();
    circuitoMinimo.clear();
}

// -------------------------------------------------------------------------------------------------- //

void RedAeroportuaria::calcularCircuitoMinimo(const unsigned int &verticeInicio, const unsigned int &indice, std::vector<unsigned int> &circuito, std::vector<unsigned int> &circuitoMinimo, double &costoActual, double &costoMinimo)
{
    circuito[indice] = verticeInicio + 1;

    if (indice == redAeroportuaria.devolverLongitud() - 1) // Me paasé de la última posición del circuito
    {
        if (redAeroportuaria.existeArco(verticeInicio, circuito[0] - 1))
        {
            Vuelo vuelo = redAeroportuaria.costoArco(verticeInicio, circuito[0] - 1);
            costoActual += vuelo.verDistancia();
            if (costoActual < costoMinimo) // Condición de corte
            {
                circuitoMinimo = circuito; // El circuito mínimo se vuelve el guardado hasta el momento
                costoMinimo = costoActual; // El costo mínimo se actualiza
            }
            costoActual -= vuelo.verDistancia();
        }
    }
    else
    {
        std::list<Grafo<Vuelo>::Arco> adyacentesVerticeInicio;
        redAeroportuaria.devolverAdyacentes(verticeInicio, adyacentesVerticeInicio);

        for (const auto &arco : adyacentesVerticeInicio)
        {
            if (!perteneceCircuito(arco.devolverAdyacente(), indice + 1, circuito))
            {
                Vuelo vuelo = arco.devolverCosto();
                if (costoActual + vuelo.verDistancia() < costoMinimo)
                {
                    costoActual += vuelo.verDistancia();
                    calcularCircuitoMinimo(arco.devolverAdyacente(), (indice + 1), circuito, circuitoMinimo, costoActual, costoMinimo);
                    costoActual -= vuelo.verDistancia();
                }
            }
        }
    }

    circuito[indice] = 0;
}

// -------------------------------------------------------------------------------------------------- //

bool RedAeroportuaria::perteneceCircuito(const unsigned int &verticeActual, const unsigned int &indice, const std::vector<unsigned int> &circuito)
{
    unsigned int i = 0;
    bool elementoEncontrado = false;

    while (i < indice && !elementoEncontrado)
    {
        if ((circuito[i] - 1) == verticeActual)
            elementoEncontrado = true;
        i++;
    }

    return elementoEncontrado;
}
