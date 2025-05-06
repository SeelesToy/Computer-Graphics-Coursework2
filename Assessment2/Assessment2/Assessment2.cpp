#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>


#include "error.h"
#include "file.h"
#include "shader.h"
#include "camera.h"
#include "shadow.h"
#include "model.h"

glm::vec3 lightDirection = glm::vec3(0.1f, -.81f, -.61f);
glm::vec3 lightPos = glm::vec3(2.f, 6.f, 7.f);
SCamera Camera;
float camX = .0f;
float camY = .0f;
float camZ = .0f;
float fov = 45.;
Model diceModel;

#define NUM_BUFFERS 1
#define NUM_VAOS 1
GLuint Buffers[NUM_BUFFERS];
GLuint VAOs[NUM_VAOS];

unsigned int VAO[3];
unsigned int VBO[3];

#define WIDTH 1024
#define HEIGHT 768
#define SH_MAP_WIDTH 2048
#define SH_MAP_HEIGHT 2048
float floorVertices[] = {
	// positions          // col			//SPECIFY ALPHA COORDINATE HERE
	-0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f, 1.f, 0, 0, -1,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f, 1.f, 0, 0, -1,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f, 1.f, 0, 0, -1,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f, 1.f, 0, 0, -1,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f, 1.f, 0, 0, -1,
	-0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f, 1.f, 0, 0, -1,

	-0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f, 1.f, 0, 0, 1,
	 0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f, 1.f, 0, 0, 1,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f, 1.f, 0, 0, 1,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f, 1.f, 0, 0, 1,
	-0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f, 1.f, 0, 0, 1,
	-0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f, 1.f, 0, 0, 1,

	-0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f, 1.f, -1, 0, 0,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f, 1.f, -1, 0, 0,
	-0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f, 1.f, -1, 0, 0,
	-0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f, 1.f, -1, 0, 0,
	-0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f, 1.f, -1, 0, 0,
	-0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f, 1.f, -1, 0, 0,

	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f, 1.f, 1, 0, 0,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f, 1.f, 1, 0, 0,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f, 1.f, 1, 0, 0,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f, 1.f, 1, 0, 0,
	 0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f, 1.f, 1, 0, 0,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f, 1.f, 1, 0, 0,

	-0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f, 1.f, 0, -1, 0,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f, 1.f, 0, -1, 0,
	 0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f, 1.f, 0, -1, 0,
	 0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f, 1.f, 0, -1, 0,
	-0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f, 1.f, 0, -1, 0,
	-0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f, 1.f, 0, -1, 0,

	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.f, 1.f, 0, 1, 0,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.f, 1.f, 0, 1, 0,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.f, 1.f, 0, 1, 0,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.f, 1.f, 0, 1, 0,
	-0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.f, 1.f, 0, 1, 0,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.f, 1.f, 0, 1, 0,
};

float cubeVertices[] = {
	// positions          // col			//SPECIFY ALPHA COORDINATE HERE
	-0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f, 1.f, 0, 0, -1,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f, 1.f, 0, 0, -1,
	 0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f, 1.f, 0, 0, -1,
	 0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f, 1.f, 0, 0, -1,
	-0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f, 1.f, 0, 0, -1,
	-0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f, 1.f, 0, 0, -1,

	-0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 1.f, 0, 0, 1,
	 0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 1.f, 0, 0, 1,
	 0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 1.f, 0, 0, 1,
	 0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 1.f, 0, 0, 1,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 1.f, 0, 0, 1,
	-0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 1.f, 0, 0, 1,

	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.f, 1.f, -1, 0, 0,
	-0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.f, 1.f, -1, 0, 0,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.f, 1.f, -1, 0, 0,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.f, 1.f, -1, 0, 0,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.f, 1.f, -1, 0, 0,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.f, 1.f, -1, 0, 0,

	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.f, 1.f, 1, 0, 0,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.f, 1.f, 1, 0, 0,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.f, 1.f, 1, 0, 0,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.f, 1.f, 1, 0, 0,
	 0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 0.f, 1.f, 1, 0, 0,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.f, 1.f, 1, 0, 0,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.f, 1.f, 0, -1, 0,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.f, 1.f, 0, -1, 0,
	 0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.f, 1.f, 0, -1, 0,
	 0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.f, 1.f, 0, -1, 0,
	-0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.f, 1.f, 0, -1, 0,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.f, 1.f, 0, -1, 0,

	-0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 1.f, 1.f, 0, 1, 0,
	 0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 1.f, 1.f, 0, 1, 0,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.f, 1.f, 0, 1, 0,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.f, 1.f, 0, 1, 0,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.f, 1.f, 0, 1, 0,
	-0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 1.f, 1.f, 0, 1, 0,
};

