#include <iostream>
#include <fstream>
#include <locale>

#include <set>
#include <map>
#include <list>
#include <string.h>
#include "Vuelo.h"
#include "Grafo.h"
#include "Colors.h"
#include "Aeropuerto.h"

using namespace std;

// --------------------------------------------------------------------------------------------- //
/**                                  Prototipos de Funciones                                     **/
// --------------------------------------------------------------------------------------------- //

void cargar_aeropuertos(string path);
void cargar_reservas(string path);
void cargar_aeropuertos_rutas(string path);
void cargar_rutas(string path);

void desplegar_menu();
void desplegar_apartado_archivos();

void mostrar_archivo(string path);

void generar_listado_aeropuertos(string path);

void generar_listado_reservas(string path);

void existe_vuelo_directo();

void solicitar_datos(string &origen, string &destino);
void solicitar_datos(string &origen, string &destino, string &aerolinea);

void mostrar_vuelos_misma_aerolinea();
void busqueda_caminos(const string &origen, const string &destino, const string &aerolinea,
                      double &distancia, unsigned int &escalas, list<string> &camino, map<string, bool> &visitado);
void imprimir_camino(const string &destino, const double &distancia, const unsigned int &escalas, const list<string> &camino);

// --------------------------------------------------------------------------------------------- //
/**                                     Variables Globales                                      **/
// --------------------------------------------------------------------------------------------- //

unsigned int reservas;

set<string> aerolineas; // Lista con todas las aerolíneas de la red de viaje.

map<string, Aeropuerto> aeropuertos; // Declaro una estructura que me permita conectar la información del aeropuerto con un �ndice.

Grafo<Vuelo> redDeViajes;

// --------------------------------------------------------------------------------------------- //
/**                                     Programa Principal                                      **/
// --------------------------------------------------------------------------------------------- //

int main()
{
    setlocale(LC_ALL, "");

    cargar_aeropuertos("datasets/Aeropuertos.txt");
    cargar_rutas("datasets/Rutas.txt");
    cargar_reservas("datasets/Reservas.txt");

    desplegar_menu();

    remove("outputs/ListadoAeropuertos.txt");
    remove("outputs/ListadoReservas.txt");

    return 0;
}

// --------------------------------------------------------------------------------------------- //
/**                               Implementaci�n de Funciones                                    **/
// --------------------------------------------------------------------------------------------- //

void cargar_aeropuertos(string path)
{
    ifstream origen(path.c_str());
    // aeropuerto aux;
    if (!origen.is_open())
        cout << "No se pudo abrir el archivo: " << path << endl;
    else
    {
        // cout << "AEROPUERTOS " << endl;
        // cout << "------------------------" << endl;
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

            aeropuertos.insert({nuevoAeropuerto.verNombre(), nuevoAeropuerto});

            redDeViajes.agregar_vertice(nombre_aeropuerto);

            // cout <<nombre_aeropuerto << " - " << nombre_ciudad << " - " << pais << endl;
        }
    }
}

// --------------------------------------------------------------------------------------------- //

void cargar_aerolineas_rutas(string aerolineas, string origen, string destino, Vuelo &nuevoVuelo)
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
        ::aerolineas.insert(aerolinea);

        nuevoVuelo.agregarAerolinea(aerolinea, asientos);

        // cout <<"[ " << aerolinea << " - " << asientos << " ]" << endl;
        inicial = pos + 1;
    }

    redDeViajes.agregar_arco(origen, destino, nuevoVuelo);
}

// --------------------------------------------------------------------------------------------- //

void cargar_rutas(string path)
{
    ifstream origen(path.c_str());
    if (!origen.is_open())
        cout << "No se pudo abrir el archivo: " << path << endl;
    else
    {
        bool cabotaje;
        // cout << "RUTAS " << endl;
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

            // cout << nombre_aeropuerto_origen << " - " << nombre_aeropuerto_destino << " - " << distancia << " - "
            //     << cabotaje_texto << " - ";
            cargar_aerolineas_rutas(aerolineas, nombre_aeropuerto_origen, nombre_aeropuerto_destino, nuevoVuelo);
        }
    }
}

// --------------------------------------------------------------------------------------------- //
/*                                        Cargar Reservas                                        */
// --------------------------------------------------------------------------------------------- //

void cargar_reservas(string path)
{
    reservas = 0;
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

            // cout <<nombre_aeropuerto_origen << " - " << nombre_aeropuerto_destino << " - " << aerolinea
            //<< " - " << asientos_reservados << endl;

            Vuelo vuelo;

            vuelo = redDeViajes.costo_arco(nombre_aeropuerto_origen, nombre_aeropuerto_destino);

            vuelo.modificarAsientosReservados(aerolinea, asientos_reservados);

            redDeViajes.modificar_costo_arco(nombre_aeropuerto_origen, nombre_aeropuerto_destino, vuelo);

            reservas++;
        }
    }
}

// --------------------------------------------------------------------------------------------- //
/*                                      Desplegar el Men�                                        */
// --------------------------------------------------------------------------------------------- //

