/* 
	settings: include in 'compiler options' : -lWs2_32, or use pragma
	file: shtcp.c ( seudoshell - tcp)
	
	example:
	
		shtcp.exe listen 192.168.18.19 12045
		shtcp.exe connect 192.168.18.25 12045	
	
	notes:
		use a service or window_proc(~gui)
		No system() or ShellExecute()
		check all arguments
		set a function to the commands execution
	
*/

#include <stdio.h>
#include <stdlib.h>
#include <winsock.h>
#include <string.h>

#define DEFAULT_LEN 100
#define COMMAND_LEN 8000



void set_socket( char* _type, char* _ip, unsigned short _port);
int checkarg( char* _arg, char* _str);
	
int main( int argc, char* argv[] )
{

	char* IP = argv[2]; // no checked
	
	// get port of argv[3]
	char* tmp;
	unsigned long qPORT = strtoul( argv[3], &tmp, 10);
	unsigned short PORT = (unsigned short)qPORT;
	
	
	set_socket( argv[1], IP, PORT);


}


int checkarg( char* _arg, char* _str){

	unsigned int i=0;

	while( *( &_arg[i] ) != '\0' ){
		
		if( *( &(_arg[i]) ) != *(&_str[i]) ){
			return -1;
		}
		++i;
	
	}
	
	return i; // number of chars
	
}

void set_socket(char* _type, char* _ip, unsigned short _port){
	
	WSADATA wsa;
	WSAStartup( 0x0202, &wsa);
	
	SOCKET sID = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP);
	
	struct sockaddr_in sDATA; 
	sDATA.sin_family = AF_INET; 
	sDATA.sin_port = htons(_port);
	sDATA.sin_addr.S_un.S_addr = inet_addr(_ip);
	
	int tmp; 
	
	
	
	if( checkarg(_type, "connect") != -1 ){
		
		tmp = connect( sID, (struct sockaddr*)&sDATA, sizeof(sDATA) );
		
		char command[DEFAULT_LEN]; memset( command, 0, sizeof(command));
		char buffer[COMMAND_LEN]; memset( buffer, 0, sizeof(buffer));
		printf("command: "); fgets( command, DEFAULT_LEN, stdin);
		
		tmp = send( sID, command, (int)strlen(command), 0);
		
		while( (tmp = recv( sID, buffer, COMMAND_LEN, 0)) > 0){
			
			printf("server: %s\n", buffer);
			printf("command: ");
				
			memset(command, 0, sizeof(command));
		
			fgets(command, DEFAULT_LEN, stdin);
			
			tmp = send( sID, command, (int)strlen(command), 0 );
			
			memset( buffer, 0, sizeof(buffer) );
		
		}
	
	
		if( tmp == 0) printf("closed connection\n");
		if( tmp < 0) printf("any error\n");
	
		WSACleanup();

	} else if ( checkarg( _type, "listen") != -1){
	
		
		bind( sID, (struct sockaddr *)&sDATA, sizeof(sDATA));
		listen( sID, SOMAXCONN);
		SOCKET clientID = accept( sID, NULL, NULL);
		
		char buffer[DEFAULT_LEN]; 
		char add[] = "2>&1";
		char c='\n';
		char* ptr; 
	
		while( ( tmp = recv( clientID, buffer, DEFAULT_LEN, 0 ) ) > 0 ){
					
			ptr = strchr( buffer, c);
			
			*ptr = ' ';
			
			strcat( buffer, add);
				
			//------------------------------------------------------------------
			
			FILE* process;
			
			process = _popen( buffer, "r"); 
			
			char* output = NULL;
			char line[DEFAULT_LEN + DEFAULT_LEN]; 
			size_t output_len=0;
			
			while (fgets(line, sizeof(line), process) != NULL) {
	    	
		    	output = realloc( output, output_len + strlen(line) + 1 );
		    	
		    	strcpy( output + output_len, line);
				
				output_len += strlen(line);
	
			}
					
			//------------------------------------------------------------------
			
				
			memset( buffer, 0, sizeof(buffer));
			
			tmp = send( clientID, output, (int)strlen(output), 0);
			
			_pclose(process);
			free(output);
		
		}
	
		if( tmp == 0) printf("closed connection\n");
	
		if( tmp < 0 ) printf("any error\n");
	    
	
		closesocket( clientID);
		closesocket( sID);
		
		WSACleanup();
	
	
	} // end of else
	
	
	
} // end of set_socket() 






