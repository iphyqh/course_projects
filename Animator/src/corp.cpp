
// The sample model.  You should build a file
// very similar to this for when you make your model.

#include "modelerview.h"
#include "modelerapp.h"
#include "modelerdraw.h"
#include <FL/gl.h>
#include "particleSystem.h"

#include "stb_image.h"
#include <math.h> 
#include <stdlib.h> 
#include "camera.h"

# define PI           3.14159265358979323846 
int ptime = 0;
int seated = 0;
int L_system = 0;
// Texture loading, reference: https://learnopengl.com/. Really helped me out!

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

Mat4f getModelViewMatrix()
{
	/**************************
	**
	**	GET THE OPENGL MODELVIEW MATRIX
	**
	**	Since OpenGL stores it's matricies in
	**	column major order and our library
	**	use row major order, we will need to
	**	transpose what OpenGL gives us before returning.
	**
	**	Hint:  Use look up glGetFloatv or glGetDoublev
	**	for how to get these values from OpenGL.
	**
	*******************************/

	GLfloat m[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, m);
	Mat4f matMV(m[0], m[1], m[2], m[3],
		m[4], m[5], m[6], m[7],
		m[8], m[9], m[10], m[11],
		m[12], m[13], m[14], m[15]);

	return matMV.transpose(); // convert to row major
}

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
	BODYX, BODYY, BODYZ,
	TRANSLATEX, TRANSLATEY, TRANSLATEZ,
	LEVEL,
	SIT,
	WHOLEX, WHOLEY, WHOLEZ,
	HEADX, HEADY, HEADZ,
	LEFTARMX, LEFTARMY, LEFTARMZ,
	RIGHTARMX, RIGHTARMY, RIGHTARMZ,
	LEFTFOREARMX, LEFTFOREARMY, LEFTFOREARMZ,
	RIGHTFOREARMX, RIGHTFOREARMY, RIGHTFOREARMZ,
	LEFTTHIGHX, LEFTTHIGHY, LEFTTHIGHZ,
	RIGHTTHIGHX, RIGHTTHIGHY, RIGHTTHIGHZ,
	LEFTLEGX, LEFTLEGY, LEFTLEGZ,
	RIGHTLEGX, RIGHTLEGY, RIGHTLEGZ,
	LEFTSOLEX,
	RIGHTSOLEX,
	PARTICLE_COUNT,
	VISCOUS_COEFFICIENT,
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




void texture_alloc(char* fname)
{
	int width, height, nrChannels;

	unsigned char* data = stbi_load(fname, &width, &height, &nrChannels, 0);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB,
		GL_UNSIGNED_BYTE, data);



	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);



	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,GL_MODULATE);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
}

void drawPyramid(double x1, double y1, double z1,
	double x2, double y2, double z2,
	double x3, double y3, double z3,
	double x4, double y4, double z4) {
	drawTriangle(x1, y1, z1, x2, y2, z2, x3, y3, z3);
	drawTriangle(x1, y1, z1, x2, y2, z2, x4, y4, z4);
	drawTriangle(x1, y1, z1, x3, y3, z3, x4, y4, z4);
	drawTriangle(x2, y2, z2, x3, y3, z3, x4, y4, z4);
}


// To make a SampleModel, we inherit off of ModelerView
class SampleModel : public ModelerView
{
public:
	SampleModel(int x, int y, int w, int h, char* label)
		: ModelerView(x, y, w, h, label) { }
	
	virtual void draw();
	//void drawLsystem(int level,float bodyx,float bodyy,float bodyz);
	//void drawOneCorp(double Tx,double Ty,double Tz,)
};

// We need to make a creator function, mostly because of
// nasty API stuff that we'd rather stay away from.
ModelerView* createSampleModel(int x, int y, int w, int h, char* label)
{
	return new SampleModel(x, y, w, h, label);
}

