#include <Renderer.h>
#include <Light.h>
#include <Material.h>
#include <iostream>
#include <GameWindow.h>
#include <Input.h>

unsigned int shadowtex;

float kernel[3*128] = {
		0.0771426,-0.00840646,0.0630741,
		0.0920939,-0.0373425,0.0116289,
		0.0602704,0.0174819,0.07814,
		0.0482725,0.0155929,0.0867511,
		-0.0101192,0.04491,0.0897622,
		-0.0965447,-0.00684349,0.0301469,
		-0.0648012,0.070261,0.0355473,
		-0.0531571,0.0594222,0.0647217,
		-0.0592193,0.0706417,0.0470989,
		-0.0240975,0.00285238,0.101592,
		0.0902936,-0.00696749,0.0541049,
		-0.0190164,-0.104582,0.00863248,
		0.0657268,-0.0824166,0.023067,
		0.0218658,-0.0802927,0.0708368,
		-0.0602902,-0.0596981,0.0712072,
		0.0816418,0.0286787,0.0716717,
		-0.0414805,-0.105888,0.00879523,
		-0.103677,-0.0471733,0.0212802,
		0.0823549,-0.0842254,0.000292082,
		-0.0529632,0.0360953,0.101249,
		0.0901977,0.0214969,0.0792438,
		-0.000965168,0.124109,0.00528347,
		-0.0617005,-0.104033,0.0373411,
		-0.100636,-0.0804604,0.00738998,
		0.0357491,-0.125448,0.0177226,
		0.073313,0.000396639,0.112562,
		0.135145,0.0149111,0.0178678,
		0.0459506,0.10996,0.0735525,
		-0.0251707,-0.0853705,0.11201,
		0.129134,-0.0487694,0.0481617,
		-0.0980472,0.104065,0.0434642,
		-0.129433,-0.0792865,0.01747,
		0.103232,0.0685122,0.0952008,
		-0.133612,-3.20702e-05,0.0876959,
		-0.115431,0.0218622,0.113711,
		-0.090411,0.0519206,0.13083,
		0.0270363,0.0571361,0.159094,
		-0.0665395,0.0218663,0.160592,
		0.12523,0.0183779,0.127026,
		-0.0595707,0.16436,0.0559296,
		-0.149956,-0.0514999,0.100815,
		0.0760082,0.174908,0.0249937,
		0.115444,0.113302,0.11227,
		-0.0888671,-0.166142,0.0716198,
		0.201263,-0.00795851,0.0448263,
		0.115277,-0.127501,0.122783,
		-0.165724,0.00606255,0.138768,
		-0.207572,0.011587,0.0759786,
		-0.158651,0.0342999,0.158063,
		0.0461878,0.0850454,0.21073,
		0.014087,0.192938,0.137483,
		-0.206368,-0.0274707,0.125088,
		0.00158543,-0.152985,0.195864,
		0.182495,-0.172544,0.0399273,
		0.145924,0.0959791,0.192842,
		-0.255481,0.0494338,0.0559571,
		0.0605877,-0.26239,0.0401155,
		-0.140371,-0.222312,0.0917637,
		-0.232468,-0.115253,0.117391,
		0.240384,-0.160486,0.0355986,
		0.126032,0.225261,0.148429,
		0.177107,-0.246411,0.0239691,
		-0.221031,0.0563532,0.211633,
		0.243448,0.0919018,0.182828,
		0.239359,-0.0827045,0.203697,
		0.280746,0.138072,0.111352,
		-0.229389,-0.236253,0.081718,
		0.14282,-0.190597,0.251791,
		-0.00772894,0.312823,0.165532,
		-0.145444,0.190448,0.270701,
		0.32699,-0.171343,0.00153057,
		0.219555,-0.152058,0.265961,
		0.0513777,0.0995525,0.368095,
		0.230635,0.20928,0.239262,
		-0.215373,0.0589843,0.332837,
		0.33213,0.1172,0.207911,
		-0.325974,-0.227489,0.126953,
		-0.135743,-0.00989731,0.403345,
		0.397146,-0.175121,0.0118739,
		0.392374,-0.189236,0.0795577,
		-0.00871997,-0.30767,0.330412,
		0.0586671,-0.373816,0.262286,
		0.160444,-0.421388,0.130342,
		-0.402429,0.0593031,0.25184,
		0.353103,0.074464,0.327909,
		0.189878,0.0592582,0.45533,
		-0.124786,-0.307658,0.382216,
		-0.37034,-0.358987,0.00176109,
		0.463592,-0.0557921,0.240842,
		0.218474,-0.348565,0.342225,
		-0.32168,0.152369,0.41264,
		0.0893545,-0.488024,0.248495,
		0.404624,-0.132833,0.371206,
		0.214577,0.399256,0.353971,
		-0.345682,-0.300355,0.364631,
		0.395271,-0.4437,0.0426457,
		-0.094662,0.417365,0.429401,
		-0.21872,0.331301,0.472131,
		0.437449,0.358731,0.271636,
		0.548703,-0.202829,0.255579,
		0.184311,-0.609826,0.125515,
		-0.2134,-0.326951,0.532575,
		0.603556,-0.178422,0.234115,
		-0.54489,0.1131,0.395572,
		-0.485536,0.131976,0.478193,
		0.21776,-0.393426,0.543781,
		-0.44868,-0.256148,0.497461,
		0.279818,-0.0519577,0.671055,
		0.722866,-0.110152,0.118328,
		-0.365765,0.559912,0.345233,
		-0.67465,-0.348881,0.0886241,
		-0.378997,0.190699,0.650717,
		0.500365,0.144144,0.592855,
		-0.534364,-0.375735,0.464279,
		0.629179,0.2313,0.461577,
		-0.368336,0.610715,0.417625,
		0.756574,-0.350266,0.0953892,
		-0.0515078,-0.594309,0.608259,
		0.4984,0.260556,0.657042,
		-0.691558,0.501698,0.201822,
		-0.559713,-0.676892,0.149823,
		0.696157,-0.500387,0.287496,
		-0.0174929,-0.517917,0.757265,
		0.813403,-0.230501,0.390025,
		0.685532,0.0273308,0.649324,
		0.723682,-0.299243,0.552349,
		-0.428401,0.427208,0.760877,
		0.667885,-0.720892,0.080157
};

