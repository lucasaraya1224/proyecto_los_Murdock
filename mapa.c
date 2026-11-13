#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mapa.h"

MapaProductos* inicializar_mapa(int capacidad) {
    MapaProductos* mapa = (MapaProductos*)malloc(sizeof(MapaProductos));
    mapa->capacidad = capacidad;
    mapa->tamano_actual = 0;
    mapa->buckets = (NodoHash**)calloc(capacidad, sizeof(NodoHash*));
    return mapa;
}

int funcion_hash(int codigo, int capacidad) {
    return codigo % capacidad;
}

void rehash(MapaProductos* mapa) {
    int nueva_capacidad = mapa->capacidad * 2;
    NodoHash** nuevos_buckets = (NodoHash**)calloc(nueva_capacidad, sizeof(NodoHash*));
    
    for (int i = 0; i < mapa->capacidad; i++) {
        NodoHash* actual = mapa->buckets[i];
        while (actual != NULL) {
            NodoHash* siguiente_temp = actual->siguiente;
            
            int nuevo_indice = funcion_hash(actual->producto.codigo, nueva_capacidad);
            actual->siguiente = nuevos_buckets[nuevo_indice];
            nuevos_buckets[nuevo_indice] = actual;
            
            actual = siguiente_temp;
        }
    }
    
    free(mapa->buckets);
    mapa->buckets = nuevos_buckets;
    mapa->capacidad = nueva_capacidad;
}

int agregar_producto(MapaProductos* mapa, Producto p) {

    float factor_carga = (float)mapa->tamano_actual / mapa->capacidad;
    if (factor_carga >= 0.75) {
        rehash(mapa);
    }

    int indice = funcion_hash(p.codigo, mapa->capacidad);
    NodoHash* actual = mapa->buckets[indice];
    
    while (actual != NULL) {
        if (actual->producto.codigo == p.codigo) {
            return 0; // Rechaza duplicados
        }
        actual = actual->siguiente;
    }
    
    NodoHash* nuevo_nodo = (NodoHash*)malloc(sizeof(NodoHash));
    nuevo_nodo->producto = p;
    nuevo_nodo->siguiente = mapa->buckets[indice];
    mapa->buckets[indice] = nuevo_nodo;
    mapa->tamano_actual++;
    
    return 1;
}

Producto* buscar_producto(MapaProductos* mapa, int codigo) {
    int indice = funcion_hash(codigo, mapa->capacidad);
    NodoHash* actual = mapa->buckets[indice];
    
    while (actual != NULL) {
        if (actual->producto.codigo == codigo) {
            return &(actual->producto);
        }
        actual = actual->siguiente;
    }
    return NULL; // No encontrado
}

int desactivar_producto(MapaProductos* mapa, int codigo) {
    Producto* p = buscar_producto(mapa, codigo);
    if (p != NULL) {
        p->activo = 0;
        return 1;
    }
    return 0;
}

void liberar_mapa(MapaProductos* mapa) {
    for (int i = 0; i < mapa->capacidad; i++) {
        NodoHash* actual = mapa->buckets[i];
        while (actual != NULL) {
            NodoHash* temp = actual;
            actual = actual->siguiente;
            free(temp);
        }
    }
    free(mapa->buckets);
    free(mapa);
}