#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "comandos.h"
#include "defendiendo_torres.h"

void crear_configuracion(char nombre_archivo_configuracion[MAX_NOMBRE]){
    FILE* archivo_configuracion = fopen(nombre_archivo_configuracion, "w");
	if (!archivo_configuracion){
		printf("No se puede crear el archivo.\n");
		return;
	}
    configuracion_t configuracion;
    // Resistencia torres.
    printf("Ingrese resistencia torre 1: ");
    scanf("%i", &configuracion.resistencia_torres[0]);
    printf("Ingrese resistencia torre 2: ");
    scanf("%i", &configuracion.resistencia_torres[1]);
    fprintf(archivo_configuracion, "%s=%i,%i\n", "RESISTENCIA_TORRES", configuracion.resistencia_torres[0], configuracion.resistencia_torres[1]);
    // Defensores iniciales.
    for (int i = 0; i < 4; i++){
        printf("Ingrese el numero defensores enanos del nivel %i: ", i+1);
        scanf("%i", &configuracion.enanos_iniciales[i]);
        printf("Ingrese el numero de defensores elfos del nivel %i: ", i+1);
        scanf("%i", &configuracion.elfos_iniciales[i]);
    }
    fprintf(archivo_configuracion, "%s=%i,%i,%i,%i\n", "ENANOS_INICIO", configuracion.enanos_iniciales[0],configuracion.enanos_iniciales[1],configuracion.enanos_iniciales[2],configuracion.enanos_iniciales[3]);
    fprintf(archivo_configuracion, "%s=%i,%i,%i,%i\n", "ELFOS_INICIO", configuracion.elfos_iniciales[0],configuracion.elfos_iniciales[1],configuracion.elfos_iniciales[2],configuracion.elfos_iniciales[3]);
    // Enanos extra.
    printf("Ingrese el numero de enanos extra: ");
    scanf("%i", &configuracion.enanos_extra[0]);
    printf("Ingrese el costo de la torre 1 para agregar enanos: ");
    scanf("%i", &configuracion.enanos_extra[1]);
    printf("Ingrese el costo de la torre 2 para agregar enanos: ");
    scanf("%i", &configuracion.enanos_extra[2]);
    fprintf(archivo_configuracion, "%s=%i,%i,%i\n", "ENANOS_EXTRA", configuracion.enanos_extra[0], configuracion.enanos_extra[1], configuracion.enanos_extra[2]);
    // Elfos extra.
    printf("Ingrese el numero de elfos extra: ");
    scanf("%i", &configuracion.elfos_extra[0]);
    printf("Ingrese el costo de la torre 1 para agregar elfos: ");
    scanf("%i", &configuracion.elfos_extra[1]);
    printf("Ingrese el costo de la torre 2 para agregar elfos: ");
    scanf("%i", &configuracion.elfos_extra[2]);
    fprintf(archivo_configuracion, "%s=%i,%i,%i\n", "ELFOS_EXTRA", configuracion.elfos_extra[0], configuracion.elfos_extra[1], configuracion.elfos_extra[2]);
    // Animo de los enanos.
    printf("Ingrese la probabilidad de fallo de los enanos: ");
    scanf("%i", &configuracion.animo_enanos[0]);
    printf("Ingrese la probabilidad de critico de los enanos: ");
    scanf("%i", &configuracion.animo_enanos[1]);
    fprintf(archivo_configuracion, "%s=%i,%i\n", "ENANOS_ANIMO", configuracion.animo_enanos[0], configuracion.animo_enanos[1]);
    // Animo de los elfos.
    printf("Ingrese la probabilidad de fallo de los elfos: ");
    scanf("%i", &configuracion.animo_elfos[0]);
    printf("Ingrese la probabilidad de critico de los elfos: ");
    scanf("%i", &configuracion.animo_elfos[1]);
    fprintf(archivo_configuracion, "%s=%i,%i\n", "ELFOS_ANIMO", configuracion.animo_elfos[0], configuracion.animo_elfos[1]);
    // Velocidad juego.
    printf("Ingrese la velocidad de transicion del juego: ");
    scanf("%f", &configuracion.velocidad);
    fprintf(archivo_configuracion, "%s=%f\n", "VELOCIDAD", configuracion.velocidad);
    // Archivo camino.
    printf("Ingrese el archivo de caminos que desea utilziar: ");
    scanf("%s", configuracion.archivo_caminos);
    fprintf(archivo_configuracion, "%s=%s\n", "CAMINOS", configuracion.archivo_caminos);
    fclose(archivo_configuracion);
}

