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

void solicitarDatos(string &origen, string &destino);
void solicitarDatos(string &origen, string &destino, string &aerolinea);

void eliminarArchivosAntiguos(const filesystem::path & carpeta);

void desplegarMenu();
void desplegarApartadoArchivos();

void mostrarArchivo(string path);

void generarListadoAeropuertos(string path);

void generarListadoReservas(string path);

void existeVueloDirecto();

void generarListadoVuelosMismaAerolinea(const string & origen, const string & destino);

void buscar_caminos(const string &origen, const string &destino, const string &aerolinea,
                      double &distancia, unsigned int &escalas, list<string> &camino, map<string, bool> &visitado, ofstream & archivo);

void cargar_camino(const string &destino, const double &distancia, const unsigned int &escalas, const list<string> &camino, ofstream & archivo);

// --------------------------------------------------------------------------------------------- //
/**                                     Variables Globales                                      **/
// --------------------------------------------------------------------------------------------- //

map<string, map<string, Reserva>> reservas; // Mapa de mapas con las reservas

map<string, unsigned int> aerolineas; // Lista con todas las aerolíneas de la red de viaje.

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
        ::aerolineas.insert({aerolinea, aerolineas.size()});

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
                    *it_destino.insert({nombre_aeropuerto_destino, nuevaReserva});
                }
            }
            else
            {
                map<string, int> nuevo_mapa;
                nuevo_mapa.insert({nombre_aeropuerto_destino, nuevaReserva});
                reservas.insert({nombre_aeropuerto_origen, nuevo_mapa})
            }
        }
    }
}

// --------------------------------------------------------------------------------------------- //
/*                                      Desplegar el Men�                                        */
// --------------------------------------------------------------------------------------------- //