float transparentVertices[] = {
	// positions				// RGB				//SPECIFY ALPHA COORDINATE HERE
	-.5f,  -0.5f,  0.0f,		1.0f,  0.0f, 0.0f,  0.2f, 0, 0, 1,
	.0f, 0.5f,  0.0f,			1.0f,  0.0f, 0.0f,  0.2f, 0, 0, 1,
	.5f, -0.5f,  0.0f,			1.0f,  0.0f, 0.0f,  0.2f, 0, 0, 1
};

void drawCubes(unsigned int shaderProgram) {
	glBindVertexArray(VAO[0]);
	glm::mat4 model = glm::mat4(1.f);
	model = glm::mat4(1.f);
	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
	model = glm::rotate(model, (float)glfwGetTime() / 2.f, glm::vec3(0.0f, 1.0f, .0f));
	model = glm::scale(model, glm::vec3(.5, .5, .5));
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniform1i(glGetUniformLocation(shaderProgram, "useTexture"), false);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(1.0f, 0.0f, -1.0f));
	model = glm::rotate(model, (float)glfwGetTime() / -2.f, glm::vec3(0.0f, 1.0f, .0f));
	model = glm::scale(model, glm::vec3(.5, .5, .5));
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniform1i(glGetUniformLocation(shaderProgram, "useTexture"), false);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

void drawFloorAndCubes(unsigned int shaderProgram)
{
	glBindVertexArray(VAO[1]);
	//floor
	glm::mat4 model = glm::mat4(1.f);
	model = glm::translate(model, glm::vec3(0, -1, 0));
	model = glm::scale(model, glm::vec3(100, 0.1, 100));
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniform1i(glGetUniformLocation(shaderProgram, "useTexture"), false);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	//cubes
	glBindVertexArray(VAO[0]);
	model = glm::mat4(1.f);
	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
	model = glm::rotate(model, (float)glfwGetTime() / 2.f, glm::vec3(0.0f, 1.0f, .0f));
	model = glm::scale(model, glm::vec3(.5, .5, .5));
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniform1i(glGetUniformLocation(shaderProgram, "useTexture"), false);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(1.0f, 0.0f, -1.0f));
	model = glm::rotate(model, (float)glfwGetTime() / -2.f, glm::vec3(0.0f, 1.0f, .0f));
	model = glm::scale(model, glm::vec3(.5, .5, .5));
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniform1i(glGetUniformLocation(shaderProgram, "useTexture"), false);
	glDrawArrays(GL_TRIANGLES, 0, 36);

}

