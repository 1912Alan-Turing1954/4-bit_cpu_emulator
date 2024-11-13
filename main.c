#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

// Define a 4-bit CPU
typedef struct {
    uint8_t registers[4]; // 4 registers of 4-bits each (can be viewed as 4-bit values)
    uint8_t PC;           // Program Counter
    uint8_t IR;           // Instruction Register
    uint8_t flags;        // Status flags (e.g., zero, carry)
    uint8_t memory[16];   // 16 bytes of memory
} CPU;

void load(CPU *cpu, uint8_t reg, uint8_t value) {
    if (reg < 4) {
        cpu->registers[reg] = value;
    } else {
        printf("Invalid Register.\n");
    }
}

void add(CPU *cpu, uint8_t reg1, uint8_t reg2) {
    uint8_t result = cpu->registers[reg1] + cpu->registers[reg2];

    cpu->flags = (result == 0) ? 1 : 0;
    cpu->registers[reg1] = result;
    cpu->registers[reg2] = 0;
}

void subtract(CPU *cpu, uint8_t reg1, uint8_t reg2) {
    uint8_t result = cpu->registers[reg1] - cpu->registers[reg2];

    cpu->flags = (result == 0) ? 1 : 0;
    cpu->registers[reg1] = result;
    cpu->registers[reg2] = 0;
}

void jump(CPU *cpu, uint8_t address) {
    if (address < 16) {
        cpu->PC = address;
    } else {
        printf("Invalid Address.\n");
    }
}

void fetch(CPU *cpu) {
    cpu->IR = cpu->memory[cpu->PC];
}

// Function to print the list of available commands and their descriptions
void print_help() {
    printf("\nWelcome to the CPU Learning Tool!\n");
    printf("This is an educational tool designed to help you understand how a simple CPU works.\n");
    printf("It allows you to explore the basic operations of a CPU, such as loading values into registers, adding or subtracting them, jumping to memory addresses, and much more.\n");
    printf("By interacting with this tool, you can experiment with a CPU's instructions and see how they affect the state of the machine step by step.\n");
    printf("\nHere are the available commands and opcodes that you can use:\n");

    printf("\nAvailable Commands:\n");
    printf("1. list memory       - Display the contents of the CPU memory (16 bytes).\n");
    printf("2. change memory     - Modify the contents of the CPU memory (multiple addresses/values).\n");
    printf("3. run               - Execute the loaded program (fetch, decode, execute instructions).\n");
    printf("4. reset             - Reset CPU registers and memory to their initial state.\n");
    printf("5. list registers    - Display the contents of the CPU registers (4 registers).\n");
    printf("6. help              - Display this help message.\n");
    printf("7. exit              - Exit the program.\n");

    printf("\nOpCodes Descriptions:\n");
    printf("0x01 - LOAD: Load a value into a register from memory.\n");
    printf("0x02 - ADD: Add the values in two registers and store the result in the first register.\n");
    printf("0x03 - SUB: Subtract the value of the second register from the first and store the result.\n");
    printf("0x04 - JUMP: Set the Program Counter (PC) to the specified memory address.\n");
    printf("0x00 - NOOP: No operation. Simply moves to the next instruction.\n");
}


void decode_and_execute(CPU *cpu) {
    switch (cpu->IR) {
        case 0x01: // Load instruction
            printf("Executing LOAD\n");
            load(cpu, cpu->memory[cpu->PC + 1], cpu->memory[cpu->PC + 2]);
            cpu->PC += 3;  // Jump over the LOAD instruction
            break;
        case 0x02: // ADD instruction
            printf("Executing ADD\n");
            add(cpu, cpu->memory[cpu->PC + 1], cpu->memory[cpu->PC + 2]);
            cpu->PC += 3;  // Jump over the ADD instruction
            break;
        case 0x03: // SUB instruction
            printf("Executing SUB\n");
            subtract(cpu, cpu->memory[cpu->PC + 1], cpu->memory[cpu->PC + 2]);
            cpu->PC += 3;  // Jump over the SUB instruction
            break;
        case 0x04: // Jump instruction
            printf("Executing JUMP\n");
            jump(cpu, cpu->memory[cpu->PC + 1]);
            break;
        case 0x00:
            cpu->PC++;
            break;
        default:
            printf("Unknown instruction 0x%02X\n", cpu->IR);
            cpu->PC++;  // Just move to the next instruction if unknown
            break;
    }
}