// Texture loading, reference: https://learnopengl.com/. Really helped me out!
void drawTexture(double x, double y, double z)
{
	ModelerDrawState* mds = ModelerDrawState::Instance();


	/* remember which matrix mode OpenGL was in. */
	int savemode;
	glGetIntegerv(GL_MATRIX_MODE, &savemode);

	/* switch to the model matrix and scale by x,y,z. */
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glScaled(x, y, z);


	glEnable(GL_TEXTURE_2D);

	int width, height, nrChannels;

	unsigned char* data = stbi_load("Durandal.jpg", &width, &height, &nrChannels, 0);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB,
		GL_UNSIGNED_BYTE, data);

	stbi_image_free(data);

	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);



	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,GL_MODULATE);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

	glBegin(GL_QUADS);

	glNormal3d(0.0, 0.0, -1.0);
	glTexCoord2f(0.0, 1.0); glVertex3d(0.0, 0.0, 0.0);
	glTexCoord2f(0.0, 0.0); glVertex3d(0.0, 1.0, 0.0);
	glTexCoord2f(1.0, 0.0); glVertex3d(1.0, 1.0, 0.0);
	glTexCoord2f(1.0, 1.0); glVertex3d(1.0, 0.0, 0.0);

	glNormal3d(0.0, -1.0, 0.0);
	glVertex3d(0.0, 0.0, 0.0); glVertex3d(1.0, 0.0, 0.0);
	glVertex3d(1.0, 0.0, 1.0); glVertex3d(0.0, 0.0, 1.0);

	glNormal3d(-1.0, 0.0, 0.0);
	glVertex3d(0.0, 0.0, 0.0); glVertex3d(0.0, 0.0, 1.0);
	glVertex3d(0.0, 1.0, 1.0); glVertex3d(0.0, 1.0, 0.0);

	glNormal3d(0.0, 0.0, 1.0);
	glVertex3d(0.0, 0.0, 1.0); glVertex3d(1.0, 0.0, 1.0);
	glVertex3d(1.0, 1.0, 1.0); glVertex3d(0.0, 1.0, 1.0);

	glNormal3d(0.0, 1.0, 0.0);
	glVertex3d(0.0, 1.0, 0.0); glVertex3d(0.0, 1.0, 1.0);
	glVertex3d(1.0, 1.0, 1.0); glVertex3d(1.0, 1.0, 0.0);

	glNormal3d(1.0, 0.0, 0.0);
	glVertex3d(1.0, 0.0, 0.0); glVertex3d(1.0, 1.0, 0.0);
	glVertex3d(1.0, 1.0, 1.0); glVertex3d(1.0, 0.0, 1.0);


	glEnd();
	glDisable(GL_TEXTURE_2D);
	/* restore the model matrix stack, and switch back to the matrix
	mode we were in. */
	glPopMatrix();
	glMatrixMode(savemode);

}

