#include <stdio.h>
#include <stdlib.h>
#include <stdio_ext.h>
#include <stdbool.h>
#include <time.h>
#include "animos.h"
#include "defendiendo_torres.h"
#include "utiles.h"

#define BUENO 'B'
#define REGULAR 'R'
#define MALO 'M'
#define RESPUESTA_AFIRMATIVA 'S'
#define ENANO 'G'
#define ELFO 'L'
#define TOPE_TERRENO_1_2 14
#define TOPE_TERRENO_3_4 19
#define NIVEL_1 1
#define NIVEL_2 2
#define NIVEL_3 3
#define NIVEL_4 4
#define CAMINO_1 1
#define CAMINO_2 2 
#define NIVEL_JUGANDO 0
#define JUEGO_JUGANDO 0
#define JUEGO_PERDIDO -1
#define FALLO_AGREGAR_DEFENSOR -1
#define DANIO_DEFENSOR_EXTRA 50
#define VIDA_ORCO_MUERTO 0
#define ENEMIGOS_NIVEL_1 100
#define ENEMIGOS_NIVEL_2 200
#define ENEMIGOS_NIVEL_3 300
#define ENEMIGOS_NIVEL_4 500
#define DEFENSORES_NIVEL_1 5
#define DEFENSORES_NIVEL_2 5
#define DEFENSORES_NIVEL_3 6
#define DEFENSORES_NIVEL_4 8

void inicializar_nivel(juego_t* juego, int nivel);
void colocar_defensor(juego_t* juego, char tipo_defensor);
void pedir_defensor_extra(juego_t* juego, int* defensores_extra_colocados);
bool se_puede_agregar_defensor_extra(juego_t juego, int defensores_extra_colocados);
void imprimir_resultado(int estado_juego);

int main(){
    srand((unsigned)time(NULL));
    /* ................... CONDICIONES INICIALES ................... */
    int viento, humedad;
    char animo_legolas, animo_gimli;
    animos(&viento, &humedad , &animo_legolas , &animo_gimli);
    /* ................... JUGAR PARTIDA ................... */
    juego_t juego;
    inicializar_juego(&juego, viento, humedad, animo_legolas, animo_gimli);
    for (int nivel = NIVEL_4; (nivel <= NIVEL_4) && (estado_juego(juego) == JUEGO_JUGANDO); nivel++){
        inicializar_nivel(&juego, nivel);
        int defensores_extra_colocados = 0;
        while ((estado_nivel(juego.nivel) == NIVEL_JUGANDO) && (estado_juego(juego) == JUEGO_JUGANDO)){
            system("clear");
            jugar_turno(&juego);
            mostrar_juego(juego);
            if (se_puede_agregar_defensor_extra(juego, defensores_extra_colocados)){
                pedir_defensor_extra(&juego,&defensores_extra_colocados);
            }
            detener_el_tiempo(0.2);
        }     
    }
    /* ................... RESULTADO FINAL ................... */
    imprimir_resultado(estado_juego(juego));
    return 0;
}

/* Recibe el estado del juego, imprime el resultado del jugador.
 *
 */
void imprimir_resultado(int estado_juego){
    system("clear");
    if (estado_juego == JUEGO_PERDIDO){
        printf("PERDISTE :(\n\n");
    }
    else {
        printf("¡¡¡GANASTE!!!\n\n");
    }
}

/* Recibe el juego, y los defensores extras que fueron colocaos.
 * Realiza validaciones para determinar si es el momento para que el usuario coloque un defensor extra en el juego.
 */
bool se_puede_agregar_defensor_extra(juego_t juego, int defensores_extra_colocados){
    int enemigos_muertos = 0;
    for (int i = 0; i < juego.nivel.tope_enemigos; i++){
        if (juego.nivel.enemigos[i].vida <= VIDA_ORCO_MUERTO){
            enemigos_muertos++;
        }
    }
    if (juego.torres.elfos_extra <= 0 && juego.torres.enanos_extra <= 0){
        return false;
    } 
    if (juego.nivel_actual == NIVEL_1){
        if ((enemigos_muertos >= 25 && defensores_extra_colocados == 0) || (enemigos_muertos >= 50 && defensores_extra_colocados == 1) || (enemigos_muertos >= 75 && defensores_extra_colocados == 2)){
            return true;
        }
    }
    else if (juego.nivel_actual == NIVEL_2){
        if ((enemigos_muertos >= 50 && defensores_extra_colocados == 0) || (enemigos_muertos >= 100 && defensores_extra_colocados == 1) || (enemigos_muertos >= 150 && defensores_extra_colocados == 2)){
            return true;
        }
    }
    else if (juego.nivel_actual == NIVEL_3){
        if ((enemigos_muertos >= 50 && defensores_extra_colocados == 0) || (enemigos_muertos >= 100 && defensores_extra_colocados == 1) || (enemigos_muertos >= 150 && defensores_extra_colocados == 2) || (enemigos_muertos >= 200 && defensores_extra_colocados == 3) || (enemigos_muertos >= 250 && defensores_extra_colocados == 4)){
            return true;
        }
    }
    else {
        if ((enemigos_muertos >= 50 && defensores_extra_colocados == 0) || (enemigos_muertos >= 100 && defensores_extra_colocados == 1) || (enemigos_muertos >= 150 && defensores_extra_colocados == 2) || (enemigos_muertos >= 200 && defensores_extra_colocados == 3) || (enemigos_muertos >= 250 && defensores_extra_colocados == 4) || (enemigos_muertos >= 300 && defensores_extra_colocados == 5) || (enemigos_muertos >= 350 && defensores_extra_colocados == 6) || (enemigos_muertos >= 400 && defensores_extra_colocados == 7) || (enemigos_muertos >= 450 && enemigos_muertos < 499 && defensores_extra_colocados == 8)){
            return true;
        }
    }
    return false;
}

