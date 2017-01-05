#define GLEW_STATIC
#define FREEGLUT_STATIC

#include <windows.h>
#include "GL/glew.h"
#include "GL/freeglut.h"
#include "gmdatacore/objreader/objreader.h"
#include "utilities/path.h"
#include "utilities/camera.h"
#include "utilities/assert.h"
#include "gmdatacore/imagereader/imagereader.h"
#include "gmengine/flow/gameloop.h"
#include "gmengine/io/keyboard.h"
#include "gmengine/elements/gameworld.h"
#include "gmengine/elements/gameobject.h"
#include "gmengine/elements/character.h"
#include "gmgl/gmglshaders.h"
#include "gmgl/gmglfunc.h"
#include "gmgl/gmgltexture.h"
#include "gmgl/shader_constants.h"
#include "gmgl/gmglgraphic_engine.h"
#include "gmgl/gmglobjectpainter.h"
#include "utilities/vmath.h"
#include "gmengine/elements/cubegameobject.h"
#include "gmengine/elements/convexhullgameobject.h"
#include "gmengine/elements/skygameobject.h"
#include "gmengine/elements/spheregameobject.h"
#include "gmdatacore/gmmap/gmmapreader.h"
#include "gmdatacore/gmmap/gameworldcreator.h"
#include "gmgl/gmglfactory.h"

using namespace gm;

float width = 600;
float height = 300;
GLfloat centerX = 0, centerY = 0, centerZ = 0;
GLfloat eyeX = 0, eyeY = 0, eyeZ = 5;

GMfloat fps = 60;
GameLoopSettings s = { fps };

GameWorld* world;
Character* character;
GMGLFactory factory;

class GameHandler : public IGameHandler
{
public:
	void setGameLoop(GameLoop* l)
	{
		m_gl = l;
	}

	void render()
	{
		Camera& camera = world->getMajorCharacter()->getCamera();
		GMGLGraphicEngine* engine = static_cast<GMGLGraphicEngine*>(world->getGraphicEngine());
		GMGLShaders& shaders = engine->getShaders();

		world->renderGameWorld();

		glutSwapBuffers();
	}

	void mouse()
	{
		GMGLGraphicEngine* engine = static_cast<GMGLGraphicEngine*>(world->getGraphicEngine());
		GMGLShaders& shaders = engine->getShaders();
		Camera& camera = world->getMajorCharacter()->getCamera();
		GMGL::lookAt(camera, shaders, GMSHADER_VIEW_MATRIX);
		int wx = glutGet(GLUT_WINDOW_X),
			wy = glutGet(GLUT_WINDOW_Y);
		camera.mouseReact(wx, wy, width, height);
	}

	void keyboard()
	{
		Character* character = world->getMajorCharacter();
		Camera* camera = &character->getCamera();
		GMfloat dis = 45;
		GMfloat v = dis / fps;
		if (Keyboard::isKeyDown(VK_ESCAPE) || Keyboard::isKeyDown('Q'))
		{
			m_gl->terminate();
		}
		if (Keyboard::isKeyDown('A'))
			character->moveRight(-v);
		if (Keyboard::isKeyDown('D'))
			character->moveRight(v);
		if (Keyboard::isKeyDown('W'))
			character->moveFront(v);
		if (Keyboard::isKeyDown('S'))
			character->moveFront(-v);
		if (Keyboard::isKeyDown(VK_SPACE))
			character->jump();
	}

	void logicalFrame(GMfloat elapsed)
	{
		world->simulateGameWorld(elapsed);
	}

	void onExit()
	{
		delete world;
	}

	GameLoop* m_gl;
};

GameHandler handler;
GameLoop* gl = GameLoop::getInstance();

void init()
{
	glEnable(GL_POLYGON_SMOOTH);

	GMMap* map;
	GMMapReader::readGMM("D:\\gmm\\demo.xml", &map);
	GameWorldCreator::createGameWorld(&factory, map, &world);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	GMGLGraphicEngine* engine = static_cast<GMGLGraphicEngine*>(world->getGraphicEngine());
	GMGLShaders& shaders = engine->getShaders();

	GMGLShadowMapping& shadow = engine->getShadowMapping();
	GMGLShaders& shadowShaders = shadow.getShaders();

	std::string currentPath("D:\\shaders\\test\\");//Path::getCurrentPath();
	{
		std::string vert = std::string(currentPath).append("gmshader.vert"),
			frag = std::string(currentPath).append("gmshader.frag");
		GMGLShaderInfo shadersInfo[] = {
			{ GL_VERTEX_SHADER, vert.c_str() },
			{ GL_FRAGMENT_SHADER, frag.c_str() },
		};
		shaders.appendShader(shadersInfo[0]);
		shaders.appendShader(shadersInfo[1]);
		shaders.load();
		shaders.useProgram();
	}

	{
		std::string vert = std::string(currentPath).append("gmshadowmapping.vert"),
			frag = std::string(currentPath).append("gmshadowmapping.frag");
		GMGLShaderInfo shadersInfo[] = {
			{ GL_VERTEX_SHADER, vert.c_str() },
			{ GL_FRAGMENT_SHADER, frag.c_str() },
		};
		shadowShaders.appendShader(shadersInfo[0]);
		shadowShaders.appendShader(shadersInfo[1]);
		shadowShaders.load();
	}

	glEnable(GL_LINE_SMOOTH);

	int wx = glutGet(GLUT_WINDOW_X),
		wy = glutGet(GLUT_WINDOW_Y);

	Camera& camera = world->getMajorCharacter()->getCamera();
	camera.mouseInitReaction(wx, wy, width, height);

	handler.setGameLoop(gl);
	camera.setSensibility(.25f);
	gl->init(s, &handler);
}

void render()
{
}

void resharp(GLint w, GLint h)
{
	glViewport(0, 0, w, h);
	width = w;
	height = h;
}

int main()
{
	WinMain(NULL, NULL, NULL, 0);
	return 0;
}

int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	char * lpCmdLine,
	int nCmdShow
)
{
	int argc = 1;
	char* l = "";
	char* argv[1];
	argv[0] = l;
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(400, 400);
	glutCreateWindow("Render");

	GLenum err = glewInit();
	init();
	glutReshapeFunc(resharp);
	glutDisplayFunc(render);

	gl->start();

	glutMainLoop();

	return 0;
}
