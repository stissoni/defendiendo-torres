#include <stdbool.h>
#include <time.h>
#include <stdlib.h>
#include "defendiendo_torres.h"

#define BUENO 'B'
#define REGULAR 'R'
#define MALO 'M'
#define RESISTENCIA_INICIAL_TORRE 600
#define RESISTENCIA_TORRE_DESTRUIDA 0
#define DEFENSORES_EXTRA 10
#define PROBABILIDAD_CRITICO_25 25
#define PROBABILIDAD_CRITICO_10 10
#define PROBABILIDAD_CRITICO_0 0
#define NIVEL_1 1
#define NIVEL_2 2
#define NIVEL_3 3
#define NIVEL_4 4
#define CAMINO_1 1
#define CAMINO_2 2 
#define CAMINO ' '
#define ENANO 'G'
#define ELFO 'L'
#define ORCO 'O'
#define ENTRADA 'E'
#define POSICION_ENTRADA 0
#define TORRE 'T'
#define VACIO 126
#define FALLO_AGREGAR_DEFENSOR -1
#define EXITO_AGREGAR_DEFENSOR 0
#define JUEGO_GANADO 1
#define JUEGO_JUGANDO 0
#define JUEGO_PERDIDO -1
#define NIVEL_GANADO 1
#define NIVEL_JUGANDO 0
#define DANIO_BASE_ENANO 60
#define DANIO_BASE_ELFO 30
#define EXTRA_GOLPRE_CRITICO 40
#define FALLO_ATAQUE 0
#define VIDA_BASICA_ORCO 200
#define VIDA_ORCO_MUERTO 0
#define POSICION_INICIAL_ORCO 0
#define VIDA_ORCO_EXTRA_MAX 100

int enemigos_muertos(nivel_t nivel){
    int enemigos_muertos = 0;
    for (int i = 0; i < nivel.tope_enemigos; i++){
        if (nivel.enemigos[i].vida <= VIDA_ORCO_MUERTO){
            enemigos_muertos++;
        }
    }
    return enemigos_muertos;
}

void inicializar_juego(juego_t* juego, int viento, int humedad, char animo_legolas, char animo_gimli){
    (*juego).torres.resistencia_torre_1 = RESISTENCIA_INICIAL_TORRE;
    (*juego).torres.resistencia_torre_2 = RESISTENCIA_INICIAL_TORRE;
    (*juego).torres.elfos_extra = DEFENSORES_EXTRA;
    (*juego).torres.enanos_extra = DEFENSORES_EXTRA;
    (*juego).fallo_legolas = viento / 2;
    (*juego).fallo_gimli = humedad / 2;
    if (animo_gimli == BUENO){
        (*juego).critico_gimli = PROBABILIDAD_CRITICO_25;
    }
    if (animo_gimli == REGULAR){
        (*juego).critico_gimli = PROBABILIDAD_CRITICO_10;
    }
    if (animo_gimli == MALO){
        (*juego).critico_gimli = PROBABILIDAD_CRITICO_0;
    }
    if (animo_legolas == BUENO){
        (*juego).critico_legolas = PROBABILIDAD_CRITICO_25;
    }
    if (animo_legolas == REGULAR){
        (*juego).critico_legolas = PROBABILIDAD_CRITICO_10;
    }
    if (animo_legolas == MALO){
        (*juego).critico_legolas = PROBABILIDAD_CRITICO_0;
    }
}