void processKeyboard(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		lightDirection = Camera.Front;
		lightPos = Camera.Position;
	}
	float x_offset = 0.f;
	float y_offset = 0.f;
	bool cam_changed = false;
	const float cameraSpeed = 0.05f;
	glm::vec3 front = glm::normalize(Camera.Front);
	glm::vec3 right = glm::normalize(glm::cross(front, Camera.Up));

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camX += front.x * cameraSpeed;
		camY += front.y * cameraSpeed;
		camZ += front.z * cameraSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camX -= front.x * cameraSpeed;
		camY -= front.y * cameraSpeed;
		camZ -= front.z * cameraSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camX += right.x * cameraSpeed;
		// 通常不改变 Y
		camZ += right.z * cameraSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camX -= right.x * cameraSpeed;
		camZ -= right.z * cameraSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		x_offset = 1.f;
		y_offset = 0.f;
		cam_changed = true;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		x_offset = -1.f;
		y_offset = 0.f;
		cam_changed = true;
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		x_offset = 0.f;
		y_offset = 1.f;
		cam_changed = true;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		x_offset = 0.f;
		y_offset = -1.f;
		cam_changed = true;
	}
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
	{
		cam_dist -= 0.1f;
		cam_changed = true;
	}
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
	{
		cam_dist += 0.1f;
		cam_changed = true;
	}
	if (cam_changed)
	{
		MoveAndOrientCamera(Camera, glm::vec3(0.f, 0.f, 0.f), cam_dist, x_offset, y_offset);
		cam_changed = false;
	}
}


void SizeCallback(GLFWwindow*, int w, int h)
{
	glViewport(0, 0, w, h);
}

