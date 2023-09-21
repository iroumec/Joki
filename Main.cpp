#include <iostream>
#include <fstream>
#include <locale>
#include <chrono>
#include <thread>

#include <map>
#include <list>
#include <vector>
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
void mostrar_listado_aeropuertos(string path);

void listar_reservas();
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

list<string> aerolineas; // Lista con todas las aerol�neas de la red de viaje.

map<string, Aeropuerto> aeropuertos; // Declaro una estructura que me permita conectar la informaci�n del aeropuerto con un �ndice.

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

            Vuelo vuelo(false, 0);

            vuelo = redDeViajes.costo_arco(nombre_aeropuerto_origen, nombre_aeropuerto_destino);

            vuelo.modificarAsientosReservados(aerolinea, asientos_reservados);

            redDeViajes.modificar_costo_arco(nombre_aeropuerto_origen, nombre_aeropuerto_destino, vuelo);
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
    unsigned int option;
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
        case 1:
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
            cout << RED << endl;
            system("pause");
            cout << RESET << endl;
            system("cls");
            break;
        case 2:
            listar_reservas();
            break;
        case 3:
            existe_vuelo_directo();
            break;
        case 4:
            //mostrar_vuelos_misma_aerolinea();
            break;
        case 5:
            break;
        case 6:
            desplegar_apartado_archivos();
            break;
        case 7:
            break;
        default:
            wcout << L"La opción ingresada no es válida. Revise lo ingresado.";
            system("cls");
        }
    } while (option != 7);
    system("cls");
    wcout << CYAN << L"¡Gracias por utilizar el servicio!" << RESET << endl;
}

void desplegar_apartado_archivos()
{
    unsigned int option;
    do
    {
        wcout << CYAN << "Elija el archivo que desea visualizar:\n"
             << RESET << endl
             << CYAN << "1. " << RESET << "Listado de aeropuertos" << endl
             << CYAN << "7. " << RESET << "Salir\n"
             << endl
             << CYAN << L"Me gustaría visualizar el: " << RESET;

        cin >> option;

        system("cls");

        switch (option)
        {
        case 1:
            cout << CYAN << "Aeropuertos en nuestro sistema:\n" << RESET << endl;
            mostrar_archivo("outputs/ListadoAeropuertos.txt");
            cout << RED << endl;
            system("pause");
            cout << RESET << endl;
            system("cls");
            break;
        default:
            "La opción ingresada no es válida. Revise lo ingresado.";
            system("cls");
        }
    } while (option != 1);
    system("cls");
}

void mostrar_archivo(string path)
{
    ifstream archivo(path.c_str());

    string linea;

    while (getline(archivo, linea))
        cout << linea << endl;
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

void listar_reservas()
{
    Vuelo vuelo;

    list<Grafo<Vuelo>::Arco> arcos;

    list<string> aerolineas;

    unsigned int asientosReservados;

    cout << CYAN << "Se registraron las siguientes reservas en nuestro sistema: \n"
         << RESET << endl;

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

                cout << asientosReservados << endl;

                if (asientosReservados > 0) // Es decir, si hay reservas
                {
                    cout << CYAN << "- " << RESET << "Vuelo de " << CYAN << aeropuerto.first << RESET
                         << " a " << CYAN << arco.devolver_adyacente() << RESET
                         << ", a través de la aerolínea " << CYAN << aerolinea << RESET
                         << " con un total de " << CYAN << vuelo.verAsientosReservados(aerolinea) << RESET
                         << "/" << CYAN << asientosReservados << RESET << endl;
                }
            }

            // Vacío la lista de aerolíneas para darle paso a la próxima iteración.
            aerolineas.clear();
        }

        // Vacío la lista de arcos para dar paso a la siguiente iteración.
        arcos.clear();
    }

    cout << RED << endl;
    system("pause");
    cout << RESET << endl;
    system("cls");
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
     cout << '\n';

     cout << CYAN << "- " << RESET << "Destino: ";
     getline(cin, destino);
     cout << '\n';

     wcout << CYAN << "- " << RESET << L"Aerolínea: ";
     getline(cin, aerolinea);
     system("cls");
 }

 void solicitar_datos(string & origen, string & destino)
{
     cout << CYAN << "Proporcione los siguientes datos: \n" << RESET << endl;

     cout << CYAN << "- " << RESET << "Origen: ";
     cin.ignore();
     getline(cin, origen);
     cout << '\n';

     cout << CYAN << "- " << RESET << "Destino: ";
     getline(cin, destino);
     system("cls");
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
            cout << "Se encontró un viaje de " << CYAN << origen << RESET << " a " << CYAN << destino << RESET
                << " a través de " << CYAN << aerolinea << RESET << ": \n" << endl
                << "Distancia de: " << CYAN << vuelo.verDistancia() << RESET << " kilómetros. \n" << endl
                << "Asientos disponibles: " << CYAN << vuelo.verAsientosLibres(aerolinea) << RESET << " asientos disponibles." << endl;
        }
    }
    else
        cout << RED << "No existe" << RESET << " un vuelo desde " << RED << origen << RESET << " a " << RED << destino
        << RESET << " a través de " <<  RED << aerolinea << RESET << endl;

    cout << RED << endl;
    system("pause");
    cout << RESET << endl;
    system("cls");
}

