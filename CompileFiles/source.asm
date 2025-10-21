        push 1
        pop r0
        jmp end
        push 2
        out
        hlt

end:
        push r0
        out
        hlt