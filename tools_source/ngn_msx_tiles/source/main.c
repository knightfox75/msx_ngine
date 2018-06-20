/*
-------------------------------------------------------------------------------
    MSX Image to VRAM converter
    Genera desde una imagen las tablas de PATTERN, COLOR & NAME
    para usarlas en el modo SCREEN 2.
    (cc)2015-2018 by Cesar Rincon "NightFox"
    http://www.nightfoxandco.com
    Version 0.1.0-a
-------------------------------------------------------------------------------
*/



/*
-------------------------------------------------------------------------------
    Includes
-------------------------------------------------------------------------------
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "defines.h"
#include "bmp.h"
#include "tilemap.h"
#include "filesystem.h"
#include "rle.h"





/*
-------------------------------------------------------------------------------
    Archivo MAIN
-------------------------------------------------------------------------------
*/

int main(int argc, char *argv[]) {

    // Variables
    u8 param = argc;
    bool rle = false;
    s32 n = 0;
    s32 original = 0;
    s32 compressed = 0;

    printf("\n\nConversor de imagenes BMP a formato SCREEN2 para MSX.\nVersion 0.1.0-a.\n(cc)2015-2018 por Cesar Rincon.\nhttp://www.nightfoxandco.com\n\n\n");

    // Inicializa las estructuras
    memset(&img, 0, sizeof(img));

    if (argc > 2) {
        for (n = 2; n < argc; n ++) {
            // Busca coincidencias en los parametros
            if (strcmp(argv[n], "-rle") == 0) {
                rle = true;
                param --;
            } else if (strcmp(argv[n], "-size") == 0) {
                img.sz = true;
                param --;
            } else if (strcmp(argv[n], "-inc") == 0) {
                img.inc = true;
                param --;
            }
        }
    }

    // Error en la linea de comandos
    if (param != 2) {
        printf("Uso: %s archivo.bmp [-rle][-size][-inc]\n\n", argv[0]);
        printf("     -rle   Comprime los datos en formato RLE\n");
        printf("     -size  Añade al archivo la informacion del tamaño de los datos\n");
        printf("     -inc   Genera el archivo con la extension .INC\n\n");
        CleanBuffers();
        return -1;
    }

    // Carga el archivo BITMAP
    if (LoadBitmap(argv[1]) == -1) {
        printf("Error al procesar el archivo BITMAP.\n");
        CleanBuffers();
        return -1;
    }

    printf("\nProcesando archivo BITMAP...\n\nNombre: %s\nTamaño: %dx%d pixeles.\n\n", argv[1], img.width, img.height);

    if (GenerateTileset() == -1) {
        printf("Error al procesar los datos.\n");
        CleanBuffers();
        return -1;
    }


    if (rle) {
        for (n = 0; n < 3; n ++) img.patterns_size_rle[n] = RleCompress(img.patterns[n], img.patterns_size_normal[n]);
        for (n = 0; n < 3; n ++) img.colors_size_rle[n] = RleCompress(img.colors[n], img.colors_size_normal[n]);
        for (n = 0; n < 3; n ++) img.names_size_rle[n] = RleCompress(img.names[n], img.names_size_normal[n]);
        if (SaveFileRLE(argv[1]) == -1) {
            printf("Error al grabar los archivos.\n");
            CleanBuffers();
            return -1;
        }
    } else {
        if (SaveFileNormal(argv[1]) == -1) {
            printf("Error al grabar los archivos.\n");
            CleanBuffers();
            return -1;
        }
    }


    // Sal del programa
    if (img.inc) {
        printf("Archivo .INC generado con exito.\n\n");
    } else {
        printf("Archivo .ASM generado con exito.\n\n");
    }


    for (n = 0; n < 3; n ++) {
        original += (img.patterns_size_normal[n] + img.colors_size_normal[n] + img.names_size_normal[n]);
        compressed += (img.patterns_size_rle[n] + img.colors_size_rle[n] + img.names_size_rle[n]);
    }
    if (rle) {
        printf("Compresion RLE realizada\n");
        printf("Tamaño original: %d bytes.\n", original);
        printf("Tamaño comprimido: %d bytes.\n", compressed);
        printf("Relacion de compresion: %d %%.\n\n\n", ((compressed * 100) / original));
    } else {
        printf("Tamaño de los datos: %d bytes.\n", original);
    }

    CleanBuffers();
    return 0;

}