//// --------------------------------------------------------------------------------------------- //
///*                     Mostrar Vuelos a trav�s de una Misma Aerol�nea                            */
//// --------------------------------------------------------------------------------------------- //
//
// void mostrar_vuelos_misma_aerolinea()
//{
//    // Declaro las variables necesarias.
//
//    string origen, destino;
//
//    list<string> camino;
//
//    list<string> aeropuertos;
//
//    list<string> aerolineas;
//
//    map<string, bool> visitado;
//
//    double distancia = 0;
//
//    unsigned int escalas = 0;
//
//    // Solicito los datos
//
//    solicitar_datos(origen, destino);
//
//    // Obtengo los aeropuertos y lleno el mapa "visitado" en false.
//
//    viajes.devolver_aeropuertos(aeropuertos);
//
//    auto it = aeropuertos.begin();
//
//    while (it != aeropuertos.end())
//    {
//        visitado.insert({*it, false});
//        it++;
//    }
//
//    // Obtengo las aerol�neas.
//
//    viajes.devolver_aerolineas(aerolineas);
//
//    auto it_aerolineas = aerolineas.begin();
//
//    // Muestro los vuelos...
//    cout << "Estos son todos los vuelos del aeropuerto " << CYAN << origen << RESET
//    << " al aeropuerto " << CYAN << destino << RESET << " a trav�s de: \n" << endl;
//
//    while (it_aerolineas != aerolineas.end())
//    {
//        cout << CYAN << "-----------------------------------------------------------------------------------------\n" << RESET << endl;
//
//        cout << "Aerol�nea " << CYAN << *it_aerolineas << RESET << ": \n" << endl;
//
//        busqueda_caminos(origen, destino, *it_aerolineas, distancia, escalas, camino, visitado);
//
//        it_aerolineas++;
//    }
//
//    cout << RED << endl;
//    system("pause");
//    cout << RESET << endl;
//    system("cls");
//}
//
//// ------------------------------------B�squeda de Caminos-------------------------------------- //
//
// void busqueda_caminos(const string & origen, const string & destino, const string & aerolinea,
//                      double & distancia, unsigned int & escalas, list<string> & camino, map<string, bool> & visitado)
//{
//    visitado[origen] = true;
//    camino.push_back(origen);
//    escalas++;
//    if (origen == destino)
//        imprimir_camino(destino, distancia, escalas, camino);
//    else
//    {
//        list<Grafo<string>::Arco> destinos;
//        viajes.devolver_adyacentes(origen, destinos);
//        auto it = destinos.begin();
//
//        while (it != destinos.end())
//        {
//            if (!(visitado[it->devolver_destino()]))
//                if (it->devolver_asientos_libres(aerolinea) > 0)
//                    //En caso que no haya asientos libres se devolver� un 0, y si la aerolinea no se encuentra un -1.
//                {
//                    distancia += it->devolver_distancia();
//                    busqueda_caminos(it->devolver_destino(), destino, aerolinea, distancia, escalas, camino, visitado);
//                    distancia -= it->devolver_distancia();
//                }
//            it++;
//        }
//    }
//    camino.pop_back();
//    escalas--;
//    visitado[origen] = false;
//}
//
//// -------------------------------------Imprimir Camino----------------------------------------- //
//
// void imprimir_camino(const string & destino, const double & distancia, const unsigned int & escalas, const list<string> & camino)
//{
//    auto it = camino.begin();
//
//    while (it != camino.end())
//    {
//        cout << *it;
//        if (*it != destino)
//            cout << " -> ";
//        it++;
//    }
//
//    if (escalas == 1)
//        cout << "\nVuelo directo. ";
//    else
//        cout << "\nVuelo con " << (escalas-2) << " escalas. ";
//
//    cout << "Distancia: " << distancia << " kil�metros." << "\n" << endl;
//}
