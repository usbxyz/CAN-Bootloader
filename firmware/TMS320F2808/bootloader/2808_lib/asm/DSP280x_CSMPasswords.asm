
      .sect "csmpasswds"

      .int  0xFFFF      ;PWL0 (LSW of 128-bit password)
      .int  0xFFFF      ;PWL1
      .int  0xFFFF      ;PWL2
      .int  0xFFFF      ;PWL3
      .int  0xFFFF      ;PWL4
      .int  0xFFFF      ;PWL5
      .int  0xFFFF      ;PWL6
      .int  0xFFFF      ;PWL7 (MSW of 128-bit password)
     .sect "csm_rsvd"
        .loop (3F7FF5h - 3F7F80h + 1)
              .int 0x0000
        .endloop
