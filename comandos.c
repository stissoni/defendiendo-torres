#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "comandos.h"
#include "defendiendo_torres.h"
#include "utiles.h"

/*
 *
 */
void ejecutar_comando_poneme_la_repe(int argc, char* argv[]){
    float velocidad = 0.3;
    bool se_encontro_grabacion = false;
    char nombre_archivo_grabacion[MAX_NOMBRE];
    FILE* archivo_grabacion;
    juego_t juego;
    for (int j = 2; j < argc; j++){
        if(strncmp(argv[j], "grabacion=", strlen("grabacion=")) == 0){
            printf("ABRIENDO PARTIDA GUARDADA...\n");
            detener_el_tiempo(3);
            char* token = strtok(argv[j], "=");
            strcpy(nombre_archivo_grabacion,strtok(NULL, "="));
            archivo_grabacion = fopen(nombre_archivo_grabacion, LECTURA);
            if (!archivo_grabacion){
                printf("\nNo se pudo abrir la partida guardada\n");
                return;
            }
            se_encontro_grabacion = true;
        }
        if (strncmp(argv[j], "velocidad=", strlen("velocidad=") ) == 0){
            velocidad = (float) atof(&(argv[j][strlen("velocidad=")])); 
        }
    }
    if (se_encontro_grabacion){
        fread(&juego, sizeof(juego_t), 1, archivo_grabacion);
        while (!feof(archivo_grabacion)){
            mostrar_juego(juego);
            detener_el_tiempo(velocidad);
            system("clear");
            fread(&juego, sizeof(juego_t), 1, archivo_grabacion);
        }
    }
    fclose(archivo_grabacion);
}

/*
 *
 */
void grabar_partida(juego_t juego, FILE* archivo_grabacion){
    fwrite(&juego, sizeof(juego_t), 1, archivo_grabacion);
}

/*
 *
 */
void guardar_puntaje(int puntaje, char nombre_jugador[MAX_NOMBRE], char nombre_archivo_configuracion[MAX_NOMBRE]){
    char nombre_archivo_ranking[MAX_NOMBRE];
    if (strcmp(nombre_archivo_configuracion, CONFIGURACION_POR_DEFECTO) == 0){
        strcpy(nombre_archivo_ranking, "ranking.csv");
    }
    else {
        strcpy(nombre_archivo_ranking, "ranking_");
        strcat(nombre_archivo_ranking,strtok(nombre_archivo_configuracion, "."));
        strcat(nombre_archivo_ranking, ".csv");
    }
    FILE* archivo_ranking = fopen(nombre_archivo_ranking, LECTURA);
    if (!archivo_ranking){
		printf("El archivo de ranking no existe, creando uno nuevo...\n");
	}
    FILE* archivo_ranking_actualizado = fopen("archivo_ranking_actualizado.csv", "w");
    if (!archivo_ranking_actualizado){
        fclose(archivo_ranking);
		printf("El archivo de ranking no pudo ser creado.\n");
		return;
	}
    int puntaje_archivo;
    char nombre_jugador_archivo[MAX_NOMBRE];
    if (archivo_ranking){
        bool se_guardo_puntaje_nuevo = false;
        int leidos = fscanf(archivo_ranking, "%[^;];%i\n", nombre_jugador_archivo, &puntaje_archivo);
        while (leidos == 2){
            if (puntaje > puntaje_archivo && !se_guardo_puntaje_nuevo){
                fprintf(archivo_ranking_actualizado, "%s;%i\n", nombre_jugador, puntaje);
                se_guardo_puntaje_nuevo = true;
            }
            if (puntaje == puntaje_archivo && !se_guardo_puntaje_nuevo){
                if (strcmp(nombre_jugador, nombre_jugador_archivo) > 0){
                    fprintf(archivo_ranking_actualizado, "%s;%i\n", nombre_jugador, puntaje);
                    se_guardo_puntaje_nuevo = true;
                }
            }
            fprintf(archivo_ranking_actualizado, "%s;%i\n", nombre_jugador_archivo, puntaje_archivo);
            leidos = fscanf(archivo_ranking, "%[^;];%i\n", nombre_jugador_archivo, &puntaje_archivo);
        }
        if (!se_guardo_puntaje_nuevo){
            fprintf(archivo_ranking_actualizado, "%s;%i\n", nombre_jugador, puntaje);
        }
        fclose(archivo_ranking);
    }
    else{
        fprintf(archivo_ranking_actualizado, "%s;%i\n", nombre_jugador, puntaje);
    }
    fclose(archivo_ranking_actualizado);
    rename("archivo_ranking_actualizado.csv", nombre_archivo_ranking);
}

