#include <stdbool.h> 
#include <stddef.h> 

#include <std/vector.h>
#include <std/string.h>

#include <env.h>
#include <tty.h> 

#include <cpu/idt.h> 
#include <devices/filesystems.h> 
#include <devices/ata.h> 
#include <gfx/vga.h>
#include <gfx/vesa.h>

std::vector<pair> ENV::environment = std::vector<pair>();

string ENV::get(char* envVar) {
	for(auto it = environment.begin(); it != environment.end(); it++) {
		if(strcmp((*it).key, envVar) == 0) 
			return (*it).value;
	}
	
	return "";
}

void ENV::set(string envVar, string envVal) {

	for(auto it = environment.begin(); it != environment.end(); it++) {
		if(strcmp((*it).key, envVar) == 0) {
			(*it).value = envVal;
			return;
		}
	}

	// if we get here, then the key wasn't found inside the vector, so let's add it!
	environment.push_back(pair(envVar, envVal));
}

void ENV::setup() {
	ENV::set("cd", "/");
	ENV::set("user", "james");
	//ENV::set()

	// terminal_printf("%x\n", &environment);
}