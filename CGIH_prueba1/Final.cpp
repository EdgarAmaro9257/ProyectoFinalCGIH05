/*---------------------Proyecto Final                 -----------*/
/*-----------------    2023-1   ---------------------------*/
/*------------- Alumno: Amaro Cantoral Edgar---------------*/
/*------------- No. Cuenta  316625368       ---------------*/
#include <Windows.h>

#include <glad/glad.h>
#include <glfw3.h>	//main
#include <stdlib.h>		
#include <glm/glm.hpp>	//camera y model
#include <glm/gtc/matrix_transform.hpp>	//camera y model
#include <glm/gtc/type_ptr.hpp>
#include <time.h>


#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>	//Texture

#define SDL_MAIN_HANDLED
#include <SDL/SDL.h>

#include <shader_m.h>
#include <camera.h>  //No es el mismo de la practica anterior, nuevo camera.h esta en carpeta include
#include <modelAnim.h>
#include <model.h>
#include <Skybox.h>
#include <iostream>

//#pragma comment(lib, "winmm.lib")

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
//void my_input(GLFWwindow *window);
void my_input(GLFWwindow* window, int key, int scancode, int action, int mods);
void animate(void);

// settings
unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;
GLFWmonitor *monitors;

void getResolution(void);

// camera
Camera camera(glm::vec3(0.0f, 10.0f, 90.0f));
float MovementSpeed = 0.1f;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
const int FPS = 60;
const int LOOP_TIME = 1000 / FPS; // = 16 milisec // 1000 millisec == 1 sec
double	deltaTime = 0.0f,
		lastFrame = 0.0f;

//Lighting
glm::vec3 lightPosition(0.0f, 4.0f, -10.0f);
glm::vec3 lightDirection(-1.0f, -1.0f, 0.0f); //Izquirda/derecha, arriba/abajo, afuera/adentro del escenario o monitor PC
glm::vec3 lightAmbiental(0.0f, 0.0f, 0.0f);  //SIEMPRE VALORES POSITIVOS EN AMBIENTAL Y DIFUSION
glm::vec3 lightDifusion(2.0f, 3.0f, 0.0f);

// posiciones
//float x = 0.0f;
//float y = 0.0f;
float	movAuto_x = 0.0f,
        movAuto_y = 0.0f,
		movAuto_z = 0.0f,
		//avanza = 0.0f,
		orienta = 0.0f,
		giraLlanta = 0.0f;
bool	animacion = false,
		recorrido1 = true,
	    //avanza = true,
		recorrido2 = false,
		recorrido3 = false,
		recorrido4 = false;
int     avanza = 0;

float tiempo = 0.0f;


//Keyframes (Manipulación y dibujo)
float	posX = 0.0f,
		posY = 0.0f,
		posZ = 0.0f,
		rotRodIzq = 0.0f,
		giroMonito = 0.0f,
	    movBrazoDer = 0.0f;
float	incX = 0.0f,
		incY = 0.0f,
		incZ = 0.0f,
		rotInc = 0.0f,
		giroMonitoInc = 0.0f,
	    movBrazoDerInc = 0.0f,
		ambosBrazos = 0.0f,
		ambosBrazosInc = 0.0f,
		ambasPiernas = 0.0f,
		ambasPiernasInc = 0.0f,
		cabezonInc = 0.0f,
		cabezon = 0.0f;

#define MAX_FRAMES 9  //ESTA LIMITADO A 9 NUEVE CUADROS POR FRAME
int i_max_steps = 60;
int i_curr_steps = 0;
typedef struct _frame
{
	//Variables para GUARDAR Key Frames
	float posX;		//Variable para PosicionX
	float posY;		//Variable para PosicionY
	float posZ;		//Variable para PosicionZ
	float rotRodIzq;
	float giroMonito;
	float movBrazoDer;
	float ambosBrazos;
	float ambasPiernas;
	float cabezon;

}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 0;			//introducir datos
bool play = false;
int playIndex = 0;

