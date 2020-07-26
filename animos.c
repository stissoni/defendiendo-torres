#include <stdio.h>
#include <stdio_ext.h>
#include <stdbool.h>

#include "comandos.h"

const int DIA_MIN = 1;
const int DIA_MAX = 30;
const char PIE_IZQUIERDO = 'I';
const char PIE_DERECHO = 'D';
const char ENSALADA = 'E';
const char HAMBURGUESA = 'H';
const char PIZZA = 'P';
const char GUISO = 'G';
const char MANANA = 'M';
const char TARDE = 'T';
const char NOCHE = 'N';
const int SUENO_MIN = 0;
const int SUENO_MAX = 12;
const char MALO = 'M';
const char REGULAR = 'R';
const char BUENO = 'B';

void recibir_dia_mes(int* dia);
bool es_dia_incorrecto(int dia);
void recibir_hora_dia(char* hora);
bool es_hora_incorrecta(char hora);
void recibir_pie_cena_suenio(char* pie, char* cena, int* suenio);
void calcular_animo(int* personaje_animo, char pie, char cena, int sueno);
bool es_pie_incorrecto(char pie);
bool es_comida_incorrecta(char cena);
bool es_sueno_incorrecto(int sueno);
int valor_velocidad_viento(int dia_mes);
int valor_porcentaje_humedad(char hora_dia);
char valor_animo(int valor_animo);


void animos(int* viento, int* humedad , char* animo_legolas , char* animo_gimli, configuracion_t configuracion){
    printf("Bienvenidos al Abismo de Helm!\n\n");
    int dia_mes;
    if (configuracion.animo_elfos[0] == POR_DEFECTO){
        recibir_dia_mes(&dia_mes);
        *viento = valor_velocidad_viento(dia_mes);
    }
    char hora_dia;
    if (configuracion.animo_enanos[0] == POR_DEFECTO){
        recibir_hora_dia(&hora_dia);
        *humedad = valor_porcentaje_humedad(hora_dia);
    }
    char pie, cena;
    int horas_suenio;
    int valor_animo_gimli;
    int valor_animo_legolas;
    if (configuracion.animo_elfos[1] == POR_DEFECTO){
        printf("\nVeamos el animo de vuestros lideres, primero el de Legolas:\n");
        recibir_pie_cena_suenio(&pie,&cena,&horas_suenio);
        calcular_animo(&valor_animo_legolas, pie, cena, horas_suenio);
        *animo_legolas = valor_animo(valor_animo_legolas);
    }
    if (configuracion.animo_enanos[1] == POR_DEFECTO){
        printf("\nAhora el animo de Gimli!:\n");
        recibir_pie_cena_suenio(&pie, &cena, &horas_suenio);
        calcular_animo(&valor_animo_gimli, pie, cena, horas_suenio);
        *animo_gimli = valor_animo(valor_animo_gimli);
    }
}

void recibir_dia_mes(int* dia){
    // Ingresa por teclado dia del mes, verifica si es mayor a 0 y menor a 31, y lo guarda.
    printf("Ingresar el dia del mes (1-30): ");
    __fpurge(stdin);
    scanf("%i", dia);
    while(es_dia_incorrecto(*dia)){
        printf("Dia incorrecto, ingresarlo nuevamente (1-30): ");
        __fpurge(stdin);
        scanf("%i", dia);
    }
}

bool es_dia_incorrecto(int dia){
    // Verificar si el dia es correcto, devuelve verdadero. Falso en caso contrario
    if ((dia < DIA_MIN) || (dia > DIA_MAX)){
        return true;
    }
    else {
        return false;
    }
}

void recibir_hora_dia(char* hora){
    // Ingresa por teclado hora del dia, y lo guarda.
    printf("Ingresar hora del dia (M/T/N): ");
    __fpurge(stdin);
    scanf("%c", hora);
    while(es_hora_incorrecta(*hora)){
        printf("Hora del dia incorrecta, ingresar nuevamente (M/T/N): ");
        __fpurge(stdin);
        scanf("%c", hora);
    }
}

