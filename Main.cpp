// --------------------------------------------------------------------------------------------- //
/**                                  Directivas del Procesador                                  **/
// --------------------------------------------------------------------------------------------- //

#include <iostream>
#include <fstream>
#include <filesystem>
#include <Windows.h>

#include <set>
#include <map>
#include <list>
#include <vector>
#include <string>
#include "Vuelo.h"
#include "Grafo.h"
#include "Reserva.h"
#include "Colors.h"
#include "Aeropuerto.h"
#include "RedAeroportuaria.h"

using namespace std;

// --------------------------------------------------------------------------------------------- //
/**                                  Prototipos de Funciones                                    **/
// --------------------------------------------------------------------------------------------- //

void cargarAeropuertos(string path);
void cargarReservas(string path);
void cargarAeropuertos_rutas(string path);
void cargarRutas(string path);

void listarAeropuertos();
void listarAerolineas();

unsigned int seleccionarAeropuerto();
string seleccionarAerolinea();

void solicitarDatos(unsigned int &origen);
void solicitarDatos(unsigned int &origen, unsigned int &destino);
void solicitarDatos(unsigned int &origen, unsigned int &destino, string &aerolinea);

void eliminarArchivosAntiguos(const filesystem::path &carpeta);

void desplegarMenu();
void desplegarApartadoArchivos();

void mostrarArchivo(string path);

void generarListadoAeropuertos(string path);

void generarListadoReservas(string path);

void existeVueloDirecto();

void generarListadoVuelosMismaAerolinea(const unsigned int &origen, const unsigned int &destino);

void buscarCaminos(const unsigned int &origen, const unsigned int &destino, const string &aerolinea,
                   double &distancia, unsigned int &escalas, list<unsigned int> &camino, vector<bool> &visitado, ofstream &archivo);

void cargarCamino(const unsigned int &destino, const double &distancia, const unsigned int &escalas, const list<unsigned int> &camino, ofstream &archivo);

void mostrarCircuito(vector<unsigned int> circuito);

void inicializarVector(vector<unsigned int> &vectorSinLlenar, unsigned int n);

bool perteneceCircuito(unsigned int verticeActual, unsigned int indice, vector<unsigned int> circuito);

void circuitoHamiltoniano(const unsigned int &verticeInicio, const unsigned int &indice, vector<unsigned int> &circuito, vector<unsigned int> &circuitoMinimo, unsigned int &costoActual, unsigned int &costoMinimo);

// --------------------------------------------------------------------------------------------- //
/**                                     Variables Globales                                      **/
// --------------------------------------------------------------------------------------------- //

RedAeroportuaria redDeViajes;

// --------------------------------------------------------------------------------------------- //
/**                                     Programa Principal                                      **/
// --------------------------------------------------------------------------------------------- //

int main()
{
    SetConsoleOutputCP(CP_UTF8);

    // En cada inicialización del programa, se eliminan aquellos archivos viejos de antiguas ejecuciones.
    filesystem::path carpeta("outputs");
    eliminarArchivosAntiguos(carpeta);

    cargarAeropuertos("datasets/Aeropuertos.txt");
    cargarReservas("datasets/Reservas.txt");
    cargarRutas("datasets/Rutas.txt");

    desplegarMenu();

    return 0;
}

// --------------------------------------------------------------------------------------------- //
//                                     Carga de los Archivos                                     //
// --------------------------------------------------------------------------------------------- //

void cargarAeropuertos(string path)
{
    ifstream origen(path.c_str());
    if (!origen.is_open())
        cout << "No se pudo abrir el archivo: " << path << endl;
    else
    {
        while (!origen.eof())
        {
            string linea;
            getline(origen, linea);
            size_t primera_pos = linea.find(',');
            string nombre_aeropuerto = linea.substr(0, primera_pos);
            size_t segunda_pos = linea.find(',', primera_pos + 1);
            string nombre_ciudad = linea.substr(primera_pos + 1, segunda_pos - primera_pos - 1);
            string pais = linea.substr(segunda_pos + 1);

            redDeViajes.agregarAeropuerto(nombre_aeropuerto, nombre_ciudad, pais);
        }
    }
}