void saveFrame(void)
{
	//printf("frameindex %d\n", FrameIndex);
	std::cout << "Frame Index = " << FrameIndex << std::endl;

	KeyFrame[FrameIndex].posX = posX;
	KeyFrame[FrameIndex].posY = posY;
	KeyFrame[FrameIndex].posZ = posZ;

	KeyFrame[FrameIndex].rotRodIzq = rotRodIzq;
	KeyFrame[FrameIndex].movBrazoDer = movBrazoDer;
	KeyFrame[FrameIndex].ambosBrazos = ambosBrazos;
	KeyFrame[FrameIndex].ambasPiernas = ambasPiernas;
	KeyFrame[FrameIndex].cabezon = cabezon;
	
	FrameIndex++;
}

void resetElements(void)
{
	posX = KeyFrame[0].posX;
	posY = KeyFrame[0].posY;
	posZ = KeyFrame[0].posZ;

	rotRodIzq = KeyFrame[0].rotRodIzq;
	giroMonito = KeyFrame[0].giroMonito;
	movBrazoDer = KeyFrame[0].movBrazoDer;
	ambosBrazos = KeyFrame[0].ambosBrazos;
	ambasPiernas = KeyFrame[0].ambasPiernas;
	cabezon = KeyFrame[0].cabezon;
	
}

void interpolation(void)
{
	incX = (KeyFrame[playIndex + 1].posX - KeyFrame[playIndex].posX) / i_max_steps;
	incY = (KeyFrame[playIndex + 1].posY - KeyFrame[playIndex].posY) / i_max_steps;
	incZ = (KeyFrame[playIndex + 1].posZ - KeyFrame[playIndex].posZ) / i_max_steps;

	rotInc = (KeyFrame[playIndex + 1].rotRodIzq - KeyFrame[playIndex].rotRodIzq) / i_max_steps;
	giroMonitoInc = (KeyFrame[playIndex + 1].giroMonito - KeyFrame[playIndex].giroMonito) / i_max_steps;
	ambosBrazosInc = (KeyFrame[playIndex + 1].ambosBrazos - KeyFrame[playIndex].ambosBrazos) / i_max_steps;
	ambasPiernasInc = (KeyFrame[playIndex + 1].ambasPiernas - KeyFrame[playIndex].ambasPiernas) / i_max_steps;
	cabezonInc = (KeyFrame[playIndex + 1].cabezon - KeyFrame[playIndex].cabezon) / i_max_steps;

}

void animate(void)             //Se ejecuta cada ciclo de programa
{
	//ANIMANDO una trayectoria DE UN CIRCULO, el de ELIPE es SOLO MODIFICAR X con 100
	//lightPosition.x = 300.0f * cos(tiempo);
	//lightPosition.y = 300.0f * cos(tiempo);
	//lightPosition.z = 300.0f * sin(tiempo);
	//tiempo += 0.1f;

	if (play)
	{
		if (i_curr_steps >= i_max_steps) //end of animation between frames?
		{
			playIndex++;
			if (playIndex > FrameIndex - 2)	//end of total animation?
			{
				std::cout << "Animation ended" << std::endl;
				//printf("termina anim\n");
				playIndex = 0;
				play = false;
			}
			else //Next frame interpolations
			{
				i_curr_steps = 0; //Reset counter
								  //Interpolation
				interpolation();
			}
		}
		else
		{
			//Draw animation
			posX += incX;
			posY += incY;
			posZ += incZ;

			rotRodIzq += rotInc;
			giroMonito += giroMonitoInc;
			movBrazoDer += movBrazoDerInc;
			ambosBrazos += ambosBrazosInc;
			ambasPiernas += ambasPiernasInc;
			cabezon += cabezonInc;

			i_curr_steps++;
		}
	}

	//Vehículo, que tan rapido se va a mover
	if (animacion) //movAuto es la variable que guarda su posicion
	{
		//ANIMACION DEL AUTO LAMBORGHINI
		//6 estados
		if (avanza == 0.0f)  //Con esto se mueve adelante y en reversa, la variable avanza se declara como variable float
		{
			movAuto_z -= 1.0f; //SE MUEVE EN REVERSA
			if (movAuto_z <= -180.0f)
				avanza = 1.0;
		}

		if (avanza == 1.0f)
		{
			movAuto_z += 1.0f; //Avanza
			if (movAuto_z >= 180.0f)
				avanza = 0.0f;
		}

		
	}
}

