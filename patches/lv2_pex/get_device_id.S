.global start

start:
    // 00 00 00 01 00 82 00 01 04 00 23 bb 5e df 37 05

    li %r6, 0x00
    stb %r6, 0(%r4)

    li %r6, 0x00
    stb %r6, 1(%r4)

    li %r6, 0x00
    stb %r6, 2(%r4)

    li %r6, 0x01
    stb %r6, 3(%r4)

    li %r6, 0x00
    stb %r6, 4(%r4)

    li %r6, 0x82
    stb %r6, 5(%r4)

    li %r6, 0x00
    stb %r6, 6(%r4)

    li %r6, 0x01
    stb %r6, 7(%r4)

    li %r6, 0x04
    stb %r6, 8(%r4)

    li %r6, 0x00
    stb %r6, 9(%r4)

    li %r6, 0x23
    stb %r6, 10(%r4)

    li %r6, 0xbb
    stb %r6, 11(%r4)

    li %r6, 0x5e
    stb %r6, 12(%r4)

    li %r6, 0xdf
    stb %r6, 13(%r4)

    li %r6, 0x37
    stb %r6, 14(%r4)

    li %r6, 0x05
    stb %r6, 15(%r4)

    li %r3, 0
    blr