// .data section is used to declare constants.
.section .data
    // The filename of the input data. This cannot be changed, as the file is in
    // the git repo, so should always be present.
    filename: .asciz "input.txt"
    // Reserve 256 bytes of memory as a buffer to store the file path to the
    // input file.
    path_buffer: .space 256
    // The path separator used to join the input file name to the directory.
    path_separator: .asciz "/"
    // Newline character used for printing text.
    newline: .asciz "\n"

// .text section contains the instructions for the program.
.section .text
    // Declare _start as a global label, so it can be referenced from other
    // files. This is used as the entry point for the program. 
    .global _start

// Program execution starts here.
// NOTE: I use a lot of syscalls in this program, and used this website as a
// reference: https://arm64.syscall.sh/.
_start:
    // Get the current working directory that the program was executed in.
    //
    // This uses the getcwd syscall, which has the following arguments:
    // - x0: Pointer to the buffer to store the path in.
    // - x1: Size of the buffer.
    ldr x0, =path_buffer    // Load the address of path_buffer into x0.
    mov x1, 256             // Move the size of the path_buffer (256) into x1.
    mov x8, 17              // Move the syscall number for getcwd into x8.
    svc 0                   // Trigger a supervisor call for the syscall in x8.

    // Check for any errors from the syscall.
    cmp x0, 0               // Compare the result in x0 with 0.
    b.lt error              // if x0 < 0, branch to the error label.

    // Append a path separator to the cwd path.
    ldr x0, =path_buffer    // Load the address of the path_buffer into x0.
    ldr x1, =path_separator // Load the address of the path_separator into x1.
    bl concat_strings       // Call concat_strings to add the path separator.

    // Append the file name to the cwd path.
    // path_buffer is already in x0 from adding the path separator.
    ldr x1, =filename       // Load the address of the file name into x1.
    bl concat_strings       // Call concat_strings to add the file name.

    // Print the file path for testing.
    //
    // This uses the write syscall, which has the following arguments:
    // - x0: File descriptor to write to.
    // - x1: Pointer to the buffer of data to write.
    // - x2: Number of characters from buffer to write.
    mov x0, 1               // Load address of stdout file descriptor into x0.
    ldr x1, =path_buffer    // Use the path_buffer as the data to write.
    mov x2, 256             // Write the entire contents of path_buffer.
    mov x8, 64              // Move the syscall number for write into x8.
    svc 0                   // Trigger a supervisor call for the syscall in x8.

    // Print the newline character, so that the terminal prompt appears on a new
    // line after the program finishes.
    //
    // This uses the write syscall, which has the following arguments:
    // - x0: File descriptor to write to.
    // - x1: Pointer to the buffer of data to write.
    // - x2: Number of characters from buffer to write.
    mov x0, 1               // Load address of stdout file descriptor into x0.
    ldr x1, =newline        // Use the newline character as the data to write.
    mov x2, 1               // Write the single newline character.
    mov x8, 64              // Move the syscall number for write into x8.
    svc 0                   // Trigger a supervisor call for the syscall in x8.

    // The program has executed successfully if it reaches here.
    mov x0, 0               // Load 0 into x0 register to indicate success.
    mov x8, 93              // Move the syscall number for exit (93) into x8.
    svc 0                   // Trigger a supervisor call to exit.

error:
    // In the event of an error, return the error code as the exit code.
    //
    // This assumes that the error code has been loaded into x0.
    mov x8, 93              // Move the syscall number for exit (93) into x8.
    svc 0                   // Trigger a supervisor call to exit.

// Function to join two strings together.
//
// Arguments:
// - x0: Pointer to the string to append to.
// - x1: Pointer to the string to concatenate on to x0.
concat_strings:
    // Copy the pointer to the string in x0 to the x2 register. This will
    // eventually be used to point to the end of the string in x0.
    mov x2, x0
    // Inner loop to find the end of the string in x0.
find_string_end:
    // Load the byte at the location the x2 register is pointing at into w3.
    ldrb w3, [x2]
    // Check if the byte is a null terminator (0). If it is, branch to the
    // append label.
    cbz w3, append
    add x2, x2, 1           // Increment the address in x2 to move to next byte.
    b find_string_end       // Branch to find_string_end to loop.
    // Inner loop to append string at x1 onto x0.
append:
    ldrb w3, [x1]           // Load byte at address x1 into w3.
    strb w3, [x2]           // Store byte in w3 to x0 string end (x2).
    cbz w3, concat_done     // Check if the byte in w3 is null. If so, return.
    add x1, x1, 1           // Move to next byte in x1 string.
    add x2, x2, 1           // Move to next byte after end of x0 string.
    b append                // Branch to append to loop.
    // Return from the function.
concat_done:
    ret
