#include <stdio.h>
#include "utils.h" // O "utils_2.h" según el nombre de tu archivo de librería

#define MAX_RECETAS 50
#define MAX_INGREDIENTES 5

// 1. ESTRUCTURA ANIDADA
typedef struct {
    int tiempoPrepMin;
    int porciones;
} tDetalle;

// 2. ESTRUCTURA PRINCIPAL
typedef struct {
    int id;
    cadena nombre;
    bool estadoActivo;
    tDetalle detalle;
    float ingredientes[MAX_INGREDIENTES];
} tReceta;

///////////////////////////////////////////////////////////////////////////
//////////////////////////// PROTOTIPOS ///////////////////////////////////
///////////////////////////////////////////////////////////////////////////

void inicializarSistema(tReceta *lista, int maxRecetas);
void cargarIngredientesBase(cadena nombresIng[]);
void copiarCadena(cadena destino, cadena origen);
void leerTexto(cadena destino, cadena mensaje);
void mostrarMenu();

// Funciones CRUD
void altaReceta(tReceta *lista, cadena nombresIng[], int *cantRecetas);
void listarRecetas(tReceta *lista, cadena nombresIng[], int cantRecetas);
void buscarRecetaPorId(tReceta *lista, cadena nombresIng[], int cantRecetas);
void modificarReceta(tReceta *lista, cadena nombresIng[], int cantRecetas);
void bajaReceta(tReceta *lista, int cantRecetas);

// Algoritmos y Persistencia (Hito Final)
int buscarPosicionPorId(tReceta *lista, int cantRecetas, int idBuscado);
void ordenarRecetasPorTiempo(tReceta *lista, int cantRecetas);
void guardarEnArchivoBinario(tReceta *lista, int cantRecetas);
int cargarDesdeArchivoBinario(tReceta *lista, int maxRecetas);
void exportarReporteTexto(tReceta *lista, cadena nombresIng[], int cantRecetas);

///////////////////////////////////////////////////////////////////////////
/////////////////////////////// MAIN //////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

int main() {
    tReceta listaRecetas[MAX_RECETAS];
    cadena nombresIngredientes[MAX_INGREDIENTES];

    int cantidadRecetas = 0;
    int opcion = 0;
    bool salir = false;

    cargarIngredientesBase(nombresIngredientes);
    inicializarSistema(listaRecetas, MAX_RECETAS);

    // PERSISTENCIA AUTOMÁTICA AL INICIAR (Carga desde archivo binario si existe)[cite: 47]
    cantidadRecetas = cargarDesdeArchivoBinario(listaRecetas, MAX_RECETAS);

    // Bucle principal del menú (Sin break ni continue)[cite: 2]
    while (!salir) {
        mostrarMenu();
        opcion = leerEnteroEntre(1, 8, "Seleccione una opcion");

        if (opcion == 1) {
            altaReceta(listaRecetas, nombresIngredientes, &cantidadRecetas);
        } else if (opcion == 2) {
            listarRecetas(listaRecetas, nombresIngredientes, cantidadRecetas);
        } else if (opcion == 3) {
            buscarRecetaPorId(listaRecetas, nombresIngredientes, cantidadRecetas);
        } else if (opcion == 4) {
            modificarReceta(listaRecetas, nombresIngredientes, cantidadRecetas);
        } else if (opcion == 5) {
            bajaReceta(listaRecetas, cantidadRecetas);
        } else if (opcion == 6) {
            ordenarRecetasPorTiempo(listaRecetas, cantidadRecetas);
        } else if (opcion == 7) {
            exportarReporteTexto(listaRecetas, nombresIngredientes, cantidadRecetas);
        } else if (opcion == 8) {
            if (confirmaUsuario("¿Desea salir del sistema?")) {
                // PERSISTENCIA AUTOMÁTICA AL SALIR (Guarda en archivo binario)[cite: 47]
                guardarEnArchivoBinario(listaRecetas, cantidadRecetas);
                salir = true;
                printf("\nDatos guardados en disco. ¡Gracias por utilizar el Recetario!\n");
            }
        }
    }

    return 0;
}

