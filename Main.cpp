// --------------------------------------------------------------------------------------------- //
/**                                 Directivas del Procesador                                    **/
// --------------------------------------------------------------------------------------------- //

#include <iostream>
#include <fstream>
#include <locale.h>

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

// --------------------------------------------------------------------------------------------- //
/*                           Referidas a la Carga de los Archivos                                */
// --------------------------------------------------------------------------------------------- //

void cargar_aeropuertos(string path);
void cargar_reservas(string path);
void cargar_aeropuertos_rutas(string path);
void cargar_rutas(string path);

// --------------------------------------------------------------------------------------------- //
/*                             Referidas a los Servicios Solicitados                             */
// --------------------------------------------------------------------------------------------- //

void desplegar_menu();

void listar_aeropuertos();
void listar_reservas();
void existe_vuelo_directo();

void solicitar_datos(string & origen, string & destino);
void solicitar_datos(string & origen, string & destino, string & aerolinea);

void mostrar_vuelos_misma_aerolinea();
void busqueda_caminos(const string & origen, const string & destino, const string & aerolinea,
                    double & distancia, unsigned int & escalas, list<string> & camino, map<string, bool> & visitado);
void imprimir_camino(const string & destino, const double & distancia, const unsigned int & escalas, const list<string> & camino);

// --------------------------------------------------------------------------------------------- //
/**                                     Variables Globales                                      **/
// --------------------------------------------------------------------------------------------- //

list<string> aerolineas; // Lista con todas las aerolíneas de la red de viaje.

map<string, Aeropuerto> aeropuertos; // Declaro una estructura que me permita conectar la información del aeropuerto con un índice.

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
/**                               Implementación de Funciones                                    **/
// --------------------------------------------------------------------------------------------- //

// --------------------------------------------------------------------------------------------- //
/*                                    Cargar Aeropuertos                                         */
// --------------------------------------------------------------------------------------------- //

void cargar_aeropuertos(string path)
{
    ifstream origen(path.c_str());
    //aeropuerto aux;
    if (!origen.is_open())
        cout << "No se pudo abrir el archivo: " << path << endl;
    else {
        //cout << "AEROPUERTOS " << endl;
        //cout << "------------------------" << endl;
        while (!origen.eof()) {
            string linea;
            getline(origen,linea);
            size_t primera_pos = linea.find(',');
            string nombre_aeropuerto = linea.substr(0, primera_pos);
            size_t segunda_pos = linea.find(',', primera_pos + 1);
            string nombre_ciudad = linea.substr(primera_pos + 1, segunda_pos - primera_pos - 1);
            string pais = linea.substr(segunda_pos + 1);

            Aeropuerto nuevoAeropuerto(aeropuertos.size(), nombre_aeropuerto, nombre_ciudad, pais);

            aeropuertos.insert({nuevoAeropuerto.verNombre(), nuevoAeropuerto});

            redDeViajes.agregar_vertice(nuevoAeropuerto.verId());

            //cout <<nombre_aeropuerto << " - " << nombre_ciudad << " - " << pais << endl;
        }
    }
}

// --------------------------------------------------------------------------------------------- //
/*                                  Cargar Rutas de Aerolineas                                   */
// --------------------------------------------------------------------------------------------- //

void cargar_aerolineas_rutas(string aerolineas, const string & origen, const string & destino, Vuelo & nuevoVuelo)
{
    aerolineas = aerolineas.substr(1);
    size_t inicial = 0;
    size_t pos = 0;
    size_t pos_final = aerolineas.find('}');
    while ((pos != std::string::npos) && (pos + 1 < pos_final)) {
        pos = aerolineas.find(',', inicial);
        string dato_aerolinea = aerolineas.substr(inicial, pos - inicial);
        size_t pos_separador = dato_aerolinea.find('-');
        string aerolinea = dato_aerolinea.substr(0, pos_separador);
        string asientos_texto = dato_aerolinea.substr(pos_separador + 1);
        int asientos = atoi(asientos_texto.c_str());

        nuevoVuelo.agregarAerolinea(aerolinea, asientos);

        //cout <<"[ " << aerolinea << " - " << asientos << " ]" << endl;
        inicial = pos + 1;
    }

    redDeViajes.agregar_arco(aeropuertos[origen].verId(), aeropuertos[destino].verId(), nuevoVuelo);
}

// --------------------------------------------------------------------------------------------- //
/*                                         Cargar Rutas                                          */
// --------------------------------------------------------------------------------------------- //