// --------------------------------------------------------------------------------------------- //

void cargarAerolineasRutas(string aerolineas, string origen, string destino, Vuelo &nuevoVuelo)
{
    aerolineas = aerolineas.substr(1);
    size_t inicial = 0;
    size_t pos = 0;
    size_t pos_final = aerolineas.find('}');
    while ((pos != std::string::npos) && (pos + 1 < pos_final))
    {
        pos = aerolineas.find(',', inicial);
        string dato_aerolinea = aerolineas.substr(inicial, pos - inicial);
        size_t pos_separador = dato_aerolinea.find('-');
        string aerolinea = dato_aerolinea.substr(0, pos_separador);
        string asientos_texto = dato_aerolinea.substr(pos_separador + 1);
        int asientos = atoi(asientos_texto.c_str());

        // Los dos puntos son porque hacemos referencia a la variable global aerolíneas y no, a la local.
        redDeViajes.agregarAerolinea(aerolinea);

        nuevoVuelo.agregarAerolinea(aerolinea, asientos);

        inicial = pos + 1;
    }

    redDeViajes.agregarRuta(origen, destino, nuevoVuelo);
}

// --------------------------------------------------------------------------------------------- //

void cargarRutas(string path)
{
    ifstream origen(path.c_str());
    if (!origen.is_open())
        cout << "No se pudo abrir el archivo: " << path << endl;
    else
    {
        bool cabotaje;
        while (!origen.eof())
        {
            string linea;
            getline(origen, linea);
            size_t primera_pos = linea.find(',');
            string nombre_aeropuerto_origen = linea.substr(0, primera_pos);

            size_t segunda_pos = linea.find(',', primera_pos + 1);
            string nombre_aeropuerto_destino = linea.substr(primera_pos + 1, segunda_pos - primera_pos - 1);

            size_t tercera_pos = linea.find(',', segunda_pos + 1);
            string distancia_texto = linea.substr(segunda_pos + 1, tercera_pos - segunda_pos - 1);
            double distancia;

            std::istringstream ss(distancia_texto);
            ss >> distancia;

            size_t cuarta_pos = linea.find(',', tercera_pos + 1);
            string cabotaje_texto = linea.substr(tercera_pos + 1, cuarta_pos - tercera_pos - 1);

            if (cabotaje_texto == "1")
                cabotaje = true;
            else
                cabotaje = false;

            Vuelo nuevoVuelo(cabotaje, distancia);

            string aerolineas = linea.substr(cuarta_pos + 1);

            cargarAerolineasRutas(aerolineas, nombre_aeropuerto_origen, nombre_aeropuerto_destino, nuevoVuelo);
        }
    }
}

// --------------------------------------------------------------------------------------------- //
//                                        Cargar Reservas                                        //
// --------------------------------------------------------------------------------------------- //

void cargarReservas(string path)
{
    ifstream origen(path.c_str());
    if (!origen.is_open())
        cout << "No se pudo abrir el archivo: " << path << endl;
    else
    {
        while (!origen.eof())
        {
            string linea;
            getline(origen, linea);
            size_t primera_pos = linea.find(',');
            string nombre_aeropuerto_origen = linea.substr(0, primera_pos);

            size_t segunda_pos = linea.find(',', primera_pos + 1);
            string nombre_aeropuerto_destino = linea.substr(primera_pos + 1, segunda_pos - primera_pos - 1);

            size_t tercera_pos = linea.find(',', segunda_pos + 1);
            string aerolinea = linea.substr(segunda_pos + 1, tercera_pos - segunda_pos - 1);

            string asientos_reservados_texto = linea.substr(tercera_pos + 1);
            int asientos_reservados = atoi(asientos_reservados_texto.c_str());

            redDeViajes.agregarReserva()
        }
    }
}

// --------------------------------------------------------------------------------------------- //
/*                                      Desplegar el Menú                                        */
// --------------------------------------------------------------------------------------------- //

