#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <stdexcept>

using namespace std;

const string NOMBRE_ARCHIVO = "jugadores.txt"; // Declaraci�n de archivo

const int FILAS = 25; // declaraci�n de los elementos en las matrices a utilizar
const int COLUMNAS = 3;

// Funci�n para imprimir una fila de la matriz
void imprimirFila(const string fila[COLUMNAS]) {
    for (int j = 0; j < COLUMNAS; ++j) {
        cout << fila[j] << " ";
    }
    cout << endl;
}

// Funci�n para obtener la pregunta de una fila de la matriz
string obtenerPregunta(const string fila[COLUMNAS]) {
    return fila[1];
}

// Funci�n para obtener la respuesta de una fila de la matriz
string obtenerRespuesta(const string fila[COLUMNAS]) {
    return fila[2];
}

// Funci�n para generar un n�mero aleatorio �nico
int generarNumeroAleatorioUnico(vector<int>& numerosDisponibles) {
    if (numerosDisponibles.empty()) {
        cerr << "Error: Se agotaron los n�meros aleatorios �nicos." << endl;
        exit(1);
    }

    // Selecciona un n�mero aleatorio de los disponibles
    int indiceAleatorio = rand() % numerosDisponibles.size();
    int numeroAleatorio = numerosDisponibles[indiceAleatorio];

    // Elimina el n�mero seleccionado de los disponibles
    numerosDisponibles.erase(numerosDisponibles.begin() + indiceAleatorio);

    return numeroAleatorio;
}

// Funci�n para verificar si el jugador ya est� registrado
bool jugadorRegistrado(const string& nombre, const string& cedula) {
    ifstream archivoEntrada(NOMBRE_ARCHIVO);

    if (!archivoEntrada.is_open()) { // Se abre archivo de lectura
        cerr << "Error al abrir el archivo para lectura." << endl;
        return false;
    }

    string nombreExistente, cedulaExistente;
    int puntajeExistente;

    while (archivoEntrada >> cedulaExistente >> nombreExistente >> puntajeExistente) { // verificaci�n de registro con c�dula
        if (cedulaExistente == cedula) {
            archivoEntrada.close();
            return true; // El jugador ya est� registrado
        }
    }

    archivoEntrada.close();
    return false; // El jugador no est� registrado
}

// Funci�n para actualizar o registrar un nuevo jugador
void actualizarORegistrarJugador(const string& nombre, const string& cedula, int& puntajeGlobal) {
    if (jugadorRegistrado(nombre, cedula)) {
        // Abrir el archivo para lectura y escritura
        ifstream archivoEntrada(NOMBRE_ARCHIVO);
        ofstream archivoSalida("temp.txt");

        if (!archivoEntrada.is_open() || !archivoSalida.is_open()) {
            cerr << "Error al abrir los archivos." << endl;
            return;
        }

        string cedulaExistente, nombreExistente;
        int puntajeExistente;

        while (archivoEntrada >> cedulaExistente >> nombreExistente >> puntajeExistente) {
            if (cedulaExistente == cedula) {
                // Actualizar el puntaje existente
                puntajeExistente += puntajeGlobal;
                puntajeGlobal += puntajeExistente; // Actualizar el puntaje global
            }

            // Escribir la l�nea en el nuevo archivo
            archivoSalida << cedulaExistente << " " << nombreExistente << " " << puntajeExistente << endl;
        }

        archivoEntrada.close();
        archivoSalida.close();

        // Renombrar el nuevo archivo como el original
        remove(NOMBRE_ARCHIVO.c_str());
        rename("temp.txt", NOMBRE_ARCHIVO.c_str());

        cout << "Puntaje actualizado para el jugador " << nombre << endl;
    } else {
        // La c�dula no existe, agregar un nuevo jugador
        ofstream archivoSalida(NOMBRE_ARCHIVO, ios::app);

        if (!archivoSalida.is_open()) {
            cerr << "Error al abrir el archivo para escritura." << endl;
            return;
        }

        archivoSalida << cedula << " " << nombre << " " << puntajeGlobal << endl;
        puntajeGlobal += puntajeGlobal; // Actualizar el puntaje global
        cout << "Nuevo jugador registrado." << endl;

        archivoSalida.close();
    }
}

