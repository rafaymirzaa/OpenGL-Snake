/*
Abdur Rafay Mirza
23I-6009
DS-C
*/

#ifndef TETRIS_CPP_
#define TETRIS_CPP_
#include "util.h"
#include <iostream>
#include<vector>
#include<algorithm>
#include<ctime>
#include<string>
#include<unistd.h>
#include<sstream>
#include<cmath>     
#include <cstring>
#include <fstream>
#define KEY_ESC 27


using namespace std;

void SetCanvasSize(int width, int height); // taken down below for neatness

                                                      // GLOBAL VARIABLES

double startx=320,starty=400;

time_t obstDisplay; // obstacle respawing
time_t foodDisplay; // stores food/obstacle for when they display after 15 seconds or 30 seconds 
 
                             // CONSTANTS
const int maxSnakeHeight = 100, maxFood = 5 , maxBonus = 1 , maxObst = 3;

                             // INTEGERS
int snakeLength = 3, xvertex, yvertex, currentSnakeX = startx, currentSnakeY = starty;

                         // ARRAYS DECLARED
int foodArr[maxFood][2];  // 2 represents x,y coordinates
int snakeArr[maxSnakeHeight][2]; // same here for segments of snake body
int bonusArr[maxBonus][2];
int obstArr[maxObst][2];

                         // FLAGS
bool death = false; // flag to control game over and end message on screen 
bool pauseButton = false; // flag for pause button
bool Food_disappear = true;
bool initialGame = false;


