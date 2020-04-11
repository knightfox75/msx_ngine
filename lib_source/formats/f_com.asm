;***********************************************************
;
;	NGN TEMPLATE para ASMSX
;	ASM Z80 MSX
;	Directivas de compilacion para .COM de MSX-DOS
;
;	(cc) 2018-2020 Cesar Rincon "NightFox"
;	https://nightfoxandco.com
;
;
;	Compilar con asMSX 0.19 o superior
;	https://github.com/Fubukimaru/asMSX
;
;***********************************************************



;***********************************************************
; Directivas para el compilador
;***********************************************************

OUTPUT_FORMAT = 3							; Define el formato de salida

; ----------------------------------------------------------
; Directivas del formato
; ----------------------------------------------------------

; Los programas de MSX-DOS no requieren direccion inicial
.MSXDOS										; Se creara el binario en formato .COM para MSX-DOS


; ----------------------------------------------------------
; Definicion de variables
; ----------------------------------------------------------

; Almacena las variables
.INCLUDE "ngn/ngn_vars.asm"


;***********************************************************
; Fin del archivo
;***********************************************************
F_COM_EOF: