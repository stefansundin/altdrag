/*
	Writes information to a .ini file.
	Used to batch-write the Language setting when using 'build all'.
	
	Copyright (C) 2009  Stefan Sundin (recover89@gmail.com)
	
	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
*/

#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <shlwapi.h>

int main(int argc, char *argv[]) {
	if (argc < 4) {
		printf("Not enough arguments\n");
		printf("Usage: ini <file> <section> <key> [new value]\n");
		return 0;
	}
	
	//Get path
	char path[MAX_PATH];
	if (PathIsRelative(argv[1])) {
		GetCurrentDirectory(sizeof(path), path);
		PathAddBackslash(path);
		strcat(path, argv[1]);
	}
	else {
		strcpy(path, argv[1]);
	}
	
	//Write/Read
	if (argc == 4) {
		char txt[1000];
		GetPrivateProfileString(argv[2], argv[3], NULL, txt, sizeof(txt), path); //No error detection possible
		printf(txt);
	}
	else if (WritePrivateProfileString(argv[2],argv[3],argv[4],path) == 0) {
		int errorcode = GetLastError();
		char *errormsg;
		int length = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM,NULL,errorcode,0,(char*)&errormsg,0,NULL);
		errormsg[length-2] = '\0'; //Remove that damn newline at the end of the formatted error message
		printf("WritePrivateProfileString() failed in file %s, line %d.\nError: %s (%d)", TEXT(__FILE__), __LINE__, errormsg, errorcode);
		LocalFree(errormsg);
		return 1;
	}
	return 0;
}