void mostrar_juego(juego_t juego){
    int filas_terreno, columnas_terreno;
    if (juego.nivel_actual == NIVEL_1 || juego.nivel_actual == NIVEL_2){
        filas_terreno = 15;
        columnas_terreno = 15;
    }
    if (juego.nivel_actual == NIVEL_3 || juego.nivel_actual == NIVEL_4){
        filas_terreno = 20;
        columnas_terreno = 20;
    }

    char terreno[filas_terreno][columnas_terreno];
    // Cargar el terreno
    for (int fil = 0; fil < filas_terreno; fil++){
        for (int col = 0; col < columnas_terreno; col++){
            terreno[fil][col] = VACIO;
        }
    }
    /* ................... CAMINOS ................... */
    // Cargar el camino 1 para el nivel 1
    if (juego.nivel_actual == NIVEL_1){
        for (int i = 0; i < juego.nivel.tope_camino_1; i++){
            terreno[juego.nivel.camino_1[i].fil][juego.nivel.camino_1[i].col] = CAMINO;
            terreno[juego.nivel.camino_1[POSICION_ENTRADA].fil][juego.nivel.camino_1[POSICION_ENTRADA].col] = ENTRADA;
            terreno[juego.nivel.camino_1[juego.nivel.tope_camino_1-1].fil][juego.nivel.camino_1[juego.nivel.tope_camino_1-1].col] = TORRE;
        }
    }
    // Cargar el camino 2 para el nivel 2
    if (juego.nivel_actual == NIVEL_2){
        for (int i = 0; i < juego.nivel.tope_camino_2; i++){
            terreno[juego.nivel.camino_2[i].fil][juego.nivel.camino_2[i].col] = CAMINO;
            terreno[juego.nivel.camino_2[POSICION_ENTRADA].fil][juego.nivel.camino_2[POSICION_ENTRADA].col] = ENTRADA;
            terreno[juego.nivel.camino_2[juego.nivel.tope_camino_2-1].fil][juego.nivel.camino_2[juego.nivel.tope_camino_2-1].col] = TORRE;
        }
    }
    // Cargar el camino 1 y 2 para el nivel 3
    if (juego.nivel_actual == NIVEL_3 || juego.nivel_actual == NIVEL_4){
        for (int i = 0; i < juego.nivel.tope_camino_1; i++){
            terreno[juego.nivel.camino_1[i].fil][juego.nivel.camino_1[i].col] = CAMINO;
            terreno[juego.nivel.camino_1[POSICION_ENTRADA].fil][juego.nivel.camino_1[POSICION_ENTRADA].col] = ENTRADA;
            terreno[juego.nivel.camino_1[juego.nivel.tope_camino_1-1].fil][juego.nivel.camino_1[juego.nivel.tope_camino_1-1].col] = TORRE;
        }
        for (int i = 0; i < juego.nivel.tope_camino_2; i++){
            terreno[juego.nivel.camino_2[i].fil][juego.nivel.camino_2[i].col] = CAMINO;
            terreno[juego.nivel.camino_2[POSICION_ENTRADA].fil][juego.nivel.camino_2[POSICION_ENTRADA].col] = ENTRADA;
            terreno[juego.nivel.camino_2[juego.nivel.tope_camino_2-1].fil][juego.nivel.camino_2[juego.nivel.tope_camino_2-1].col] = TORRE;
        }
    }
    /* ................... DEFENSORES ................... */
    for (int defensor = 0; defensor < juego.nivel.tope_defensores; defensor++){
        if (juego.nivel.defensores[defensor].posicion.fil != 42 && juego.nivel.defensores[defensor].posicion.col != 42){
            terreno[juego.nivel.defensores[defensor].posicion.fil][juego.nivel.defensores[defensor].posicion.col] = juego.nivel.defensores[defensor].tipo;
        }
    }
    /* ................... ENEMIGOS ................... */
    for (int enemigo = 0; enemigo < juego.nivel.tope_enemigos; enemigo++){
        if (juego.nivel.enemigos[enemigo].camino == CAMINO_1 && juego.nivel.enemigos[enemigo].pos_en_camino >= POSICION_INICIAL_ORCO && juego.nivel.enemigos[enemigo].pos_en_camino < juego.nivel.tope_camino_1 && juego.nivel.enemigos[enemigo].vida > VIDA_ORCO_MUERTO){
            terreno[juego.nivel.camino_1[juego.nivel.enemigos[enemigo].pos_en_camino].fil][juego.nivel.camino_1[juego.nivel.enemigos[enemigo].pos_en_camino].col] = ORCO;
        }
        if (juego.nivel.enemigos[enemigo].camino == CAMINO_2 && juego.nivel.enemigos[enemigo].pos_en_camino >= POSICION_INICIAL_ORCO && juego.nivel.enemigos[enemigo].pos_en_camino < juego.nivel.tope_camino_2 && juego.nivel.enemigos[enemigo].vida > VIDA_ORCO_MUERTO){
            terreno[juego.nivel.camino_2[juego.nivel.enemigos[enemigo].pos_en_camino].fil][juego.nivel.camino_2[juego.nivel.enemigos[enemigo].pos_en_camino].col] = ORCO;
        }
    }
  
    /* ................... IMPRIMIENDO EL TERRENO ................... */
    printf("    ");
    for (int i = 0; i < columnas_terreno; i++){
        if (i == 0){
            printf("  0 ");
        }
        else{
            printf(" %2.i ", i);
        }
    }
    printf("\n    ");
    for (int i = 0; i < columnas_terreno; i++){
        printf(" -- ");
    }    
    printf("\n");
    for (int fil = 0; fil < filas_terreno; fil++){
        for (int col = 0; col < columnas_terreno; col++){
            if (col == 0){
                if (fil == 0){
                    if (terreno[fil][col] == ORCO){
                        printf(" 0 |   %c ", terreno[fil][col]);
                    }
                    else {
                        printf(" 0 | %c%c ", terreno[fil][col], terreno[fil][col]);
                    }
                }
                else {
                    if (terreno[fil][col] == ELFO || terreno[fil][col] == ENANO || terreno[fil][col] == ORCO || terreno[fil][col] == TORRE || terreno[fil][col] == ENTRADA){
                        printf("%2.i |  %c ", fil, terreno[fil][col]);
                    }
                    else {
                        printf("%2.i | %c%c ",fil, terreno[fil][col], terreno[fil][col]);
                    }
                }
            }
            else {
                if (terreno[fil][col] == ELFO || terreno[fil][col] == ENANO || terreno[fil][col] == ORCO || terreno[fil][col] == TORRE || terreno[fil][col] == ENTRADA){
                    printf("  %c ", terreno[fil][col]);
                }
                else {
                    printf(" %c%c ", terreno[fil][col], terreno[fil][col]);
                }
            }
        }
        printf("\n");
    }
    printf("    ");
    for (int i = 0; i < columnas_terreno; i++){
        printf(" -- ");
    }
    printf("\n\n");
    printf("Resistencia torre 1: %i \nResistencia torre 2: %i\n", juego.torres.resistencia_torre_1, juego.torres.resistencia_torre_2);
    printf("Enemigos muertos: %i\n", enemigos_muertos(juego.nivel));
    int enemigos_vivos = 0;
    int i = 0;
    while (enemigos_vivos < 3 && i < juego.nivel.tope_enemigos){
        if (juego.nivel.enemigos[i].vida > VIDA_ORCO_MUERTO){
            printf("Vida enemigo %i: %i HP\n", i, juego.nivel.enemigos[i].vida);
            enemigos_vivos++;
        }
        i++;
    }
    printf("\n");
}

bool defensor_posicion_correcta(nivel_t nivel, int fila, int columna){
    for (int i = 0; i < nivel.tope_camino_1; i++ ){
        if ((fila == nivel.camino_1[i].fil) && (columna == nivel.camino_1[i].col)){
            return false;
        }
    }
    for (int i = 0; i < nivel.tope_camino_2; i++ ){
        if ((fila == nivel.camino_2[i].fil) && (columna == nivel.camino_2[i].col)){
            return false;
        }
    }
    for (int i = 0; i < nivel.tope_defensores; i++ ){
        if ((fila == nivel.defensores[i].posicion.fil ) && (columna == nivel.defensores[i].posicion.col)){
            return false;
        }
    }
    if (fila < 0 || columna < 0){
        return false;
    }
    else {
        return true;
    }
}

int agregar_defensor(nivel_t* nivel, coordenada_t posicion, char tipo){
    if (!defensor_posicion_correcta((*nivel), posicion.fil, posicion.col)){
        return FALLO_AGREGAR_DEFENSOR;
    }
    else {
        (*nivel).defensores[(*nivel).tope_defensores].posicion.fil = posicion.fil;
        (*nivel).defensores[(*nivel).tope_defensores].posicion.col = posicion.col;
        (*nivel).defensores[(*nivel).tope_defensores].tipo = tipo;
        if (tipo == ENANO){
            (*nivel).defensores[(*nivel).tope_defensores].fuerza_ataque = DANIO_BASE_ENANO;
        }
        else {
            (*nivel).defensores[(*nivel).tope_defensores].fuerza_ataque = DANIO_BASE_ELFO;
        }
        (*nivel).tope_defensores ++;   
        return EXITO_AGREGAR_DEFENSOR;
    }
}

int estado_nivel(nivel_t nivel){
    if (enemigos_muertos(nivel) == nivel.max_enemigos_nivel){
        return NIVEL_GANADO;
    }
    else {
        return NIVEL_JUGANDO;
    }
}

int danio_defensor(juego_t juego, int numero_defensor){
    if (juego.nivel.defensores[numero_defensor].tipo == ENANO){
        if (rand()%100 < juego.fallo_gimli){
            return FALLO_ATAQUE;
        }
        else if (rand()%100 < juego.critico_gimli){
            return juego.nivel.defensores[numero_defensor].fuerza_ataque + EXTRA_GOLPRE_CRITICO;
        } 
        else {
            return juego.nivel.defensores[numero_defensor].fuerza_ataque;
        }
    }
    else {
        if (rand()%100 < juego.fallo_legolas){
            return FALLO_ATAQUE;
        }
        else if (rand()%100 < juego.critico_legolas){
            return juego.nivel.defensores[numero_defensor].fuerza_ataque + EXTRA_GOLPRE_CRITICO;
        }
        else {
            return juego.nivel.defensores[numero_defensor].fuerza_ataque;
        }
    }
}

void jugar_turno_enanos(juego_t* juego){
    for (int defensor = 0; defensor < (*juego).nivel.tope_defensores; defensor++){
        if ((*juego).nivel.defensores[defensor].tipo == ENANO){
            for (int enemigo = 0; enemigo < (*juego).nivel.tope_enemigos; enemigo++){
                if ((*juego).nivel.enemigos[enemigo].camino == CAMINO_1 && (*juego).nivel.enemigos[enemigo].vida > VIDA_ORCO_MUERTO){
                    // Misma fila
                    if (((*juego).nivel.defensores[defensor].posicion.fil == (*juego).nivel.camino_1[(*juego).nivel.enemigos[enemigo].pos_en_camino].fil) && ((*juego).nivel.defensores[defensor].posicion.col == (*juego).nivel.camino_1[(*juego).nivel.enemigos[enemigo].pos_en_camino].col - 1 || (*juego).nivel.defensores[defensor].posicion.col == (*juego).nivel.camino_1[(*juego).nivel.enemigos[enemigo].pos_en_camino].col + 1)){
                        (*juego).nivel.enemigos[enemigo].vida -= danio_defensor((*juego), defensor);
                        break;
                    }
                    // Misma columna
                    if (((*juego).nivel.defensores[defensor].posicion.col == (*juego).nivel.camino_1[(*juego).nivel.enemigos[enemigo].pos_en_camino].col) && ((*juego).nivel.defensores[defensor].posicion.fil == (*juego).nivel.camino_1[(*juego).nivel.enemigos[enemigo].pos_en_camino].fil - 1 || (*juego).nivel.defensores[defensor].posicion.fil == (*juego).nivel.camino_1[(*juego).nivel.enemigos[enemigo].pos_en_camino].fil + 1)){
                        (*juego).nivel.enemigos[enemigo].vida -= danio_defensor((*juego), defensor);
                        break;
                    }
                    // En diagonal
                    if ((((*juego).nivel.defensores[defensor].posicion.col == (*juego).nivel.camino_1[(*juego).nivel.enemigos[enemigo].pos_en_camino].col + 1) || ((*juego).nivel.defensores[defensor].posicion.col == (*juego).nivel.camino_1[(*juego).nivel.enemigos[enemigo].pos_en_camino].col - 1)) && ((*juego).nivel.defensores[defensor].posicion.fil == (*juego).nivel.camino_1[(*juego).nivel.enemigos[enemigo].pos_en_camino].fil - 1 || (*juego).nivel.defensores[defensor].posicion.fil == (*juego).nivel.camino_1[(*juego).nivel.enemigos[enemigo].pos_en_camino].fil + 1)){
                        (*juego).nivel.enemigos[enemigo].vida -= danio_defensor((*juego), defensor);
                        break;
                    }
                }   
                if ((*juego).nivel.enemigos[enemigo].camino == CAMINO_2 && (*juego).nivel.enemigos[enemigo].vida > VIDA_ORCO_MUERTO){
                    // Misma fila
                    if (((*juego).nivel.defensores[defensor].posicion.fil == (*juego).nivel.camino_2[(*juego).nivel.enemigos[enemigo].pos_en_camino].fil) && ((*juego).nivel.defensores[defensor].posicion.col == (*juego).nivel.camino_2[(*juego).nivel.enemigos[enemigo].pos_en_camino].col - 1 || (*juego).nivel.defensores[defensor].posicion.col == (*juego).nivel.camino_2[(*juego).nivel.enemigos[enemigo].pos_en_camino].col + 1)){
                        (*juego).nivel.enemigos[enemigo].vida -= danio_defensor((*juego), defensor);
                        break;
                    }
                    // Misma columna
                    if (((*juego).nivel.defensores[defensor].posicion.col == (*juego).nivel.camino_2[(*juego).nivel.enemigos[enemigo].pos_en_camino].col) && ((*juego).nivel.defensores[defensor].posicion.fil == (*juego).nivel.camino_2[(*juego).nivel.enemigos[enemigo].pos_en_camino].fil - 1 || (*juego).nivel.defensores[defensor].posicion.col == (*juego).nivel.camino_2[(*juego).nivel.enemigos[enemigo].pos_en_camino].fil + 1)){
                        (*juego).nivel.enemigos[enemigo].vida -= danio_defensor((*juego), defensor);
                        break;
                    }
                    // En diagonal
                    if ((((*juego).nivel.defensores[defensor].posicion.col == (*juego).nivel.camino_2[(*juego).nivel.enemigos[enemigo].pos_en_camino].col + 1) || ((*juego).nivel.defensores[defensor].posicion.col == (*juego).nivel.camino_2[(*juego).nivel.enemigos[enemigo].pos_en_camino].col - 1)) && ((*juego).nivel.defensores[defensor].posicion.fil == (*juego).nivel.camino_2[(*juego).nivel.enemigos[enemigo].pos_en_camino].fil - 1 || (*juego).nivel.defensores[defensor].posicion.col == (*juego).nivel.camino_2[(*juego).nivel.enemigos[enemigo].pos_en_camino].fil + 1)){
                        (*juego).nivel.enemigos[enemigo].vida -= danio_defensor((*juego), defensor);
                        break;
                    }
                }
            }
        }
    }
}

void jugar_turno_elfos(juego_t* juego){
    for (int defensor = 0; defensor < (*juego).nivel.tope_defensores; defensor++){
        if ((*juego).nivel.defensores[defensor].tipo == ELFO){
            for (int enemigo = 0; enemigo < (*juego).nivel.tope_enemigos; enemigo++){
                if ((*juego).nivel.enemigos[enemigo].camino == CAMINO_1 && (*juego).nivel.enemigos[enemigo].vida > VIDA_ORCO_MUERTO){
                    for (int i = 1; i < 4; i++){
                        // Misma fila
                        if ((*juego).nivel.defensores[defensor].posicion.fil == (*juego).nivel.camino_1[(*juego).nivel.enemigos[enemigo].pos_en_camino].fil && ((*juego).nivel.defensores[defensor].posicion.col == (*juego).nivel.camino_1[(*juego).nivel.enemigos[enemigo].pos_en_camino].col - i || (*juego).nivel.defensores[defensor].posicion.col == (*juego).nivel.camino_1[(*juego).nivel.enemigos[enemigo].pos_en_camino].col + i)){
                            (*juego).nivel.enemigos[enemigo].vida -= danio_defensor((*juego), defensor);
                        }
                        // Misma columna
                        if (((*juego).nivel.defensores[defensor].posicion.col == (*juego).nivel.camino_1[(*juego).nivel.enemigos[enemigo].pos_en_camino].col) && ((*juego).nivel.defensores[defensor].posicion.fil == (*juego).nivel.camino_1[(*juego).nivel.enemigos[enemigo].pos_en_camino].fil - i || (*juego).nivel.defensores[defensor].posicion.fil == (*juego).nivel.camino_1[(*juego).nivel.enemigos[enemigo].pos_en_camino].fil + i)){
                            (*juego).nivel.enemigos[enemigo].vida -= danio_defensor((*juego), defensor);
                        }
                    }
                    // En diagonal
                    if ((((*juego).nivel.defensores[defensor].posicion.col == (*juego).nivel.camino_1[(*juego).nivel.enemigos[enemigo].pos_en_camino].col + 1) || ((*juego).nivel.defensores[defensor].posicion.col == (*juego).nivel.camino_1[(*juego).nivel.enemigos[enemigo].pos_en_camino].col - 1)) && ((*juego).nivel.defensores[defensor].posicion.fil == (*juego).nivel.camino_1[(*juego).nivel.enemigos[enemigo].pos_en_camino].fil - 1 || (*juego).nivel.defensores[defensor].posicion.fil == (*juego).nivel.camino_1[(*juego).nivel.enemigos[enemigo].pos_en_camino].fil + 1)){
                            (*juego).nivel.enemigos[enemigo].vida -= danio_defensor((*juego), defensor);
                        }
                    // Ubicaciones especiales
                    if (((*juego).nivel.defensores[defensor].posicion.fil == (*juego).nivel.camino_1[(*juego).nivel.enemigos[enemigo].pos_en_camino].fil + 1 || (*juego).nivel.defensores[defensor].posicion.fil == (*juego).nivel.camino_1[(*juego).nivel.enemigos[enemigo].pos_en_camino].fil -1) && ((*juego).nivel.defensores[defensor].posicion.col == (*juego).nivel.camino_1[(*juego).nivel.enemigos[enemigo].pos_en_camino].col - 2 || (*juego).nivel.defensores[defensor].posicion.col == (*juego).nivel.camino_1[(*juego).nivel.enemigos[enemigo].pos_en_camino].col + 2)){
                        (*juego).nivel.enemigos[enemigo].vida -= danio_defensor((*juego), defensor);
                    }
                    if (((*juego).nivel.defensores[defensor].posicion.fil == (*juego).nivel.camino_1[(*juego).nivel.enemigos[enemigo].pos_en_camino].fil + 2 || (*juego).nivel.defensores[defensor].posicion.fil == (*juego).nivel.camino_1[(*juego).nivel.enemigos[enemigo].pos_en_camino].fil - 2) && ((*juego).nivel.defensores[defensor].posicion.col == (*juego).nivel.camino_1[(*juego).nivel.enemigos[enemigo].pos_en_camino].col - 1 || (*juego).nivel.defensores[defensor].posicion.col == (*juego).nivel.camino_1[(*juego).nivel.enemigos[enemigo].pos_en_camino].col + 1)){
                        (*juego).nivel.enemigos[enemigo].vida -= danio_defensor((*juego), defensor);
                    }
                }
                if ((*juego).nivel.enemigos[enemigo].camino == CAMINO_2 && (*juego).nivel.enemigos[enemigo].vida > VIDA_ORCO_MUERTO){
                    for (int i = 1; i < 4; i++){
                        // Misma fila
                        if ((*juego).nivel.defensores[defensor].posicion.fil == (*juego).nivel.camino_2[(*juego).nivel.enemigos[enemigo].pos_en_camino].fil && ((*juego).nivel.defensores[defensor].posicion.col == (*juego).nivel.camino_2[(*juego).nivel.enemigos[enemigo].pos_en_camino].col - i || (*juego).nivel.defensores[defensor].posicion.col == (*juego).nivel.camino_2[(*juego).nivel.enemigos[enemigo].pos_en_camino].col + i)){
                            (*juego).nivel.enemigos[enemigo].vida -= danio_defensor((*juego), defensor);
                        }
                        // Misma columna
                        if (((*juego).nivel.defensores[defensor].posicion.col == (*juego).nivel.camino_2[(*juego).nivel.enemigos[enemigo].pos_en_camino].col) && ((*juego).nivel.defensores[defensor].posicion.fil == (*juego).nivel.camino_2[(*juego).nivel.enemigos[enemigo].pos_en_camino].fil - i || (*juego).nivel.defensores[defensor].posicion.fil == (*juego).nivel.camino_2[(*juego).nivel.enemigos[enemigo].pos_en_camino].fil + i)){
                            (*juego).nivel.enemigos[enemigo].vida -= danio_defensor((*juego), defensor);
                        }
                    }
                    // En diagonal
                    if ((((*juego).nivel.defensores[defensor].posicion.col == (*juego).nivel.camino_2[(*juego).nivel.enemigos[enemigo].pos_en_camino].col + 1) || ((*juego).nivel.defensores[defensor].posicion.col == (*juego).nivel.camino_2[(*juego).nivel.enemigos[enemigo].pos_en_camino].col - 1)) && ((*juego).nivel.defensores[defensor].posicion.fil == (*juego).nivel.camino_2[(*juego).nivel.enemigos[enemigo].pos_en_camino].fil - 1 || (*juego).nivel.defensores[defensor].posicion.fil == (*juego).nivel.camino_2[(*juego).nivel.enemigos[enemigo].pos_en_camino].fil + 1)){
                        (*juego).nivel.enemigos[enemigo].vida -= danio_defensor((*juego), defensor);
                    }
                    // Ubicaciones especiales Manhattan
                    if (((*juego).nivel.defensores[defensor].posicion.fil == (*juego).nivel.camino_2[(*juego).nivel.enemigos[enemigo].pos_en_camino].fil + 1 || (*juego).nivel.defensores[defensor].posicion.fil == (*juego).nivel.camino_2[(*juego).nivel.enemigos[enemigo].pos_en_camino].fil -1) && ((*juego).nivel.defensores[defensor].posicion.col == (*juego).nivel.camino_2[(*juego).nivel.enemigos[enemigo].pos_en_camino].col - 2 || (*juego).nivel.defensores[defensor].posicion.col == (*juego).nivel.camino_2[(*juego).nivel.enemigos[enemigo].pos_en_camino].col + 2)){
                        (*juego).nivel.enemigos[enemigo].vida -= danio_defensor((*juego), defensor);
                    }
                    if (((*juego).nivel.defensores[defensor].posicion.fil == (*juego).nivel.camino_2[(*juego).nivel.enemigos[enemigo].pos_en_camino].fil + 2 || (*juego).nivel.defensores[defensor].posicion.fil == (*juego).nivel.camino_2[(*juego).nivel.enemigos[enemigo].pos_en_camino].fil - 2) && ((*juego).nivel.defensores[defensor].posicion.col == (*juego).nivel.camino_2[(*juego).nivel.enemigos[enemigo].pos_en_camino].col - 1 || (*juego).nivel.defensores[defensor].posicion.col == (*juego).nivel.camino_2[(*juego).nivel.enemigos[enemigo].pos_en_camino].col + 1)){
                        (*juego).nivel.enemigos[enemigo].vida -= danio_defensor((*juego), defensor);
                    }
                }
            }
        }
    }
}

