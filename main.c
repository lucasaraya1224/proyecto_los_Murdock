#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "estructuras.h"
#include "mapa.h"
#include "cola.h"
#include "lista_aux.h"

/* =========================================
   PROTOTIPOS DE FUNCIONES DE INTERFAZ
   ========================================= */
void menu_catalogo(MapaProductos* mapa);
void menu_ventas(ColaVentas* cola, MapaProductos* mapa);
void menu_reposicion(MapaProductos* mapa);
void menu_estado(MapaProductos* mapa);
void menu_archivos(MapaProductos* mapa);
void limpiar_pantalla();
void pausar();

/* =========================================
   FUNCIÓN PRINCIPAL
   ========================================= */
int main() {
    MapaProductos* catalogo = inicializar_mapa(1000);
    ColaVentas* cola_pendientes = inicializar_cola();
    
    int opcion = -1;

    do {
        limpiar_pantalla();
        printf("=========================================\n");
        printf("   SISTEMA DE INVENTARIO MINIMARKET PUCV \n");
        printf("=========================================\n");
        printf("1. Catalogo de productos\n");
        printf("2. Ventas\n");
        printf("3. Reposicion\n");
        printf("4. Estado del inventario\n");
        printf("5. Archivos\n");
        printf("0. Salir\n");
        printf("=========================================\n");
        printf("Ingrese una opcion: ");
        
        if (scanf("%d", &opcion) != 1) {
            while (getchar() != '\n'); 
            opcion = -1;
        }

        switch (opcion) {
            case 1: menu_catalogo(catalogo); break;
            case 2: menu_ventas(cola_pendientes, catalogo); break;
            case 3: menu_reposicion(catalogo); break;
            case 4: menu_estado(catalogo); break;
            case 5: menu_archivos(catalogo); break;
            case 0: 
                if (!cola_vacia(cola_pendientes)) {
                    printf("\n=================================================================\n");
                    printf("ADVERTENCIA: Hay solicitudes de venta en la cola sin procesar.\n");
                    printf("Si cierras el programa ahora, estas operaciones se perderan.\n");
                    printf("=================================================================\n");
                    printf("Estas seguro que deseas salir de todos modos? (1 = Si, 0 = No, volver al menu): ");
                    
                    int confirmacion = 0;
                    if (scanf("%d", &confirmacion) != 1) {
                        while (getchar() != '\n'); 
                        confirmacion = 0;
                    }
                    
                    if (confirmacion != 1) {
                        opcion = -1; // Reseteamos la opcion para evitar que el bucle termine
                        printf("\nSalida cancelada. Volviendo al menu principal...\n");
                        pausar();
                        break; // Rompe el switch, pero el bucle do-while continua
                    }
                }
                printf("\nSaliendo del sistema...\n");
                break;
            default:
                printf("\nOpcion invalida. Por favor, intente de nuevo.\n");
                pausar();
                break;
        }
    } while (opcion != 0);

    liberar_mapa(catalogo);
    liberar_cola(cola_pendientes);
    return 0;
}