void mostrar_camino_creado(juego_t* juego_temporal, char terreno[MAX_TERRENO][MAX_TERRENO], int nivel, int filas_terreno, int columnas_terreno){
    cargar_camino(*juego_temporal, terreno, 1);
    cargar_camino(*juego_temporal, terreno, 2);
    imprimir_numeros(columnas_terreno);
    imprimir_lineas(columnas_terreno);
    imprimir_terreno(terreno,filas_terreno,columnas_terreno);
    imprimir_lineas(columnas_terreno);
}

bool movimiento_valido(char movimiento, juego_t juego_temporal, coordenada_t coordenada_actual){
    if (juego_temporal.nivel_actual == 1 || juego_temporal.nivel_actual == 2){
        if (coordenada_actual.fil == 0 && movimiento == 'w'){
            return false;
        }
        if (coordenada_actual.col == 0 && movimiento == 'a'){
            return false;
        }
        if (coordenada_actual.col == 14 && movimiento == 'd'){
            return false;
        }
        if (coordenada_actual.fil == 14 && movimiento == 's'){
            return false;
        }
    }
    if (juego_temporal.nivel_actual == 3 || juego_temporal.nivel_actual == 4){
        if (coordenada_actual.fil == 0 && movimiento == 'w'){
            return false;
        }
        if (coordenada_actual.col == 0 && movimiento == 'a'){
            return false;
        }
        if (coordenada_actual.col == 19 && movimiento == 'd'){
            return false;
        }
        if (coordenada_actual.fil == 19 && movimiento == 's'){
            return false;
        }
    }
    return true;
}

void crear_caminos(char nombre_archivo_camino[MAX_NOMBRE]){
    FILE* caminos = fopen(nombre_archivo_camino, "w");
	if (!caminos){
		printf("No se puede crear el archivo.\n");
		return;
	}
    juego_t juego_temporal;
    int columnas_terreno;
    int filas_terreno;
    char terreno[MAX_TERRENO][MAX_TERRENO];
    for (int nivel = 1; nivel < 5; nivel++){
        fprintf(caminos, "%s=%i\n", "NIVEL", nivel);
        printf("Cree su camino para el nivel %i\n", nivel);
        (juego_temporal).nivel_actual = nivel;
        (juego_temporal).nivel.tope_camino_1 = 0;
        (juego_temporal).nivel.tope_camino_2 = 0;
        cargar_tamanio_terreno(juego_temporal, &filas_terreno, &columnas_terreno);
        inicializar_terreno(terreno, filas_terreno, columnas_terreno);
        coordenada_t coordenada; // Variable donde se guarda la nueva coordenada.
        mostrar_camino_creado(&juego_temporal, terreno, nivel, filas_terreno, columnas_terreno);
        int numero_caminos = 1;
        if (nivel == 3 || nivel == 4){
            numero_caminos = 2;
        }
        for (int camino = 1; camino < numero_caminos + 1; camino++){
            fprintf(caminos, "%s=%i\n", "CAMINO", camino);
            printf("Ingrese coordenada inicial para el camino (x-y): ");
            scanf("%i-%i", &coordenada.fil, &coordenada.col);
            if (camino == 1){
                juego_temporal.nivel.camino_1[juego_temporal.nivel.tope_camino_1] = coordenada;
                juego_temporal.nivel.tope_camino_1++;
            }
            if (camino == 2){
                juego_temporal.nivel.camino_2[juego_temporal.nivel.tope_camino_2] = coordenada;
                juego_temporal.nivel.tope_camino_2++;
            }
            mostrar_camino_creado(&juego_temporal, terreno, nivel, filas_terreno, columnas_terreno);
            char movimiento; // El usuario decidira la direccion del camino con W A S D.
            printf("Con W A S D muevase para ir definiendo el camino, X para terminar: ");
            scanf(" %c", &movimiento);
            while(movimiento != 'x'){ // Con X dejara de cargar nuevas posiciones para el camino.
                if (movimiento_valido(movimiento, juego_temporal, coordenada)){
                    if(movimiento == ARRIBA){
                        coordenada.fil--;
                    }
                    if(movimiento == ABAJO){
                        coordenada.fil++;
                    }
                    if(movimiento == IZQUIERDA){
                        coordenada.col--;
                    }
                    if(movimiento == DERECHA){
                        coordenada.col++;
                    }
                    if (camino == 1){
                        juego_temporal.nivel.camino_1[juego_temporal.nivel.tope_camino_1] = coordenada;
                        juego_temporal.nivel.tope_camino_1++;
                    }
                    if (camino == 2){
                        juego_temporal.nivel.camino_2[juego_temporal.nivel.tope_camino_2] = coordenada;
                        juego_temporal.nivel.tope_camino_2++;
                    }
                    mostrar_camino_creado(&juego_temporal, terreno, nivel, filas_terreno, columnas_terreno);
                    fprintf(caminos, "%i;%i\n", coordenada.fil, coordenada.col);
                }
                printf("Con W A S D muevase para ir definiendo el camino, X para terminar: ");
                scanf(" %c", &movimiento);
            }
        }
    }
    fclose(caminos);
}

