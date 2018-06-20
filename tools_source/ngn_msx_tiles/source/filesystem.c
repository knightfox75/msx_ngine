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
#include "filesystem.h"



/*
-------------------------------------------------------------------------------
    Graba los datos a los archivos (Normal)
-------------------------------------------------------------------------------
*/

s32 SaveFileNormal (const char* filename) {

    // Variables
    u32 b = 0;
    u32 tbl = 0;
    u32 l = 0;
    u32 i = 0;
    u8 hi = 0, lo = 0;
    char fnam[256];
    char output[256];
    char txt[32];
    char name[256];
    u32 n = 0;
    char lt;
    u32 original = 0;

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
        if (img.sz) original = 2;
        for (n = 0; n < 3; n ++) {
            original += (img.patterns_size_normal[n] + img.colors_size_normal[n] + img.names_size_normal[n]);
        }
        sprintf(output, ";************************************************************\n");
        fwrite(output, strlen(output), 1, file);
        sprintf(output, ";*    Imagen convertida para MSX - SCREEN 2\n");
        fwrite(output, strlen(output), 1, file);
        sprintf(output, ";*    Archivo de origen: %s\n", filename);
        fwrite(output, strlen(output), 1, file);
        sprintf(output, ";*    Tamaño: %03dx%03d pixeles\n", img.width, img.height);
        fwrite(output, strlen(output), 1, file);
        sprintf(output, ";*    Total de datos: %d bytes\n", original);
        fwrite(output, strlen(output), 1, file);
        sprintf(output, ";************************************************************\n\n\n");
        fwrite(output, strlen(output), 1, file);

        // Etiqueta general
        sprintf(output, "%s_IMAGE:\n\n", name);
        fwrite(output, strlen(output), 1, file);

        // Informacion de los CHR
        sprintf(output, "; Datos CHR\n");
        fwrite(output, strlen(output), 1, file);
        for (b = 0; b < 3; b ++) {
            sprintf(output, "; Banco %01d: %03d bloques de CHR (%d bytes) [$%04x bytes]\n", b, img.tiles[b], img.patterns_size_normal[b], img.patterns_size_normal[b]);
            fwrite(output, strlen(output), 1, file);
        }
        sprintf(output, "\n");
        fwrite(output, strlen(output), 1, file);

        // Genera los patterns
        printf("Grabando datos CHR ");
        for (b = 0; b < 3; b ++) {
            tbl = 0;
            // Genera la etiqueta
            sprintf(output, "%s_CHR_%01d:\n", name, b);
            fwrite(output, strlen(output), 1, file);
            if (img.sz) {
                // Genera los bytes de tamaño
                sprintf(output, "; Tamaño:\n");
                fwrite(output, strlen(output), 1, file);
                hi = (img.patterns_size_normal[b] >> 8) & 0xff;
                lo = (img.patterns_size_normal[b] & 0xff);
                sprintf(output, "db $%02x, $%02x\n", hi, lo);
                fwrite(output, strlen(output), 1, file);
                sprintf(output, "; Datos:\n");
                fwrite(output, strlen(output), 1, file);
            }
            for (n = 0; n < img.tiles[b]; n ++) {
                // Tiles
                sprintf(output, "db ");
                // Lineas
                for (l = 0; l < 8; l ++) {      // Linea
                    if (l != 7) {
                        sprintf(txt, "$%02x, ", img.patterns[b][tbl]);
                    } else {
                        sprintf(txt, "$%02x\n", img.patterns[b][tbl]);
                    }
                    strcat(output, txt);
                    tbl ++;
                }
                // Guarda la info
                printf(".");
                fwrite(output, strlen(output), 1, file);
            }
            // Genera un espacio entre bancos
            sprintf(output, "\n");
            fwrite(output, strlen(output), 1, file);
        }
        // Genera un espacio entre bancos
        sprintf(output, "\n");
        fwrite(output, strlen(output), 1, file);
        printf(" ok\n\n");


        // Informacion de los CLR
        sprintf(output, "; Datos CLR\n");
        fwrite(output, strlen(output), 1, file);
        for (b = 0; b < 3; b ++) {
            sprintf(output, "; Banco %01d: %03d bloques de CLR (%d bytes) [$%04x bytes]\n", b, img.tiles[b], img.colors_size_normal[b], img.colors_size_normal[b]);
            fwrite(output, strlen(output), 1, file);
        }
        sprintf(output, "\n");
        fwrite(output, strlen(output), 1, file);


        // Genera los colores
        printf("Grabando datos CLR ");
        for (b = 0; b < 3; b ++) {
            tbl = 0;
            // Genera la etiqueta
            sprintf(output, "%s_CLR_%01d:\n", name, b);
            fwrite(output, strlen(output), 1, file);
            if (img.sz) {
                // Genera los bytes de tamaño
                sprintf(output, "; Tamaño:\n");
                fwrite(output, strlen(output), 1, file);
                hi = (img.colors_size_normal[b] >> 8) & 0xff;
                lo = (img.colors_size_normal[b] & 0xff);
                sprintf(output, "db $%02x, $%02x\n", hi, lo);
                fwrite(output, strlen(output), 1, file);
                sprintf(output, "; Datos:\n");
                fwrite(output, strlen(output), 1, file);
            }
            for (n = 0; n < img.tiles[b]; n ++) {
                // Tiles
                sprintf(output, "db ");
                // Lineas
                for (l = 0; l < 8; l ++) {      // Linea
                    if (l != 7) {
                        sprintf(txt, "$%02x, ", img.colors[b][tbl]);
                    } else {
                        sprintf(txt, "$%02x\n", img.colors[b][tbl]);
                    }
                    strcat(output, txt);
                    tbl ++;
                }
                // Guarda la info
                printf(".");
                fwrite(output, strlen(output), 1, file);
            }
            // Genera un espacio entre bancos
            sprintf(output, "\n");
            fwrite(output, strlen(output), 1, file);
        }
        // Genera un espacio entre bancos
        sprintf(output, "\n");
        fwrite(output, strlen(output), 1, file);
        printf(" ok\n\n");


        // Informacion de los NAMES
        sprintf(output, "; Datos NAME\n");
        fwrite(output, strlen(output), 1, file);
        for (b = 0; b < 3; b ++) {
            sprintf(output, "; Banco %01d: (%d bytes) [$%04x bytes]\n", b, img.names_size_normal[b], img.names_size_normal[b]);
            fwrite(output, strlen(output), 1, file);
        }
        sprintf(output, "\n");
        fwrite(output, strlen(output), 1, file);

        // Genera la tabla de nombres
        printf("Grabando datos NAME ");
        for (b = 0; b < 3; b ++) {
            // Genera la etiqueta
            sprintf(output, "%s_NAM_%01d:\n", name, b);
            fwrite(output, strlen(output), 1, file);
            if (img.sz) {
                // Genera los bytes de tamaño
                sprintf(output, "; Tamaño:\n");
                fwrite(output, strlen(output), 1, file);
                hi = (img.names_size_normal[b] >> 8) & 0xff;
                lo = (img.names_size_normal[b] & 0xff);
                sprintf(output, "db $%02x, $%02x\n", hi, lo);
                fwrite(output, strlen(output), 1, file);
                sprintf(output, "; Datos:\n");
                fwrite(output, strlen(output), 1, file);
            }
            l = 0; i = 0;
            sprintf(output, "db ");
            for (n = 0; n < img.map_pos[b]; n ++) {
                // Lineas
                if (i != 31) {
                    sprintf(txt, "$%02x, ", img.names[b][n]);
                } else {
                    sprintf(txt, "$%02x\n", img.names[b][n]);
                }
                strcat(output, txt);
                // Ajuste de indice
                i ++;
                if (i > 31) {
                    i = 0;
                    // Guarda la info de la linea
                    printf(".");
                    fwrite(output, strlen(output), 1, file);
                    sprintf(output, "db ");
                }
            }
            // Genera un espacio entre bancos
            if (b < 2) {
                sprintf(output, "\n");
                fwrite(output, strlen(output), 1, file);
            }
        }
        // Msg
        printf(" ok\n\n");

    } else {

        printf("No se puede generar el archivo.\n");

        return -1;

    }

    // Cierra el archivo
    fclose(file);

    printf("\n");

    return 0;

}