void desplegarMenu()
{
    cout << CYAN << "¡Bienvenido a la red de viajes Joki!\n"
         << RESET << endl;
    char option;
    bool listadoGenerado = false;
    unsigned int origen, destino, indice = 0, costoActual = 0, costoMinimo = INF;
    vector<unsigned int> circuito, circuitoMinimo;
    do
    {
        cout << CYAN << "Estas son las opciones disponibles en nuestro sistema:\n"
             << RESET << endl
             << CYAN << "1. " << RESET << "Listar todos los aeropuertos" << endl
             << CYAN << "2. " << RESET << "Listar todas las reservas realizadas" << endl
             << CYAN << "3. " << RESET << "Verificar si existe un vuelo directo entre dos aeropuertos a través de una aerolínea" << endl
             << CYAN << "4. " << RESET << "Listar todos los vuelos posibles de un origen a un destino a través de una misma aerolínea" << endl
             << CYAN << "5. " << RESET << "Ver el circuito mínimo de aaeropuertos a partir de un aeropuerto de origen" << endl
             << CYAN << "6. " << RESET << "Ir al apartado de archivos" << endl
             << CYAN << "7. " << RESET << "Salir\n"
             << endl
             << CYAN << "Me gustaría: " << RESET;

        cin >> option;

        system("cls");

        switch (option)
        {
        case '1':
            if (!redDeViajes.estaVacia())
            {
                generarListadoAeropuertos("outputs/ListadoAeropuertos.txt");
                listadoGenerado = true;
            }
            else
                cout << RED << "No se hallaron aeropuertos en el sistema" << RESET << endl;
            break;
        case '2':
            if (redDeViajes.retornarNumeroReservas() > 0)
            {
                generarListadoReservas("outputs/ListadoReservas.txt");
                listadoGenerado = true;
            }
            else
                cout << RED << "No se hallaron reservas en nuestro sistema." << RESET << endl;
            break;
        case '3':
            existeVueloDirecto();
            break;
        case '4':
            solicitarDatos(origen, destino);
            system("cls");
            if (redDeViajes.existeVertice(origen) && redDeViajes.existeVertice(destino))
            {
                generarListadoVuelosMismaAerolinea(origen, destino);
                listadoGenerado = true;
            }
            else
                cout << RED << "Los aeropuertos indicados no han sido hallados en el sistema" << RESET << endl;
            break;
        case '5':
            solicitarDatos(origen);
            system("cls");
            redDeViajes.verCircuitoMinimo(origen);
            break;
        case '6':
            desplegarApartadoArchivos();
            break;
        case '7':
            break;
        default:
            cout << RED << "La opción ingresada no es válida. Revise lo ingresado.\n"
                 << RESET;
        }
        if (listadoGenerado)
        {
            cout << CYAN << "¡Listado generado! " << RESET << "\n\nPuede verlo en el "
                 << CYAN << "apartado de archivos" << RESET
                 << " del menú de opciones." << endl;
            listadoGenerado = false;
        }
        if ((option != '6' && option != '7'))
        {
            cout << RED << endl;
            system("pause");
            cout << RESET << endl;
            system("cls");
        }
    } while (option != '7');
    system("cls");
    cout << CYAN << "¡Gracias por utilizar el servicio!" << RESET << endl;
}

// --------------------------------------------------------------------------------------------- //

void eliminarArchivosAntiguos(const filesystem::path &carpeta)
{
    for (const auto &archivo : filesystem::directory_iterator(carpeta))
        filesystem::remove_all(archivo.path());
}

// --------------------------------------------------------------------------------------------- //

void desplegarApartadoArchivos()
{
    char option;
    unsigned int origen, destino;
    do
    {
        cout << CYAN << "Elija el archivo que desea visualizar:\n"
             << RESET << endl
             << CYAN << "1. " << RESET << "Listado de aeropuertos" << endl
             << CYAN << "2. " << RESET << "Listado de reservas" << endl
             << CYAN << "3. " << RESET << "Listado de vuelos de un origen a un destino a través de una misma aerolínea" << endl
             << CYAN << "4. " << RESET << "Volver al menú principal\n"
             << endl
             << CYAN << "Me gustaría visualizar el: " << RESET;

        cin >> option;

        system("cls");

        switch (option)
        {
        case '1':
            mostrarArchivo("outputs/ListadoAeropuertos.txt");
            break;
        case '2':
            mostrarArchivo("outputs/ListadoReservas.txt");
            break;
        case '3':
            solicitarDatos(origen, destino);
            system("cls");
            mostrarArchivo("outputs/VuelosMismaAerolinea_" + redDeViajes.nombreAeropuerto(origen) + "_" + redDeViajes.nombreAeropuerto(destino) + ".txt");
            break;
        case '4':
            break;
        default:
            cout << RED << "La opción ingresada no es válida. Revise lo ingresado.\n"
                 << RESET;
        }
        if (option != '4')
        {
            cout << RED << endl;
            system("pause");
            cout << RESET << endl;
            system("cls");
        }
    } while (option != '4');
}

