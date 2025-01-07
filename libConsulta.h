#ifndef LIBCONSULTA_H
#define LIBCONSULTA_H

// Función para inicializar la conexión con la base de datos
int inicializar_conexion(const char *server, const char *user, const char *password, const char *database);

// Función para insertar los datos en la base de datos
int insertar_en_bd(int matricula, const char *nombre, const char *primer_apellido, const char *segundo_apellido,
                   const char *materia, const char *calificacion, const char *carrera);

// Función para cerrar la conexión con la base de datos
void cerrar_conexion();

#endif