322128083 smdyshel

301117800 moshemandel

Exercise Description
--------------------
We implemented a model that creates balls and animates them on the screen, changing the balls course and size 
with respect to collisions with window boundaries and other balls.

Description of new files:
-------------------------
The new object we added -- Ball, implemented in the files Ball.h and Ball.cpp, holds relevant data to the ball/s seen in window: such as speed, direction, coordinates,
color, radius and scale with respect to original size. 
Each ball created by clicking the mouse leads to a new instance of this object and is set with initial speed, color, 
coordinates and direction. The Ball class implements all the neccesary fucntionality to change the properties of the ball over time: moving, shrinking or growing, and so on.

Description of implementation:
------------------------------
All the information about the balls is held in the _balls array. All the calculations considering the balls properties (especially shrinking the balls because of collision) is done in Model.cpp, in calculateCollisions() function. The collision shrinking calculation is based on the following principle: if the distance between ball centers is less than the sum of their radii, then the distance shortage is divided equally between the two balls - each one's radius shrinks by half the distance shortage.

Our implementation efficiently uses the glDrawArraysInstanced function, by drawing a single ball at the window center 
with an initial radius. For each new ball (with different coordinates and radius), it scales the initial ball to 
a (maybe) new radius and translates the ball center by the according offset. We hold the relevant data of each ball at 
its respective Ball object, which is stored in an array of Ball instances. In order to prepare the data for use by 
the draw function, we hold arrays of relevant data (translation, scaling, etc.), that already takes into account collisions with other balls and the walls,  and fill them before
drawing using fillDataArrays() function. We then connect the uniform arrays handles to the filled arrays. These uniform arrays are then used by the vertex and fragment shaders. Each time, glDrawInstancedArrays draws ten balls.

TODO: change accordingly :...
As required and specified from us by the teacher assistant, we keep the balls' radius unchanged when resizing window.

Changes in code w.r.t. exercise 1:
----------------------------------
	New Files:
		1.Ball.h	: ball object header.
		2.Ball.cpp	: ball object implementation.
	Modified Files:
		1. ex0.cpp	: changed following functions;
			1a. mouse	- creates new ball upon clicking left button.
			1b. timer	- calls model.move() to animate all objects in model at each frame.
		2. Model.h : 
			Added following data members;
				Uniform handles (of type GLint) :
					_scaleUV		- handle to scale each of the balls' radius. 
					_translationUV	- handle of array of translation of each of the balls' vertices' coordinates.
					_ballRadiusUV	- handle of array of balls' radius.
					_windowSizeUV	- handle to current window size (width, height).
				Data arrays (of type float):
					_fillColorArr
					_scaleARr
					_translateArr
					_ballRadiusArr
				Miscellaneous:
				_initWidth, _initHeight	- initial width and height of window.
			Added following functions:
				Public:
					createBall(int x, int y)
				Private:
					calculateCollisions()
					calculateDist(float x0, float y0, float x1, float y1);
					isColliding(Ball& ball1, Ball& ball2, float& distanceShortage);
					fillDataArrays(int i);
					setUniformHandles (int numInstances);
					computeLargestRadius(float newX, float newY);
		3. Model.cpp :
			3a. Implemented the added functions mentioned above.
			3b. Modified following functions:
				~Model()	- delete allocated memory (TODO: Maria- is this different w.r.t ex1a???)
				init()		- gets uniform handles, adds first ball to array.
				draw()		- draws all balls in array (opposed to drawing single ball), and adds relevant handles to shaders.
				move()		- sets scale in each ball w.r.t current window size.
		4. SimpleShader.vert : Vertex Shader
			Added uniform translation and scale arrays.
			Added InstanceID out variable (to send to fragment shader).
			Draws vertex according to relevant gl_InstanceID in uniform arrays.
		5. SimpleShader.frag : Fragment Shader
			Added fillColor, ballRadius and translation uniform arrays.
			Added "in" data member: InstanceID -- instanceID of each ball instance.
			Added data members:
				vec4 fillColorFrag - fill color of ball w.r.t. instanceID.
				float ballRadiusFrag - radius of ball w.r.t. instanceID.
				vec2 offset - translation of ball w.r.t. instanceID.
				float gradientAlpha - set gradient factor, adjusts "light intensity".
				vec4 lightColor - color of light (white).
				vec2 lightPosition - set coordinates of light source.
				Additional data members (specified in code) used to calculate center of projected light on ball.
			Calculates center of projected light on ball with respect to set position of light source. Then sets how
			the light affects pixel color w.r.t the distance of pixel from center of projected light on ball. Finally,
			combines unchaged fillColor with highlighted gradient according to set gradientAlpha variable (specified above).
		6. Makefile :
			Modified according to new dependencies and Ex1b requirements.