void desplegarMenu()
{
    wcout << CYAN << L"¡Bienvenido, usuario!\n"
         << RESET << endl;
    char option;
    bool listadoGenerado = false;
    string origen, destino;
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
            if (reservas > 0)
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
            solicitarDatos(origen, destino); system("cls");
            if (redDeViajes.existe_vertice(origen) && redDeViajes.existe_vertice(destino))
            {
                generar_listado_vuelos_misma_aerolinea(origen, destino);
                listadoGenerado = true;
            }
            else
                cout << RED << "Los aeropuertos indicados no han sido hallados en el sistema" << RESET << endl;
            break;
        case '5':
            break;
        case '6':
            desplegar_apartado_archivos();
            break;
        case '7':
            break;
        default:
            wcout << RED << L"La opción ingresada no es válida. Revise lo ingresado.\n" << RESET;
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

void eliminarArchivosAntiguos(const filesystem::path & carpeta)
{
    for (const auto & archivo : filesystem::directory_iterator(carpeta))
        filesystem::remove_all(archivo.path());
}

// --------------------------------------------------------------------------------------------- //

void desplegar_apartado_archivos()
{
    char option;
    string origen, destino;
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
            mostrar_archivo("outputs/ListadoAeropuertos.txt");
            break;
        case '2':
            mostrar_archivo("outputs/ListadoReservas.txt");
            break;
        case '3':
            solicitar_datos(origen, destino); system("cls");
            mostrar_archivo("outputs/VuelosMismaAerolinea_" + origen + "_" + destino + ".txt");
            break;
        case '4':
            break;
        default:
            wcout << RED << L"La opción ingresada no es válida. Revise lo ingresado.\n" << RESET;
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

    archivo << "Aeropuertos en nuestro sistema:\n" << endl;

    for (const auto & aeropuerto : aeropuertos)
    {
        archivo << "- " << (aeropuerto.first).c_str() << " (" << (aeropuerto.second.verCiudad()).c_str() << ", "
            << aeropuerto.second.verPais() << ")." << endl;
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
    unsigned int asientosReservados;

    ofstream archivo;

    archivo.open(path.c_str(), fstream::out);

    archivo << "Reservas registradas en el sistema: \n" << endl;

    for (const auto & aeropuerto : aeropuertos)
    {
        // Busco los aeropuertos destinos a partir del origen.
        redDeViajes.devolver_adyacentes(aeropuerto.first, arcos);

        for (const auto & arco : arcos)
        {
            vuelo = arco.devolver_costo();

            vuelo.verAerolineas(aerolineas);

            for (string aerolinea : aerolineas)
            {
                asientosReservados = vuelo.verAsientosReservados(aerolinea);

                // Si hay reservas realizadas...
                if (asientosReservados > 0)
                {
                    archivo << "- Vuelo de " << aeropuerto.first << " a " << arco.devolver_adyacente()
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
    }

    archivo.close();
}

// --------------------------------------------------------------------------------------------- //
/*                                        Solicitar Datos                                        */
// --------------------------------------------------------------------------------------------- //

 void solicitar_datos(string & origen, string & destino, string & aerolinea)
{
     cout << CYAN << "Proporcione los siguientes datos: \n" << RESET << endl;

     cout << CYAN << "- " << RESET << "Origen: ";
     cin.ignore();
     getline(cin, origen);

     cout << CYAN << "\n- " << RESET << "Destino: ";
     getline(cin, destino);

     wcout << CYAN << "\n- " << RESET << L"Aerolínea: ";
     getline(cin, aerolinea);
 }

 void solicitar_datos(string & origen, string & destino)
{
     cout << CYAN << "Proporcione los siguientes datos: \n" << RESET << endl;

     cout << CYAN << "- " << RESET << "Origen: ";
     cin.ignore();
     getline(cin, origen);

     cout << CYAN << "\n- " << RESET << "Destino: ";
     getline(cin, destino);
 }

// --------------------------------------------------------------------------------------------- //
/*                           Verificar si Existe un Vuelo Directo                                */
// --------------------------------------------------------------------------------------------- //

void existe_vuelo_directo()
{
    string origen, destino, aerolinea;

    solicitar_datos(origen, destino, aerolinea);

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
            cout << "\n¡Vuelo de " << CYAN << origen << RESET << " a " << CYAN << destino << RESET
                << " a través de " << CYAN << aerolinea << RESET << " hallado! \n"
                << "\n- Distancia del vuelo: " << CYAN << vuelo.verDistancia() << " km \n" << RESET
                << "\n- Asientos disponibles: " << CYAN << vuelo.verAsientosLibres(aerolinea) << RESET << endl;
        }
    }
    else
        cout << RED << "\nNo existe" << RESET << " un vuelo desde " << RED << origen << RESET << " a " << RED << destino
        << RESET << " a través de " <<  RED << aerolinea << RESET << endl;
}

// --------------------------------------------------------------------------------------------- //
/*                     Mostrar Vuelos a través de una Misma Aerolínea                            */
// --------------------------------------------------------------------------------------------- //

 void generar_listado_vuelos_misma_aerolinea(const string & origen, const string & destino)
{
    // Declaro las variables necesarias.

    list<string> camino;

    map<string, bool> visitado;

    double distancia = 0;

    unsigned int escalas = 0;

    // Solicito los datos

    ofstream archivo;

    archivo.open("outputs/VuelosMismaAerolinea_" + origen + "_" + destino + ".txt", fstream::out);

    // Por cada aeropuerto en el mapa de aeropuertos, inicializo su estado de visitado en falso
    for (const auto & aeropuerto : aeropuertos)
        visitado.insert({aeropuerto.first, false});

    archivo << "Vuelos del aeropuerto " << origen << " al aeropuerto " << destino << " a través de: \n" << endl;

    // Por cada aerolínea en la lista de aerolíneas...
    for (const string & aerolinea : aerolineas)
    {
        archivo << "-----------------------------------------------------------------------------------------\n" << endl;

        archivo << "Aerolínea " << aerolinea << ": \n" << endl;

        buscar_caminos(origen, destino, aerolinea, distancia, escalas, camino, visitado, archivo);
    }

    archivo.close();
}

// ------------------------------------Búsqueda de Caminos-------------------------------------- //

 void buscar_caminos(const string & origen, const string & destino, const string & aerolinea,
                      double & distancia, unsigned int & escalas, list<string> & camino, map<string, bool> & visitado, ofstream & archivo)
{
    Vuelo vuelo;

    visitado[origen] = true;
    camino.push_back(origen);
    escalas++;

    // Si estoy en el destino...
    if (origen == destino)
        cargar_camino(destino, distancia, escalas, camino, archivo);
    else
    {
        list<Grafo<Vuelo>::Arco> arcos;
        redDeViajes.devolver_adyacentes(origen, arcos);

        for (const auto & arco : arcos)
        {
            vuelo = arco.devolver_costo();

            if (!(visitado[arco.devolver_adyacente()]))
                if (vuelo.verAsientosLibres(aerolinea) > 0)
                {
                    distancia += vuelo.verDistancia();
                    buscar_caminos(arco.devolver_adyacente(), destino, aerolinea, distancia, escalas, camino, visitado, archivo);
                    distancia -= vuelo.verDistancia();
                }
        }
    }
    camino.pop_back();
    escalas--;
    visitado[origen] = false;
}

// -------------------------------------Imprimir Camino----------------------------------------- //

 void cargar_camino(const string & destino, const double & distancia, const unsigned int & escalas, const list<string> & camino, ofstream & archivo)
{
    for (const string & aeropuerto : camino)
    {
        archivo << aeropuerto;
        if (aeropuerto != destino)
            archivo << " -> ";
    }

    if (escalas == 2)
        archivo << "\nVuelo directo. ";
    else if (escalas == 3)
        archivo << "\nVuelo con 1 escala. ";
    else
        archivo << "\nVuelo con " << (escalas-2) << " escalas. ";

    archivo << "Distancia: " << distancia << " kilómetros." << "\n" << endl;
}
