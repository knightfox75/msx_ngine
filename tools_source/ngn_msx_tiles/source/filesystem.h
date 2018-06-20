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



#ifndef FILESYSTEM_H_INCLUDED
#define FILESYSTEM_H_INCLUDED



/*
-------------------------------------------------------------------------------
    Includes
-------------------------------------------------------------------------------
*/

#include "defines.h"


/*
-------------------------------------------------------------------------------
    Graba los datos a los archivos
-------------------------------------------------------------------------------
*/

s32 SaveFileNormal (const char* filename);
s32 SaveFileRLE (const char* filename);




#endif // FILESYSTEM_H_INCLUDED
