// ----------------------------------------------------------------------------------------------------------------- //
//                                            Directivas del Procesador                                              //
// ----------------------------------------------------------------------------------------------------------------- //

#include <iostream>
#include <fstream>
#include <filesystem>
namespace fs = std::filesystem;
#include <limits>
#include <sstream>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include <set>
#include <map>
#include <list>
#include <vector>
#include "include/Colors.h"
#include "include/RedAeroportuaria.h"

using namespace std;

#ifdef _WIN32
#define CLEAR_SCREEN "cls"
void PAUSE()
{
    system("pause");
}
#else
#define CLEAR_SCREEN "clear"
void PAUSE()
{
    cout << "Press ENTER to continue..." << flush;
    // Se limpia el buffer de entrada para asegurar que no haya caracteres pendientes
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    cin.get(); // Se espera a que se presione enter.
}
#endif

// ----------------------------------------------------------------------------------------------------------------- //
//                                               Prototipos de Funciones                                             //
// ----------------------------------------------------------------------------------------------------------------- //

void cargarRutas(string path);
void cargarReservas(string path);
void cargarAeropuertos(string path);
void cargarAerolineasRutas(string aerolineas, string origen, string destino, RedAeroportuaria::Vuelo &nuevoVuelo);

void listarAerolineas();
void listarAeropuertos();

string seleccionarAerolinea();
unsigned int seleccionarAeropuerto();

string nombreAerolinea(const int &index);

void solicitarDatos(unsigned int &origen);
void solicitarDatos(unsigned int &origen, unsigned int &destino);
void solicitarDatos(unsigned int &origen, unsigned int &destino, string &aerolinea);

void desplegarMenu();
void desplegarApartadoArchivos();

void mostrarArchivo(string path);

void generarListadoReservas(string path);

void generarListadoAeropuertos(string path);

void eliminarArchivosAntiguos(const filesystem::path &carpeta);

void generarArchivoCircuito(const vector<unsigned int> &circuito, const double &distancia);

void generarListadoVuelosMismaAerolinea(const unsigned int &origen, const unsigned int &destino);

void existeVueloDirecto(const unsigned int &origen, const unsigned int &destino, const string &aerolinea);

void cargarCamino(const pair<list<string>, double> &recorrido, const unsigned int &destino, ofstream &archivo);

// ----------------------------------------------------------------------------------------------------------------- //
//                                               Variables Globales                                                  //
// ----------------------------------------------------------------------------------------------------------------- //

RedAeroportuaria redDeViajes;

// ----------------------------------------------------------------------------------------------------------------- //
//                                               Programa Principal                                                  //
// ----------------------------------------------------------------------------------------------------------------- //

int main()
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif

    // Elimino los archivos de ejecuciones anteriores
    if (!fs::exists("outputs"))
    {
        fs::create_directory("outputs");
    }
    fs::path carpeta("outputs");
    eliminarArchivosAntiguos(carpeta);

    cargarAeropuertos("datasets/AeropuertosS2.txt");
    cargarReservas("datasets/ReservasS2.txt");
    cargarRutas("datasets/RutasS2.txt"); // Rutas4 contiene un circuito hamiltoniano de 21 aeropuertos

    desplegarMenu();

    return 0;
}

// ----------------------------------------------------------------------------------------------------------------- //
//                                                 Carga de los Archivos                                             //
// ----------------------------------------------------------------------------------------------------------------- //

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

// ----------------------------------------------------------------------------------------------------------------- //

void cargarAerolineasRutas(string aerolineas, string origen, string destino, RedAeroportuaria::Vuelo &nuevoVuelo)
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

        // Agrego la aerolinea a la red de viajes.
        redDeViajes.agregarAerolinea(aerolinea);

        // Cargo el vuelo con las aerolineas que posibiitan el viaje y sus asientos totales.
        nuevoVuelo.agregarAerolinea(aerolinea, asientos);

        inicial = pos + 1;
    }

    // Agrego el vuelo a la red de viajes.
    redDeViajes.agregarRuta(origen, destino, nuevoVuelo);
}

// ----------------------------------------------------------------------------------------------------------------- //

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

            RedAeroportuaria::Vuelo nuevoVuelo(cabotaje, distancia);

            string aerolineas = linea.substr(cuarta_pos + 1);

            cargarAerolineasRutas(aerolineas, nombre_aeropuerto_origen, nombre_aeropuerto_destino, nuevoVuelo);
        }
    }
}