/*
 *
 */
void obtener_puntaje(configuracion_t configuracion, int enemigos_muertos, int* puntaje, char nombre_jugador[MAX_NOMBRE], char nombre_archivo_configuracion[MAX_NOMBRE]){
    int numero_defensores = 0;
    for (int i = 0; i < 4; i++){
        numero_defensores += configuracion.enanos_iniciales[i];
        numero_defensores += configuracion.elfos_iniciales[i];
    }
    (*puntaje) = (enemigos_muertos*1000) / ((configuracion.resistencia_torres[0]) + (configuracion.resistencia_torres[1]) + (numero_defensores) + (configuracion.elfos_extra[0]) + (configuracion.enanos_extra[0]));
    printf("%s tu puntaje fue de %i. Guardando puntaje...\n", nombre_jugador, (*puntaje));
    detener_el_tiempo(3);
    guardar_puntaje((*puntaje), nombre_jugador, nombre_archivo_configuracion);
}

/*
 *
 */
void obtener_camino_creado(FILE* archivo_caminos, coordenada_t camino_1[MAX_LONGITUD_CAMINO],coordenada_t camino_2[MAX_LONGITUD_CAMINO], int* tope_camino_1, int* tope_camino_2){
    int nivel;
    int numero_camino;
    int i;
    fscanf(archivo_caminos, "NIVEL=%i\n", &nivel);
    if (nivel == 1){
        fscanf(archivo_caminos, "CAMINO=%i\n", &numero_camino);
        int leidos = fscanf(archivo_caminos, "%i;%i\n", &camino_1[0].fil, &camino_1[0].col);
        i = 0;
        while (leidos == 2){
            i++;
            leidos = fscanf(archivo_caminos, "%i;%i\n", &camino_1[i].fil, &camino_1[i].col);
            (*tope_camino_1)++;
        }
        return;
    }
    if (nivel == 2){
        fscanf(archivo_caminos, "CAMINO=%i\n", &numero_camino);
        int leidos = fscanf(archivo_caminos, "%i;%i\n", &camino_2[0].fil, &camino_2[0].col);
        i = 0;
        while (leidos == 2){
            i++;
            leidos = fscanf(archivo_caminos, "%i;%i\n", &camino_2[i].fil, &camino_2[i].col);
            (*tope_camino_2)++;
        }
        return;
    }
    if (nivel == 3 || nivel == 4){
        fscanf(archivo_caminos, "CAMINO=%i\n", &numero_camino);
        int leidos = fscanf(archivo_caminos, "%i;%i\n", &camino_1[0].fil, &camino_1[0].col);
        i = 0;
        while (leidos == 2){
            i++;
            leidos = fscanf(archivo_caminos, "%i;%i\n", &camino_1[i].fil, &camino_1[i].col);
            (*tope_camino_1)++;
        }
        fscanf(archivo_caminos, "CAMINO=%i\n", &numero_camino);
        leidos = fscanf(archivo_caminos, "%i;%i\n", &camino_2[0].fil, &camino_2[0].col);
        i = 0;
        while (leidos == 2){
            i++;
            leidos = fscanf(archivo_caminos, "%i;%i\n", &camino_2[i].fil, &camino_2[i].col);
            (*tope_camino_2)++;
        }
        return;
    }
}

/*
 *
 */
