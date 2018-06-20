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
#include "rle.h"


/*
-------------------------------------------------------------------------------
    Comprime los datos en RLE
-------------------------------------------------------------------------------
*/

s32 RleCompress(u8* data, u32 length) {

    // Variables
    u32 pos = 0;
    u32 dest_pos = 0;
    u32 new_length = 0;
    u8 rle_data = 0;
    u8 rle_repeat = 0;
    u32 start_point = 0;
    u32 end_point = 0 ;
    u8 command = 1;
    u8 next_command = 1;
    s32 n = 0;
    bool repeat = false;

    u8* buffer = (u8*) calloc(4096, sizeof(u8));
    if (buffer == NULL) {
        printf("Memoria insuficiente.\n");
        return -1;
    }

    printf("\nCompresion RLE en proceso:\n\n");


    // Si aun quedan datos por evaluar
    do {

        // Primero verifica si no es el ultimo dato del buffer
        switch (command) {

            // Empieza a comparar datos
            case 1:
                // Si el siguiente dato, aun esta dentro del rango...
                if ((pos + 1) < length) {
                    if (data[pos] == data[(pos + 1)]) {
                        // Guarda la informacion de inicio
                        rle_data = data[pos];
                        rle_repeat = 1;
                        start_point = pos;
                        // Salta a la seccion de busqueda
                        next_command = 2;
                    } else {
                        // El dato no se repite, escribelo tal cual
                        next_command = 3;
                    }
                    break;
                } else {        // El dato es el ultimo del buffer
                    next_command = 3;
                }
                break;


            // Busca cuantos datos iguales consecutivos hay
            case 2:

                n = start_point;    // Punto de incio de la busqueda
                do {
                    // Si no estamos al final de buffer
                    if ((n + 1) < length) {
                        // Si los datos concuerdan
                        if (data[n] == data[(n + 1)]) {
                            n ++;
                            rle_repeat ++;
                            // Verifica si no has sobrepasado el numero maximo de repeticiones
                            if (rle_repeat >= 63) {
                                end_point = n;
                                repeat = false;
                            } else {
                                repeat = true;
                            }
                        } else {
                            // Si no concuerdan, sal
                            end_point = n;
                            repeat = false;
                        }
                    } else {
                        // Fin del buffer, sal si o si
                        end_point = n;
                        repeat = false;
                    }
                } while (repeat);
                next_command = 4;

                break;


            // Escribe el dato tal cual
            case 3:
                // Si el dato es >= 192...
                if (data[pos] >= 192) {
                    printf("*");
                    buffer[dest_pos] = 192 | 1;         // Indica que es un dato comprimido
                    dest_pos ++;                        // con una sola repeticion
                    buffer[dest_pos] = data[pos];       // Escribe el dato en el buffer de destino
                } else {
                    printf("O");
                    buffer[dest_pos] = data[pos];       // Escribe los datos tal cual
                }
                dest_pos ++;                            // E incrementa los contadores
                pos ++;
                next_command = 1;                       // Indica que busque de nuevo
                break;

            // Escribe el dato comprimido
            case 4:
                //printf("[%d.%d]", rle_data, rle_repeat);
                if (rle_repeat >= 16) {
                    printf("²");
                } else if (rle_repeat >= 8) {
                    printf("±");
                } else {
                    printf("°");
                }
                buffer[dest_pos] = 192 | rle_repeat;    // Escribe la ID de compresion
                dest_pos ++;                            // [192 + repeticiones]
                buffer[dest_pos] = rle_data;            // Escribe el dato
                dest_pos ++;
                pos = (end_point + 1);                  // Offset de busqueda al inicio del siguiente bloque
                next_command = 1;
                break;

        }

        // Actualiza el comando
        command = next_command;


    } while (pos < length);

    new_length = dest_pos;

    // Actualiza el buffer
    memcpy(data, buffer, new_length);

    // Si la compresion es exitosa
    if (new_length < length) {

        // Mensage
        printf("\n\nCompresion finalizada.\n");
        printf("Tamaño original: %d bytes.\n", length);
        printf("Tamaño comprimido: %d bytes.\n", dest_pos);
        printf("Tamaño reducido en un: %d%%.\n\n", (100 - ((new_length * 100) / length)));

    } else {

        // Mensage
        printf("\n\nCompresion fallida.\n");
        printf("Tamaño original: %d bytes.\n", length);
        printf("Tamaño comprimido: %d bytes.\n", dest_pos);
        printf("Tamaño incrementado al: %d%%.\n\n", ((new_length * 100) / length));

    }

    // Limpia los buffers
    free (buffer);
    buffer = NULL;

    // Devuelve la nueva longitud
    return new_length;

}
