#ifndef MAPA_H
#define MAPA_H

#include "estructuras.h"

// Inicializa la tabla hash con una capacidad específica.
MapaProductos* inicializar_mapa(int capacidad);

// Función hash básica para obtener el índice a partir del código.
int funcion_hash(int codigo, int capacidad);

// Agrega un producto; rechaza duplicados evaluando la unicidad del código[cite: 1].
int agregar_producto(MapaProductos* mapa, Producto p);

// Busca un producto por su código con complejidad promedio O(1)[cite: 1].
Producto* buscar_producto(MapaProductos* mapa, int codigo);

// Marca un artículo como no disponible para nuevas ventas (activo = 0)[cite: 1].
int desactivar_producto(MapaProductos* mapa, int codigo);

// Libera la memoria asignada a la tabla hash.
void liberar_mapa(MapaProductos* mapa);

#endif