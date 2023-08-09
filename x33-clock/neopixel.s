@@@ x2400/neopixel.s                         
@@@ Copyright (c) 2020 J. M. Spivey
        
        .syntax unified
        .text

        .set GPIO_BASE, 0x50000500 @ Base address of GPIO registers
        .set SET, 0x8              @ Offset of GPIO_OUTSET
        .set CLR, 0xc              @ Offset of GPIO_OUTCLR

@@@ The signal for a neopixel string consists of a sequence of pixels.
@@@ Each pixel contains 24 bits, and a bit lasts 1.25 +/- 0.6 usec.
@@@ A zero bit is high for 0.4 +/- 0.15 usec then low for 0.85 usec.
@@@ A one bit is high for 0.8 +/- 0.15 usec then low for 0.45 usec
@@@ One frame is separated from the next by a low period lasting
@@@ 50 usec or longer.
@@@ 
@@@ Each bit is transmitted in three phases.
@@@   Phase 1: High for 6 cycles
@@@   Phase 2: High/Low for 7 cycles
@@@   Phase 3: Low for 7 cycles
@@@ The cycles are counted out below in comments like [2.4],
@@@ meaning cycle 4 of phase 2.
        
        .global neoframe
        .thumb_func
neoframe:       
        @@  args: r0 = pin, r1 = buffer, r2 = npixels
        push {r4, r5}

        @@ r0 -- scratch
        @@ r1 -- limit of buffer
        @@ r2 -- buffer index (-ve)
        @@ r3 -- pixel index
        @@ r4 -- bit for GPIO pin
        @@ r5 -- pointer to GPIO regs

        lsls r2, #2             @ Multiply pixel count by 4
        beq done                @ Finished if it was zero
        adds r1, r1, r2         @ Adjust r1 to end of buffer
        negs r2, r2             @ Set r2 to (-ve) offset of first pixel
        movs r3, #24            @ Set r3 to bit index in pixel
        movs r4, #1             @ Set r4 to GPIO bit
        lsls r4, r4, r0
        ldr r5, =GPIO_BASE      @ Set r5 to GPIO base address
nextbit:
        @@ Phase 1
        str r4, [r5, #SET]      @ [1.1] Output high
        ldr r0, [r1, r2]        @ [1.3] Fetch pixel
        lsrs r0, r0, r3         @ [1.5] Put relevant bit in C flag
        bcs high                @ [1.6] Branch if bit is 1
        @@ Phase 2
        str r4, [r5, #CLR]      @ [2.1] Output low if bit is 0
high:   
        nop                     @ [2.3]
        subs r3, r3, #1         @ [2.4] Update bit index
        beq lastbit             @ [2.5] Branch if last bit
        nop; nop                @ [2.6]
        @@ Phase 3 -- normal bit
        str r4, [r5, #CLR]      @ [3.1] Output low if not already done
        nop; nop                @ [3.3]
        b nextbit               @ [3.5] Go for next bit
lastbit:        
        @@ Phase 3 -- last bit of pixel
        str r4, [r5, #CLR]      @ [3.1] Output low
        movs r3, #24            @ [3.3] Reset bit index
        adds r2, r2, #4         @ [3.4] Update pixel index      
        blt nextbit             @ [3.5] Go for first bit of next pixel
done:   
        pop {r4, r5}
        bx lr