/* Recibe el juego, y el tipo de defensor extra que se quiere colocar.
 * Devuelve si es posible colocar el defensor en funcion del tamanio del terreno, es decir, si no se lo esta colocando afuera.
 */
bool defensor_dentro_terreno(juego_t juego, int fil, int col){
    if ((juego.nivel_actual == NIVEL_1 || juego.nivel_actual == NIVEL_2) && (fil > TOPE_TERRENO_1_2 || col > TOPE_TERRENO_1_2)){
        return false;
    }
    if ((juego.nivel_actual == NIVEL_3 || juego.nivel_actual == NIVEL_4) && (fil > TOPE_TERRENO_3_4 || col > TOPE_TERRENO_3_4)){
        return false;
    }
    return true;
}

/* Recibe el juego, y el tipo de defensor extra que se quiere colocar.
 * Devuelve si es posible colocar el defensor extra en funcion de la resistencia de la torre.
 */
bool torres_resistencia_suficiente(juego_t juego, char tipo_defensor){
    if (tipo_defensor == ENANO && juego.torres.resistencia_torre_1 - DANIO_DEFENSOR_EXTRA > 0){
        return true;
    }
    if (tipo_defensor == ELFO && juego.torres.resistencia_torre_2 - DANIO_DEFENSOR_EXTRA > 0){
        return true;
    }
    return false;
}

/* Recibe el juego y el numero de llamados a esta funcion.
 * Interactua con el usuario, verificando si quiere colocar un defensor, y de que tipo desea. Si todo es correcto, llama a la funcion que coloca el defensor.
 */
void pedir_defensor_extra(juego_t* juego, int* defensores_extra_colocados){
    char respuesta, tipo_defensor;
    printf("Deseas agregar defensor? (S/n): ");
    __fpurge(stdin);
    scanf("%c", &respuesta);
    if (respuesta == RESPUESTA_AFIRMATIVA){
        printf("Que tipo de defensor desea agregar? (G/L): ");
        __fpurge(stdin);
        scanf("%c", &tipo_defensor);
        if (torres_resistencia_suficiente((*juego), tipo_defensor)){
            colocar_defensor(juego, tipo_defensor);
            if (tipo_defensor == ENANO){
                (*juego).torres.resistencia_torre_1 -= DANIO_DEFENSOR_EXTRA;
            }
            else {
                (*juego).torres.resistencia_torre_2 -= DANIO_DEFENSOR_EXTRA;
            }
        }
        else {
            printf("No se pudo agregar el defensor.\n");
        }
    }
    (*defensores_extra_colocados)++;
}

/* Recibe el juego, y el tipo de defensor.
 * Interactua con el usuario para recibir la posicion deseada para el defensor, lo valida y lo coloca en el juego.
 */
void colocar_defensor(juego_t* juego, char tipo_defensor){
    coordenada_t coordenada;
    int fil, col;
    if (tipo_defensor == ENANO){       
        printf("Agregar defensor enano (formato fila-columna): ");
    }
    else if (tipo_defensor == ELFO){
        printf("Agregar defensor elfo (formato fila-columna): ");
    }
    __fpurge(stdin);
    scanf("%i-%i", &fil, &col);
    coordenada.fil = fil;        
    coordenada.col = col;
    while (agregar_defensor(&(*juego).nivel, coordenada, tipo_defensor) == FALLO_AGREGAR_DEFENSOR || !defensor_dentro_terreno((*juego), fil, col)){
        printf("Error al agregar defensor, intentarlo nuevamente: ");
        __fpurge(stdin);
        scanf("%i-%i", &fil, &col);
        coordenada.fil = fil;
        coordenada.col = col;
    }
}