bool es_hora_incorrecta(char hora){
    // Verificar si la hora es M, T o N, si es asi devuelve verdadero. Falso en caso contrario
    if ((hora != MANANA) && (hora != TARDE) && (hora != NOCHE)){
        return true;
    }
    else {
        return false;
    }
}

void recibir_pie_cena_suenio(char* pie, char* cena, int* suenio){
    // Recibe una serie de parametros por teclado: pie, cena y horas de suenio. Los guarda en las variables correspondientes.
    printf("Ingresar el pie con el que se levanto (I/D): ");
    __fpurge(stdin);
    scanf("%c", pie);
    while(es_pie_incorrecto(*pie)){
        printf("Pie incorrecto, ingresar nuevamente (I/D): ");
        __fpurge(stdin);
        scanf("%c", pie);
    }
    printf("Ingresar la cena de la noche anterior (E/H/P/G): ");
    __fpurge(stdin);
    scanf("%c", cena);
    while(es_comida_incorrecta(*cena)){
        printf("Cena incorrecta, ingresar nuevamente (E/H/P/G): ");
        __fpurge(stdin);
        scanf("%c", cena);
    }
    printf("Ingresar cantidad de horas que durmio (0-12): ");
    __fpurge(stdin);
    scanf("%i", suenio);
    while(es_sueno_incorrecto(*suenio)){
        printf("Cantidad de horas incorrecta, ingresar nuevamente (0-12): ");
        __fpurge(stdin);
        scanf("%i", suenio);
    }
} 

void calcular_animo(int* personaje_animo, char pie, char cena, int sueno){
    // En funcion del lider y el pie, la cena y las horas de suenio, calcula un numero que corresponde a su animo.
    int animo = 0;
    if (pie == PIE_DERECHO){
        animo+=10;
    }
    if (cena == ENSALADA){
        animo+=20;
    }
    if (cena == HAMBURGUESA){
        animo+=15;
    }
    if (cena == PIZZA){
        animo+=10;
    }
    if (cena == GUISO){
        animo+=5;
    }
    if ((sueno > 4) && 9 > sueno){
        animo+=10;
    }
    if (sueno > 8){
        animo+=20;
    }
    *personaje_animo = animo;
}

bool es_pie_incorrecto(char pie){
    // Verificar si el pie es I o D, si es asi devuelve verdadero. Falso en caso contrario.
    if ((pie != PIE_IZQUIERDO) && (pie != PIE_DERECHO)){
        return true;
    }
    else {
        return false;
    }
}

bool es_comida_incorrecta(char cena){
    // Verificar si la comida es E, H, P o G, si es asi devuelve verdadero. Falso en caso contrario
    if ((cena != ENSALADA) && (cena != HAMBURGUESA) && (cena != PIZZA) && (cena != GUISO)){
        return true;
    }
    else {
        return false;
    }
}

bool es_sueno_incorrecto(int sueno){
    // Verificar si las horas de sueño estan entre 0 y 12, si es asi devuelve verdadero. Falso en caso contrario
    if ((sueno < SUENO_MIN) || (sueno > SUENO_MAX)){
        return true;
    }
    else {
        return false;
    }
}

int valor_velocidad_viento(int dia_mes){
    // Recibe un numero entre 1 y 31 para decidr la velocidad del viento. Devuelve 25, 50 o 75.
    if ((dia_mes > 0) && (10 >= dia_mes)){
        return 75;
    }
    if ((dia_mes > 10) && (20 >= dia_mes)){
        return 50;
    }
    else
        return 25;
}

int valor_porcentaje_humedad(char hora_dia){
    // Recibe un caracter M, T o N, y calcula la humedad. Devuelve 25, 50 o 75.
    if (hora_dia == MANANA){
        return 75;
    }
    if (hora_dia == TARDE){
        return 25;
    }
    else
        return 50;
}

char valor_animo(int valor_animo){
    // En funcion del numero de animo, devuelve M si es menor a 20, B si es mayor a 36, en otro caso R.
    if (valor_animo >= 36){
        return BUENO;
    }
    if ((valor_animo >= 21) && (35 >= valor_animo)){
        return REGULAR;
    }
    if (20 >= valor_animo){
        return MALO;
    }
    else{
        return 0;
    }
}