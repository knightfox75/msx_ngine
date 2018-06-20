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



#ifndef TILEMAP_H_INCLUDED
#define TILEMAP_H_INCLUDED



/*
-------------------------------------------------------------------------------
    Includes
-------------------------------------------------------------------------------
*/

#include "defines.h"



/*
-------------------------------------------------------------------------------
    Analiza los datos y genera el archivo tileset
-------------------------------------------------------------------------------
*/

s32 GenerateTileset(void);



#endif // TILEMAP_H_INCLUDED
