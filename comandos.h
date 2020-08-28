#ifndef __COMANDOS_H__
#define __COMANDOS_H__

#include "defendiendo_torres.h"

#define POR_DEFECTO -1
#define CAMINO_POR_DEFECTO "-1"
#define CONFIGURACION_POR_DEFECTO "-1"
#define MAX_TERRENO 100
#define MOSTRAR_TODOS 999999
#define ARRIBA 'w'
#define ABAJO 's'
#define IZQUIERDA 'a'
#define DERECHA 'd'
#define RANKING "ranking"
#define CREAR_CAMINO "crear_camino"
#define CREAR_CONFIGURACION "crear_configuracion"
#define PONEME_LA_REPE "poneme_la_repe"
#define JUGAR "jugar"
#define CONFIG "config="
#define LISTAR "listar="
#define GRABACION "grabacion="
#define FORMATO "%[^;];%i\n"
#define DEFENSORES_NIVEL_1 5
#define DEFENSORES_NIVEL_2 5
#define DEFENSORES_NIVEL_3 6
#define DEFENSORES_NIVEL_4 8
#define DEFENSORES_EXTRA 10
#define DANIO_DEFENSOR_EXTRA 50
#define VELOCIDAD_POR_DEFECTO (float) 0.2
#define LECTURA "r"
#define ESCRITURA "w"

/* Ejecuta el comando poneme_la_repe. Se le debe pasar el archivo . Se debe pasar el nombre del archivo de la partida grabada.
 * La velocidad de reproduccion es opcional.
 */
void ejecutar_comando_poneme_la_repe(int argc, char* argv[]);

/* Ejecuta el comando crear_configuracion. Se debe pasar el nombre del archivo de configuracion.
 *
 */
void ejecutar_comando_crear_configuracion(int argc, char* argv[]);

/* Ejecuta el comando crear_camino. Se debe pasar el nombre del archivo de caminos.
 *
 */
void ejecutar_comando_crear_camino(int argc, char* argv[]);

/* Ejecuta el comando mostrar_ranking. Parametros listar y archivo de ranking opcionales.
 *
 */
void ejecutar_comando_ranking(int argc, char* argv[]);

/* Recibe el nombre del archivo de configuracion y el struct que controla la configuracion de la partida.
 * Abre el archivo de configuracion y carga los datos del archivo en el struct de configuracion.
 * Devuelve true en caso de poder cargar la configuracion, false en caso contrario. 
 */ 
bool obtener_configuracion(char nombre_archivo_configuracion[MAX_NOMBRE], configuracion_t* configuracion);

/* Recibe el struct de configuracion sin inicializar, y carga la configuracion por defecto del juego.
 *
 */
void cargar_configuracion_por_defecto(configuracion_t* configuracion);

/* Recibe el archivo de caminos con los caminos guardados, y los carga en los vectores de caminos de la partida.
 * Actualiza los topes.
 */
void obtener_camino_creado(FILE* archivo_caminos, coordenada_t camino_1[MAX_LONGITUD_CAMINO],coordenada_t camino_2[MAX_LONGITUD_CAMINO], int* tope_camino_1, int* tope_camino_2);

/* Calcula el puntaje final de la partida. Lo guarda en el archivo pasado como parametro.
 *
 */
void obtener_puntaje(configuracion_t configuracion, int enemigos_muertos, int* puntaje, char nombre_jugador[MAX_NOMBRE], char nombre_archivo_configuracion[MAX_NOMBRE]);

/* Graba la partida en el archivo dado como parametro.
 *
 */
void grabar_partida(juego_t juego, FILE* archivo_grabacion);

// Defendiendo torres

void imprimir_numeros(int columnas_terreno);
void imprimir_terreno(char terreno[MAX_TERRENO][MAX_TERRENO], int filas_terreno, int columnas_terreno);
void imprimir_lineas(int columnas_terreno);
void cargar_tamanio_terreno(juego_t juego, int* filas_terreno, int* columnas_terreno);
void cargar_camino(juego_t juego, char terreno[MAX_TERRENO][MAX_TERRENO],int camino);
void inicializar_terreno(char terreno[MAX_TERRENO][MAX_TERRENO], int filas_terreno, int columnas_terreno);
int enemigos_muertos(nivel_t nivel);

#endif