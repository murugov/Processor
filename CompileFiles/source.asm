        push 1
        push 2
        add
        call inc_r1
        pop r2
        push r2
        push 777
        out
        hlt

end:
        out
        hlt

inc_r1:
        push r1
        push 1
        add
        pop r1
        ret