void drawTransparentTriangles(unsigned int renderProgram, std::vector<glm::vec3> positions) {

	glEnable(GL_BLEND);                              // 打开混合
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDepthMask(GL_FALSE);
	glm::mat4 model = glm::mat4(1.f);
	//SORT THE TRANSPARENT TRIANGLE POSITIONS HERE
	std::map<float, glm::vec3> sorted_positions;
	for (int i = 0; i < positions.size(); i++)
	{
		glm::vec3 toObj = positions[i] - Camera.Position;
		float depth = glm::dot(Camera.Front, toObj);
		sorted_positions.insert(std::make_pair(depth, positions[i]));
	}


	glBindVertexArray(VAO[2]);
	glUniform1i(glGetUniformLocation(renderProgram, "useTexture"), false);
	for (auto it = sorted_positions.rbegin(); it != sorted_positions.rend(); ++it)
	{
		model = glm::mat4(1.0f);
		model = glm::translate(model, it->second);
		glUniformMatrix4fv(glGetUniformLocation(renderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));


		glDrawArrays(GL_TRIANGLES, 0, 3);
	}

	glDepthMask(GL_TRUE);
}
void generateDepthMap(unsigned int shadowShaderProgram, ShadowStruct shadow, glm::mat4 ProjectedLightSpaceMatrix)
{

	glViewport(0, 0, SH_MAP_WIDTH, SH_MAP_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, shadow.FBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	glUseProgram(shadowShaderProgram);
	glUniformMatrix4fv(glGetUniformLocation(shadowShaderProgram, "projectedLightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(ProjectedLightSpaceMatrix));
	drawCubes(shadowShaderProgram);
	glm::mat4 model = glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.f, 0.f));
	model = glm::scale(model, glm::vec3(.5, .5, .5));
	renderModel(diceModel, shadowShaderProgram, model);
	glCullFace(GL_BACK);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}
void renderWithShadow(unsigned int renderShadowProgram, ShadowStruct shadow, glm::mat4 ProjectedLightSpaceMatrix)
{
	glViewport(0, 0, WIDTH, HEIGHT);

	static const GLfloat bgd[] = { .8f, .8f, .8f, 1.f };
	glClearBufferfv(GL_COLOR, 0, bgd);
	glClear(GL_DEPTH_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glUseProgram(renderShadowProgram);

	glBindTexture(GL_TEXTURE_2D, shadow.Texture);

	glUniformMatrix4fv(glGetUniformLocation(renderShadowProgram, "projectedLightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(ProjectedLightSpaceMatrix));
	glUniform3f(glGetUniformLocation(renderShadowProgram, "lightDirection"), lightDirection.x, lightDirection.y, lightDirection.z);
	glUniform3f(glGetUniformLocation(renderShadowProgram, "lightColour"), 1.f, 1.f, 1.f);
	glUniform3f(glGetUniformLocation(renderShadowProgram, "camPos"), Camera.Position.x, Camera.Position.y, Camera.Position.z);
	glUniform3f(glGetUniformLocation(renderShadowProgram, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	glm::mat4 view = glm::mat4(1.f);
	view = glm::lookAt(Camera.Position, Camera.Position + Camera.Front, Camera.Up);
	view = glm::translate(view, -glm::vec3(camX, camY ,camZ));
	glUniformMatrix4fv(glGetUniformLocation(renderShadowProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));

	glm::mat4 projection = glm::mat4(1.f);
	projection = glm::perspective(glm::radians(fov), (float)WIDTH / (float)HEIGHT, .01f, 100.f);
	glUniformMatrix4fv(glGetUniformLocation(renderShadowProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));


	drawFloorAndCubes(renderShadowProgram);
	glm::mat4 model = glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.f, 0.f));
	model = glm::scale(model, glm::vec3(.5, .5, .5));
	renderModel(diceModel, renderShadowProgram, model);

}



int main(int argc, char** argv)
{
	glfwInit();
	glfwWindowHint(GLFW_SAMPLES, 64);
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Render CW2", NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSetWindowSizeCallback(window, SizeCallback);

	gl3wInit();

	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(DebugCallback, 0);

	glEnable(GL_DEPTH_TEST);
	//ENABLE BLENDING HERE
	glEnable(GL_BLEND);

	//SET BLEND FUNCTION HERE
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	ShadowStruct shadow = setup_shadowmap(SH_MAP_WIDTH, SH_MAP_HEIGHT);
	GLuint program = CompileShader("triangle.vert", "triangle.frag");
	GLuint shadow_program = CompileShader("shadow.vert", "shadow.frag");
	InitCamera(Camera);
	cam_dist = 5.f;
	MoveAndOrientCamera(Camera, glm::vec3(0.f, 0.f, 0.f), cam_dist, 0.f, 0.f);

	if (!loadModel("resources/models/Dice.obj",
		"resources/textures/dice_text.png",
		diceModel)) {
		std::cerr << "加载模型失败\n";
		return -1;
	}


	glGenVertexArrays(3, VAO);
	glGenBuffers(3, VBO);

	//INCLUDE ALPHA COORDINATE IN THE VAO HERE
	//cube
	glBindVertexArray(VAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(7 * sizeof(float)));
	glEnableVertexAttribArray(2);
	//floor
	glBindVertexArray(VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(floorVertices), floorVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(7 * sizeof(float)));
	glEnableVertexAttribArray(2);
	//transparent
	glBindVertexArray(VAO[2]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(transparentVertices), transparentVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(7 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glUseProgram(program);


	std::vector<glm::vec3> positions;
	positions.push_back(glm::vec3(0, 0, 3.f));
	positions.push_back(glm::vec3(0.5f, 0, 1.f));
	positions.push_back(glm::vec3(0, 0, 2.f));
	positions.push_back(glm::vec3(-.5f, 0, 1.5f));
	positions.push_back(glm::vec3(0, 0, 2.5f));


	while (!glfwWindowShouldClose(window))
	{
		float near_plane = 1.0f, far_plane = 70.5f;
		glm::mat4 lightProjection = glm::ortho(-10.f, 10.f, -10.f, 10.f, 1.f, 70.f);
		glm::mat4 lightView = glm::lookAt(lightPos, lightPos + lightDirection, glm::vec3(0.f, 1.f, 0.f));
		glm::mat4 projectedLightSpaceMatrix = lightProjection * lightView;

		generateDepthMap(shadow_program, shadow, projectedLightSpaceMatrix);

		//saveShadowMapToBitmap(shadow.Texture, SH_MAP_WIDTH, SH_MAP_HEIGHT);
		renderWithShadow(program, shadow, projectedLightSpaceMatrix);

		//drawFloorAndCubes(program);

		drawTransparentTriangles(program, positions);

		glfwSwapBuffers(window);

		glfwPollEvents();
		processKeyboard(window);
	}

	glDeleteVertexArrays(3, VAO);
	glDeleteBuffers(3, VBO);

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}