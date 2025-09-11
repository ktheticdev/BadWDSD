.global start

start:
    // 0001 0082 0001 0001

    li %r6, 0x0001
    sth %r6, 0(%r4)

    li %r6, 0x0082
    sth %r6, 2(%r4)

    li %r6, 0x0001
    sth %r6, 4(%r4)

    li %r6, 0x0001
    sth %r6, 6(%r4)

    li %r3, 0
    blr