#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

char mensaje[243];
int filas, columnas;
const char *imagenTXT = "imagen_rgb.txt";
int numPixeles;
const char *coordenasTXT = "coordenadas.txt";

typedef struct
{
    int r, g, b, x, y;
} RGB;

/// funciones compartidas o únicas

int aleatorioHastaMax(int min, int max)
{
    return rand() % (max - min + 1) + min;
}

void obtenerDimensionesXY(const char *coordenasTXT, int *numPixeles)
{
    FILE *archivo = fopen(coordenasTXT, "r");
    if (archivo == NULL)
    {
        perror("Error al abrir el archivo");
        exit(EXIT_FAILURE);
    }

    *numPixeles = 0;
    char c;

    while ((c = fgetc(archivo)) != EOF)
    {
        if (c == '(')
        {
            (*numPixeles)++;
        }
    }

    fclose(archivo);
}

void obtenerDimensionesRGB(const char *nombre_archivo, int *filas, int *columnas)
{
    FILE *archivo = fopen(nombre_archivo, "r");
    if (archivo == NULL)
    {
        perror("Error al abrir el archivo");
        exit(EXIT_FAILURE);
    }

    *filas = 0;
    *columnas = 0;
    char c;
    int comienzoLinea = 1; // indicador de nueva linea
    int tempCols = 0;
    int contandorColumnas = 1; // solo contamos columnas en la primera línea (después cambia a 0)

    // Leer caracter por caracter para contar
    while ((c = fgetc(archivo)) != EOF)
    {
        if (comienzoLinea && c != ' ' && c != '\t')
        {
            (*filas)++;
            comienzoLinea = 0;
        }

        if (c == '(' && contandorColumnas)
        {
            tempCols++;
        }

        if (c == '\n')
        {
            comienzoLinea = 1;
            contandorColumnas = 0; // Ya contamos las columnas de la primera línea
        }
    }
    *columnas = tempCols;

    fclose(archivo);
}

RGB **leerDatosRGB(const char *imagenTXT, int filas, int columnas)
{
    FILE *archivo = fopen(imagenTXT, "r");
    if (archivo == NULL)
    {
        perror("Error al abrir el archivo");
        exit(EXIT_FAILURE);
    }

    RGB **imagenRGB = (RGB **)malloc(filas * sizeof(RGB *)); // matriz de RGB de imagen_rgb.txt
    if (imagenRGB == NULL)
    {
        perror("Error de asignación de memoria para filas");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < filas; i++)
    { // Asignar memoria para las columnas de cada fila
        imagenRGB[i] = (RGB *)malloc(columnas * sizeof(RGB));
        if (imagenRGB[i] == NULL) // liberar memoria si imagenRGB es nulo
        {
            perror("Error de asignación de memoria para columnas");
            for (int k = 0; k < i; k++)
                free(imagenRGB[k]);
            free(imagenRGB);
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < filas; i++)
    { // Leer los valores usando fscanf con el formato de tupla (R,G,B)
        for (int j = 0; j < columnas; j++)
        {
            if (fscanf(archivo, " (%d,%d,%d)", &imagenRGB[i][j].r, &imagenRGB[i][j].g, &imagenRGB[i][j].b) != 3)
            {
                fprintf(stderr, "Error al leer datos en la fila %d, columna %d\n", i, j);
                exit(EXIT_FAILURE);
            }
        }
    }

    fclose(archivo);
    return imagenRGB;
}

RGB *leerDatosXY(const char *coordenasTXT, int numPixeles)
{
    FILE *archivo = fopen(coordenasTXT, "r");
    if (archivo == NULL)
    {
        perror("Error al abrir el archivo");
        exit(EXIT_FAILURE);
    }

    RGB *arrASCII = (RGB *)malloc(numPixeles * sizeof(RGB)); // matriz de RGB de imagen_rgb.txt
    if (arrASCII == NULL)
    {
        perror("Error de asignación de memoria para el arreglo");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < numPixeles; i++)
    { // Leer los valores usando fscanf con el formato de tupla (x,y)
        if (fscanf(archivo, "(%d,%d) ", &arrASCII[i].x, &arrASCII[i].y) != 2)
        {
            fprintf(stderr, "Error al leer datos en la fila %d\n", i);
            exit(EXIT_FAILURE);
        }
    }

    fclose(archivo);
    return arrASCII;
}

void limpiarBuffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

enum opciones
{
    Ingresar = 1,
    Leer,
    Salir
};

void ingresarMensaje();

void leerMensaje();

int main()
{
    int opcion;
    char repetir;

    do
    {
        printf("--------MENU--------\n");
        printf("1. Ingresar un mensaje\n");
        printf("2. Leer mensaje\n");
        printf("3. Salir\n");
        printf("Seleccione una opción: ");
        scanf("%d", &opcion);
        limpiarBuffer();

        switch (opcion)
        {
        case Ingresar:
            ingresarMensaje();
            break;
        case Leer:
            leerMensaje();
            break;
        case Salir:
            printf("Saliendo del programa...");
            return 0;
        default:
            printf("Opcion invalida. Intente de nuevo.\n");
        }

        printf("¿Desea volver al menu? (s/n): ");
        scanf("%c", &repetir);
        limpiarBuffer();
    } while (repetir == 's' || repetir == 'S');

    printf("Programa finalizado.");
    return 0;
}

void ingresarMensaje()
{
    printf("ingreso\n");
    printf("Escribe tu mensaje (hasta 243 caracteres, sin acentos): ");

    fgets(mensaje, sizeof(mensaje), stdin);
    mensaje[strcspn(mensaje, "\n")] = '\0'; // sustituir salto de línea con valor nulo
    int longitudMensaje = strlen(mensaje);  // tamaño del mensaje

    for (int i = 0; i < longitudMensaje; i++)
    {
        printf("%c = %03d\n", mensaje[i], mensaje[i]); // correspondencia de mensaje con valor ASCII
    }

    int numeroPixeles = (longitudMensaje + 2) / 3;
    RGB *arrMensajeRGB = (RGB *)malloc(numeroPixeles * sizeof(RGB));
    if (arrMensajeRGB == NULL)
    {
        printf("Error de asignación de memoria");
        exit(EXIT_FAILURE);
    }

    int j = 0;
    for (int i = 0; i < longitudMensaje; i += 3)
    {
        arrMensajeRGB[j].r = mensaje[i];
        arrMensajeRGB[j].g = (i + 1 < longitudMensaje) ? mensaje[i + 1] : '\0';
        arrMensajeRGB[j].b = (i + 2 < longitudMensaje) ? mensaje[i + 2] : '\0';
        j++;
    }

    printf("\nValores ASCII como RGB:\n");
    for (int i = 0; i < numeroPixeles; i++)
    {
        printf("(%03d, %03d, %03d) ", arrMensajeRGB[i].r, arrMensajeRGB[i].g, arrMensajeRGB[i].b); // 000 001
    }

    obtenerDimensionesRGB(imagenTXT, &filas, &columnas);

    (filas > 0 && columnas > 0) ? printf("\n\nDimensiones de la imagen:\nFilas = %d, Columnas = %d\n", filas, columnas) : printf("No se pudieron determinar las dimensiones del archivo.\n");

    int maxAleatorio = (filas > columnas) ? columnas - 1 : filas - 1; // guarda el valor menor entre columa o fila en maxAleatorio. Coordenadas aleatorias dentro de un rango mm o nn.
    //--------------------------------------------------------------------------------
    srand(time(NULL));

    int *usados = calloc(filas * columnas, sizeof(int));
    if (!usados)
    {
        perror("Error asignando memoria");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < numeroPixeles; i++)
    {
        int x, y, index;
        do
        {
            x = aleatorioHastaMax(0, columnas - 1);
            y = aleatorioHastaMax(0, filas - 1);
            index = y * columnas + x;
        } while (usados[index] == 1);

        usados[index] = 1;
        arrMensajeRGB[i].x = x;
        arrMensajeRGB[i].y = y;
    }

    printf("\nCoordenadas generadas (únicas):\n");
    for (int i = 0; i < numeroPixeles; i++)
        printf("(%d, %d) ", arrMensajeRGB[i].x, arrMensajeRGB[i].y);
    printf("\n");
    // -------------------------------------------------------------------------

    RGB **matrizImagenRGB = leerDatosRGB(imagenTXT, filas, columnas);

    for (int i = 0; i < numeroPixeles; i++)
    {
        int x = arrMensajeRGB[i].x, y = arrMensajeRGB[i].y;
        printf("\nPixel original en (%d,%d): RGB(%d,%d,%d)", x, y, matrizImagenRGB[y][x].r, matrizImagenRGB[y][x].g, matrizImagenRGB[y][x].b);
        matrizImagenRGB[y][x].r = arrMensajeRGB[i].r;
        matrizImagenRGB[y][x].g = arrMensajeRGB[i].g;
        matrizImagenRGB[y][x].b = arrMensajeRGB[i].b;
        printf("\nNuevo pixel en (%d,%d): RGB(%d,%d,%d)\n", x, y, matrizImagenRGB[y][x].r, matrizImagenRGB[y][x].g, matrizImagenRGB[y][x].b);
    }
    //----------------------------------------------------------------------------------
    fclose(fopen("imagen_rgb.txt", "w")); // abre y cierra el documento, eliminando el contenido

    FILE *archivoImagen = fopen("imagen_rgb.txt", "w");
    if (archivoImagen == NULL)
    {
        perror("Error al abrir el archivo");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < filas; i++)
    {
        for (int j = 0; j < columnas; j++)
        {
            fprintf(archivoImagen, "(%d,%d,%d) ", matrizImagenRGB[i][j].r, matrizImagenRGB[i][j].g, matrizImagenRGB[i][j].b);
        }
        fprintf(archivoImagen, "\n");
    }

    fclose(archivoImagen);
    //----------------------------------------------------------------------------------

    FILE *archivoCoordenadas = fopen("coordenadas.txt", "w");
    if (archivoCoordenadas == NULL)
    {
        perror("Error al abrir el archivo");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < numeroPixeles; i++)
    {
        fprintf(archivoCoordenadas, "(%d,%d) ", arrMensajeRGB[i].x, arrMensajeRGB[i].y);
    }

    fclose(archivoCoordenadas);
    //----------------------------------------------------------------------------------
    free(usados);
    free(arrMensajeRGB);

    for (int i = 0; i < filas; i++)
        free(matrizImagenRGB[i]);
    free(matrizImagenRGB);
}

void leerMensaje()
{
    printf("leer\n");
    obtenerDimensionesXY(coordenasTXT, &numPixeles);
    //----------------------------------------------------------------------------------

    obtenerDimensionesRGB(imagenTXT, &filas, &columnas);

    (filas > 0 && columnas > 0) ? printf("\n\nDimensiones de la imagen:\nFilas = %d, Columnas = %d\n", filas, columnas) : printf("No se pudieron determinar las dimensiones del archivo.\n");

    //----------------------------------------------------------------------------------

    RGB **matrizImagenRGB2 = leerDatosRGB(imagenTXT, filas, columnas);
    RGB *arrASCIImensaje = leerDatosXY(coordenasTXT, numPixeles);

    for (int i = 0; i < numPixeles; i++)
    {
        int x = arrASCIImensaje[i].x, y = arrASCIImensaje[i].y;
        if ((y >= 0 && y < filas) && (x >= 0 && x < columnas))
        {
            printf("\nEl pixel en la coordenada (%d, %d) es: RGB(%d, %d, %d)", x, y, matrizImagenRGB2[y][x].r, matrizImagenRGB2[y][x].g, matrizImagenRGB2[y][x].b);
            arrASCIImensaje[i].r = matrizImagenRGB2[y][x].r;
            arrASCIImensaje[i].g = matrizImagenRGB2[y][x].g;
            arrASCIImensaje[i].b = matrizImagenRGB2[y][x].b;
            printf("\nEl pixel guardado en la posición %d arrASCIImensaje es: RGB(%d, %d, %d)\n", i, arrASCIImensaje[i].r, arrASCIImensaje[i].g, arrASCIImensaje[i].b);
        }
    }

    int c, k = 0;
    // mensaje[] ya fue declarado en la PARTE 1
    for (int i = 0; i < numPixeles; i++)
    {
        mensaje[k] = (char)arrASCIImensaje[i].r;
        mensaje[k + 1] = (char)arrASCIImensaje[i].g;
        mensaje[k + 2] = (char)arrASCIImensaje[i].b;
        k += 3;
    }
    printf("\nMensaje: ");
    for (int i = 0; i < (numPixeles * 3); i++)
    {
        printf("%c", mensaje[i]);
    }
    printf("\n");

    // liberar memoria usada en la parte de extracción
    for (int i = 0; i < filas; i++)
    {
        free(matrizImagenRGB2[i]);
    }
    free(matrizImagenRGB2);
    free(arrASCIImensaje);
}