void menu_catalogo(MapaProductos* mapa) {
    int opcion = -1;
    do {
        limpiar_pantalla();
        printf("=========================================\n");
        printf("         CATALOGO DE PRODUCTOS           \n");
        printf("=========================================\n");
        printf("1. Agregar producto\n");
        printf("2. Buscar por codigo\n");
        printf("3. Editar datos\n");
        printf("4. Desactivar producto\n");
        printf("5. Listar todos\n");
        printf("0. Volver\n");
        printf("=========================================\n");
        printf("Ingrese una opcion: ");
        
        if (scanf("%d", &opcion) != 1) {
            while (getchar() != '\n'); 
            opcion = -1;
        }

        switch (opcion) {
            case 1: {
                Producto p;
                printf("\n--- 1. Agregar Producto ---\n");
                printf("Codigo: ");
                scanf("%d", &p.codigo);
                while (getchar() != '\n'); // Limpiar buffer para leer cadenas de texto
                
                printf("Nombre: ");
                fgets(p.nombre, 50, stdin);
                p.nombre[strcspn(p.nombre, "\n")] = 0; // Quitar salto de linea de fgets
                
                printf("Categoria: ");
                fgets(p.categoria, 30, stdin);
                p.categoria[strcspn(p.categoria, "\n")] = 0;
                
                printf("Precio: ");
                scanf("%f", &p.precio);
                
                printf("Stock inicial: ");
                scanf("%d", &p.stock);
                
                printf("Stock minimo: ");
                scanf("%d", &p.minimo);
                
                p.activo = 1; // Activo por defecto al crearse
                
                // Validaciones logicas exigidas en el informe[cite: 1]
                if (p.precio < 0 || p.stock < 0 || p.minimo < 0) {
                    printf("Error: El precio, stock y minimo no pueden ser negativos.\n");
                } else {
                    if (agregar_producto(mapa, p)) {
                        printf("Exito: Producto agregado al catalogo.\n");
                    } else {
                        printf("Error: Ya existe un producto con el codigo %d.\n", p.codigo);
                    }
                }
                pausar();
                break;
            }
            case 2: {
                int codigo;
                printf("\n--- 2. Buscar Producto ---\n");
                printf("Ingrese el codigo a buscar: ");
                scanf("%d", &codigo);
                
                Producto* p = buscar_producto(mapa, codigo);
                if (p != NULL) {
                    printf("\n[Producto Encontrado]\n");
                    printf("Codigo: %d\nNombre: %s\nCategoria: %s\n", p->codigo, p->nombre, p->categoria);
                    printf("Precio: $%.2f\nStock: %d (Minimo: %d)\nEstado: %s\n", 
                           p->precio, p->stock, p->minimo, p->activo ? "Activo" : "Desactivado");
                } else {
                    printf("Error: Producto no encontrado.\n");
                }
                pausar();
                break;
            }
            case 3: {
                int codigo;
                printf("\n--- 3. Editar Datos ---\n");
                printf("Ingrese el codigo del producto a editar: ");
                scanf("%d", &codigo);
                
                Producto* p = buscar_producto(mapa, codigo);
                if (p != NULL) {
                    while (getchar() != '\n'); 
                    printf("Nuevo nombre (actual: %s): ", p->nombre);
                    fgets(p->nombre, 50, stdin);
                    p->nombre[strcspn(p->nombre, "\n")] = 0;
                    
                    printf("Nueva categoria (actual: %s): ", p->categoria);
                    fgets(p->categoria, 30, stdin);
                    p->categoria[strcspn(p->categoria, "\n")] = 0;
                    
                    printf("Nuevo precio (actual: %.2f): ", p->precio);
                    scanf("%f", &p->precio);
                    
                    printf("Nuevo minimo (actual: %d): ", p->minimo);
                    scanf("%d", &p->minimo);
                    
                    // Nota: Segun el informe, el stock NO se edita aqui, solo con ventas o reposicion[cite: 1]
                    printf("Datos actualizados con exito.\n");
                } else {
                    printf("Error: Producto no encontrado.\n");
                }
                pausar();
                break;
            }
            case 4: {
                int codigo;
                printf("\n--- 4. Desactivar Producto ---\n");
                printf("Ingrese el codigo del producto a desactivar: ");
                scanf("%d", &codigo);
                
                if (desactivar_producto(mapa, codigo)) {
                    printf("Exito: Producto desactivado. Se conservara en catalogo pero no aceptara ventas.\n");
                } else {
                    printf("Error: Producto no encontrado.\n");
                }
                pausar();
                break;
            }
            case 5: {
                // Se crea la lista auxiliar dinamicamente para mostrar los datos ordenados
                int capacidad_inicial = mapa->tamano_actual > 0 ? mapa->tamano_actual : 10;
                ListaAuxiliar* lista = inicializar_lista(capacidad_inicial);
                
                extraer_productos_mapa(mapa, lista);
                ordenar_lista_por_codigo(lista);
                
                if (lista->cantidad == 0) {
                    printf("\nEl catalogo esta vacio.\n");
                    pausar();
                } else {
                    int productos_por_pagina = 15;
                    int total_paginas = (lista->cantidad + productos_por_pagina - 1) / productos_por_pagina;
                    int pagina_actual = 1;
                    char accion = ' ';
                    
                    while (accion != '0') {
                        limpiar_pantalla();
                        printf("\n==========================================================================================================\n");
                        printf("                          CATALOGO DE PRODUCTOS (Pagina %d de %d)\n", pagina_actual, total_paginas);
                        printf("==========================================================================================================\n");
                        
                        // Encabezados con separadores de tabla para mayor legibilidad
                        printf("%-8s | %-32s | %-15s | %-10s | %-7s | %-7s | %-11s\n", 
                               "Codigo", "Nombre", "Categoria", "Precio", "Stock", "Minimo", "Estado");
                        printf("---------+----------------------------------+-----------------+------------+---------+---------+------------\n");
                        
                        // Calculo del rango de productos a mostrar en la pagina actual
                        int inicio = (pagina_actual - 1) * productos_por_pagina;
                        int fin = inicio + productos_por_pagina;
                        if (fin > lista->cantidad) fin = lista->cantidad;
                        
                        for (int i = inicio; i < fin; i++) {
                            Producto p = lista->productos[i];
                            // El formato %-32.32s asegura que ocupe exactamente 32 caracteres, sin desbordar ni desalinear
                            printf("%-8d | %-32.32s | %-15.15s | $%-9.2f | %-7d | %-7d | %-11s\n", 
                                   p.codigo, p.nombre, p.categoria, p.precio, p.stock, p.minimo, p.activo ? "Activo" : "Desactivado");
                        }
                        printf("---------+----------------------------------+-----------------+------------+---------+---------+------------\n");
                        
                        // Menu de navegacion inferior
                        printf("\nOpciones de navegacion:\n");
                        if (pagina_actual < total_paginas) printf("[S] Pagina Siguiente    ");
                        if (pagina_actual > 1) printf("[A] Pagina Anterior     ");
                        printf("[0] Volver al menu\n");
                        printf("Ingrese su accion: ");
                        
                        scanf(" %c", &accion);
                        while (getchar() != '\n'); // Limpiar el buffer del teclado
                        
                        // Logica de transicion de paginas
                        if ((accion == 's' || accion == 'S') && pagina_actual < total_paginas) {
                            pagina_actual++;
                        } else if ((accion == 'a' || accion == 'A') && pagina_actual > 1) {
                            pagina_actual--;
                        }
                    }
                }
                liberar_lista(lista); // Se libera la memoria de la vista temporal
                break; // No requiere usar la funcion pausar() ya que el while retiene la pantalla
            }
            case 0:
                break;
            default:
                printf("\nOpcion invalida. Por favor, intente de nuevo.\n");
                pausar();
                break;
        }
    } while (opcion != 0);
}
void menu_ventas(ColaVentas* cola, MapaProductos* mapa) {
    int opcion = -1;
    static int contador_operaciones = 1; // Generador automatico de IDs unicos para solicitudes

    do {
        limpiar_pantalla();
        printf("=========================================\n");
        printf("            MODULO DE VENTAS             \n");
        printf("=========================================\n");
        printf("1. Registrar solicitud de venta\n");
        printf("2. Ver solicitudes pendientes\n");
        printf("3. Procesar siguiente venta\n");
        printf("4. Cancelar siguiente solicitud\n");
        printf("0. Volver\n");
        printf("=========================================\n");
        printf("Ingrese una opcion: ");
        
        if (scanf("%d", &opcion) != 1) {
            while (getchar() != '\n'); 
            opcion = -1;
        }

        switch (opcion) {
            case 1: {
                printf("\n--- 1. Registrar Solicitud ---\n");
                SolicitudVenta nueva_solicitud;
                nueva_solicitud.id_operacion = contador_operaciones++;
                
                printf("Ingrese el codigo del producto: ");
                scanf("%d", &nueva_solicitud.codigo_producto);
                
                printf("Ingrese la cantidad a comprar: ");
                scanf("%d", &nueva_solicitud.cantidad);
                
                // Validacion: la cantidad debe ser positiva[cite: 1]
                if (nueva_solicitud.cantidad <= 0) {
                    printf("Error: La cantidad debe ser mayor a 0.\n");
                } else {
                    encolar_venta(cola, nueva_solicitud);
                    printf("Exito: Solicitud #%d encolada correctamente. (Stock sin alterar hasta procesar)\n", 
                           nueva_solicitud.id_operacion);
                }
                pausar();
                break;
            }
            case 2: {
                if (cola_vacia(cola)) {
                    printf("\nNo hay solicitudes de venta pendientes.\n");
                    pausar();
                } else {
                    // 1. Contar elementos y pasarlos a un arreglo temporal para paginarlos
                    int total_solicitudes = 0;
                    NodoCola* actual = cola->frente;
                    while (actual != NULL) {
                        total_solicitudes++;
                        actual = actual->siguiente;
                    }

                    SolicitudVenta* solicitudes_array = (SolicitudVenta*)malloc(total_solicitudes * sizeof(SolicitudVenta));
                    actual = cola->frente;
                    int idx = 0;
                    while (actual != NULL) {
                        solicitudes_array[idx++] = actual->solicitud;
                        actual = actual->siguiente;
                    }

                    // 2. Logica de Paginacion
                    int items_por_pagina = 15;
                    int total_paginas = (total_solicitudes + items_por_pagina - 1) / items_por_pagina;
                    int pagina_actual = 1;
                    char accion = ' ';

                    while (accion != '0') {
                        limpiar_pantalla();
                        printf("\n=========================================================\n");
                        printf("          SOLICITUDES PENDIENTES (Pagina %d de %d)\n", pagina_actual, total_paginas);
                        printf("=========================================================\n");
                        
                        printf("%-10s | %-15s | %-10s\n", "ID Oper.", "Cod. Producto", "Cantidad");
                        printf("-----------+-----------------+-----------\n");

                        int inicio = (pagina_actual - 1) * items_por_pagina;
                        int fin = inicio + items_por_pagina;
                        if (fin > total_solicitudes) fin = total_solicitudes;

                        for (int i = inicio; i < fin; i++) {
                            printf("%-10d | %-15d | %-10d\n",
                                   solicitudes_array[i].id_operacion,
                                   solicitudes_array[i].codigo_producto,
                                   solicitudes_array[i].cantidad);
                        }
                        printf("-----------+-----------------+-----------\n");

                        // Navegacion
                        printf("\nOpciones de navegacion:\n");
                        if (pagina_actual < total_paginas) printf("[S] Pagina Siguiente    ");
                        if (pagina_actual > 1) printf("[A] Pagina Anterior     ");
                        printf("[0] Volver al menu\n");
                        printf("Ingrese su accion: ");

                        scanf(" %c", &accion);
                        while (getchar() != '\n');

                        if ((accion == 's' || accion == 'S') && pagina_actual < total_paginas) {
                            pagina_actual++;
                        } else if ((accion == 'a' || accion == 'A') && pagina_actual > 1) {
                            pagina_actual--;
                        }
                    }
                    free(solicitudes_array); // Liberar arreglo temporal
                }
                break;
            }
            case 3: {
                printf("\n--- 3. Procesar Venta ---\n");
                SolicitudVenta solicitud_actual;
                ResultadoOperacion resultado;
                
                // Se retira del frente de la cola en O(1)[cite: 9]
                if (desencolar_venta(cola, &solicitud_actual)) {
                    resultado.id_operacion = solicitud_actual.id_operacion;
                    
                    // Se busca el producto en la tabla Hash[cite: 9]
                    Producto* p = buscar_producto(mapa, solicitud_actual.codigo_producto);
                    
                    // Validaciones fisicas y logicas[cite: 1, 9]
                    if (p == NULL) {
                        resultado.estado = 0; // Rechazada
                        snprintf(resultado.motivo, sizeof(resultado.motivo), "Producto con codigo %d no existe.", solicitud_actual.codigo_producto);
                    } else if (p->activo == 0) {
                        resultado.estado = 0; // Rechazada
                        snprintf(resultado.motivo, sizeof(resultado.motivo), "Producto '%s' se encuentra desactivado.", p->nombre);
                    } else if (p->stock < solicitud_actual.cantidad) {
                        resultado.estado = 0; // Rechazada
                        snprintf(resultado.motivo, sizeof(resultado.motivo), "Stock insuficiente. Solicitado: %d, Disponible: %d.", solicitud_actual.cantidad, p->stock);
                    } else {
                        // Confirmacion y descuento de unidades[cite: 1, 9]
                        p->stock -= solicitud_actual.cantidad;
                        resultado.estado = 1; // Confirmada
                        snprintf(resultado.motivo, sizeof(resultado.motivo), "Venta confirmada exitosamente. Nuevo stock: %d", p->stock);
                    }
                    
                    // Imprimir el resultado de la operacion
                    printf("Solicitud #%d procesada:\n", resultado.id_operacion);
                    printf("Estado: %s\n", resultado.estado == 1 ? "CONFIRMADA" : "RECHAZADA");
                    printf("Detalle: %s\n", resultado.motivo);
                    
                } else {
                    printf("No hay solicitudes pendientes para procesar.\n");
                }
                pausar();
                break;
            }
            case 4: {
                printf("\n--- 4. Cancelar Solicitud ---\n");
                SolicitudVenta solicitud_cancelada;
                
                if (desencolar_venta(cola, &solicitud_cancelada)) {
                    // Se retira sin alterar el inventario[cite: 1]
                    printf("Exito: La solicitud #%d por el producto %d ha sido cancelada.\n", 
                           solicitud_cancelada.id_operacion, solicitud_cancelada.codigo_producto);
                    printf("El stock del catalogo no ha sido modificado.\n");
                } else {
                    printf("No hay solicitudes pendientes para cancelar.\n");
                }
                pausar();
                break;
            }
            case 0:
                break;
            default:
                printf("\nOpcion invalida. Por favor, intente de nuevo.\n");
                pausar();
                break;
        }
    } while (opcion != 0);
}
void menu_reposicion(MapaProductos* mapa) {
    int opcion = -1;
    do {
        limpiar_pantalla();
        printf("=========================================\n");
        printf("          MODULO DE REPOSICION           \n");
        printf("=========================================\n");
        printf("1. Registrar entrada\n");
        printf("2. Consultar producto antes de reponer\n");
        printf("0. Volver\n");
        printf("=========================================\n");
        printf("Ingrese una opcion: ");
        
        if (scanf("%d", &opcion) != 1) {
            while (getchar() != '\n'); 
            opcion = -1;
        }

        switch (opcion) {
            case 1: {
                int codigo, cantidad;
                printf("\n--- 1. Registrar Entrada ---\n");
                printf("Ingrese el codigo del producto: ");
                scanf("%d", &codigo);
                
                printf("Ingrese la cantidad a reponer: ");
                scanf("%d", &cantidad);
                
                if (cantidad <= 0) {
                    printf("Error: La cantidad debe ser positiva.\n");
                } else {
                    Producto* p = buscar_producto(mapa, codigo);
                    if (p != NULL) {
                        // Muestra stock anterior, suma y comunica el nuevo total[cite: 1]
                        int stock_anterior = p->stock;
                        p->stock += cantidad;
                        printf("Exito: Reposicion completada.\n");
                        printf("Producto: %s | Stock anterior: %d | Nuevo total: %d\n", 
                               p->nombre, stock_anterior, p->stock);
                    } else {
                        printf("Error: Producto no existe. Los articulos nuevos deben registrarse desde el catalogo.\n");
                    }
                }
                pausar();
                break;
            }
            case 2: {
                int codigo;
                printf("\n--- 2. Consultar Producto ---\n");
                printf("Ingrese el codigo a consultar: ");
                scanf("%d", &codigo);
                
                Producto* p = buscar_producto(mapa, codigo);
                if (p != NULL) {
                    printf("Producto: %s | Stock actual: %d | Minimo: %d | Estado: %s\n", 
                           p->nombre, p->stock, p->minimo, p->activo ? "Activo" : "Desactivado");
                } else {
                    printf("Error: Producto no encontrado.\n");
                }
                pausar();
                break;
            }
            case 0:
                break;
            default:
                printf("\nOpcion invalida.\n");
                pausar();
                break;
        }
    } while (opcion != 0);
}

void menu_estado(MapaProductos* mapa) {
    int opcion = -1;
    do {
        limpiar_pantalla();
        printf("=========================================\n");
        printf("         ESTADO DEL INVENTARIO           \n");
        printf("=========================================\n");
        printf("1. Productos agotados\n");
        printf("2. Productos bajo minimo\n");
        printf("3. Resumen general\n");
        printf("0. Volver\n");
        printf("=========================================\n");
        printf("Ingrese una opcion: ");
        
        if (scanf("%d", &opcion) != 1) {
            while (getchar() != '\n'); 
            opcion = -1;
        }

        switch (opcion) {
            case 1: 
            case 2: {
                int capacidad_inicial = mapa->tamano_actual > 0 ? mapa->tamano_actual : 10;
                
                // Obtenemos todos los productos y los ordenamos
                ListaAuxiliar* lista_completa = inicializar_lista(capacidad_inicial);
                extraer_productos_mapa(mapa, lista_completa);
                ordenar_lista_por_codigo(lista_completa);
                
                // Creamos una lista filtrada solo con los que cumplen la condicion para paginarlos
                ListaAuxiliar* lista_filtrada = inicializar_lista(capacidad_inicial);
                
                for (int i = 0; i < lista_completa->cantidad; i++) {
                    Producto p = lista_completa->productos[i];
                    int condicion = (opcion == 1) ? (p.stock == 0) : (p.stock > 0 && p.stock <= p.minimo);
                    
                    if (condicion && p.activo) {
                        agregar_a_lista(lista_filtrada, p);
                    }
                }
                
                if (lista_filtrada->cantidad == 0) {
                    printf("\nNo se encontraron productos en esta categoria.\n");
                    pausar();
                } else {
                    int productos_por_pagina = 15;
                    int total_paginas = (lista_filtrada->cantidad + productos_por_pagina - 1) / productos_por_pagina;
                    int pagina_actual = 1;
                    char accion = ' ';
                    
                    while (accion != '0') {
                        limpiar_pantalla();
                        printf("\n=================================================================================\n");
                        printf("                 %s (Pagina %d de %d)\n", 
                               opcion == 1 ? "PRODUCTOS AGOTADOS" : "PRODUCTOS BAJO MINIMO", 
                               pagina_actual, total_paginas);
                        printf("=================================================================================\n");
                        
                        printf("%-8s | %-32s | %-7s | %-7s\n", "Codigo", "Nombre", "Stock", "Minimo");
                        printf("---------+----------------------------------+---------+---------\n");
                        
                        int inicio = (pagina_actual - 1) * productos_por_pagina;
                        int fin = inicio + productos_por_pagina;
                        if (fin > lista_filtrada->cantidad) fin = lista_filtrada->cantidad;
                        
                        for (int i = inicio; i < fin; i++) {
                            Producto p = lista_filtrada->productos[i];
                            printf("%-8d | %-32.32s | %-7d | %-7d\n", p.codigo, p.nombre, p.stock, p.minimo);
                        }
                        printf("---------+----------------------------------+---------+---------\n");
                        
                        // Navegacion
                        printf("\nOpciones de navegacion:\n");
                        if (pagina_actual < total_paginas) printf("[S] Pagina Siguiente    ");
                        if (pagina_actual > 1) printf("[A] Pagina Anterior     ");
                        printf("[0] Volver al menu\n");
                        printf("Ingrese su accion: ");
                        
                        scanf(" %c", &accion);
                        while (getchar() != '\n');
                        
                        if ((accion == 's' || accion == 'S') && pagina_actual < total_paginas) {
                            pagina_actual++;
                        } else if ((accion == 'a' || accion == 'A') && pagina_actual > 1) {
                            pagina_actual--;
                        }
                    }
                }
                
                // Liberamos ambas listas de la memoria
                liberar_lista(lista_completa);
                liberar_lista(lista_filtrada);
                break;
            }
            case 3: {
                printf("\n--- 3. Resumen General ---\n");
                int referencias_activas = 0;
                int unidades_totales = 0;
                
                // Recorrido de la tabla para calcular totales[cite: 1]
                for (int i = 0; i < mapa->capacidad; i++) {
                    NodoHash* actual = mapa->buckets[i];
                    while (actual != NULL) {
                        if (actual->producto.activo) {
                            referencias_activas++;
                            unidades_totales += actual->producto.stock;
                        }
                        actual = actual->siguiente;
                    }
                }
                
                printf("Cantidad de referencias activas en catalogo: %d\n", referencias_activas);
                printf("Total de unidades fisicas disponibles: %d\n", unidades_totales);
                pausar();
                break;
            }
            case 0:
                break;
            default:
                printf("\nOpcion invalida.\n");
                pausar();
                break;
        }
    } while (opcion != 0);
}