void cargar_configuracion_por_defecto(configuracion_t* configuracion){
    (*configuracion).resistencia_torres[0] = 600;
    (*configuracion).resistencia_torres[1] = 600;
    (*configuracion).elfos_extra[0] = DEFENSORES_EXTRA;
    (*configuracion).enanos_extra[0] = DEFENSORES_EXTRA;
    (*configuracion).elfos_extra[1] = 0;
    (*configuracion).enanos_extra[1] = DANIO_DEFENSOR_EXTRA;
    (*configuracion).elfos_extra[2] = DANIO_DEFENSOR_EXTRA;
    (*configuracion).enanos_extra[2] = 0;
    (*configuracion).animo_enanos[0] = -1;
    (*configuracion).animo_enanos[1] = -1;
    (*configuracion).animo_elfos[0] = -1;
    (*configuracion).animo_elfos[1] = -1;
    (*configuracion).enanos_iniciales[0] = DEFENSORES_NIVEL_1;
    (*configuracion).enanos_iniciales[1] = 0;
    (*configuracion).enanos_iniciales[2] = DEFENSORES_NIVEL_3 / 2;
    (*configuracion).enanos_iniciales[3] = DEFENSORES_NIVEL_4 / 2;
    (*configuracion).elfos_iniciales[0] = 0;
    (*configuracion).elfos_iniciales[1] = DEFENSORES_NIVEL_2;
    (*configuracion).elfos_iniciales[2] = DEFENSORES_NIVEL_3 / 2;
    (*configuracion).elfos_iniciales[3] = DEFENSORES_NIVEL_4 / 2;
    (*configuracion).velocidad = VELOCIDAD_POR_DEFECTO;
    strcpy((*configuracion).archivo_caminos, "-1");
}

/*
 *
 */
bool obtener_configuracion(char nombre_archivo_configuracion[MAX_NOMBRE], configuracion_t* configuracion){
    FILE* archivo_configuracion = fopen(nombre_archivo_configuracion, LECTURA);
	if (!archivo_configuracion){
		printf("El archivo de configuracion no existe\n");
		return false;
	}
    fscanf(archivo_configuracion, "RESISTENCIA_TORRES=%i,%i\n", &(*configuracion).resistencia_torres[0], &(*configuracion).resistencia_torres[1]);
    fscanf(archivo_configuracion, "ENANOS_INICIO=%i,%i,%i,%i\n", &(*configuracion).enanos_iniciales[0], &(*configuracion).enanos_iniciales[1], &(*configuracion).enanos_iniciales[2],&(*configuracion).enanos_iniciales[3]);
    fscanf(archivo_configuracion, "ELFOS_INICIO=%i,%i,%i,%i\n", &(*configuracion).elfos_iniciales[0], &(*configuracion).elfos_iniciales[1], &(*configuracion).elfos_iniciales[2],&(*configuracion).elfos_iniciales[3]);
    fscanf(archivo_configuracion, "ENANOS_EXTRA=%i,%i,%i\n", &(*configuracion).enanos_extra[0], &(*configuracion).enanos_extra[1], &(*configuracion).enanos_extra[2]);
    fscanf(archivo_configuracion, "ELFOS_EXTRA=%i,%i,%i\n", &(*configuracion).elfos_extra[0], &(*configuracion).elfos_extra[1], &(*configuracion).elfos_extra[2]);
    fscanf(archivo_configuracion, "ENANOS_ANIMO=%i,%i\n", &(*configuracion).animo_enanos[0], &(*configuracion).animo_enanos[1]); 
    fscanf(archivo_configuracion, "ELFOS_ANIMO=%i,%i\n", &(*configuracion).animo_elfos[0], &(*configuracion).animo_elfos[1]);
    fscanf(archivo_configuracion, "VELOCIDAD=%f\n", &(*configuracion).velocidad);
    fscanf(archivo_configuracion, "CAMINOS=%s\n", (*configuracion).archivo_caminos);
    if ((*configuracion).resistencia_torres[0] == POR_DEFECTO){
        (*configuracion).resistencia_torres[0] = 600;
    }
    if ((*configuracion).resistencia_torres[1] == POR_DEFECTO){
        (*configuracion).resistencia_torres[1] = 600;
    }
    if ((*configuracion).elfos_extra[0] == POR_DEFECTO){
        (*configuracion).elfos_extra[0] = DEFENSORES_EXTRA;
    }
    if ((*configuracion).enanos_extra[0] == POR_DEFECTO){
        (*configuracion).enanos_extra[0] = DEFENSORES_EXTRA;
    }
    if ((*configuracion).elfos_extra[1] == POR_DEFECTO){
        (*configuracion).elfos_extra[1] = 0;
    }
    if ((*configuracion).enanos_extra[1] == POR_DEFECTO){
        (*configuracion).enanos_extra[1] = DANIO_DEFENSOR_EXTRA;
    }
    if ((*configuracion).elfos_extra[2] == POR_DEFECTO){
        (*configuracion).elfos_extra[2] = DANIO_DEFENSOR_EXTRA;
    }
    if ((*configuracion).enanos_extra[2] == POR_DEFECTO){
        (*configuracion).enanos_extra[2] = 0;
    }
    if ((*configuracion).enanos_iniciales[0] == POR_DEFECTO){
        (*configuracion).enanos_iniciales[0] = DEFENSORES_NIVEL_1;
    }
    if ((*configuracion).enanos_iniciales[1] == POR_DEFECTO){
        (*configuracion).enanos_iniciales[1] = 0;
    }
    if ((*configuracion).enanos_iniciales[2] == POR_DEFECTO){
        (*configuracion).enanos_iniciales[2] = DEFENSORES_NIVEL_3 / 2;
    }
    if ((*configuracion).enanos_iniciales[3] == POR_DEFECTO){
        (*configuracion).enanos_iniciales[3] = DEFENSORES_NIVEL_4 / 2;
    }
    if ((*configuracion).elfos_iniciales[0] == POR_DEFECTO){
        (*configuracion).elfos_iniciales[0] = 0;
    }
    if ((*configuracion).elfos_iniciales[1] == POR_DEFECTO){
        (*configuracion).elfos_iniciales[1] = DEFENSORES_NIVEL_2;
    }
    if ((*configuracion).elfos_iniciales[2] == POR_DEFECTO){
        (*configuracion).elfos_iniciales[2] = DEFENSORES_NIVEL_3 / 2;
    }
    if ((*configuracion).elfos_iniciales[3] == POR_DEFECTO){
        (*configuracion).elfos_iniciales[3] = DEFENSORES_NIVEL_4 / 2;
    }
    if ((*configuracion).velocidad == POR_DEFECTO){
        (*configuracion).velocidad = VELOCIDAD_POR_DEFECTO;
    }
    return true;
}