void Display()
{
   glClearColor(0.6, 0.8, 1.0, 0); 
    glClear(GL_COLOR_BUFFER_BIT);                                      
       
   string scoreStr = "Score: " + to_string((snakeLength - 3) * 5);  // string concatenation
 string scoreStr1 = "High Score: " + to_string((snakeLength - 3) * 5);
 if (initialGame == 0)
 {
 DrawString( 300, 360, "PRESS 'S' TO START GAME" , colors[BLACK]);
 }

 
   
   DrawString( 50, 600, scoreStr , colors[BLACK]); // for score 
    DrawString( 580, 600, scoreStr1 , colors[BLACK]); // for high score
   DrawCircle(currentSnakeX + 10, currentSnakeY + 10, 15, colors[BLACK]);
   
   for (int i = 0; i < snakeLength; i++) {
 
   	DrawSquare(snakeArr[i][0], snakeArr[i][1], 20, colors[BLACK]);
      	}
  
    	
   // border detection  (use this if you want snake to die when it collides with the boundaries)
   
   /*if (snakeArr[0][0] < 0 || snakeArr[0][0] >= 650 || snakeArr[0][1] < 0 || snakeArr[0][1] >= 650) {
   cout << "You killed yourself"<<endl;
        exit(0);
   }*/
                                                        // TELEPORTATION
                                                        
                                                        
if (snakeArr[0][0] < 0) 
{
    snakeArr[0][0] = 630; // Teleport to the right side
} 
else if (snakeArr[0][0] >= 650) 
{
    snakeArr[0][0] = 0; // Teleport to the left side
}

if (snakeArr[0][1] < 0) 
{
    snakeArr[0][1] = 630; // Teleport to the bottom
} else if (snakeArr[0][1] >= 650) 
{
    snakeArr[0][1] = 0; // Teleport to the top
}
currentSnakeX = snakeArr[0][0];
currentSnakeY = snakeArr[0][1];  // initalize the snake's x and y coordinates after they have succesfully teleported :D

   
   for(int i = 0 ; i < maxFood ; i++) 
   {            
                                                          // DEBUG STATEMENTS
                                                     //(abs function is used to prevent any negative indexing of the values)
      /*cout << "CurrentSnakeY "<<currentSnakeY<<endl;
 	cout << "FoodY "<<foodArr[i][1] << endl;
 	cout <<endl<<"CurrentSnakeX "<<currentSnakeX<<endl;
 	cout << "FoodX "<<foodArr[0][i] << endl;*/
 	
 	if (abs(currentSnakeY - foodArr[i][1]) <= 20 && abs(currentSnakeX - foodArr[i][0]) <= 20) { 
 	    // Here i have used the absolute difference formula the y part checks that if the difference is less than 20 pixels if it then collision is valid the number can be made more precise or not depending on me or whoever is reading this code
 	
        	snakeLength++;
        	
        	
        	foodArr[i][0] = rand() % 600;
        	foodArr[i][1] = rand() % 600;
   	}
   	DrawSquare(foodArr[i][0], foodArr[i][1], 20, colors[AZURE]); 
   }
     
            // For displaying our BONUS food option here
    for(int i = 0 ; i < maxBonus ; i++) 
   {
 	if (abs(currentSnakeY - bonusArr[i][1]) <= 20 && abs(currentSnakeX - bonusArr[i][0]) <= 20) {
 	
        	snakeLength += 2;
        	
        	bonusArr[i][0] = rand() % 600;
        	bonusArr[i][1] = rand() % 600;
        	 DrawString(300, 340, "Bonus Detected", colors[BLACK]);  // when snake collides with the bonus point this prints a message on the screen
   	            }
   	
   	DrawSquare(bonusArr[i][0], bonusArr[i][1], 20, colors[RED]); // used to redraw square func
   	
   }
   
                                          // OBSTACLE DETECTION
   for(int i = 0 ; i < maxObst ; i++) 
   {
     if (abs(currentSnakeY - obstArr[i][1]) <= 20 && abs(currentSnakeX - obstArr[i][0]) <= 20) {  
 	
        	
        	death = true;
        	obstArr[i][0] = rand() % 600;
        	obstArr[i][1] = rand() % 600;
   	}
   	DrawLine(obstArr[i][0], obstArr[i][1], obstArr[i][0] + 50, obstArr[i][1], 20, colors[GREEN_YELLOW]); // 20 is thickness + 50 used to add horizontal componenet length 
   }

                                             // DEATH
    for (int i = 1; i < snakeLength; i++) {
       if (currentSnakeX == snakeArr[i][0] && currentSnakeY == snakeArr[i][1]) {  // if current position of the snake is equal to any one of the i coordinate in loop then it dies
          death = true;
          break;
       }
   }

   	
    if (death) {
        DrawString(300, 340, "GAME OVER!!", colors[BLACK]); // incase of collision with self this displays on screen stopping all funcs
        DrawString(280, 320, "PRESS ESC TO EXIT", colors[BLACK]);
        glutSwapBuffers();
        return;
    }

     if (pauseButton) {
        DrawString(300 , 340 , "Game Paused" , colors[BLACK]);
    }
   glutSwapBuffers(); // do not modify this line..
}



 int snakeMove = 0;
 

void NonPrintableKeys(int key, int x, int y) {
    if (key == GLUT_KEY_LEFT && snakeMove != 2) {  // if snake is moving left and you press right it wont work because of condition this was done to prevent the premature killing of snake
        snakeMove  = 1;  
    } 
    else if (key == GLUT_KEY_RIGHT && snakeMove != 1) { 
        snakeMove = 2;  
    }
    else if (key == GLUT_KEY_UP && snakeMove != 4) {
        snakeMove = 3; 
    } 
    else if (key == GLUT_KEY_DOWN && snakeMove != 3) {
        snakeMove  = 4;  
    }
}

void snakeMovement() 
{

   if (snakeMove == 1) {
        xvertex = -20; // moving -20 on x axis towards negative side
        yvertex = 0;
        currentSnakeX -= 20;
    } 
    else if (snakeMove == 2) {
        xvertex = 20;
        yvertex = 0;
        currentSnakeX += 20;
    } 
    else if (snakeMove == 3) {
        xvertex = 0;
        yvertex = 20;
        currentSnakeY += 20;
    } 
    else if (snakeMove == 4) {
        xvertex = 0;
        yvertex = -20;
        currentSnakeY -= 20;
    } 
    else {
        return;
    }

    for (int i = snakeLength ; i > 0; i--)  // this loop ensures that the body trails with each other in an orderly manner
    {
        snakeArr[i][0] = snakeArr[i - 1][0]; // - 1 ensures coordinates to be the same as preceeding one 
        snakeArr[i][1] = snakeArr[i - 1][1];
    }

    snakeArr[0][0] += xvertex;
    snakeArr[0][1] += yvertex;
}


