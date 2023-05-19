global _puts

; TODO retrieve pointer and 
_puts:
    xchg bx, bx
    sysenter
    ret