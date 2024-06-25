#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// 顶点着色器源代码
const char* verticesShaderSource = "#version 330 core\n"
"layout	(location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\0";


int main() {
	glfwInit();
	// 设置主要版本和次要版本
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_CORE_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// 创建窗口对象
	GLFWwindow* window = glfwCreateWindow(800, 600, "opengl-test1", NULL, NULL);
	if (window == NULL) {
		std::cout << "failed to create window " << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window); // 将窗口的上下文设置为当前线程的主上下文'
	// 注册回调函数
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


	// 初始化GLAD glad管理顶点指针
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "failed to initialize GLAD" << std::endl;
		return -1;
	}

	// 设置视口（窗口维度）
	glViewport(0, 0, 800, 600); // 前两个左下角坐标，后两个创就长宽


	// 顶点坐标
	float vertices[] = {
		 -0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};



	// 1.创建顶点着色器
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	// 着色器源码复附加到着色器
	glShaderSource(vertexShader, 1, &verticesShaderSource, NULL);
	glCompileShader(vertexShader);

	// 检测编译是否成功
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "vertexShader compile error" << std::endl;
	}

	// 2.创建片段着色器
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "fragmentShader compile error" << std::endl;
	}

	// 3.创建着色器程序对象
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram(); // 返回程序ID引用

	// 将两个着色器链接到着色器程序上
	glAttachShader(shaderProgram, vertexShader); // 将着色器附加到程序上
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram); // 链接

	// 检测链接是否失效
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "shaderProgram link error" << std::endl;
	}

	// 链接后即可删除着色器对象
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	// 创建VAO（顶点数组对象）
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);

	// 生成VBO对象（顶点缓冲对象）
	unsigned int VBO;
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	// 绑定缓冲
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// 填充数据
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// 告诉OpenGL如何解析顶点数据(设置顶点属性指针)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);




	// 渲染循环
	while (!glfwWindowShouldClose(window)) {
		processInput(window); // 输入
		 
		// 渲染指令
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// 激活程序对象
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO); // 绑定VAO
		glDrawArrays(GL_TRIANGLES, 0, 3); // 绘制三角形


		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);

	glfwTerminate();

	return 0;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}


// 接收输入
void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}