/* Recibe el juego y coordenadas. Las modifica asignandole valores correspondietes a la entrada y a la torre de cada nivel
 *
 */
void inicializar_entradas_torres(juego_t* juego, coordenada_t* entrada, coordenada_t* torre){
    if ((*juego).nivel_actual == NIVEL_1){
        (*entrada).fil = 9;
        (*entrada).col = 14;
        (*torre).fil = 3;
        (*torre).col = 0;
    }
    else if ((*juego).nivel_actual == NIVEL_2){
        (*entrada).fil = 3;
        (*entrada).col = 0;
        (*torre).fil = 9;
        (*torre).col = 14;
    }
    else if ((*juego).nivel_actual == NIVEL_3){
        if ((*juego).nivel.tope_camino_1 == 0){
            (*entrada).fil = 0;
            (*entrada).col = 4;
            (*torre).fil = 19;
            (*torre).col = 4;
        }
        else {
            (*entrada).fil = 0;
            (*entrada).col = 16;
            (*torre).fil = 19;
            (*torre).col = 16;
        }
    }
    else {
        if ((*juego).nivel.tope_camino_1 == 0){
            (*entrada).fil = 19;
            (*entrada).col = 4;
            (*torre).fil = 0;
            (*torre).col = 4;
        }
        else {
            (*entrada).fil = 19;
            (*entrada).col = 16;
            (*torre).fil = 0;
            (*torre).col = 16;
        }
    }
}

/* Recibe un numero, verifica si es par o impar.
 *
 */
bool es_par(int numero){
    if (numero % 2 == 0){
        return true;
    }
    return false;
}

/* Recibe el juego, y el nivel actual.
 * Inicializa en nivel, con los caminos, y posiciona los defensores.
 */
void inicializar_nivel(juego_t* juego, int nivel){
    system("clear");
    int numero_defensores;
    char tipo_defensor;
    coordenada_t entrada, torre;
    (*juego).nivel_actual = nivel;
    (*juego).nivel.tope_defensores = 0;
    (*juego).nivel.tope_enemigos = 0;
    (*juego).nivel.tope_camino_1 = 0;
    (*juego).nivel.tope_camino_2 = 0;
    if (nivel == NIVEL_1){
        (*juego).nivel.max_enemigos_nivel = ENEMIGOS_NIVEL_1;
        printf("NIVEL 1\n\n");
        inicializar_entradas_torres(juego, &entrada, &torre);
        obtener_camino((*juego).nivel.camino_1, &(*juego).nivel.tope_camino_1, entrada, torre);
        numero_defensores = DEFENSORES_NIVEL_1;
        tipo_defensor = ENANO;
    }
    else if (nivel == NIVEL_2){
        (*juego).nivel.max_enemigos_nivel = ENEMIGOS_NIVEL_2;
        printf("NIVEL 2\n\n");
        inicializar_entradas_torres(juego, &entrada, &torre);
        obtener_camino((*juego).nivel.camino_2, &(*juego).nivel.tope_camino_2, entrada, torre);
        numero_defensores = DEFENSORES_NIVEL_2;
        tipo_defensor = ELFO;
    }
    else if (nivel == NIVEL_3){
        (*juego).nivel.max_enemigos_nivel = ENEMIGOS_NIVEL_3;
        printf("NIVEL 3\n\n");
        inicializar_entradas_torres(juego, &entrada, &torre);
        obtener_camino((*juego).nivel.camino_1, &(*juego).nivel.tope_camino_1, entrada, torre);
        inicializar_entradas_torres(juego, &entrada, &torre);
        obtener_camino((*juego).nivel.camino_2, &(*juego).nivel.tope_camino_2, entrada, torre);
        numero_defensores = DEFENSORES_NIVEL_3;
    }
    else {
        (*juego).nivel.max_enemigos_nivel = ENEMIGOS_NIVEL_4;
        printf("NIVEL 4\n\n");
        inicializar_entradas_torres(juego, &entrada, &torre);
        obtener_camino((*juego).nivel.camino_1, &(*juego).nivel.tope_camino_1, entrada, torre);
        inicializar_entradas_torres(juego, &entrada, &torre);
        obtener_camino((*juego).nivel.camino_2, &(*juego).nivel.tope_camino_2, entrada, torre);
        numero_defensores = DEFENSORES_NIVEL_4;
    }
    mostrar_juego(*juego);
    for (int defensor = 0; defensor < numero_defensores; defensor++){
        if (nivel == NIVEL_3 || nivel == NIVEL_4){
            if (es_par(defensor)){
                tipo_defensor = ENANO;
            }
            else {
                tipo_defensor = ELFO;
            }
        }
        colocar_defensor(juego, tipo_defensor);
    }
    system("clear");
}   