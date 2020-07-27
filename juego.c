#include <stdio.h>
#include <stdlib.h>
#include <stdio_ext.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include "animos.h"
#include "defendiendo_torres.h"
#include "utiles.h"
#include "comandos.h"

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


void inicializar_nivel(juego_t* juego, int nivel, configuracion_t configuracion, FILE* archivo_caminos);
void colocar_defensor(juego_t* juego, char tipo_defensor);
void pedir_defensor_extra(juego_t* juego, int* defensores_extra_colocados, configuracion_t configuracion);
bool se_puede_agregar_defensor_extra(juego_t juego, int defensores_extra_colocados);
void imprimir_resultado(int estado_juego);
void jugar_partida(int argc, char* argv[]);

int main(int argc , char *argv[]){
    system("clear");
    if (strcmp(argv[1], RANKING) == 0){
        ejecutar_comando_ranking(argc,argv);
    }
    if (strcmp(argv[1], CREAR_CAMINO) == 0){
        ejecutar_comando_crear_camino(argc,argv);
    }
    if (strcmp(argv[1], CREAR_CONFIGURACION) == 0){
        ejecutar_comando_crear_configuracion(argc,argv);
    }
    if (strcmp(argv[1], PONEME_LA_REPE) == 0){
        ejecutar_comando_poneme_la_repe(argc,argv);
    }
    if (strcmp(argv[1], JUGAR) == 0){    
        jugar_partida(argc,argv);
    }
    return 0;
}

void jugar_partida(int argc, char* argv[]){
    char nombre_archivo_configuracion[MAX_NOMBRE];
    char nombre_archivo_grabacion[MAX_NOMBRE];
    configuracion_t configuracion;
    bool se_cargo_configuracion = false;
    bool se_graba_partida = false;
    FILE* archivo_grabacion;
    for (int j = 2; j < argc; j++){
        if (strncmp(argv[j], GRABACION, strlen(GRABACION)) == 0){
            char* token = strtok(argv[j], "=");
            strcpy(nombre_archivo_grabacion,strtok(NULL, "="));
            archivo_grabacion = fopen(nombre_archivo_grabacion, "w");
            if (!archivo_grabacion){
                printf("No se podra grabar la partida\n");
            }
            else {
                se_graba_partida = true;
            }
        }
        if (strncmp(argv[j], CONFIG, strlen(CONFIG)) == 0){
            char* token = strtok(argv[j], "=");
            strcpy(nombre_archivo_configuracion, strtok(NULL, "="));
            se_cargo_configuracion = obtener_configuracion(nombre_archivo_configuracion, &configuracion);
        }
    }
    /* ................... CONDICIONES INICIALES ................... */
    FILE* archivo_caminos;
    bool se_abrio_archivo_caminos;
    if (se_cargo_configuracion){
        archivo_caminos = fopen(configuracion.archivo_caminos, "r");
        if (!archivo_caminos){
            printf("El archivo de caminos no existe\n");
            return;
        }
        else {
            se_abrio_archivo_caminos = true;
        }
    }
    if (!se_cargo_configuracion){
        cargar_configuracion_por_defecto(&configuracion);
        strcpy(nombre_archivo_configuracion, "-1");
    }
    srand((unsigned)time(NULL));
    int viento, humedad;
    char animo_legolas, animo_gimli;
    animos(&viento, &humedad , &animo_legolas , &animo_gimli, configuracion);
    int numero_enemigos_muertos = 0;
    int numero_de_defensores = 0;
    char nombre_jugador[MAX_NOMBRE];
    printf("Ingresa tu usuario por favor: ");
    scanf("%s", nombre_jugador);
    /* ................... JUGAR PARTIDA ................... */
    juego_t juego;
    inicializar_juego(&juego, viento, humedad, animo_legolas, animo_gimli, configuracion);
    for (int nivel = NIVEL_1; (nivel <= NIVEL_4) && (estado_juego(juego) == JUEGO_JUGANDO); nivel++){
        inicializar_nivel(&juego, nivel, configuracion, archivo_caminos);
        numero_de_defensores = numero_de_defensores + (juego.nivel.tope_defensores);
        int defensores_extra_colocados = 0;
        while ((estado_nivel(juego.nivel) == NIVEL_JUGANDO) && (estado_juego(juego) == JUEGO_JUGANDO)){
            system("clear");
            jugar_turno(&juego);
            mostrar_juego(juego);
            if (se_puede_agregar_defensor_extra(juego, defensores_extra_colocados)){
                pedir_defensor_extra(&juego, &defensores_extra_colocados,configuracion);
            }
            detener_el_tiempo(configuracion.velocidad);
            if (se_graba_partida){
                grabar_partida(juego, archivo_grabacion);
            }
        }     
        numero_enemigos_muertos = numero_enemigos_muertos + enemigos_muertos(juego.nivel);
    }
    /* ................... RESULTADO FINAL ................... */
    if (se_abrio_archivo_caminos){
        fclose(archivo_caminos);
    }
    if(se_graba_partida){
        fclose(archivo_grabacion);
    }
    int puntaje;
    obtener_puntaje(configuracion, numero_enemigos_muertos, &puntaje, nombre_jugador, nombre_archivo_configuracion);
    imprimir_resultado(estado_juego(juego));
}

/* Recibe el estado del juego, imprime el resultado del jugador.
 *
 */
