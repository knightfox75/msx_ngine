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



#ifndef RLE_H_INCLUDED
#define RLE_H_INCLUDED



/*
-------------------------------------------------------------------------------
    Includes
-------------------------------------------------------------------------------
*/

#include "defines.h"


/*
-------------------------------------------------------------------------------
    Compresion RLE
-------------------------------------------------------------------------------
*/

s32 RleCompress(u8* data, u32 length);



#endif // RLE_H_INCLUDED
