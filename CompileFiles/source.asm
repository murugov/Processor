        push 1
        push 2
        add
        call end
        pop r2
        push r2
        push 777
        out
        hlt

end:
        out
        ret