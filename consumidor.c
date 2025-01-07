#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mosquitto.h>
#include "libConsulta.h"

// Estructura para almacenar el mensaje recibido
typedef struct {
    int matricula;
    char nombre[100];
    char primer_apellido[100];
    char segundo_apellido[100];
    char materia[100];
    char calificacion[10];
    char carrera[100];
} Mensaje;

// Callback cuando el consumidor recibe un mensaje del broker
void on_message(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *message) {
    // Extraemos los datos del mensaje recibido (suponemos que están en formato JSON o un formato similar)
    Mensaje msg;
    sscanf(message->payload, "%d,%99[^,],%99[^,],%99[^,],%99[^,],%9[^,],%99[^,]",
           &msg.matricula, msg.nombre, msg.primer_apellido, msg.segundo_apellido,
           msg.materia, msg.calificacion, msg.carrera);

    // Imprimimos el mensaje para confirmar que los datos son correctos
    printf("Mensaje recibido: %d, %s, %s, %s, %s, %s, %s\n", msg.matricula, msg.nombre, msg.primer_apellido,
           msg.segundo_apellido, msg.materia, msg.calificacion, msg.carrera);

    // Insertamos los datos en la base de datos usando la librería libConsulta
    if (insertar_en_bd(msg.matricula, msg.nombre, msg.primer_apellido, msg.segundo_apellido,
                       msg.materia, msg.calificacion, msg.carrera) == 0) {
        printf("Datos insertados correctamente en la base de datos.\n");
    } else {
        printf("Error al insertar los datos en la base de datos.\n");
    }
}

int main() {
    struct mosquitto *mosq;
    char *host = "localhost";
    int port = 1883;
    char *topic = "estudiantes/topic";

    // Inicializar la conexión al broker MQTT
    mosquitto_lib_init();
    mosq = mosquitto_new(NULL, true, NULL);

    if (mosquitto_connect(mosq, host, port, 60) != MOSQ_ERR_SUCCESS) {
        fprintf(stderr, "Error al conectar con el broker MQTT.\n");
        return -1;
    }

    // Nos suscribimos al topic para recibir mensajes
    mosquitto_subscribe(mosq, NULL, topic, 0);

    // Configurar el callback para el manejo de mensajes
    mosquitto_message_callback_set(mosq, on_message);

    // Conectamos a la base de datos
    if (inicializar_conexion("localhost", "root", "tesoem", "prueba") != 0) {
        fprintf(stderr, "Error al conectar con la base de datos.\n");
        return -1;
    }

    // Loop para mantener el consumidor esperando mensajes
    while (1) {
        mosquitto_loop(mosq, -1, 1);
    }

    // Cerrar la conexión con el broker y la base de datos
    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();
    cerrar_conexion();

    return 0;
}