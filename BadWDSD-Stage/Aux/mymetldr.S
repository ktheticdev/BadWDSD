.global start

start:
    // sp = 0x11F00
    ila sp, 0x11F00

    // jump to main
    br main

    //
    stop 1