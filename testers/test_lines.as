;this is a note 
macr mov
macr cmp
macr add
macr sub
macr lea
macr not
macr clr
macr inc
macr dec
macr jmp
macr bne
macr red
macr prn
macr jsr
macr rts
macr stop


;cheking inst 

;bad label name 

.string: .data 1,2
.data: .data 1,2
mov: .data 1,2
cmp: .data 1,2
add: .data 1,2
sub: .data 1,2
lea: .data 1,2
not: .data 1,2
clr: .data 1,2
inc: .data 1,2
dec: .data 1,2
jmp: .data 1,2
bne: .data 1,2
red: .data 1,2
prn: .data 1,2
jsr: .data 1,2
rts: .data 1,2
stop: .data 1,2

;bad label syntex
hello .data 1,2
:hello .data 1,2



;data checking

;invalid number
hello: .data 7,7.5

;comma issues
hello: .data 7 2,
hello: .data ,7 ,2
hello: .data ,7 ,2,

;invalid name 
hello: .datA 7,2
hello: data 7,2


;string cheking
;invalid string
hello: .string "hello

;too many strings
hello: .string "hello" "hello"

;entry and extern 

;no labale name 
.entry
.extrn
;too many names
.entry hello: hello:
.extrn hello: hello:


;command line capital letters

;invalid command
hello: MOV r1,r2
hello: CMP r1,r2
hello: ADD r1,r2
hello: SUB r1,r2
hello: LEA 1
hello: NOT helo
hello: CLR helo
hello: INC helo
hello: DEC helo
hello: JMP helo
hello: BNE helo
hello: RED helo
hello: PRN helo
hello: JSR helo
hello: RTS 
hello: STOP

;2 operand commands check
;only one operand , need to be 2 
hello: mov r1
hello: cmp r1
hello: add r1
hello: sun r1

;invalid operand cannot be a number
hello: mov #4
hello: add #4
hello: sun #4

;lea can resive only a labale as the first operand
 hello: lea #4,label

;lea cant resive number as second operand
hello: lea label,#4


;too many operands

hello: not 1,2
hello: clr 1,2
hello: inc 1,2
hello: dec 1,2
hello: jmp 1,2
hello: bne 1,2
hello: red 1,2
hello: prn 1,2
hello: jsr 1,2
hello: stop 1

;cant resive label 

hello: not hello
hello: clr hello
hello: inc hello
hello: dec hello
hello: jmp hello
hello: bne hello
hello: red hello
hello: jsr hello

;cant resive register without *

hello: jmp r2
hello: bne r2
hello: prn r2
hello: jsr r2