// ----------------------------------------------------------------------------------------------------------------- //

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

            redDeViajes.agregarReserva(nombre_aeropuerto_origen, nombre_aeropuerto_destino,
                                       aerolinea, asientos_reservados);
        }
    }
}

// ----------------------------------------------------------------------------------------------------------------- //

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

// ----------------------------------------------------------------------------------------------------------------- //

void eliminarArchivosAntiguos(const filesystem::path &carpeta)
{
    for (const auto &archivo : filesystem::directory_iterator(carpeta))
        filesystem::remove_all(archivo.path());
}

// ----------------------------------------------------------------------------------------------------------------- //
//                                                Menú Principal                                                     //
// ----------------------------------------------------------------------------------------------------------------- //

void desplegarMenu()
{
    cout << CYAN << "¡Bienvenido a la red de viajes Joki!\n"
         << RESET << endl;

    cout << "░░░░░░░░▒▒▒▒▒░░░░░▄█░░" << endl
         << "▒▒─▄▄▄▄▄▄▄▄▄▄▄▄▄▄███▄░" << endl
         << "░▄█▄██▄█▄█▄█▄█▄██▀▀▒░░" << endl
         << "░▒▒▒▒░░▀▀████▄─░░░▒▒▒░" << endl
         << "▒▒▒▒▒▒▒▒▒▒▒▀▀██▄▒▒▒▒▒▒\n"
         << endl;

    char option;
    bool listadoGenerado = false;
    unsigned int origen, destino;
    double distancia = 0;
    vector<unsigned int> circuito;
    string aerolinea;

    do
    {
        cout << CYAN << "Estas son las opciones disponibles en nuestro sistema:\n"
             << RESET << endl
             << CYAN << "1. " << RESET << "Listar todos los aeropuertos" << endl
             << CYAN << "2. " << RESET << "Listar todas las reservas realizadas" << endl
             << CYAN << "3. " << RESET << "Verificar si existe un vuelo directo entre dos aeropuertos a través de una aerolínea" << endl
             << CYAN << "4. " << RESET << "Listar todos los vuelos posibles de un origen a un destino a través de una misma aerolínea" << endl
             << CYAN << "5. " << RESET << "Ver el circuito mínimo de aeropuertos a partir de un aeropuerto de origen" << endl
             << CYAN << "6. " << RESET << "Ir al apartado de archivos" << endl
             << CYAN << "7. " << RESET << "Salir\n"
             << endl
             << CYAN << "Me gustaría: " << RESET;

        cin >> option;

        system(CLEAR_SCREEN);

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
            if (redDeViajes.numeroReservas() > 0)
            {
                generarListadoReservas("outputs/ListadoReservas.txt");
                listadoGenerado = true;
            }
            else
                cout << RED << "No se hallaron reservas en nuestro sistema." << RESET << endl;
            break;
        case '3':
            solicitarDatos(origen, destino, aerolinea);
            system(CLEAR_SCREEN);
            if (origen != destino)
                existeVueloDirecto(origen, destino, aerolinea);
            else
                cout << RED << "El origen y el destino no pueden ser los mismos.\n"
                     << RESET;
            break;
        case '4':
            solicitarDatos(origen, destino);
            system(CLEAR_SCREEN);
            if (origen != destino)
            {
                if (redDeViajes.existeAeropuerto(origen) && redDeViajes.existeAeropuerto(destino))
                {
                    generarListadoVuelosMismaAerolinea(origen, destino);
                    listadoGenerado = true;
                }
                else
                    cout << RED << "Los aeropuertos indicados no han sido hallados en el sistema" << RESET << endl;
            }
            else
                cout << RED << "El origen y el destino no pueden ser los mismos.\n"
                     << RESET;
            break;
        case '5':
            solicitarDatos(origen);
            system(CLEAR_SCREEN);
            cout << CYAN << "Calculando..." << RESET;
            redDeViajes.obtenerCircuitoMinimo(origen, circuito, distancia);
            system(CLEAR_SCREEN);
            if (!circuito.empty())
            {
                generarArchivoCircuito(circuito, distancia);
                circuito.clear();
                listadoGenerado = true;
            }
            else
                cout << RED << "No se halló ningún circuito en la red aeroportuaria" << RESET << endl;
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
            PAUSE();
            cout << RESET << endl;
            system(CLEAR_SCREEN);
        }
    } while (option != '7');
    system(CLEAR_SCREEN);
    cout << CYAN << "¡Gracias por utilizar el servicio! ¡Esperamos que tenga un excelente viaje!" << RESET << endl;
    cout << "\n░░░░░░░░░░█          " << endl
         << "░░░░░░░░▄▄█▄▄        " << endl
         << "░░░░▀▀▀██▀▀▀██▀▀▀    " << endl
         << "▄▄▄▄▄▄▄███████▄▄▄▄▄▄▄" << endl
         << "░░█▄█░░▀██▄██▀░░█▄█  " << endl;
}

