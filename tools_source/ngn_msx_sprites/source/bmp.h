/*
-------------------------------------------------------------------------------
    MSX Sprite to VRAM converter
    Genera desde una imagen las tablas de SPRITE.
    (cc)2015-2018 by Cesar Rincon "NightFox"
    http://www.nightfoxandco.com
    Version 0.1.0-a
-------------------------------------------------------------------------------
*/



#ifndef BMP_H_INCLUDED
#define BMP_H_INCLUDED



/*
-------------------------------------------------------------------------------
    Includes
-------------------------------------------------------------------------------
*/

#include "defines.h"


/*
-------------------------------------------------------------------------------
    Carga el archivo BIPMAP y conviertelo a RAW
-------------------------------------------------------------------------------
*/

extern s32 LoadBitmap(const char* file);



#endif // BMP_H_INCLUDED
