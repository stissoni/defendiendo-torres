#ifndef __COMANDOS_H__
#define __COMANDOS_H__

#include "defendiendo_torres.h"

#define POR_DEFECTO -1
#define CAMINO_POR_DEFECTO "-1"
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
#define VELOCIDAD_POR_DEFECTO 0.2

void ejecutar_comando_poneme_la_repe(int argc, char* argv[]);
void ejecutar_comando_crear_configuracion(int argc, char* argv[]);
void ejecutar_comando_crear_camino(int argc, char* argv[]);
void ejecutar_comando_ranking(int argc, char* argv[]);
bool obtener_configuracion(char nombre_archivo_configuracion[MAX_NOMBRE], configuracion_t* configuracion);
void cargar_configuracion_por_defecto(configuracion_t* configuracion);
void obtener_camino_creado(FILE* archivo_caminos, coordenada_t camino_1[MAX_LONGITUD_CAMINO],coordenada_t camino_2[MAX_LONGITUD_CAMINO], int* tope_camino_1, int* tope_camino_2);
void obtener_puntaje(configuracion_t configuracion, int enemigos_muertos, int* puntaje, char nombre_jugador[MAX_NOMBRE], char nombre_archivo_configuracion[MAX_NOMBRE]);

// Defendiendo torres

void imprimir_numeros(int columnas_terreno);
void imprimir_terreno(char terreno[MAX_TERRENO][MAX_TERRENO], int filas_terreno, int columnas_terreno);
void imprimir_lineas(int columnas_terreno);
void cargar_tamanio_terreno(juego_t juego, int* filas_terreno, int* columnas_terreno);
void cargar_camino(juego_t juego, char terreno[MAX_TERRENO][MAX_TERRENO],int camino);
void inicializar_terreno(char terreno[MAX_TERRENO][MAX_TERRENO], int filas_terreno, int columnas_terreno);
int enemigos_muertos(nivel_t nivel);

#endif