#include "RedAeroportuaria.h"

// ----------------------------------------------RESERVAS------------------------------------------------- //

RedAeroportuaria::Reserva::Reserva()
{
}

RedAeroportuaria::Reserva::Reserva(const std::string &origen, const std::string &destino, const std::string &aerolinea, const unsigned int &asientosResevados, const unsigned int &asientosTotales)
{
    this->aeropuertoOrigen = origen;
    this->aeropuertoDestino = destino;
    this->aerolinea = aerolinea;
    this->asientosReservados = asientosReservados;
    this->asientosTotales = asientosTotales;
}

std::string RedAeroportuaria::Reserva::retornarOrigen() const
{
    return aeropuertoOrigen;
}

std::string RedAeroportuaria::Reserva::retornarDestino() const
{
    return aeropuertoDestino;
}

std::string RedAeroportuaria::Reserva::retornarAerolinea() const
{
    return aerolinea;
}

unsigned int RedAeroportuaria::Reserva::retornarAsientosReservados() const
{
    return asientosReservados;
}

RedAeroportuaria::Reserva::unsigned int retornarAsientosTotales() const
{
    return asientosTotales;
}

RedAeroportuaria::Reserva::~Reserva()
{
}

// ----------------------------------------------AEROPUERTO----------------------------------------------- //

RedAeroportuaria::Aeropuerto::Aeropuerto()
{
}

RedAeroportuaria::Aeropuerto::Aeropuerto(const std::string &nombre, const std::string &ciudad, const std::string &pais)
{
    // Cargo los datos del aeropuerto.
    this->nombre = nombre;
    this->ciudad = ciudad;
    this->pais = pais;
}

// --------------------------------------------------------------------------------------------- //

RedAeroportuaria::Aeropuerto::~Aeropuerto()
{
}

// --------------------------------------------------------------------------------------------- //

std::string RedAeroportuaria::Aeropuerto::verNombre() const
{
    return nombre;
}

// --------------------------------------------------------------------------------------------- //

std::string RedAeroportuaria::Aeropuerto::verCiudad() const
{
    return ciudad;
}

// --------------------------------------------------------------------------------------------- //

std::string RedAeroportuaria::Aeropuerto::verPais() const
{
    return pais;
}

std::string RedAeroportuaria::nombreAeropuerto(const unsigned int &identificador)
{
    return aeropuertos[identificador].verNombre();
}

void RedAeroportuaria::agregarAeropuerto(const std::string &nombre, const std::string &ciudad, const std::string &pais)
{
    Aeropuerto nuevoAeropuerto(nombre_aeropuerto, nombre_ciudad, pais);

    redAeroportuaria.agregarVertice(aeropuertos.size());

    aeropuertos.push_back(nuevoAeropuerto);
}