void imprimir_resultado(int estado_juego){
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
bool torres_resistencia_suficiente(juego_t juego, char tipo_defensor, configuracion_t configuracion){
    if (tipo_defensor == ENANO && juego.torres.resistencia_torre_1 - configuracion.enanos_extra[1] > 0 && juego.torres.resistencia_torre_2 - configuracion.enanos_extra[2] > 0){
        return true;
    }
    if (tipo_defensor == ELFO && juego.torres.resistencia_torre_1 - configuracion.elfos_extra[1] > 0 && juego.torres.resistencia_torre_2 - configuracion.elfos_extra[2] > 0){
        return true;
    }
    return false;
}

/* Recibe el juego y el numero de llamados a esta funcion.
 * Interactua con el usuario, verificando si quiere colocar un defensor, y de que tipo desea. Si todo es correcto, llama a la funcion que coloca el defensor.
 */
void pedir_defensor_extra(juego_t* juego, int* defensores_extra_colocados, configuracion_t configuracion){
    char respuesta, tipo_defensor;
    printf("Deseas agregar defensor? (S/n): ");
    __fpurge(stdin);
    scanf("%c", &respuesta);
    if (respuesta == RESPUESTA_AFIRMATIVA){
        printf("Que tipo de defensor desea agregar? (G/L): ");
        __fpurge(stdin);
        scanf("%c", &tipo_defensor);
        if (torres_resistencia_suficiente((*juego), tipo_defensor, configuracion)){
            colocar_defensor(juego, tipo_defensor);
            if (tipo_defensor == ENANO){
                (*juego).torres.resistencia_torre_1 -= configuracion.enanos_extra[1];
                (*juego).torres.resistencia_torre_2 -= configuracion.enanos_extra[2];
            }
            else {
                (*juego).torres.resistencia_torre_1 -= configuracion.elfos_extra[1];
                (*juego).torres.resistencia_torre_2 -= configuracion.elfos_extra[2];
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
void inicializar_nivel(juego_t* juego, int nivel, configuracion_t configuracion, FILE* archivo_caminos){
    system("clear");
    bool camino_por_defecto = (strcmp(configuracion.archivo_caminos, CAMINO_POR_DEFECTO) == 0);
    int numero_defensores_enanos;
    int numero_defensores_elfos;
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
        if (camino_por_defecto){
            inicializar_entradas_torres(juego, &entrada, &torre);
            obtener_camino((*juego).nivel.camino_1, &(*juego).nivel.tope_camino_1, entrada, torre);
        }
        numero_defensores_enanos = configuracion.enanos_iniciales[0];
        numero_defensores_elfos = configuracion.elfos_iniciales[0];
    }
    else if (nivel == NIVEL_2){
        (*juego).nivel.max_enemigos_nivel = ENEMIGOS_NIVEL_2;
        printf("NIVEL 2\n\n");
        if (camino_por_defecto){
            inicializar_entradas_torres(juego, &entrada, &torre);
            obtener_camino((*juego).nivel.camino_2, &(*juego).nivel.tope_camino_2, entrada, torre);
        }
        numero_defensores_enanos = configuracion.enanos_iniciales[1];
        numero_defensores_elfos = configuracion.elfos_iniciales[1];
    }
    else if (nivel == NIVEL_3){
        (*juego).nivel.max_enemigos_nivel = ENEMIGOS_NIVEL_3;
        printf("NIVEL 3\n\n");
        if (camino_por_defecto){
            inicializar_entradas_torres(juego, &entrada, &torre);
            obtener_camino((*juego).nivel.camino_1, &(*juego).nivel.tope_camino_1, entrada, torre);
            inicializar_entradas_torres(juego, &entrada, &torre);
            obtener_camino((*juego).nivel.camino_2, &(*juego).nivel.tope_camino_2, entrada, torre);
        }
        numero_defensores_enanos = configuracion.enanos_iniciales[2];
        numero_defensores_elfos = configuracion.elfos_iniciales[2];
    }
    else {
        (*juego).nivel.max_enemigos_nivel = ENEMIGOS_NIVEL_4;
        printf("NIVEL 4\n\n");
        if (camino_por_defecto){
            inicializar_entradas_torres(juego, &entrada, &torre);
            obtener_camino((*juego).nivel.camino_1, &(*juego).nivel.tope_camino_1, entrada, torre);
            inicializar_entradas_torres(juego, &entrada, &torre);
            obtener_camino((*juego).nivel.camino_2, &(*juego).nivel.tope_camino_2, entrada, torre);
        }
        numero_defensores_enanos = configuracion.enanos_iniciales[3];
        numero_defensores_elfos = configuracion.elfos_iniciales[3];
    }
    if (!camino_por_defecto){
        printf("Obteniendo camino del nivel...\n\n");
        detener_el_tiempo(2);
        obtener_camino_creado(archivo_caminos, (*juego).nivel.camino_1, (*juego).nivel.camino_2, &(*juego).nivel.tope_camino_1, &(*juego).nivel.tope_camino_2);
    }
    mostrar_juego(*juego);
    for (int defensor = 0; defensor < (numero_defensores_enanos+numero_defensores_elfos); defensor++){
        if (defensor < numero_defensores_enanos){
            tipo_defensor = ENANO;
        }
        else {
            tipo_defensor = ELFO;
        }
        colocar_defensor(juego, tipo_defensor);
    }
    system("clear");
}   