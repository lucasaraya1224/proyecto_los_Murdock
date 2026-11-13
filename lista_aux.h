#ifndef LISTA_AUX_H
#define LISTA_AUX_H

#include "estructuras.h"
#include "mapa.h"

// Inicializa el arreglo dinámico temporal para reportes
ListaAuxiliar* inicializar_lista(int capacidad_inicial);

// Agrega una referencia de producto a la lista.
void agregar_a_lista(ListaAuxiliar* lista, Producto p);

// Llena la lista auxiliar recorriendo secuencialmente la tabla hash
void extraer_productos_mapa(MapaProductos* mapa, ListaAuxiliar* lista);

// Ordena la lista utilizando un algoritmo eficiente (ej. Quicksort O(n log n))
void ordenar_lista_por_codigo(ListaAuxiliar* lista);

// Libera la memoria del arreglo de punteros (sin liberar los productos en sí)
void liberar_lista(ListaAuxiliar* lista);

#endif