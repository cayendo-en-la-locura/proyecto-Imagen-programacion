#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct // IGUAL-MAIN
{
    int r, g, b, x, y;
} RGB;

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

void obtenerDimensionesRGB(const char *nombre_archivo, int *filas, int *columnas) // IGUAL-MAIN
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

RGB **leerDatosRGB(const char *imagenTXT, int filas, int columnas) // IGUAL-MAIN
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

    // for (int i = 0; i < filas; i++)
    // { // Leer los valores usando fscanf con el formato de tupla (R,G,B)
    //     for (int j = 0; j < columnas; j++)
    //     {
    //         printf("(%d,%d,%d) ", imagenRGB[i][j].r, imagenRGB[i][j].g, imagenRGB[i][j].b);
    //     }
    //     printf("\n");
    // }

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

    // for (int i = 0; i < numPixeles; i++)
    // {
    //     printf("(%d,%d)", arrASCII[i].x, arrASCII[i].y);
    // }

    fclose(archivo);
    return arrASCII;
}

int main()
{
    int numPixeles, filas, columnas;                                             // IGUAL-MAIN (FILAS, COLUMNAS)
    const char *coordenasTXT = "coordenadas.txt", *imagenTXT = "imagen_rgb.txt"; // IGUAL-MAIN (IMAGEN)

    obtenerDimensionesXY(coordenasTXT, &numPixeles);
    printf("número de tuplas: %d", numPixeles);
    //----------------------------------------------------------------------------------

    obtenerDimensionesRGB(imagenTXT, &filas, &columnas);

    if (filas > 0 && columnas > 0)
    {
        printf("\n\nDimenssiones de la imagen a modificar:\n");
        printf("Filas: %d\n", filas);
        printf("Columnas: %d\n", columnas);
    }
    else
    {
        printf("No se pudieron determinar las dimensiones del archivo.\n");
    }
    //----------------------------------------------------------------------------------

    RGB **matrizImagenRGB = leerDatosRGB(imagenTXT, filas, columnas);
    RGB *arrASCIImensaje = leerDatosXY(coordenasTXT, numPixeles);

    for (int i = 0; i < numPixeles; i++)
    {
        if ((arrASCIImensaje[i].y >= 0 && arrASCIImensaje[i].y < filas) && (arrASCIImensaje[i].x >= 0 && arrASCIImensaje[i].x < columnas))
        {
            printf("\nEl pixel en la coordenada (%d, %d) es: RGB(%d, %d, %d)", arrASCIImensaje[i].x, arrASCIImensaje[i].y, matrizImagenRGB[arrASCIImensaje[i].x][arrASCIImensaje[i].y].r, matrizImagenRGB[arrASCIImensaje[i].x][arrASCIImensaje[i].y].g, matrizImagenRGB[arrASCIImensaje[i].x][arrASCIImensaje[i].y].b);
            arrASCIImensaje[i].r = matrizImagenRGB[arrASCIImensaje[i].x][arrASCIImensaje[i].y].r;
            arrASCIImensaje[i].g = matrizImagenRGB[arrASCIImensaje[i].x][arrASCIImensaje[i].y].g;
            arrASCIImensaje[i].b = matrizImagenRGB[arrASCIImensaje[i].x][arrASCIImensaje[i].y].b;
            printf("\nEl pixel guardado en la posición %d arrASCIImensaje es: RGB(%d, %d, %d)\n", i, arrASCIImensaje[i].r, arrASCIImensaje[i].g, arrASCIImensaje[i].b);
        }
        else
        {
            // printf("\nCoordenadas fuera del rango %d*%d.\n", maxAleatorio, maxAleatorio);
        }
    }
    char mensaje[243]; // IGUAL-MAIN
    int c, k = 0;      // IGUAL-MAIN (C)
    for (int i = 0; i < numPixeles; i++)
    {
        // printf("%c = %03d\n", arrASCIImensaje[i].r, arrASCIImensaje[i].r); // correspondencia de ASCII con mensaje
        // printf("%c = %03d\n", arrASCIImensaje[i].g, arrASCIImensaje[i].g); // correspondencia de ASCII con mensaje
        // printf("%c = %03d\n", arrASCIImensaje[i].b, arrASCIImensaje[i].b); // correspondencia de ASCII con mensaje
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

    return 0;
}
