   .def _DSP28x_DisableInt
   .def _DSP28x_RestoreInt


_DSP28x_DisableInt:
    PUSH  ST1
    SETC  INTM,DBGM
    MOV   AL, *--SP
    LRETR

_DSP28x_RestoreInt:
    MOV   *SP++, AL
    POP   ST1
    LRETR


;//===========================================================================
;// End of file.
;//===========================================================================

   