Renderer::Renderer() {
    scene = NULL;
    camera = NULL;
    fov=55.0;   //  Field of view (angles)
    asp=1;      //  Screen aspect ratio
    defaultBasicShader = -1;
    defaultLightShader = -1;
    deferredShader = -1;
}

Renderer::~Renderer() {
    // Delete scene
    if (scene)
        delete scene;
}

int Renderer::createShaderProg(std::string vertShaderFile, std::string fragShaderFile) {
    // Create shader
    int shader = CreateShaderProg(vertShaderFile.c_str(), fragShaderFile.c_str());

    // We need to bind this shader with the projection buffer
    if (camera) {
        camera->bindProjectionBuffer(shader);
    } else {
        std::cout << "Renderer::createShaderProg - camera is NULL" << std::endl;
    }

    // Bind to light and material as well
    Light::bindLightBuffers(shader);
    Material::bindMaterialBuffers(shader);

    // Return shader
    return shader;
}

int Renderer::getDefaultBasicShader() {
    if (defaultBasicShader == -1)
        defaultBasicShader = createShaderProg("shaders/phong.vert","shaders/phong.frag");
    return defaultBasicShader;
}

int Renderer::getDefaultLightShader() {
    if (defaultLightShader == -1)
        defaultLightShader = createShaderProg("shaders/phongl.vert","shaders/phongl.frag");
    return defaultLightShader;
}

int Renderer::getDeferredShader() {
	if (deferredShader == -1)
		deferredShader = createShaderProg("shaders/def.vert", "shaders/def.frag");
	return deferredShader;
}

