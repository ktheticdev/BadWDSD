.global start

start:
    // 00 01 00 82 00 01 00 01

    li %r6, 0x00
    stb %r6, 0(%r4)

    li %r6, 0x01
    stb %r6, 1(%r4)

    li %r6, 0x00
    stb %r6, 2(%r4)

    li %r6, 0x82
    stb %r6, 3(%r4)

    li %r6, 0x00
    stb %r6, 4(%r4)

    li %r6, 0x01
    stb %r6, 5(%r4)

    li %r6, 0x00
    stb %r6, 6(%r4)

    li %r6, 0x01
    stb %r6, 7(%r4)

    li %r3, 0
    blr