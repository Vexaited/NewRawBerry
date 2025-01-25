/* 
 * ---------------------------------------------------------------
 *                            RawBerry
 * (c) RawBerry Team
 * Published on license MIT. Read `/LICENSE` for more details.
 * ---------------------------------------------------------------
*/
#include "main.h"
#include "../drivers/vga.h"
#include "../drivers/port.h"
#include "../drivers/keyboard.h"
#include "../include/String.h"
#include "../include/memory.h"
#include "../include/ascii.h"
#include "../include/math.h"
#include "../drivers/fs.h"
#include "../drivers/ata.h"
#include "test.h"
#include "../include/stdio.h" // :: There is no standard yet, either switch to "stdio.h" or comment it out until
//                               :: there is implementation for it
char* commands[] = {
    "echo %1", "prints argument", // 0 1
    "clear", "clears screen", // 2 3
    "help", "display help", // 4 5
    "logo", "shows rawberry logo", // 6 7
    "about", "about the os", // 8 9
    "write %1", "write string to memory", // 10 11
    "read", "read from memory", // 12 13
    "mem-usage", "print memory usage (from write)",
};
void display() {
    for (int i = 0; i < 8; i++) {  
        vga_print_string(commands[i * 2]);   
        vga_print_string(": ");
        vga_print_string(commands[i * 2 + 1]); 
        vga_print_string("\n");
    }
}
uint8_t buf[512];
void ata_test_write(){
    uint8_t a[] = {104, 101, 108, 108, 111, 111};
    for (int i = 0; i < 512; i++) {
        if (i < 5){
            buf[i] = a[i];
        }
        else{
            buf[i] = 0;
        }
    }
    uint32_t lba = 100;
    ata_write(lba, buf);
}
void ata_test_read(){
    uint32_t lba = 100;
    ata_read(lba, buf);
}


void launch_kernel(void) {
    vga_initialize();
    vga_print_string("Welcome to RawBerry OS!\n");
    vga_print_string("[YANE TERMINAL 1.0.0]\n");
    memory_entry memory_table[15] = {};
    
    ata_test_write();
    ata_test_read();

    const int32_t start_address = 0x400000; // keep it as const
    int32_t last_address = 0x400000 + Strlen("Hello, RawBerryOS!");
    int32_t last = 1;
    // to use end_adress do: start_address + Strlen(string)
    char* Keyboard_buffer;
    while (1) {
        vga_print_string(": ");
        Keyboard_buffer = readStr();

        if (strEql(strSlice(Keyboard_buffer, 0, 4), "echo") == 0) {
            char* sliced = strSlice(Keyboard_buffer, 5, Strlen(Keyboard_buffer));
            vga_print_string(sliced);
            
        } else if (strEql(strSlice(Keyboard_buffer, 0, 5), "clear") == 0) {
            vga_clear_screen();

        } else if (strEql(strSlice(Keyboard_buffer, 0, 4), "help") == 0) {
            display();

        } else if (strEql(strSlice(Keyboard_buffer, 0, 4), "logo") == 0) {
            logo();

        } else if (strEql(strSlice(Keyboard_buffer, 0, 5), "about") == 0) {
            print_info();

        } else if (strEql(strSlice(Keyboard_buffer, 0, 5), "write") == 0) {
            char* buffer = strSlice(Keyboard_buffer, 6, Strlen(Keyboard_buffer));
            memory_write(memory_table, last_address, last_address+Strlen(buffer), last, buffer);
            last++;
            last_address = last_address + Strlen(buffer);
            vga_print_string("Written to memory.\n");

        } else if (strEql(strSlice(Keyboard_buffer, 0, 4), "read") == 0) {
            memory_print(memory_table, 15);
            char* buffer = memre(start_address, last_address);
            //vga_print_string("Read: ");
            //vga_print_string(buffer);
        } else if (strEql(strSlice(Keyboard_buffer, 0, 5), "crash") == 0){
            asm volatile ("ud2");
        } else if (strEql(strSlice(Keyboard_buffer, 0, 9), "mem-usage") == 0){
            memory_info(memory_table, 15);
        } else if (strEql(strSlice(Keyboard_buffer, 0, 6), "delete") == 0){
            const char* buffer = strSlice(Keyboard_buffer, 7, Strlen(Keyboard_buffer)-1);
            int index = stol(buffer);
            memory_delete(memory_table, index);
            vga_print_string("memory deleted\n");
        } else if (strEql(strSlice(Keyboard_buffer, 0, 3), "fat") == 0){
            fat_test();
        } else if(strEql(strSlice(Keyboard_buffer, 0, 3), "get") == 0){
            get_data();
        } 
    }
    return;
}

