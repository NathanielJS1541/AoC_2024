// -----------------------------------------------------------------------------
// |                                 Preamble                                  |
// -----------------------------------------------------------------------------
//
// Throughout this program I have tried to adhere to the AArch64 procedure call
// standard:
//
// For more information, see the AArch64 procedure call standard at
// https://github.com/ARM-software/abi-aa/blob/main/aapcs64/aapcs64.rst.
//
// - x0-x7: Function arguments and return values.
// - x8: Indirect result location register. Often used as syscall number.
// - x9-x15: Temporary "scratch" registers, not preserved across function calls.
// - x16-x17: Intraprocedure-call temporary registers.
// - x18: Platform register if needed, or scratch register.
// - x19-x28: Callee-saved registers, preserved across function calls.
// - x29: Frame pointer (FP), used to point to the current stack frame.
// - x30: Link register (LR), stores function return address.

// -----------------------------------------------------------------------------
// |                                 Constants                                 |
// -----------------------------------------------------------------------------

// Define return codes.
.equ SUCCESS, 0             // Program ran successfully.
.equ ERR_ARGS, 1            // Incorrect arguments were used with the program.
.equ ERR_OPENING_FILE, 2    // An error occurred opening the input file.
.equ ERR_READING_FILE, 3    // An error occurred reading the input file.

// Syscall number definitions. See https://arm64.syscall.sh/.
.equ SYS_OPENAT, 56         // Syscall number for "openat".
.equ SYS_CLOSE, 57          // Syscall number for "close".
.equ SYS_READ, 63           // Syscall number for "read".
.equ SYS_WRITE, 64          // Syscall number for "write".
.equ SYS_EXIT, 93           // Syscall number for "exit".

// Syscall argument definitions.
.equ SVC_INVOKE, 0          // Used as the value when invoking supervisor calls.
.equ STDOUT, 1              // File descriptor for standard output.
.equ DFD_AT_FDCWD, -100     // The directory file descriptor for the cwd.
.equ O_RDONLY, 0            // Open the file as read only.

// Program constants.
.equ INPUT_SIZE, 4000       // The number of bytes in each input data column.
.equ LINE_BUFFER_SIZE, 15   // The number of bytes in the line buffer.
.equ PRINT_BUFFER_SIZE, 32  // The number of bytes to use as a print buffer.

// -----------------------------------------------------------------------------
// |                         Initialised "Static" Data                         |
// -----------------------------------------------------------------------------

.section .data
    // Align the data section to 16-byte boundaries.
    .align 4                // Align to 2^4 = 16 bytes.
    // The expected option to signify that the file name is being passed in to
    // the program.
    file_option: .asciz "-f"
    // Flag to get the program to print the usage information.
    help_flag: .asciz "-h"
    // Message strings, along with the lengths of the strings calculated at
    // assembly time.
    //
    // The string lengths are calculated using the difference between the
    // current location counter (.) and the start of the string. Note that this
    // includes the null terminator in the length of the string.
    success_string: .asciz "The program ran successfully.\n"
    .equ success_string_len, . - success_string
    err_file_open_string: .asciz "Failed to open the input file.\n"
    .equ err_file_open_string_len, . - err_file_open_string
    err_file_read_string: .asciz "Failed to read the input file.\n"
    .equ err_file_read_string_len, . - err_file_read_string
    distance_prefix: .asciz "Calculated a total distance of: "
    .equ distance_prefix_len, . - distance_prefix
    similarity_prefix: .asciz "Calculated a similarity score of: "
    .equ similarity_prefix_len, . - similarity_prefix
    output_suffix: .asciz ".\n"
    .equ output_suffix_len, . - output_suffix
    invalid_arguments: .asciz "Error: Invalid arguments provided.\n"
    .equ invalid_arguments_len, . - invalid_arguments
    program_usage: .asciz "Usage: ./main -f <input_file>\n"
    .equ program_usage_len, . - program_usage

// -----------------------------------------------------------------------------
// |                            Uninitialised Data                             |
// -----------------------------------------------------------------------------
.section .bss
    // Buffers are included in the .bss section to ensure they do not increase
    // the binary size, since they do not need to be initialised.
    //
    // Align the Block Started by Symbol section to 16-byte boundaries.
    .align 4                // Align to 2^4 = 16 bytes.
    // Buffers used to store and manipulate the entire dataset.
    left_column: .skip INPUT_SIZE
    right_column: .skip INPUT_SIZE
    // Reserve 15 bytes of memory as a buffer to read lines from the input file
    // into.
    line_buffer: .skip LINE_BUFFER_SIZE
    // Reserve 32 bytes in memory for creating dynamic output messages.
    print_buffer: .skip PRINT_BUFFER_SIZE

// -----------------------------------------------------------------------------
// |                               Program Data                                |
// -----------------------------------------------------------------------------

.section .text
    // Declare _start as a global label, so it can be referenced from other
    // files. This is used as the entry point for the program.
    .global _start

// -----------------------------------------------------------------------------
// |                               Main Program                                |
// -----------------------------------------------------------------------------

// Program execution starts here.
_start:
    // Parse the command line arguments for later use in the program.
    //
    // This will attempt to parse the arguments and return a success code in x0,
    // and if one was provided a pointer to the file name in x1. If the supplied
    // arguments are invalid x0 will contain a non-zero value.
    mov x0, sp              // Load initial stack pointer value as argument x0.
    bl parse_arguments      // Call function to parse arguments.

    // Handle the return status of the parse_arguments function.
    cbnz x0, open_input     // If file path is returned, open it.

    // If the input file path was not provided, exit the program with the
    // success code (if help was requested) or error code (if argument parsing
    // failed) from the return of parse_arguments.
    mov x0, x1              // Move return code to x0.
    b exit                  // Exit the program with the error code.

open_input:
    // Command line arguments were successfully parsed. Open input file.
    //
    // This uses the openat syscall, which has the following arguments:
    // - x0: Directory file descriptor.
    // - x1: Path to the file.
    // - x2: Flags to open the file with.
    // - x3: (Optional) File mode (permissions) if file is created.
    mov x1, x0              // Move file path from parse_arguments return to x1.
    mov x0, DFD_AT_FDCWD    // Set directory to cwd (AT_FDCWD).
    mov x2, O_RDONLY        // Open the file with read-only (O_RDONLY) flag.
    mov x8, SYS_OPENAT      // Move the syscall number for openat into x8.
    svc SVC_INVOKE          // Trigger a supervisor call for the syscall in x8.
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
    mov x2, LINE_BUFFER_SIZE// Read enough data to fill the buffer.
    mov x8, SYS_READ        // Move the syscall number for read into x8.
    svc SVC_INVOKE          // Trigger a supervisor call for the syscall in x8.

    // Check the status of the read syscall.
    cmp x0, #0              // Check if the return from read syscall against 0.
    b.lt file_read_error    // if x0 < 0, an error occurred.
    beq close_file          // if x0 == 0, the end of file was reached.

    // Set the x0 register to the start of the line_buffer.
    ldr x0, =line_buffer
    // Parse the current line into two numbers in the w0 and w1 registers.
    bl parse_line

    // Store the two numbers in the respective columns, using a post-index
    // offset of 4 to move the pointer to accept the next number.
    str w0, [x20], #4
    str w1, [x21], #4

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
    mov x8, SYS_CLOSE       // Move the syscall number for close into x8.
    svc SVC_INVOKE          // Trigger a supervisor call for the syscall in x8.

    // Ignore any errors, as all required data has already been loaded from the
    // file, and subsequent close syscalls would likely fail in the same way.

sort_arrays:
    // Work out the maximum array index for both arrays and save it to a
    // callee-saved register, since both arrays are the same size.
    mov x22, INPUT_SIZE     // x22 = size of column arrays in bytes.
    lsr x22, x22, #2        // Divide x22 by 4 to get maximum index (1-based).
    sub x22, x22, #1        // Max index - 1 as it needs to be 0-based.

    // Call the quicksort function on the left data column.
    ldr x0, =left_column    // x0 = address of the array.
    mov x1, #0              // x1 = array start index.
    mov x2, x22             // x2 = max array index.
    bl quicksort            // Call quicksort function with arguments prepared.

    // Call the quicksort function on the right data column.
    ldr x0, =right_column   // Load the address of the array into x0.
    mov x1, #0              // x1 = array start index.
    mov x2, x22             // x2 = max array index.
    bl quicksort            // Call quicksort function with arguments prepared.

    // Initialise loop variables for the distance_loop.
    // Note that the distance sum is stored in a callee-saved register.
    mov x9, INPUT_SIZE      // x9 = size of column arrays in bytes.
    lsr x9, x9, #2          // Divide x9 by 4 to get maximum index (1-based).
    sub x9, x9, #1          // Max index - 1 as it needs to be 0-based.
    mov x23, #0             // Zero x23 to store the total sum.
    ldr x11, =left_column   // Pointer to the left_column array.
    ldr x12, =right_column  // Pointer to the right_column array.

    // Internal loop to iterate over each element in both arrays, and work out
    // the cumulative sum of distances between them.
    //
    // Note that this loops backwards for speed™!