/*
-------------------------------------------------------------------------------
    Graba los datos a los archivos (RLE)
-------------------------------------------------------------------------------
*/

s32 SaveFileRLE (const char* filename) {

   // Variables
    u32 b = 0;
    u32 idx = 0;
    u32 l = 0;
    u8 hi = 0, lo = 0;
    char fnam[256];
    char output[256];
    char txt[32];
    char name[256];
    u32 n = 0;
    char lt;
    u32 compressed = 0;

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
        if (img.sz) compressed = 4;
        for (n = 0; n < 3; n ++) {
            compressed += (img.patterns_size_rle[n] + img.colors_size_rle[n] + img.names_size_rle[n]);
        }
        sprintf(output, ";************************************************************\n");
        fwrite(output, strlen(output), 1, file);
        sprintf(output, ";*    Imagen convertida para MSX - SCREEN 2\n");
        fwrite(output, strlen(output), 1, file);
        sprintf(output, ";*    Archivo de origen: %s\n", filename);
        fwrite(output, strlen(output), 1, file);
        sprintf(output, ";*    Tamaño: %03dx%03d pixeles\n", img.width, img.height);
        fwrite(output, strlen(output), 1, file);
        sprintf(output, ";*    Compresion RLE habilitada\n");
        fwrite(output, strlen(output), 1, file);
        sprintf(output, ";*    Total de datos: %d bytes\n", compressed);
        fwrite(output, strlen(output), 1, file);
        sprintf(output, ";************************************************************\n\n\n");
        fwrite(output, strlen(output), 1, file);

        // Etiqueta general
        sprintf(output, "%s_IMAGE:\n\n", name);
        fwrite(output, strlen(output), 1, file);

        // Informacion de los CHR
        sprintf(output, "; Datos CHR\n");
        fwrite(output, strlen(output), 1, file);
        for (b = 0; b < 3; b ++) {
            sprintf(output, "; Banco %01d: %03d bloques de CHR\n", b, img.tiles[b]);
            fwrite(output, strlen(output), 1, file);
            sprintf(output, ";          Tamaño descomprimido: %d bytes [$%04x bytes]\n", img.patterns_size_normal[b], img.patterns_size_normal[b]);
            fwrite(output, strlen(output), 1, file);
            sprintf(output, ";          Tamaño RLE: %d bytes [$%04x bytes]\n", img.patterns_size_rle[b], img.patterns_size_rle[b]);
            fwrite(output, strlen(output), 1, file);
        }
        sprintf(output, "\n");
        fwrite(output, strlen(output), 1, file);

        // Genera los patterns
        printf("Grabando datos CHR ");
        for (b = 0; b < 3; b ++) {
            // Incializa el indice
            idx = 0;
            l = 0;
            // Genera la etiqueta
            sprintf(output, "%s_CHR_%01d:\n",name, b);
            fwrite(output, strlen(output), 1, file);
            if (img.sz) {
                // Genera los bytes de tamaño
                sprintf(output, "; Tamaño descomprimido:\n");
                fwrite(output, strlen(output), 1, file);
                hi = (img.patterns_size_normal[b] >> 8) & 0xff;
                lo = (img.patterns_size_normal[b] & 0xff);
                sprintf(output, "db $%02x, $%02x\n", hi, lo);
                fwrite(output, strlen(output), 1, file);
                sprintf(output, "; Tamaño comprimido (RLE):\n");
                fwrite(output, strlen(output), 1, file);
                hi = (img.patterns_size_rle[b] >> 8) & 0xff;
                lo = (img.patterns_size_rle[b] & 0xff);
                sprintf(output, "db $%02x, $%02x\n", hi, lo);
                fwrite(output, strlen(output), 1, file);
                sprintf(output, "; Datos:\n");
                fwrite(output, strlen(output), 1, file);
            }
            while (idx < img.patterns_size_rle[b]) {
                // Cabecera de la linea
                if (l == 0) {
                    sprintf(output, "db ");
                }
                l ++;
                // Mientras no llegues al final de linea...
                if ((l < 8) && (idx < (img.patterns_size_rle[b] - 1))) {
                    sprintf(txt, "$%02x, ", img.patterns[b][idx]);
                    strcat(output, txt);
                } else {
                    // Reinicia el contador
                    l = 0;
                    // Genera la linea
                    sprintf(txt, "$%02x\n", img.patterns[b][idx]);
                    strcat(output, txt);
                    // Guarda la info
                    printf(".");
                    fwrite(output, strlen(output), 1, file);
                }
                idx ++;
            }
            // Genera un espacio entre bancos
            sprintf(output, "\n");
            fwrite(output, strlen(output), 1, file);
        }
        // Genera un espacio entre bancos
        sprintf(output, "\n");
        fwrite(output, strlen(output), 1, file);
        printf(" ok\n\n");



         // Informacion de los CLR
        sprintf(output, "; Datos CLR\n");
        fwrite(output, strlen(output), 1, file);
        for (b = 0; b < 3; b ++) {
            sprintf(output, "; Banco %01d: %03d bloques de CLR\n", b, img.tiles[b]);
            fwrite(output, strlen(output), 1, file);
            sprintf(output, ";          Tamaño descomprimido: %d bytes [$%04x bytes]\n", img.colors_size_normal[b], img.colors_size_normal[b]);
            fwrite(output, strlen(output), 1, file);
            sprintf(output, ";          Tamaño RLE: %d bytes [$%04x bytes]\n", img.colors_size_rle[b], img.colors_size_rle[b]);
            fwrite(output, strlen(output), 1, file);
        }
        sprintf(output, "\n");
        fwrite(output, strlen(output), 1, file);

        // Genera los colors
        printf("Grabando datos CLR ");
        for (b = 0; b < 3; b ++) {
            // Incializa el indice
            idx = 0;
            l = 0;
            // Genera la etiqueta
            sprintf(output, "%s_CLR_%01d:\n",name, b);
            fwrite(output, strlen(output), 1, file);
            if (img.sz) {
                // Genera los bytes de tamaño
                sprintf(output, "; Tamaño descomprimido:\n");
                fwrite(output, strlen(output), 1, file);
                hi = (img.colors_size_normal[b] >> 8) & 0xff;
                lo = (img.colors_size_normal[b] & 0xff);
                sprintf(output, "db $%02x, $%02x\n", hi, lo);
                fwrite(output, strlen(output), 1, file);
                sprintf(output, "; Tamaño comprimido (RLE):\n");
                fwrite(output, strlen(output), 1, file);
                hi = (img.colors_size_rle[b] >> 8) & 0xff;
                lo = (img.colors_size_rle[b] & 0xff);
                sprintf(output, "db $%02x, $%02x\n", hi, lo);
                fwrite(output, strlen(output), 1, file);
                sprintf(output, "; Datos:\n");
                fwrite(output, strlen(output), 1, file);
            }
            while (idx < img.colors_size_rle[b]) {
                // Cabecera de la linea
                if (l == 0) {
                    sprintf(output, "db ");
                }
                l ++;
                // Mientras no llegues al final de linea...
                if ((l < 8) && (idx < (img.colors_size_rle[b] - 1))) {
                    sprintf(txt, "$%02x, ", img.colors[b][idx]);
                    strcat(output, txt);
                } else {
                    // Reinicia el contador
                    l = 0;
                    // Genera la linea
                    sprintf(txt, "$%02x\n", img.colors[b][idx]);
                    strcat(output, txt);
                    // Guarda la info
                    printf(".");
                    fwrite(output, strlen(output), 1, file);
                }
                idx ++;
            }
            // Genera un espacio entre bancos
            sprintf(output, "\n");
            fwrite(output, strlen(output), 1, file);
        }
        // Genera un espacio entre bancos
        sprintf(output, "\n");
        fwrite(output, strlen(output), 1, file);
        printf(" ok\n\n");



         // Informacion de los NAME
        sprintf(output, "; Datos NAME\n");
        fwrite(output, strlen(output), 1, file);
        for (b = 0; b < 3; b ++) {
            sprintf(output, "; Banco %01d: 256 bloques de NAME\n", b);
            fwrite(output, strlen(output), 1, file);
            sprintf(output, ";          Tamaño descomprimido: %d bytes [$%04x bytes]\n", img.names_size_normal[b], img.names_size_normal[b]);
            fwrite(output, strlen(output), 1, file);
            sprintf(output, ";          Tamaño RLE: %d bytes [$%04x bytes]\n", img.names_size_rle[b], img.names_size_rle[b]);
            fwrite(output, strlen(output), 1, file);
        }
        sprintf(output, "\n");
        fwrite(output, strlen(output), 1, file);

        // Genera los NAMES
        printf("Grabando datos NAME ");
        for (b = 0; b < 3; b ++) {
            // Incializa el indice
            idx = 0;
            l = 0;
            // Genera la etiqueta
            sprintf(output, "%s_NAM_%01d:\n",name, b);
            fwrite(output, strlen(output), 1, file);
            if (img.sz) {
                // Genera los bytes de tamaño
                sprintf(output, "; Tamaño descomprimido:\n");
                fwrite(output, strlen(output), 1, file);
                hi = (img.names_size_normal[b] >> 8) & 0xff;
                lo = (img.names_size_normal[b] & 0xff);
                sprintf(output, "db $%02x, $%02x\n", hi, lo);
                fwrite(output, strlen(output), 1, file);
                sprintf(output, "; Tamaño comprimido (RLE):\n");
                fwrite(output, strlen(output), 1, file);
                hi = (img.names_size_rle[b] >> 8) & 0xff;
                lo = (img.names_size_rle[b] & 0xff);
                sprintf(output, "db $%02x, $%02x\n", hi, lo);
                fwrite(output, strlen(output), 1, file);
                sprintf(output, "; Datos:\n");
                fwrite(output, strlen(output), 1, file);
            }
            while (idx < img.names_size_rle[b]) {
                // Cabecera de la linea
                if (l == 0) {
                    sprintf(output, "db ");
                }
                l ++;
                // Mientras no llegues al final de linea...
                if ((l < 8) && (idx < (img.names_size_rle[b] - 1))) {
                    sprintf(txt, "$%02x, ", img.names[b][idx]);
                    strcat(output, txt);
                } else {
                    // Reinicia el contador
                    l = 0;
                    // Genera la linea
                    sprintf(txt, "$%02x\n", img.names[b][idx]);
                    strcat(output, txt);
                    // Guarda la info
                    printf(".");
                    fwrite(output, strlen(output), 1, file);
                }
                idx ++;
            }
            // Genera un espacio entre bancos
            if (b < 2) {
                sprintf(output, "\n");
                fwrite(output, strlen(output), 1, file);
            }
        }
        // Msg
        printf(" ok\n\n");

    } else {

        printf("No se puede generar el archivo.\n");

        return -1;

    }

    // Cierra el archivo
    fclose(file);

    printf("\n");

    return 0;

}