// --------------------------------------------------------------------------------------------- //

void mostrarArchivo(string path)
{
    ifstream archivo(path.c_str());

    if (!archivo.is_open())
        cout << "No existe ningún archivo generado para la opción elegida." << endl;
    else
    {
        string linea;

        while (getline(archivo, linea))
            cout << linea << endl;
    }
}

// --------------------------------------------------------------------------------------------- //
/*                               Generar Listado de Aeropuertos                                  */
// --------------------------------------------------------------------------------------------- //

void generarListadoAeropuertos(string path)
{
    ofstream archivo;

    archivo.open(path.c_str(), fstream::out);

    archivo << "Aeropuertos en nuestro sistema:\n"
            << endl;

    list<RedAeroportuaria::Aeropuerto> aeropuertos;

    redDeViajes.listarAeropuertos(aeropuertos);

    for (const RedAeroportuaria::Aeropuerto &aeropuerto : aeropuertos)
    {
        archivo << "- " << (aeropuerto.verNombre()).c_str() << " (" << (aeropuerto.verCiudad()).c_str() << ", "
                << aeropuerto.verPais() << ")." << endl;
    }

    archivo.close();
}

// --------------------------------------------------------------------------------------------- //
/*                                       Listar las Reservas                                     */
// --------------------------------------------------------------------------------------------- //

void generarListadoReservas(string path)
{
    ofstream archivo;

    archivo.open(path.c_str(), fstream::out);

    list<RedAeroportuaria::Reserva> reservas;

    redDeViajes.listarReservas(reservas);

    if (!reservas.empty())
    {
        archivo << "Reservas registradas en el sistema: \n"
                << endl;

        for (const RedAeroportuaria::Reserva &reserva : reservas)
        {
            archivo << "- Vuelo de " << reserva.retornarAeropuertoOrigen() << " a " << reserva.retornarAeropuertoDestino()
                    << ", a través de la aerolínea " << reserva.retornarAeropuertoDestino() << " con un total de "
                    << reserva.retornarAsientosReservados() << "/" << reserva.retornarAsientosTotales()
                    << " asientos reservados." << endl;
        }
    }

    archivo.close();
}

// --------------------------------------------------------------------------------------------- //
/*                                        Solicitar Datos                                        */
// --------------------------------------------------------------------------------------------- //

void solicitarDatos(unsigned int &origen)
{
    listarAeropuertos();

    cout << CYAN << "\n- Seleccione un aeropuerto de origen: " << RESET;
    origen = seleccionarAeropuerto();
}

void solicitarDatos(unsigned int &origen, unsigned int &destino)
{
    listarAeropuertos();

    cout << CYAN << "\n- Seleccione un aeropuerto de origen: " << RESET;
    origen = seleccionarAeropuerto();

    cout << CYAN << "\n- Seleccione un aeropuerto de destino: " << RESET;
    destino = seleccionarAeropuerto();
}

void solicitarDatos(unsigned int &origen, unsigned int &destino, string &aerolinea)
{
    listarAeropuertos();

    cout << CYAN << "\n- Seleccione un aeropuerto de origen: " << RESET;
    origen = seleccionarAeropuerto();

    cout << CYAN << "\n- Seleccione un aeropuerto de destino: " << RESET;
    destino = seleccionarAeropuerto();

    system("cls");

    listarAerolineas();

    cout << CYAN << "\n- Seleccione una aerolínea: " << RESET;
    aerolinea = seleccionarAerolinea();

    system("cls");
}

// --------------------------------------------------------------------------------------------- //

