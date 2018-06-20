/*
-------------------------------------------------------------------------------
    MSX Sprite to VRAM converter
    Genera desde una imagen las tablas de SPRITE.
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

    u32 format;         // Formato del Sprite (8x8 o 16x16)
    u32 frames;         // nº de frames (si los hay)

    u8* data;           // Datos de la imagen
    u8* tiles;          // Tiles en bruto

    u32 tiles_size_normal;

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
