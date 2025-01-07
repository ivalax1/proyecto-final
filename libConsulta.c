#include <stdio.h>
#include <stdlib.h>
#include <mariadb/mysql.h>  // Cambié la cabecera a la correcta para MariaDB

// Función para conectar a la base de datos
MYSQL* conectar_base_datos() {
    MYSQL *conn = mysql_init(NULL);

    if (conn == NULL) {
        fprintf(stderr, "Error al inicializar la conexión: %s\n", mysql_error(conn));
        exit(1);
    }

    // Parámetros de conexión
    char *server = "localhost";   // Dirección del servidor
    char *user = "root";          // Nombre de usuario
    char *password = "tesoem";    // Contraseña
    char *database = "prueba";    // Nombre de la base de datos

    // Intentar establecer la conexión
    if (mysql_real_connect(conn, server, user, password, database, 0, NULL, 0) == NULL) {
        fprintf(stderr, "Error de conexión: %s\n", mysql_error(conn));
        mysql_close(conn);  // Cerrar la conexión si falla
        exit(1);
    }

    return conn;
}

// Función para insertar datos en la base de datos
void insertar_datos(MYSQL *conn, int matricula, const char *nombre, const char *primer_apellido, 
                    const char *segundo_apellido, const char *materia, const char *calificacion, const char *carrera) {
    // Preparar la consulta de inserción
    char query[512];
    snprintf(query, sizeof(query), 
             "INSERT INTO estudiantes (matricula, nombre, primer_apellido, segundo_apellido, materia, calificacion, carrera) "
             "VALUES (%d, '%s', '%s', '%s', '%s', '%s', '%s');",
             matricula, nombre, primer_apellido, segundo_apellido, materia, calificacion, carrera);

    // Ejecutar la consulta
    if (mysql_query(conn, query)) {
        fprintf(stderr, "Error al insertar datos: %s\n", mysql_error(conn));
        exit(1);
    } else {
        printf("Datos insertados correctamente.\n");
    }
}

int main() {
    // Conectar a la base de datos
    MYSQL *conn = conectar_base_datos();

    // Datos de ejemplo (puedes cambiar estos valores por los que recibas del consumidor)
    int matricula = 12345;
    const char *nombre = "Juan";
    const char *primer_apellido = "Perez";
    const char *segundo_apellido = "Lopez";
    const char *materia = "Matematicas";
    const char *calificacion = "A";
    const char *carrera = "Ingenieria";

    // Insertar los datos en la base de datos
    insertar_datos(conn, matricula, nombre, primer_apellido, segundo_apellido, materia, calificacion, carrera);

    // Cerrar la conexión a la base de datos
    mysql_close(conn);

    return 0;
}