void listarAeropuertos()
{
    unsigned int i = 1;
    list<RedAeroportuaria::Aeropuerto> aeropuertos;
    for (const auto &aeropuerto : aeropuertos)
    {
        cout << CYAN << i << ". " << RESET << aeropuerto.verNombre() << endl;
    }
}

// --------------------------------------------------------------------------------------------- //

void listarAerolineas()
{
    unsigned int index = 1;

    for (const string &aerolinea : aerolineas)
    {
        cout << CYAN << index << ". " << RESET << aerolinea << endl;
        index++;
    }
}

// --------------------------------------------------------------------------------------------- //

string seleccionarAerolinea()
{
    unsigned int index, element = 1;

    do
    {
        cin >> index;

        if (index > aerolineas.size())
        {
            // Mensaje de error
        }

    } while (index > aerolineas.size());

    for (const auto &aerolinea : aerolineas)
    {
        if (element == index)
            return aerolinea;
        element++;
    }
}

unsigned int seleccionarAeropuerto()
{
    unsigned int index;

    do
    {
        cin >> index;

        if (index > aeropuertos.size())
        {
            // Mensaje de error
        }

    } while (index > aeropuertos.size());

    return index - 1;
}

// --------------------------------------------------------------------------------------------- //
/*                           Verificar si Existe un Vuelo Directo                                */
// --------------------------------------------------------------------------------------------- //

void existeVueloDirecto()
{
    unsigned int origen, destino, asientosReservados;
    string nombreAeropuertoOrigen, nombreAeropuertoDestino, aerolinea;

    solicitarDatos(origen, destino, aerolinea);

    if (redDeViajes.existeVueloDirecto(origen, destino, aerolinea))
    {
        unsigned int distancia = redDeViajes.distanciaVuelo(origen, destino, aerolinea);
        unsigned int asientosDisponibles = redDeViajes.asientosDisponibles(origen, destino, aerolinea);

        cout << "¡Vuelo de " << CYAN << nombreAeropuertoOrigen << RESET << " a " << CYAN << nombreAeropuertoDestino << RESET
             << " a través de " << CYAN << aerolinea << RESET << " hallado! \n"
             << "\n- Distancia del vuelo: " << CYAN << distancia << " km \n"
             << RESET
             << "\n- Asientos disponibles: " << CYAN << asientosDisponibles << RESET << endl;
    }
    else
    {
        cout << RED << "No existe" << RESET << " un vuelo desde " << RED << nombreAeropuertoOrigen << RESET << " a " << RED << nombreAeropuertoDestino
             << RESET << " a través de " << RED << aerolinea << RESET << endl;
    }
}

// --------------------------------------------------------------------------------------------- //
/*                     Mostrar Vuelos a través de una Misma Aerolínea                            */
// --------------------------------------------------------------------------------------------- //

void generarListadoVuelosMismaAerolinea(const unsigned int &origen, const unsigned int &destino)
{
    // Declaro las variables necesarias.

    list<unsigned int> camino;

    vector<bool> visitado(aeropuertos.size(), false);

    double distancia = 0;

    unsigned int escalas = 0;

    ofstream archivo;

    archivo.open("outputs/VuelosMismaAerolinea_" + aeropuertos[origen].verNombre() + "_" + aeropuertos[destino].verNombre() + ".txt", fstream::out);

    archivo << "Vuelos del aeropuerto " << aeropuertos[origen].verNombre() << " al aeropuerto " << aeropuertos[destino].verNombre() << " a través de: \n"
            << endl;

    // Por cada aerolínea en la lista de aerolíneas...
    for (const auto &aerolinea : aerolineas)
    {
        archivo << "-----------------------------------------------------------------------------------------\n"
                << endl;

        archivo << "Aerolínea " << aerolinea << ": \n"
                << endl;

        buscarCaminos(origen, destino, aerolinea, distancia, escalas, camino, visitado, archivo);
    }

    archivo.close();
}

// ------------------------------------Búsqueda de Caminos-------------------------------------- //