void desplegar_menu()
{
    wcout << CYAN << L"¡Bienvenido, usuario!\n"
         << RESET << endl;
    char option;
    do
    {
        wcout << CYAN << "Estas son las opciones disponibles en nuestro sistema:\n"
             << RESET << endl
             << CYAN << "1. " << RESET << "Listar todos los aeropuertos" << endl
             << CYAN << "2. " << RESET << "Listar todas las reservas realizadas" << endl
             << CYAN << "3. " << RESET << "Verificar si existe un vuelo directo" << endl
             << CYAN << "4. " << RESET << L"Obtener todos los vuelos a través de una misma aerolínea" << endl
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
                generar_listado_aeropuertos("outputs/ListadoAeropuertos.txt");
                wcout << CYAN << L"¡Listado generado! " << RESET << "\n\nPuede verlo en el "
                    << CYAN << "apartado de archivos" << RESET
                    << L" del menú de opciones." << endl;
            }
            else
                cout << CYAN << "No se encontraron aeropuertos en nuestro \
                    sistema para generar el listado." << RESET << endl;
            break;
        case '2':
            if (!aeropuertos.empty())
            {
                generar_listado_reservas("outputs/ListadoReservas.txt");
                wcout << CYAN << L"¡Listado generado! " << RESET << "\n\nPuede verlo en el "
                    << CYAN << "apartado de archivos" << RESET
                    << L" del menú de opciones." << endl;
            }
            else
                cout << CYAN << "No se encontraron reservas en nuestro \
                    sistema para generar el listado." << RESET << endl;
            break;
        case '3':
            existe_vuelo_directo();
            break;
        case '4':
            mostrar_vuelos_misma_aerolinea();
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

void desplegar_apartado_archivos()
{
    char option;
    do
    {
        wcout << CYAN << "Elija el archivo que desea visualizar:\n"
             << RESET << endl
             << CYAN << "1. " << RESET << "Listado de aeropuertos" << endl
             << CYAN << "2. " << RESET << "Listado de reservas" << endl
             << CYAN << "3. " << RESET << L"Volver al menú principal\n"
             << endl
             << CYAN << L"Me gustaría visualizar el: " << RESET;

        cin >> option;

        system("cls");

        switch (option)
        {
        case '1':
            cout << CYAN << "Aeropuertos en nuestro sistema:\n" << RESET << endl;
            mostrar_archivo("outputs/ListadoAeropuertos.txt");
            break;
        case '2':
            cout << CYAN << "Se registraron las siguientes reservas en nuestro sistema: \n"
                << RESET << endl;
            mostrar_archivo("outputs/ListadoReservas.txt");
        case '3':
            break;
        default:
            wcout << RED << L"La opción ingresada no es válida. Revise lo ingresado.\n" << RESET;
        }
    if (option != '3')
    {
        cout << RED << endl;
        system("pause");
        cout << RESET << endl;
        system("cls");
    }
    } while (option != '3');
}

// --------------------------------------------------------------------------------------------- //

void mostrar_archivo(string path)
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

void generar_listado_aeropuertos(string path)
{
    ofstream archivo;

    archivo.open(path.c_str(), fstream::out);

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

void generar_listado_reservas(string path)
{
    Vuelo vuelo;
    list<Grafo<Vuelo>::Arco> arcos;
    list<string> aerolineas;
    unsigned int asientosReservados;

    ofstream archivo;

    archivo.open(path.c_str(), fstream::out);

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

 void mostrar_vuelos_misma_aerolinea()
{
    // Declaro las variables necesarias.

    string origen, destino;

    list<string> camino;

    map<string, bool> visitado;

    double distancia = 0;

    unsigned int escalas = 0;

    // Solicito los datos

    solicitar_datos(origen, destino);

    system("cls");

    // Por cada aeropuerto en el mapa de aeropuertos, inicializo su estado de visitado en falso
    for (const auto & aeropuerto : aeropuertos)
        visitado.insert({aeropuerto.first, false});

    cout << "Estos son todos los vuelos del aeropuerto " << CYAN << origen << RESET
    << " al aeropuerto " << CYAN << destino << RESET << " a trav�s de: \n" << endl;

    // Por cada aerolínea en la lista de aerolíneas...
    for (const string & aerolinea : aerolineas)
    {
        cout << CYAN << "-----------------------------------------------------------------------------------------\n" << RESET << endl;

        cout << "Aerolínea " << CYAN << aerolinea << RESET << ": \n" << endl;

        busqueda_caminos(origen, destino, aerolinea, distancia, escalas, camino, visitado);
    }
}

// ------------------------------------Búsqueda de Caminos-------------------------------------- //

 void busqueda_caminos(const string & origen, const string & destino, const string & aerolinea,
                      double & distancia, unsigned int & escalas, list<string> & camino, map<string, bool> & visitado)
{
    Vuelo vuelo;

    visitado[origen] = true;
    camino.push_back(origen);
    escalas++;

    // Si estoy en el destino...
    if (origen == destino)
        imprimir_camino(destino, distancia, escalas, camino);
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
                    busqueda_caminos(arco.devolver_adyacente(), destino, aerolinea, distancia, escalas, camino, visitado);
                    distancia -= vuelo.verDistancia();
                }
        }
    }
    camino.pop_back();
    escalas--;
    visitado[origen] = false;
}

// -------------------------------------Imprimir Camino----------------------------------------- //

 void imprimir_camino(const string & destino, const double & distancia, const unsigned int & escalas, const list<string> & camino)
{
    for (const string & aeropuerto : camino)
    {
        cout << aeropuerto;
        if (aeropuerto != destino)
            cout << " -> ";
    }

    if (escalas == 2)
        cout << "\nVuelo directo. ";
    else if (escalas == 3)
        cout << "\nVuelo con 1 escala. ";
    else
        cout << "\nVuelo con " << (escalas-2) << " escalas. ";

    cout << "Distancia: " << distancia << " kilómetros." << "\n" << endl;
}