void Renderer::bindDeferredTextures(int shader) {
	int id = glGetUniformLocation(shader, "gPositionMap");
	if (id >= 0)
		glUniform1i(id, 6);
	id = glGetUniformLocation(shader, "gColorMap");
	if (id >= 0)
		glUniform1i(id, 7);
	id = glGetUniformLocation(shader, "gNormalMap");
	if (id >= 0)
		glUniform1i(id, 8);
	id = glGetUniformLocation(shader, "gDepth");
	if (id >= 0)
		glUniform1i(id, 10);
	id = glGetUniformLocation(shader, "gNoise");
		if (id >= 0)
			glUniform1i(id, 11);
//	id = glGetUniformLocation(shader, "kernel");
//	if (id >= 0)
//		glUniform3fv(id, 128, kernel);
	id = glGetUniformLocation(shader, "dimension");
		if (id >= 0) {
			float dime[] = {float(GameWindow::width), float(GameWindow::height)};
			glUniform2fv(id, 1, dime);
		}
}

void Renderer::setScene(Scene * s) {
    this->scene = s;
}

Scene * Renderer::getScene() {
    return this->scene;
}


void Renderer::setCamera(Camera * c) {
    this->camera = c;
}

Camera * Renderer::getCamera() {
    return this->camera;
}

void Renderer::display() {
	// Print fps
//	std::cout << "FPS: " << FramesPerSecond() << std::endl;
    // Update scripts
    if (scene)
        scene->updateScripts();
    // Reset Input
    Input::resetMaps();

    // Enable z-buffer 
    glEnable(GL_DEPTH_TEST);
    // Disable culling
    glDisable(GL_CULL_FACE);
    // Set depth test function
    glDepthFunc(GL_LEQUAL);

    // Clear color and depth buffer
    glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw scene with basic shader
    if (scene)
        scene->firstDraw();

    // Enable additive blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE);
    // Set depth test function
    glDepthFunc(GL_EQUAL);

    // Draw scene with LIGHT shader
    if (scene)
        scene->drawObjectsWithLights();

    // Disable blending
    glDisable(GL_BLEND);

    //  Render the scene and make it visible
    ErrCheck("display");
    glFlush();
    glutSwapBuffers();

}
void Renderer::reshape(int width, int height) {
    //  Ratio of the width to the height of the window
    asp = (height>0) ? (double)width/height : 1;
    //  Set the viewport to the entire window
    glViewport(0,0, width,height);
    if (camera) {
    	camera->setAspectRatio(asp);
    }
    GBuffer::cleanUp();
    GBuffer::init(width,height);
}
void Renderer::special(int key, int x, int y) {
    //  Right arrow key - increase angle by 5 degrees
    if (key == GLUT_KEY_RIGHT) {
    	Input::setKey(KeyCode::RightArrow);
    }
    //  Left arrow key - decrease angle by 5 degrees
    else if (key == GLUT_KEY_LEFT) {
    	Input::setKey(KeyCode::LeftArrow);
    }
	//  Up arrow key - increase elevation by 5 degrees
	else if (key == GLUT_KEY_UP) {
		Input::setKey(KeyCode::UpArrow);
	}
	//  Down arrow key - decrease elevation by 5 degrees
	else if (key == GLUT_KEY_DOWN) {
		Input::setKey(KeyCode::DownArrow);
	}
    //  PageUp key - increase dim
    else if (key == GLUT_KEY_PAGE_DOWN) {
    	Input::setKey(KeyCode::PageDown);
    }
    //  PageDown key - decrease dim
    else if (key == GLUT_KEY_PAGE_UP) {
    	Input::setKey(KeyCode::PageUp);
    }
    //  Tell GLUT it is necessary to redisplay the scene
    glutPostRedisplay();
}
void Renderer::key(unsigned char ch, int x, int y) {
	// Set input
	Input::setKey(ch);
    //  Tell GLUT it is necessary to redisplay the scene
    glutPostRedisplay();
}
void Renderer::idle() {
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}