// ----------------------------------------------------------------------------------------------------------------- //
//                                             Menú Archivos                                                         //
// ----------------------------------------------------------------------------------------------------------------- //

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
             << CYAN << "4. " << RESET << "Circuito de mínimo costo a partir de un aeropuerto de origen" << endl
             << CYAN << "5. " << RESET << "Volver al menú principal\n"
             << endl
             << CYAN << "Me gustaría visualizar el: " << RESET;

        cin >> option;

        system(CLEAR_SCREEN);

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
            system(CLEAR_SCREEN);
            mostrarArchivo("outputs/VuelosMismaAerolinea_" + redDeViajes.nombreAeropuerto(origen) + "_" + redDeViajes.nombreAeropuerto(destino) + ".txt");
            break;
        case '4':
            solicitarDatos(origen);
            system(CLEAR_SCREEN);
            mostrarArchivo("outputs/CircuitoMinimo_" + redDeViajes.nombreAeropuerto(origen) + ".txt");
            break;
        case '5':
            break;
        default:
            cout << RED << "La opción ingresada no es válida. Revise lo ingresado.\n"
                 << RESET;
        }
        if (option != '5')
        {
            cout << RED << endl;
            PAUSE();
            cout << RESET << endl;
            system(CLEAR_SCREEN);
        }
    } while (option != '5');
}

// ----------------------------------------------------------------------------------------------------------------- //
//                                        Generar Listado de Aeropuertos                                             //
// ----------------------------------------------------------------------------------------------------------------- //

void generarListadoAeropuertos(string path)
{
    ofstream archivo;

    archivo.open(path.c_str(), fstream::out);

    archivo << "Aeropuertos en nuestro sistema:\n"
            << endl;

    // Obtengo una lista con los aeropuertos de la red aeroportuaria.
    list<RedAeroportuaria::Aeropuerto> aeropuertos;
    redDeViajes.listarAeropuertos(aeropuertos);

    // Recorro la lista a medida que cargo los aeropuertos al archivo.
    for (const RedAeroportuaria::Aeropuerto &aeropuerto : aeropuertos)
    {
        archivo << "- " << (aeropuerto.verNombre()).c_str() << " (" << (aeropuerto.verCiudad()).c_str() << ", "
                << aeropuerto.verPais() << ")." << endl;
    }

    archivo.close();
}

// ----------------------------------------------------------------------------------------------------------------- //
//                                          Generar Listado de Reservas                                              //
// ----------------------------------------------------------------------------------------------------------------- //

void generarListadoReservas(string path)
{
    ofstream archivo;

    archivo.open(path.c_str(), fstream::out);

    // Se obtiene un listado con las reservas de la red aeroportuaria.
    list<RedAeroportuaria::Reserva> reservas;
    redDeViajes.listarReservas(reservas);

    archivo << "Reservas registradas en el sistema: \n"
            << endl;

    // Se realiza la carga de las reservas al archivo a medida que se recorre el listado.
    for (const RedAeroportuaria::Reserva &reserva : reservas)
    {
        archivo << "- Vuelo de " << reserva.retornarOrigen() << " a " << reserva.retornarDestino()
                << ", a través de la aerolínea " << reserva.retornarAerolinea() << " con un total de "
                << reserva.retornarAsientosReservados() << "/" << reserva.retornarAsientosTotales()
                << " asientos reservados." << endl;
    }

    archivo.close();
}

// ----------------------------------------------------------------------------------------------------------------- //
//                                      Verificar si Existe un Vuelo Directo                                         //
// ----------------------------------------------------------------------------------------------------------------- //

