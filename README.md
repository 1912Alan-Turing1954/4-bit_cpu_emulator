# Hands-On CPU Learning Tool

Welcome to the **Hands-On CPU Learning Tool**, designed to help you understand CPU basics by simulating a simple CPU. Interact with the CPU, modify memory, execute instructions, and see how the state changes step-by-step.

---

## Commands

- **`list memory`**: View the 16-byte memory.
- **`change memory`**: Modify memory values (multiple addresses).
- **`run`**: Execute the program, step by step.
- **`reset`**: Reset registers and memory to default.
- **`list registers`**: View the 4 registers.
- **`help`**: Show this help message.
- **`exit`**: Exit the tool.

---

![image](https://github.com/user-attachments/assets/922b804a-f3f4-4d6f-a185-1e4830957bf1)


## Opcodes

### `0x01 - LOAD`
Load a value into a register.
- **Example**: `LOAD 0x00 0x02` loads `0x02` into Register `0x00`.

### `0x02 - ADD`
Add two registers and store in the first.
- **Example**: `ADD 0x00 0x01` adds values of `R0` and `R1`, stores in `R0`.

### `0x03 - SUB`
Subtract the second register from the first.
- **Example**: `SUB 0x00 0x01` subtracts `R1` from `R0` and stores in `R0`.

### `0x04 - JUMP`
Set the Program Counter (PC) to a memory address.
- **Example**: `JUMP 0x03` sets PC to `0x03`.

### `0x00 - NOOP`
No operation, increments PC.
- **Example**: `NOOP` moves PC to the next instruction.

---

## Execution Flow

1. **Fetch**: CPU gets the current instruction from memory.
2. **Decode**: CPU interprets the instruction.
3. **Execute**: CPU performs the operation and updates its state.

---

## Learn By Doing

Experiment by modifying memory, running instructions, and watching how registers and memory change. The tool simulates how a CPU works step by step, providing hands-on experience.

---

Enjoy.
