
#include "GL/glew.h"
#include <cstdlib>
#include <cstdio>
#include <GLFW/glfw3.h>

#include "SceneShader.h"

double mouse_old_x, mouse_old_y;
int mouse_buttons = 0;
float rotate_x = 0.0, rotate_y = 0.0;
float translate_z = -1.0;

int width = 1024;
int height = 1024;
GLFWwindow* window;

SceneShader Sshader;

//Keyboard Controls

void keyboard( unsigned char key, int x, int y )
{
	float factor = 0.05f;

	switch (key)
	{
	case (27):
						exit(EXIT_FAILURE);

        case (GLFW_KEY_X):
						rotate_y += 1.0f;
	break;
	case (GLFW_KEY_Z):
						rotate_y -=  1.0f;
	break;

	case (GLFW_KEY_A):
                                Sshader.updateLightPositionX(-factor);
	break;
	case (GLFW_KEY_D):
                                Sshader.updateLightPositionX(factor);
	break;
	case (GLFW_KEY_S):
                                Sshader.updateLightPositionY(-factor);
		break;
	case (GLFW_KEY_W):
                                Sshader.updateLightPositionY(factor);
	break;

	case (GLFW_KEY_Q):
                                Sshader.updateLightPositionZ(-factor);
	break;
	case (GLFW_KEY_E):
                                Sshader.updateLightPositionZ(factor);
		break;
        case (GLFW_KEY_B):
                                Sshader.updateKBlue(factor);
        break;
        case (GLFW_KEY_N):
                                Sshader.updateKBlue(-factor);
        break;

        case (GLFW_KEY_Y):
                                Sshader.updateKYellow(factor);
        break;
        case (GLFW_KEY_U):
                                Sshader.updateKYellow(-factor);
        break;

        case (GLFW_KEY_1):
                                Sshader.updateAlpha(factor);
        break;
        case (GLFW_KEY_2):
                                Sshader.updateAlpha(-factor);
        break;

        case (GLFW_KEY_3):
                                Sshader.updateBeta(factor);
        break;
        case (GLFW_KEY_4):
                                Sshader.updateBeta(-factor);
        break;

        case (GLFW_KEY_5):
                                Sshader.updateRC(factor);
        break;
        case (GLFW_KEY_6):
                                Sshader.updateRC(-factor);
        break;

        case (GLFW_KEY_7):
                                Sshader.updateGC(factor);
        break;
        case (GLFW_KEY_8):
                                Sshader.updateGC(-factor);
        break;

        case (GLFW_KEY_9):
                                Sshader.updateBC(factor);
        break;
        case (GLFW_KEY_0):
                                Sshader.updateBC(-factor);
        break;

        case (GLFW_KEY_O):
                                Sshader.updateZoom(factor);
        break;
        case (GLFW_KEY_P):
                                Sshader.updateZoom(-factor);
        break;

	default:
		break;
	}


}


void mouse( GLFWwindow* window, int button, int action, int mods )
{
	if (action == GLFW_PRESS)
	{
		double x, y;
		glfwGetCursorPos(window, &x, &y);
		mouse_old_x = x;
		mouse_old_y = y;
	}
}

void motion( GLFWwindow* w, double x, double y )
{

	double dx, dy;
	dx = (x - mouse_old_x);
	dy = (y - mouse_old_y);

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1))
	{
		rotate_x += dy * 0.5f;
		rotate_y += dx * 0.5f;
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2))
	{
		translate_z += dy * 0.03f;
	}

	mouse_old_x = x;
	mouse_old_y = y;
}

void scroll( GLFWwindow* w, double x, double y )
{
	double dy;
	dy = (x - y);
	translate_z += dy * 0.03f;
}

void render( )
{
        GLfloat color[] = {0.3215f, 0.3411f, 0.4352f, 1.0f};
        //GLfloat color[] = {0.3f, 0.3f, 0.4f, 1.0f};
	const GLfloat zero = 1.0f;

	glClearBufferfv(GL_COLOR, 0, color);
	glClearBufferfv(GL_DEPTH, 0, &zero);
	glEnable(GL_DEPTH_TEST);

        Sshader.setRotationY(rotate_y);
        Sshader.setZTranslation( translate_z );

        Sshader.render();

	glDisable(GL_DEPTH_TEST);
}

void reshape( GLFWwindow* w, int widthX, int heightY )
{
	width = widthX;
	height = heightY;
	glViewport(0, 0, width, height);
}

static void error_callback( int error, const char* description )
{
	fputs(description, stderr);
}

static void key_callback( GLFWwindow* window, int key, int scancode, int action, int mods )
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	keyboard(key, 0, 0);

}

void startGlew( )
{
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong */
		fprintf(stderr, "Error: %s \n", glewGetErrorString(err));
	}
	fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

	// get version info
	const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
	const GLubyte* vendor = glGetString(GL_VENDOR); // vendor name string
	const GLubyte* version = glGetString(GL_VERSION); // version as a string
	// GLSL version string
	const GLubyte* glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
	GLint major, minor;
	glGetIntegerv(GL_MAJOR_VERSION, &major); // get integer (only if gl version > 3)
	glGetIntegerv(GL_MINOR_VERSION, &minor); // get dot integer (only if gl version > 3)
	printf("OpenGL on %s %s\n", vendor, renderer);
	printf("OpenGL version supported %s\n", version);
	printf("GLSL version supported %s\n", glslVersion);
	printf("GL version major, minor: %i.%i\n", major, minor);

        Sshader.startup();
}

int main( int argc, char**argv )
{

        float rc, gc, bc, bl, yl, alpha, beta;
        int modelID;
        printf("Select Model:\n\t1. Bunny on a plane\n\t2. Dragon\n\t3. Teapot\n");
        scanf("%d",&modelID);
        printf("Enter Color components of the Model (Range: 0.0 to 1.0)");
        printf("\nRed Component = ");
        scanf("%f",&rc);
        printf("\nGreen Component = ");
        scanf("%f",&gc);
        printf("\nBlue Component = ");
        scanf("%f",&bc);

        printf("\nEnter Gooch Shading Modifiers (Range: 0.0 to 1.0)");
        printf("\nValue of b from k_blue = ");
        scanf("%f",&bl);
        printf("\nValue of y from k_yellow = ");
        scanf("%f",&yl);
        printf("\nValue of alpha from k_cool = ");
        scanf("%f",&alpha);
        printf("\nValue of beta from k_warm = ");
        scanf("%f",&beta);

        Sshader.updateObjectColor(rc, gc, bc);
        Sshader.setGoochVars(bl, yl, alpha, beta, modelID);
        glfwSetErrorCallback(error_callback);

	if (!glfwInit())
		exit(EXIT_FAILURE);

	glfwWindowHint(GLFW_SAMPLES, 16);

	window = glfwCreateWindow(width, height, "OpenGL Window", NULL, NULL);

	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse);
	glfwSetCursorPosCallback(window, motion);
	glfwSetScrollCallback(window, scroll);
	glfwSetWindowSizeCallback(window, reshape);

	startGlew();

	while ( !glfwWindowShouldClose(window) )
	{
		float ratio;
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float) height;
                Sshader.setAspectRatio(ratio);

		render();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	glfwDestroyWindow(window);
	glfwTerminate();

	exit(EXIT_SUCCESS);
}