void menu_archivos(MapaProductos* mapa) {
    int opcion = -1;
    do {
        limpiar_pantalla();
        printf("=========================================\n");
        printf("          MODULO DE ARCHIVOS             \n");
        printf("=========================================\n");
        printf("1. Cargar catalogo desde archivo\n");
        printf("2. Guardar catalogo en archivo\n");
        printf("0. Volver\n");
        printf("=========================================\n");
        printf("Ingrese una opcion: ");
        
        if (scanf("%d", &opcion) != 1) {
            while (getchar() != '\n'); 
            opcion = -1;
        }

        switch (opcion) {
            case 1: {
                char nombre_archivo[100];
                printf("\n--- 1. Cargar Archivo ---\n");
                printf("Ingrese el nombre del archivo (ej. inventario.txt): ");
                while (getchar() != '\n'); // Limpiar buffer
                fgets(nombre_archivo, 100, stdin);
                nombre_archivo[strcspn(nombre_archivo, "\n")] = 0;

                FILE* archivo = fopen(nombre_archivo, "r");
                if (archivo == NULL) {
                    printf("Error: No se pudo abrir el archivo '%s'. Verifique que exista.\n", nombre_archivo);
                } else {
                    int aceptados = 0;
                    int rechazados = 0;
                    Producto p;
                    char linea[200];
                    
                    // Lee el archivo linea por linea
                    while (fgets(linea, sizeof(linea), archivo)) {
                        // El formato esperado es separado por punto y coma (;)
                        if (sscanf(linea, "%d;%49[^;];%29[^;];%f;%d;%d;%d",
                                   &p.codigo, p.nombre, p.categoria, &p.precio, 
                                   &p.stock, &p.minimo, &p.activo) == 7) {
                            
                            // Se validan los datos y se inserta en el mapa hash[cite: 1]
                            if (p.precio >= 0 && p.stock >= 0 && p.minimo >= 0) {
                                if (agregar_producto(mapa, p)) {
                                    aceptados++;
                                } else {
                                    rechazados++; // Falla por codigo duplicado[cite: 1]
                                }
                            } else {
                                rechazados++; // Falla por valores negativos
                            }
                        } else {
                            rechazados++; // Fila invalida (formato incorrecto)[cite: 1]
                        }
                    }
                    fclose(archivo);
                    printf("Lectura finalizada.\n");
                    printf("Registros aceptados: %d\n", aceptados);
                    printf("Registros rechazados (duplicados o invalidos): %d\n", rechazados);
                }
                pausar();
                break;
            }
            case 2: {
                char nombre_archivo[100];
                printf("\n--- 2. Guardar Archivo ---\n");
                printf("Ingrese el nombre del archivo para guardar (ej. inventario.txt): ");
                while (getchar() != '\n');
                fgets(nombre_archivo, 100, stdin);
                nombre_archivo[strcspn(nombre_archivo, "\n")] = 0;

                FILE* archivo = fopen(nombre_archivo, "w");
                if (archivo == NULL) {
                    printf("Error: No se pudo crear el archivo '%s'.\n", nombre_archivo);
                } else {
                    int guardados = 0;
                    // Se recorre la tabla hash para exportar el estado actual[cite: 1]
                    for (int i = 0; i < mapa->capacidad; i++) {
                        NodoHash* actual = mapa->buckets[i];
                        while (actual != NULL) {
                            Producto p = actual->producto;
                            // Se guarda usando punto y coma para evitar problemas con nombres que tengan espacios
                            fprintf(archivo, "%d;%s;%s;%.2f;%d;%d;%d\n",
                                    p.codigo, p.nombre, p.categoria, p.precio, 
                                    p.stock, p.minimo, p.activo);
                            guardados++;
                            actual = actual->siguiente;
                        }
                    }
                    fclose(archivo);
                    printf("Exito: Se han guardado %d productos en '%s'.\n", guardados, nombre_archivo);
                }
                pausar();
                break;
            }
            case 0:
                break;
            default:
                printf("\nOpcion invalida.\n");
                pausar();
                break;
        }
    } while (opcion != 0);
}

/* =========================================
   FUNCIONES UTILITARIAS
   ========================================= */
void limpiar_pantalla() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void pausar() {
    printf("\nPresione Enter para continuar...");
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    getchar(); 
}