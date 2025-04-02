        mov	r1, r2
LABEL1:	mov r1, r2
        .extern RESERVED_NAME ; RESERVED_NAME
        .string "Hello, "World!"" ; EXTRA_QUOTATION_MARK
mcro a_mc
;nothing
mcroend

mcro mcro ; unallowed name
prn #62
mcroend

mcro a_mc
;duplicate macro
prn #566
mcroend

        .data 5, 10, 15
LABEL1:	 mov r1, r2 ; SYMBOL_ALREADY_EXISTS
        .extern EXTERNAL_LABEL
:	    mov r1, r2 ; EMPTY_LABEL
        mov r1, r9 ; SYMBOL_NOT_FOUND_DURING_BUILD
        add #5, LABEL
EXTERNAL_LABEL:	.data 5 ; INITIALIZING_EXTERN
        add LABEL ; MISSING_ARGS
        .string "Hello, World!"
        .entry ENTRY_LABEL
        .entry UNDEFINED_LABEL ; ENTRY_SYMBOL_NOT_FOUND
        stop
        stop EXTRA_OPERAND ; EXTRA_TEXT_AFTER_STRING