///////////////////////////////////////////////////////////////////////////
/////////////////////////// IMPLEMENTACIONES //////////////////////////////
///////////////////////////////////////////////////////////////////////////

void copiarCadena(cadena destino, cadena origen) {
    int i = 0;
    while (origen[i] != '\0') {
        destino[i] = origen[i];
        i++;
    }
    destino[i] = '\0';
}

void leerTexto(cadena destino, cadena mensaje) {
    printf("%s: ", mensaje);
    scanf(" %[^\n]", destino);
}

void cargarIngredientesBase(cadena nombresIng[]) {
    copiarCadena(nombresIng[0], "Harina (gr)");
    copiarCadena(nombresIng[1], "Azucar (gr)");
    copiarCadena(nombresIng[2], "Huevos (unid)");
    copiarCadena(nombresIng[3], "Carne/Proteina (gr)");
    copiarCadena(nombresIng[4], "Aceite (ml)");
}

void inicializarSistema(tReceta *lista, int maxRecetas) {
    int i = 0;
    int j = 0;
    while (i < maxRecetas) {
        (lista + i)->id = 0;
        (lista + i)->estadoActivo = false;
        j = 0;
        while (j < MAX_INGREDIENTES) {
            (lista + i)->ingredientes[j] = 0.0f;
            j++;
        }
        i++;
    }
}

void mostrarMenu() {
    printf("\n=======================================================\n");
    printf("   RECETARIO DE CATERING - HITO FINAL (PERSISTENCIA)    \n");
    printf("=======================================================\n");
    printf("1. Cargar nueva receta (Alta)\n");
    printf("2. Listar todas las recetas registradas (Consulta)\n");
    printf("3. Buscar receta por ID\n");
    printf("4. Modificar receta existente (Actualizacion)\n");
    printf("5. Dar de baja una receta (Baja Logica)\n");
    printf("6. Ordenar recetas por tiempo de preparacion\n");
    printf("7. Exportar Reporte de Auditoria (.txt)\n");
    printf("8. Salir (Guardar datos y cerrar)\n");
    printf("-------------------------------------------------------\n");
}

int buscarPosicionPorId(tReceta *lista, int cantRecetas, int idBuscado) {
    int posicion = -1;
    int i = 0;
    bool encontrado = false;

    while (i < cantRecetas && !encontrado) {
        if (lista[i].estadoActivo && lista[i].id == idBuscado) {
            posicion = i;
            encontrado = true;
        } else {
            i++;
        }
    }
    return posicion;
}

void altaReceta(tReceta *lista, cadena nombresIng[], int *cantRecetas) {
    int nuevoId;
    int posExistente;

    printf("\n--- ALTA DE RECETA ---\n");

    if (*cantRecetas < MAX_RECETAS) {
        nuevoId = leerEnteroEntre(1, 9999, "Ingrese el ID de la receta");
        posExistente = buscarPosicionPorId(lista, *cantRecetas, nuevoId);

        if (posExistente != -1) {
            printf("ERROR: Ya existe una receta activa con el ID %d.\n", nuevoId);
        } else {
            int i = *cantRecetas;
            
            lista[i].id = nuevoId;
            leerTexto(lista[i].nombre, "Ingrese el nombre de la receta");
            
            lista[i].detalle.tiempoPrepMin = leerEnteroEntre(1, 1440, "Ingrese el tiempo de preparacion (minutos)");
            lista[i].detalle.porciones = leerEnteroEntre(1, 500, "Ingrese cantidad de porciones");

            printf("\n--- Cantidad de Ingredientes Requeridos ---\n");
            int j = 0;
            while (j < MAX_INGREDIENTES) {
                printf("%s: ", nombresIng[j]);
                lista[i].ingredientes[j] = leerFloatEntre(0.0f, 10000.0f, "Ingrese cantidad");
                j++;
            }

            lista[i].estadoActivo = true;
            (*cantRecetas)++;
            printf("\nSUCCESS: ¡Receta agregada exitosamente!\n");
        }
    } else {
        printf("ERROR: Sistema lleno. No se pueden agregar mas recetas.\n");
    }
}

