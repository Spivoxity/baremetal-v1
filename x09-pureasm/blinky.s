@@@ x09-pureasm/blinky.s
@@@ Copyright (c) 2019 J. M. Spivey

    .thumb                      @ Blinky for Micro:bit V1
    .syntax unified
        .global __reset

    .section .vectors
    @@ Vector table at address 0
    .word __stack               @ Initial stack pointer
    .word __reset               @ Reset vector
    @@ Rest of the vectors are unused

    .text
    .thumb_func
delay:                          @ Delay in microseconds (16MHz clock)
    lsls r0, r0, #2             @ 250ns per iteration
repeat: 
    subs r0, #1                 @ One iteration takes 4 cycles
    bhi repeat
    bx lr

    .thumb_func
__reset:
    ldr r0, =0x50000514         @ GPIO_DIR register
    ldr r1, =0xfff0             @ LED pins as outputs
    str r1, [r0]

    ldr r4, =0x50000504         @ Address of GPIO_OUT register
    ldr r5, =0x00005fb0         @ Pattern for middle LED on
    movs r6, #0                 @ Pattern for all off
again:
    str r5, [r4]                @ Light an LED
    ldr r0, =500000             @ Wait 500ms
    bl delay
    str r6, [r4]                @ All LEDs off
    ldr r0, =500000             @ Wait 500ms
    bl delay
    b again                     @ and repeat
