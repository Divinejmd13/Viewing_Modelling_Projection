#include "utils.h"

#define  GLM_FORCE_RADIANS
#define  GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

//Globals
int screen_width = 640, screen_height=640;
GLint vModel_uniform, vView_uniform, vProjection_uniform;
GLint vColor_uniform;
glm::mat4 modelT, viewT, projectionT;//The model, view and projection transformations
glm::vec4 camPosition;
glm::vec3 cameraFront;
glm::vec3 cameraUp ;
float currentcam;
bool  ctrlKeyPressed ;
float cameraSpeed;
int circle=-1;
char textKeyStatus[256];

void createCubeObject(unsigned int &, unsigned int &);
void createAxesLine(unsigned int &, unsigned int &);

void setupModelTransformationCube(unsigned int &);
void setupModelTransformationAxis(unsigned int &program, float rot_angle, glm::vec3 rot_axis);
void setupViewTransformation(unsigned int &);
void setupProjectionTransformation(unsigned int &);
void setupOrthographicProjection(unsigned int &);
int main(int, char**)
{
	// Setup window
	GLFWwindow *window = setupWindow(screen_width, screen_height);
	ImGuiIO& io = ImGui::GetIO(); // Create IO 
	ImVec4 clearColor = ImVec4(1.0f, 1.0f, 1.0f, 1.00f);
	camPosition = glm::vec4(20.0, 30.0, 80.0, 1.0);
	cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	
	cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);
	
	unsigned int shaderProgram = createProgram("./shaders/vshader.vs", "./shaders/fshader.fs");
	//Get handle to color variable in shader
	vColor_uniform = glGetUniformLocation(shaderProgram, "vColor");
	if(vColor_uniform == -1){
		fprintf(stderr, "Could not bind location: vColor\n");
		exit(0);
	}

	glUseProgram(shaderProgram);

	//setupModelTransformation(shaderProgram);
	// Modelling transformation is setup in the display loop
	setupProjectionTransformation(shaderProgram);

	
	unsigned int cube_VAO, axis_VAO;
	createCubeObject(shaderProgram, cube_VAO);
	createAxesLine(shaderProgram, axis_VAO);
	cameraSpeed = 1.0f;
	currentcam=0;
	while (!glfwWindowShouldClose(window))
	{	setupViewTransformation(shaderProgram);
		
		glfwPollEvents();

				// Get key presses


    if (ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_LeftArrow))) {
		if (ctrlKeyPressed) {
			// Snap to left view (along -X axis)
			camPosition = glm::vec4(-100.0f, 0.0f, 0.0, 1.0f);
			cameraFront = glm::vec3(1.0f, 0.0f, 0.0f); // Look along +X axis
			
		}
		else {
			strcpy(textKeyStatus, "Key status: Left");
			camPosition -= glm::vec4(cameraSpeed, 0.0f, 0.0f, 0.0f); // Move along -X
		}
	}
	else if (ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_RightArrow))) {
		if (ctrlKeyPressed) {
			// Snap to right view (along +X axis)
			camPosition = glm::vec4(100, 0, 0, 1.0f);
			cameraFront = glm::vec3(-1.0f, 0.0f, 0.0f); // Look along -X axis
			
			
		}
		else {
			strcpy(textKeyStatus, "Key status: Right");
			camPosition += glm::vec4(cameraSpeed, 0.0f, 0.0f, 0.0f); // Move along +X
		}
	}
	else if (ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_UpArrow))) {
		if (ctrlKeyPressed) {
			// Snap to top view (along +Y axis)
			camPosition = glm::vec4(0.0f, 100.0f, 0.0f, 1.0f);
			// camPosition = glm::vec4(0.0f, 107.0f, 0.0f, 1.0); 
			cameraFront = glm::vec3(0.1, -0.9f, 0.0f); // Look along -Y axis
			// anday wala burger
			
			
		}
		else {
			if (io.KeyShift) {
				strcpy(textKeyStatus, "Key status: Shift + Up");
				camPosition += glm::vec4(0.0f, 0.0f, -cameraSpeed, 0.0f); // Zoom in along -Z
			}
			else {
				strcpy(textKeyStatus, "Key status: Up");
				camPosition += glm::vec4(0.0f, cameraSpeed, 0.0f, 0.0f); // Move along +Y
			}
		}
	}
	else if (ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_DownArrow))) {
		if (ctrlKeyPressed) {
			// Snap to bottom view (along -Y axis)
			camPosition = glm::vec4(0.0f, 0.0f, -100.0f, 1.0f);
			// camPosition = glm::vec4(0.0f, 107.0f, 0.0f, 1.0); 
			cameraFront = glm::vec3(0.1f, 0.9f, 0.0f); // Look along -Y axis
			
			
		}
		else {
			if (io.KeyShift) {
				strcpy(textKeyStatus, "Key status: Shift + Down");
				camPosition += glm::vec4(0.0f, 0.0f, cameraSpeed, 0.0f); // Zoom out along +Z
			}
			else {
				strcpy(textKeyStatus, "Key status: Down");
				camPosition -= glm::vec4(0.0f, cameraSpeed, 0.0f, 0.0f); // Move along -Y
			}
		}
	}else if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Enter))) {
		strcpy(textKeyStatus, "Key status: Enter");
		currentcam++;
		if(currentcam==1){//one point perspective
			camPosition = glm::vec4(45.0f, 30.0f, 115.0f, 1.0f);  
			cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);  
			
			
		}else if(currentcam==2){//two point perspetive
			float pi=0.0f;
			float yaw=45;
			camPosition = glm::vec4(-69.0f, 15.0f, -71.0f, 1.0f); 
			cameraFront = glm::vec3(glm::cos(pi)*glm::cos(yaw), glm::sin(pi), glm::cos(pi)*glm::cos(yaw));  
			
			
		}else if(currentcam==3){
			float pi=45.0f;  //Rat view (three point perspective)
			float yaw=0.f;
			camPosition = glm::vec4(-43.0f, -33.0f, -43.0f, 1.0); 
			cameraFront = glm::vec3(glm::cos(pi)*glm::cos(yaw), glm::sin(pi), glm::cos(pi)*glm::cos(yaw));  
			
			
		}else if(currentcam==4){
			float pi=-45.0f;   //Bird view (three point perspective)
			float yaw=0.0f;
			camPosition = glm::vec4(-43.0f, 83.0f, -43.0f, 1.0); 
			cameraFront = glm::vec3(glm::cos(pi)*glm::cos(yaw), glm::sin(pi), glm::cos(pi)*glm::cos(yaw));  
			
			
		}else{
			currentcam=0;
			camPosition = glm::vec4(20.0, 30.0, 80.0, 1.0);
			cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
			
			
		}
		}else if(ImGui::IsKeyPressed('o') || ImGui::IsKeyPressed('O')){
			setupOrthographicProjection(shaderProgram);
		}else if(ImGui::IsKeyPressed('p') || ImGui::IsKeyPressed('P')){
			setupProjectionTransformation(shaderProgram);
		}else if(circle==1){
			strcpy(textKeyStatus, "Key status: A/a");
			// camPosition = glm::vec4(20.0, 70.0, 80.0, 1.0);
			const float radius = 50.0f;
			float camX = sin(glfwGetTime()) * radius;
			float camZ = cos(glfwGetTime()) * radius;
			camPosition.y=70;
			camPosition.x=camX;
			camPosition.z=camZ;
			// cameraFront=glm::vec3(0.0f,0.0f,0.0f);

		}
		else {strcpy(textKeyStatus, "Key status:");
			}
    // Handle Ctrl key state
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
        ctrlKeyPressed = true;
    }
    else {
        ctrlKeyPressed = false;
    }

	if(ImGui::IsKeyPressed('a') || ImGui::IsKeyPressed('A')){
		circle=circle*-1;
	}
	

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		glUseProgram(shaderProgram);

		//ImGui UI menu
		ImGui::Begin("Main", NULL, ImGuiWindowFlags_AlwaysAutoResize);                          
		if(ImGui::CollapsingHeader("Information", ImGuiTreeNodeFlags_DefaultOpen))
		{
		  ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::Text("%s", textKeyStatus);
			ImGui::Text("Camera position: (%.2f, %.2f, %.2f)", camPosition.x, camPosition.y, camPosition.z);
		}
		ImGui::End();

		// Rendering
		ImGui::Render();
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		setupModelTransformationCube(shaderProgram);
		glBindVertexArray(cube_VAO); 
		glUniform4f(vColor_uniform, 0.5, 0.5, 0.5, 1.0);
		glDrawArrays(GL_TRIANGLES, 0, 6*2*3);
		glUniform4f(vColor_uniform, 0.0, 0.0, 0.0, 1.0);
		glDrawArrays(GL_LINE_STRIP, 0, 6*2*3);

    glDisable(GL_DEPTH_TEST); // Disable depth test for drawing axes. We want axes to be drawn on top of all

		glBindVertexArray(axis_VAO); 
		setupModelTransformationAxis(shaderProgram, 0.0, glm::vec3(0, 0, 1));
		glUniform4f(vColor_uniform, 1.0, 0.0, 0.0, 1.0); //Red -> X 
		glDrawArrays(GL_LINES, 0, 2);

		setupModelTransformationAxis(shaderProgram, glm::radians(90.0), glm::vec3(0, 0, 1));
		glUniform4f(vColor_uniform, 0.0, 1.0, 0.0, 1.0); //Green -> Y
		glDrawArrays(GL_LINES, 0, 2);

		setupModelTransformationAxis(shaderProgram, -glm::radians(90.0), glm::vec3(0, 1, 0));
		glUniform4f(vColor_uniform, 0.0, 0.0, 1.0, 1.0); //Blue -> Z
		glDrawArrays(GL_LINES, 0, 2);

    glEnable(GL_DEPTH_TEST); // Enable depth test again

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
	}

	// Cleanup
	cleanup(window);

	return 0;
}