void getResolution()
{
	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	SCR_WIDTH = mode->width;
	SCR_HEIGHT = (mode->height) - 80;
}


int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	// --------------------
	monitors = glfwGetPrimaryMonitor();
	getResolution();

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CGeIHC", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwSetWindowPos(window, 0, 30);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetKeyCallback(window, my_input);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// build and compile shaders
	// -------------------------
	Shader staticShader("Shaders/shader_Lights.vs", "Shaders/shader_Lights_mod.fs");
	Shader skyboxShader("Shaders/skybox.vs", "Shaders/skybox.fs");
	Shader animShader("Shaders/anim.vs", "Shaders/anim.fs");

	vector<std::string> faces
	{
		"resources/skybox/right.jpg",
		"resources/skybox/left.jpg",
		"resources/skybox/top.jpg",
		"resources/skybox/bottom.jpg",
		"resources/skybox/front.jpg",
		"resources/skybox/back.jpg"
	};

	Skybox skybox = Skybox(faces);

	// Shader configuration
	// --------------------
	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);

	// load models, aqui se cargan los modelos de los personajes y escenarios
	// -----------
	Model piso("resources/objects/piso/piso.obj");
	Model botaDer("resources/objects/Personaje/bota.obj");
	Model piernaDer("resources/objects/Personaje/piernader.obj");
	Model piernaIzq("resources/objects/Personaje/piernader.obj");
	Model torso("resources/objects/Personaje/torso.obj");
	Model brazoDer("resources/objects/Personaje/brazoder.obj");
	Model brazoIzq("resources/objects/Personaje/brazoizq.obj");
	Model cabeza("resources/objects/Personaje/cabeza.obj");
	Model carro("resources/objects/lambo/carroceria.obj");   //AUTO 1
	Model llanta("resources/objects/lambo/Wheel.obj");       //AUTO 2
	Model casaproy("resources/objects/Casaproyecto/house3.obj");
	Model cama("resources/objects/Cama/cama.obj");
	Model cama2("resources/objects/Cama/cama.obj");
	Model cama3("resources/objects/Cama/cama.obj");
	Model cama4("resources/objects/Cama/cama.obj");
	Model cama5("resources/objects/Cama/cama.obj");
	Model sidetable1("resources/objects/SideTable/Side_Table.obj");
	Model sidetable2("resources/objects/SideTable/Side_Table.obj");
	Model sidetable3("resources/objects/SideTable/Side_Table.obj");
	Model sidetable4("resources/objects/SideTable/Side_Table.obj");
	Model lampara("resources/objects/Lampara/lamp2.obj");
	Model lampara2("resources/objects/Lampara/lamp2.obj");
	Model lampara3("resources/objects/Lampara/lamp2.obj");
	Model lampara4("resources/objects/Lampara/lamp2.obj");
	Model escrit1("resources/objects/Escritorio/desk.obj");
	Model escrit2("resources/objects/Escritorio/desk.obj");
	Model escrit3("resources/objects/Escritorio/desk.obj");
	Model laptop1("resources/objects/Laptop/Laptop2.obj");
	Model laptop2("resources/objects/Laptop/Laptop2.obj");
	Model laptop3("resources/objects/Laptop/Laptop2.obj");
	Model sillaoff("resources/objects/SillaOffice/sillaModerno.obj");
	Model sillaoff2("resources/objects/SillaOffice/sillaModerno.obj");
	Model sillaoff3("resources/objects/SillaOffice/sillaModerno.obj");
	Model cocina("resources/objects/Cocina/fkc.obj");
	Model setmcs("resources/objects/MesaconSillas/Table_witch_chairs.obj");
	Model sofa("resources/objects/Sofa/sofa3.obj"); 
	Model piscina("resources/objects/PiscinaJardin/piscina.obj");
	//Model furniBath("resources/objects/FurnitureBath/furnitureBath.obj");
	//Model furniBath2("resources/objects/FurnitureBath/furnitureBath.obj");
	
	//Inicialización de KeyFrames
	/*
	for (int i = 0; i < MAX_FRAMES; i++)
	{
		KeyFrame[i].posX = 0;
		KeyFrame[i].posY = 0;
		KeyFrame[i].posZ = 0;
		KeyFrame[i].rotRodIzq = 0;
		KeyFrame[i].giroMonito = 0;
	}
	*/

	for (int i = 0; i < MAX_FRAMES; i++)
	{
		KeyFrame[i].posX = 0;
		KeyFrame[i].posY = 0;
		KeyFrame[i].posZ = 0;
		KeyFrame[i].rotRodIzq = 0;
		KeyFrame[i].giroMonito = 0;
		KeyFrame[i].movBrazoDer = 0;
		KeyFrame[i].ambosBrazos = 0;
		KeyFrame[i].ambasPiernas = 0;
		KeyFrame[i].cabezon = 0;
	}
	/*
	for (int i = 0; i < MAX_FRAMES; i++)
	{
		KeyFrame[1].posX = 100.0f;
		KeyFrame[1].posY = 0;
		KeyFrame[1].posZ = 30.0f;
		KeyFrame[1].rotRodIzq = 40.0f;
		KeyFrame[1].giroMonito = 90.0f;
		KeyFrame[1].movBrazoDer = -40.0f;
		KeyFrame[1].ambosBrazos = 30.0f;
		KeyFrame[1].ambasPiernas = 45.0f;
		KeyFrame[1].cabezon = 77.0f;
	}

	for (int i = 0; i < MAX_FRAMES; i++)
	{
		KeyFrame[2].posX = 100.0f;
		KeyFrame[2].posY = 50.0f;
		KeyFrame[2].posZ = 30.0f;
		KeyFrame[2].rotRodIzq = 40.0f;
		KeyFrame[2].giroMonito = 90.0f;
		KeyFrame[2].movBrazoDer = -40.0f;
		KeyFrame[2].ambosBrazos = 60.0f;
		KeyFrame[2].ambasPiernas = -45.0f;
		KeyFrame[2].cabezon = -77.0f;
	}
	*/

	// draw in wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		skyboxShader.setInt("skybox", 0);
		
		// per-frame time logic
		// --------------------
		lastFrame = SDL_GetTicks();

		// input
		// -----
		//my_input(window);
		animate();

		// render
		// ------
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// don't forget to enable shader before setting uniforms
		staticShader.use();
		//Setup Advanced Lights
		staticShader.setVec3("viewPos", camera.Position);
		staticShader.setVec3("dirLight.direction", lightDirection);
		staticShader.setVec3("dirLight.ambient", glm::vec3(0.5f, 0.5f, 0.5f)); //CONFIGURANDO LUZ DEL AMBIENTE            0.2,0,0--> rojo    0,0.2,0 --> verde
		staticShader.setVec3("dirLight.diffuse", glm::vec3(0.6f, 0.6f, 0.6f)); //CONFIGURANDO LUZ DE DIFUSION
		staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));

		staticShader.setVec3("pointLight[0].position", lightPosition);  //Este codigo es para controlarposicion de luz
		staticShader.setVec3("pointLight[0].ambient", glm::vec3(0.2f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[0].diffuse", glm::vec3(0.0f, 0.0f, 1.0f)); //COLOR DE LUZ
		staticShader.setVec3("pointLight[0].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[0].constant", 0.08f);
		staticShader.setFloat("pointLight[0].linear", 0.009f); //Tamaño de la luz
		staticShader.setFloat("pointLight[0].quadratic", 0.00032f); //Tamaño de la luz

		staticShader.setVec3("pointLight[1].position", glm::vec3(-80.0, 3.0f, 0.0f));
		staticShader.setVec3("pointLight[1].ambient", lightAmbiental);
		staticShader.setVec3("pointLight[1].diffuse", lightDifusion);
		staticShader.setVec3("pointLight[1].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[1].constant", 1.0f);
		staticShader.setFloat("pointLight[1].linear", 0.009f);
		staticShader.setFloat("pointLight[1].quadratic", 0.032f);

		staticShader.setVec3("spotLight[0].position", glm::vec3(0.0f, 20.0f, 10.0f));
		staticShader.setVec3("spotLight[0].direction", glm::vec3(0.0f, -1.0f, 0.0f));
		staticShader.setVec3("spotLight[0].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("spotLight[0].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("spotLight[0].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("spotLight[0].cutOff", glm::cos(glm::radians(10.0f)));
		staticShader.setFloat("spotLight[0].outerCutOff", glm::cos(glm::radians(60.0f)));
		staticShader.setFloat("spotLight[0].constant", 1.0f);
		staticShader.setFloat("spotLight[0].linear", 0.0009f);
		staticShader.setFloat("spotLight[0].quadratic", 0.0005f);

		staticShader.setFloat("material_shininess", 32.0f);

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 tmp = glm::mat4(1.0f);
		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
		glm::mat4 view = camera.GetViewMatrix();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);

		//// Light
		glm::vec3 lightColor = glm::vec3(0.6f);
		glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.75f);
		

		// -------------------------------------------------------------------------------------------------------------------------
		// Personaje Animacion
		// -------------------------------------------------------------------------------------------------------------------------
		//Remember to activate the shader with the animation
		animShader.use();
		animShader.setMat4("projection", projection);
		animShader.setMat4("view", view);
	
		animShader.setVec3("material.specular", glm::vec3(0.5f));
		animShader.setFloat("material.shininess", 32.0f);
		animShader.setVec3("light.ambient", ambientColor);
		animShader.setVec3("light.diffuse", diffuseColor);
		animShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
		animShader.setVec3("light.direction", lightDirection);
		animShader.setVec3("viewPos", camera.Position);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-40.3f, 1.75f, 0.3f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(1.2f));	// it's a bit too big for our scene, so scale it down
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		animShader.setMat4("model", model);
		//animacionPersonaje.Draw(animShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Segundo Personaje Animacion
		// -------------------------------------------------------------------------------------------------------------------------

		model = glm::translate(glm::mat4(1.0f), glm::vec3(40.3f, 1.75f, 0.3f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.5f));	// it's a bit too big for our scene, so scale it down
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		animShader.setMat4("model", model);
		//ninja.Draw(animShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Escenario
		// -------------------------------------------------------------------------------------------------------------------------
		staticShader.use();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);


		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		staticShader.setMat4("model", model);
		piso.Draw(staticShader);

		//Casa del proyecto
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-10.0f, 0.0f, -70.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		staticShader.setMat4("model", model);
		casaproy.Draw(staticShader);

		//Camas
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-30.0f, 4.0f, -42.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		cama.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-18.0f, 4.0f, -42.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		cama2.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-6.0f, 4.0f, -42.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		cama3.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(6.0f, 4.0f, -42.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		cama4.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-25.0f, 4.0f, -87.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		cama5.Draw(staticShader);

		//Side Tables
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-24.0f, 2.5f, -41.5f));
		model = glm::scale(model, glm::vec3(0.06f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		sidetable1.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 2.5f, -41.5f));
		model = glm::scale(model, glm::vec3(0.06f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		sidetable2.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-31.0f, 2.5f, -88.0f));
		model = glm::scale(model, glm::vec3(0.06f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		sidetable3.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-18.0f, 2.5f, -88.0f));
		model = glm::scale(model, glm::vec3(0.06f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		sidetable4.Draw(staticShader);

		//Lamparas
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-24.0f, 5.6f, -41.5f));
		model = glm::scale(model, glm::vec3(0.05f));
		staticShader.setMat4("model", model);
		lampara.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 5.6f, -41.5f));
		model = glm::scale(model, glm::vec3(0.05f));
		staticShader.setMat4("model", model);
		lampara2.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-31.0f, 5.6f, -88.0f));
		model = glm::scale(model, glm::vec3(0.05f));
		staticShader.setMat4("model", model);
		lampara3.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-18.0f, 5.6f, -88.0f));
		model = glm::scale(model, glm::vec3(0.05f));
		staticShader.setMat4("model", model);
		lampara4.Draw(staticShader);

		//Escritorio
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-32.0f, 2.5f, -60.0f));
		model = glm::scale(model, glm::vec3(2.0f));
		staticShader.setMat4("model", model);
		escrit1.Draw(staticShader);
		
		model = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 2.5f, -60.0f));
		model = glm::scale(model, glm::vec3(2.0f));
		staticShader.setMat4("model", model);
		escrit2.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-32.0f, 2.5f, -70.0f));
		model = glm::scale(model, glm::vec3(2.0f));
		staticShader.setMat4("model", model);
		escrit3.Draw(staticShader);

		//Silla Moderno
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-27.0f, 2.5f, -55.0f));
		model = glm::scale(model, glm::vec3(0.05f));
		staticShader.setMat4("model", model);
		sillaoff.Draw(staticShader);
		
		model = glm::translate(glm::mat4(1.0f), glm::vec3(3.5f, 2.5f, -55.0f));
		model = glm::scale(model, glm::vec3(0.05f));
		staticShader.setMat4("model", model);
		sillaoff2.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-27.0f, 2.5f, -75.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.05f));
		staticShader.setMat4("model", model);
		sillaoff3.Draw(staticShader);
		
		//Laptop
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-27.0f, 5.3f, -60.0f));
		model = glm::scale(model, glm::vec3(0.03f));
		staticShader.setMat4("model", model);
		laptop1.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(3.5f, 5.3f, -60.0f));
		model = glm::scale(model, glm::vec3(0.03f));
		staticShader.setMat4("model", model);
		laptop2.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-27.0f, 5.3f, -70.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.03f));
		staticShader.setMat4("model", model);
		laptop3.Draw(staticShader);

		//Cocina
		model = glm::translate(glm::mat4(1.0f), glm::vec3(16.0f, 2.5f, -84.0f));
		model = glm::scale(model, glm::vec3(0.15f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		cocina.Draw(staticShader);

		//Mesa con silla
		model = glm::translate(glm::mat4(1.0f), glm::vec3(29.0f, 2.5f, -80.0f));
		model = glm::scale(model, glm::vec3(0.7f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		setmcs.Draw(staticShader);
		
		//Sofa
		model = glm::translate(glm::mat4(1.0f), glm::vec3(25.0f, 3.5f, -53.0f));
		model = glm::scale(model, glm::vec3(0.1f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		sofa.Draw(staticShader);

		//Furniture Bath o Kit lavamanos del baño
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-0.3f, 2.5f, -84.0f));
		model = glm::scale(model, glm::vec3(0.05f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		//furniBath.Draw(staticShader);
		
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-0.9f, 2.5f, -84.0f));
		model = glm::scale(model, glm::vec3(0.05f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		//furniBath2.Draw(staticShader);
		
		//Piscina con jardin
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-7.0f, 0.0f, -126.0f));
		model = glm::scale(model, glm::vec3(0.05f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		piscina.Draw(staticShader);
		

		// -------------------------------------------------------------------------------------------------------------------------
		// Carro
		// -------------------------------------------------------------------------------------------------------------------------
		model = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(15.0f + movAuto_x,	movAuto_y, movAuto_z));
		tmp = model = glm::rotate(model, glm::radians(orienta), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		staticShader.setMat4("model", model);
		carro.Draw(staticShader);

		model = glm::translate(tmp, glm::vec3(8.5f, 2.5f, 12.9f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		model = glm::rotate(model, glm::radians(giraLlanta), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		llanta.Draw(staticShader);	//Izq delantera

		model = glm::translate(tmp, glm::vec3(-8.5f, 2.5f, 12.9f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		llanta.Draw(staticShader);	//Der delantera

		model = glm::translate(tmp, glm::vec3(-8.5f, 2.5f, -14.5f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		llanta.Draw(staticShader);	//Der trasera

		model = glm::translate(tmp, glm::vec3(8.5f, 2.5f, -14.5f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		staticShader.setMat4("model", model);
		llanta.Draw(staticShader);	//Izq trase
		// -------------------------------------------------------------------------------------------------------------------------
		// Personaje
		// -------------------------------------------------------------------------------------------------------------------------
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		tmp = model = glm::rotate(model, glm::radians(giroMonito), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setMat4("model", model);
		torso.Draw(staticShader);

		//Pierna Der
		model = glm::translate(tmp, glm::vec3(-0.5f, 0.0f, -0.1f));
		model = glm::rotate(model, glm::radians(ambasPiernas), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::rotate(model, glm::radians(-rotRodIzq), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		piernaDer.Draw(staticShader);

		//Pie Der
		model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		staticShader.setMat4("model", model);
		botaDer.Draw(staticShader);

		//Pierna Izq
		model = glm::translate(tmp, glm::vec3(0.5f, 0.0f, -0.1f));
		model = glm::rotate(model, glm::radians(ambasPiernas), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		piernaIzq.Draw(staticShader);

		//Pie Iz
		model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		staticShader.setMat4("model", model);
		botaDer.Draw(staticShader);	//Izq trase

		//Brazo derecho
		model = glm::translate(tmp, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-0.75f, 2.5f, 0));
		model = glm::rotate(model, glm::radians(ambosBrazos), glm::vec3(0.0f, 0.0f, 1.0f));
		staticShader.setMat4("model", model);
		brazoDer.Draw(staticShader);

		//Brazo izquierdo
		model = glm::translate(tmp, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.75f, 2.5f, 0));
		model = glm::rotate(model, glm::radians(ambosBrazos), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		brazoIzq.Draw(staticShader);

		//Cabeza
		model = glm::translate(tmp, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(cabezon), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::translate(model, glm::vec3(0.0f, 2.5f, 0));
		staticShader.setMat4("model", model);
		cabeza.Draw(staticShader);
		// -------------------------------------------------------------------------------------------------------------------------
		// Caja Transparente --- Siguiente Práctica
		// -------------------------------------------------------------------------------------------------------------------------
		/*glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -70.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		staticShader.setMat4("model", model);
		cubo.Draw(staticShader);
		glEnable(GL_BLEND);*/
		// -------------------------------------------------------------------------------------------------------------------------
		// Termina Escenario
		// -------------------------------------------------------------------------------------------------------------------------

		//-------------------------------------------------------------------------------------
		// draw skybox as last
		// -------------------
		skyboxShader.use();
		skybox.Draw(skyboxShader, view, projection, camera);

		// Limitar el framerate a 60
		deltaTime = SDL_GetTicks() - lastFrame; // time for full 1 loop
		//std::cout <<"frame time = " << frameTime << " milli sec"<< std::endl;
		if (deltaTime < LOOP_TIME)
		{
			SDL_Delay((int)(LOOP_TIME - deltaTime));
		}

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	skybox.Terminate();

	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void my_input(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, (float)deltaTime);
	//To Configure Model
	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
		posZ++;
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
		posZ--;
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
		posX--;
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
		posX++;
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
		rotRodIzq--;
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
		rotRodIzq++;
	if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
		giroMonito--;
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
		giroMonito++;
	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
		ambosBrazos++;
	if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
		ambosBrazos--;
	if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
		ambasPiernas++;
	if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
		ambasPiernas--;
	if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS)
		cabezon++;
	if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS)
		cabezon--;
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
		posY++;
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
		posY--;
	
	/*
	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
		movBrazoDer++;
	if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
		movBrazoDer--;
	*/
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
		lightPosition.x++;
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
		lightPosition.x--;
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		lightAmbiental.x++;
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		lightDifusion.x--;


	//Car animation
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
		animacion ^= true;

	if (key == GLFW_KEY_R && action == GLFW_PRESS) //Declaramos una nueva tecla y guardamos movAutoZ en 0 para que regrese a posicion original
	{
		movAuto_z = 0.0f;
		animacion = false;  //Con esto el coche no se mueve hasta que presionamos la tecla SPACE
	}
		

	//To play KeyFrame animation 
	if (key == GLFW_KEY_P && action == GLFW_PRESS)
	{
		if (play == false && (FrameIndex > 1))
		{
			std::cout << "Play animation" << std::endl;
			resetElements();
			//First Interpolation				
			interpolation();

			play = true;
			playIndex = 0;
			i_curr_steps = 0;
		}
		else
		{
			play = false;
			std::cout << "Not enough Key Frames" << std::endl;
		}
	}

	//To Save a KeyFrame
	if (key == GLFW_KEY_L && action == GLFW_PRESS)
	{
		if (FrameIndex < MAX_FRAMES)
		{
			saveFrame();
		}
	}
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}
// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}
