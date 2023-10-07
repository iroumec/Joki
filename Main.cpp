#include <iostream>
#include <fstream>
#include <filesystem>
#include <locale>

#include <set>
#include <map>
#include <list>
#include <vector>
#include <string.h>
#include "Vuelo.h"
#include "Grafo.h"
#include "Reserva.h"
#include "Colors.h"
#include "Aeropuerto.h"

using namespace std;

// --------------------------------------------------------------------------------------------- //
/**                                  Prototipos de Funciones                                     **/
// --------------------------------------------------------------------------------------------- //

void cargarAeropuertos(string path);
void cargarReservas(string path);
void cargarAeropuertos_rutas(string path);
void cargarRutas(string path);

void listarAeropuertos();
void listarAerolineas();

unsigned int seleccionarAeropuerto();
string seleccionarAerolinea();

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

// --------------------------------------------------------------------------------------------- //
/**                                     Variables Globales                                      **/
// --------------------------------------------------------------------------------------------- //

unsigned int numeroReservas = 0;

map<string, map<string, Reserva>> reservas; // Mapa de mapas con las reservas

set<string> aerolineas; // Conjunto con todas las aerolíneas de la red de viaje.

vector<Aeropuerto> aeropuertos; // Declaro una estructura que me permita conectar la información del aeropuerto con un �ndice.

Grafo<Vuelo> redDeViajes;

// --------------------------------------------------------------------------------------------- //
/**                                     Programa Principal                                      **/
// --------------------------------------------------------------------------------------------- //

int main()
{
    setlocale(LC_ALL, "");

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
/**                               Implementaci�n de Funciones                                    **/
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

            Aeropuerto nuevoAeropuerto(nombre_aeropuerto, nombre_ciudad, pais);

            redDeViajes.agregar_vertice(aeropuertos.size());

            aeropuertos.push_back(nuevoAeropuerto);
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
        ::aerolineas.insert({aerolinea});

        nuevoVuelo.agregarAerolinea(aerolinea, asientos);

        // cout <<"[ " << aerolinea << " - " << asientos << " ]" << endl;
        inicial = pos + 1;
    }

    unsigned int claveOrigen, claveDestino;

    for (unsigned int i = 0; i < aeropuertos.size(); i++)
    {
        if (aeropuertos[i].verNombre() == origen)
            claveOrigen = i;
        if (aeropuertos[i].verNombre() == destino)
            claveDestino = i;
    }

    redDeViajes.agregar_arco(claveOrigen, claveDestino, nuevoVuelo);
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
            double distancia = atof(distancia_texto.c_str());

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
/*                                        Cargar Reservas                                        */
// --------------------------------------------------------------------------------------------- //

void cargarReservas(string path)
{
    ifstream origen(path.c_str());
    if (!origen.is_open())
        cout << "No se pudo abrir el archivo: " << path << endl;
    else
    {
        // cout << "RESERVAS " << endl;
        // cout << "------------------------" << endl;
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

            // Creo el objeto reservas y lo cargo
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

            //cout << reservas[nombre_aeropuerto_origen][nombre_aeropuerto_destino].retornarAsientosReservados(aerolinea) << endl;

            numeroReservas++;
        }
    }
}

// --------------------------------------------------------------------------------------------- //
/*                                      Desplegar el Menú                                        */
// --------------------------------------------------------------------------------------------- //