void createCubeObject(unsigned int &program, unsigned int &cube_VAO)
{
	glUseProgram(program);

	//Bind shader variables
	int vVertex_attrib = glGetAttribLocation(program, "vVertex");
	if(vVertex_attrib == -1) {
		fprintf(stderr, "Could not bind location: vVertex\n");
		exit(0);
	}

	//Cube data
	GLfloat cube_vertices[] = {10, 10, -10, -10, 10, -10, -10, -10, -10, 10, -10, -10, //Front
		10, 10, 10, -10, 10, 10, -10, -10, 10, 10, -10, 10}; //Back
	GLushort cube_indices[] = {
		0, 1, 2, 0, 2, 3, //Front
		4, 7, 5, 5, 7, 6, //Back
		1, 6, 2, 1, 5, 6, //Left
		0, 3, 4, 4, 7, 3, //Right
		0, 4, 1, 4, 5, 1, //Top
		2, 6, 3, 3, 6, 7 //Bottom
	};

	//Generate VAO object
	glGenVertexArrays(1, &cube_VAO);
	glBindVertexArray(cube_VAO);

	//Create VBOs for the VAO
	//Position information (data + format)
	int nVertices = (6*2)*3; //(6 faces) * (2 triangles each) * (3 vertices each)
	GLfloat *expanded_vertices = new GLfloat[nVertices*3];
	for(int i=0; i<nVertices; i++) {
		expanded_vertices[i*3] = cube_vertices[cube_indices[i]*3];
		expanded_vertices[i*3 + 1] = cube_vertices[cube_indices[i]*3+1];
		expanded_vertices[i*3 + 2] = cube_vertices[cube_indices[i]*3+2];
	}
	GLuint vertex_VBO;
	glGenBuffers(1, &vertex_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_VBO);
	glBufferData(GL_ARRAY_BUFFER, nVertices*3*sizeof(GLfloat), expanded_vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(vVertex_attrib);
	glVertexAttribPointer(vVertex_attrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
	delete []expanded_vertices;

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0); //Unbind the VAO to disable changes outside this function.
}

void createAxesLine(unsigned int & program, unsigned int &axis_VAO)
{
  glUseProgram(program);

	//Bind shader variables
	int vVertex_attrib_position = glGetAttribLocation(program, "vVertex");
	if(vVertex_attrib_position == -1) {
		fprintf(stderr, "Could not bind location: vVertex\n");
		exit(0);
	}

	//Axes data
	GLfloat axis_vertices[] = {	0, 0, 0, 20, 0, 0}; //X-axis
  glGenVertexArrays(1, &axis_VAO);
	glBindVertexArray(axis_VAO);

	//Create VBO for the VAO
	int nVertices = 2; // 2 vertices
	GLuint vertex_VBO;
	glGenBuffers(1, &vertex_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_VBO);
	glBufferData(GL_ARRAY_BUFFER, nVertices*3*sizeof(GLfloat), axis_vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(vVertex_attrib_position);
	glVertexAttribPointer(vVertex_attrib_position, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0); //Unbind the VAO to disable changes outside this function.
}

void setupModelTransformationCube(unsigned int &program)
{
	//Modelling transformations (Model -> World coordinates)
	modelT = glm::scale(glm::mat4(1.0f), glm::vec3(1.0, 2.0, 1.0));
	modelT = glm::translate(modelT, glm::vec3(0.0f, 10.0f, 0.0f));

	//Pass on the modelling matrix to the vertex shader
	glUseProgram(program);
	vModel_uniform = glGetUniformLocation(program, "vModel");
	if(vModel_uniform == -1){
		fprintf(stderr, "Could not bind location: vModel\n");
		exit(0);
	}
	glUniformMatrix4fv(vModel_uniform, 1, GL_FALSE, glm::value_ptr(modelT));
}

void setupModelTransformationAxis(unsigned int &program, float rot_angle, glm::vec3 rot_axis)
{
	//Modelling transformations (Model -> World coordinates)
	modelT = glm::rotate(glm::mat4(1.0f), rot_angle, rot_axis);

	//Pass on the modelling matrix to the vertex shader
	glUseProgram(program);
	vModel_uniform = glGetUniformLocation(program, "vModel");
	if(vModel_uniform == -1){
		fprintf(stderr, "Could not bind location: vModel\n");
		exit(0);
	}
	glUniformMatrix4fv(vModel_uniform, 1, GL_FALSE, glm::value_ptr(modelT));
}


void setupViewTransformation(unsigned int &program)
{
	//Viewing transformations (World -> Camera coordinates
	const float radius = 50.0f;
float camX = sin(glfwGetTime()) * radius;
float camZ = cos(glfwGetTime()) * radius;
	// viewT = glm::lookAt(glm::vec3(camX-30, 100, camZ-30), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));  
	viewT = glm::lookAt(glm::vec3(camPosition),  cameraFront, cameraUp);


	//Pass-on the viewing matrix to the vertex shader
	glUseProgram(program);
	vView_uniform = glGetUniformLocation(program, "vView");
	if(vView_uniform == -1){
		fprintf(stderr, "Could not bind location: vView\n");
		exit(0);
	}
	glUniformMatrix4fv(vView_uniform, 1, GL_FALSE, glm::value_ptr(viewT));
}
void setupOrthographicProjection(unsigned int &program)
{
    // Orthographic projection transformation
    projectionT = glm::ortho(-50.0f, 50.0f, -50.0f, 50.0f, 0.1f, 1000.0f);

    // Pass the orthographic projection matrix to the vertex shader
    glUseProgram(program);
    vProjection_uniform = glGetUniformLocation(program, "vProjection");
    if (vProjection_uniform == -1) {
        fprintf(stderr, "Could not bind location: vProjection\n");
        exit(0);
    }
    glUniformMatrix4fv(vProjection_uniform, 1, GL_FALSE, glm::value_ptr(projectionT));
}

void setupProjectionTransformation(unsigned int &program)
{
	//Projection transformation
	projectionT = glm::perspective(45.0f, (GLfloat)screen_width/(GLfloat)screen_height, 0.1f, 1000.0f);

	//Pass on the projection matrix to the vertex shader
	glUseProgram(program);
	vProjection_uniform = glGetUniformLocation(program, "vProjection");
	if(vProjection_uniform == -1){
		fprintf(stderr, "Could not bind location: vProjection\n");
		exit(0);
	}
	glUniformMatrix4fv(vProjection_uniform, 1, GL_FALSE, glm::value_ptr(projectionT));
}