void buscarCaminos(const unsigned int &origen, const unsigned int &destino, const string &aerolinea,
                   double &distancia, unsigned int &escalas, list<unsigned int> &camino, vector<bool> &visitado, ofstream &archivo)
{
    Vuelo vuelo;

    visitado[origen] = true;
    camino.push_back(origen);
    escalas++;

    unsigned int asientosReservados;
    string nombreOrigen = aeropuertos[origen].verNombre(), nombreDestino;

    // Si estoy en el destino...
    if (origen == destino)
        cargarCamino(destino, distancia, escalas, camino, archivo);
    else
    {
        list<Grafo<Vuelo>::Arco> arcos;
        redDeViajes.devolverAdyacentes(origen, arcos);

        for (const auto &arco : arcos)
        {
            vuelo = arco.devolverCosto();

            nombreDestino = aeropuertos[arco.devolverAdyacente()].verNombre();

            try
            {
                asientosReservados = reservas[nombreOrigen][nombreDestino].retornarAsientosReservados(aerolinea);
            }

            catch (invalid_argument &exc)
            {
                asientosReservados = 0;
            }

            if (!(visitado[arco.devolverAdyacente()]))
                if ((vuelo.verAsientosTotales(aerolinea) - asientosReservados) > 0)
                {
                    distancia += vuelo.verDistancia();
                    buscarCaminos(arco.devolverAdyacente(), destino, aerolinea, distancia, escalas, camino, visitado, archivo);
                    distancia -= vuelo.verDistancia();
                }
        }
    }
    camino.pop_back();
    escalas--;
    visitado[origen] = false;
}

// -------------------------------------Imprimir Camino----------------------------------------- //

void cargarCamino(const unsigned int &destino, const double &distancia, const unsigned int &escalas, const list<unsigned int> &camino, ofstream &archivo)
{
    for (const unsigned int &indice : camino)
    {
        archivo << aeropuertos[indice].verNombre();
        if (aeropuertos[indice].verNombre() != aeropuertos[destino].verNombre())
            archivo << " -> ";
    }

    if (escalas == 2)
        archivo << "\nVuelo directo. ";
    else if (escalas == 3)
        archivo << "\nVuelo con 1 escala. ";
    else
        archivo << "\nVuelo con " << (escalas - 2) << " escalas. ";

    archivo << "Distancia: " << distancia << " kilómetros."
            << "\n"
            << endl;
}

// -------------------------------------Circuito Aeropuertos----------------------------------------- //

void inicializarVector(vector<unsigned int> &vectorSinLlenar, unsigned int n)
{
    for (unsigned int i = 0; i < n; i++)
        vectorSinLlenar.push_back(0);
}

// --------------------------------------------------------------------------------------------- //

void mostrarCircuito(vector<unsigned int> circuito)
{
    for (const auto &vertice : circuito)
        cout << redDeViajes.nombreAeropuerto(vertice - 1) << endl;
    cout << redDeViajes.nombreAeropuerto(circuito[0] - 1) << endl;
}

// --------------------------------------------------------------------------------------------- //

bool perteneceCircuito(unsigned int verticeActual, unsigned int indice, vector<unsigned int> circuito)
{
    for (unsigned int i = 0; i < indice; i++)
    {
        if ((circuito[i] - 1) == verticeActual)
            return true;
    }
    return false;
}

// --------------------------------------------------------------------------------------------- //

void circuitoHamiltoniano(const unsigned int &verticeInicio, const unsigned int &indice, vector<unsigned int> &circuito, vector<unsigned int> &circuitoMinimo, unsigned int &costoActual, unsigned int &costoMinimo)
{
    circuito[indice] = verticeInicio + 1;

    if (indice == redDeViajes.devolverLongitud() - 1) // Me paasé de la última posición del circuito
    {
        bool existeArco = true;

        Vuelo vuelo;

        try
        {
            vuelo = redDeViajes.costoArco(verticeInicio, circuito[0] - 1);
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
        redDeViajes.devolverAdyacentes(verticeInicio, arcos);

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

void RedAeroportuaria::imprimirCircuitoMinimo()
{
    for (const auto &vertice : circuito)
        cout << aeropuertos[vertice - 1].verNombre() << endl;
    cout << aeropuertos[circuito[0] - 1].verNombre() << endl;
}