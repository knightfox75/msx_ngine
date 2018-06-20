;***********************************************************
;
; NGN TEMPLATE para ASMSX
; ASM Z80 MSX
;
; (cc)2018 Cesar Rincon "NightFox"
; http://www.nightfoxandco.com
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
; Otras directivas
; ----------------------------------------------------------

.PAGE 1					; Selecciona la pagina 1 [$4000] (Codigo del programa)
.BIOS					; Nombres de las llamadas a BIOS
.ROM					; Se creara el binario en formato ROM de hasta 32kb
.START PROGRAM_START_ADDRESS		; Indicale al compilador donde empieza el programa
.db 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0	; 12 ceros para completar la cabecera de la ROM



;***********************************************************
; Programa
;***********************************************************

PROGRAM_START_ADDRESS:

	; ----------------------------------------------------------
	; Punto de incicio
	; ----------------------------------------------------------

	.SEARCH			; Busca un punto de inicio valido


	; ----------------------------------------------------------
	; Ejecuta el programa
	; ----------------------------------------------------------

	; Ejecuta el programa
	jp FUNCTION_MAIN

	; Punto final del programa
	ret


	; ----------------------------------------------------------
	; Definicion de constantes
	; ----------------------------------------------------------

	.INCLUDE "ngn/ngn_consts.asm"	; Constantes de N'gine



	; ----------------------------------------------------------
	; Codigo principal
	; ----------------------------------------------------------

	; Archivo principal
	.INCLUDE "prog/main.asm"



	; ----------------------------------------------------------
	; Libreria N'gine
	; ----------------------------------------------------------

	.INCLUDE "ngn/ngn.asm"



	; ----------------------------------------------------------
	; Datos del programa
	; ----------------------------------------------------------

	; Textos del programa
	.INCLUDE "data/text.asm"



;***********************************************************
; Fin del archivo
;***********************************************************
NGN_TEMPLATE_EOF: