#ifndef __MAINGAME_H__
#define __MAINGAME_H__

#include "Frog.h"
#include "MenuState.h"
//#include <allegro.h>
//int main(int argc, char** argv)
	namespace Webfoot {

		class Ball;
		//class Player;
		//==============================================================================

		class MainGame : public MenuState
		{
		public:
			typedef MenuState Inherited;

			MainGame();
			virtual ~MainGame() {};

			virtual void Init();
			virtual void Deinit();

			/// Call this on every frame to update the positions.
			virtual void Update();
			/// Call this on every frame to draw the images.
			virtual void Draw();

			static MainGame instance;

		protected:
			/// Returns the name of the GUI layer
			virtual const char* GUILayerNameGet();

			/// The ball that bounces around the screen.
			Ball* ball;
			Ball* ghostball; 
			Ball* player1;
			Ball* player2; 
		};

		MainGame* const theMainGame = &MainGame::instance;

		//==============================================================================

		/// A bouncing ball
		class Ball
		{
		public:
			Ball();

			/// Initialize the ball
			void Init();
			/// Clean up the ball
			void Deinit();

			/// Make any changes for the given frame.  'dt' is the amount of time that
			/// has passed since the last frame, in milliseconds.
			void Update(unsigned int dt);
			/// Draw the ball.
			void Draw();

		protected:
			/// Appearance of the ball.
			Image* image;
			//Ghostball temp
			Image* gball; 
			/// Current position of the ball.
			Point2F position;
			Point2F gposition; 
			/// Current velocity of the ball.
			Point2F ballVelocity;
			Point2F gbvelocity; 
			Point2F velocity;
			Image* image2;
			Image* image3;
			Image* spacebar; 
			/// Current position of the paddle.
			Point2F position1;
			Point2F position2;
			Point2F positionSp;
			int Score1; 
			int Score2; 
			int player1Hit;
		};

		//==============================================================================
/*		class Player
		{
		public:
			Player();
			
			/// Initialize the player
			void Init();
			/// Clean up the player
			void Deinit();
			void Set();
			Point2F Get();
			/// Make any changes for the given frame.  'dt' is the amount of time that
			/// has passed since the last frame, in milliseconds.
			void Update(unsigned int dt);
			/// Draw the paddle.
			//void Draw();
		protected:
			/// Appearance of the paddle.
			Image* image2;
			/// Current position of the paddle.
			Point2F position1;
			Point2F position2;
			/// Current velocity of the paddle.
			Point2F velocity;
			
		}; */

	} //namespace Webfoot {

#endif //#ifndef __MAINGAME_H__