void print_memory(CPU *cpu) {
    for (int i = 0; i < 16; i++) {
        printf("Memory[%d]: %d\n", i, cpu->memory[i]);
    }
}

void print_registers(CPU *cpu) {
    for (int i = 0; i < 4; i++) {
        printf("Register[%d]: %d\n", i, cpu->registers[i]);
    }
}
// Function to modify multiple memory locations in one line
void modifyMemory(CPU *cpu) {
    char input[256];
    int address, value;

    while (1) {
        system("clear");  // Optionally clear the terminal screen
        print_memory(cpu); // Print current CPU state

        // Prompt user for input
        printf("\nEnter memory addresses and values to modify (e.g., '0 100 2 255'): ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = 0;  // Remove trailing newline

        // Parse the input
        char *token = strtok(input, " ");  // Tokenize by spaces

        while (token != NULL) {
            if (sscanf(token, "%d", &address) == 1) {
                // Get the next token for the value
                token = strtok(NULL, " ");
                if (token == NULL) {
                    printf("Error: Missing value for address %d\n", address);
                    break;
                }

                if (sscanf(token, "%d", &value) == 1) {
                    // Check if the address and value are within bounds
                    if (address >= 0 && address < 16 && value >= 0 && value <= 255) {
                        cpu->memory[address] = value;
                        printf("Memory[%d] set to %d\n", address, value);
                    } else {
                        printf("Error: Invalid address (%d) or value (%d)\n", address, value);
                    }
                } else {
                    printf("Error: Invalid value '%s' for address %d\n", token, address);
                }
            } else {
                printf("Error: Invalid address format '%s'\n", token);
            }
            token = strtok(NULL, " ");  // Move to the next token
        }

        // Ask if the user wants to continue modifying
        char inputChoice[10];
        printf("\nWould you like to modify more memory locations? (y/n): ");
        fgets(inputChoice, sizeof(inputChoice), stdin);
        inputChoice[strcspn(inputChoice, "\n")] = 0;  // Remove trailing newline

        if (strcmp(inputChoice, "n") == 0 || strcmp(inputChoice, "N") == 0) {
            break;
        }
    }
}


void reset(CPU *cpu) {
    memset(cpu, 0, sizeof(CPU)); // Reset all fields to zero
    cpu->PC = 0;  // Ensure PC is set to 0 after reset
}

int main() {
    CPU cpu = {0};
    char input[32];

    // Sample program in memory (uncomment to test)
    // cpu.memory[0x00] = 0x01;  // LOAD opcode (0x01)
    // cpu.memory[0x01] = 0x00;  // Load into Register 0 (R0)
    // cpu.memory[0x02] = 0x02;  // Load value 2 into R0

    while (1) {
        printf("CPU/");
        fgets(input, sizeof(input), stdin);

        // Trim newline character
        input[strcspn(input, "\n")] = '\0';

        if (strcmp(input, "list memory") == 0) {
            print_memory(&cpu);
        } else if (strcmp(input, "change memory") == 0) {
            modifyMemory(&cpu);
        } else if (strcmp(input, "run") == 0) {
            // Execute the program
            while (cpu.PC < 16) {
                fetch(&cpu);
                decode_and_execute(&cpu);
            }
            printf("CPU Run Cycle Finished.\n");
        } else if (strcmp(input, "reset") == 0) {
            reset(&cpu);
            printf("CPU registers and memory have been reset.\n");
        } else if (strcmp(input, "list registers") == 0) {
            print_registers(&cpu);
        } else if (strcmp(input, "help") == 0) {
            print_help();
        } else if (strcmp(input, "exit") == 0) {
            exit(0);
        } else {
            printf("Invalid command.\n");
        }
    }

    return 0;
}