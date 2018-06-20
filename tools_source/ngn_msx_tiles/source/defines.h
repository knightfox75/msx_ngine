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



#ifndef DEFINES_H_INCLUDED
#define DEFINES_H_INCLUDED



/*
-------------------------------------------------------------------------------
    Definicion de tipos
-------------------------------------------------------------------------------
*/

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

typedef unsigned char bool;
#define true 1
#define false 0




/*
-------------------------------------------------------------------------------
    Variables globales
-------------------------------------------------------------------------------
*/

typedef struct {

    u32 width;          // Ancho de la imagen
    u32 height;         // Alto de la imagen

    u8* data;           // Datos de la imagen
    u8* raw[3];         // Tiles en bruto
    u32 tiles[3];       // Tiles en cada "banco" (3)
    u32 map_pos[3];     // Posicion en el mapa

    u8* patterns[3];    // Buffer para los patterns
    u32 patterns_size_normal[3];
    u32 patterns_size_rle[3];

    u8* colors[3];      // Buffer para los colores
    u32 colors_size_normal[3];
    u32 colors_size_rle[3];

    u8* names[3];       // Buffer para los nombres (mapa)
    u32 names_size_normal[3];
    u32 names_size_rle[3];

    u8* rle;            // Buffer temporal para la compresion RLE

    bool sz;            // Flag de generacion de info del tamaño
    bool inc;           // Flag de generacion de archivo de .INCLUDE

} img_info;

extern img_info img;           // Guarda la imagen cargada y su informacion




/*
-------------------------------------------------------------------------------
    Limpia los buffers
-------------------------------------------------------------------------------
*/

extern void CleanBuffers(void);



#endif // DEFINES_H_INCLUDED