void PrintableKeys(unsigned char key, int x, int y) {
    if (key == KEY_ESC) {
        exit(1);
     }
   

if (key == 'P' || key == 'p') {
        pauseButton = !pauseButton;  // Toggle the pause state works as a button of sorts
        
        glutPostRedisplay();
    }

if (initialGame == 0 && key == 's' || key == 'S') {  // key to start game 

       initialGame = true;
        snakeMove = 1;
    }

    if (initialGame) {
        if (key == GLUT_KEY_LEFT && snakeMove != 2) {
            snakeMove = 1;
        } else if (key == GLUT_KEY_RIGHT && snakeMove != 1) {
            snakeMove = 2;
        } else if (key == GLUT_KEY_UP && snakeMove != 4) {
            snakeMove = 3;
        } else if (key == GLUT_KEY_DOWN && snakeMove != 3) {
            snakeMove = 4;
        }
    }
}
    
    
    

void Timer(int m) {
    if ( pauseButton == false) { 
        snakeMovement(); // snake keeps moving if and only of it hasnt been paused and or collided with itself
        
       
        
        time_t currentTime = time(NULL); // since january 1970!!
       
        if (currentTime - foodDisplay >= 15) // conditon to see if 15 seconds have passed or not
        {
            Food_disappear = false; // food disappears
            foodDisplay = currentTime; // update the last food appearance time
        }
        
       

        if (Food_disappear == false) {    //  print food in a new  position
            for (int i = 0; i < maxFood; i++) {
                foodArr[i][0] = rand() % 600;
                foodArr[i][1] = rand() % 600;
           
            }
   
            Food_disappear = true; // food reappears
        }
      
         if (currentTime - obstDisplay >= 15) {  // Respawn the obstacle
    for (int i = 0; i < maxObst; i++) {
        obstArr[i][0] = rand() % 600;
        obstArr[i][1] = rand() % 600;
    }
    
    obstDisplay = currentTime; // this updates the last obstacles time on the screen
}
        
         if (death) 
        {
        return;  // if snake dies then it returns from the display function and no code runs hence paused screen 
        }

        glutPostRedisplay();
    }

    glutTimerFunc(1000.0 / (2*FPS), Timer, 0); // for a more smoother gameplay we can multiply the frames per second
}


int main(int argc, char*argv[]) 
{
    int width = 650, height = 650;
    for (int i = 0; i < snakeLength; i++) 
    {
        snakeArr[i][0] = startx + i * 20; // multiplied it by 20 to ensure spacing 
        snakeArr[i][1] = starty;
    }
    for (int i = 0 ; i < maxFood ; i++) 
    {
    	foodArr[i][0] = rand() % 600;   // initialzie the food x pos
    	foodArr[i][1] = rand() % 600;  // initialize the food y pos
    }
 
	for (int i = 0 ; i < maxBonus ; i++) 
    {
    	bonusArr[i][0] = rand() % 600;   
    	bonusArr[i][1] = rand() % 600; 
    }
       for (int i = 0 ; i < maxObst ; i++) 
    {
    	obstArr[i][0] = rand() % 600;   
    	obstArr[i][1] = rand() % 600; 
    }
    
    obstDisplay = time(NULL);
    foodDisplay = time(NULL);
    InitRandomizer();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(50, 50);
    glutInitWindowSize(width, height);
    glutCreateWindow("Rafay's Snake Game");
    SetCanvasSize(width , height); 
    glutDisplayFunc(Display);
    glutSpecialFunc(NonPrintableKeys);
    glutKeyboardFunc(PrintableKeys);
    glutTimerFunc(5.0 / FPS, Timer, 0);
    
    glutMainLoop();
    
    return 1;
}

void SetCanvasSize(int width, int height)
 {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, 0, height, -1, 1);
    glMatrixMode( GL_MODELVIEW);
    glLoadIdentity();
  }

#endif 
