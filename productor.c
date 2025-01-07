GNU nano 5.4                                                                                                                                                                             productor.c                                                                                                                                                                                       
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mosquitto.h>

// Estructura para representar los datos del estudiante
typedef struct {
    int matricula;
    char nombre[100];
    char primer_apellido[100];
    char segundo_apellido[100];
    char materia[100];
    char calificacion[10];
    char carrera[100];
} Estudiante;

// Función para generar un mensaje con los datos del estudiante
void generar_mensaje(Estudiante *estudiante, char *mensaje) {
    snprintf(mensaje, 512, "%d,%s,%s,%s,%s,%s,%s", estudiante->matricula, estudiante->nombre,
             estudiante->primer_apellido, estudiante->segundo_apellido, estudiante->materia,
             estudiante->calificacion, estudiante->carrera);
}

int main() {
    struct mosquitto *mosq;
    char *host = "localhost";
    int port = 1883;
    char *topic = "estudiantes/topic";
    
    // Inicializar la librería MQTT
    mosquitto_lib_init();
    mosq = mosquitto_new(NULL, true, NULL);

    if (mosquitto_connect(mosq, host, port, 60) != MOSQ_ERR_SUCCESS) {
        fprintf(stderr, "Error al conectar con el broker MQTT.\n");
        return -1;
    }

    // Establecer datos para enviar como mensaje
    Estudiante estudiante1 = {12345, "Juan", "Perez", "Lopez", "Matematicas", "A", "Ingenieria"};
    Estudiante estudiante2 = {67890, "Maria", "Gonzalez", "Sanchez", "Filosofia", "B", "Humanidades"};

    // Crear los mensajes a partir de los datos de los estudiantes
    char mensaje1[512];
    char mensaje2[512];
    generar_mensaje(&estudiante1, mensaje1);
    generar_mensaje(&estudiante2, mensaje2);

    // Publicar los mensajes en el broker
    if (mosquitto_publish(mosq, NULL, topic, strlen(mensaje1), mensaje1, 0, false) != MOSQ_ERR_SUCCESS) {
        fprintf(stderr, "Error al publicar el mensaje 1.\n");
        return -1;
    }

    printf("Mensaje publicado: %s\n", mensaje1);

    if (mosquitto_publish(mosq, NULL, topic, strlen(mensaje2), mensaje2, 0, false) != MOSQ_ERR_SUCCESS) {
        fprintf(stderr, "Error al publicar el mensaje 2.\n");
        return -1;
    }

    printf("Mensaje publicado: %s\n", mensaje2);

    // Loop para permitir la comunicación con el broker
    mosquitto_loop_forever(mosq, -1, 1);

    // Destruir el cliente MQTT y liberar recursos
    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();

    return 0;
}