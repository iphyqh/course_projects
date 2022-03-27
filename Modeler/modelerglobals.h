#ifndef _MODELER_GLOBALS_H
#define _MODELER_GLOBALS_H

#ifndef M_PI
#define M_PI 3.141592653589793238462643383279502
#endif

// This is a list of the controls for the SampleModel
// We'll use these constants to access the values 
// of the controls from the user interface.
//enum SampleModelControls
//{ 
//	XPOS, YPOS, ZPOS, HEIGHT, ROTATE, NUMCONTROLS
//};
// body, head,eys, big arm, forearm, palm, finger
//thigh, leg,sole
//
enum SampleModelControls
{	
	FRAMEALL,
	MOOD,
	ANIMATE,
	ANIMATEARM,
	ANIMATEFOREARM,
	ANIMATETHIGH,
	ANIMATELEG,
	LSYSTEM,
	LSYSTEMLEVEL,
	BODYX,BODYY,BODYZ,
	TRANSLATEX,TRANSLATEY, TRANSLATEZ,
	LEVEL,
	SIT,
	WHOLEX, WHOLEY, WHOLEZ,
	HEADX, HEADY, HEADZ,
	LEFTARMX, LEFTARMY, LEFTARMZ,
	RIGHTARMX,RIGHTARMY,RIGHTARMZ,
	LEFTFOREARMX,LEFTFOREARMY,LEFTFOREARMZ,
	RIGHTFOREARMX,RIGHTFOREARMY,RIGHTFOREARMZ,
	LEFTTHIGHX, LEFTTHIGHY, LEFTTHIGHZ,
	RIGHTTHIGHX,RIGHTTHIGHY, RIGHTTHIGHZ,
	LEFTLEGX, LEFTLEGY, LEFTLEGZ,
	RIGHTLEGX, RIGHTLEGY, RIGHTLEGZ,
	 LEFTSOLEX,
	 RIGHTSOLEX,
	//BODY, ROTATE,
	NUMCONTROLS
};
// Colors
#define COLOR_RED		1.0f, 0.0f, 0.0f
#define COLOR_GREEN		0.0f, 1.0f, 0.0f
#define COLOR_BLUE		0.0f, 0.0f, 1.0f

// We'll be getting the instance of the application a lot; 
// might as well have it as a macro.
#define VAL(x) (ModelerApplication::Instance()->GetControlValue(x))

#endif