void mostrar_ranking(int numero_jugadores_a_mostrar, char nombre_archivo_ranking[MAX_NOMBRE]){
    printf("Archivo de ranking: %s\n", nombre_archivo_ranking);
    FILE* ranking = fopen(nombre_archivo_ranking, "r");
	if (!ranking){
		printf("El archivo de ranking no existe\n");
		return;
	}
    char jugador[MAX_NOMBRE];
    int puntaje;
    int leidos = fscanf(ranking, FORMATO, jugador, &puntaje);
    int i;
    while(leidos == 2 && i < numero_jugadores_a_mostrar){
        printf("%s: %i\n", jugador, puntaje);
        leidos = fscanf(ranking, FORMATO, jugador, &puntaje);
        i++;
    }
    fclose(ranking);
}

void ejecutar_comando_poneme_la_repe(int argc, char* argv[]){
    float velocidad_repeticion = 5.0;
    char nombre_archivo_repeticion[MAX_NOMBRE];
    for (int j = 2; j < argc; j++){
        if(strncmp(argv[j], "velocidad=", strlen("velocidad=")) == 0){
            velocidad_repeticion = (float) atof(&(argv[j][strlen("velocidad=")])); 
            printf("Velocidad de reproduccion: %f\n", velocidad_repeticion);
        }
        if (strncmp(argv[j], "grabacion=", strlen("grabacion=")) == 0){
            char* token = strtok(argv[j], "=");
            strcpy(nombre_archivo_repeticion,strtok(NULL, "="));
            printf("Nombre archivo de repeticion: %s\n", nombre_archivo_repeticion);
        }
    }
}

void ejecutar_comando_ranking(int argc, char* argv[]){
    int numero_jugadores_a_mostrar = MOSTRAR_TODOS;
    char nombre_archivo_configuracion[MAX_NOMBRE];
    char nombre_archivo_ranking[MAX_NOMBRE];
    strcpy(nombre_archivo_ranking, "ranking.csv");
    for (int j = 2; j < argc; j++){
        if(strncmp(argv[j], LISTAR, strlen(LISTAR)) == 0){
            numero_jugadores_a_mostrar = atoi(&(argv[j][strlen(LISTAR)])); 
            printf("Jugadores a mostrar: %i\n", numero_jugadores_a_mostrar);
        }
        if (strncmp(argv[j], CONFIG, strlen(CONFIG)) == 0){
            char* token = strtok(argv[j], "=");
            strcpy(nombre_archivo_configuracion,strtok(NULL, "."));
            strcpy(nombre_archivo_ranking, "ranking_");
            strcat(nombre_archivo_ranking, nombre_archivo_configuracion);
            strcat(nombre_archivo_ranking, ".csv");
        }
    }
    mostrar_ranking(numero_jugadores_a_mostrar, nombre_archivo_ranking);
}

void ejecutar_comando_crear_camino(int argc, char* argv[]){
    if (argc < 3){
        printf("Debes especificar el nombre del archivo para el camino\n");
        return;
    }
    char nombre_archivo_camino[MAX_NOMBRE];
    strcpy(nombre_archivo_camino, argv[2]);
    crear_caminos(nombre_archivo_camino);
}

void ejecutar_comando_crear_configuracion(int argc, char* argv[]){
    if (argc < 3){
        printf("Debes especificar el nombre del archivo para la configuracion\n");
        return;
    }
    char nombre_archivo_configuracion[MAX_NOMBRE];
    strcpy(nombre_archivo_configuracion, argv[2]);
    crear_configuracion(nombre_archivo_configuracion);
}