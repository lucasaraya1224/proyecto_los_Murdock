#ifndef COLA_H
#define COLA_H

#include "estructuras.h"

// Inicializa la cola FIFO.
ColaVentas* inicializar_cola();

void encolar_venta(ColaVentas* cola, SolicitudVenta solicitud);

int desencolar_venta(ColaVentas* cola, SolicitudVenta* solicitud_salida);

// Consulta la solicitud ubicada al frente sin retirarla.
int ver_frente(ColaVentas* cola, SolicitudVenta* solicitud_frente);

// Verifica si la cola está vacía.
int cola_vacia(ColaVentas* cola);

// Libera la memoria de la cola.
void liberar_cola(ColaVentas* cola);

#endif