void existeVueloDirecto(const unsigned int &origen, const unsigned int &destino, const string &aerolinea)
{
    if (redDeViajes.existeVueloDirecto(origen, destino, aerolinea))
    {
        double distancia = redDeViajes.devolverDistancia(origen, destino);
        unsigned int asientosDisponibles = redDeViajes.devolverAsientosDisponibles(origen, destino, aerolinea);

        cout << "¡Vuelo de " << CYAN << redDeViajes.nombreAeropuerto(origen) << RESET << " a " << CYAN
             << redDeViajes.nombreAeropuerto(destino) << RESET << " a través de " << CYAN << aerolinea << RESET
             << " hallado! \n"
             << "\n- Distancia del vuelo: " << CYAN << distancia << " km \n"
             << RESET << "\n- Asientos disponibles: " << CYAN << asientosDisponibles << RESET << endl;
    }
    else
    {
        cout << RED << "No existe" << RESET << " un vuelo desde " << RED << redDeViajes.nombreAeropuerto(origen)
             << RESET << " a " << RED << redDeViajes.nombreAeropuerto(destino) << RESET << " a través de "
             << RED << aerolinea << RESET << endl;
    }
}

// ----------------------------------------------------------------------------------------------------------------- //
//                                 Mostrar Vuelos a través de una Misma Aerolínea                                    //
// ----------------------------------------------------------------------------------------------------------------- //

void generarListadoVuelosMismaAerolinea(const unsigned int &origen, const unsigned int &destino)
{
    map<string, list<pair<list<string>, double>>> vuelos;

    // Obtengo los vuelos de un aeropuerto de origen a uno de destino a través de cada aerolínea.
    redDeViajes.vuelosMismaAerolinea(origen, destino, vuelos);

    ofstream archivo;

    archivo.open("outputs/VuelosMismaAerolinea_" + redDeViajes.nombreAeropuerto(origen) + "_" + redDeViajes.nombreAeropuerto(destino) + ".txt", fstream::out);

    archivo << "Vuelos del aeropuerto " << redDeViajes.nombreAeropuerto(origen) << " al aeropuerto " << redDeViajes.nombreAeropuerto(destino) << " a través de: \n"
            << endl;

    // Por cada aerolinea...
    for (const auto &aerolinea : vuelos)
    {
        archivo << "-----------------------------------------------------------------------------------------\n"
                << endl;

        archivo << "Aerolínea " << aerolinea.first << ": \n"
                << endl;

        // Por cada recorrido...
        for (const auto &recorrido : aerolinea.second)
            cargarCamino(recorrido, destino, archivo);
    }

    archivo.close();
}

// ----------------------------------------------------------------------------------------------------------------- //

void cargarCamino(const pair<list<string>, double> &recorrido, const unsigned int &destino, ofstream &archivo)
{
    unsigned int escalas = 0;

    // Se carga cada aeropuerto del recorrido en el archivo.
    for (const string &aeropuerto : recorrido.first)
    {
        archivo << aeropuerto;
        if (aeropuerto != redDeViajes.nombreAeropuerto(destino))
            archivo << " -> ";
    }

    escalas = recorrido.first.size();

    if (escalas == 2)
        archivo << "\nVuelo directo. ";
    else if (escalas == 3)
        archivo << "\nVuelo con 1 escala. ";
    else
        archivo << "\nVuelo con " << (escalas - 2) << " escalas. ";

    archivo << "Distancia: " << recorrido.second << " kilómetros."
            << "\n"
            << endl;
}

// ----------------------------------------------------------------------------------------------------------------- //
//                                        Circuito Mínimo de Aeropuertos                                             //
// ----------------------------------------------------------------------------------------------------------------- //

void generarArchivoCircuito(const vector<unsigned int> &circuito, const double &distancia)
{
    ofstream archivo;

    archivo.open("outputs/CircuitoMinimo_" + redDeViajes.nombreAeropuerto(circuito[0] - 1) + ".txt", fstream::out);

    archivo << "Circuito de mínimo costo con origen en " << redDeViajes.nombreAeropuerto(circuito[0] - 1) << ": \n\n";

    // Se realiza la carga de cada aeropuerto en el circuito en el archivo.
    for (const auto &aeropuerto : circuito)
        archivo << redDeViajes.nombreAeropuerto(aeropuerto - 1) << " -> \n";
    archivo << redDeViajes.nombreAeropuerto(circuito[0] - 1) << endl;

    archivo << "\nDistancia a recorrer: " << distancia << " kilómetros" << endl;

    archivo.close();
}

