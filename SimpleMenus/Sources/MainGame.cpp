#include <iostream>
#include "Frog.h"
#include<stdio.h>
#include <stdlib.h>
#include <conio.h> 
#include "MainGame.h"
#include "MainUpdate.h"

using namespace Webfoot;

/// Speed of the ball along a given axis in pixels per second.
#define BALL_AXIS_SPEED 700.0f
#define BALL_AXIS_SPEED2 400.0f

//--------------------------------------------


MainGame MainGame::instance;

//==============================================================================

/// Main GUI
#define GUI_LAYER_NAME "MainGame"

//-----------------------------------------------------------------------------

MainGame::MainGame()
{
   ball = NULL;
   ghostball = NULL;
   player1 = NULL;
   player2 = NULL;
}

//-----------------------------------------------------------------------------

void MainGame::Init()
{
	
   Inherited::Init();


   // Create and initialize the player/ball.
   player1 = frog_new Ball(); 
   player2 = frog_new Ball();
   player1->Init();
   player2->Init();
   ball = frog_new Ball();
   ghostball = frog_new Ball();
   ball->Init();
   ghostball->Init();
}

//-----------------------------------------------------------------------------

void MainGame::Deinit()
{
   // Deinitialize and delete the ball.
   if(ball)
   {
      ball->Deinit();
      frog_delete ball;
      ball = NULL;

	  ghostball->Deinit();
	  frog_delete ghostball;
	  ghostball = NULL;

	  ///player
	  player1->Deinit();
	  frog_delete player1;
	  player1 = NULL;
	  player2->Deinit();
	  frog_delete player2;
	  player2 = NULL;
   }
   
   Inherited::Deinit();
}

//-----------------------------------------------------------------------------

const char* MainGame::GUILayerNameGet()
{
   return GUI_LAYER_NAME;

}

//-----------------------------------------------------------------------------

void MainGame::Update()
{
   Inherited::Update();

   unsigned int dt = theClock->LoopDurationGet();

   ball->Update(dt);
   ghostball->Update(dt);
   player1->Update(dt);
   player2->Update(dt);
   // Return to the previous menu if the escape key is pressed.
   if(!theStates->StateChangeCheck() && theKeyboard->KeyJustPressed(KEY_ESCAPE))
   {
      theMainGame->StateChangeTransitionBegin(true);
      theStates->Pop();
   }

}

//-----------------------------------------------------------------------------

void MainGame::Draw()
{
   ball->Draw();
   ghostball->Draw();
   player1->Draw();
   player2->Draw();
}

//==============================================================================

Ball::Ball()
{
   // Initialize pointers to NULL for safety.
   image = NULL;
   gball = NULL;
   image2 = NULL;
   image3 = NULL;
   spacebar = NULL; 

}

//------------------------------------------------------------------------------

void Ball::Init()
{
   // Load the image of the ball.
   image = theImages->Load("Ball");
   gball = theImages->Load("gBall"); //Ghostball | will not display
   Score1 = 0; 
   Score2 = 0;
   player1Hit = 0;
   // Start the ball in the middle of the screen.
   position = Point2F::Create(theScreen->SizeGet() / 2);
   ballVelocity.Set(0, 0);
   gbvelocity.Set(0, 0); 

   //Player
   // Load the image of the paddles and prompt.
   image2 = theImages->Load("paddle0");
   image3 = theImages->Load("paddle0");
   spacebar = theImages->Load("PressSpacebartoPlay"); 
   // Start the player in the middle of the left side of the screen.
   position1 = Point2F::Create((float)image2->WidthGet() / 2, ((float)theScreen->HeightGet() / 2));

   // Start the player in the middle of the right side of the screen.
   position2 = Point2F::Create((float)theScreen->WidthGet() - (float)image2->WidthGet() / 2, ((float)theScreen->HeightGet() / 2));

   //Puts promt int middle of screen above ball 
   positionSp = Point2F::Create(position.x, position.y + (float)image->HeightGet());
   // The player has a 0,0 velocity
   velocity.Set(0, 0);
}

//------------------------------------------------------------------------------

