.data
cmd0_arg0:
        .quad 2
cmd0_arg1:
        .string "/bin/cat"
cmd0_arg2:
        .string "01-tiny-access.log"


cmd1_arg0:
        .quad 2
cmd1_arg1:
        .string "/bin/awk"
cmd1_arg2:
        .string "BEGIN{FS=\" \"}($0 !~ /.*\"-\".*/){print $11}"


cmd2_arg0:
        .quad 2
cmd2_arg1:
        .string "/bin/sed"
cmd2_arg2:
        .string "s/\"//g"


cmd3_arg0:
        .quad 2
cmd3_arg1:
        .string "/bin/sort"
cmd3_arg2:
        .string "-s"


cmd4_arg0:
        .quad 2
cmd4_arg1:
        .string "/bin/uniq"
cmd4_arg2:
        .string "-c"


cmd5_arg0:
        .quad 2
cmd5_arg1:
        .string "/bin/awk"
cmd5_arg2:
        .string "BEGIN{FS=\" \";nrows=10010}{print $2\"\t\"$1\"\t\"$1/nrows*100\"%\"}"


cmd6_arg0:
        .quad 3
cmd6_arg1:
        .string "/bin/sort"
cmd6_arg2:
        .string "-k"
cmd6_arg3:
        .string "3n"


cmd7_arg0:
        .quad 2
cmd7_arg1:
        .string "/bin/tail"
cmd7_arg2:
        .string "-10"


cmd8_arg0:
        .quad 1
cmd8_arg1:
        .string "/bin/tac"


command0:
        .quad cmd0_arg0, cmd0_arg1, cmd0_arg2
command1:
        .quad cmd1_arg0, cmd1_arg1, cmd1_arg2
command2:
        .quad cmd2_arg0, cmd2_arg1, cmd2_arg2
command3:
        .quad cmd3_arg0, cmd3_arg1, cmd3_arg2
command4:
        .quad cmd4_arg0, cmd4_arg1, cmd4_arg2
command5:
        .quad cmd5_arg0, cmd5_arg1, cmd5_arg2
command6:
        .quad cmd6_arg0, cmd6_arg1, cmd6_arg2, cmd6_arg3
command7:
        .quad cmd7_arg0, cmd7_arg1, cmd7_arg2
command8:
        .quad cmd8_arg0, cmd8_arg1        
commands:
        .quad command0
        .quad command1
        .quad command2
        .quad command3
        .quad command4
        .quad command5
        .quad command6
        .quad command7
        .quad command8
fds:
        .space 4, 0 # pipe 0
        .space 4, 0
        .space 4, 0 # pipe 1
        .space 4, 0
        .space 4, 0 # pipe 2
        .space 4, 0
        .space 4, 0 # pipe 3
        .space 4, 0
        .space 4, 0 # pipe 4
        .space 4, 0
        .space 4, 0 # pipe 5
        .space 4, 0
        .space 4, 0 # pipe 6
        .space 4, 0
        .space 4, 0 # pipe 7
        .space 4, 0

NUM_PROCS:
        .quad 9
NUM_PIPES:
        .quad 8
.text
.global main
main:
        call func_loop_pipe
        lea NUM_PROCS(%rip), %rax
        mov (%rax), %rdx

        xor %rcx, %rcx
        loop1:
                call func_fork
                cmp $0, %rax
                je child_process
                jne loop1_end_part
                child_process:
                        cmp $0, %rcx
                        je child_process_first
                        jne child_process_last_or_common
                        child_process_first:
                                call func_get_current_fds
                                call func_dup2_STDOUT
                                jp child_process_end
                        child_process_last_or_common:
                                mov %rdx, %rbx
                                dec %rbx
                                cmp %rbx, %rcx
                                je child_process_last
                                jne child_process_common
                                child_process_last:
                                        call func_get_current_fds
                                        sub $8, %rdi
                                        call func_dup2_STDIN
                                        jp child_process_end
                                child_process_common:
                                        call func_get_current_fds
                                        call func_dup2_STDOUT
                                        sub $8, %rdi
                                        call func_dup2_STDIN
                                        jp child_process_end
                child_process_end:
                        call func_loop_close
                        call func_get_current_command
                        
                        xor %rdx, %rdx
                        push %rdx
                        mov 0(%rdi), %rbx
                        mov (%rbx), %rcx
                        loop2:
                                mov %rdi, %rbx
                                mov $8, %rax
                                mul %rcx
                                add %rax, %rbx
                                push (%rbx)
                                dec %rcx
                                cmp $0, %rcx
                                jg loop2
                        push 8(%rdi)
                        pop %rdi
                        mov %rsp, %rsi
                        
                        call execve
                loop1_end_part:
                        inc %rcx
                        cmp %rdx, %rcx
                        jl loop1
        call func_loop_close
        call func_loop_wait
proc_finish: 
        mov $60, %rax
        mov $0, %rdi
        syscall
func_loop_pipe:
        push %rax
        push %rbx
        push %rcx
        push %rdx
        push %rdi
        push %rsi
        
        lea NUM_PIPES(%rip), %rax
        mov (%rax), %rdx
        lea fds(%rip), %rdi
        xor %rcx, %rcx
        loop_func_loop_pipe:
                call func_pipe
                add $8, %rdi
                inc %rcx
                cmp %rdx, %rcx
                jl loop_func_loop_pipe
        
        pop %rsi
        pop %rdi
        pop %rdx
        pop %rcx
        pop %rbx
        pop %rax
        ret