// We are going to override (is that the right word?) the draw()
// method of ModelerView to draw out SampleModel
void SampleModel::draw()
{
	if (VAL(FRAMEALL) == 1)
		m_camera->setDolly(-20);

	ptime++;
	int time = ptime;
	//drawLsystem();
	// This call takes care of a lot of the nasty projection 
	// matrix stuff.  Unless you want to fudge directly with the 
	// projection matrix, don't bother with this ...
	ModelerView::draw();

	Mat4f CameraMatrix = getModelViewMatrix();

	float bodyx = VAL(BODYX);
	float bodyy = VAL(BODYY);
	float bodyz = VAL(BODYZ);
	float headr = bodyx * 3 / 8;
	float armx = bodyx / 4;
	float army = bodyy / 3;
	float armz = bodyz / 2;
	float forearmx = bodyx / 4;
	float forearmy = bodyy / 3;
	float forearmz = bodyz / 2;
	float thighx = bodyx * 0.5;
	float thighy = bodyy * 0.5;
	float thighz = bodyz * 0.75;
	float legx = thighx / 2;
	float legy = thighy;
	float legz = thighz;
	float solex = thighx;
	float soley = legy / 4;
	float solez = thighz;
	float pyramid = thighx;
	// draw the floor
	setAmbientColor(.1f, .1f, .1f);
	setSpecularColor(.0f, .5f, .0f);
	setDiffuseColor(COLOR_RED);
	glPushMatrix();
	setAmbientColor(.1f, .5f, .1f);
	glTranslated(-5, 0, -5);
	glTranslated(0, -1.5 * bodyy, 0);
	drawBox(10, 0.01f, 10);
	glPopMatrix();

	if (VAL(LSYSTEM) == 1) {
		if (L_system == 0)
			ModelerApplication::Instance()->SetControlValue(LEFTTHIGHX, 45);

		drawLsystem(VAL(LSYSTEMLEVEL), bodyx, bodyy, bodyz);
		L_system = 1;
	}
	else {
		if (L_system == 1)
		{
			ModelerApplication::Instance()->SetControlValue(LEFTTHIGHX, 0);
			L_system = 0;
		}
			
			// draw the sample model
			setAmbientColor(.1f, .1f, .1f);
			setDiffuseColor(COLOR_BLUE);
			glPushMatrix();
			//glTranslated(VAL(XPOS), VAL(YPOS), VAL(ZPOS));

			 //rotate the whole body
			if (VAL(SIT) == 0) {/*
				glTranslated(0, 1.5 * bodyy, 0);*/
				if (seated == 1) {
					ModelerApplication::Instance()->SetControlValue(LEFTTHIGHX, 0);
					ModelerApplication::Instance()->SetControlValue(RIGHTTHIGHX, 0);
					ModelerApplication::Instance()->SetControlValue(LEFTARMX, 0);
					ModelerApplication::Instance()->SetControlValue(RIGHTARMX, 0);
					seated = 0;
				}

				glTranslated(VAL(TRANSLATEX), VAL(TRANSLATEY), VAL(TRANSLATEZ));
				if (VAL(ANIMATE) == 0) {
					glRotated(VAL(WHOLEX), 1.0, 0.0, 0.0);
					glRotated(VAL(WHOLEY), 0.0, 1.0, 0.0);
					glRotated(VAL(WHOLEZ), 0.0, 0.0, 1.0);
				}
				else if (VAL(ANIMATE) == 1) {
					glRotated(VAL(WHOLEX) - VAL(MOOD) * 5 * sin(VAL(MOOD) * 0.2 * time), 1.0, 0.0, 0.0);
					glRotated(VAL(WHOLEY), 0.0, 1.0, 0.0);
					glRotated(VAL(WHOLEZ), 0.0, 0.0, 1.0);
				}

			}
			else {
				seated = 1;
				glRotated(VAL(WHOLEX), 1.0, 0.0, 0.0);
				glRotated(VAL(WHOLEY), 0.0, 1.0, 0.0);
				glRotated(VAL(WHOLEZ), 0.0, 0.0, 1.0);
				glTranslated(0, -thighy - legy - soley, 0);
				ModelerApplication::Instance()->SetControlValue(LEFTTHIGHX, 90);
				ModelerApplication::Instance()->SetControlValue(RIGHTTHIGHX, 90);
				ModelerApplication::Instance()->SetControlValue(LEFTARMX, 180);
				ModelerApplication::Instance()->SetControlValue(RIGHTARMX, 180);

			}
			//draw body
			//glPushMatrix();
			//glTranslated(-1.5, 0, -2);
			//glScaled(3, 1, 4);
			drawTexture(bodyx, bodyy, bodyz);

			drawTorus(1.5, 2);
			//glPopMatrix();

			// draw head
			glPushMatrix();

			glTranslated(bodyx / 2, bodyy + headr, bodyz / 2);
			glRotated(VAL(HEADX), 1.0, 0.0, 0.0);
			glRotated(VAL(HEADY), 0.0, 1.0, 0.0);
			glRotated(VAL(HEADZ), 0.0, 0.0, 1.0);
			drawSphere(headr);
			glPopMatrix();

			//draw right arm (Level 2)
			if (VAL(LEVEL) >= 2) {
				glPushMatrix();
				glTranslated(bodyx, bodyy, bodyz / 2);
				if (VAL(ANIMATEARM) == 0) {
					glRotated(VAL(RIGHTARMX), 1.0, 0.0, 0.0);
					glRotated(VAL(RIGHTARMY), 0.0, 1.0, 0.0);
					glRotated(VAL(RIGHTARMZ), 0.0, 0.0, 1.0);
				}
				else if (VAL(ANIMATEARM) == 1) {
					
					glRotated(VAL(RIGHTARMX) + 50 * cos(VAL(MOOD) * 0.1 * time), 1.0, 0.0, 0.0);
					glRotated(VAL(RIGHTARMY), 0.0, 1.0, 0.0);
					glRotated(VAL(RIGHTARMZ), 0.0, 0.0, 1.0);
				}
				drawBox(armx, army, armz);
				//draw right forearm (Level 3)
				if (VAL(LEVEL) >= 3) {
					glPushMatrix();
					glTranslated(0, army, 0);
					if (VAL(ANIMATEFOREARM) == 0) {
						glRotated(VAL(RIGHTFOREARMZ), 1.0, 0.0, 0.0);
						glRotated(VAL(RIGHTFOREARMY), 0.0, 1.0, 0.0);
						glRotated(VAL(RIGHTFOREARMX), 0.0, 0.0, 1.0);
					}
					else if (VAL(ANIMATEFOREARM) == 1) {
						glRotated(VAL(RIGHTFOREARMX) + 50 * cos(VAL(MOOD) * 0.1 * time), 1.0, 0.0, 0.0);
						glRotated(VAL(RIGHTFOREARMY), 0.0, 1.0, 0.0);
						glRotated(VAL(RIGHTFOREARMZ), 0.0, 0.0, 1.0);
					}
					drawBox(armx, army, armz);
					glPopMatrix();
				}
				glPopMatrix();

			}
			//draw left arm (Level 2)
			if (VAL(LEVEL) >= 2) {
				glPushMatrix();
				glTranslated(-armx, bodyy, bodyz / 2);
				glTranslated(armx, 0, 0);
				if (VAL(ANIMATEARM) == 0) {
					glRotated(VAL(LEFTARMX), 1.0, 0.0, 0.0);
					glRotated(VAL(LEFTARMY), 0.0, 1.0, 0.0);
					glRotated(VAL(LEFTARMZ), 0.0, 0.0, 1.0);
				}
				else if (VAL(ANIMATEARM) == 1) {
					glRotated(VAL(LEFTARMZ), 0.0, 0.0, 1.0);
					glRotated(VAL(LEFTARMY), 0.0, 1.0, 0.0);
					glRotated(VAL(LEFTARMX) + 50 * cos(VAL(MOOD) * 0.1 * time), 1.0, 0.0, 0.0);
					}
				glTranslated(-armx, 0, 0);
				drawBox(armx, army, armz);
				//draw left forearm (Level 3)
				if (VAL(LEVEL) >= 3) {
					glPushMatrix();
					glTranslated(0, army, 0);
					if (VAL(ANIMATEFOREARM) == 0) {
						glRotated(VAL(LEFTFOREARMX), 1.0, 0.0, 0.0);
						glRotated(VAL(LEFTFOREARMY), 0.0, 1.0, 0.0);
						glRotated(VAL(LEFTFOREARMZ), 0.0, 0.0, 1.0);
					}
					else if (VAL(ANIMATEFOREARM) == 1) {
						glRotated(VAL(LEFTFOREARMX) + 50 * cos(VAL(MOOD) * 0.1 * time), 1.0, 0.0, 0.0);
						glRotated(VAL(LEFTFOREARMY), 0.0, 1.0, 0.0);
						glRotated(VAL(LEFTFOREARMZ), 0.0, 0.0, 1.0);
					}
					drawBox(armx, army, armz);

					// Generating particles
					Vec3f WorldPosition(0, 0, 0);
					Mat4f WorldMatrix = CameraMatrix.inverse() * getModelViewMatrix();
					ModelerApplication::Instance()->GetParticleSystem()->SpawnParticles(WorldMatrix, VAL(PARTICLE_COUNT));
					ModelerApplication::Instance()->GetParticleSystem()->setViscous(VAL(VISCOUS_COEFFICIENT));
					glPopMatrix();
				}
				glPopMatrix();

			}

			//draw right thigh (Level 2)
			if (VAL(LEVEL) >= 2) {
				glPushMatrix();
				glTranslated(bodyx, 0, bodyz / 2);
				if (VAL(ANIMATETHIGH) == 0) {
					glRotated(VAL(RIGHTTHIGHX), 1.0, 0.0, 0.0);
					glRotated(VAL(RIGHTTHIGHY), 0.0, 1.0, 0.0);
					glRotated(VAL(RIGHTTHIGHZ), 0.0, 0.0, 1.0);
				}
				else if (VAL(ANIMATETHIGH) == 1) {
					glRotated(VAL(RIGHTTHIGHX)+ 20 * cos(VAL(MOOD) * 0.1 * time), 1.0, 0.0, 0.0);
					glRotated(VAL(RIGHTTHIGHY), 0.0, 1.0, 0.0);
					glRotated(VAL(RIGHTTHIGHZ), 0.0, 0.0, 1.0);
				}
				glPushMatrix();
				glTranslated(-thighx / 2, -thighy, -thighz / 2);
				drawBox(thighx, thighy, thighz);
				glPopMatrix();
				if (VAL(LEVEL) >= 3) {
					//draw right leg (Level 3)
					glPushMatrix();
					glTranslated(0, -thighy, 0);
					if (VAL(ANIMATELEG) == 0) {
						glRotated(VAL(RIGHTLEGX), 1.0, 0.0, 0.0);
						glRotated(VAL(RIGHTLEGY), 0.0, 1.0, 0.0);
						glRotated(VAL(RIGHTLEGZ), 0.0, 0.0, 1.0);
					}
					else if (VAL(ANIMATELEG) == 1) {
						glRotated(VAL(RIGHTLEGX) + max(0,20  * VAL(MOOD) * cos(VAL(MOOD) * 0.1 * time + 2 * PI / 2) ), 1.0, 0.0, 0.0);
						glRotated(VAL(RIGHTLEGY), 0.0, 1.0, 0.0);
						glRotated(VAL(RIGHTLEGZ), 0.0, 0.0, 1.0);
					}
					glPushMatrix();
					glTranslated(-legx / 2, -legy, -legz / 2);
					drawBox(legx, legy, legz);
					glPopMatrix();
					//draw right sole (Level 4)
					if (VAL(LEVEL) >= 4) {
						glPushMatrix();
						glTranslated(0, -legy, 0);
						/*glRotated(VAL(RIGHTSOLEX), 1.0, 0.0, 0.0);
						glRotated(VAL(RIGHTSOLEY), 0.0, 1.0, 0.0);*/
						glRotated(VAL(RIGHTSOLEX), 1.0, 0.0, 0.0);
						drawPyramid(0.87 * pyramid, 0, -pyramid / 2,
							-0.87 * pyramid, 0, -pyramid / 2,
							0, 0, pyramid,
							0, -pyramid, 0);
						glPopMatrix();
					}
					glPopMatrix();
				}
				glPopMatrix();


			}
			//draw left thigh (Level 2)
			if (VAL(LEVEL) >= 2) {
				glPushMatrix();
				glTranslated(0, 0, bodyz / 2);
				if (VAL(ANIMATETHIGH) == 0) {
					glRotated(VAL(LEFTTHIGHX), 1.0, 0.0, 0.0);
					glRotated(VAL(LEFTTHIGHY), 0.0, 1.0, 0.0);
					glRotated(VAL(LEFTTHIGHZ), 0.0, 0.0, 1.0);
				}
				else if (VAL(ANIMATETHIGH) == 1) {
					glRotated(VAL(LEFTTHIGHX) - 20 * cos(VAL(MOOD) * 0.1 * time), 1.0, 0.0, 0.0);
					glRotated(VAL(LEFTTHIGHY), 0.0, 1.0, 0.0);
					glRotated(VAL(LEFTTHIGHZ), 0.0, 0.0, 1.0);
				}
				glPushMatrix();
				glTranslated(-thighx / 2, -thighy, -thighz / 2);
				drawBox(thighx, thighy, thighz);
				glPopMatrix();
				//draw left leg (Level 3)
				if (VAL(LEVEL) >= 3) {
					glPushMatrix();
					glTranslated(0, -thighy, 0);
					
					if (VAL(ANIMATELEG) == 0) {
						glRotated(VAL(LEFTLEGX), 1.0, 0.0, 0.0);
						glRotated(VAL(LEFTLEGY), 0.0, 1.0, 0.0);
						glRotated(VAL(LEFTLEGZ), 0.0, 0.0, 1.0);
					}
					else if (VAL(ANIMATELEG) == 1) {
						glRotated(VAL(LEFTLEGX) + max(0,20 * VAL(MOOD) * cos(VAL(MOOD) * 0.1 * time)), 1.0, 0.0, 0.0);
						glRotated(VAL(LEFTLEGY), 0.0, 1.0, 0.0);
						glRotated(VAL(LEFTLEGZ), 0.0, 0.0, 1.0);
					}
					glPushMatrix();
					glTranslated(-legx / 2, -legy, -legz / 2);
					drawBox(legx, legy, legz);
					glPopMatrix();
					//draw left sole (Level 4)
					if (VAL(LEVEL) >= 4) {
						glPushMatrix();
						glTranslated(0, -legy, 0);
						/*glRotated(VAL(RIGHTSOLEX), 1.0, 0.0, 0.0);
						glRotated(VAL(RIGHTSOLEY), 0.0, 1.0, 0.0);*/
						glRotated(VAL(LEFTSOLEX), 1.0, 0.0, 0.0);
						drawPyramid(0.87 * pyramid, 0, -pyramid / 2,
							-0.87 * pyramid, 0, -pyramid / 2,
							0, 0, pyramid,
							0, -pyramid, 0);
						glPopMatrix();
					}
					glPopMatrix();
				}
				glPopMatrix();
			

			//for (int i = 0; i < 100; i++) {
			//	glPushMatrix();
			//	glRotated(3.6 * i, 0, 1, 0);
			//	glTranslated(2+ (2 - 1.5) / 2, 0, 0);
			//	drawSphere(2 - 1.5);
			//	glPopMatrix();
			//}
			glPopMatrix();
		}
		
	}
	endDraw();
}
//WHOLEX, WHOLEY, WHOLEZ,
//HEADX, HEADY, HEADZ,
//LEFTARMX, LEFTARMY, LEFTARMZ,
//RIGHTARMX, RIGHTARMY, RIGHTARMZ,
//LEFTFOREARMX, LEFTFOREARMY, LEFTFOREARMZ,
//RIGHTFOREARMX, RIGHTFOREARMY, RIGHTFOREARMZ,
//LEFTPALMX, LEFTPALMY, LEFTPALMZ,
//RIGHTPALMX, RIGHTPALMY, RIGHTPALMZ,
//LEFTTHIGHX, LEFTTHIGHY, LEFTTHIGHZ,
//RIGHTTHIGHX, RIGHTTHIGHY, RIGHTTHIGHZ,
//LEFTLEGX, LEFTLEGY, LEFTLEGZ,
//RIGHTLEGX, RIGHTLEGY, RIGHTLEGZ,
int main()
{
	// Initialize the controls
	// Constructor is ModelerControl(name, minimumvalue, maximumvalue, 
	// stepsize, defaultvalue)
	ModelerControl controls[NUMCONTROLS];
	/*controls[WHOLEX] = ModelerControl("WHOLEX", -180, 180, 1, 0);
	controls[WHOLEY] = ModelerControl("WHOLEY", -180, 180, 1, 0);
	controls[WHOLEZ] = ModelerControl("WHOLEZ", -180, 180, 1, 0);
	controls[HEIGHT] = ModelerControl("Height", 1, 2.5, 0.1f, 1);
	controls[ROTATE] = ModelerControl("Rotate", -135, 135, 1, 0);*/
	/*WHOLEX, WHOLEY, WHOLEZ,
		HEADX, HEADY, HEADZ,
		LEFTARMX, LEFTARMY, LEFTARMZ,
		RIGHTARMX, RIGHTARMY, RIGHTARMZ,
		LEFTFOREARMX, LEFTFOREARMY, LEFTFOREARMZ,
		RIGHTFOREARMX, RIGHTFOREARMY, RIGHTFOREARMZ,
		LEFTTHIGHX, LEFTTHIGHY, LEFTTHIGHZ,
		RIGHTTHIGHX, RIGHTTHIGHY, RIGHTTHIGHZ,
		LEFTLEGX, LEFTLEGY, LEFTLEGZ,
		RIGHTLEGX, RIGHTLEGY, RIGHTLEGZ,
		LEFTSOLEX,
		RIGHTSOLEX,*/
	controls[FRAMEALL] = ModelerControl("Frame all", 0, 1, 1, 0);
	controls[MOOD] = ModelerControl("Mood", 1, 10, 1, 1);
	controls[ANIMATE] = ModelerControl("ANIMATE", 0, 1, 1, 0);
	controls[ANIMATEARM] = ModelerControl("ANIMATEARM", 0, 1, 1, 0);
	controls[ANIMATEFOREARM] = ModelerControl("ANIMATEFOREARM", 0, 1, 1, 0);
	controls[ANIMATETHIGH] = ModelerControl("ANIMATETHIGH", 0, 1, 1, 0);
	controls[ANIMATELEG] = ModelerControl("ANIMATELEG", 0, 1, 1, 0);
	controls[LSYSTEM] = ModelerControl("draw Lsystem", 0, 1, 1, 0);
	controls[LSYSTEMLEVEL] = ModelerControl("Lsystem depth", 1, 4, 1, 4);
	controls[BODYX] = ModelerControl("body width", 0, 3, 0.1, 1);
	controls[BODYY] = ModelerControl("body height", 0, 3, 0.1, 1.5);
	controls[BODYZ] = ModelerControl("body length", 0, 3, 0.1, 0.5);
	controls[TRANSLATEX] = ModelerControl("translate x", -10, 10, 0.1, 0);
	controls[TRANSLATEY] = ModelerControl("translate y", -10, 10, 0.1, 0);
	controls[TRANSLATEZ] = ModelerControl("translate z", -10, 10, 0.1, 0);
	controls[LEVEL] = ModelerControl("hierarchy level", 1, 4, 1, 4);
	controls[SIT] = ModelerControl("sit down", 0, 1, 1, 0);
	controls[WHOLEX] = ModelerControl("whole rotate x", -180, 180, 1, 0);
	controls[WHOLEY] = ModelerControl("whole rotate y", -180, 180, 1, 0);
	controls[WHOLEZ] = ModelerControl("whole rotate z", -180, 180, 1, 0);
	controls[HEADX] = ModelerControl("head rotate x", -180, 180, 1, 0);
	controls[HEADY] = ModelerControl("head rotate y", -180, 180, 1, 0);
	controls[HEADZ] = ModelerControl("head rotate z", -180, 180, 1, 0);
	controls[LEFTARMX] = ModelerControl("leftarm rotate x", -180, 180, 1, 0);
	controls[LEFTARMY] = ModelerControl("leftarm rotate y", -180, 180, 1, 0);
	controls[LEFTARMZ] = ModelerControl("leftarm rotate z", -180, 180, 1, 0);
	controls[RIGHTARMX] = ModelerControl("rightarm rotate x", -180, 180, 1, 0);
	controls[RIGHTARMY] = ModelerControl("rightarm rotate y", -180, 180, 1, 0);
	controls[RIGHTARMZ] = ModelerControl("rightarm rotate z", -180, 180, 1, 0);
	controls[LEFTFOREARMX] = ModelerControl("leftforearm rotate x", -180, 180, 1, 0);
	controls[LEFTFOREARMY] = ModelerControl("leftforearm rotate y", -180, 180, 1, 0);
	controls[LEFTFOREARMZ] = ModelerControl("leftforearm rotate z", -180, 180, 1, 0);
	controls[RIGHTFOREARMX] = ModelerControl("rightforearm rotate x", -180, 180, 1, 0);
	controls[RIGHTFOREARMY] = ModelerControl("rightforearm rotate x", -180, 180, 1, 0);
	controls[RIGHTFOREARMZ] = ModelerControl("rightforearm rotate x", -180, 180, 1, 0);
	controls[LEFTTHIGHX] = ModelerControl("left thigh rotate x", -180, 180, 1, 0);
	controls[LEFTTHIGHY] = ModelerControl("left thigh rotate y", -180, 180, 1, 0);
	controls[LEFTTHIGHZ] = ModelerControl("left thigh rotate z", -180, 180, 1, 0);
	controls[RIGHTTHIGHX] = ModelerControl("right thigh rotate x", -180, 180, 1, 0);
	controls[RIGHTTHIGHY] = ModelerControl("right thigh rotate y", -180, 180, 1, 0);
	controls[RIGHTTHIGHZ] = ModelerControl("right thigh rotate z", -180, 180, 1, 0);
	controls[LEFTLEGX] = ModelerControl("left leg rotate x", -180, 180, 1, 0);
	controls[LEFTLEGY] = ModelerControl("left leg rotate y", -180, 180, 1, 0);
	controls[LEFTLEGZ] = ModelerControl("left leg rotate z", -180, 180, 1, 0);
	controls[RIGHTLEGX] = ModelerControl("right leg rotate x", -180, 180, 1, 0);
	controls[RIGHTLEGY] = ModelerControl("right leg rotate y", -180, 180, 1, 0);
	controls[RIGHTLEGZ] = ModelerControl("right leg rotate z", -180, 180, 1, 0);
	controls[LEFTSOLEX] = ModelerControl("left sole rotate x", -180, 180, 1, 0);
	controls[RIGHTSOLEX] = ModelerControl("right sole rotate x", -180, 180, 1, 0);
	controls[PARTICLE_COUNT] = ModelerControl("particle count (pc)", 0.0, 5.0, 0.1, 5.0);
	controls[VISCOUS_COEFFICIENT] = ModelerControl("Viscous Force Coefficient", 0.0, 10.0, 0.1, 0.5);
	/*for (int i = 0; i < NUMCONTROLS; i++) {
	controls[i]= ModelerControl("test", -180, 180, 1, 0);
	}*/

	ParticleSystem* ps = new ParticleSystem(9.8, 0.5); // A particle system with gravity and viscous forces
	ModelerApplication::Instance()->SetParticleSystem(ps);

	ModelerApplication::Instance()->Init(&createSampleModel, controls, NUMCONTROLS);
	return ModelerApplication::Instance()->Run();
}