/*
 *
 */
void crear_configuracion(char nombre_archivo_configuracion[MAX_NOMBRE]){
    FILE* archivo_configuracion = fopen(nombre_archivo_configuracion, ESCRITURA);
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

/*
 *
 */
void mostrar_camino_creado(juego_t* juego_temporal, char terreno[MAX_TERRENO][MAX_TERRENO], int nivel, int filas_terreno, int columnas_terreno){
    cargar_camino(*juego_temporal, terreno, 1);
    cargar_camino(*juego_temporal, terreno, 2);
    imprimir_numeros(columnas_terreno);
    imprimir_lineas(columnas_terreno);
    imprimir_terreno(terreno,filas_terreno,columnas_terreno);
    imprimir_lineas(columnas_terreno);
}

/*
 *
 */
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

/*
 *
 */
void crear_caminos(char nombre_archivo_camino[MAX_NOMBRE]){
    FILE* caminos = fopen(nombre_archivo_camino, ESCRITURA);
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

/*
 *
 */
void mostrar_ranking(int numero_jugadores_a_mostrar, char nombre_archivo_ranking[MAX_NOMBRE]){
    printf("Archivo de ranking: %s\n", nombre_archivo_ranking);
    FILE* ranking = fopen(nombre_archivo_ranking, LECTURA);
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

/*
 *
 */
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

/*
 *
 */
void ejecutar_comando_crear_camino(int argc, char* argv[]){
    if (argc < 3){
        printf("Debes especificar el nombre del archivo para el camino\n");
        return;
    }
    char nombre_archivo_camino[MAX_NOMBRE];
    strcpy(nombre_archivo_camino, argv[2]);
    crear_caminos(nombre_archivo_camino);
}

/*
 *
 */
void ejecutar_comando_crear_configuracion(int argc, char* argv[]){
    if (argc < 3){
        printf("Debes especificar el nombre del archivo para la configuracion\n");
        return;
    }
    char nombre_archivo_configuracion[MAX_NOMBRE];
    strcpy(nombre_archivo_configuracion, argv[2]);
    crear_configuracion(nombre_archivo_configuracion);
}