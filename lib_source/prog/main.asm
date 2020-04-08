;***********************************************************
;
;	NGN TEMPLATE para ASMSX
;	ASM Z80 MSX
;
;	Archivo principal
;
;	(cc) 2018-2020 Cesar Rincon "NightFox"
;	https://nightfoxandco.com
;
;***********************************************************



; ----------------------------------------------------------
; Funcion principal
; ----------------------------------------------------------

FUNCTION_MAIN:

	; Inicializaciones
	call NGN_START				; Inicia la libreria NGN

	; Pon la VDP en MODO SCR0 / 40 columnas
	ld bc, $0F04			; Color de frente/fondo
	ld de, $0128			; Color de bore/ancho en columnas (40)
	call NGN_SCREEN_SET_MODE_0

	; Ejecuta la rutina [DISSCR] para deshabilitar la pantalla
	call $0041
	; Borra la pantalla
	call NGN_TEXT_CLS
	; Hello world!
	ld hl, TEXT_HELLO_WORLD		; Apunta al texto a mostrar
	call NGN_TEXT_PRINT		; E imprimelo en pantalla
	; Ejecuta la rutina [ENASCR] para habilitar la pantalla
	call $0044

	; Espera a que se pulse una tecla para salir
	@@LOOP:

		call NGN_KEYBOARD_READ		; Lee el teclado

		ld a, [NGN_KEY_ANY]		; Lee el estado de "Cualquier tecla"
		and $02				; Estado "PRESS"
		jr nz, FUNCTION_EXIT		; Si se ha pulsado, sal del bucle

		halt				; Espera a la interrupcion del VDP

		jr @@LOOP			; Repite el bucle



; Fin del programa
FUNCTION_EXIT:

	; Ejecuta la rutina [DISSCR] para deshabilitar la pantalla
	call $0041
	; Borra la pantalla
	call NGN_TEXT_CLS
	; Texto de reinicio
	ld hl, $0F0B			; Posicion del cursor de texto [XXYY]
	call NGN_TEXT_POSITION		; Posiciona el cursor
	ld hl, TEXT_RESTART		; Apunta al texto a mostrar
	call NGN_TEXT_PRINT		; E imprimelo en pantalla
	; Ejecuta la rutina [ENASCR] para habilitar la pantalla
	call $0044

	; Reinicia el MSX en caso de que pase algo fuera de lo comun y se salga del programa (o se haya pulsado la tecla de salir)
	call $0000		; (CHKRAM)



;***********************************************************
; Fin del archivo
;***********************************************************
MAIN_EOF: