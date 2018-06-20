;***********************************************************
;
; NGN TEMPLATE para ASMSX
; ASM Z80 MSX
;
; Textos del programa
;
; (cc)2018 Cesar Rincon "NightFox"
; http://www.nightfoxandco.com
;
;***********************************************************



; ----------------------------------------------------------
; Funcion principal
; ----------------------------------------------------------

; Texto de bienvenida
TEXT_HELLO_WORLD:
	db	"Hello World!", $0D, $0A
	db	$00

; Texto de reinicio
TEXT_RESTART:

	db	"REBOOTING...", $0D, $0A
	db	$00



;***********************************************************
; Fin del archivo
;***********************************************************
TEXT_EOF: