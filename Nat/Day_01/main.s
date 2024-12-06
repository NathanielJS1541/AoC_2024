// .data section is used to declare constants.
.section .data
    // The filename of the input data. Maybe this should be passed in as an
    // argument...
    filename: .asciz "input.txt"
    // Newline character used for printing text.
    newline: .asciz "\n"
    // Reserve 15 bytes of memory as a buffer to read lines from the input file
    // into.
    line_buffer: .space 15
    left_column: .space 4000
    right_column: .space 4000

// .text section contains the instructions for the program.
.section .text
    // Declare _start as a global label, so it can be referenced from other
    // files. This is used as the entry point for the program.
    .global _start

// -----------------------------------------------------------------------------
// |                          Return Code Definitions                          |
// -----------------------------------------------------------------------------

// Define return codes.
.equ SUCCESS, 0             // Program ran successfully.
.equ ERR_OPENING_FILE, 1    // An error occurred opening the input file.
.equ ERR_READING_FILE, 2    // An error occurred reading the input file.

// -----------------------------------------------------------------------------
// |                           Function Definitions                            |
// -----------------------------------------------------------------------------

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
    add x2, x2, #1          // Increment the address in x2 to move to next byte.
    b find_string_end       // Branch to find_string_end to loop.
    // Inner loop to append string at x1 onto x0.
append:
    ldrb w3, [x1]           // Load byte at address x1 into w3.
    strb w3, [x2]           // Store byte in w3 to x0 string end (x2).
    cbz w3, concat_done     // Check if the byte in w3 is null. If so, return.
    add x1, x1, #1          // Move to next byte in x1 string.
    add x2, x2, #1          // Move to next byte after end of x0 string.
    b append                // Branch to append to loop.
    // Return from the function.
concat_done:
    ret

// In the event of an error, exit with a specified error code.
//
// Arguments:
// - x0: Error code to return.
error:
    mov x8, #93             // Move the syscall number for exit (93) into x8.
    svc #0                  // Trigger a supervisor call to exit.

// Exit the program with the ERR_OPENING_FILE error code.
file_open_error:
    mov x0, ERR_OPENING_FILE// Move the ERR_OPENING_FILE error code into x0.
    b error                 // Branch to the error label to exit with the error.

// Exit the program with the ERR_READING_FILE error code.
file_read_error:
    mov x0, ERR_READING_FILE// Move the ERR_READING_FILE error code into x0.
    b error                 // Branch to the error label to exit with the error.

// Function to convert one of the input numbers in string format to (unsigned)
// integer format. For simplicity sake, this assumes that the number is from the
// input, and is therefore 5 ASCII characters (and therefore bytes) long.
//
// Note: This will increment the pointer at x1 by 4!
//
// Arguments:
// - x1: A pointer to the first digit of the number.
//
// Returns:
// - w3: The 32-bit uint representation of the number string.
input_string_to_uint:
    mov w3, #0              // initialize result to 0
    mov w5, #10             // load immediate value 10 into w6
    mov w6, #5              // Initialise loop variable to 5.

    // Loop through each byte that needs to be converted.
conversion_loop:
    ldrb w4, [x1]           // Load a character from the number into w4.
    sub w4, w4, #48         // Subtract the value of an ASCII 0 from the char.
    mul w3, w3, w5          // Multiply the previous result by 10.
    add w3, w3, w4          // Add the value of the ASCII char to the result.
    add x1, x1, #1          // Move to the next character.
    subs w6, w6, #1         // Subtract 1 from the loop counter and set flags.
    bne conversion_loop     // Restart loop if counter in w6 > 0.

    // Return after the conversion loop exits, as the desired return is already
    // in the w3 register.
    ret