// Funci�n para imprimir los nombres y c�dulas de los jugadores
void imprimirJugadores() {
    ifstream archivoEntrada(NOMBRE_ARCHIVO);

    if (!archivoEntrada.is_open()) {
        cerr << "Error al abrir el archivo para lectura." << endl;
        return;
    }

    string cedula, nombre;
    int puntaje;

    cout << "Listado de Jugadores:" << endl;
    while (archivoEntrada >> cedula >> nombre >> puntaje) {
        cout << "C�dula: " << cedula << ", Nombre: " << nombre << ", Puntaje: " << puntaje << endl;
    }

    archivoEntrada.close();
}

// Funci�n para obtener los 10 mejores puntajes
void obtenerMejoresPuntajes() {
    ifstream archivoEntrada(NOMBRE_ARCHIVO);

    if (!archivoEntrada.is_open()) {
        cerr << "Error al abrir el archivo para lectura." << endl;
        return;
    }

    vector<vector<string>> jugadores;

    string cedula, nombre;
    int puntaje;

    while (archivoEntrada >> cedula >> nombre >> puntaje) {
        vector<string> jugador = {cedula, nombre, to_string(puntaje)};
        jugadores.push_back(jugador);
    }

    archivoEntrada.close();

    sort(jugadores.begin(), jugadores.end(), [](const vector<string>& a, const vector<string>& b) {
        return stoi(a[2]) > stoi(b[2]);
    });

    cout << "Los 10 mejores puntajes:" << endl;
    for (int i = 0; i < min(10, static_cast<int>(jugadores.size())); ++i) {
        cout << "C�dula: " << jugadores[i][0] << ", Nombre: " << jugadores[i][1] << ", Puntaje: " << jugadores[i][2] << endl;
    }
}

// Funci�n para validar que una cadena sea un n�mero
bool esNumero(const string& cadena) {
    return all_of(cadena.begin(), cadena.end(), ::isdigit);
}


// Funci�n para obtener una c�dula v�lida (n�mero) mediante manejo de excepciones
string obtenerCedula() {
    string cedula;
    while (true) {
        try {
            cout << "Ingrese la c�dula del nuevo jugador: ";
            cin >> cedula;

            // Verificar si la c�dula es un n�mero y tiene al menos 9 d�gitos
            if (!esNumero(cedula)) {
                throw invalid_argument("C�dula no v�lida. Debe ser un n�mero.");
            }

            if (cedula.length() < 9) {
                throw invalid_argument("C�dula no v�lida. Debe tener al menos 9 d�gitos.");
            }

            break; // Si llegamos aqu�, la c�dula es v�lida, salimos del bucle

        } catch (const invalid_argument& e) {
            cin.clear(); // Limpiar el estado de error de cin
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Descartar la entrada inv�lida
            cerr << e.what() << " Int�ntelo nuevamente." << endl;
        }
    }
    return cedula;
}
void leerArchivo(const string& nombreArchivo, string datos[FILAS][COLUMNAS]) {
    ifstream archivoEntrada(nombreArchivo);

    if (!archivoEntrada.is_open()) {
        cerr << "Error al abrir el archivo para lectura." << endl;
        return;
    }

    for (int i = 0; i < FILAS; ++i) {
        string linea;
        if (getline(archivoEntrada, linea)) {
            istringstream stream(linea);
            for (int j = 0; j < COLUMNAS; ++j) {
                // Utilizar getline con espacio como delimitador para leer la cadena completa
                getline(stream, datos[i][j], ' ');
            }
        }
    }

    archivoEntrada.close();
}
bool evaluaRespuesta(string respuestaArreglo, string respuestaUsuario) { // tal cu�l se indica en el documento, para evaluar respueta
// Se recibe como par�metro del arreglo datosle�dos, pero el mismo arreglo lo que hace es extraer datos del archivo.

    return respuestaArreglo == respuestaUsuario;
}