// ----------------------------------------------------------------------------------------------------------------- //
//                                     Solicitar Datos y Funciones Auxiliares                                        //
// ----------------------------------------------------------------------------------------------------------------- //

void solicitarDatos(unsigned int &origen)
{
    listarAeropuertos();

    cout << CYAN << "\n- Seleccione un aeropuerto de origen: " << RESET;
    origen = seleccionarAeropuerto();
}

// ----------------------------------------------------------------------------------------------------------------- //

void solicitarDatos(unsigned int &origen, unsigned int &destino)
{
    listarAeropuertos();

    cout << CYAN << "\n- Seleccione un aeropuerto de origen: " << RESET;
    origen = seleccionarAeropuerto();

    cout << CYAN << "\n- Seleccione un aeropuerto de destino: " << RESET;
    destino = seleccionarAeropuerto();
}

// ----------------------------------------------------------------------------------------------------------------- //

void solicitarDatos(unsigned int &origen, unsigned int &destino, string &aerolinea)
{
    listarAeropuertos();

    cout << CYAN << "\n- Seleccione un aeropuerto de origen: " << RESET;
    origen = seleccionarAeropuerto();

    cout << CYAN << "\n- Seleccione un aeropuerto de destino: " << RESET;
    destino = seleccionarAeropuerto();

    system(CLEAR_SCREEN);

    listarAerolineas();

    cout << CYAN << "\n- Seleccione una aerolínea: " << RESET;
    aerolinea = seleccionarAerolinea();

    system(CLEAR_SCREEN);
}

// ----------------------------------------------------------------------------------------------------------------- //

void listarAeropuertos()
{
    unsigned int i = 1;
    list<RedAeroportuaria::Aeropuerto> aeropuertos;

    redDeViajes.listarAeropuertos(aeropuertos);

    for (const auto &aeropuerto : aeropuertos)
    {
        cout << CYAN << i << ". " << RESET << aeropuerto.verNombre() << endl;
        i++;
    }
}

// ----------------------------------------------------------------------------------------------------------------- //

void listarAerolineas()
{
    unsigned int index = 1;
    list<string> aerolineas;

    redDeViajes.listarAerolineas(aerolineas);

    for (const string &aerolinea : aerolineas)
    {
        cout << CYAN << index << ". " << RESET << aerolinea << endl;
        index++;
    }
}

// ----------------------------------------------------------------------------------------------------------------- //

string nombreAerolinea(const int &index)
{
    int indice = 1;

    list<string> aerolineas;
    redDeViajes.listarAerolineas(aerolineas);

    for (const auto &aerolinea : aerolineas)
    {
        if (indice == index)
            return aerolinea;
        indice++;
    }

    throw out_of_range("Ocurrió un error inesperado");
}

// ----------------------------------------------------------------------------------------------------------------- //

string seleccionarAerolinea()
{
    int index;

    do
    {
        cin >> index;

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cout << CYAN << "\nEntrada inválida. Lo ingresado no es un número. Por favor, revise la entrada y vuelva a intentarlo: " << RESET;
        }
        else if (index <= 0 || index > int(redDeViajes.numeroAerolineas()))
        {
            cout << CYAN << "\nEl número ingresado no le corresponde a ningún aerolínea. Por favor, revise la entrada y vuelva a intentarlo: " << RESET;
        }

    } while (index <= 0 || index > int(redDeViajes.numeroAerolineas()));

    return nombreAerolinea(index);
}

// ----------------------------------------------------------------------------------------------------------------- //

unsigned int seleccionarAeropuerto()
{
    int index;

    do
    {
        cin >> index;

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cout << CYAN << "\nEntrada inválida. Lo ingresado no es un número. Por favor, revise la entrada y vuelva a intentarlo: " << RESET;
        }
        else if (index <= 0 || index > int(redDeViajes.numeroAeropuertos()))
        {
            cout << CYAN << "\nEl número ingresado no le corresponde a ningún aeropuerto. Por favor, revise la entrada y vuelva a intentarlo: " << RESET;
        }

    } while (index <= 0 || index > int(redDeViajes.numeroAeropuertos()));

    return index - 1;
}
