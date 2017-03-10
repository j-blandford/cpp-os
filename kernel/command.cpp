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

	// for(int i = 0; i < tokens.size(); i++) {
	// 	terminal_printf("%s(l) %d\n", (char*)tokens[i], tokens[i] == "ls");
	// }

	if(tokens[0] == "ls") {
		//ATA::getDirectoryPath(0, "boot");
		init_pci();
		update_buffer(false);
	} 
	else if(tokens[0] == "listpci") {
		init_pci();
		update_buffer(false);
	}

	else if(strncmp(tokens[0], "ls", 2) == 0) {
	    //std::vector<Filesystems::DirectoryEntry> dir = found_devices[0]->readDirectoryTable(512);

		std::vector<Filesystems::DirectoryEntry> dir = ATA::getDirectoryPath(0, tokens[1]);

		for(auto it = dir.begin(); it != dir.end(); it++) {
			char* fileType = new char[4];

			if((*it).attr == Filesystems::FATAttributes::shortNameFile) {
				strncpy(fileType, "FILE", 4);
			} 
			else {
				strncpy(fileType, "DIR", 3);
			}
			
			terminal_printf("%s \t\t %s @ %x\n", (*it).name, fileType, (*it).location);
		}


	    update_buffer(false);
	}
}