#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

typedef struct
{
    int r, g, b, x, y;
} RGB;

int aleatorioHastaMax(int min, int max)
{
    return rand() % (max - min + 1) + min;
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

RGB **leerDatosRGB(const char *nombre_archivo, int filas, int columnas)
{
    FILE *archivo = fopen(nombre_archivo, "r");
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

int main()
{
    char mensaje[243];
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
        arrMensajeRGB[j].r = mensaje[i]; // R
        if (i + 1 < longitudMensaje)     // G
        {
            arrMensajeRGB[j].g = mensaje[i + 1];
        }
        else
        {
            arrMensajeRGB[j].g = '\0'; // nulo si no hay más
        }
        // arrMensajeRGB[j].g = (i + 1 < longitudMensaje) ? mensaje[i + 1] : '\0'; // si el mensaje es más grande que la posición G, copiar el ASCII a arrMensajeRGB[j].g, si no, poner '0'.

        if (i + 2 < longitudMensaje) // B      HOL A--
        {
            arrMensajeRGB[j].b = mensaje[i + 2];
        }
        else
        {
            arrMensajeRGB[j].b = '\0'; // nulo si no hay más
        }
        // arrMensajeRGB[j].b = (i + 2 < longitudMensaje) ? mensaje[i + 2] : '\0'; // si el mensaje es más grande que la posición G, copiar el ASCII a arrMensajeRGB[j].b, si no, poner '0'.

        j++;
    }

    printf("Valores ASCII como RGB:\n");
    for (int i = 0; i < numeroPixeles; i++)
    {
        printf("(%03d, %03d, %03d) ", arrMensajeRGB[i].r, arrMensajeRGB[i].g, arrMensajeRGB[i].b); // 000 001
    }

    //-------------------------------------------------------------------------------
    int filas, columnas;
    const char *imagenTXT = "imagen_rgb.txt";

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

    int maxAleatorio = (filas > columnas) ? columnas - 1 : filas - 1; // guarda el valor menor entre columa o fila en maxAleatorio. Coordenadas aleatorias dentro de un rango mm o nn.
    //--------------------------------------------------------------------------------
    srand(time(NULL)); // semilla

    printf("\nGenerando %d coordenadas aleatorias entre 0 y %d:", numeroPixeles, maxAleatorio);

    for (int i = 0; i < numeroPixeles; i++)
    {
        int numeroAleatorioX = aleatorioHastaMax(0, maxAleatorio);
        arrMensajeRGB[i].x = numeroAleatorioX;
    }
    for (int i = 0; i < numeroPixeles; i++)
    {
        int numeroAleatorioY = aleatorioHastaMax(0, maxAleatorio);
        arrMensajeRGB[i].y = numeroAleatorioY;
    }

    for (int i = 0; i < numeroPixeles; i++)
    {
        printf("(%d, %d)", arrMensajeRGB[i].x, arrMensajeRGB[i].y);
    }
    printf("\n");

    //----------------------------------------------------------------------------------
    RGB **matrizImagenRGB = leerDatosRGB(imagenTXT, filas, columnas);

    for (int i = 0; i < numeroPixeles; i++)
    {
        if ((arrMensajeRGB[i].y >= 0 && arrMensajeRGB[i].y < filas) && (arrMensajeRGB[i].x >= 0 && arrMensajeRGB[i].x < columnas))
        {
            // RGB pixel = matrizImagenRGB[arrMensajeRGB[i].x][arrMensajeRGB[i].y]; // va al pixel en la matriz segun la coordenada en arrMensajeRGB  pixel.r
            printf("\nEl pixel en la coordenada (%d, %d) es: RGB(%d, %d, %d)", arrMensajeRGB[i].x, arrMensajeRGB[i].y, matrizImagenRGB[arrMensajeRGB[i].x][arrMensajeRGB[i].y].r, matrizImagenRGB[arrMensajeRGB[i].x][arrMensajeRGB[i].y].g, matrizImagenRGB[arrMensajeRGB[i].x][arrMensajeRGB[i].y].b);
            matrizImagenRGB[arrMensajeRGB[i].x][arrMensajeRGB[i].y].r = arrMensajeRGB[i].r; //                         matrizImagenRGB[xy].r
            matrizImagenRGB[arrMensajeRGB[i].x][arrMensajeRGB[i].y].g = arrMensajeRGB[i].g;
            matrizImagenRGB[arrMensajeRGB[i].x][arrMensajeRGB[i].y].b = arrMensajeRGB[i].b;
            printf("\nEl NUEVO pixel en la coordenada (%d, %d) es: RGB(%d, %d, %d)\n", arrMensajeRGB[i].x, arrMensajeRGB[i].y, matrizImagenRGB[arrMensajeRGB[i].x][arrMensajeRGB[i].y].r, matrizImagenRGB[arrMensajeRGB[i].x][arrMensajeRGB[i].y].g, matrizImagenRGB[arrMensajeRGB[i].x][arrMensajeRGB[i].y].b);
        } // blur box
        else
        {
            printf("\nCoordenadas fuera del rango %d*%d.\n", maxAleatorio, maxAleatorio);
        }
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
    //---------------------------------------------------------------------------------- LIBERAR MEMEMORIA
    free(matrizImagenRGB);
    free(arrMensajeRGB); // liberear la memeoria de matriz y el arreglo

    return 0;
}

void ingresarmensaje()
{
}
