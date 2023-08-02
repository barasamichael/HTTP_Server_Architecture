#include "c_http_server.h"

#define PORT 8080
#define BUFFER_SIZE 104857600

/**
 * get_current_time
 * 	Gives current time in string format
 * 	@format: string format representing format of date
 * 	@return: a C-type string representing the formatted date retrieved
 */
const char* get_current_time(const char* format)
{
	time_t current_time;
	char* c_time_string;
	struct tm* local_time;
	char* formatted_time;

	time(&current_time);
	c_time_string = ctime(&current_time);
	if (c_time_string == NULL)
	{
		(void) fprintf(stderr, "Failure to convert the current time.\n");
		exit(EXIT_FAILURE);
	}
	c_time_string[strlen(c_time_string) - 1] = '\0';
	local_time = localtime(&current_time);
	formatted_time = (char*) malloc(80 * sizeof(char));

	if (formatted_time == NULL)
	{
		fprintf(stderr, "Memory allocation failed.\n");
		exit(EXIT_FAILURE);
	}
	strftime(formatted_time, 80, format, local_time);

	return formatted_time;
}

/**
 * get_file_extension
 *	Extracts the file extension from a file name
 *
 *	@file_name: C-style string representing a file name
 *	@return: pointer to constant character representing the file extension
 */
const char* get_file_extension (const char* file_name)
{
	// find last occurrence of the dot in the filename
	const char* dot = strrchr(file_name, '.');

	// check if dot is not found or is the first character of file_name
	if (!dot || dot == file_name)
	{
		return "";
	}
	// return pointer to first character after '.' found
	return dot + 1;
}

/**
 * get_mime_type
 * 	Determines the MIME type of a file based on its file extension.
 *	@file_extension: C-style string representing file extension
 *	@return: a pointer to a constant character representing the MIME type 
 *		associated with the given file extension
 */
const char* get_mime_type(const char* file_extension)
{
	if (strcasecmp(file_extension, "html") == 0 || 
			strcasecmp(file_extension, "htm") == 0)
	{
		return "text/html";
	}
	else if (strcasecmp(file_extension, "txt") == 0)
	{
		return "text/plain";
	}
	else if (strcasecmp(file_extension, "jpg") == 0 || 
			strcasecmp(file_extension, "jpeg") == 0)
	{
		return "image/jpeg";
	}
	else if (strcasecmp(file_extension, "png") == 0)
	{
		return "image/png";
	}
	else
	{
		return "application/octet-stream";
	}
}

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
bool case_insensitive_compare(const char* string_one, const char* string_two)
{
	// iterate through all characters as long as not null-terminated ('\0')
	// 	and characters are equal (case-insensitive).
	while (*string_one && *string_two)
	{
		if (tolower((unsigned char) *string_one) != tolower(
				(unsigned char) *string_two))
		{
			return false;
		}
		string_one++;
		string_two++;
	}
	return *string_one == *string_two;
}

/**
 * get_file_case_insensitive
 * 	Searches for a file in current directory case-insensitively
 * 	@file_name: a C-type string representing file name to be searched for.
 * 	@return: a C-type string representing case insensitive file names.
 */
char* get_file_name_insensitive(const char* file_name)
{
	DIR* dir = opendir(".");
	if (dir == NULL)
	{
		perror("opendir");
		return NULL;
	}

	struct dirent* entry;
	char* found_file_name = NULL;
	while ((entry = readdir(dir)) != NULL)
	{
		if (case_insensitive_compare(entry->d_name, file_name))
		{
			found_file_name = entry->d_name;
			break;
		}
	}
	closedir(dir);
	return found_file_name;
}

/**
 * url_decode
 * 	Decodes URL-encoded strings by replacing '%2x' hex sequences with the 
 * 	corresponding characters.
 *
 * 	@source: a C-type string representing the URL with '%2x' hex sequences.
 * 	@return: a C-type string representing the decoded '%2x' hex sequences.
 */
char* url_decode(const char* source)
{
	size_t string_length = strlen(source);
	char* decoded = malloc(string_length + 1);
	size_t decoded_length = 0;

	// decode %2x to hex
	for (size_t i = 0; i < string_length; i++)
	{
		if (source[i] == '%' && i + 2 < string_length)
		{
			int hexadecimal_value;
			sscanf(source + i + 1, "%2x", &hexadecimal_value);
			decoded[decoded_length++] = hexadecimal_value;
			i += 2;
		}
		else
		{
			decoded[decoded_length++] = source[i];
		}
	}
	// add a null terminator
	decoded[decoded_length] = '\0';
	return decoded;
}

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
		char* response, size_t* response_length)
{
	const char* mime_type = get_mime_type(file_extension);
	char* header = (char*) malloc(BUFFER_SIZE * sizeof(char));
	snprintf(header, BUFFER_SIZE, "HTTP/1.1 200 OK\r\n"
			"Content-Type: %s\r\n\r\n", mime_type);

	// if file not exists, response is 404 Not Found
	int file_fd = open(file_name, O_RDONLY);
	if (file_fd == -1)
	{
		snprintf(response, BUFFER_SIZE, "HTTP/1.1 404 Not Found\r\n"
				"Content-Type: text/plain\r\n\r\n404 Not Found");
		*response_length = strlen(response);
		return;
	}

	// get file size for content-length
	struct stat file_stat;
	fstat(file_fd, &file_stat);
	off_t file_size = file_stat.st_size;

	// copy header to response buffer
	*response_length = 0;
	memcpy(response, header, strlen(header));
	*response_length += strlen(header);

	// copy file to response buffer
	ssize_t bytes_read;
	while ((bytes_read = read(file_fd, response + *response_length, 
					BUFFER_SIZE - *response_length)) > 0)
	{
		*response_length += bytes_read;
	}
	free(header);
	close(file_fd);
}

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
void* handle_client(void* arg)
{
	int client_fd = *((int *) arg);
	char* buffer = (char *) malloc(BUFFER_SIZE * sizeof(char));

	// receive request_data from client and store into buffer
	size_t bytes_received = recv(client_fd, buffer, BUFFER_SIZE, 0);
	if (bytes_received > 0)
	{
		// check if request is GET
		regex_t regex;
		regcomp(&regex, "^GET /([^ ]*) HTTP/1", REG_EXTENDED);
		regmatch_t matches[2];

		if (regexec(&regex, buffer, 2, matches, 0) == 0)
		{
			// extract filename from request and decode URL
			buffer[matches[1].rm_eo] = '\0';
			const char* url_encoded_file_name = buffer + 
				matches[1].rm_so;
			char* file_name = url_decode(url_encoded_file_name);

			// get file extension
			char file_extension[32];
			strcpy(file_extension, get_file_extension(file_name));

			// build http response
			char* response = (char*) malloc(BUFFER_SIZE * 2 * 
					sizeof(char));
			size_t response_length;
			build_http_response(file_name, file_extension, response,
					&response_length);

			// send HTTP response to client
			send(client_fd, response, response_length, 0);
			free(response);
			free(file_name);
		}
		regfree(&regex);
	}
	close(client_fd);
	free(arg);
	free(buffer);
	return NULL;
}

/**
 * main
 * 	Sets up the server and listens for incoming connections.
 *
 * 	@argc: an integer representing the number of command-line arguments 
 * 		passed to the program. 
 * 	@argv: array of character pointers that holds the actual command-line 
 * 		arguments as C-style strings.
 *
 * 	@return: integer representing exit status
 */
int main(int argc, char* argv)
{
	int server_fd;
	struct sockaddr_in server_address;

	// create server socket
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("Socket failed");
		exit(EXIT_FAILURE);
	}

	// config socket
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = INADDR_ANY;
	server_address.sin_port = htons(PORT);

	// bind server to port
	if (bind(server_fd, (struct sockaddr *) &server_address, 
			sizeof(server_address)) < 0)
	{
		perror("Bind failed");
		exit(EXIT_FAILURE);
	}

	// listen for connections
	if (listen(server_fd, 10) < 0)
	{
		perror("Listen failed");
		exit(EXIT_FAILURE);
	}

	// Display success message
	printf("-------------------------------------------------------------\n");
	printf("                                                             \n");
	printf("              SIMPLE HTTP SERVER Prototype 0.0.1             \n");
	printf("                                                             \n");
	printf("-------------------------------------------------------------\n");
	printf(" * Server initiated at %s\n", 
			get_current_time("%a %d %B %Y %I:%M:%S %p"));
	printf(" * Listening on port %d ...\n", PORT);
	printf(" * Browse to http://127.0.0.1:%d to access the server\n", PORT);
	printf(" * POWERED BY: Kamau Ngengi Enterprises\n\n");

	while (1)
	{
		// Client Information
		struct sockaddr_in client_address;
		socklen_t client_address_length = sizeof(client_address);
		int* client_fd = malloc(sizeof(int));

		// Accept client connections
		if ((*client_fd = accept(server_fd, (struct sockaddr*) 
			&client_address, &client_address_length)) < 0)
		{
			perror("Accept failed");
			continue;
		}

		// Create a new thread to handle client request
		pthread_t thread_id;
		pthread_create (&thread_id, NULL, handle_client, 
				(void *) client_fd);
		pthread_detach(thread_id);
	}
	close(server_fd);
	return 0;
}
