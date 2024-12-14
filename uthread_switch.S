.text

# Switch from current_thread to next_thread. Make next_thread
# the current_thread, and set next_thread to 0.
# Use eax as a temporary register; it is caller saved.

.globl thread_switch
thread_switch:
  # 1. Save context
  # Push all general-purpose x86 registers (eax, ecx, edx, ebx, esp, ebp, esi, edi) onto the stack
  pushal

  # 2. Save stack pointer of the current_thread
  # Save current_thread into %eax
  movl current_thread, %eax
  # Save current stack pointer into current_thread->sp
  movl %esp, (%eax)

  # 3. Change stack pointer to stack pointer of the next_thread
  # Copy next_thread into %eax
  movl next_thread, %eax
  # Save next_thread->sp in %esp
  movl (%eax), %esp

  # 4. Update the current_thread pointer to point to the next_thread
  movl %eax, current_thread

  # 5. Restore context
  # Pop all general-purpose x86 registers
  popal

  # 6. Pop return address from stack
  ret