void listarRecetas(tReceta *lista, cadena nombresIng[], int cantRecetas) {
    printf("\n=======================================================\n");
    printf("              LISTADO GENERAL DE RECETAS               \n");
    printf("=======================================================\n");

    int activas = 0;
    int i = 0;

    while (i < cantRecetas) {
        if (lista[i].estadoActivo) {
            activas++;
            printf("ID: %d | Receta: %s\n", lista[i].id, lista[i].nombre);
            printf("Tiempo Prep: %d min | Porciones: %d\n", lista[i].detalle.tiempoPrepMin, lista[i].detalle.porciones);
            printf("Ingredientes:\n");

            int j = 0;
            while (j < MAX_INGREDIENTES) {
                if (lista[i].ingredientes[j] > 0) {
                    printf("  - %s: %.2f\n", nombresIng[j], lista[i].ingredientes[j]);
                }
                j++;
            }
            printf("-------------------------------------------------------\n");
        }
        i++;
    }

    if (activas == 0) {
        printf("No hay recetas activas registradas.\n");
    }
}

void buscarRecetaPorId(tReceta *lista, cadena nombresIng[], int cantRecetas) {
    printf("\n--- BUSCAR RECETA POR ID ---\n");
    int idBuscado = leerEntero("Ingrese el ID de la receta");
    int pos = buscarPosicionPorId(lista, cantRecetas, idBuscado);

    if (pos != -1) {
        printf("\nRECETA ENCONTRADA:\n");
        printf("ID: %d | Nombre: %s\n", lista[pos].id, lista[pos].nombre);
        printf("Tiempo: %d min | Porciones: %d\n", lista[pos].detalle.tiempoPrepMin, lista[pos].detalle.porciones);
        printf("Ingredientes:\n");

        int j = 0;
        while (j < MAX_INGREDIENTES) {
            printf("  - %s: %.2f\n", nombresIng[j], lista[pos].ingredientes[j]);
            j++;
        }
    } else {
        printf("No existe una receta activa con el ID %d.\n", idBuscado);
    }
}

void modificarReceta(tReceta *lista, cadena nombresIng[], int cantRecetas) {
    printf("\n--- MODIFICAR RECETA ---\n");
    int idBuscado = leerEntero("Ingrese el ID de la receta a modificar");
    int pos = buscarPosicionPorId(lista, cantRecetas, idBuscado);

    if (pos != -1) {
        printf("Modificando receta: %s (ID: %d)\n", lista[pos].nombre, lista[pos].id);

        if (confirmaUsuario("¿Desea cambiar el nombre?")) {
            leerTexto(lista[pos].nombre, "Ingrese nuevo nombre");
        }

        if (confirmaUsuario("¿Desea cambiar el tiempo de preparacion?")) {
            lista[pos].detalle.tiempoPrepMin = leerEnteroEntre(1, 1440, "Ingrese nuevo tiempo (minutos)");
        }

        if (confirmaUsuario("¿Desea cambiar la cantidad de porciones?")) {
            lista[pos].detalle.porciones = leerEnteroEntre(1, 500, "Ingrese nuevas porciones");
        }

        if (confirmaUsuario("¿Desea actualizar las cantidades de ingredientes?")) {
            int j = 0;
            while (j < MAX_INGREDIENTES) {
                printf("Nueva cantidad para '%s': ", nombresIng[j]);
                lista[pos].ingredientes[j] = leerFloatEntre(0.0f, 10000.0f, "Ingrese cantidad");
                j++;
            }
        }

        printf("\nSUCCESS: ¡Receta modificada correctamente!\n");
    } else {
        printf("No existe una receta activa con el ID %d.\n", idBuscado);
    }
}

void bajaReceta(tReceta *lista, int cantRecetas) {
    printf("\n--- BAJA DE RECETA ---\n");
    int idBuscado = leerEntero("Ingrese el ID de la receta a eliminar");
    int pos = buscarPosicionPorId(lista, cantRecetas, idBuscado);

    if (pos != -1) {
        printf("Receta: %s (ID: %d)\n", lista[pos].nombre, lista[pos].id);
        if (confirmaUsuario("¿Esta seguro de dar de baja esta receta?")) {
            lista[pos].estadoActivo = false;
            printf("\nSUCCESS: Receta dada de baja exitosamente.\n");
        } else {
            printf("Operacion cancelada.\n");
        }
    } else {
        printf("No existe una receta activa con el ID %d.\n", idBuscado);
    }
}

void ordenarRecetasPorTiempo(tReceta *lista, int cantRecetas) {
    printf("\n--- ORDENANDO RECETAS POR TIEMPO DE PREPARACION ---\n");

    int i = 0;
    int j = 0;
    tReceta aux;

    while (i < cantRecetas - 1) {
        j = 0;
        while (j < cantRecetas - i - 1) {
            if (lista[j].detalle.tiempoPrepMin > lista[j + 1].detalle.tiempoPrepMin) {
                aux = lista[j];
                lista[j] = lista[j + 1];
                lista[j + 1] = aux;
            }
            j++;
        }
        i++;
    }

    printf("SUCCESS: ¡Recetas ordenadas exitosamente por tiempo!\n");
}

// =========================================================================
// FUNCIONES DE PERSISTENCIA Y REPORTES (HITO FINAL)
// =========================================================================

void guardarEnArchivoBinario(tReceta *lista, int cantRecetas) {
    FILE *archivo = fopen("recetas.dat", "wb");
    if (archivo != NULL) {
        // Guardamos primero la cantidad total de recetas válidas
        fwrite(&cantRecetas, sizeof(int), 1, archivo);
        if (cantRecetas > 0) {
            // Guardamos el bloque completo del array en binario
            fwrite(lista, sizeof(tReceta), cantRecetas, archivo);
        }
        fclose(archivo); // Cierre seguro del canal
    } else {
        printf("ERROR CRITICO: No se pudo abrir el archivo binario para guardado.\n");
    }
}

int cargarDesdeArchivoBinario(tReceta *lista, int maxRecetas) {
    FILE *archivo = fopen("recetas.dat", "rb");
    int cantRecetas = 0;
    if (archivo != NULL) {
        size_t leidos = fread(&cantRecetas, sizeof(int), 1, archivo);
        if (leidos > 0 && cantRecetas > 0 && cantRecetas <= maxRecetas) {
            fread(lista, sizeof(tReceta), cantRecetas, archivo);
        } else {
            cantRecetas = 0;
        }
        fclose(archivo); // Cierre seguro
    } else {
        // Si el archivo no existe (primera ejecución), arranca en 0 de forma limpia
        cantRecetas = 0;
    }
    return cantRecetas;
}

void exportarReporteTexto(tReceta *lista, cadena nombresIng[], int cantRecetas) {
    FILE *archivo = fopen("reporte_recetas.txt", "w");
    if (archivo != NULL) {
        fprintf(archivo, "=======================================================\n");
        fprintf(archivo, "       REPORTE DE AUDITORIA - RECETARIO CATERING       \n");
        fprintf(archivo, "=======================================================\n\n");
        
        int i = 0;
        int activas = 0;
        while (i < cantRecetas) {
            if (lista[i].estadoActivo) {
                activas++;
                fprintf(archivo, "Receta Activa #%d\n", activas);
                fprintf(archivo, "  - ID: %d\n", lista[i].id);
                fprintf(archivo, "  - Nombre: %s\n", lista[i].nombre);
                fprintf(archivo, "  - Tiempo de Preparacion: %d min\n", lista[i].detalle.tiempoPrepMin);
                fprintf(archivo, "  - Porciones: %d\n", lista[i].detalle.porciones);
                fprintf(archivo, "  - Ingredientes:\n");
                
                int j = 0;
                while (j < MAX_INGREDIENTES) {
                    if (lista[i].ingredientes[j] > 0) {
                        fprintf(archivo, "    * %s: %.2f\n", nombresIng[j], lista[i].ingredientes[j]);
                    }
                    j++;
                }
                fprintf(archivo, "-------------------------------------------------------\n");
            }
            i++;
        }
        fprintf(archivo, "\nTotal de registros activos en auditoria: %d\n", activas);
        fclose(archivo); // Cierre seguro del archivo de texto
        printf("\nSUCCESS: ¡Reporte exportado exitosamente a 'reporte_recetas.txt'!\n");
    } else {
        printf("ERROR: No se pudo crear el archivo de texto para el reporte.\n");
    }
}