.global start

start:
    // 0000 0001 0082 0001 0400 23bb 5edf 3705

    li %r6, 0x0000
    sth %r6, 0(%r4)

    li %r6, 0x0001
    sth %r6, 2(%r4)

    li %r6, 0x0082
    sth %r6, 4(%r4)

    li %r6, 0x0001
    sth %r6, 6(%r4)

    li %r6, 0x0400
    sth %r6, 8(%r4)

    li %r6, 0x23bb
    sth %r6, 10(%r4)

    li %r6, 0x5edf
    sth %r6, 12(%r4)

    li %r6, 0x3705
    sth %r6, 14(%r4)

    li %r3, 0
    blr