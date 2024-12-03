// .data section is used to declare constants.
.section .data
    // The filename of the input data. This cannot be changed, as the file is in
    // the git repo, so should always be present. This also represents the
    // relative path to the file from where the program will be running. 
    filename: .asciz "input.txt"
    // The path separator used to join the input file name to the directory.
    path_separator: .asciz "/"
    // Newline character used for printing text.
    newline: .asciz "\n"
    // Reserve 15 bytes of memory as a buffer to read lines from the input file
    // into.
    line_buffer: .space 15

// .text section contains the instructions for the program.
.section .text
    // Declare _start as a global label, so it can be referenced from other
    // files. This is used as the entry point for the program. 
    .global _start

// Define return codes.
.equ SUCCESS, 0             // Program ran successfully.
.equ ERR_OPENING_FILE, 1    // An error occurred opening the input file.
.equ ERR_READING_FILE, 2    // An error occurred reading the input file.

// Program execution starts here.
// NOTE: I use a lot of syscalls in this program, and used this website as a
// reference: https://arm64.syscall.sh/.
_start:
    // Open the input file.
    //
    // This uses the openat syscall, which has the following arguments:
    // - x0: Directory file descriptor.
    // - x1: Path to the file.
    // - x2: Flags to open the file with.
    // - x3: (Optional) File mode (permissions) if file is created.
    mov x0, -100            // Set directory to cwd (AT_FDCWD).
    ldr x1, =filename       // Use the file name as the relative path from cwd.
    mov x2, 0               // Open the file with read-only (O_RDONLY) flag.
    mov x8, 56              // Move the syscall number for openat into x8.
    svc 0                   // Trigger a supervisor call for the syscall in x8.
    mov x19, x0             // Save the file descriptor to x19.

    // Check for any errors from the syscall.
    cmp x0, 0               // Compare the result in x0 with 0.
    b.lt file_open_error    // if x0 < 0, branch to the file_open_error label.

// Internal loop to read the file line by line.
read_line:
    // This uses the read syscall, which has the following arguments:
    // - x0: File descriptor.
    // - x1: Buffer to store the data to.
    // - x2: Number of bytes to read.
    //
    // Returns:
    // - x0: The number of bytes read on success.
    // - x0: A negative value on error.
    mov x0, x19             // Set file descriptor to saved output of openat.
    ldr x1, =line_buffer    // Write the data to the line_buffer.
    mov x2, 15              // Each line of the input is 15 bytes long.
    mov x8, 63              // Move the syscall number for read into x8.
    svc 0                   // Trigger a supervisor call for the syscall in x8.

    mov x0, -1

    // Check the status of the read syscall.
    cmp x0, 0               // Check if the return from read syscall against 0.
    b.lt file_read_error    // if x0 < 0, an error occurred.
    beq close_file          // if x0 == 0, the end of file was reached.

    // Print the line from the file for testing.
    //
    // This uses the write syscall, which has the following arguments:
    // - x0: File descriptor to write to.
    // - x1: Pointer to the buffer of data to write.
    // - x2: Number of characters from buffer to write.
    mov x0, 1               // Load address of stdout file descriptor into x0.
    ldr x1, =line_buffer    // Use the line_buffer as the data to write.
    mov x2, 15              // Write the entire contents of line_buffer.
    mov x8, 64              // Move the syscall number for write into x8.
    svc 0                   // Trigger a supervisor call for the syscall in x8.

    b read_line             // Loop to read the next line.

// Close the file after reading the entire contents.
close_file:
    // This uses the close syscall, which has the following arguments:
    // - x0: File descriptor.
    //
    // Returns:
    // - x0: 0 on success.
    // - x0: -1 on error.
    mov x0, x19             // Set file descriptor to saved output of openat.
    mov x8, 57              // Move the syscall number for close into x8.
    svc 0                   // Trigger a supervisor call for the syscall in x8.

    // The program has executed successfully if it reaches here.
    mov x0, SUCCESS         // Load success return code into x0 register.
    mov x8, 93              // Move the syscall number for exit (93) into x8.
    svc 0                   // Trigger a supervisor call to exit.

// Exit the program with the ERR_READING_FILE error code.
file_read_error:
    mov x0, ERR_READING_FILE// Move the ERR_READING_FILE error code into x0.
    b error  

// Exit the program with the ERR_OPENING_FILE error code.
file_open_error:
    mov x0, ERR_OPENING_FILE// Move the ERR_OPENING_FILE error code into x0.
    b error                 // Branch to the error label to exit with the error.

// In the event of an error, exit with a specified error code.
//
// Arguments:
// - x0: Error code to return.
error:
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
