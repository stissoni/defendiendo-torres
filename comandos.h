#ifndef __COMANDOS_H__
#define __COMANDOS_H__

#include "defendiendo_torres.h"

#define MAX_NOMBRE 100
#define MAX_VECTOR 100
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
#define FORMATO "%[^;];%i\n"

void ejecutar_comando_poneme_la_repe(int argc, char* argv[]);
void ejecutar_comando_crear_configuracion(int argc, char* argv[]);
void ejecutar_comando_crear_camino(int argc, char* argv[]);
void crear_caminos(char nombre_archivo_camino[MAX_NOMBRE]);
void ejecutar_comando_ranking(int argc, char* argv[]);
void mostrar_ranking(int numero_jugadores_a_mostrar, char nombre_archivo_configuracion[MAX_NOMBRE]);

// Defendiendo torres

void imprimir_numeros(int columnas_terreno);
void imprimir_terreno(char terreno[MAX_TERRENO][MAX_TERRENO], int filas_terreno, int columnas_terreno);
void imprimir_lineas(int columnas_terreno);
void cargar_tamanio_terreno(juego_t juego, int* filas_terreno, int* columnas_terreno);
void cargar_camino(juego_t juego, char terreno[MAX_TERRENO][MAX_TERRENO],int camino);
void inicializar_terreno(char terreno[MAX_TERRENO][MAX_TERRENO], int filas_terreno, int columnas_terreno);

typedef struct configuracion{
    int resistencia_torres[MAX_VECTOR];
    int enanos_iniciales[MAX_VECTOR];
    int elfos_iniciales[MAX_VECTOR];
    int enanos_extra[MAX_VECTOR];
    int elfos_extra[MAX_VECTOR];
    int animo_enanos[MAX_VECTOR];
	int animo_elfos[MAX_VECTOR];
    float velocidad;
    char archivo_caminos[MAX_NOMBRE];
} configuracion_t;

#endif