.global ap_offload_asm
.type ap_offload_asm, @function

ap_offload_asm:
    li      a0, 0x80002000      # destino
    li      a1, 0x80004000      # origem
    li      t0, 32              # 512 / 16

loop:
    lbu     t1, 0(a1)
    lbu     t2, 1(a1)
    lbu     t3, 2(a1)
    lbu     t4, 3(a1)
    lbu     t5, 4(a1)
    lbu     t6, 5(a1)
    lbu     a2, 6(a1)
    lbu     a3, 7(a1)

    sb      t1, 0(a0)
    sb      t2, 1(a0)
    sb      t3, 2(a0)
    sb      t4, 3(a0)
    sb      t5, 4(a0)
    sb      t6, 5(a0)
    sb      a2, 6(a0)
    sb      a3, 7(a0)

    lbu     t1, 8(a1)
    lbu     t2, 9(a1)
    lbu     t3, 10(a1)
    lbu     t4, 11(a1)
    lbu     t5, 12(a1)
    lbu     t6, 13(a1)
    lbu     a2, 14(a1)
    lbu     a3, 15(a1)

    sb      t1, 8(a0)
    sb      t2, 9(a0)
    sb      t3, 10(a0)
    sb      t4, 11(a0)
    sb      t5, 12(a0)
    sb      t6, 13(a0)
    sb      a2, 14(a0)
    sb      a3, 15(a0)

    addi    a1, a1, 16
    addi    a0, a0, 16

    addi    t0, t0, -1
    bnez    t0, loop

    ret