func_pipe:                      # accepts fds[i] in rdi
        push %rax
        push %rbx
        push %rcx
        push %rdx
        push %rdi
        push %rsi

        xor %rax, %rax
        mov $22, %al
        syscall

        pop %rsi
        pop %rdi
        pop %rdx
        pop %rcx
        pop %rbx
        pop %rax  
        ret
func_fork:                      # returns number of process in %rax
        push %rbx
        push %rcx
        push %rdx
        push %rdi
        push %rsi 

        xor %rax, %rax
        mov $57, %al
        syscall

        pop %rsi
        pop %rdi
        pop %rdx
        pop %rcx
        pop %rbx
        ret 
func_loop_close:
        push %rax
        push %rbx
        push %rcx
        push %rdx
        push %rdi
        push %rsi

        lea NUM_PIPES(%rip), %rax
        mov (%rax), %rdx
        lea fds(%rip), %rdi
        xor %rcx, %rcx
        loop_func_loop_close:
            call func_close_STDIN
            call func_close_STDOUT
            add $8, %rdi
            inc %rcx
            cmp %rdx, %rcx
            jl loop_func_loop_close

        pop %rsi
        pop %rdi
        pop %rdx
        pop %rcx
        pop %rbx
        pop %rax
        ret
func_close_STDIN:               # accepts fds[i] in %rdi
        push %rax
        push %rbx
        push %rcx
        push %rdx
        push %rdi
        push %rsi

        xor %rax, %rax
        mov $3, %al
        mov 0(%rdi), %ebx
        mov %rbx, %rdi
        syscall

        pop %rsi
        pop %rdi
        pop %rdx
        pop %rcx
        pop %rbx
        pop %rax
        ret
func_close_STDOUT:              # accepts fds[i] in %rdi
        push %rax
        push %rbx
        push %rcx
        push %rdx
        push %rdi
        push %rsi

        xor %rax, %rax
        mov $3, %al
        mov 4(%rdi), %ebx
        mov %rbx, %rdi
        syscall
        
        pop %rsi
        pop %rdi
        pop %rdx
        pop %rcx
        pop %rbx
        pop %rax
        ret
func_get_current_command:       # accepts i in %rcx; returns commands[i] in %rdi
        push %rax
        push %rbx
        push %rcx
        push %rdx
        

        mov $8, %rax
        mul %rcx
        lea commands(%rip), %rdi
        add %rax, %rdi
        mov (%rdi), %rax
        mov %rax, %rdi

        
        pop %rdx
        pop %rcx
        pop %rbx
        pop %rax
        ret
func_execve:                    # accepts command[0] in %rdi, command and 0 in %rsi
        push %rax
        push %rbx
        push %rcx
        push %rdx
        push %rdi
        push %rsi
        
        xor %rax, %rax
        mov $59, %al
        mov $0, %rdi
        syscall

        pop %rsi
        pop %rdi
        pop %rdx
        pop %rcx
        pop %rbx
        pop %rax
        ret
func_loop_wait:
        push %rax
        push %rbx
        push %rcx
        push %rdx
        push %rdi
        push %rsi

        lea NUM_PROCS(%rip), %rax
        mov (%rax), %rdx
        xor %rcx, %rcx
        loop_func_loop_wait:
                call func_wait
                inc %rcx
                cmp %rdx, %rcx
                jl loop_func_loop_wait

        pop %rsi
        pop %rdi
        pop %rdx
        pop %rcx
        pop %rbx
        pop %rax
        ret
func_wait:
        push %rax
        push %rbx
        push %rcx
        push %rdx
        push %rdi
        push %rsi
        
        mov $0, %rdi
        call wait
        
        pop %rsi
        pop %rdi
        pop %rdx
        pop %rcx
        pop %rbx
        pop %rax
        ret
func_get_current_fds:           # accepts i in %rcx; returns fds[i] in %rdi
        push %rax
        push %rbx
        push %rcx
        push %rdx
        

        mov $8, %rax
        mul %rcx
        lea fds(%rip), %rdi
        add %rax, %rdi

        
        pop %rdx
        pop %rcx
        pop %rbx
        pop %rax
        ret
func_dup2_STDOUT:               # accepts fds[i] in %rdi
        push %rax
        push %rbx
        push %rcx
        push %rdx
        push %rdi
        push %rsi

        xor %rax, %rax
        mov $33, %al
        mov 4(%rdi), %edx
        mov %rdx, %rdi
        xor %rsi, %rsi
        inc %rsi
        syscall

        pop %rsi
        pop %rdi
        pop %rdx
        pop %rcx
        pop %rbx
        pop %rax
        ret
func_dup2_STDIN:                # accepts fds[i] in %rdi
        push %rax
        push %rbx
        push %rcx
        push %rdx
        push %rdi
        push %rsi

        xor %rax, %rax
        mov $33, %al
        mov 0(%rdi), %edx
        mov %rdx, %rdi
        xor %rsi, %rsi
        syscall

        pop %rsi
        pop %rdi
        pop %rdx
        pop %rcx
        pop %rbx
        pop %rax
        ret