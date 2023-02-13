#include "./../include/processA_utilities.h"
#include <stdio.h>
#include <bmpfile.h>
#include <stdlib.h>
#include <math.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h> // inet_addr()
#include <netdb.h>
#include <strings.h> // bzero()
#include <sys/socket.h>


#define width 1600	// width of the image (in pixels)
#define height 600 	// Height of the image (in pixels)
#define depth 4		// Depth of the image (1 for greyscale images, 4 for colored images)
#define SIZE 80
#define SA struct sockaddr





// Function to draw the bmp circle
void draw_bmp(bmpfile_t *bmp, int x_pos, int y_pos)

{
	// Data type for defining pixel colors (BGRA)
  	rgb_pixel_t pixel = {255, 0, 0, 0}; // Blue

	// Code for drawing a centered circle of given radius...
  	int radius = 50;
  	for(int x = -radius; x <= radius; x++) {
    		
		for(int y = -radius; y <= radius; y++) {
      			
			// If distance is smaller, point is within the circle
      			if(sqrt(x*x + y*y) < radius) {
          		
			// Color the pixel at the specified (x,y) position moltiply times 20  with the given pixel values
         		 bmp_set_pixel(bmp,  x_pos + x,  y_pos + y, pixel);

      }
    }
  }
}


// Function for painting the background to delete the old bitmap
void paint_background(bmpfile_t *bmp)
{

    // Data type for defining pixel colors (BGRA)
    rgb_pixel_t pixel = {255, 255, 255, 0};  // White

    // painting
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            bmp_set_pixel(bmp, i, j, pixel);
        }
    }
}