void RedAeroportuaria::agregarRuta(const std::string &origen, const std::string &destino, Vuelo &nuevoVuelo)
{
    unsigned int claveOrigen, claveDestino;

    // Debido a que nuestro grafo trabaja con enteros, debemos recorrer el mapa de aeropuertos
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

void RedAeroportuaria::agregarReserva(const string &origen, const string &destino, const string &aerolinea, const unsigned int &asientosReservados)
{
    Reserva nuevaReserva;
    nuevaReserva.agregarReserva(aerolinea, asientos_reservados);

    auto it_origen = reservas.find(nombre_aeropuerto_origen);

    if (it_origen != reservas.end())
    {
        auto it_destino = (it_origen->second).find(nombre_aeropuerto_destino);

        if (it_destino != (it_origen->second).end())
        {
            (it_destino->second).agregarReserva(aerolinea, asientos_reservados);
        }
        else
        {
            (it_origen->second).insert({nombre_aeropuerto_destino, nuevaReserva});
        }
    }
    else
    {
        map<string, Reserva> nuevo_mapa;
        nuevo_mapa.insert({nombre_aeropuerto_destino, nuevaReserva});
        reservas.insert({nombre_aeropuerto_origen, nuevo_mapa});
    }

    numeroReservas++;
}

void RedAeroportuaria::agregarAerolinea(const string &aerolinea, const unsigned int &asientosTotales)
{
    aerolineas.insert({aerolinea}); // O(log n)
}

bool RedAeroportuaria::estaVacia() const
{
    return redAeroportuaria.devolverLongitud();
}

unsigned int RedAeroportuaria::retornarNumeroReservas() const
{
    return numeroReservas;
}

// -------------------------------------------------------------------------------------------------- //
//                                          Listar Reservas                                           //
// -------------------------------------------------------------------------------------------------- //

void RedAeroportuaria::generarListadoAeropuertos()
{
    Vuelo vuelo;
    list<Grafo<Vuelo>::Arco> arcos;
    list<string> aerolineas;
    unsigned int numeroAeropuerto = 0, asientosReservados;
    string aeropuertoOrigen, aeropuertoDestino;

    ofstream archivo;

    archivo.open(path.c_str(), fstream::out);

    archivo << "Reservas registradas en el sistema: \n"
            << endl;

    for (const auto &aeropuerto : aeropuertos)
    {
        // Busco los aeropuertos destinos a partir del origen.
        redDeViajes.devolverAdyacentes(numeroAeropuerto, arcos);

        aeropuertoOrigen = aeropuertos[numeroAeropuerto].verNombre();

        for (const auto &arco : arcos)
        {
            vuelo = arco.devolverCosto();

            vuelo.verAerolineas(aerolineas);

            aeropuertoDestino = aeropuertos[arco.devolverAdyacente()].verNombre();

            for (string aerolinea : aerolineas)
            {
                try
                {
                    asientosReservados = reservas[aeropuertoOrigen][aeropuertoDestino].retornarAsientosReservados(aerolinea);
                }

                catch (invalid_argument &exc)
                {
                    asientosReservados = 0;
                }

                // Si hay reservas realizadas...
                if (asientosReservados > 0)
                {
                    archivo << "- Vuelo de " << aeropuertoOrigen << " a " << aeropuertoDestino
                            << ", a través de la aerolínea " << aerolinea << " con un total de "
                            << asientosReservados << "/" << vuelo.verAsientosTotales(aerolinea)
                            << " asientos reservados." << endl;
                }
            }

            // Vacío la lista de aerolíneas para darle paso a la próxima iteración.
            aerolineas.clear();
        }

        // Vacío la lista de arcos para dar paso a la siguiente iteración.
        arcos.clear();

        numeroAeropuerto++;
    }

    archivo.close();
}

void RedAeroportuaria::listarAeropuertos(list<Aeropuerto> &listaAeropuertos)
{
    for (const Aeropuerto &aeropuerto : aeropuertos)
        listaAeropuertos.push_back(aeropuerto);
}

unsigned int RedAeroportuaria::numeroAeropuertos() const
{
    return aeropuertos.size();
}

// -------------------------------------------------------------------------------------------------- //
//                                          Listar Reservas                                           //
// -------------------------------------------------------------------------------------------------- //

void RedAeroportuaria::listarReservas(list<Reserva> &listaReservas)
{
    Vuelo vuelo;
    list<Grafo<Vuelo>::Arco> arcos;
    list<string> aerolineas;
    unsigned int nroAeropuerto = 0, asientosReservados;
    string aeropuertoOrigen, aeropuertoDestino;

    for (const auto &aeropuerto : aeropuertos)
    {
        // Busco los aeropuertos destinos a partir del origen.
        redAeroportuaria.devolverAdyacentes(nroAeropuerto, arcos);

        aeropuertoOrigen = aeropuertos[nroAeropuerto].verNombre();

        for (const auto &arco : arcos)
        {
            vuelo = arco.devolverCosto();

            vuelo.verAerolineas(aerolineas);

            aeropuertoDestino = aeropuertos[arco.devolverAdyacente()].verNombre();

            for (string aerolinea : aerolineas)
            {
                try
                {
                    asientosReservados = reservas[aeropuertoOrigen][aeropuertoDestino][aerolinea];
                }

                catch (invalid_argument &exc)
                {
                    asientosReservados = 0;
                }

                // Si hay reservas realizadas...
                if (asientosReservados > 0)
                {
                    Reserva nuevaReserva(aeropuertoOrigen, aeropuertoDestino, aerolinea, asientosReservados, vuelo.verAsientosTotales(aerolinea));

                    listaReservas.push_back(nuevaReserva);
                }
            }

            // Vacío la lista de aerolíneas para darle paso a la próxima iteración.
            aerolineas.clear();
        }

        // Vacío la lista de arcos para dar paso a la siguiente iteración.
        arcos.clear();

        nroAeropuerto++;
    }
}

// -------------------------------------------------------------------------------------------------- //
//                              Verificar si existe un vuelo directo                                  //
// -------------------------------------------------------------------------------------------------- //

bool RedAeroportuaria::existeVueloDirecto(const unsigned int &aeropuertoOrigen, const unsigned int &aeropuertoDestino, const std::string &aerolinea) const
{
    unsigned int asientosReservados;
    string nombreAeropuertoOrigen, nombreAeropuertoDestino;

    nombreAeropuertoOrigen = aeropuertos[aeropuertoOrigen].verNombre();
    nombreAeropuertoDestino = aeropuertos[aeropuertoDestino].verNombre();

    list<Grafo<Vuelo>::Arco> adyacentes;

    redAeroportuaria.devolverAdyacentes(aeropuertoOrigen, adyacentes);

    auto it = adyacentes.begin();

    double distancia;
    unsigned int asientos_disponibles;

    while ((it != adyacentes.end()) && ((it->devolverAdyacente()) != aeropuertoDestino))
        it++;

    if ((it != adyacentes.end()) && (it->devolverAdyacente() == aeropuertoDestino))
    {
        Vuelo vuelo = it->devolverCosto();

        if (vuelo.existeAerolinea(aerolinea))
        {
            try
            {
                asientosReservados = reservas[nombreAeropuertoOrigen][nombreAeropuertoDestino].retornarAsientosReservados(aerolinea);
            }

            catch (invalid_argument &exc)
            {
                asientosReservados = 0;
            }

            cout << "¡Vuelo de " << CYAN << nombreAeropuertoOrigen << RESET << " a " << CYAN << nombreAeropuertoDestino << RESET
                 << " a través de " << CYAN << aerolinea << RESET << " hallado! \n"
                 << "\n- Distancia del vuelo: " << CYAN << vuelo.verDistancia() << " km \n"
                 << RESET
                 << "\n- Asientos disponibles: " << CYAN << vuelo.verAsientosTotales(aerolinea) - asientosReservados << RESET << endl;
        }
        else
        {
            cout << RED << "No existe" << RESET << " un vuelo desde " << RED << nombreAeropuertoOrigen << RESET << " a " << RED << nombreAeropuertoDestino
                 << RESET << " a través de " << RED << aerolinea << RESET << endl;
        }
    }
    else
        cout << RED << "No existe" << RESET << " un vuelo desde " << RED << nombreAeropuertoOrigen << RESET << " a " << RED << nombreAeropuertoDestino
             << RESET << " a través de " << RED << aerolinea << RESET << endl;
}

unsigned int RedAeroportuaria::devolverAsientosDisponibles(const unsigned int &aeropuertoOrigen, const unsigned int &aeropuertoDestino, const std::string &aerolinea)
{
    unsigned int asientosDisponible;

    try
    {
        asientosDisponible = reservas[nombreAeropuertoOrigen][nombreAeropuertoDestino].retornarAsientosReservados(aerolinea);
    }

    catch (invalid_argument &exc)
    {
        asientosReservados = 0;
    }
}

unsigned int RedAeroportuaria::devolverDistancia(const unsigned int &aeropuertoOrigen, const unsigned int &aeropuertoDestino, const std::string &aerolinea)
{
    Vuelo vuelo;

    vuelo = de
}

// -------------------------------------------------------------------------------------------------- //
//                                      Circuito Aeropuertos                                          //
// -------------------------------------------------------------------------------------------------- //

// Ver circuito mínimo
void RedAeroportuaria::verCircuitoMinimo(const string &nombre)
{
    unsigned int indice = 0, costoActual = 0, costoMinimo = INF;
    vector<unsigned int> circuito(redAeroportuaria.devolverLongitud(), 0), circuitoMinimo(redAeroportuaria.devolverLongitud(), 0);

    circuitoHamiltoniano(origen, indice, circuito, circuitoMinimo, costoActual, costoMinimo);

    if (costoMinimo != INF)
    {
        cout << "El circuito mínimo con origen en " << CYAN << aeropuertos[origen].verNombre() << RESET << " es:\n\n";
        mostrarCircuito(circuitoMinimo);
        cout << "\nCon una distancia de: " << CYAN << costoMinimo << " kilómetros" << RESET << endl;
    }
    else
    {
        cout << RED << "No fue posible hallar ningún circuito en la red de viajes con origen en " << RESET << aeropuertos[origen].verNombre() << endl;
    }

    imprimirCircuitoMinimo(circuitoMinimo);

    circuito.clear();
    circuitoMinimo.clear();
    costoActual = 0;
    costoMinimo = INF;
    indice = 0;
}

// Obtener circuito mínimo
void RedAeroportuaria::obtenerCircuitoMinimo(const unsigned int &verticeInicio, const unsigned int &indice, vector<unsigned int> &circuito, vector<unsigned int> &circuitoMinimo, unsigned int &costoActual, unsigned int &costoMinimo)
{
    circuito[indice] = verticeInicio + 1;

    if (indice == redAeroportuaria.devolverLongitud() - 1) // Me paasé de la última posición del circuito
    {
        bool existeArco = true;

        Vuelo vuelo;

        try
        {
            vuelo = redAeroportuaria.costoArco(verticeInicio, circuito[0] - 1);
        }

        catch (const invalid_argument &exc)
        {
            existeArco = false;
        }

        if (existeArco)
        {
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
        list<Grafo<Vuelo>::Arco> arcos;
        redAeroportuaria.devolverAdyacentes(verticeInicio, arcos);

        for (const auto &arco : arcos) // Por cada adyacente al vértice
        {
            if (!perteneceCircuito(arco.devolverAdyacente(), indice + 1, circuito))
            {
                Vuelo vuelo = arco.devolverCosto();
                if (costoActual + vuelo.verDistancia() < costoMinimo)
                {
                    costoActual += vuelo.verDistancia();
                    circuitoHamiltoniano(arco.devolverAdyacente(), (indice + 1), circuito, circuitoMinimo, costoActual, costoMinimo);
                    costoActual -= vuelo.verDistancia();
                }
            }
        }
    }

    circuito[indice] = 0;
}

// Pertenece al
bool RedAeroportuaria::perteneceCircuito(const unsigned int &verticeActual, const unsigned int &indice, const vector<unsigned int> &circuito)
{
    for (unsigned int i = 0; i < indice; i++)
    {
        if ((circuito[i] - 1) == verticeActual)
            return true;
    }
    return false;
}

void RedAeroportuaria::imprimirCircuitoMinimo()
{
    for (const auto &vertice : circuito)
        cout << aeropuertos[vertice - 1].verNombre() << endl;
    cout << aeropuertos[circuito[0] - 1].verNombre() << endl;
}
