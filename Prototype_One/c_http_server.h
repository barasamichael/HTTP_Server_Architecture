#ifndef C_HTTP_SERVER_H
#define C_HTTP_SERVER_H

#include <arpa/inet.h>
#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <pthread.h>
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>

/**
 * get_file_extension
 * 	Extracts the file extension from a file name
 *
 * 	@file_name: C-style string representing a file name
 * 	@return: pointer to constant character representing the file extension
 */
const char* get_file_extension (const char* file_name);

/**
 * get_mime_type
 * 	Determines the MIME type of a file based on its file extension.
 *	@file_extension: C-style string representing file extension
 *	@return: a pointer to a constant character representing the MIME type 
 *		associated with the given file extension
 */
const char* get_mime_type(const char* file_extension);

/*
 * case_insensitive_compare
 * 	Compares two C-style strings in a case-insensitive manner checking for
 * 	equality.
 *
 * 	@string_one: The first C-style string
 * 	@string_two: The second C-style string
 *
 * 	@returns: (bool) true (1) if equal; else false (0).
 */
bool case_insensitive_compare(const char* string_one, const char* string_two);

/**
 * get_file_case_insensitive
 * 	Searches for a file in current directory case-insensitively
 * 	@file_name: a C-type string representing file name to be searched for.
 * 	@return: a C-type string representing case insensitive file names.
 */
char* get_file_name_insensitive(const char* file_name);

/**
 * url_decode
 * 	Decodes URL-encoded strings by replacing '%2x' hex sequences with the 
 * 	corresponding characters.
 *
 * 	@source: a C-type string representing the URL with '%2x' hex sequences.
 * 	@return: a C-type string representing the decoded '%2x' hex sequences.
 */
char* url_decode(const char* source);

/**
 * build_http_response
 * 	Builds the HTTP response for a given file name and file extension. 
 * 	It generates the response header, reads the file content (if exists), 
 * 	and appends it to the response buffer.
 *
 * 	@file_name: a C-type string representing file name (requested resource)
 * 	@file_extension: a C-type string representing file extension
 * 	@response: a C-type string representing the response
 * 	@response_length: length of the response
 *
 * 	@return: void
 */
void build_http_response(const char* file_name, const char* file_extension, 
		char* response, size_t* response_length);

/**
 * handle_client
 * 	It receives the HTTP request from the client, extracts the requested 
 * 	file name, and decodes the URL.
 * 	It checks if the request is a GET request using a regular expression.
 * 	If it is a valid GET request, it processes the requested file, builds 
 * 	the HTTP response, and sends it back to the client.
 * 	
 * 	@arg: the client socket file descriptor
 * 	@return: generic pointer
 */
void* handle_client(void* arg);

/**
 * get_current_time
 * 	Gives current time in string format
 * 	@format: string format representing format of date
 * 	@return: a C-type string representing the formatted date retrieved
 */
const char* get_current_time(const char* format);

#endif /* C_HTTP_SERVER_H */