int main() {
    setlocale(LC_ALL, ""); // manejo de lenguaje
    char Guardar; //


    char rpt = 's';
    string nombre, cedula;

    while (rpt == 's' || rpt == 'S') {
        int option;
        bool opcionValida = false;

        while (!opcionValida) {
            cout << "Men� Principal\n"
                 << "1. Ingresar jugador\n"
                 << "2. Jugar �Qu� tanto sabes de las partes del cuerpo?\n"
                 << "3. Reporte de jugadores\n"
                 << "4. Los 10 mejores jugadores\n"
                 << "Ingrese una opci�n del Men�: ";

            if (!(cin >> option)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Entrada inv�lida. Ingrese un n�mero v�lido." << endl;
            } else {
                if (option >= 1 && option <= 4) {
                    opcionValida = true;
                }
            }
            system("cls");
        }

        switch (option) {
            case 1: {
                cedula = obtenerCedula();
                cout << "Ingrese el nombre del nuevo jugador: ";
                cin >> nombre;

                bool jugadorExistente = jugadorRegistrado(nombre, cedula);
                cout << (jugadorExistente ? "El jugador ya est� registrado." : "El jugador no est� registrado.") << endl;
                system("pause");
                system("cls");
            }
            break;


            case 2:  if(cedula.empty()) {
               break;
            }
            {int cambiosRealizados = 0;
                int incorrectas = 0;
                string datosLeidos[FILAS][COLUMNAS];

                int puntaje = 0;
                int puntajeGlobal = 0;

            do{ // Array bidimensional original
    string datos[FILAS][COLUMNAS] = { // Este arreglo se puede poner como un comentario, el



            {"1", "Es_la_parte_intermedia_del_cuerpo_desde_donde_se_puede_conectar_con_la_cabeza_a_trav�s_del_cuello_y_llega_hasta_la_zona_de_la_ingle", "tronco"},
            {"2", "Es_por_donde_ingresan_los_alimentos_a_nuestro_cuerpo", "boca"},
            {"3", "Se_ubican_en_la_frente,_sobre_los_ojos,_est�n_formadas_por_vellos_o_pelos_peque�os", "ceja"},
            {"4", "Es_parte_de_la_piel_de_la_cara_o_el_rostro", "mejilla"},
            {"5", "Parte_baja_de_la_cara._Se_encuentra_debajo_de_la_boca", "barbilla"},
{"6", "Es_por_donde_entra_el_aire_al_cuerpo,_puedes_oler_la_comida_y_est�_sobre_la_boca._Aqu�_est�_el_sentido_del_olfato.", "nariz"},
{"7", "Son_por_donde_miras,_se_encuentran_en_el_rostro_o_cara_y_ah�_est�_el_sentido_de_la_visi�n", "ojos"},
{"8", "�rganos_que_est�n_a_los_dos_lados_de_la_cabeza_y_sirve_para_escuchar_(o�r)_sonidos", "oreja"},
{"9", "Es_la_parte_superior_de_nuestro_cuerpo_donde_se_encuentra_el_cerebro", "cabeza"},
{"10", "Es_la_uni�n_de_la_cabeza_con_el_cuerpo", "cuello"},
{"11", "Est�n_dentro_de_la_boca_y_son_para_cortar_los_alimentos", "dientes"},
{"12", "Es_la_parte_superior_del_rostro", "frente"},
{"13", "Es_el_conducto_por_donde_pasan_los_alimentos_hacia_el_est�mago", "garganta"},
{"14", "Es_el_�rgano_en_donde_tenemos_el_sentido_del_gusto._Se_usa_para_lamer_y_articular_palabras", "lengua"},
{"15", "Est�n_dentro_de_la_boca_en_la_parte_superior_e_inferior;_y_son_para_triturar_/_moler_los_alimentos", "muelas"},
{"16", "Cobertura_externa_de_los_ojos,_se_abren_y_se_cierran_para_lubricar_el_ojo", "parpados"},
{"17", "Son_vellos_que_est�n_en_los_p�rpados_y_sirven_para_proteger_al_ojo", "pestanas"},
{"18", "Parte_inferior_de_las_piernas_y_con_el_cual_tocamos_el_suelo", "pie"},
{"19", "Extremidad_inferior_del_cuerpo_con_la_cual_nos_movilizamos", "pierna"},
{"20", "Articulaci�n_media_de_la_pierna", "rodilla"},
{"21", "Articulaci�n_inferior_de_la_pierna_que_conecta_con_el_pie_y_le_da_movilidad.", "tobillo"},
{"22","Es_la_parte_inferior_de_la_cintura_y_define_la_parte_media_entre_el_t�rax_y_las_piernas", "cadera"},
{"23", "Est�_en_la_parte_inferior_entre_el_brazo_y_el_tronco", "axila"},
{"24", "Conducto_cerrado_por_donde_el_ser_humano_se_alimentaba_antes_de_nacer", "ombligo"},
{"25", "Es_el_hueso_m�s_largo_del_cuerpo_humano,_est�_en_la_pierna", "femur"}};//

   // Abre el archivo para escritura
  ofstream archivoSalida("archivo.txt");

  // Itera a trav�s de cada fila del arreglo
  for (int i = 0; i < FILAS; ++i) {
    // Concatena los elementos del arreglo en una sola cadena de caracteres
    string cadena;
    for (int j = 0; j < COLUMNAS; ++j) {
      cadena += datos[i][j] + " ";
    }

    // Escribe la cadena de caracteres en el archivo
    archivoSalida << cadena << endl;
  }

  // Cierra el archivo
  archivoSalida.close();

  cout<<"Iniciaremos el juego, por favor escriba las respuestas en min�scula y sin tildes, sustituya las '�' con 'n' "<<endl<<endl;


            leerArchivo("archivo.txt", datosLeidos);
        srand(time(0));


        // Inicializa un vector con n�meros del 1 al 25
        vector<int> numerosDisponibles = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25};


        for (int i = 0; i < 5; ++i) {
                if (incorrectas ==4) {
            goto fin;}
            int puntajeGlobal = 0;

                cout << "Preguntas Correctas" <<" "<<  puntaje << endl <<endl;
                cout << "Preguntas incorrectas" <<" "<< incorrectas << endl <<endl;
            // Genera un n�mero aleatorio �nico
            int numeroAleatorio = generarNumeroAleatorioUnico(numerosDisponibles);
            int j = numeroAleatorio - 1; // Ajusta el �ndice para usarlo en el array

            // Ciclo while para permitir dos cambios de pregunta
            // Obtiene la pregunta
            string pregunta = datosLeidos[j][1];
            cout << "Pregunta extra�da: " << pregunta << endl <<endl;


            if (cambiosRealizados < 2) {
                // Pregunta al usuario si desea cambiar de pregunta
                char deseaCambiar;
                cout << "�Desea cambiar de pregunta? (S/N): "<< endl;
                cin >> deseaCambiar;

                if (deseaCambiar == 'S' || deseaCambiar == 's') {
                    ++cambiosRealizados;
                    i--;
                    continue; // Salta a la siguiente iteraci�n del bucle while
                    system("cls");
                }
            }

            // Si no cambia, imprimir la pregunta nuevamente no es necesario
            string respuesta = datosLeidos[j][2];

            string respuestaUsuario;
            cout << "Respuesta: ";
            cin >> respuestaUsuario;

            // Si la respuesta del usuario es correcta, muestra la puntuaci�n
            if (evaluaRespuesta(respuesta, respuestaUsuario)) {
                cout << "Respuesta correcta!" << endl;
                // Actualiza la puntuaci�n
                puntaje++;
            } else {
                // Si la respuesta del usuario es incorrecta, muestra la puntuaci�n
                cout << "Respuesta incorrecta." << endl;
                incorrectas++;
            }

            if (puntaje == 10) {
            cout << "Bonus por 10 preguntas correctas!" << endl << endl;
            puntaje = 15; }

            if (puntaje == 20) {
            cout << "Bonus por 15 preguntas correctas!" << endl << endl;
            puntaje = 20;
             }

             if (puntaje == 25){
                        system("cls");

                    cout<<"Felicidades, has ganado el gran premio de 25 puntos"<<endl<<endl;

                    puntaje=50;

                }




            system("cls");
            }








                puntajeGlobal = puntaje;
                actualizarORegistrarJugador(nombre, cedula, puntajeGlobal);
                system("cls");



                cout<<"Deseas continuar jugando: "<<endl<<endl;

                cin>>Guardar;

                 }while (Guardar == 'S' || Guardar == 's');


                fin:
                    system("cls");

                    if (incorrectas=4){

                    cout<<"Has perdido"<<endl<<endl;}
                    system("pause");

                system("cls");}

                break;

                case 3: {
                imprimirJugadores();
                system("pause");
                system("cls");
            }
            break;

            case 4: {
                obtenerMejoresPuntajes();
                system("pause");
                system("cls");
            }
            break;

            default:
                cout << "Opci�n inv�lida. Por favor, ingrese una opci�n v�lida." << endl;
                system("pause");
                system("cls");
        }
    }

    return 0;
}