int main(int argc, char *argv[])
{
	int input_x;
	int input_y;

	void *shared_memory;
	void *shared_memory2;
	char buff[100];
	char buff2[100];
	char buffer[SIZE];
	int shmid, shmid2;

	
	
	
// choose the Mode: 0 (2 assignment), 1 (client), 2 (server)	

int TYPE;
printf("Choose the Mode: 0 (2 assignment), 1 (client), 2 (server)");
scanf("%d", &TYPE);
	

			/* 	       SHARED MEMORY           */

if (TYPE == 0)	{

// Shared memory name: shared_memory manage x position while shared_memory2 y position

	//creates shared memory segment with key 2345, having size 1024 bytes. IPC_CREAT is used to create the shared segment if it does not exist. 0666 are the permisions on the shared segment
	shmid=shmget((key_t)2345, 1024, 0666|IPC_CREAT); // x pos
	// the same but with key 5432  
	shmid2=shmget((key_t)5432, 1024, 0666|IPC_CREAT); // y pos


	//process attached to shared memory segment
	shared_memory=shmat(shmid,NULL,0); 
	shared_memory2=shmat(shmid2,NULL,0); 

	    
	    // convert int to chart
	    sprintf(buff,"%d", input_x);
	    sprintf(buff2,"%d", input_y);
	    
	    // Write into the memory segment
	    strcpy(shared_memory,buff); //x pos written to shared memory
	    
	    strcpy(shared_memory2,buff2); //y pos written to shared memory
 
 
 
    	
    	
    		/* 	       BITMAP           */
    		

	// Data structure for storing the bitmap file
 	bmpfile_t *bmp;

	// Instantiate bitmap, passing three parameters
	bmp = bmp_create(width, height, depth);
   
    
    // Utility variable to avoid trigger resize event on launch
    int first_resize = TRUE;

    // Initialize UI
    init_console_ui();

	// start position to be in the middle of the image
	int start_x = width/2;
	int start_y = height/2;
	
	// Draw the circle 
    draw_bmp(bmp, start_x, start_y);



    // Infinite loop
    while (TRUE)
    {
        // Write into the memory segment
	strcpy(shared_memory,buff); //x pos written to shared memory 
    strcpy(shared_memory2,buff2); //y pos written to shared memory
	    
       
       
        // Get input in non-blocking mode
        int cmd = getch();

        // If user resizes screen, re-draw UI...
        if(cmd == KEY_RESIZE) {
            if(first_resize) {
                first_resize = FALSE;
            }
            else {
                reset_console_ui();
            }
        }

        // Else, if user presses print button...
        else if(cmd == KEY_MOUSE) {
            if(getmouse(&event) == OK) {
                if(check_button_pressed(print_btn, &event)) {

			// SNAPSHOT: Save image as .bmp file
  			bmp_save(bmp, "out/circle.bmp");


                    mvprintw(LINES - 1, 1, "Print button pressed");
                    refresh();
                    sleep(1);
                    for(int j = 0; j < COLS - BTN_SIZE_X - 2; j++) {
                        mvaddch(LINES - 1, j, ' ');
                    }
                }
            }
        }

        // If input is an arrow key, move circle accordingly...
        else if(cmd == KEY_LEFT || cmd == KEY_RIGHT || cmd == KEY_UP || cmd == KEY_DOWN) {
            move_circle(cmd);
            draw_circle();

	    // paint background
	    paint_background(bmp);

            // Draw the circle
            draw_bmp(bmp,  20*circle.x, 20*circle.y);
	    
	    
	    // convert int to chart
	    sprintf(buff,"%d", circle.y);
	    sprintf(buff2,"%d", circle.x);
	    
	    
	    
	  } 
    }
    
	// Free resources before termination
 	bmp_destroy(bmp);

    	
    endwin();
    
 }   
    
    



    
    
    
    
    			/* 	   TCP - CLIENT          */
		
if (TYPE == 1){

	// get the server address
	char HOST[80];
	struct hostent *server;
	printf("Insert the host name:");
	scanf("%s", HOST);

	// get the port number
	int PORT;
	printf("Insert the port number:");
	scanf("%d", &PORT);
	server = gethostbyname(HOST);


	// Utility variable to avoid trigger resize event on launch
    int first_resize = TRUE;

    // Initialize UI
    init_console_ui();
    	
    	
	int sockfd;
	int connfd;
	struct sockaddr_in servaddr, cli;
        
    // socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        mvprintw(LINES - 1, 1,"socket creation failed...\n");
        refresh();
        sleep(1);
        exit(0);
    }
    else
        mvprintw(LINES - 1, 1,"Socket successfully created..\n");
        refresh();
        sleep(1);
        
    bzero(&servaddr, sizeof(servaddr));
            
    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(HOST);
    servaddr.sin_port = htons(PORT);
            
    // connect the client socket to server socket
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr))
        != 0) {
        mvprintw(LINES - 1, 1,"connection with the server failed...\n");
        refresh();
        sleep(1);
        exit(0);
    }
    else
        mvprintw(LINES - 1, 1,"connected to the server..\n");
    	refresh();
        sleep(1);
    
    
    
    	// Infinite loop
    	while (TRUE){
		    
		    // Get input in non-blocking mode
        	int cmd = getch();

		
        	// If user resizes screen, re-draw UI...
        	if(cmd == KEY_RESIZE) {
            	
            	if(first_resize) {
                	first_resize = FALSE;
            	}
            	else {
                	reset_console_ui();
            	}
       	 	}

        	// Else, if user presses print button...
        	else if(cmd == KEY_MOUSE) {
            	if(getmouse(&event) == OK) {
                if(check_button_pressed(print_btn, &event)) {

				// Send keyboard input: value sent 409
				sprintf(buffer,"%d", cmd); 
	    			write(sockfd, buffer, sizeof(buffer));
				fflush(stdout);
				
				mvprintw(LINES - 1, 1, "Print button pressed");
                		refresh();
                		sleep(1);
                    
                    
			}
                   }  
            	}
        

        	// If input is an arrow key, move circle accordingly...
        	else if(cmd == KEY_LEFT || cmd == KEY_RIGHT || cmd == KEY_UP || cmd == KEY_DOWN) {
           
				move_circle(cmd);
            			draw_circle();
            
            		// Send keyboard input: value  left 260, right 261, up 259, down 258
	    		sprintf(buffer,"%d", cmd); 
	    		write(sockfd, buffer, sizeof(buffer));
	    		fflush(stdout);
				
			mvprintw(LINES - 1, 1, "Arrow button pressed");
                	refresh();
               		sleep(1);
			
			} 			
		} 
    
    
    // close the socket
    close(sockfd);

    	
    endwin();
    
 }   
		
		




	




				/* 	   TCP - SERVER         */
		
if (TYPE == 2){	 
   

	// get the port number
	int PORT;
	printf("\n Insert the port number:");
	scanf("%d", &PORT);

   // SHARED MEMORY name: shared_memory manage x position while shared_memory2 y position

	//creates shared memory segment with key 2345, having size 1024 bytes. IPC_CREAT is used to create the shared segment if it does not exist. 0666 are the permisions on the shared segment
	shmid=shmget((key_t)2345, 1024, 0666|IPC_CREAT); // x pos
	// the same but with key 5432  
	shmid2=shmget((key_t)5432, 1024, 0666|IPC_CREAT); // y pos


	//process attached to shared memory segment
	shared_memory=shmat(shmid,NULL,0); 
	shared_memory2=shmat(shmid2,NULL,0); 

	    
	// convert int to chart
    	sprintf(buff,"%d", input_x);	
    	sprintf(buff2,"%d", input_y);
	    
	// Write into the memory segment
	strcpy(shared_memory,buff); //x pos written to shared memory
	    
	strcpy(shared_memory2,buff2); //y pos written to shared memory
 
 
 
    	
    	
    		/* 	       BITMAP           */
    		

	// Data structure for storing the bitmap file
 	bmpfile_t *bmp;

	// Instantiate bitmap, passing three parameters
	bmp = bmp_create(width, height, depth);
   
    
    	// Utility variable to avoid trigger resize event on launch
    	int first_resize = TRUE;

    	// Initialize UI
    	init_console_ui();

	// start position to be in the middle of the image
	int start_x = width/2;
	int start_y = height/2;
	
	// Draw the circle 
    	draw_bmp(bmp, start_x, start_y);





   // SOCKET

	int sockfd, connfd, len;
	struct sockaddr_in servaddr, cli;
	
	// socket create and verification
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		mvprintw(LINES - 1, 1,"socket creation failed...\n");
		refresh();
        	sleep(1);
		exit(0);
	}
	else
		mvprintw(LINES - 1, 1,"Socket successfully created..\n");
		refresh();
        sleep(1);
        
	bzero(&servaddr, sizeof(servaddr));
		
	// assign IP, PORT
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(PORT);
		
	// Binding newly created socket to given IP and verification
	if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
		mvprintw(LINES - 1, 1,"socket bind failed...\n");
		refresh();
        	sleep(1);
		exit(0);
	}
	
	else
		mvprintw(LINES - 1, 1,"Socket successfully binded..\n");
		refresh();
        	sleep(1);
		
	// Now server is ready to listen and verification
	if ((listen(sockfd, 5)) != 0) {
		mvprintw(LINES - 1, 1,"Listen failed...\n");
		refresh();
        	sleep(1);
		exit(0);
	}

	else
		mvprintw(LINES - 1, 1,"Server listening..\n");
		refresh();
        	sleep(1);
		len = sizeof(cli);
			
	// Accept the data packet from client and verification
	connfd = accept(sockfd, (SA*)&cli, &len);
	if (connfd < 0) {
		mvprintw(LINES - 1, 1,"server accept failed...\n");
		refresh();
        	sleep(1);
		exit(0);
	}
	else
		mvprintw(LINES - 1, 1,"server accept the client...\n");
		refresh();
        	sleep(1);





	// Loop for chatting between client and server
	
	while (1) {

		// Write into the memory segment
		strcpy(shared_memory,buff); //x pos written to shared memory 
    		strcpy(shared_memory2,buff2); //y pos written to shared memory
	    
		bzero(buffer, SIZE);
		
		// read the message from client and copy it in buffer
		read(connfd, buffer, sizeof(buffer));
		
		// print buffer which contains the client contents
		mvprintw(LINES - 1, 1,"Received from client: %s ", buffer);
	
		int cmd = atoi(buffer);
		
		// cmd values: left 260, right 261, up 259, down 258, print 409		 
		
		
		
       // Button: PRINT
       if(cmd == KEY_MOUSE || cmd == 409) {
            
		// SNAPSHOT: Save image as .bmp file
  		bmp_save(bmp, "out/circle.bmp");

		mvprintw(LINES - 1, 1, "Print button pressed");
            	refresh();
           	 sleep(1);
         }

        // Arrow key: KEY_LEFT
        else if(cmd == KEY_LEFT || cmd == 260) {
            
		cmd == KEY_LEFT;

		move_circle(cmd);
            	draw_circle();

	    	// paint background
	    	paint_background(bmp);

        	// Draw the circle
        	draw_bmp(bmp,  20*circle.x, 20*circle.y);
	    
	    
	    	// convert int to chart to send trough shared memory
	    	sprintf(buff,"%d", circle.y);
	    	sprintf(buff2,"%d", circle.x);
	    
	    } 

	// Arrow key: KEY_RIGHT
        else if(cmd == KEY_RIGHT || cmd == 261) {
            
		cmd == KEY_RIGHT;

		move_circle(cmd);
            	draw_circle();

	    	// paint background
	    	paint_background(bmp);

        	// Draw the circle
        	draw_bmp(bmp,  20*circle.x, 20*circle.y);
	    
	    
	    	// convert int to chart to send trough shared memory
	    	sprintf(buff,"%d", circle.y);
	    	sprintf(buff2,"%d", circle.x);
	    
	    } 

	// Arrow key: KEY_UP
        else if(cmd == KEY_UP || cmd == 259) {
            
		cmd == KEY_UP;

		move_circle(cmd);
            	draw_circle();

	    	// paint background
	    	paint_background(bmp);

        	// Draw the circle
        	draw_bmp(bmp,  20*circle.x, 20*circle.y);
	    
	    
	    	// convert int to chart to send trough shared memory
	    	sprintf(buff,"%d", circle.y);
	    	sprintf(buff2,"%d", circle.x);
	    
	    } 

	// Arrow key: KEY_DOWN
        else if(cmd == KEY_DOWN || cmd == 258) {
            
		cmd == KEY_DOWN;
	
		move_circle(cmd);
            	draw_circle();

	    	// paint background
	    	paint_background(bmp);

        	// Draw the circle
        	draw_bmp(bmp,  20*circle.x, 20*circle.y);
	    
	    
	    	// convert int to chart to send trough shared memory
	    	sprintf(buff,"%d", circle.y);
	    	sprintf(buff2,"%d", circle.x);
	    
	    } 

		sleep(1);


    }
    
	// Free resources before termination
 	bmp_destroy(bmp);
	
	// close the socket
    	close(sockfd);
	
   	endwin();
    
 }   
    
    
    
    return 0;
    
}
