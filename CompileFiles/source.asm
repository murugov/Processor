        push 1
        push 2
        add
        jmp end
        pop r2
        push r2
        push 777
        out
        hlt

end:
        out
        hlt