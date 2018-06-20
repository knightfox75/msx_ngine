/*
-------------------------------------------------------------------------------
    MSX Sprite to VRAM converter
    Genera desde una imagen las tablas de SPRITE.
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
#include "tiles.h"
#include "filesystem.h"




/*
-------------------------------------------------------------------------------
    Archivo MAIN
-------------------------------------------------------------------------------
*/

int main(int argc, char *argv[]) {

    // Variables
    u8 param = argc;
    s32 n = 0;

    // Mensage de Bienvenida
    printf("\n\nConversor de imagenes BMP a formato SPRITE para MSX.\nVersion 0.1.0-a.\n(cc)2015-2018 por Cesar Rincon.\nhttp://www.nightfoxandco.com\n\n\n");

    // Inicializa las estructuras
    memset(&img, 0, sizeof(img));

    if (argc > 2) {
        for (n = 2; n < argc; n ++) {
            // Busca coincidencias en los parametros
            if (strcmp(argv[n], "-size") == 0) {
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
    printf("Imagen de %dx%d pixeles cargada con exito.\n\n", img.width, img.height);

    // Procesa el archivo
    if (GenerateTiles() == -1) {
        printf("Error al procesar los datos.\n");
        CleanBuffers();
        return -1;
    }

    // Genera y guarda el fichero
    if (SaveFile(argv[1]) == -1) {
        printf("Error al grabar el archivo.\n");
        CleanBuffers();
        return -1;
    }
    if (img.inc) {
        printf("Archivo .INC generado con exito.\n\n");
    } else {
        printf("Archivo .ASM generado con exito.\n\n");
    }


    // Sal del programa
    return 0;

}