void orcos_atacan_torre(juego_t* juego){

    /* COMPLETAR */
    
}

void jugar_turno_orcos(juego_t* juego){
    (*juego).nivel.enemigos[(*juego).nivel.tope_enemigos].pos_en_camino = POSICION_INICIAL_ORCO;
    (*juego).nivel.enemigos[(*juego).nivel.tope_enemigos].vida = VIDA_BASICA_ORCO + rand()%VIDA_ORCO_EXTRA_MAX;
    if ((*juego).nivel_actual == NIVEL_1 || (*juego).nivel_actual == NIVEL_3 || (*juego).nivel_actual == NIVEL_4){
        (*juego).nivel.enemigos[(*juego).nivel.tope_enemigos].camino = CAMINO_1;
    }
    if ((*juego).nivel_actual == NIVEL_2){
        (*juego).nivel.enemigos[(*juego).nivel.tope_enemigos].camino = CAMINO_2;
    }
    if ((*juego).nivel_actual == NIVEL_3 || (*juego).nivel_actual == NIVEL_4){   
        (*juego).nivel.enemigos[(*juego).nivel.tope_enemigos + 1].pos_en_camino = POSICION_INICIAL_ORCO;
        (*juego).nivel.enemigos[(*juego).nivel.tope_enemigos + 1].camino = CAMINO_2;   
        (*juego).nivel.enemigos[(*juego).nivel.tope_enemigos + 1].vida = VIDA_BASICA_ORCO + rand()%VIDA_ORCO_EXTRA_MAX;
    }
    for (int i = 0; i < (*juego).nivel.tope_enemigos; i++){
        ((*juego).nivel.enemigos[i].pos_en_camino)++;
    }
    if ((*juego).nivel.tope_enemigos < (*juego).nivel.max_enemigos_nivel){
        if ((*juego).nivel_actual == NIVEL_1 || (*juego).nivel_actual == NIVEL_2){
            ((*juego).nivel.tope_enemigos)++;
        }
        if ((*juego).nivel_actual == NIVEL_3 || (*juego).nivel_actual == NIVEL_4){
            ((*juego).nivel.tope_enemigos) = ((*juego).nivel.tope_enemigos) + 2;
        }
    }
    for (int i = 0; i < (*juego).nivel.tope_enemigos; i++){
        if ((*juego).nivel.enemigos[i].pos_en_camino == (*juego).nivel.tope_camino_1 - 1 && (*juego).nivel.enemigos[i].camino == CAMINO_1  && (*juego).nivel.enemigos[i].vida > VIDA_ORCO_MUERTO){
            (*juego).torres.resistencia_torre_1 -= (*juego).nivel.enemigos[i].vida;
            (*juego).nivel.enemigos[i].vida = VIDA_ORCO_MUERTO;
        } 
        if ((*juego).nivel.enemigos[i].pos_en_camino == (*juego).nivel.tope_camino_2 - 1 && (*juego).nivel.enemigos[i].camino == CAMINO_2 && (*juego).nivel.enemigos[i].vida > VIDA_ORCO_MUERTO){
            (*juego).torres.resistencia_torre_2 -= (*juego).nivel.enemigos[i].vida;
            (*juego).nivel.enemigos[i].vida = VIDA_ORCO_MUERTO;
        } 
    }
}

void jugar_turno(juego_t* juego){
    srand((unsigned)time(NULL));
    // ATACAN ENANOS
    jugar_turno_enanos(juego);
    // ATACAN ELFOS
    jugar_turno_elfos(juego);
    // MUEVEN ENEMIGOS
    jugar_turno_orcos(juego);
}

int estado_juego(juego_t juego){
    if ((juego.nivel_actual == NIVEL_4) && (enemigos_muertos(juego.nivel) >= juego.nivel.max_enemigos_nivel) && (juego.torres.resistencia_torre_1 > RESISTENCIA_TORRE_DESTRUIDA) && (juego.torres.resistencia_torre_2 > RESISTENCIA_TORRE_DESTRUIDA)){
        return JUEGO_GANADO;
    }
    else if(juego.torres.resistencia_torre_1 <= RESISTENCIA_TORRE_DESTRUIDA || juego.torres.resistencia_torre_2 <= RESISTENCIA_TORRE_DESTRUIDA){
        return JUEGO_PERDIDO;
    }
    else {
        return JUEGO_JUGANDO;
    }
}