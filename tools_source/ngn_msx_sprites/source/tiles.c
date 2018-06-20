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
#include "tiles.h"


/*
-------------------------------------------------------------------------------
    Analiza los datos y genera el archivo tileset
-------------------------------------------------------------------------------
*/

s32 GenerateTiles(void) {

    // Variables
    s32 n = 0;
    s32 i = 0;
    s32 img_size = (img.width * img.height);
    s32 buffer_size = (img_size / 8);

    // Crea los buffers para las tablas de pattern del Sprite
    // (Asume que todos los tiles son unicos)
    img.tiles = (u8*) calloc(buffer_size, sizeof(u8));
    if (img.tiles == NULL) {
        printf("No se ha podido crear el buffer para los tiles.\nError critico.\n");
        CleanBuffers();
    }


    // Analisis del RAW
    u32 src_offset = 0;
    u32 dst_offset = 0;
    u8 pixel = 0;
    u8 encode = 0;
    u32 tile_num = 0;
    u32 last_tile = 0;
    u32 temp_tile = 0;
    u8 idx = 0;

    // Copia la informacion de la imagen en el formato correcto de tiles
    if (img.format == 8) {
         printf("Codificando Sprite de 8x8...\n\n");
        // Formato 8x8
        for (n = 0; n < img_size; n += 8) {
            encode = 0;
            idx = 0;
            tile_num = ((int)(n / 64));
            if (tile_num != last_tile) {
                last_tile = tile_num;
                printf("\n");
            }
            printf("T: %03d    ", tile_num);
            for (i = n; i < (n + 8); i ++) {
                pixel = img.data[i] & 0x0f;     // Adquiere el color del pixel
                // Si el pixel no es transparente
                if (pixel != 0x0f) {
                    // Codifica esta BIT
                    encode |= (1 << (7 - idx));
                    printf("Û");
                } else {
                    printf(" ");
                }
                idx ++;
            }
            printf("\n");
            // Guarda el valor de la fila del tile
            img.tiles[dst_offset] = encode;
            dst_offset ++;
        }
        printf("\n");

    } else {

        printf("Codificando Sprite de 16x16...\n\n");

        // Formato 16x16
        for (src_offset = 0 ; src_offset < img_size; src_offset += 256) {

            // Filas bloques 0 y 1
            for (n = src_offset; n < (src_offset + 256); n += 16) {
                // Columnas bloques 0 y 1
                encode = 0;
                idx = 0;
                if (tile_num != last_tile) {
                    last_tile = tile_num;
                    printf("\n");
                }
                printf("T: %03d    ", tile_num);
                for (i = n; i < (n + 8); i ++) {
                    pixel = img.data[i] & 0x0f;     // Adquiere el color del pixel
                    // Si el pixel no es transparente
                    if (pixel != 0x0f) {
                        // Codifica esta BIT
                        encode |= (1 << (7 - idx));
                        printf("Û");
                    } else {
                        printf(" ");
                    }
                    idx ++;
                }
                printf("\n");
                // Guarda el valor de la fila del tile
                img.tiles[dst_offset] = encode;
                dst_offset ++;
                temp_tile ++;
                if (temp_tile >= 8) {
                    temp_tile = 0;
                    tile_num ++;
                }
            }

            // Filas bloques 2 y 3
            temp_tile = 0;
            for (n = src_offset; n < (src_offset + 256); n += 16) {
                // Columnas bloques 2 y 3
                encode = 0;
                idx = 0;
                if (tile_num != last_tile) {
                    last_tile = tile_num;
                    printf("\n");
                }
                printf("T: %03d    ", tile_num);
                for (i = (n + 8); i < (n + 16); i ++) {
                    pixel = img.data[i] & 0x0f;     // Adquiere el color del pixel
                    // Si el pixel no es transparente
                    if (pixel != 0x0f) {
                        // Codifica esta BIT
                        encode |= (1 << (7 - idx));
                        printf("Û");
                    } else {
                        printf(" ");
                    }
                    idx ++;
                }
                printf("\n");
                // Guarda el valor de la fila del tile
                img.tiles[dst_offset] = encode;
                dst_offset ++;
                temp_tile ++;
                if (temp_tile >= 8) {
                    temp_tile = 0;
                    tile_num ++;
                }
            }

        }
        printf("\n");

    }


    printf("\nCodificacion finalizada.\n\n");

    img.tiles_size_normal = dst_offset;

    return 0;

}