void desplegarMenu()
{
    wcout << CYAN << L"¡Bienvenido, usuario!\n"
          << RESET << endl;
    char option;
    bool listadoGenerado = false;
    unsigned int origen, destino;
    do
    {
        wcout << CYAN << "Estas son las opciones disponibles en nuestro sistema:\n"
              << RESET << endl
              << CYAN << "1. " << RESET << "Listar todos los aeropuertos" << endl
              << CYAN << "2. " << RESET << "Listar todas las reservas realizadas" << endl
              << CYAN << "3. " << RESET << "Verificar si existe un vuelo directo" << endl
              << CYAN << "4. " << RESET << L"Listar todos los vuelos posibles de un origen a un destino a través de una misma aerolínea" << endl
              << CYAN << "5. " << RESET << "Circuito de aeropuertos" << endl
              << CYAN << "6. " << RESET << "Ir al apartado de archivos" << endl
              << CYAN << "7. " << RESET << "Salir\n"
              << endl
              << CYAN << L"Me gustaría: " << RESET;

        cin >> option;

        system("cls");

        switch (option)
        {
        case '1':
            if (!aeropuertos.empty())
            {
                generarListadoAeropuertos("outputs/ListadoAeropuertos.txt");
                listadoGenerado = true;
            }
            else
                cout << RED << "No se hallaron aeropuertos en el sistema" << RESET << endl;
            break;
        case '2':
            if (numeroReservas > 0)
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
            if (redDeViajes.existe_vertice(origen) && redDeViajes.existe_vertice(destino))
            {
                generarListadoVuelosMismaAerolinea(origen, destino);
                listadoGenerado = true;
            }
            else
                cout << RED << "Los aeropuertos indicados no han sido hallados en el sistema" << RESET << endl;
            break;
        case '5':
            break;
        case '6':
            desplegarApartadoArchivos();
            break;
        case '7':
            break;
        default:
            wcout << RED << L"La opción ingresada no es válida. Revise lo ingresado.\n"
                  << RESET;
        }
        if (listadoGenerado)
        {
            wcout << CYAN << L"¡Listado generado! " << RESET << "\n\nPuede verlo en el "
                  << CYAN << "apartado de archivos" << RESET
                  << L" del menú de opciones." << endl;
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
    wcout << CYAN << L"¡Gracias por utilizar el servicio!" << RESET << endl;
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
        wcout << CYAN << "Elija el archivo que desea visualizar:\n"
              << RESET << endl
              << CYAN << "1. " << RESET << "Listado de aeropuertos" << endl
              << CYAN << "2. " << RESET << "Listado de reservas" << endl
              << CYAN << "3. " << RESET << "Listado de vuelos de un origen a un destino a través de una misma aerolínea" << endl
              << CYAN << "4. " << RESET << L"Volver al menú principal\n"
              << endl
              << CYAN << L"Me gustaría visualizar el: " << RESET;

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
            mostrarArchivo("outputs/VuelosMismaAerolinea_" + aeropuertos[origen].verNombre() + "_" + aeropuertos[destino].verNombre() + ".txt");
            break;
        case '4':
            break;
        default:
            wcout << RED << L"La opción ingresada no es válida. Revise lo ingresado.\n"
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
        wcout << L"No existe ningún archivo generado para la opción elegida." << endl;
    else
    {
        string linea;

        while (getline(archivo, linea))
            cout << linea << endl;
    }
}

// --------------------------------------------------------------------------------------------- //
/*                                    Listar los Aeropuertos                                     */
// --------------------------------------------------------------------------------------------- //

void generarListadoAeropuertos(string path)
{
    ofstream archivo;

    archivo.open(path.c_str(), fstream::out);

    archivo << "Aeropuertos en nuestro sistema:\n"
            << endl;

    for (const auto &aeropuerto : aeropuertos)
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
    Vuelo vuelo;
    list<Grafo<Vuelo>::Arco> arcos;
    list<string> aerolineas;
    unsigned int nroAeropuerto = 0, asientosReservados;
    string aeropuertoOrigen, aeropuertoDestino;

    ofstream archivo;

    archivo.open(path.c_str(), fstream::out);

    archivo << "Reservas registradas en el sistema: \n"
            << endl;

    for (const auto &aeropuerto : aeropuertos)
    {
        // Busco los aeropuertos destinos a partir del origen.
        redDeViajes.devolver_adyacentes(nroAeropuerto, arcos);

        aeropuertoOrigen = aeropuertos[nroAeropuerto].verNombre();

        for (const auto &arco : arcos)
        {
            vuelo = arco.devolver_costo();

            vuelo.verAerolineas(aerolineas);

            aeropuertoDestino = aeropuertos[arco.devolver_adyacente()].verNombre();

            for (string aerolinea : aerolineas)
            {
                try
                {
                    asientosReservados = reservas[aeropuertoOrigen][aeropuertoDestino].retornarAsientosReservados(aerolinea);
                }

                catch(invalid_argument & exc)
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

        nroAeropuerto++;
    }

    archivo.close();
}

// --------------------------------------------------------------------------------------------- //
/*                                        Solicitar Datos                                        */
// --------------------------------------------------------------------------------------------- //

void solicitarDatos(unsigned int &origen, unsigned int &destino, string &aerolinea)
{
    listarAeropuertos();

    cout << CYAN << "\n- Seleccione un aeropuerto de origen: " << RESET;
    origen = seleccionarAeropuerto();

    cout << CYAN << "\n- Seleccione un aeropuerto de origen: " << RESET;
    destino = seleccionarAeropuerto();

    system("cls");

    listarAerolineas();

    wcout << CYAN << L"\n- Seleccione una aerolínea: " << RESET;
    aerolinea = seleccionarAerolinea();

    system("cls");
}

void solicitarDatos(unsigned int &origen, unsigned int &destino)
{
    listarAeropuertos();

    cout << CYAN << "\n- Seleccione un aeropuerto de origen: " << RESET;
    origen = seleccionarAeropuerto();

    cout << CYAN << "\n- Seleccione un aeropuerto de destino: " << RESET;
    destino = seleccionarAeropuerto();
}

void listarAeropuertos()
{
    for (unsigned int i = 0; i < aeropuertos.size(); i++)
        cout << CYAN << i+1 << ". " << RESET << aeropuertos[i].verNombre() << endl;
}

void listarAerolineas()
{
    unsigned int index = 1;

    for (const string & aerolinea : aerolineas)
    {
        cout << CYAN << index << ". " << RESET << aerolinea << endl;
        index++;
    }
}

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

    } while(index > aerolineas.size());

    for (const auto & aerolinea : aerolineas)
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

    } while(index > aeropuertos.size());

    return index-1;
}

// --------------------------------------------------------------------------------------------- //
/*                           Verificar si Existe un Vuelo Directo                                */
// --------------------------------------------------------------------------------------------- //

void existeVueloDirecto()
{
    unsigned int origen, destino, asientosReservados;
    string nombreAeropuertoOrigen, nombreAeropuertoDestino, aerolinea;

    solicitarDatos(origen, destino, aerolinea);

    nombreAeropuertoOrigen = aeropuertos[origen].verNombre();
    nombreAeropuertoDestino = aeropuertos[destino].verNombre();

    list<Grafo<Vuelo>::Arco> adyacentes;

    redDeViajes.devolver_adyacentes(origen, adyacentes);

    auto it = adyacentes.begin();

    double distancia;
    unsigned int asientos_disponibles;

    while ((it != adyacentes.end()) && ((it->devolver_adyacente()) != destino))
        it++;

    if ((it != adyacentes.end()) && (it->devolver_adyacente() == destino))
    {
        Vuelo vuelo = it->devolver_costo();

        if (vuelo.existeAerolinea(aerolinea))
        {
            try
            {
                asientosReservados = reservas[nombreAeropuertoOrigen][nombreAeropuertoDestino].retornarAsientosReservados(aerolinea);
            }

            catch(invalid_argument & exc)
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
        redDeViajes.devolver_adyacentes(origen, arcos);

        for (const auto &arco : arcos)
        {
            vuelo = arco.devolver_costo();

            nombreDestino = aeropuertos[arco.devolver_adyacente()].verNombre();

            try
            {
                asientosReservados = reservas[nombreOrigen][nombreDestino].retornarAsientosReservados(aerolinea);
            }

            catch(invalid_argument & exc)
            {
                asientosReservados = 0;
            }

            if (!(visitado[arco.devolver_adyacente()]))
                if ((vuelo.verAsientosTotales(aerolinea) - asientosReservados) > 0)
                {
                    distancia += vuelo.verDistancia();
                    buscarCaminos(arco.devolver_adyacente(), destino, aerolinea, distancia, escalas, camino, visitado, archivo);
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

    cout << distancia;

    archivo << "Distancia: " << distancia << " kilómetros."
            << "\n"
            << endl;
}