// Function to parse a single line of the input file. The input lines are always
// in the following format:
// - XXXXX   YYYYY\r\n
// Where XXXXX and YYYYY represent two unsigned integers, separated by 3 spaces.
// Each line is also terminated by CRLF.
//
// Knowing this, we can take some shortcuts when parsing the text file as the
// format is consistent:
// - Each number starts at a fixed location, and is a fixed length, so we can
//   hard-code these locations when parsing rather than trying to detect them at
//   runtime.
// - Each number is between 10000 and 99999, so can be stored in 32 bits.
//
// This will add the fist number in the line to left_column, and the second
// number in the line to right_column.
//
// Arguments:
// - x1: Pointer to the line to parse.
//
// Returns:
// - w2: The 32-bit uint representing the first number on the line.
// - w3: The 32-bit uint representing the second number on the line.
parse_line:
    // Since this function uses branches with links, store the state of the
    // frame pointer (x29) and link register (x30) so they can be restored
    // later.
    //
    // These are stored on top of the stack by taking the current stack pointer
    // (sp), decrementing it by 16 bytes before the store instruction (using
    // pre-indexing denoted by !), and using the stack pointer value to store
    // the registers to.
    stp x29, x30, [sp, #-16]!

    // x0 currently points to the start of the line to parse, so it is already
    // a pointer to the start of the first number.
    bl input_string_to_uint   // Convert the first number to a uint in w3.
    mov w2, w3                // Since this is the first number, store it in w2.

    // The x0 register pointer has been incremented by 4 to the end of the first
    // number by the input_string_to_uint function. To get to the start of the
    // second number, increment x0 by 3.
    add x1, x1, 3

    // x0 now points to the start of the second number to parse.
    bl input_string_to_uint   // Convert the second number to a uint in w3.
    // w3 is the return register for the second number, so it does not need to
    // be moved.

    // Before returning, load the original frame pointer (x29) and link register
    // (x30) states back so that the ret functions as expected.
    //
    // The values are taken from the top of the stack, and the stack pointer
    // value is then incremented using a post-index offset to "free" the memory.
    ldp x29, x30, [sp], #16
    ret

// Function implementing Hoare's Partition Algorithm using a "median of three"
// pivot selection if there are enough elements in the array segment.
//
// Arguments:
// - x0: Pointer to first element of the array segment.
// - x1: Pointer to the last element of the array segment.
//
// Returns:
// - x0: Pointer to the array element that splits the next partition.
partition:
    // TODO: cmp to x9 stores (number of elements - 1) * 4...
    sub x9, x1, x0          // Calculate offset between start and end pointer.
    cmp x9, #41             // Compare the number of elements with 41.
    ble partition_setup     // Use first element as pivot if elements < 10.

    // Calculate a pointer to the middle element.
    lsr x9, x9, #1          // Shift x9 to the right to divide by 2.
    and x9, x9, #-4         // Ensure offset is a multiple of 4 for alignment.
    add x9, x9, x0          // Add start pointer & offset to get middle element.

    // Dereference pointers to get the values of the start, middle and end.
    ldr w10, [x0]           // w10 = first element.
    ldr w11, [x9]           // w11 = middle element.
    ldr w12, [x1]           // w12 = last element.

    // Using the median element as the pivot provides the best time complexity.
    // This is way overkill for an AoC challenge, but I can so I will :).

    // Find the median value of w10, w11, and w12 to get the median "pivot". The
    // final pivot value will be stored in w13.
    cmp w10, w11            // Compare w10 and w11.
    csel w13, w10, w11, lt  // w13 = min(w10, w11).
    csel w9, w11, w10, lt   // w9 = max(w10, w11).

    cmp w9, w12             // Compare w9 and w12.
    csel w10, w9, w12, lt   // w10 = min(max(w10, w11), w12).
    csel w11, w12, w9, lt   // w11 = max(max(w10, w11), w12).

    cmp w13, w10            // Compare min(w10, w11) and min(max(w10, w11), w12).
    csel w12, w13, w10, gt  // w12 = max(min(w10, w11), min(max(w10, w11), w12)).

    // Finally, the median is the smallest between the following values:
    // - w11 = max(max(w10, w11), w12).
    // - w12 = max(min(w10, w11), min(max(w10, w11), w12)).
    cmp w12, w11
    csel w13, w12, w11, lt  // w13 = median(w10, w11, w12).

    // Hoare's partition algorithm requires the pivot to be at the first element
    // of the array, so swap the median element and the first element.
    str w13, [x0]           // Always overwrite first element for simplicity.
    cmp w13, w10            // Compare the median value with the first element.
    b.eq partition_setup    // Skip swap if the first element is the median.
    cmp w13, w11            // Compare the median value with the middle element.
    b.eq swap_middle_element// Swap first and middle element.
    str w10, [x1]           // Otherwise, swap first and last element.
    b partition_setup       // Go to the partition setup.

swap_middle_element:
    str w10, [x4]           // Swap first and middle element.

partition_setup:
    // Load the pivot (first array element) into w9. This is done regardless of
    // whether the median of three pivot selection method was used or not, as
    // the pivot is always moved to the first array element. If there are too
    // few elements to perform the median of three pivot selection, this will
    // just use the first element.
    ldr w9, [x0]            // w9 = selected pivot.

    // Load the pointers to the start and end of the array segment into scratch
    // registers to perform Hoare's partitioning algorithm.
    //
    // Not that the pointers are started outside of the array range (by
    // subtracting 4 from the left pointer and adding 4 to the right pointer) to
    // simplify the loop.
    sub x10, x0, #4         // x10 = left pointer (first element).
    add x11, x1, #4         // x11 = right pointer (last element).

    // Internal loop over the entire array segment.
partition_loop:
    add x10, x10, #4        // Move left pointer to the right.
    ldr w12, [x10]          // Load the value at the left pointer into w12.
    cmp w12, w9             // Compare the left pointer value to the pivot.
    ble partition_loop      // Restart the loop if the value < pivot.

    // Internal loop to move the right pointer to the left until the value is
    // less than the pivot.
right_pointer_loop:
    sub x11, x11, #4        // Move right pointer to the left.
    ldr w13, [x11]          // Load the value at the left pointer into w13.
    cmp w13, w9             // Compare the right pointer value to the pivot.
    bge right_pointer_loop  // Restart the loop if the value > pivot.

    // Check if the left and right pointer have crossed over.
    cmp x10, x11            // Compare the pointer values.
    bge partition_done      // Exit the loop if left pointer > right pointer.

    // If this point is reached, then the value at the left pointer is greater
    // than the pivot, and the value at the right pointer is less than the
    // pivot. Swap them to get them on the correct side of the pivot.
    str w13, [x10]          // Right pointer value -> left pointer.
    str w12, [x11]          // Left pointer value -> right pointer.
    b partition_loop        // Restart the loop until all data is processed.

partition_done:
    // Return the position of the right pointer as the partition index for the
    // next call to this method x0.
    mov x0, x11
    ret

// Function implementing the quick sort algorithm.
//
// Arguments:
// - x0: Pointer to the start of the array to sort.
// - x1: Size of the array to sort.
quicksort:
    // Since this function uses branches with links, store the state of the
    // frame pointer (x29) and link register (x30) so they can be restored
    // later.
    //
    // These are stored on top of the stack by taking the current stack pointer
    // (sp), decrementing it by 16 bytes before the store instruction (using
    // pre-indexing denoted by !), and using the stack pointer value to store
    // the registers to.
    stp x29, x30, [sp, #-16]!

    // Store the current values of the used callee-saved registers that will be
    // used to the stack, so they can be reset on return. Note that 8 extra
    // bytes of stack are allocated to store x21, as the stack must stay 16-byte
    // aligned.
    stp x19, x20, [sp, #-16]!
    sub sp, sp, #16         // Allocate another 16 bytes on the stack.
    str x21, [sp, #8]       // Store x21 at sp + 8 on the stack.

    // Store the array pointer (x0) and the size of the array (x1) into
    // callee-saved registers.
    mov x19, x0             // x19 = stored array start pointer.
    mov x20, x1             // x20 = stored array size.

    // Prepare the arguments to call the partition method.
    // Note that x0 already contains a pointer to the start of the array.
    add x1, x0, x1          // x1 = array + size (end of array).
    sub x1, x1, #4          // x1 = end of array - 4 (last element).

    // Partition the array segment contained between x0 and x1. Store the
    // returned partition pointer in the callee-saved register x21.
    bl partition
    mov x21, x0             // x21 = partition pointer.

    // Prepare arguments to call quicksort on the left array segment.
    // TODO: Which partition should contain the partition pointer?
    mov x0, x19             // Array start pointer = stored array pointer.
    sub x1, x21, x19        // Size = partition pointer - array start.
    bl quicksort            // Sort left array segment.

    // TODO: Check if the element at the partition pointer is sorted twice...

    // Prepare arguments to call quicksort on the right array segment.
    mov x0, x21             // Array start pointer = partition pointer (+4?).
    add x1, x21, x19        // x1 = left segment size (partition - start).
    sub x1, x20, x1         // Size = array size - left segment size.
    bl quicksort            // Sort right array segment.

    // Restore previous callee-saved register states from the stack.
    ldr x21, [sp, #8]       // Load x21 from sp + 8 on the stack.
    add sp, sp, #16         // Deallocate 16 bytes from the stack.
    ldp x19, x20, [sp], #16

    // Before returning, load the original frame pointer (x29) and link register
    // (x30) states back so that the ret functions as expected.
    //
    // The values are taken from the top of the stack, and the stack pointer
    // value is then incremented using a post-index offset to "free" the memory.
    ldp x29, x30, [sp], #16
    ret

// -----------------------------------------------------------------------------
// |                               Main Program                                |
// -----------------------------------------------------------------------------

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
    mov x0, #-100           // Set directory to cwd (AT_FDCWD).
    ldr x1, =filename       // Use the file name as the relative path from cwd.
    mov x2, #0              // Open the file with read-only (O_RDONLY) flag.
    mov x8, #56             // Move the syscall number for openat into x8.
    svc #0                  // Trigger a supervisor call for the syscall in x8.
    mov x19, x0             // Save the file descriptor to x19.

    // Check for any errors from the syscall.
    cmp x0, #0              // Compare the result in x0 with 0.
    b.lt file_open_error    // if x0 < 0, branch to the file_open_error label.

    // Initialise pointers to the start of the output arrays.
    ldr x20, =left_column
    ldr x21, =right_column

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
    mov x2, #15             // Each line of the input is 15 bytes long.
    mov x8, #63             // Move the syscall number for read into x8.
    svc 0                   // Trigger a supervisor call for the syscall in x8.

    // Check the status of the read syscall.
    cmp x0, #0              // Check if the return from read syscall against 0.
    b.lt file_read_error    // if x0 < 0, an error occurred.
    beq close_file          // if x0 == 0, the end of file was reached.

    // Re-set the x1 register to the start of the line_buffer.
    ldr x1, =line_buffer
    // Parse the current line into two numbers in the w2 and w3 registers.
    bl parse_line

    // Store the two numbers in the respective columns, using a post-index
    // offset of 4 to move the pointer to accept the next number.
    str w2, [x20], #4
    str w3, [x21], #4

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
    mov x8, #57             // Move the syscall number for close into x8.
    svc #0                  // Trigger a supervisor call for the syscall in x8.

sort_arrays:
    ldr x0, =left_column    // Load the address of the array into x0
    mov x1, #40             // Load the size of the array into x1
    //ldr x1, =size   // Load the size of the array into x1
    //ldr x1, [x1]    // Dereference to get the actual size

    // Call quicksort
    bl quicksort

    // The program has executed successfully if it reaches here.
    mov x0, SUCCESS         // Load success return code into x0 register.
    mov x8, #93             // Move the syscall number for exit (93) into x8.
    svc #0                  // Trigger a supervisor call to exit.