void cargar_rutas(string path)
{
    ifstream origen(path.c_str());
    if (!origen.is_open())
        cout << "No se pudo abrir el archivo: " << path << endl;
    else {
        bool cabotaje;
        //cout << "RUTAS " << endl;
        //cout << "------------------------" << endl;
        while (!origen.eof()) {
            string linea;
            getline(origen,linea);
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

            //cout << nombre_aeropuerto_origen << " - " << nombre_aeropuerto_destino << " - " << distancia << " - "
            //    << cabotaje_texto << " - ";
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
    else {
        //cout << "RESERVAS " << endl;
        //cout << "------------------------" << endl;
        while (!origen.eof()) {
            string linea;
            getline(origen,linea);
            size_t primera_pos = linea.find(',');
            string nombre_aeropuerto_origen = linea.substr(0, primera_pos);

            size_t segunda_pos = linea.find(',', primera_pos + 1);
            string nombre_aeropuerto_destino = linea.substr(primera_pos + 1, segunda_pos - primera_pos - 1);

            size_t tercera_pos = linea.find(',', segunda_pos + 1);
            string aerolinea = linea.substr(segunda_pos + 1, tercera_pos - segunda_pos - 1);

            string asientos_reservados_texto = linea.substr(tercera_pos + 1);
            int asientos_reservados = atoi(asientos_reservados_texto.c_str());

            //cout <<nombre_aeropuerto_origen << " - " << nombre_aeropuerto_destino << " - " << aerolinea
                //<< " - " << asientos_reservados << endl;

            Vuelo vuelo(false, 0);

            vuelo = redDeViajes.costo_arco(aeropuertos[nombre_aeropuerto_origen].verId(), aeropuertos[nombre_aeropuerto_destino].verId());

            vuelo.modificarAsientosReservados(aerolinea, asientos_reservados);

            redDeViajes.modificar_costo_arco(aeropuertos[nombre_aeropuerto_origen].verId(), aeropuertos[nombre_aeropuerto_destino].verId(), vuelo);
        }
    }
}

// --------------------------------------------------------------------------------------------- //
/*                                      Desplegar el Menú                                        */
// --------------------------------------------------------------------------------------------- //

void desplegar_menu()
{
    cout << CYAN << "¡Bienvenido, usuario!\n" << RESET << endl;
    unsigned int option;
    do
    {
        cout << CYAN << "Estas son las opciones disponibles en nuestro sistema:\n" << RESET << endl
        << CYAN << "1. " << RESET << "Listar todos los aeropuertos" << endl
        << CYAN << "2. " << RESET << "Listar todas las reservas realizadas" << endl
        << CYAN << "3. " << RESET << "Verificar si existe un vuelo directo" << endl
        << CYAN << "4. " << RESET << "Obtener todos los vuelos a través de una misma aerolínea" << endl
        << CYAN << "5. " << RESET << "Circuito de aeropuertos" << endl
        << CYAN << "6. " << RESET << "Salir\n" << endl
        << CYAN << "Me gustaría: " << RESET;

        cin >> option;

        system("cls");

        switch(option) {
            case 1: listar_aeropuertos(); break;
            case 2: listar_reservas(); break;
//            case 3: existe_vuelo_directo(); break;
//            case 4: mostrar_vuelos_misma_aerolinea(); break;
            case 5: break;
            case 6: break;
            default: "La opción ingresada no es válida. Reviso lo ingresado"; system("cls");
        }
    } while(option != 6);
    system("cls");
    cout << CYAN << "¡Gracias por utilizar el servicio!" << RESET << endl;
}

// --------------------------------------------------------------------------------------------- //
/*                                    Listar los Aeropuertos                                     */
// --------------------------------------------------------------------------------------------- //


void listar_aeropuertos()
{
    auto it = aeropuertos.begin();

    // Si hay aeropuertos...
    if (it != aeropuertos.end())
    {
        cout << CYAN << "Contamos con los siguientes aeropuertos en nuestro sistema:\n" << RESET << endl;

        while (it != aeropuertos.end())
        {
            cout << CYAN << "- " << RESET << (it->first) << endl;
            it++;
        }
    }
    // En caso contrario...
    else
        cout << CYAN << "No se encontraron aeropuertos en nuestro sistema." << RESET << endl;

    cout << RED << endl;
    system("pause");
    cout << RESET << endl;
    system("cls");
}

// --------------------------------------------------------------------------------------------- //
/*                                       Listar las Reservas                                     */
// --------------------------------------------------------------------------------------------- //

void listar_reservas()
{
    // Obtengo una lista con los aeropuertos.
    auto it = aeropuertos.begin();

    Vuelo vuelo(false, 0);

    list<Grafo<Vuelo>::Arco> destinos;

    list<string> aerolineas;

    cout << CYAN << "Se registraron las siguientes reservas en nuestro sistema: \n" << RESET << endl;

    while (it != aeropuertos.end())
    {
        // Busco los aeropuertos destinos a partir del origen.
        destinos.clear();

        redDeViajes.devolver_adyacentes((it->second).verId(), destinos);

        auto it_destinos = destinos.begin();

        while (it_destinos != destinos.end())
        {
            vuelo = (*it_destinos).devolver_costo();

            vuelo.verAerolineas(aerolineas);

            auto it_aerolineas = aerolineas.begin();

            while (it_aerolineas != aerolineas.end())
            {
                if (vuelo.verAsientosReservados(*it_aerolineas) != 0) // Es decir, si hay reservas
                {
                    cout << CYAN << "- " << RESET << "Vuelo de " << CYAN << (it->first) << RESET
                    << " a " << CYAN << (*it_destinos).devolver_adyacente() << RESET
                    << ", a través de la aerolínea " << CYAN << *it_aerolineas << RESET
                    << " con un total de " << CYAN << vuelo.verAsientosReservados(*it_aerolineas) << RESET
                    << "/" << CYAN << vuelo.verAsientosTotales(*it_aerolineas) << RESET << endl;
                }
                it_aerolineas++;
            }

            it_destinos++;
        }

        it++;
    }

    cout << RED << endl;
    system("pause");
    cout << RESET << endl;
    system("cls");
}

// --------------------------------------------------------------------------------------------- //
/*                                        Solicitar Datos                                        */
// --------------------------------------------------------------------------------------------- //

//void solicitar_datos(string & origen, string & destino, string & aerolinea)
//{
//    cout << CYAN << "Proporcione los siguientes datos: \n" << RESET << endl;
//
//    cout << CYAN << "- " << RESET << "Origen: ";
//    cin.ignore();
//    getline(cin, origen);
//    cout << '\n';
//
//    cout << CYAN << "- " << RESET << "Destino: ";
//    getline(cin, destino);
//    cout << '\n';
//
//    cout << CYAN << "- " << RESET << "Aerolinea: ";
//    getline(cin, aerolinea);
//    system("cls");
//}
//
//void solicitar_datos(string & origen, string & destino)
//{
//    cout << CYAN << "Proporcione los siguientes datos: \n" << RESET << endl;
//
//    cout << CYAN << "- " << RESET << "Origen: ";
//    cin.ignore();
//    getline(cin, origen);
//    cout << '\n';
//
//    cout << CYAN << "- " << RESET << "Destino: ";
//    getline(cin, destino);
//    system("cls");
//}
//
//// --------------------------------------------------------------------------------------------- //
///*                           Verificar si Existe un Vuelo Directo                                */
//// --------------------------------------------------------------------------------------------- //
//
//void existe_vuelo_directo()
//{
//    string origen, destino, aerolinea;
//
//    solicitar_datos(origen, destino, aerolinea);
//
//    double distancia;
//    unsigned int asientos_disponibles;
//
//    if (redDeViajes.existe_vuelo_directo_aerolinea(origen, destino, aerolinea, distancia, asientos_disponibles))
//    {
//        cout << "Se encontró un viaje de " << CYAN << origen << RESET << " a " << CYAN << destino << RESET
//        << " a través de " << CYAN << aerolinea << RESET << ": \n" << endl
//        << "Distancia de: " << CYAN << distancia << RESET << " kilómetros. \n" << endl
//        << "Asientos disponibles: " << CYAN << asientos_disponibles << RESET << " asientos disponibles." << endl;
//    }
//    else
//        cout << RED << "No existe" << RESET << " un vuelo desde " << RED << origen << RESET << " a " << RED << destino
//        << RESET << " a través de " <<  RED << aerolinea << RESET << endl;
//
//    cout << RED << endl;
//    system("pause");
//    cout << RESET << endl;
//    system("cls");
//}
//
//// --------------------------------------------------------------------------------------------- //
///*                     Mostrar Vuelos a través de una Misma Aerolínea                            */
//// --------------------------------------------------------------------------------------------- //
//
//void mostrar_vuelos_misma_aerolinea()
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
//    // Obtengo las aerolíneas.
//
//    viajes.devolver_aerolineas(aerolineas);
//
//    auto it_aerolineas = aerolineas.begin();
//
//    // Muestro los vuelos...
//    cout << "Estos son todos los vuelos del aeropuerto " << CYAN << origen << RESET
//    << " al aeropuerto " << CYAN << destino << RESET << " a través de: \n" << endl;
//
//    while (it_aerolineas != aerolineas.end())
//    {
//        cout << CYAN << "-----------------------------------------------------------------------------------------\n" << RESET << endl;
//
//        cout << "Aerolínea " << CYAN << *it_aerolineas << RESET << ": \n" << endl;
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
//// ------------------------------------Búsqueda de Caminos-------------------------------------- //
//
//void busqueda_caminos(const string & origen, const string & destino, const string & aerolinea,
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
//                    //En caso que no haya asientos libres se devolverá un 0, y si la aerolinea no se encuentra un -1.
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
//void imprimir_camino(const string & destino, const double & distancia, const unsigned int & escalas, const list<string> & camino)
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
//    cout << "Distancia: " << distancia << " kilómetros." << "\n" << endl;
//}
