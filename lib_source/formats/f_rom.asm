;***********************************************************
;
;	NGN TEMPLATE para ASMSX
;	ASM Z80 MSX
;	Directivas de compilacion para ROM de 32kb
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

; ----------------------------------------------------------
; Definicion de variables [PAGE 3] $C000
; ----------------------------------------------------------

; Almacena las variables en la pagina 3 (Comentar si no es una ROM)
.PAGE 3
.INCLUDE "ngn/ngn_vars.asm"



; ----------------------------------------------------------
; Directivas del formato
; ----------------------------------------------------------

.PAGE 1												; Selecciona la pagina 1 [$4000] (Codigo del programa)
.ROM												; Se creara el binario en formato ROM de hasta 32kb
.START PROGRAM_START_ADDRESS						; Indicale al compilador donde empieza el programa
.db 78, 71, 78, 95, 84, 69, 77, 80, 76, 65, 84, 69  ; 12 digitos para completar la cabecera de la ROM

OUTPUT_FORMAT = 2									; Define el formato de salida



;***********************************************************
; Fin del archivo
;***********************************************************
F_ROM_EOF: