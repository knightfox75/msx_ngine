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
#include "filesystem.h"



/*
-------------------------------------------------------------------------------
    Graba los datos a los archivos (Normal)
-------------------------------------------------------------------------------
*/

s32 SaveFile (const char* filename) {

    // Variables
    u32 l = 0;
    u8 hi = 0, lo = 0;
    char fnam[256];
    char output[256];
    char txt[32];
    char name[256];
    u32 n = 0;
    char lt;
    u32 filesize = 0;
    u32 tiles_num = 0;
    u32 tiles_temp = 4;

    // Genera un nombre base para el archivo
    for (n = 0; n < (strlen(filename) - 4); n ++) {
        lt = filename[n];
        if (lt >= 65 && lt <= 90) lt += 32;
        name[n] = lt;
    }
    name[n] = '\0';

    // Fase 3, genera el archivo codificado
    if (img.inc) {
        sprintf(fnam, "%s.inc", name);
    } else {
        sprintf(fnam, "%s.asm", name);
    }
    FILE* file = fopen(fnam, "wb");

    // Genera un nombre base para las etiquetas
    for (n = 0; n < (strlen(filename) - 4); n ++) {
        lt = filename[n];
        if (lt >= 97 && lt <= 122) lt -= 32;
        name[n] = lt;
    }
    name[n] = '\0';

    // Si se puede crear el archivo...
    if (file != NULL) {

        // Cabecera del archivo
        if (img.sz) filesize = 2;
        filesize += img.tiles_size_normal;
        sprintf(output, ";************************************************************\n");
        fwrite(output, strlen(output), 1, file);
        sprintf(output, ";*    Imagen convertida para MSX - SCREEN 2\n");
        fwrite(output, strlen(output), 1, file);
        sprintf(output, ";*    Archivo de origen: %s\n", filename);
        fwrite(output, strlen(output), 1, file);
        sprintf(output, ";*    Tamaño: %02dx%02d pixeles\n", img.format, img.format);
        fwrite(output, strlen(output), 1, file);
        sprintf(output, ";*    Nº de frames: %02d\n", img.frames);
        fwrite(output, strlen(output), 1, file);
        sprintf(output, ";*    Total de datos: %d bytes\n", filesize);
        fwrite(output, strlen(output), 1, file);
        sprintf(output, ";************************************************************\n\n\n");
        fwrite(output, strlen(output), 1, file);

        // Etiqueta general
        sprintf(output, "%s_SPRITE:\n\n", name);
        fwrite(output, strlen(output), 1, file);

        // Genera los CHR del sprite
        printf("Grabando datos del SPRITE ");


        if (img.sz) {
            // Genera los bytes de tamaño
            sprintf(output, "; Tamaño:\n");
            fwrite(output, strlen(output), 1, file);
            hi = (img.tiles_size_normal >> 8) & 0xff;
            lo = (img.tiles_size_normal & 0xff);
            sprintf(output, "db $%02x, $%02x\n", hi, lo);
            fwrite(output, strlen(output), 1, file);
        }

        for (n = 0; n < img.tiles_size_normal; n += 8) {
            // Contador de frames
            if (img.format == 8) {
                sprintf(output, "; CHR: %02d\n", tiles_num);
                fwrite(output, strlen(output), 1, file);
                tiles_num ++;
            } else {
                tiles_temp ++;
                if (tiles_temp >= 4) {
                    sprintf(output, "; CHR: %02d\n", (tiles_num * 4));
                    fwrite(output, strlen(output), 1, file);
                    tiles_num ++;
                    tiles_temp = 0;
                }
            }
            // Tiles
            sprintf(output, "db ");
            // Lineas
            for (l = 0; l < 8; l ++) {      // Linea
                if (l != 7) {
                    sprintf(txt, "$%02x, ", img.tiles[(n + l)]);
                } else {
                    sprintf(txt, "$%02x\n", img.tiles[(n + l)]);
                }
                strcat(output, txt);
            }
            // Guarda la info
            printf(".");
            fwrite(output, strlen(output), 1, file);
        }

        // Fin del archivo
        sprintf(output, "\n");
        fwrite(output, strlen(output), 1, file);
        printf(" ok\n\n");

    } else {

        printf("No se puede generar el archivo.\n");
        return -1;

    }

    // Cierra el archivo
    fclose(file);

    return 0;

}