void Ball::Deinit()
{
   // Unload the image of the ball.
   if(image)
   {
      theImages->Unload(image);
      image = NULL;
   }
   if (gball)
   {
	   theImages->Unload(gball);
	   gball = NULL;
   }
   if (image2)
   {
	   theImages->Unload(image2);
	   image2 = NULL;
   }
   if (image3)
   {
	   theImages->Unload(image3);
	   image3 = NULL;
   }
	if (spacebar)
	{
		theImages->Unload(spacebar);
		spacebar = NULL;
	}
}

//-----------------------------------------------------------------------------

void Ball::Update(unsigned int dt)
{
	//Press space to start - starts the ball movement and removes prompt from screen
	if (theKeyboard->KeyPressed(KEY_SPACE)){
		if ((Score1 < 8) && (Score2 < 8)){
			//ball position
			position = Point2F::Create(theScreen->SizeGet() / 2);
			//ball velocity
			ballVelocity.Set(-BALL_AXIS_SPEED, 0);
			//ghostball position
			gposition = Point2F::Create(position.x, position.y);
			//ghostball velocity
			gbvelocity.Set((float)ballVelocity.x * 20.0f, (float)ballVelocity.y*20.0f);

			spacebar = theImages->Load("blank"); //Not sure how to remove a png that is already onscreen? 
		}
	}
   // Get a floating point number for the duration of the last frame in seconds.
   float dtSeconds = (float)dt / 1000.0f;

   // Update the position of the ball.
   position += ballVelocity * dtSeconds;
   // Update the position of the ghost ball.
   gposition += gbvelocity * dtSeconds;

   //Ball
   Point2F ballSize = Point2F::Create(image->SizeGet());
   Point2F halfBallSize = ballSize / 2.0f;
   Box2F ballArea = {halfBallSize.x, halfBallSize.y,
      (float)theScreen->WidthGet() - ballSize.x,
      (float)theScreen->HeightGet() - ballSize.y};

   //Ghost Ball
   Point2F ballSize2 = Point2F::Create(gball->SizeGet());
   Point2F halfBallSize2 = ballSize2 / 2.0f;
   Box2F ballArea2 = { halfBallSize2.x, halfBallSize2.y,
	   (float)theScreen->WidthGet() - ballSize2.x,
	   (float)theScreen->HeightGet() - ballSize2.y };

   //Player1
   Point2F paddlesize = Point2F::Create(image2->SizeGet());
   Point2F halfPaddleSize = paddlesize / 2.0f;
   Box2F paddleArea = { halfPaddleSize.x, halfPaddleSize.y,
	   (float)theScreen->WidthGet() - paddlesize.x,
	   (float)theScreen->HeightGet() - paddlesize.y };


   //Player2
   Point2F paddlesize2 = Point2F::Create(image3->SizeGet());
   Point2F halfPaddleSize2 = paddlesize2 / 2.0f;
   Box2F paddleArea2 = { halfPaddleSize2.x, halfPaddleSize2.y,
	   (float)theScreen->WidthGet() - paddlesize2.x,
	   (float)theScreen->HeightGet() - paddlesize2.y };

   // See if it hits the right paddle (PLAYER 2 PADDLE) | (PLAYER1 SCORE)
   if ((position.y > position2.y - halfPaddleSize2.y) && (position.y < position2.y + halfPaddleSize2.y)){
		if ((position.x > position2.x - paddlesize2.x) && (ballVelocity.x > 0.0f)) {
		   ballVelocity.x *= -1.0f;
	
		   //ghostball position
		   gposition = Point2F::Create(position.x, position.y);
		   //ghostball velocity
		   gbvelocity.Set(ballVelocity.x * 20, ballVelocity.y* 2);
		   // Update the position of the ghost ball.
		   gposition += gbvelocity * dtSeconds;
	   }
   }
   //if it misses the paddle2 the ball stops off screen
   if (position.x > position2.x + paddlesize2.x){
	   player1Hit = 0;
	   if (Score1 < 6) {
		   position = Point2F::Create(theScreen->SizeGet() / 2); 
		   ballVelocity.Set(0, 0);
		   spacebar = theImages->Load("PressSpaceBartoPlay");
		   Score1 += 1;
	   } else if (Score1 >= 6) {
		  ballVelocity.Set(0, 0);
		   Score1 = 7; 
		   if (theKeyboard->KeyPressed(KEY_X)){
			   Score1 = -1;
			   Score2 = 0; 
		   }
	   }
   }
   // See if it hits the left paddle (PLAYER 1 PADDLE) | (PLAYER2 SCORE){
   if ((position.y > position1.y - halfPaddleSize.y) && (position.y < position1.y + halfPaddleSize.y)){
		if ((position.x < position1.x + paddlesize.x) && (ballVelocity.x < 0.0f)){
			ballVelocity.Set(-BALL_AXIS_SPEED, BALL_AXIS_SPEED2 * -1);
			ballVelocity.x *= -1.0f;
			if (player1Hit % 3 ){
				ballVelocity.Set(-BALL_AXIS_SPEED, BALL_AXIS_SPEED2);
				ballVelocity.x *= -1.0f;
			}if (player1Hit == 2){
				gbvelocity.Set(ballVelocity.x * 20.0f, ballVelocity.y * 20.0f);
				gposition.y += 30.0f;
				DebugPrintf("GBvelocity: %d \n", (float)ballVelocity.x);
			}

		   //ghostball velocity
			gbvelocity.Set(ballVelocity.x * 20.0f, ballVelocity.y * 20.0f);
		   DebugPrintf("GBvelocity: %d \n", (float)ballVelocity.x);
		   //ghostball position
		   gposition = Point2F::Create(position.x, position.y);
		   // Update the position of the ghost ball.
		   gposition += gbvelocity * dtSeconds;
		   player1Hit += 1; 
	   }
   }
   //if it misses the paddle the ball stops off screen
   if (position.x < 0 - ballArea.x){
	   player1Hit = 0; 
	   //Score is updated as ball misses, the 'press space to play' message is also redisplayed after goals
	   if (Score2 < 6) {
		   position = Point2F::Create(theScreen->SizeGet() / 2); 
		   ballVelocity.Set(0, 0);
		   spacebar = theImages->Load("PressSpaceBartoPlay");
		   Score2 += 1;
	   }
	   //When final point is scored the score is set and ball is stopped, and 'KEY X' will reset the score
	   else if (Score2 >= 6) {
		   Score2 = 7; 
		   ballVelocity.Set(0, 0);
		   if (theKeyboard->KeyPressed(KEY_X)){
			   Score2 = -1; 
			   Score1 = 0;
			  
		   }
	   }
   }
   //See where ghost ball hit on right goal 
   if ((gposition.x > position2.x - paddlesize2.x) && (gbvelocity.x > 0.0f)) {
	    gbvelocity.Set(0, 0);
   }
   //See where ghost ball hit on left goal 
   if ((gposition.x < position1.x + paddlesize.x) && (gbvelocity.x < 0.0f)){
	   gbvelocity.Set(0, 0);
   }
	//See if it's too far down.
   if ((position.y > ballArea.MaxYGet()) && (ballVelocity.y > 0.0f)){
	   ballVelocity.y *= -1.0f;
   }
   //See if ghost ball is too far down
   if ((gposition.y > ballArea2.MaxYGet()) && (gbvelocity.y > 0.0f)){
	   gbvelocity.y *= -1.0f;
   }

   // See if it's too far up.
   if ((position.y < ballArea.y) && (ballVelocity.y < 0.0f)){
	   ballVelocity.y *= -1.0f;
   }
   //See if ghost ball is to far up
   if ((gposition.y < ballArea2.y) && (gbvelocity.y < 0.0f)){
	   gbvelocity.y *= -1.0f;
   }
   // Update the position of the paddles. Not sure if velocity is necessary?
   position1 += velocity * dtSeconds;
   position2 += velocity * dtSeconds;
   //---------------------------------------------------------------------------
   //Decided to have score displayed on paddles, thought it was a unique touch that I hadn't seen on many online versions of the game
   //A switch statement guides the progarm through selecting the correct png file for displaying the current score on the paddle.
   /*=============================================================
					SCORE DISPLAY
   ==============================================================*/
   switch (Score1){
   case 0:
	   image2 = theImages->Load("paddle0");
	   break;
   case 1: 
	   image2 = theImages->Load("paddle1");
	   break; 
   case 2:
	   image2 = theImages->Load("paddle2");
	   break;
   case 3:
	   image2 = theImages->Load("paddle3");
	   break;
   case 4:
	   image2 = theImages->Load("paddle4");
	   break;
   case 5:
	   image2 = theImages->Load("paddle5");
	   break;
   case 6:
	   image2 = theImages->Load("paddle6");
	   break;
   case 7:
	   image2 = theImages->Load("paddle7r");
	   spacebar = theImages->Load("Player1Win");
	   //Score1 += 1;
	   break;
   }
   switch (Score2){
   case 0:
	   image3 = theImages->Load("paddle0");
	   break;
   case 1:
	   image3 = theImages->Load("paddle1");
	   break;
   case 2:
	   image3 = theImages->Load("paddle2");
	   break;
   case 3:
	   image3 = theImages->Load("paddle3");
	   break;
   case 4:
	   image3 = theImages->Load("paddle4");
	   break;
   case 5:
	   image3 = theImages->Load("paddle5");
	   break;
   case 6:
	   image3 = theImages->Load("paddle6");
	   break;
   case 7:
	   image3 = theImages->Load("paddle7r");
	   spacebar = theImages->Load("Player2Win");
	   Score2 += 1; 
	   break;
   }
   //-------------------------------------------------------------------------------------------
   //Player controls, different versions of this pong game have had PVP(player vs player), PVC (Player vs Computer), and also CVC
   //Player vs CPU option is active. The hope was to have multiple playing options but was tripped up in AI logic
   /*=============================================================
			PLAYER 1 CONTROLS
   ==============================================================*/
   // Make sure paddle does not go offscreen
   if (position1.y < paddleArea.y)
	   position1.y = paddleArea.y - 1.0f;
   //CPU Player Down Movement | Commented out
   //if (position1.y < gposition.y){
	 //  position1.y += 10.0f;
   //}
   else
	   //Human Player UP Movent || Left or right handed options
	   if (theKeyboard->KeyPressed(KEY_W))
		   position1.y += -10.0f;
		   printf("UP");
	   if (theKeyboard->KeyPressed(KEY_UP))
			   position1.y += -10.0f;
			   printf("UP");
	// Make sure paddle does not go offscreen
   if ((position1.y > paddleArea.MaxYGet()))
	   position1.y = (float)theScreen->HeightGet() - paddleArea.y + 1.0f;
   //CPU Player UP Movement | Commented out
  // if (position1.y > gposition.y){
	//   position1.y += -10.0f;
   //}
	  else
		  //Human Player DOWN Movent || Left or right handed options
		   if (theKeyboard->KeyPressed(KEY_S))
			   position1.y += 10.0f;
			   printf("down");
		   if (theKeyboard->KeyPressed(KEY_DOWN))
				position1.y += 10.0f;
				printf("down");
	  
   /*=============================================================
				PLAYER 2 CONTROLS
	==============================================================*/
	// Make sure paddle does not go offscreen
   if (position2.y < paddleArea2.y)
	   position2.y = paddleArea2.y - 1.0f;
		//CPU Player Down Movement
	   if (position2.y < gposition.y){
		   position2.y += 10.0f;
	   }
   
   else
	   //Human Player UP | commented out
	   /*if (theKeyboard->KeyPressed(KEY_UP)){
		   position2.y += -10.0f;
		   printf("UP");
	   }
		*/ 
   // Make sure paddle does not go offscreen
   if ((position2.y > paddleArea2.MaxYGet()))
	   position2.y = (float)theScreen->HeightGet() - paddleArea2.y + 1.0f;
		//CPU Player UP Movement
	   if (position2.y > gposition.y){
		   position2.y += -10.0f;
	   }//Human Player DOWN | commented out 
	   /*
   else
	   if (theKeyboard->KeyPressed(KEY_DOWN)){
		   position2.y += 10.0f;
		   printf("down");
	   } */
}

//------------------------------------------------------------------------------

void Ball::Draw()
{
	// Prompts | for start/restart game - (copied the ball's draw)
	spacebar->Draw(positionSp - (Point2F::Create(spacebar->SizeGet()) / 2.0f));
   // The center of the ball is in the center of the image, so use an offset.
   image->Draw(position - (Point2F::Create(image->SizeGet()) / 2.0f));
   //Ghost ball used for "AI" ball tracking for CPU
   //gball->Draw(gposition - (Point2F::Create(gball->SizeGet()) / 2.0f));
   // Paddles - (copied the ball's draw)
   image2->Draw(position1 - (Point2F::Create(image2->SizeGet()) / 2.0f));
   image3->Draw(position2 - (Point2F::Create(image2->SizeGet()) / 2.0f));

 

}

//------------------------------------------------------------------------------
