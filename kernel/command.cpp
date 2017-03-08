#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include <std/string.h>
#include <std/vector.h>

#include <devices/ata.h>
#include <drivers/pci.h>

#include <gfx/vesa.h>

#include <command.h>
#include <tty.h>

std::vector<string> Command::split(char* command) {
	std::vector<string> commandTokens;

	char* s = strtok(command, " ");

	// Lets split the command buffer
	while(s != NULL) {
		commandTokens.push_back(string(s));
		s = strtok(NULL, " ");
	}

	return commandTokens;
}

void Command::Parse(char * buffer) {
	std::vector<string> tokens = Command::split(buffer);

	if(tokens[0] == "ls") {
		ATA::getDirectoryPath(0, tokens[1]);
		update_buffer(false);
	} 
	else if(tokens[0] == "listpci") {
		init_pci();
		update_buffer(false);
	}

	// else if(strncmp(tokens[0], "ls boot", 7) == 0) {
	//     ATA::getDirectory(0, 544);
	//     update_buffer(false);
	// }
}