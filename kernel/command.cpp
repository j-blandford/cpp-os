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
#include <env.h>

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
		ATA::getDirectoryPath(0, ENV::get("cd"));
		update_buffer(false);
	} 
	else if(tokens[0] == "listpci") {
		init_pci();
		update_buffer(false);
	}

	else if(strncmp(tokens[0], "ls", 2) == 0) {
		string currentPath;

		// Is this an absolute or relative path? 
		// we need to add the current working directory if it's relative
		if(tokens[1][0] == '/') {
			// absolute path, do nothing to it :)
			currentPath = tokens[1];
		} 
		else {
			// Relative path
			currentPath = ENV::get("cd");
			currentPath += tokens[1];
			currentPath += '/';
		}

		std::vector<Filesystems::DirectoryEntry> dir = ATA::getDirectoryPath(0, currentPath);

		for(auto it = dir.begin(); it != dir.end(); it++) {
			char* fileType = new char[4];
			RGBA col;

			if((*it).attr == Filesystems::FATAttributes::shortNameFile) {
				strncpy(fileType, "FILE", 4);
				col = RGBA(0xFFFFFF);
			} 
			else {
				strncpy(fileType, "DIR", 3);
				col = RGBA(0xc3e88d);
			}
			
			terminal_printf_rgba("%s \t\t %s @ %x\n", col, (*it).name, fileType, (*it).location);
		}


	    update_buffer(false);
	}

	else if(strncmp(tokens[0], "cd", 2) == 0) {
		if(tokens[1][0] == '/') {
			ENV::set("cd", tokens[1]);
		}
		else {
			string currentPath = ENV::get("cd");

			currentPath += tokens[1];
			currentPath += '/';
			
			ENV::set("cd", currentPath);
		}
	}
}