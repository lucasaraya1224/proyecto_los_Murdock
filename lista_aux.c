#include <stdio.h>
#include <stdlib.h>
#include "lista_aux.h"

ListaAuxiliar* inicializar_lista(int capacidad_inicial) {
    ListaAuxiliar* lista = (ListaAuxiliar*)malloc(sizeof(ListaAuxiliar));
    lista->capacidad = capacidad_inicial;
    lista->cantidad = 0;
    lista->productos = (Producto*)malloc(capacidad_inicial * sizeof(Producto)); 
    return lista;
}

void agregar_a_lista(ListaAuxiliar* lista, Producto p) {
    if (lista->cantidad == lista->capacidad) {
        lista->capacidad *= 2;
        lista->productos = (Producto*)realloc(lista->productos, lista->capacidad * sizeof(Producto));
    }
    // Guarda la copia por valor
    lista->productos[lista->cantidad] = p;
    lista->cantidad++;
}

void extraer_productos_mapa(MapaProductos* mapa, ListaAuxiliar* lista) {
    for (int i = 0; i < mapa->capacidad; i++) {
        NodoHash* actual = mapa->buckets[i];
        while (actual != NULL) {
            agregar_a_lista(lista, actual->producto);
            actual = actual->siguiente;
        }
    }
}

// Función auxiliar para qsort comparando valores
int comparar_por_codigo(const void* a, const void* b) {
    Producto* p1 = (Producto*)a;
    Producto* p2 = (Producto*)b;
    return p1->codigo - p2->codigo;
}

void ordenar_lista_por_codigo(ListaAuxiliar* lista) {
    if (lista->cantidad > 1) {
        qsort(lista->productos, lista->cantidad, sizeof(Producto), comparar_por_codigo);
    }
}

void liberar_lista(ListaAuxiliar* lista) {
    free(lista->productos);
    free(lista);
}