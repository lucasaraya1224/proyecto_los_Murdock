#ifndef ESTRUCTURAS_H
#define ESTRUCTURAS_H

/* =========================================
   1. ENTIDADES DEL NEGOCIO
   ========================================= */

typedef struct {
    int codigo;           // Identificador único
    char nombre[50];      // Nombre del producto
    char categoria[30];   // Categoría del producto
    float precio;         // Valor unitario (debe ser >= 0)
    int stock;            // Unidades disponibles (debe ser >= 0)
    int minimo;           // Umbral mínimo de existencias
    int activo;           // 1: disponible, 0: desactivado
} Producto;

typedef struct {
    int id_operacion;     // Identificador único de la solicitud
    int codigo_producto;  // Artículo solicitado en el catálogo
    int cantidad;         // Número de unidades a adquirir (debe ser > 0)
} SolicitudVenta;

typedef struct {
    int id_operacion;     // Identifica la solicitud correspondiente
    int estado;           // 1: confirmada, 0: rechazada, -1: cancelada
    char motivo[100];     // Causa del resultado de la operación
} ResultadoOperacion;

/* =========================================
   2. ESTRUCTURAS FÍSICAS (TDAs)
   ========================================= */

typedef struct NodoHash {
    Producto producto;
    struct NodoHash* siguiente;
} NodoHash;

typedef struct {
    NodoHash** buckets;   // Arreglo estático de punteros a listas enlazadas
    int capacidad;        // Tamaño físico del arreglo
    int tamano_actual;    // <--- CAMBIADO: Cantidad de productos reales registrados
} MapaProductos;

// TDA Cola FIFO: Lista enlazada simple con doble puntero
typedef struct NodoCola {
    SolicitudVenta solicitud;
    struct NodoCola* siguiente;
} NodoCola;

typedef struct {
    NodoCola* frente;     // Referencia directa al primer elemento
    NodoCola* final;      // Referencia directa al último elemento
} ColaVentas;

// TDA Lista Auxiliar: Arreglo dinámico
typedef struct {
    Producto* productos;  //Arreglo dinámico de copias de Producto
    int capacidad;
    int cantidad;
} ListaAuxiliar;

#endif