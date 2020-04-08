;***********************************************************
;
;	NGN TEMPLATE para ASMSX
;	ASM Z80 MSX
;
;	(cc) 2018-2020 Cesar Rincon "NightFox"
;	https://nightfoxandco.com
;
;***********************************************************





;***********************************************************
; Directivas para el compilador
;***********************************************************

; ----------------------------------------------------------
; Directivas de principales
; ----------------------------------------------------------

.BIOS					; Nombres de las llamadas a BIOS

OUTPUT_FORMAT_BINARY = 1	; Formato de salida binario de BASIC
OUTPUT_FORMAT_ROM = 2		; Formato de salida ROM



; ----------------------------------------------------------
; Selecciona la directiva de compilacion (descomentar)
; ----------------------------------------------------------

;.INCLUDE "formats/f_binary.asm"		; Binario de BASIC
.INCLUDE "formats/f_rom.asm"			; Cartucho ROM




;***********************************************************
; Programa
;***********************************************************

PROGRAM_START_ADDRESS:

	; ----------------------------------------------------------
	; Punto de incicio
	; ----------------------------------------------------------

	IF (OUTPUT_FORMAT == OUTPUT_FORMAT_ROM)
		.SEARCH			; Busca un punto de inicio valido
	ENDIF


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