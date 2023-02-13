# ARP-3rd_Assignment - Group AA98

Andrea Bolla 4482930

Anna Possamai 5621738

# Introduction
The task is to design, develop, test and deploy an interactive simulator of a (simplified) typical vision system, as the second assignment, able to track an object in a 2-D plane. This is a true example of an industrial vision system in a robotic manufacturing workshop.

This assignment requires the use of a shared memory in which two processes operate simultaneously, as happens in reality in similar applications, including TCP client/server features.

In our case we don't have a camera, so we will simulate the creation of the moving image using an ncurses window. Using arrow keys, we will move a spot in a window to simulate the perception of the camera. The spot that we will see by moving will produce the creation of a realistic RGB image (a circle in this case) in the simulated, shared, video memory.
We will use an 80 x 30 ncurses window. The video memory, for realism, will be 1600 x 600 pixels RGB true color 3 (3 bytes/pixel). There will then be a factor of 20x in the location of the interactive spot and the corresponding image in the video memory.

- In the modified application, process B, shared memory and the second ncurses window are unchanged. 
- Process A includes two new features:
1. client connection toward a similar application running on a different machine in the network
2. server connection for a similar application running on a different machine in the network


Therefore the application, when launched, asks for one execution modality:
0. normal, as assignment 2
1. client: the application runs normally as assignment 2 and sends its keyboard input to another
application (on a different machine) running in server mode
2. server: the application does not use the keyboard for input: it receives input from another
application (on a different machine) running in client mode

When selecting modes 1 and 2 the application obviously asks address and port of the companion application.

# Protocol
To assure that any application is able to properly connect to any other application (implemented by some other student/group), a communication protocol must be defined.
- IP protocol: TCP
- data: a byte stream of char, one per key pressed.

# Structure

1. The master process responsible of spawning the other process
2. Process A, which simulates capture from a video camera and fills the video memory.
3. Process B will simulate the extraction of a feature from the acquired moving image instead. For simplicity, the image in the video memory will be scanned and the center of the image will be extracted. 

Two ncurses window will spawn:
1. Using arrow keys, we will move a spot in a window to simulate the perception of the camera. The spot that we will see by moving will produce the creation of a realistic RGB image. By pressing a key, or by operating the mouse on a button, a snapshot of the image memory will be saved on a .bmp file.
2. In a second ncurses window, also 80 x 30, the position trace of the center of the image will be shown.










# *libbitmap* installation and usage
To work with the bitmap library, you need to follow these steps:
1. Download the source code from [this GitHub repo](https://github.com/draekko/libbitmap.git) in your file system.
2. Navigate to the root directory of the downloaded repo and run the configuration through command ```./configure```. Configuration might take a while.  While running, it prints some messages telling which features it is checking for.
3. Type ```make``` to compile the package.
4. Run ```make install``` to install the programs and any data files and documentation.
5. Upon completing the installation, check that the files have been properly installed by navigating to ```/usr/local/lib```, where you should find the ```libbmp.so``` shared library ready for use.
6. In order to properly compile programs which use the *libbitmap* library, you first need to notify the **linker** about the location of the shared library. To do that, you can simply add the following line at the end of your ```.bashrc``` file: ```export LD_LIBRARY_PATH="/usr/local/lib:$LD_LIBRARY_PATH"```     


# Using *libbitmap* in your code

Now that you have properly installed the library in your system, it's time to use it in your programs:
1. Include the library in your programs via ```#include <bmpfile.h>```. If you want to check the content of ```bmpfile.h``` to glimpse the functionalities of the library, navigate to ```/usr/local/include```, where the header file should be located.
2. Compile programs which use the *libbitmap* library by linking the shared library with the ```-lbmp``` command.     
Example for compiling **circle.c**: ```gcc src/circle.c -lbmp -lm -o bin/circle``` 

# Compiling and running 
The two processes are implemented as UIs through *ncurses* library, therefore you need to compile their source files by linking the shared library via ```-lncurses```. As for the first assignment, exploit the resize event of the windows to get out of situations in which the graphical elements do not properly spawn.

To compile the code type:
     
     ./compile.sh
     
then to run it:
     
     ./run.sh


# Code
     
The code work perfectly on the localhost both as server and as client, we have to try on different machine.

We've noticed that when we are in client mode and we press the arrow or the print button, we send and receive the following integer: 

     values: 
     Arrow    left 260, right 261, up 259, down 258
     Button   print 409	
     


this is why we made the following code in server mode, we didn't understand if for example the ```KEY_DOWN``` value is equal to 258 or not:     
     
    // read the message from client and copy it in buffer
    read(connfd, buffer, sizeof(buffer));
    
    // Conversion to integer
	int cmd = atoi(buffer);
		
    // Arrow key: KEY_DOWN
    else if(cmd == KEY_DOWN || cmd == 258) {
            
	 cmd == KEY_DOWN;

               .
               .
               .


than we setted ```cmd``` equal to ```KEY_DOWN``` because in the file header there is the formula  responsible for the creation of both the values ```circle.x``` and ```circle.y``` that use the KEY_ input.  Then, we use it with the shared memory to move the circle center in the ncurse window of the process B.


# Improvement

A possible improvement could be the creation of functions to tidy up the code and make it more clear, for example function with inside the code for the  TCP/Socket creation and the same for the shared memory...
In addition, a check could be added to verify that the values entered are those required and, if not, print an error message on the terminal and ask the user to re-enter the values.