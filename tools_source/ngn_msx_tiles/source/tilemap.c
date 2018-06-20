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
#include "tilemap.h"


/*
-------------------------------------------------------------------------------
    Analiza los datos y genera el archivo tileset
-------------------------------------------------------------------------------
*/

s32 GenerateTileset(void) {

    // Variables
    s32 n = 0;

    // Buffers temporales para los tiles
    u8* tile_pattern = (u8*) calloc(64, sizeof(u8));
    u8* test_pattern = (u8*) calloc(64, sizeof(u8));
    // Verifica si se han creado todos los buffers
    if (
        (tile_pattern == NULL)
        ||
        (test_pattern == NULL)
        ) {
        printf("Error creando los buffers internos.\nError critico.\n");
        free(tile_pattern);
        tile_pattern = NULL;
        free(test_pattern);
        test_pattern = NULL;
        return -1;
    }

    // Crea los buffers para las tablas de pattern y color
    // (Asume que todos los tiles son unicos y haz el buffer el doble de grande para la compresion RLE)
    for (n = 0; n < 3; n ++) {
        img.raw[n] = (u8*) calloc(32768, sizeof(u8));           // Tiles diferentes de 8x8 x 256
        img.patterns[n] = (u8*) calloc(4096, sizeof(u8));      // 8 bytes por tile (8 filas de 8 pixeles)
        img.colors[n] = (u8*) calloc(4096, sizeof(u8));        // Cada fila de pixeles tiene dos colores de 4 bits
        img.names[n] = (u8*) calloc(512, sizeof(u8));          // Tabla de nombres de cada banco (mapa)
        if ((img.raw[n] == NULL) || (img.patterns[n] == NULL) || (img.colors[n] == NULL) || (img.names[n] == NULL)) {
            printf("Error creando los buffers internos.\nError critico.\n");
            free(tile_pattern);
            tile_pattern = NULL;
            free(test_pattern);
            test_pattern = NULL;
            return -1;
        }
    }

    // Analisis del RAW
    u32 main_offset = 0;
    u32 local_offset = 0;
    u32 b = 0;
    s32 match = 0;
    s32 exist = -1;

    // Analiza el archivo en pasos de 8x8 pixeles
    printf("Generando banco de tiles:\n\n");
    u32 pos_y = 0, pos_x = 0, temp_y = 0, temp_x = 0, t = 0;

    for (pos_y = 0; pos_y < img.height; pos_y += 8) {

        // Calcula el banco
        b = (int)(pos_y / 64);

        for (pos_x = 0; pos_x < img.width; pos_x += 8) {

            // Copia los datos al tile
            for (temp_y = 0; temp_y < 8; temp_y ++) {
                for (temp_x = 0; temp_x < 8; temp_x ++) {
                    local_offset = (temp_y << 3) + temp_x;
                    main_offset = ((pos_y + temp_y) * img.width) + (pos_x + temp_x);
                    tile_pattern[local_offset] = img.data[main_offset];
                }
            }



            // Borra la asignacion de tile existente
            exist = -1;

            // Ahora verifica si ya hay algun tile registrado
            if (img.tiles[b] > 0) {
                // Verifica si el tile es nuevo o ya existe
                for (n = 0; n < img.tiles[b]; n ++) {
                    // Flag de repeticion
                    match = 0;
                    // Compara la info con la almacenada en el buffer de tiles
                    for (t = 0; t < 64; t ++) {
                        local_offset = (n * 64) + t;
                        // Si los pixeles son iguales...
                        if (tile_pattern[t] == img.raw[b][local_offset]) {
                            match ++;
                        }
                        // Si todos los pixeles eran iguales, indicalo
                        if (match == 64) {
                            exist = n;
                            n = img.tiles[b];       // Sal
                        }
                    }
                }

                // Si el tile ya existe...
                if (exist != -1) {
                    // Indicador
                    img.names[b][img.map_pos[b]] = exist;
                    // Indicador
                    printf(".");
                    img.map_pos[b] ++;
                } else {    // SI no existe, agregalo a los buffers
                    // Indicador
                    //printf("B:%01d.P%03d.M%03d", b, img.tiles[b], img.map_pos[b]);
                    // Actualiza los buffers
                    memcpy(img.raw[b] + (img.tiles[b] * 64), tile_pattern, 64);                   // Copia la informacion del tile
                    img.names[b][img.map_pos[b]] = img.tiles[b];                // Indica al mapa que tile es
                    // Indicador
                    printf("%01d", b);
                    // Actualiza los contadores
                    img.map_pos[b] ++;
                    img.tiles[b] ++;
                }

            } else {

                // Si el el primero, registralo
                memcpy(img.raw[b], tile_pattern, 64);           // Copia la informacion del tile
                img.names[b][img.map_pos[b]] = img.tiles[b];    // Indica al mapa que tile es
                // Indicador
                printf("%01d", b);
                // Actualiza los contadores
                img.map_pos[b] ++;
                img.tiles[b] ++;

            }

        }
    }

    // Resumen
    printf("\n\nBanco de tiles creado con exito.\n\n");
    printf("Banco 1: %03d tiles.\n", img.tiles[0]);
    printf("Banco 2: %03d tiles.\n", img.tiles[1]);
    printf("Banco 3: %03d tiles.\n\n", img.tiles[2]);


    // Guarda los tamaños en bytes
    for (n = 0; n < 3; n ++) {
        img.patterns_size_normal[n] = (img.tiles[n] << 3);
        img.colors_size_normal[n] = (img.tiles[n] << 3);
        img.names_size_normal[n] = 256;
    }


    // Fase 2, genera las tablas de pattern y color para cada banco
    u32 l = 0;
    u32 i = 0;
    u8 bg_color = 0;
    u8 pixel_color = 0;
    u32 encode = 0;
    u32 tbl = 0;
    u8 color_table[16];
    u8 max_val = 0;
    printf("Codificando los tiles a filas de patterns...\n\n");
    for (b = 0; b < 3; b ++) {
        tbl = 0;
        // Analiza los tiles de ese banco
        for (n = 0; n < img.tiles[b]; n ++) {
            // Paso 1, analiza cada linea y calcula su valor en binario
            printf("B: %01d   T: %03d   LINES: ", b, n);
            for (l = 0; l < 8; l ++) {          // Linea del tile
                // Guarda el primer color de la fila
                local_offset = (n * 64) + (l * 8);
                bg_color = (img.raw[b][local_offset] & 0x0f);
                pixel_color = bg_color;
                encode = 0;
                // Borra la tabla de analisis de color
                memset(color_table, 0, 16);
                max_val = 0;
                // Analiza la fila de 8 pixeles
                for (i = 0; i < 8; i ++) {      // Pixel
                    local_offset = (n * 64) + (l * 8) + i;  // Calcula el offset del pixel en el buffer
                    if (bg_color != img.raw[b][local_offset]) {
                        // Si el color no es el de fondo, codifica el bit
                        encode |= (1 << (7 - i));
                        pixel_color = (img.raw[b][local_offset] & 0x0f);
                        // Y registralo en la tabla de colores
                        color_table[pixel_color] ++;
                    }
                }
                // Calcula el color predominante de pixel
                for (i = 0; i < 16; i ++) {
                    if (color_table[i] > max_val) {
                        max_val = color_table[i];
                        pixel_color = i;
                    }
                }
                // Guarda la fila
                img.patterns[b][tbl] = encode;
                img.colors[b][tbl] = ((((pixel_color + 1) << 4) & 0xf0)| ((bg_color + 1) & 0x0f));
                printf("$%02x ", img.patterns[b][tbl]);
                tbl ++;
            }
            printf("\n");
        }

    }

    // Limpia los buffers
    free(tile_pattern);
    tile_pattern = NULL;
    free(test_pattern);
    test_pattern = NULL;

    printf("\n\n");

    return 0;

}
