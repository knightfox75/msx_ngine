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



/*
-------------------------------------------------------------------------------
    Variables globales
-------------------------------------------------------------------------------
*/

img_info img;






/*
-------------------------------------------------------------------------------
    Limpia los buffers
-------------------------------------------------------------------------------
*/

void CleanBuffers(void) {

    free(img.data);
    img.data = NULL;

    int n = 0;
    for (n = 0; n < 3; n ++) {
        free(img.raw[n]);
        img.raw[n] = NULL;
        free(img.patterns[n]);
        img.patterns[n] = NULL;
        free(img.colors[n]);
        img.colors[n] = NULL;
        free(img.names[n]);
        img.names[n] = NULL;
        free(img.rle);
        img.rle = NULL;
    }

}
