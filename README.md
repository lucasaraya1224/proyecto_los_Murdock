# Sistema de Administración de Inventario - Minimarket PUCV

## Descripción del Proyecto

Este proyecto es una aplicación de consola desarrollada en C para gestionar el inventario de un supermercado (minimarket). Permite registrar productos, controlar el stock mediante una cola de ventas (FIFO), y generar reportes de estado y alertas de reposición utilizando una tabla hash para búsquedas eficientes en tiempo promedio O(1).

## Cómo Ejecutar el Programa

El programa está dividido en múltiples archivos (`main.c`, `mapa.c`, `cola.c`, `lista_aux.c` y sus respectivos `.h`). Para compilar y ejecutar correctamente en **Windows con Visual Studio Code (MinGW / GCC)**, sigue estos pasos:

### Opción 1: Compilación Manual por Terminal (Recomendado)

1. Abre tu terminal integrada en Visual Studio Code (`Ctrl` + `ñ`).

2. Asegúrate de estar en la carpeta del proyecto.

3. Compila todos los archivos `.c` con el siguiente comando:

   gcc *.c -o main.exe
   

4. Ejecuta el programa compilado:

   .\main.exe
   

### Opción 2: Usando VS Code (Botón Play / F5)

Si tienes configurado tu archivo `tasks.json` en VS Code para compilar múltiples archivos:

1. Abre el archivo `main.c`.

2. Presiona `Ctrl + F5` (Ejecutar sin depuración) para lanzar la consola de manera limpia y comenzar a interactuar con el menú.

*Nota: Se incluye un archivo `inventario.txt` de prueba. Para cargarlo, inicia el programa, ve a la opción 5 (Archivos) > 1 (Cargar catálogo) y escribe `inventario.txt`.*

## Ejemplo de Ejecución (Prueba Paso a Paso)
Sigue este flujo para validar todas las integraciones de los TDAs y las reglas del negocio:

### Fase 1: Carga y Creación (TDA Mapa)
1. Inicia el programa y ve a **5. Archivos** > **1. Cargar catalogo**. Ingresa `inventario.txt` (debe existir en la misma carpeta).
2. Ve a **1. Catalogo de productos** > **1. Agregar producto**. Registra un artículo de prueba (Ej: Código `9999`, Precio `1500`, Stock `5`, Mínimo `10`).
3. Ve a **1. Catalogo de productos** > **2. Buscar por codigo**. Ingresa `9999` para confirmar que se guardó correctamente.

### Fase 2: Mutación de Stock (Reposición)
4. Ve a **3. Reposicion** > **1. Registrar entrada**. Ingresa el código `9999` y repón `2` unidades. El sistema informará que el stock subió de `5` a `7`.

### Fase 3: Ventas y Validaciones (TDA Cola + Mapa)
5. Ve a **2. Ventas** > **1. Registrar solicitud**. Ingresa código `9999` y cantidad `4` (ID 1).
6. Registra una segunda solicitud con el código `1000` y cantidad `2` (ID 2).
7. Ve a **1. Catalogo de productos** > **4. Desactivar producto** e inhabilita el código `1000`.
8. Ve a **2. Ventas** > **3. Procesar siguiente venta**. Se procesará la solicitud ID 1 (Código 9999) y se confirmará, bajando su stock a `3`.
9. Vuelve a presionar **3. Procesar siguiente venta**. Se procesará la solicitud ID 2 (Código 1000) y el sistema la **rechazará** automáticamente indicando que el producto está desactivado.

### Fase 4: Vistas y Persistencia (TDA Lista Auxiliar)
10. Ve a **4. Estado del inventario** > **2. Productos bajo minimo**. El artículo `9999` aparecerá listado porque su stock actual (`3`) es inferior a su mínimo (`10`).
11. Ve a **5. Archivos** > **2. Guardar catalogo**. Escribe `inventario_final.txt`.
12. Ve a **0. Salir** para cerrar la aplicación de forma segura.

## Estado de las Funciones y Limitaciones

De acuerdo a la pauta de evaluación, a continuación se detalla el estado actual del programa:

### Opciones que FUNCIONAN correctamente:

* **Catálogo (Opción 1):** Agregar (con validación de duplicados), Buscar, Editar, Desactivar y Listar todos (con paginación).

* **Ventas (Opción 2):** Encolar solicitudes, Ver pendientes, Procesar venta (validando stock real) y Cancelar.

* **Reposición (Opción 3):** Registrar nuevas entradas de stock y Consultar.

* **Estado (Opción 4):** Filtrar productos agotados y bajo mínimo, y mostrar el resumen general. Todo incluye paginación.

* **Archivos (Opción 5):** Carga y guardado persistente en archivos de texto (`.txt` o `.csv`) separados por punto y coma (`;`).

### Limitaciones conocidas (Lo que el sistema NO hace por diseño):

* **Búsqueda por nombre:** El sistema actual requiere ingresar el código numérico exacto del producto. No admite búsquedas por aproximación de texto (substring).

* **Stock Comprometido:** Al encolar ventas, el sistema no reserva el stock inmediatamente. Si se encolan múltiples ventas que superan el stock de un mismo producto, el sistema lo permite temporalmente y recién rechazará las transacciones inválidas al momento de procesarlas (Opción 2 > 3).

* **Flujo de dinero:** El sistema es exclusivamente para control de unidades físicas; no calcula ganancias, vuelto, ni flujo de caja.

* **Multiusuario:** El programa está diseñado para un solo operador local. No maneja transacciones concurrentes.

## Contribuciones del Equipo (Grupo Los Murdock)

El desarrollo, diseño y documentación de este proyecto fue un esfuerzo conjunto. A continuación, se detalla la contribución específica de cada integrante:

* **Vicente Vasquez (Coordinación):** Organizó y coordinó al grupo, asegurando que cada miembro cumpliera sus roles y funciones en los plazos establecidos.

* **Lucas Araya (Comunicador):** Encargado de dictar las instrucciones, guiar el avance del proyecto y articular la comunicación entre las diferentes partes del desarrollo.

* **Agustin Miranda (Encargado de presentación):** Creador y diseñador de la presentación visual en Canva, asegurando que los TDA y diagramas fueran claros para la defensa.

* **Bruno Guaiquiante (Gestor de calidad):** Evaluó de manera crítica los puntos a mejorar del proyecto, comprobando el código, los errores lógicos y asegurando el cumplimiento de la rúbrica.

* **Antonio Puebla y Vania Caceres (Escritores de Informe):** Redactaron el informe técnico, modelaron el comportamiento de las estructuras (Mapa, Cola, Lista) y documentaron las complejidades teóricas.

* **Pablo Salinas (Integrador):** Responsable de unir las diferentes partes del proyecto, integrando los módulos de código (TDA) en una sola unidad funcional (`main.c`).