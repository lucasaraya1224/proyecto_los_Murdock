#include <stdio.h>
#include <stdlib.h>
#include "cola.h"

ColaVentas* inicializar_cola() {
    ColaVentas* cola = (ColaVentas*)malloc(sizeof(ColaVentas));
    cola->frente = NULL;
    cola->final = NULL;
    return cola;
}

void encolar_venta(ColaVentas* cola, SolicitudVenta solicitud) {
    NodoCola* nuevo_nodo = (NodoCola*)malloc(sizeof(NodoCola));
    nuevo_nodo->solicitud = solicitud;
    nuevo_nodo->siguiente = NULL;
    
    if (cola->final == NULL) {
        cola->frente = nuevo_nodo;
        cola->final = nuevo_nodo;
    } else {
        cola->final->siguiente = nuevo_nodo;
        cola->final = nuevo_nodo;
    }
}

int desencolar_venta(ColaVentas* cola, SolicitudVenta* solicitud_salida) {
    if (cola->frente == NULL) return 0;
    
    NodoCola* temp = cola->frente;
    *solicitud_salida = temp->solicitud;
    
    cola->frente = cola->frente->siguiente;
    if (cola->frente == NULL) {
        cola->final = NULL;
    }
    
    free(temp);
    return 1;
}

int ver_frente(ColaVentas* cola, SolicitudVenta* solicitud_frente) {
    if (cola->frente == NULL) return 0;
    *solicitud_frente = cola->frente->solicitud;
    return 1;
}

int cola_vacia(ColaVentas* cola) {
    return cola->frente == NULL;
}

void liberar_cola(ColaVentas* cola) {
    SolicitudVenta temp;
    while (!cola_vacia(cola)) {
        desencolar_venta(cola, &temp);
    }
    free(cola);
}