distance_loop:
    // Load the two values at the current array indexes into w13 and w14.
    ldr w13, [x11, x9, lsl #2] // Load value from left_column @ index into w13.
    ldr w14, [x12, x9, lsl #2] // Load value from right_column @ index into w14.

    // Get the absolute "distance" between the two numbers.
    //
    // csneg selects the first source register if the condition is true, or
    // negates the second source register and selects it if the condition is
    // false.
    //
    // Using the "plus" (pl) condition, the first register is selected if the
    // subtraction left the negative flag clear. Otherwise (in the case the
    // subtraction resulted in a negative value) the x13 register is negated and
    // returned. This ensures x13 will always be positive.
    subs x13, x13, x14      // Subtract one value from another and set flags.
    csneg x13, x13, x13, pl // Keep x13, or negate it if it is negative.

    add x23, x23, x13       // Add the "distance" to the cumulative total.

    subs x9, x9, #1         // Decrement the array index (and set flags).
    b.ge distance_loop      // Restart loop if index in x9 >= 0.

    // Print the calculated distance score.
    ldr x0, =distance_prefix // x0 = pointer to the distance prefix.
    ldr x1, =distance_prefix_len // x1 = distance prefix string length.
    mov x2, x23             // x2 = calculated distance value.
    bl print_output_value   // Print the prefix text and output value.

    // Prepare registers to calculate the "similarity" between the two lists.
    //
    // Load them into callee-saved registers to preserve their states.
    mov x19, #0             // x19 = total similarity score.
    ldr x20, =left_column   // x20 = pointer to left array.
    ldr x21, =right_column  // x21 = pointer to right array.
    // Initialise the indexes to -1 to simplify the loops.
                             // x22 = left segment start index.
    mov x23, #-1             // x23 = left segment end index.
                             // x24 = right segment start index.
    mov x25, #-1             // x25 = right segment end index.
    // Calculate maximum array index.
    mov x26, INPUT_SIZE     // x26 = size of column arrays in bytes.
    lsr x26, x26, #2        // Divide x26 by 4 to get maximum index (1-based).
    sub x26, x26, #1        // Max index - 1 as it needs to be 0-based.

    // Internal loop to calculate the "similarity" between the two lists.
    //
    // Since the lists are already in order, each of the same number will occur
    // in sequence.
similarity_loop:
    add x22, x23, #1        // Move start to after the previous segment end.
    cmp x22, x26            // Compare new index to max index.
    b.hi similarity_loop_finished // Exit loop if array end reached.

    // Find the end index of the current left segment.
    mov x0, x20             // x0 = pointer to left array.
    mov x1, x22             // x1 = left segment start index.
    mov x2, x26             // x2 = maximum array index.
    bl get_uniform_segment  // Find the index of the end of the segment.
    mov x23, x0             // Store the index of the end of the segment.

    // Prepare to find the matching segment in the right array.
    ldr w9, [x20, x22, lsl #2] // Load the value at current left start index.
    mov x24, x25            // Move start to the previous segment end.

    // Internal loop to identify the start of the uniform segment in the right
    // array that matches the segment in the left array.
match_start_index:
    add x24, x24, #1        // Increment the right index start pointer.
    cmp x24, x26            // Compare new index to max index.
    b.hi similarity_loop_finished // Exit loop if array end reached.
    ldr w10, [x21, x24, lsl #2] // Load the value at current right start index.
    cmp w9, w10             // Compare the values at the start array indexes.
    b.hi match_start_index  // If left > right, go to next right index.

    // If this point is reached, the current value at the right start index is
    // either greater than or equal to the value in the left uniform segment.
    //
    // - If the right value is greater, then the value in the uniform left
    //   segment doesn't occur in the right array. The next uniform segment in
    //   the left array should be identified and the right end index should be
    //   updated to the current start index - 1, so that the next loop iteration
    //   starts at the current position.
    // - If the right value is equal, then the two arrays share a segment with a
    //   uniform value. The end of this segment should be identified.

    b.eq find_right_segment // If the value is equal, find the segment end.

    // The right value is greater, so prepare the right end index for the next
    // call to the loop. The end index is moved to the previous value, as it has
    // not yet been identified whether the larger (current) value in the right
    // array is present in the left array.
    sub x25, x24, #1        // Move the end index to the previous lower value.
    b similarity_loop       // Move to the next left index segment.

find_right_segment:
    // Find the end index of the current right segment.
    mov x0, x21             // x0 = pointer to right array.
    mov x1, x24             // x1 = right segment start index.
    mov x2, x26             // x2 = maximum array index.
    bl get_uniform_segment  // Find the index of the end of the segment.
    mov x25, x0             // Store the index of the end of the segment.

    // Calculate the similarity score for both segments.
    ldr w9, [x20, x22, lsl #2] // Load the value at current left start index.
    sub x10, x23, x22       // Get number of elements in left segment.
    add x10, x10, #1        // No. of elements + 1 to account for end element.
    sub x11, x25, x24       // Get number of elements in right segment.
    add x11, x11, #1        // No. of elements + 1 to account for end element.
    mul x12, x11, x9        // Similarity = no. right elements * value.
    mul x13, x10, x12       // Tot. similarity = no. left elements * similarity.
    add x19, x19, x13       // Add to cumulative similarity score.

    b similarity_loop       // Move to the next pair of segments.

    // All segments in the arrays have been identified and the similarity score
    // has been calculated.
similarity_loop_finished:

    // Print the calculated similarity score.
    ldr x0, =similarity_prefix // x0 = pointer to the similarity prefix.
    ldr x1, =similarity_prefix_len // x1 = similarity prefix string length.
    mov x2, x19             // x2 = calculated similarity value.
    bl print_output_value   // Print the prefix text and output value.

    // Exit the program with the success message.
program_end:

    // Print success message, loading the string pointer into x0 and the length
    // into x1.
    ldr x0, =success_string // x0 = pointer to the success string.
    ldr x1, =success_string_len // x1 = success string length.
    bl print_string         // Call print_string with provided arguments.

    // Return success code.
    mov x0, SUCCESS         // Load success return code into x0 register.
    b exit                  // Exit the program and return the loaded exit code.

// -----------------------------------------------------------------------------
// |                           Function Definitions                            |
// -----------------------------------------------------------------------------

// Function to check if two strings are equal up until a null terminator.
//
// Arguments:
// - x0: Pointer to first string.
// - x1: Pointer to second string.
//
// Returns:
// - x0: 1 if strings are equal, 0 otherwise.
compare_strings:
    // Loop over each character in each string in parallel and compare them.
    ldrb w9, [x0], #1       // Load char from string 1 and increment pointer.
    ldrb w10, [x1], #1      // Load char from string 2 and increment pointer.
    cmp w9, w10             // Compare string 1 char with string 2 char.
    b.ne strings_not_equal  // If not equal, return false.
    // Both characters are equal. Only one needs to be checked for end of
    // string. Since the string is null-terminated, if the character is non-zero
    // then the loop can be restarted until null is reached.
    cbnz w9, compare_strings // Restart the loop until null terminator.

    // If the loop exits after reaching the end of the strings, they were equal.
    mov x0, #1              // x0 = 1 to represent "true" boolean.
    ret
strings_not_equal:
    mov x0, #0              // x0 = 0 to represent "false" boolean.
    ret

// Exit the program with the specified exit code.
//
// Arguments:
// - x0: Exit code to return.
exit:
    mov x8, SYS_EXIT        // Move the syscall number for exit (93) into x8.
    svc SVC_INVOKE          // Trigger a supervisor call to exit.

// In the event of an error, print an error message and then exit the program
// with an error code.
//
// Arguments:
// - x0: Error string to print.
// - x1: Length of error string.
// - x2: Error code to return.
error_with_message:
    bl print_string         // Print the error message in x0.
    mov x0, x2              // Prepare the error code for the error function.
    b exit                  // Call the error function to exit the program.

// Exit the program with the ERR_OPENING_FILE error code, and print an error to
// the standard output.
file_open_error:
    // Prepare x0 and x1 with the error string and the length of the string to
    // print an error message to the standard output.
    ldr x0, =err_file_open_string
    ldr x1, =err_file_open_string_len
    mov x2, ERR_OPENING_FILE // Move the ERR_OPENING_FILE error code into x2.
    b error_with_message    // Print the error message and exit with an error.

// Exit the program with the ERR_READING_FILE error code, and print an error to
// the standard output.
file_read_error:
    // Prepare x0 and x1 with the error string and the length of the string to
    // print an error message to the standard output.
    ldr x0, =err_file_read_string
    ldr x1, =err_file_read_string_len
    mov x2, ERR_READING_FILE // Move the ERR_READING_FILE error code into x2.
    b error_with_message    // Print the error message and exit with an error.

// Function to find a segment of an array, where each element is the same. The
// index is limited to the maximum array index passed in to register x2.
//
// Arguments:
// - x0: Pointer to the start of the array.
// - x1: Index of the array to start the segment at.
// - x2: The maximum index of the array.
//
// Returns:
// - x0: Index of the end of the segment, where every element is uniform.
get_uniform_segment:
    ldr w9, [x0, x1, lsl #2] // Load the value at the current start index.

    // Internal loop to identify the last index of the segment where every
    // element is uniform.
segment_end_loop:
    add x1, x1, #1          // Increment the array index.
    cmp x1, x2              // Compare new index with index limit.
    b.hi uniform_segment_end // If index > limit, return.
    ldr w10, [x0, x1, lsl #2] // Load the value at the current index.
    cmp w9, w10             // Compare start value and current value.
    b.eq segment_end_loop   // Restart loop if values equal.

uniform_segment_end:
    // After the first non-uniform value is found, decrement the index in x1 for
    // return, so that it is the index of the last uniform value.
    //
    // Note that if the index exceeded the maximum index, this will also correct
    // it to be equal to the maximum index.
    sub x0, x1, #1          // Get index of last uniform value.
    ret                     // Return the end index.

// Function to convert one of the input numbers in string format to (unsigned)
// integer format. For simplicity sake, this assumes that the number is from the
// input, and is therefore 5 ASCII characters (and therefore bytes) long.
//
// Arguments:
// - x0: A pointer to the first digit of the number.
//
// Returns:
// - w0: The 32-bit uint representation of the number string.
input_string_to_uint:
    mov x9, x0              // Load start pointer into scratch register.
    mov w0, #0              // Initialise the return value to 0.
    mov w10, #5             // Loop variable = 5 to loop over each digit.
    mov w11, #10            // Load 10 into w11 to use for the mul instruction.
    // Loop through each byte that needs to be converted.
uint_convert_loop:
    ldrb w12, [x9]          // Load a character from the number into w12.
    sub w12, w12, #'0'      // Subtract the value of an ASCII 0 from the char.
    mul w0, w0, w11         // Multiply the previous result by 10.
    add w0, w0, w12         // Add the numeric value of the char to the result.
    add x9, x9, #1          // Move to the next character.
    subs w10, w10, #1       // Subtract 1 from the loop counter and set flags.
    b.hi uint_convert_loop  // Restart loop if counter in w10 > 0.
    // Return after the conversion loop exits, as the desired return is already
    // in the w0 register.
    ret

// Function to parse the command line arguments provided to the program. For
// versatility, this function does not reference the stack pointer itself,
// rather it relies on the x0 argument to pass in the initial stack pointer
// value from when the program was started. This allows the arguments to still
// be parsed after the stack has been added to.
//
// Arguments:
// - x0: Initial stack pointer value at program start.
//
// Returns:
// - x0: Pointer to the file path if supplied. 0 otherwise.
// - x1: Parse success code. Can be used as program exit code if x0 = 0.
parse_arguments:
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
    // used to the stack, so they can be reset on return.
    stp x19, x20, [sp, #-16]!

    // Arguments are provided on the stack when the program starts:
    // |---------------------|
    // |       argc          | <- sp
    // |---------------------|
    // |     argv[0]         | <- sp + 8
    // |---------------------|
    // |     argv[1]         | <- sp + 16
    // |---------------------|
    // |     argv[2]         | <- sp + 24
    // |---------------------|
    // |       ...           |
    // |---------------------|
    // |       NULL          | <- sp + 8 * (argc + 1)
    // |---------------------|
    //
    // - argc contains the number of arguments.
    // - argv[0] contains a pointer to the first argument, (the program name).
    // - argv[1] should contain a pointer to the option "-f".
    // - argv[2] should contain a pointer to the file name.
    mov x19, x0             // Store initial sp value in calle-saved register.
    ldr x20, [x0]           // Load argc into callee-saved register x20.

    // Check whether 2 or more arguments have been provided. This indicates that
    // at least one argument other than the program name have been provided.
    cmp x20, #2             // Compare argc and immediate value 2.
    b.lo argument_error     // If argc < 2, go to argument_error.

    // Check whether the argument provided is "-h". Prepare data to call
    // compare_strings on argv[1].
    ldr x0, [x19, #16]      // Load pointer to argv[1].
    ldr x1, =help_flag      // Load expected help flag string pointer.
    bl compare_strings      // Compare the two strings.
    cbnz x0, argument_help  // If strings match, print usage info.

    // If the provided argument was not "-h", then there should be exactly 3
    // arguments supplied: main -f <filename>. If not, there is an argument
    // error.
    cmp x20, #3             // Compare argc and immediate value 3.
    b.ne argument_error     // Go to argument error handling if argc != 3.

    // If 3 arguments were supplied, check that argv[1] is the file option (-f).
    ldr x0, [x19, #16]      // Load pointer to argv[1].
    ldr x1, =file_option    // Load expected file option string pointer.
    bl compare_strings      // Compare the two strings.
    cbz x0, argument_error  // If strings don't match, go to argument_error.

    // At this point, the correct number of arguments has been supplied, and the
    // file option (-f) has been supplied. Therefore, the next argument can be
    // interpreted as the path to the input file.
    ldr x0, [x19, #24]      // Load pointer to argv[2] into x0 for return.
    mov x1, SUCCESS         // Success code to return on program exit.
    b arguments_parsed      // Exit function with loaded return values.

    // Handler for argument error.
argument_error:
    ldr x0, =invalid_arguments // x0 = pointer to the invalid arguments string.
    ldr x1, =invalid_arguments_len // x1 = invalid arguments string length.
    bl print_string         // Call print_string with provided arguments.
    mov x1, ERR_ARGS        // Error code to return on program exit.
    // For an argument error, also display program usage information.
    b display_program_usage // Display usage information.

    // Handler for the help flag being supplied.
argument_help:
    mov x1, SUCCESS         // Success code to return on program exit.
    // Drop into display_program_usage to display help message.

    // Handler for usage information being requested. This assumes that a
    // success or error code has already been loaded into x1 for return.
display_program_usage:
    mov x20, x1             // Save x1 state in callee-saved register.
    bl print_usage          // Print the program usage information.
    mov x0, #0              // No file path supplied, set x0 to 0.
    mov x1, x20             // Restore x1 state from callee-saved register.

    // Function exit, restoring callee-saved registers from the stack.
    // This assumes return value have already been loaded into x0 and x1.
arguments_parsed:
    // Restore previous callee-saved register states from the stack.
    ldp x19, x20, [sp], #16

    // Before returning, load the original frame pointer (x29) and link register
    // (x30) states back so that the ret functions as expected.
    //
    // The values are taken from the top of the stack, and the stack pointer
    // value is then incremented using a post-index offset to "free" the memory.
    ldp x29, x30, [sp], #16
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
// - x0: Pointer to the line to parse.
//
// Returns:
// - w0: The 32-bit uint representing the first number on the line.
// - w1: The 32-bit uint representing the second number on the line.
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

    // Store the current values of the used callee-saved registers that will be
    // used to the stack, so they can be reset on return.
    stp x19, x20, [sp, #-16]!

    // input_string_to_uint returns in the w0 register, so store the current x0
    // register into a callee-saved register for use later.
    mov x19, x0             // Store the pointer to the line in x19.

    // x0 currently points to the start of the line to parse, so it is already
    // a pointer to the start of the first number.
    bl input_string_to_uint // Convert the first number to a uint in w0.
    mov w20, w0             // Store the first parsed number in w20.

    // Set x0 to the start of the second number in the line. This is done by
    // incrementing the stored value in x19 by 8 (5 bytes for the first number
    // and 3 bytes for the spaces).
    add x0, x19, #8

    // x0 now points to the start of the second number to parse.
    bl input_string_to_uint // Convert the second number to a uint in w0.

    // Move the return values into the correct registers.
    mov w1, w0              // Move the second number into w1.
    mov w0, w20             // Move the first number into w0.

    // Restore previous callee-saved register states from the stack.
    ldp x19, x20, [sp], #16

    // Before returning, load the original frame pointer (x29) and link register
    // (x30) states back so that the ret functions as expected.
    //
    // The values are taken from the top of the stack, and the stack pointer
    // value is then incremented using a post-index offset to "free" the memory.
    ldp x29, x30, [sp], #16
    ret

// Function implementing Hoare's partition algorithm using a "median of three"
// pivot selection if there are enough elements in the array segment.
//
// Arguments:
// - x0: Pointer to the array containing the segment to partition.
// - x1: Index of the start of the array segment to partition.
// - x2: Index of the end of the array segment to partition.
//
// Returns:
// - x0: Partition index indicating where the array should next be partitioned.
partition:
    // Note that when calculating the number of elements, the result is
    // non-inclusive of the last element.
    //
    // Normally 1 would be added to the total, but since this value is only used
    // to decide whether to branch it saves on an instruction to just compare to
    // the non-inclusive minimum value (2) instead of 3.
    sub x9, x2, x1          // Get no. of elements (not including last).
    cmp x9, #2              // Compare with #2 as last element not counted.
    b.lo partition_setup    // Just use first element as pivot if elements < 3.

    // If there are 3 or more elements, use the "median of three" elements as
    // the pivot. The three elements being the start, middle and end.
    //
    // Using the median element as the pivot provides the best time complexity.
    // This is way overkill for an AoC challenge, but I can so I will :).

    // Calculate the index of the middle element.
    add x9, x1, x2          // x9 = Start index + end index.
    lsr x9, x9, #1          // x9 = Middle index = (start + end) / 2.

    // Load data from the indexes to get the values of the start, middle and end
    // array segment elements.
    //
    // These are achieved using the Load Register (Indexed) instruction:
    // - ldr (destination), [(base address), (index), lsl #2]:
    //   - (destination): The register to load data to.
    //   - (base address): The address of the start of the array.
    //   - (index): The index of the array to load data from.
    //   - lsl #2: Shift index left 2 bits (multiply mby 4), since the array
    //             stores 32-bit numbers, each "index" accounts for 4 bytes of
    //             data.
    ldr w10, [x0, x1, lsl #2] // w10 = first element.
    ldr w11, [x0, x9, lsl #2] // w11 = middle element.
    ldr w12, [x0, x2, lsl #2] // w12 = last element.

    // Find the median value of w10, w11, and w12 to get the median "pivot". The
    // final pivot value will be stored in w13.
    cmp w10, w11            // Compare w10 and w11.
    csel w13, w10, w11, lt  // w13 = min(w10, w11).
    csel w14, w11, w10, lt  // w14 = max(w10, w11).

    cmp w14, w12            // Compare w14 and w12.
    csel w15, w14, w12, lt  // w15 = min(max(w10, w11), w12).
    csel w16, w12, w14, lt  // w16 = max(max(w10, w11), w12).

    cmp w13, w15            // Compare min(w10, w11) and min(max(w10, w11), w12).
    csel w17, w13, w15, gt  // w17 = max(min(w10, w11), min(max(w10, w11), w12)).

    // Finally, the median is the smallest between the following values:
    // - w16 = max(max(w10, w11), w12).
    // - w17 = max(min(w10, w11), min(max(w10, w11), w12)).
    cmp w17, w16
    csel w13, w17, w16, lt  // w13 = median(w10, w11, w12).

    // Hoare's partition algorithm requires the pivot to be at the first element
    // of the array, so swap the median element and the first element.
    str w13, [x0, x1, lsl #2] // Always overwrite first index.
    cmp w13, w10            // Compare the median value with the first element.
    b.eq partition_setup    // Skip swap if the first element is the median.
    cmp w13, w11            // Compare the median value with the middle element.
    b.eq swap_middle_element// Swap first and middle element.
    str w10, [x0, x2, lsl #2] // Otherwise, swap first and last element.
    b partition_setup       // Go to the partition setup.

swap_middle_element:
    str w10, [x0, x9, lsl #2] // Swap first and middle element.

partition_setup:
    // Load the pivot (first array element) into w9. This is done regardless of
    // whether the median of three pivot selection method was used or not, as
    // the pivot is always moved to the first array element. If there are too
    // few elements to perform the median of three pivot selection, this will
    // just use the first element.
    ldr w9, [x0, x1, lsl #2] // w9 = selected pivot.

    // Load the start and end indexes into scratch registers so they can be
    // iterated.
    //
    // Not that the indexes are started outside of the array range (by
    // subtracting 1 from the left index and adding 1 to the right index) to
    // simplify the loop.
    sub x10, x1, #1         // x10 = left index (start index - 1).
    add x11, x2, #1         // x11 = right index (end index + 1).

    // Internal loop to iterate the indexes over the entire array segment.
partition_loop:
    add x10, x10, #1        // Increment the left index to move it to the right.
    ldr w12, [x0, x10, lsl #2] // Load the value at the left index into w12.
    cmp w12, w9             // Compare the value at the left index to the pivot.
    b.lo partition_loop     // Restart the loop if the value < pivot.

    // Internal loop to move the right index to the left until the value is
    // less than or equal to the pivot.
right_index_loop:
    sub x11, x11, #1        // Decrement the right index to move it to the left.
    ldr w13, [x0, x11, lsl #2] // Load the value at the right index into w13.
    cmp w13, w9             // Compare the value at right index to the pivot.
    b.hi right_index_loop   // Restart the loop if the value > pivot.

    // Check if the indexes crossed over. The point at which they crossed will
    // be the next pivot index.
    cmp x10, x11            // Compare the right index to the left index.
    b.ge partition_done     // If the indexes crossed over, exit the loop.

    // If this point is reached, then the value at the left index is greater
    // than the pivot, and the value at the right index is less than the pivot.
    // Swap them to get them on the correct side of the pivot.
    str w13, [x0, x10, lsl #2] // Value at right index -> left index.
    str w12, [x0, x11, lsl #2] // Value at left index -> right index.
    b partition_loop        // Restart the loop until all data is processed.

partition_done:
    // At this point, the left and right index have crossed over. The right
    // index value (x11) stops at the lest element it encounters (travelling
    // right to left) that is less than or equal to the pivot. This ensures that
    // all elements on the left of the pivot are less than the pivot, and all
    // elements on the right of the pivot are greater than the pivot.
    mov x0, x11             // Return the index of the next partition.
    ret

// Function to print an output value with descriptor text. The prefix text gives
// context to the value.
//
// Arguments:
// - x0: Pointer to value prefix text.
// - x1: Prefix text length.
// - x2: The value to print after the prefix text.
print_output_value:
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
    // used to the stack, so they can be reset on return. Note the use of the
    // zero register (zxr) as a dummy register, to maintain 16-byte alignment of
    // the stack.
    stp x19, xzr, [sp, #-16]!

    mov x19, x2             // Move the output value to a callee-saved register.

    // Print the prefix text that explains the value being printed to stdout.
    // Note that x0 and x1 are already loaded with the correct arguments.
    bl print_string         // Call print_string with provided arguments.

    // Convert the output value to a string.
    mov x0, x19             // x0 = stored value.
    ldr x1, =print_buffer   // x1 = destination array = print buffer.
    bl uint_to_string       // Call uint_to_string function with provided args.

    // Print the string representation of the output value.
    mov x1, x0              // Move string length to x1 for print_string call.
    ldr x0, =print_buffer   // x0 = pointer to the print buffer.
    bl print_string         // Call print_string with provided arguments.

    // Print the suffix text that terminates the line.
    ldr x0, =output_suffix  // x0 = pointer to the output suffix.
    ldr x1, =output_suffix_len // x1 = output suffix length.
    bl print_string         // Call print_string with provided arguments.

    // Restore previous callee-saved register states from the stack. Note the
    // use of the zero register (zxr) as a dummy register, to maintain 16-byte
    // alignment of the stack.
    ldp x19, xzr, [sp], #16

    // Before returning, load the original frame pointer (x29) and link register
    // (x30) states back so that the ret functions as expected.
    //
    // The values are taken from the top of the stack, and the stack pointer
    // value is then incremented using a post-index offset to "free" the memory.
    ldp x29, x30, [sp], #16
    ret

// Function to write a string to standard output.
//
// Arguments:
// - x0: Pointer to the string to write to standard output.
// - x1: Length of the string.
print_string:
    // This uses the write syscall, which has the following arguments:
    // - x0: File descriptor.
    // - x1: Buffer to write data from.
    // - x2: Number of bytes to write.
    //
    // Returns:
    // - x0: The number of bytes written on success.
    // - x0: A negative value on error.
    mov x2, x1              // Move string length to x2 for syscall.
    mov x1, x0              // Move the buffer pointer to x1 for syscall.
    mov x0, STDOUT          // Set file descriptor to standard output.
    mov x8, SYS_WRITE       // Move the syscall number for write (64) into x8.
    svc SVC_INVOKE          // Trigger a supervisor call for the syscall in x8.
    ret                     // Return from the function.

// Function to print the program usage information to standard output and then
// return.
print_usage:
    // Since this function uses branches with links, store the state of the
    // frame pointer (x29) and link register (x30) so they can be restored
    // later.
    //
    // These are stored on top of the stack by taking the current stack pointer
    // (sp), decrementing it by 16 bytes before the store instruction (using
    // pre-indexing denoted by !), and using the stack pointer value to store
    // the registers to.
    stp x29, x30, [sp, #-16]!

    // Prepare x0 and x1 with the program usage string and the length of the
    // string to print to standard output.
    ldr x0, =program_usage
    ldr x1, =program_usage_len
    bl print_string         // Print the usage information.

    // Before returning, load the original frame pointer (x29) and link register
    // (x30) states back so that the ret functions as expected.
    //
    // The values are taken from the top of the stack, and the stack pointer
    // value is then incremented using a post-index offset to "free" the memory.
    ldp x29, x30, [sp], #16
    ret

// Function implementing the quick sort algorithm using Hoare's partition
// algorithm.
//
// Arguments:
// - x0: Pointer to the array containing the segment to sort.
// - x1: Index of the start of the array segment to sort.
// - x2: Index of the end of the array segment to sort.
quicksort:
    // Check if there is any data to sort.
    cmp x1, x2              // Compare the start and end index.
    b.ge quicksort_exit     // If start >= end, exit quicksort.

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
    // used to the stack, so they can be reset on return.
    stp x19, x20, [sp, #-16]!
    stp x21, x22, [sp, #-16]!

    // Store the array pointer (x0), array segment start index (x1), and array
    // segment end index (x2) into callee-saved registers.
    mov x19, x0             // x19 = stored array pointer.
    mov x20, x1             // x20 = array segment start index.
    mov x21, x2             // x21 = array segment end index.

    // Partition the array segment contained between x1 and x2. Store the
    // returned partition index in the callee-saved register x22.
    //
    // Note that the arguments for the partition function are identical to that
    // of the quicksort function, so the values currently in x0, x1 and x2 are
    // already valid.
    bl partition
    mov x22, x0             // x22 = partition index.

    // Prepare arguments to call quicksort on the left array segment.
    mov x0, x19             // Array pointer pointer = stored array pointer.
    mov x1, x20             // Array segment start index = original start index.
    mov x2, x22             // Array segment end index = partition index.
    bl quicksort            // Sort left array segment.

    // Prepare arguments to call quicksort on the right array segment.
    mov x0, x19             // Array pointer pointer = stored array pointer.
    add x1, x22, #1         // Array segment start index = partition index + 1.
    mov x2, x21             // Array segment end index = original end index.
    bl quicksort            // Sort right array segment.

    // Restore previous callee-saved register states from the stack.
    ldp x21, x22, [sp], #16
    ldp x19, x20, [sp], #16

    // Before returning, load the original frame pointer (x29) and link register
    // (x30) states back so that the ret functions as expected.
    //
    // The values are taken from the top of the stack, and the stack pointer
    // value is then incremented using a post-index offset to "free" the memory.
    ldp x29, x30, [sp], #16

quicksort_exit:
    ret

// Function to convert a uint value to a string to display.
//
// Arguments:
// - x0: The uint value to display.
// - x1: Pointer to the buffer to store the string to.
//
// Returns:
// - x0: The number of bytes written to the buffer.
uint_to_string:
    // Since this function uses branches with links, store the state of the
    // frame pointer (x29) and link register (x30) so they can be restored
    // later.
    //
    // These are stored on top of the stack by taking the current stack pointer
    // (sp), decrementing it by 16 bytes before the store instruction (using
    // pre-indexing denoted by !), and using the stack pointer value to store
    // the registers to.
    stp x29, x30, [sp, #-16]!

    mov x9, x0              // Move integer to scratch register x9.
    mov x10, x1             // Move buffer address to scratch register x10.
    mov x11, #10            // Constant divisor to get the value of each digit.

    // Inner loop to convert the uint to ascii characters.
string_convert_loop:
    udiv x12, x9, x11       // Divide the integer by 10.
    msub x13, x12, x11, x9  // x13 = x9 - (x12 * 10). Equivalent to x9 % 10.
    add x13, x13, #'0'      // Add the value of ASCII 0 to get digit as ASCII.
    strb w13, [x10], #1     // Store string byte and increment pointer.
    mov x9, x12             // Update x9 with quotient.
    cbnz x9, string_convert_loop // Loop if quotient is not zero.

    // Decrement string buffer pointer by 1, as it was incremented after the
    // last character was added to the buffer.
    sub x10, x10, #1

    // Data was converted with the least significant digit first. For standard
    // string representation, this now needs to be reversed.
    mov x11, x1             // Move string start pointer to scratch register.
    mov x12, x10            // Move string end pointer to scratch register.

    // Inner loop to reverse the converted string characters, as it was
    // converted with the least-significant digit first.
reverse_loop:
    ldrb w14, [x11]         // Load the byte from the start pointer.
    ldrb w15, [x12]         // Load the byte from the end pointer.
    strb w15, [x11]         // End pointer value -> start pointer.
    strb w14, [x12]         // Start pointer value -> end pointer.
    add x11, x11, #1        // Increment start pointer.
    sub x12, x12, #1        // Decrement end pointer.
    cmp x11, x12            // Compare pointers.
    b.lo reverse_loop       // Loop if start pointer < end pointer.

    // Before returning, load the original frame pointer (x29) and link register
    // (x30) states back so that the ret functions as expected.
    //
    // The values are taken from the top of the stack, and the stack pointer
    // value is then incremented using a post-index offset to "free" the memory.
    ldp x29, x30, [sp], #16

    sub x0, x10, x1         // x0 = string end pointer - string start pointer.
    add x0, x0, #1          // Add 1 to string length to include last char.
    ret