//void SampleModel::drawLsystem(int level, float bodyx, float bodyy, float bodyz) {
//	// This call takes care of a lot of the nasty projection 
//	// matrix stuff.  Unless you want to fudge directly with the 
//	// projection matrix, don't bother with this ...
//
//
//	
//	if (level == 0) { return; }
//	else {
//		ModelerApplication::Instance()->SetControlValue(BODYX, bodyx);
//		ModelerApplication::Instance()->SetControlValue(BODYY, bodyy);
//		ModelerApplication::Instance()->SetControlValue(BODYZ, bodyz);
//		ModelerView::draw();
//		float headr = bodyx * 3 / 8;
//		float armx = bodyx / 4;
//		float army = bodyy / 3;
//		float armz = bodyz / 2;
//		float forearmx = bodyx / 4;
//		float forearmy = bodyy / 3;
//		float forearmz = bodyz / 2;
//		float thighx = bodyx * 0.5;
//		float thighy = bodyy * 0.5;
//		float thighz = bodyz * 0.75;
//		float legx = thighx / 2;
//		float legy = thighy;
//		float legz = thighz;
//		float solex = thighx;
//		float soley = legy / 4;
//		float solez = thighz;
//		float pyramid = thighx;
//
//		
//
//
//		// draw the sample model
//		setAmbientColor(.1f, .1f, .1f);
//		setDiffuseColor(COLOR_BLUE);
//		glPushMatrix();
//		//glTranslated(VAL(XPOS), VAL(YPOS), VAL(ZPOS));
//
//		 //rotate the whole body
//		
//			glTranslated(VAL(TRANSLATEX), VAL(TRANSLATEY), VAL(TRANSLATEZ));
//			glRotated(VAL(WHOLEX), 1.0, 0.0, 0.0);
//			glRotated(VAL(WHOLEY), 0.0, 1.0, 0.0);
//			glRotated(VAL(WHOLEZ), 0.0, 0.0, 1.0);
//
//		
//		//draw body
//		//glPushMatrix();
//		//glTranslated(-1.5, 0, -2);
//		//glScaled(3, 1, 4);
//		drawTexture(bodyx, bodyy, bodyz);
//		//glPopMatrix();
//
//		// draw head
//		glPushMatrix();
//
//		glTranslated(bodyx / 2, bodyy + headr, bodyz / 2);
//		glRotated(VAL(HEADX), 1.0, 0.0, 0.0);
//		glRotated(VAL(HEADY), 0.0, 1.0, 0.0);
//		glRotated(VAL(HEADZ), 0.0, 0.0, 1.0);
//		drawSphere(headr);
//		glPopMatrix();
//
//		//draw right arm (Level 2)
//		if (VAL(LEVEL) >= 2) {
//			glPushMatrix();
//			glTranslated(bodyx, bodyy, bodyz / 2);
//			glRotated(VAL(RIGHTARMX), 1.0, 0.0, 0.0);
//			glRotated(VAL(RIGHTARMY), 0.0, 1.0, 0.0);
//			glRotated(VAL(RIGHTARMZ), 0.0, 0.0, 1.0);
//			drawBox(armx, army, armz);
//			//draw right forearm (Level 3)
//			if (VAL(LEVEL) >= 3) {
//				glPushMatrix();
//				glTranslated(0, army, 0);
//				glRotated(VAL(RIGHTFOREARMZ), 1.0, 0.0, 0.0);
//				glRotated(VAL(RIGHTFOREARMY), 0.0, 1.0, 0.0);
//				glRotated(VAL(RIGHTFOREARMX), 0.0, 0.0, 1.0);
//				drawBox(armx, army, armz);
//				glPopMatrix();
//			}
//			glPopMatrix();
//
//		}
//		//draw left arm (Level 2)
//		if (VAL(LEVEL) >= 2) {
//			glPushMatrix();
//			glTranslated(-armx, bodyy, bodyz / 2);
//			glTranslated(armx, 0, 0);
//			glRotated(VAL(LEFTARMX), 1.0, 0.0, 0.0);
//			glRotated(VAL(LEFTARMY), 0.0, 1.0, 0.0);
//			glRotated(VAL(LEFTARMZ), 0.0, 0.0, 1.0);
//			glTranslated(-armx, 0, 0);
//			drawBox(armx, army, armz);
//			//draw left forearm (Level 3)
//			if (VAL(LEVEL) >= 3) {
//				glPushMatrix();
//				glTranslated(0, army, 0);
//				glRotated(VAL(LEFTFOREARMX), 1.0, 0.0, 0.0);
//				glRotated(VAL(LEFTFOREARMY), 0.0, 1.0, 0.0);
//				glRotated(VAL(LEFTFOREARMZ), 0.0, 0.0, 1.0);
//				drawBox(armx, army, armz);
//				glPopMatrix();
//			}
//			glPopMatrix();
//
//		}
//		//draw right thigh (Level 2)
//		if (VAL(LEVEL) >= 2) {
//			glPushMatrix();
//			glTranslated(bodyx, 0, bodyz / 2);
//			glRotated(VAL(RIGHTTHIGHX), 1.0, 0.0, 0.0);
//			glRotated(VAL(RIGHTTHIGHY), 0.0, 1.0, 0.0);
//			glRotated(VAL(RIGHTTHIGHZ), 0.0, 0.0, 1.0);
//			glPushMatrix();
//			glTranslated(-thighx / 2, -thighy, -thighz / 2);
//			drawBox(thighx, thighy, thighz);
//			glPopMatrix();
//			if (VAL(LEVEL) >= 3) {
//				//draw right leg (Level 3)
//				glPushMatrix();
//				glTranslated(0, -thighy, 0);
//				glRotated(VAL(RIGHTLEGX), 1.0, 0.0, 0.0);
//				glRotated(VAL(RIGHTLEGY), 0.0, 1.0, 0.0);
//				glRotated(VAL(RIGHTLEGZ), 0.0, 0.0, 1.0);
//				glPushMatrix();
//				glTranslated(-legx / 2, -legy, -legz / 2);
//				drawBox(legx, legy, legz);
//				glPopMatrix();
//				//draw right sole (Level 4)
//				if (VAL(LEVEL) >= 4) {
//					glPushMatrix();
//					glTranslated(0, -legy, 0);
//					/*glRotated(VAL(RIGHTSOLEX), 1.0, 0.0, 0.0);
//					glRotated(VAL(RIGHTSOLEY), 0.0, 1.0, 0.0);*/
//					glRotated(VAL(RIGHTSOLEX), 1.0, 0.0, 0.0);
//					drawPyramid(0.87 * pyramid, 0, -pyramid / 2,
//						-0.87 * pyramid, 0, -pyramid / 2,
//						0, 0, pyramid,
//						0, -pyramid, 0);
//					glPopMatrix();
//				}
//				glPopMatrix();
//			}
//			glPopMatrix();
//
//
//		}
//		//draw left thigh (Level 2)
//		if (VAL(LEVEL) >= 2) {
//			glPushMatrix();
//			glTranslated(0, 0, bodyz / 2);
//			glRotated(VAL(LEFTTHIGHX), 1.0, 0.0, 0.0);
//			glRotated(VAL(LEFTTHIGHY), 0.0, 1.0, 0.0);
//			glRotated(VAL(LEFTTHIGHZ), 0.0, 0.0, 1.0);
//			glPushMatrix();
//			glTranslated(-thighx / 2, -thighy, -thighz / 2);
//			drawBox(thighx, thighy, thighz);
//			glPopMatrix();
//			//draw left leg (Level 3)
//			if (VAL(LEVEL) >= 3) {
//				glPushMatrix();
//				glTranslated(0, -thighy, 0);
//				glRotated(VAL(LEFTLEGX), 1.0, 0.0, 0.0);
//				glRotated(VAL(LEFTLEGY), 0.0, 1.0, 0.0);
//				glRotated(VAL(LEFTLEGZ), 0.0, 0.0, 1.0);
//				glPushMatrix();
//				glTranslated(-legx / 2, -legy, -legz / 2);
//				drawBox(legx, legy, legz);
//				glPopMatrix();
//				//draw left sole (Level 4)
//				if (VAL(LEVEL) >= 4) {
//					glPushMatrix();
//					glTranslated(0, -legy, 0);
//					/*glRotated(VAL(RIGHTSOLEX), 1.0, 0.0, 0.0);
//					glRotated(VAL(RIGHTSOLEY), 0.0, 1.0, 0.0);*/
//					glRotated(VAL(LEFTSOLEX), 1.0, 0.0, 0.0);
//					drawPyramid(0.87 * pyramid, 0, -pyramid / 2,
//						-0.87 * pyramid, 0, -pyramid / 2,
//						0, 0, pyramid,
//						0, -pyramid, 0);
//					glPopMatrix();
//				}
//				glPopMatrix();
//			}
//			glPopMatrix();
//		}
//		//drawTorus(1.5, 2);
//	//for (int i = 0; i < 100; i++) {
//	//	glPushMatrix();
//	//	glRotated(3.6 * i, 0, 1, 0);
//	//	glTranslated(2+ (2 - 1.5) / 2, 0, 0);
//	//	drawSphere(2 - 1.5);
//	//	glPopMatrix();
//	//}
//		glPopMatrix();
//		glPushMatrix();
//		glTranslated(thighx / 2, thighy, thighz / 2 - bodyz);
//		drawLsystem(level-1,thighx,thighy,thighz);
//		glPopMatrix();
//	}
//
//}