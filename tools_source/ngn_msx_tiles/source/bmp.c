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



/*
-------------------------------------------------------------------------------
    Carga el archivo BIPMAP y conviertelo a RAW
-------------------------------------------------------------------------------
*/

// Funcion NF_LoadBMP();
s32 LoadBitmap(const char* file) {

	// Buffers locales
	u8* buffer;		// Buffer temporal
	buffer = NULL;
	u8* palette;		// Paleta (requerida por algun modo)
	palette = NULL;

	// Magic ID
	char magic_id[4];
	memset(magic_id, 0, 4);

	// Define la estructura para almacenar la cabecera del BMP
	typedef struct {
		u32 bmp_size;		// Tamaño en bytes del BMP
		u16 res_a;			// Reservado
		u16 res_b;			// Reservado
		u32 offset;			// Offset donde empiezan los datos
		u32 header_size;	// Tamaño de la cabecera (40 bytes)
		u32 bmp_width;		// Ancho de la imagen en pixeles
		u32 bmp_height;		// Altura de la imagen en pixeles
		u16 color_planes;	// Numero de planos de color
		u16 bpp;			// Numero de bits por pixel
		u32 compression;	// Compresion usada
		u32 raw_size;		// Tamaño de los datos en RAW despues de la cabecera
		u32 dpi_hor;		// Puntos por pulgada (horizontal)
		u32 dpi_ver;		// Puntos por pulgada (vertical)
		u32 pal_colors;		// Numero de colores en la paleta
		u32 imp_colors;		// Colores importantes
	} bmp_header_info;
	bmp_header_info bmp_header;

	// Pon todos los bytes de la estructura a 0
	memset(&bmp_header, 0, sizeof(bmp_header));

	// Declara los punteros a los ficheros
	FILE* file_id;

	// Carga el archivo .BMP
	file_id = fopen(file, "rb");

	if (file_id) {	// Si el archivo existe...
		// Posicionate en el byte 0
		fseek(file_id, 0, SEEK_SET);
		// Lee el Magic String del archivo BMP (2 primeros Bytes, "BM") / (0x00 - 0x01)
		fread(magic_id, 1, 2, file_id);
		// Si es un archivo BMP... (Magic string == "BM")
		if (strcmp(magic_id, "BM") == 0) {
			// Posicionate en el byte 2
			fseek(file_id, 2, SEEK_SET);
			// Lee la cabecera del archivo BMP (0x02 - 0x36)
			fread((void*)&bmp_header, 1, sizeof(bmp_header), file_id);
			/////////////////////////////////////////////////////////////
			// Es un archivo BMP valido, cargalo en un buffer temporal //
			/////////////////////////////////////////////////////////////
			// Crea un buffer temporal
			buffer = (u8*) calloc (bmp_header.raw_size, sizeof(u8));
			if (buffer == NULL) {
                printf("No se ha podido crear el buffer temporal para la imagen.\nError critico.\n");
                free(buffer);
                buffer = NULL;
                free(palette);
                palette = NULL;
                return -1;
			}
			// Si se ha creado con exito, carga el archivo al buffer
			fseek(file_id, bmp_header.offset, SEEK_SET);
			fread(buffer, 1, bmp_header.raw_size, file_id);
		} else {
			// No es un archivo BMP valido
			printf("El archivo %s no es un BMP valido.\nError critico.\n", file);
            free(buffer);
            buffer = NULL;
            free(palette);
            palette = NULL;
			return -1;
		}
	} else {
		// El archivo no existe
		printf("El archivo %s no existe.\nError critico.\n", file);
        free(buffer);
        buffer = NULL;
        free(palette);
        palette = NULL;
		return -1;
	}
	// Cierra el archivo
	fclose(file_id);

	// Variables que se usaran a partir de aqui
	u32 x = 0;			// Coordemada X
	u32 y = 0;			// Coordenada Y
	u32 idx = 0;		// Indice en el buffer temporal
	u32 offset = 0;		// Indice en el buffer de destino
	u16 colors = 0;		// En 8 bits, numero de colores
	u32 size = 0;		// Tamaño del buffer (en bytes)

    // Guarda el tamaño de la imagen
    img.width = bmp_header.bmp_width;
    img.height = bmp_header.bmp_height;
    if ((img.width != 256) || (img.height != 192)) {
        printf("Tamaño ilegal de archivo.\nSe requiere una imagen de 256x192 pixeles.\n");
        free(buffer);
        buffer = NULL;
        free(palette);
        palette = NULL;
        return -1;
    }
	// Habilita el buffer de destino (u8 de alto x ancho del tamaño de imagen)
	size = (bmp_header.bmp_width * bmp_header.bmp_height);
	img.data = (u8*) calloc (size, sizeof(u8));
	if (img.data == NULL) {
        printf("No se ha podido crear el buffer de datos para la imagen.\nError critico.\n");
        free(buffer);
        buffer = NULL;
        free(palette);
        palette = NULL;
        return -1;
	}

	// Segun los BITS por Pixel (8, 16, 24)
	switch (bmp_header.bpp) {

		case 8:		// 8 bits por pixel
			// Calcula el tamaño de la paleta
			colors = ((bmp_header.offset - 0x36) >> 2);
			palette = (u8*) calloc ((colors << 2), sizeof(u8));
			if (palette == NULL) {
                printf("No se ha podido crear el buffer para la paleta.\nError critico.\n");
                free(buffer);
                buffer = NULL;
                free(palette);
                palette = NULL;
                return -1;
			}
			// Abre de nuevo el archivo y carga la paleta
			file_id = fopen(file, "rb");
			if (!file_id) {
                printf("El archivo %s no existe.\nError critico.\n", file);
                free(buffer);
                buffer = NULL;
                free(palette);
                palette = NULL;;
                return -1;
            }
			fseek(file_id, 0x36, SEEK_SET);
			fread(palette, 1, (colors << 2), file_id);
			fclose(file_id);
			// Convierte el archivo a RAW de 8 bits
			for (y = 0; y < bmp_header.bmp_height; y ++) {
				for (x = 0; x < bmp_header.bmp_width; x ++) {
					// Calcula los offsets
					offset = ((((bmp_header.bmp_height - 1) - y) * bmp_header.bmp_width) + x);
                    // Guarda el pixel del buffer en img_data
					img.data[offset] = buffer[idx];
					// Siguiente pixel
					idx ++;
				}
				// Ajusta la alineacion a 4 bytes al final de linea
				while ((idx % 4) != 0) idx ++;
			}
			// Elimina la paleta de la RAM
			free(palette);
			palette = NULL;
			break;

        default:
            printf("La imagen no esta indexada.\nError critico.\n");
            free(buffer);
            buffer = NULL;
            free(palette);
            palette = NULL;
            return -1;
            break;

	}


	// Libera el buffer temporal
	free(buffer);
	buffer = NULL;


	// Devuelve el puntero al buffer de datos
	return 0;

}
