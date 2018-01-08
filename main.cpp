//internal includes
#include "common.h"
#include "ShaderProgram.h"
#include "Camera.h"
#include "Mesh.h"
#include "Model.h"

//External dependencies
#define GLFW_DLL
#include <GLFW/glfw3.h>
#include <SOIL/SOIL.h>
#include <random>
#include <ctime>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

static const GLsizei WIDTH = 1280, HEIGHT = 1024; //размеры окна
static float max_h = 0.0f, min_h = 100000.0f;
static int filling = 0;
static int def_vision = 1;
static int col_norm = 0;
static int fog = 0;
static bool keys[1024]; //массив состояний кнопок - нажата/не нажата
static GLfloat lastX = 400, lastY = 300; //исходное положение мыши
static bool firstMouse = true;
static bool g_captureMouse         = true;  // Мышка захвачена нашим приложением или нет?
static bool g_capturedMouseJustNow = false;


float max_x = 0.0f, max_z = 0.0f;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

Camera camera(float3(5.0f, 10.0f, 20.0f));

//функция для обработки нажатий на кнопки клавиатуры
void OnKeyboardPressed(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	//std::cout << key << std::endl;
	switch (key)
	{
	case GLFW_KEY_ESCAPE: //на Esc выходим из программы
		if (action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GL_TRUE);
		break;
	case GLFW_KEY_SPACE: //на пробел переключение в каркасный режим и обратно
		if (action == GLFW_PRESS)
		{
			if (filling == 0)
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				filling = 1;
			}
			else
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				filling = 0;
			}
		}
		break;
	case GLFW_KEY_1:
		if (action == GLFW_PRESS)
		{
			if (col_norm == 1)
			{
				col_norm = 0;
				def_vision = 1;
			}
		}
		break;
	case GLFW_KEY_2:
		if (action == GLFW_PRESS)
		{
			if (def_vision == 1)
			{
				def_vision = 0;
				col_norm = 1;
			}
		}
		break;
	case GLFW_KEY_F:
		if (action == GLFW_PRESS)
		{
			if (fog == 0)
			{
				fog = 1;
			}
			else 
			{
				fog = 0;
			}
		}
		break;
	default:
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}

//функция для обработки клавиш мыши
void OnMouseButtonClicked(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
	g_captureMouse = !g_captureMouse;


	if (g_captureMouse)
	{
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	g_capturedMouseJustNow = true;
	}
	else
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

}

//функция для обработки перемещения мыши
void OnMouseMove(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
	lastX = float(xpos);
	lastY = float(ypos);
	firstMouse = false;
	}

	GLfloat xoffset = float(xpos) - lastX;
	GLfloat yoffset = lastY - float(ypos);  

	lastX = float(xpos);
	lastY = float(ypos);

	if (g_captureMouse)
	camera.ProcessMouseMove(xoffset, yoffset);
}


void OnMouseScroll(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(GLfloat(yoffset));
}

void doCameraMovement(Camera &camera, GLfloat deltaTime)
{
	if (keys[GLFW_KEY_W])
	camera.ProcessKeyboard(FORWARD, deltaTime);
	if (keys[GLFW_KEY_A])
	camera.ProcessKeyboard(LEFT, deltaTime);
	if (keys[GLFW_KEY_S])
	camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (keys[GLFW_KEY_D])
	camera.ProcessKeyboard(RIGHT, deltaTime);
}


/**
\brief создать triangle strip плоскость и загрузить её в шейдерную программу
\param rows - число строк
\param cols - число столбцов
\param size - размер плоскости
\param vao - vertex array object, связанный с созданной плоскостью
*/


double myrand(double min, double max)
{
	double tmp = (double)rand() / RAND_MAX;
	return min + tmp * (max - min);
}

static int createTriStrip(int rows, int cols, float size, GLuint &vao, float R)
{

	float MIN = -R * cols, MAX = R * cols;
	int numIndices = 2 * cols*(rows - 1) + rows - 1;

	std::vector<GLfloat> vertices_vec; //вектор атрибута координат вершин
	vertices_vec.reserve(rows * cols * 3);

	std::vector<GLfloat> normals_vec; //вектор атрибута нормалей к вершинам
	normals_vec.reserve(rows * cols * 3);

	std::vector<GLfloat> texcoords_vec; //вектор атрибут текстурных координат вершин
	texcoords_vec.reserve(rows * cols * 2);

	std::vector<float3> normals_vec_tmp(rows * cols, float3(0.0f, 0.0f, 0.0f)); //временный вектор нормалей, используемый для расчетов

	std::vector<int3> faces;         //вектор граней (треугольников), каждая грань - три индекса вершин, её составляющих; используется для удобства расчета нормалей
	faces.reserve(numIndices / 3);

	std::vector<GLuint> indices_vec; //вектор индексов вершин для передачи шейдерной программе
	indices_vec.reserve(numIndices);

	srand(500);

	double h = R;
	std::vector< std::vector<double> > matrix(rows, std::vector<double>(cols));

	for (int z = 0; z < rows; ++z)
	{
	for (int x = 0; x < cols; ++x)
	{
		//вычисляем координаты каждой из вершин 
		float xx = -size / 2 + x*size / cols;
		float zz = -size / 2 + z*size / rows;
		float yy = 0.0f;

		// if (max_x < xx) max_x = x;
		// if (max_z < zz) max_z = z;

		matrix[z][x] = 0.0f;

		vertices_vec.push_back(xx);
		vertices_vec.push_back(yy);
		vertices_vec.push_back(zz);

		texcoords_vec.push_back(x / float(cols - 1)); // вычисляем первую текстурную координату u, для плоскости это просто относительное положение вершины
		texcoords_vec.push_back(z / float(rows - 1)); // аналогично вычисляем вторую текстурную координату v
	}
	}

	for (int step_size = rows - 1; step_size >= 2; step_size /= 2, h /= 2.0) 
	{
		int half_step = step_size / 2;

		for (int x = 0; x < rows - 1; x += step_size) 
		{
			for (int y = 0; y < rows - 1; y += step_size) 
			{
				double mid = 
										matrix[x][y] + 
										matrix[x + step_size][y] + 
										matrix[x][y + step_size] + 
										matrix[x + step_size][y + step_size];
				mid /= 4.0;

				matrix[x + half_step][y + half_step] = mid + (myrand(0, 1) * 2 * h) - h;
			}
		}
		for (int x = 0; x < rows - 1; x += half_step) 
		{
			for (int y = (x + half_step) % step_size; y < rows - 1; y += step_size) 
			{
				float mid =
					matrix[(x - half_step + rows - 1) % (rows - 1)][y] + 
					matrix[(x + half_step) % (rows - 1)][y] + 
					matrix[x][(y + half_step) % (rows - 1)] + 
					matrix[x][(y - half_step + rows - 1) % (rows - 1)]; 
				mid /= 4.0;

				mid = mid + (myrand(0, 1) * 2 * h) - h;
				matrix[x][y] = mid;

				if (x == 0) matrix[rows - 1][y] = mid;
				if (y == 0) matrix[x][rows - 1] = mid;
			}
		}
	}
	std::vector<double> h_vec;
	for(int i = 0; i < matrix.size(); i++)
		for (int j = 0; j < matrix.size(); j++)
		{
			h_vec.push_back(matrix[i][j]);
			if (matrix[i][j] > max_h)  max_h = matrix[i][j];
			if (matrix[i][j] < min_h) min_h = matrix[i][j];
		}

	int it = 0;
	for(int i = 0; i < vertices_vec.size(); i++)
	{
		if ((i % 3) == 1) 
		{
			vertices_vec[i] = h_vec[it++];
		} 
	}
	std::cout << "Max H: " << max_h << std::endl << "Min H: " << min_h << std::endl;
	//primitive restart - специальный индекс, который обозначает конец строки из треугольников в triangle_strip
	//после этого индекса формирование треугольников из массива индексов начнется заново - будут взяты следующие 3 индекса для первого треугольника
	//и далее каждый последующий индекс будет добавлять один новый треугольник пока снова не встретится primitive restart index

	int primRestart = cols * rows;

	for (int x = 0; x < cols - 1; ++x)
	{
		for (int z = 0; z < rows - 1; ++z)
		{
			int offset = x*cols + z;
	
			//каждую итерацию добавляем по два треугольника, которые вместе формируют четырехугольник
			if (z == 0) //если мы в начале строки треугольников, нам нужны первые четыре индекса
			{
			indices_vec.push_back(offset + 0);
			indices_vec.push_back(offset + rows);
			indices_vec.push_back(offset + 1);
			indices_vec.push_back(offset + rows + 1);
			}
			else // иначе нам достаточно двух индексов, чтобы добавить два треугольника
			{
			indices_vec.push_back(offset + 1);
			indices_vec.push_back(offset + rows + 1);
	
			if (z == rows - 2) indices_vec.push_back(primRestart); // если мы дошли до конца строки, вставляем primRestart, чтобы обозначить переход на следующую строку
			}
	}
	}

	///////////////////////
	//формируем вектор граней(треугольников) по 3 индекса на каждый
	int currFace = 1;
	for (int i = 0; i < indices_vec.size() - 2; ++i)
	{
	int3 face;

	int index0 = indices_vec.at(i);
	int index1 = indices_vec.at(i + 1);
	int index2 = indices_vec.at(i + 2);

	if (index0 != primRestart && index1 != primRestart && index2 != primRestart)
	{
		if (currFace % 2 != 0) //если это нечетный треугольник, то индексы и так в правильном порядке обхода - против часовой стрелки
		{
		face.x = indices_vec.at(i);
		face.y = indices_vec.at(i + 1);
		face.z = indices_vec.at(i + 2);

		currFace++;
		}
		else //если треугольник четный, то нужно поменять местами 2-й и 3-й индекс;
		{    //при отрисовке opengl делает это за нас, но при расчете нормалей нам нужно это сделать самостоятельно
		face.x = indices_vec.at(i);
		face.y = indices_vec.at(i + 2);
		face.z = indices_vec.at(i + 1);

		currFace++;
		}
		faces.push_back(face);
	}
	}


	///////////////////////
	//расчет нормалей
	for (int i = 0; i < faces.size(); ++i)
	{
		//получаем из вектора вершин координаты каждой из вершин одного треугольника
		float3 A(vertices_vec.at(3 * faces.at(i).x + 0), vertices_vec.at(3 * faces.at(i).x + 1), vertices_vec.at(3 * faces.at(i).x + 2));
		float3 B(vertices_vec.at(3 * faces.at(i).y + 0), vertices_vec.at(3 * faces.at(i).y + 1), vertices_vec.at(3 * faces.at(i).y + 2));
		float3 C(vertices_vec.at(3 * faces.at(i).z + 0), vertices_vec.at(3 * faces.at(i).z + 1), vertices_vec.at(3 * faces.at(i).z + 2));
	
		//получаем векторы для ребер треугольника из каждой из 3-х вершин
		float3 edge1A(normalize(B - A));
		float3 edge2A(normalize(C - A));
	
		float3 edge1B(normalize(A - B));
		float3 edge2B(normalize(C - B));
	
		float3 edge1C(normalize(A - C));
		float3 edge2C(normalize(B - C));
	
		//нормаль к треугольнику - векторное произведение любой пары векторов из одной вершины
		float3 face_normal = cross(edge1A, edge2A);
	
		//простой подход: нормаль к вершине = средняя по треугольникам, к которым принадлежит вершина
		normals_vec_tmp.at(faces.at(i).x) += face_normal;
		normals_vec_tmp.at(faces.at(i).y) += face_normal;
		normals_vec_tmp.at(faces.at(i).z) += face_normal;
	}

	//нормализуем векторы нормалей и записываем их в вектор из GLFloat, который будет передан в шейдерную программу
	for (int i = 0; i < normals_vec_tmp.size(); ++i)
	{
	float3 N = normalize(normals_vec_tmp.at(i));

	normals_vec.push_back(N.x);
	normals_vec.push_back(N.y);
	normals_vec.push_back(N.z);
	}

	GLuint vboVertices, vboIndices, vboNormals, vboTexCoords;

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vboVertices);
	glGenBuffers(1, &vboIndices);
	glGenBuffers(1, &vboNormals);
	glGenBuffers(1, &vboTexCoords);


	glBindVertexArray(vao); 
	GL_CHECK_ERRORS;
	{

	//передаем в шейдерную программу атрибут координат вершин
	glBindBuffer(GL_ARRAY_BUFFER, vboVertices); 
	GL_CHECK_ERRORS;
	glBufferData(GL_ARRAY_BUFFER, vertices_vec.size() * sizeof(GL_FLOAT), &vertices_vec[0], GL_STATIC_DRAW); 
	GL_CHECK_ERRORS;
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid*)0); 
	GL_CHECK_ERRORS;
	glEnableVertexAttribArray(0); 
	GL_CHECK_ERRORS;

	//передаем в шейдерную программу атрибут нормалей
	glBindBuffer(GL_ARRAY_BUFFER, vboNormals); 
	GL_CHECK_ERRORS;
	glBufferData(GL_ARRAY_BUFFER, normals_vec.size() * sizeof(GL_FLOAT), &normals_vec[0], GL_STATIC_DRAW); 
	GL_CHECK_ERRORS;
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid*)0); 
	GL_CHECK_ERRORS;
	glEnableVertexAttribArray(1); 
	GL_CHECK_ERRORS;

	//передаем в шейдерную программу атрибут текстурных координат
	glBindBuffer(GL_ARRAY_BUFFER, vboTexCoords); 
	GL_CHECK_ERRORS;
	glBufferData(GL_ARRAY_BUFFER, texcoords_vec.size() * sizeof(GL_FLOAT), &texcoords_vec[0], GL_STATIC_DRAW); 
	GL_CHECK_ERRORS;
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GL_FLOAT), (GLvoid*)0); 
	GL_CHECK_ERRORS;
	glEnableVertexAttribArray(2); 
	GL_CHECK_ERRORS;

	//передаем в шейдерную программу индексы
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndices); 
	GL_CHECK_ERRORS;
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_vec.size() * sizeof(GLuint), &indices_vec[0], GL_STATIC_DRAW); 
	GL_CHECK_ERRORS;

	glEnable(GL_PRIMITIVE_RESTART); 
	GL_CHECK_ERRORS;
	glPrimitiveRestartIndex(primRestart); 
	GL_CHECK_ERRORS;
	}
	//отвязываем vboIndices
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//отвязываем VAO
	glBindVertexArray(0);
	return numIndices;
}

int createWater(int size, GLuint &vao)
{
	
	GLfloat vertices[] = {
		1.0f, 0.0f,  1.0f, //0
	   -1.0f, 0.0f,  1.0f, //1
	   -1.0f, 0.0f, -1.0f, //2
		1.0f, 0.0f, -1.0f  //3
	};

	GLfloat texcoords[] = {
		1.0f, 1.0f, // Top Right
		1.0f, 0.0f, // Bottom Right
		0.0f, 0.0f, // Bottom Left
		0.0f, 1.0f  // Top Left
	};

	GLuint indices[] = {
		0, 1, 3,
		3, 2, 1
	};

	for(int i = 0; i < 12; i++) 
	{
		vertices[i] *= size;
	}
	float h = (max_h - min_h) / 10.0f;
	vertices[1] = h;
	vertices[4] = h;
	vertices[7] = h;
	vertices[10] = h;

	GLuint vboIndices, vboVertices, vboTexCoords;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vboVertices);
	glGenBuffers(1, &vboIndices);
	glGenBuffers(1, &vboTexCoords);

	glBindVertexArray(vao); 
	GL_CHECK_ERRORS;
	{
		glBindBuffer(GL_ARRAY_BUFFER, vboVertices); 
		GL_CHECK_ERRORS;
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); 
		GL_CHECK_ERRORS;
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid*)0); 
		GL_CHECK_ERRORS;
		glEnableVertexAttribArray(0); 
		GL_CHECK_ERRORS;

		glBindBuffer(GL_ARRAY_BUFFER, vboTexCoords); 
		GL_CHECK_ERRORS;
		glBufferData(GL_ARRAY_BUFFER, sizeof(texcoords), texcoords, GL_STATIC_DRAW); 
		GL_CHECK_ERRORS;
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GL_FLOAT), (GLvoid*)0); 
		GL_CHECK_ERRORS;
		glEnableVertexAttribArray(1); 
		GL_CHECK_ERRORS;
		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndices); 
		GL_CHECK_ERRORS;
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); 
		GL_CHECK_ERRORS;
		glEnable(GL_BLEND);
		GL_CHECK_ERRORS;
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		GL_CHECK_ERRORS;
		glDisable(GL_BLEND);
	}
	//отвязываем vboIndices
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//отвязываем VAO
	glBindVertexArray(0);

	return (int)(sizeof(indices) / sizeof(GLuint));
}

int skybox(int size, GLuint &vao)
{
	GLfloat vertices[] = {
			1.0f, -1.0f, -1.0f,	//0
			1.0f, -1.0f,  1.0f,	//1
		   -1.0f, -1.0f,  1.0f,	//2
		   -1.0f, -1.0f, -1.0f,	//3	
			1.0f,  1.0f, -1.0f, //4 
			1.0f,  1.0f,  1.0f, //5
		   -1.0f,  1.0f,  1.0f, //6 
		   -1.0f,  1.0f, -1.0f,	//7

	};
	GLuint indices[] = {
		0, 4, 5,
		5, 1, 0,
		0, 3, 4,
		4, 7, 3,
		3, 7, 6,
		6, 3, 2,
		2, 3, 0,
		0, 1, 2,
		2, 1, 5,
		5, 2, 6,
		6, 5, 7,
		7, 4, 5,
	};
	
	for(int i = 0; i < 24; i++) vertices[i] *= size;

	GLuint vboIndices, vboVertices;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vboVertices);
	glGenBuffers(1, &vboIndices);

	glBindVertexArray(vao); 
	GL_CHECK_ERRORS;
	{
		glBindBuffer(GL_ARRAY_BUFFER, vboVertices); 
		GL_CHECK_ERRORS;
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); 
		GL_CHECK_ERRORS;
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid*)0); 
		GL_CHECK_ERRORS;
		glEnableVertexAttribArray(0); 
		GL_CHECK_ERRORS;
		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndices); 
		GL_CHECK_ERRORS;
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); 
		GL_CHECK_ERRORS;
	}
	//отвязываем vboIndices
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//отвязываем VAO
	glBindVertexArray(0);

	return (int)(sizeof(indices) / sizeof(GLuint));
}


int initGL()
{
	int res = 0;

	//грузим функции opengl через glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize OpenGL context" << std::endl;
		return -1;
	}

	//выводим в консоль некоторую информацию о драйвере и контексте opengl
	std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

	std::cout << "Controls: "<< std::endl;
	std::cout << "\tPress left mose button to capture/release mouse cursor  "<< std::endl;
	std::cout << "\tPress spacebar to alternate between shaded wireframe and fill display modes" << std::endl;
	std::cout << "\tPress 1 to default lanscape" << std::endl;
	std::cout << "\tPress 2 to color normals mode" << std::endl;
	std::cout << "\tPress F to alternate between 'with FOG' and 'without FOG' display modes" << std::endl;
	std::cout << "\tPress ESC to exit" << std::endl;

	return 0;
}

int main(int argc, char** argv)
{
	if(!glfwInit())
	return -1;

	//запрашиваем контекст opengl версии 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); 
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); 


	GLFWwindow*  window = glfwCreateWindow(WIDTH, HEIGHT, "Task 4", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	
	glfwMakeContextCurrent(window); 

	//регистрируем коллбеки для обработки сообщений от пользователя - клавиатура, мышь..
	glfwSetKeyCallback        (window, OnKeyboardPressed);  
	glfwSetCursorPosCallback  (window, OnMouseMove); 
	glfwSetMouseButtonCallback(window, OnMouseButtonClicked);
	glfwSetScrollCallback     (window, OnMouseScroll);
	glfwSetInputMode          (window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); 

	if(initGL() != 0) 
		return -1;
	
	//Reset any OpenGL errors which could be present for some reason
	GLenum gl_error = glGetError();
	while (gl_error != GL_NO_ERROR)
		gl_error = glGetError();

	//создание шейдерной программы из двух файлов с исходниками шейдеров
	//используется класс-обертка ShaderProgram
	std::unordered_map<GLenum, std::string> shaders;
	//Основная поверхность
	shaders[GL_VERTEX_SHADER]   = "vertex.glsl";
	shaders[GL_FRAGMENT_SHADER] = "fragment.glsl";
	ShaderProgram program(shaders); 
	GL_CHECK_ERRORS;
	shaders.clear();
	//Небо
	shaders[GL_VERTEX_SHADER]   = "dummy_vertex.glsl";
	shaders[GL_FRAGMENT_SHADER] = "dummy_fragment.glsl";
	ShaderProgram sky_program(shaders); 
	GL_CHECK_ERRORS;
	shaders.clear(); 
	//Вода
	shaders[GL_VERTEX_SHADER] = "water_vertex.glsl";
	shaders[GL_FRAGMENT_SHADER] = "water_fragment.glsl";
	ShaderProgram water_program(shaders);
	GL_CHECK_ERRORS;
	shaders.clear();
	//1-ый самолет
	shaders[GL_VERTEX_SHADER] = "plane_model_vertex.glsl";
	shaders[GL_FRAGMENT_SHADER] = "plane_model_fragment.glsl";
	ShaderProgram plane_model_program(shaders);
	GL_CHECK_ERRORS;
	shaders.clear();
	//2-ой самолет
	shaders[GL_VERTEX_SHADER] = "sea_plane_model_vertex.glsl";
	shaders[GL_FRAGMENT_SHADER] = "sea_plane_model_fragment.glsl";
	ShaderProgram sea_plane_model_program(shaders);
	GL_CHECK_ERRORS;
	shaders.clear();
	//Лодка
	shaders[GL_VERTEX_SHADER] = "boat_vertex.glsl";
	shaders[GL_FRAGMENT_SHADER] = "boat_fragment.glsl";
	ShaderProgram boat_model_program(shaders);
	GL_CHECK_ERRORS;
	shaders.clear();
	//Crysis 
	shaders[GL_VERTEX_SHADER] = "man_vertex.glsl";
	shaders[GL_FRAGMENT_SHADER] = "man_fragment.glsl";
	ShaderProgram man_model_program(shaders);
	GL_CHECK_ERRORS;
	shaders.clear();
	//House
	shaders[GL_VERTEX_SHADER] = "house_vertex.glsl";
	shaders[GL_FRAGMENT_SHADER] = "house_fragment.glsl";
	ShaderProgram house_model_program(shaders);
	GL_CHECK_ERRORS;
	shaders.clear();
	//Flowers
	shaders[GL_VERTEX_SHADER] = "flower_vertex.glsl";
	shaders[GL_FRAGMENT_SHADER] = "flower_fragment.glsl";
	ShaderProgram flower_model_program(shaders);
	GL_CHECK_ERRORS;
	shaders.clear();
	//Rocks
	shaders[GL_VERTEX_SHADER] = "rocks_vertex.glsl";
	shaders[GL_FRAGMENT_SHADER] = "rocks_fragment.glsl";
	ShaderProgram rocks_model_program(shaders);
	GL_CHECK_ERRORS;
	shaders.clear();

	//Создаем и загружаем геометрию поверхности
	GLuint vaoTriStrip;
	int triStripIndices = createTriStrip(513, 513, 60, vaoTriStrip, 10);

	GLuint vaoSkyBox;
	int skyBox = skybox(500, vaoSkyBox);

	GLuint vaoWater;
	int waterIndices = createWater(30, vaoWater);
	//Создаем текстуры
	//Grass
	GLuint texture_grass;
	glGenTextures(1, &texture_grass);
	glBindTexture(GL_TEXTURE_2D, texture_grass);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height;
	unsigned char* image = SOIL_load_image("textures/grass.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	GLuint texture_sand;
	glGenTextures(1, &texture_sand);
	glBindTexture(GL_TEXTURE_2D, texture_sand);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	image = SOIL_load_image("textures/sand.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	GLuint texture_stone;
	glGenTextures(1, &texture_stone);
	glBindTexture(GL_TEXTURE_2D, texture_stone);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	image = SOIL_load_image("textures/mountain.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);


	GLuint texture_water;
	glGenTextures(1, &texture_water);
	glBindTexture(GL_TEXTURE_2D, texture_water);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	image = SOIL_load_image("textures/water.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	GLuint tex_cube;
	glGenTextures(1, &tex_cube);
	glBindTexture(GL_TEXTURE_CUBE_MAP, tex_cube);
	
	unsigned char* left = SOIL_load_image("textures/left.bmp", &width, &height, 0, SOIL_LOAD_RGB);
	unsigned char* right = SOIL_load_image("textures/right.bmp", &width, &height, 0, SOIL_LOAD_RGB);
	unsigned char* back = SOIL_load_image("textures/back.bmp", &width, &height, 0, SOIL_LOAD_RGB);
	unsigned char* front = SOIL_load_image("textures/front.bmp", &width, &height, 0, SOIL_LOAD_RGB);
	unsigned char* top = SOIL_load_image("textures/top.bmp", &width, &height, 0, SOIL_LOAD_RGB);
	unsigned char* bottom = SOIL_load_image("textures/bottom.bmp", &width, &height, 0, SOIL_LOAD_RGB);

	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, left);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, right);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, top);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, bottom);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, back);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, front);
	
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	
	SOIL_free_image_data(left);
	SOIL_free_image_data(right);
	SOIL_free_image_data(back);
	SOIL_free_image_data(front);
	SOIL_free_image_data(top);
	SOIL_free_image_data(bottom);

	glViewport(0, 0, WIDTH, HEIGHT);  
	GL_CHECK_ERRORS;
	glEnable(GL_DEPTH_TEST);  
	GL_CHECK_ERRORS;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	float2 min_max(min_h, max_h);
	Model ourManModel("models/nanosuit/nanosuit.obj"); 			//++
	Model ourRocksModel("models/rocks/rocks_01_model.obj"); 	//++
	Model ourBoatModel("models/boat/OldBoat.obj"); 				//++
	Model ourFlowerModel("models/flower/PrimroseP.obj"); 		//++
	Model ourHouseModel("models/house/model3343ds.3ds"); 		//++
	Model ourPlaneModel("models/plane/ToyPlane.obj"); 			//++
	Model ourSeaPlaneModel("models/sea_plane/SeaPlane.obj"); 	//++

	float3 pos = float3(0.0f, 0.0f, 0.0f); 	
	//цикл обработки сообщений и отрисовки сцены каждый кадр
	while (!glfwWindowShouldClose(window))
	{
		//считаем сколько времени прошло за кадр
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		//std::cout << "FPS: " <<  (int)(deltaTime * 100) << "\r";
		//std::cout << "X: " << camera.pos.x << " Y: " << camera.pos.y << " Z: " <<  camera.pos.z << "\r";
		glfwPollEvents();
		doCameraMovement(camera, deltaTime);

		//очищаем экран каждый кадр
		glClearColor(0.0f, 0.0f, 0.0f, 0.5f); 
		GL_CHECK_ERRORS;
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
		GL_CHECK_ERRORS;
		
		program.StartUseShader(); 
		GL_CHECK_ERRORS;
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture_grass);
		GL_CHECK_ERRORS;
		program.SetUniform("ourTexture0", 0);
		GL_CHECK_ERRORS;
		
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture_sand);
		GL_CHECK_ERRORS;
		program.SetUniform("ourTexture1", 1);
		GL_CHECK_ERRORS;

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, texture_stone);
		GL_CHECK_ERRORS;
		program.SetUniform("ourTexture2", 2);
		GL_CHECK_ERRORS;

		//обновляем матрицы камеры и проекции каждый кадр
		float4x4 view       = camera.GetViewMatrix();
		float4x4 projection = projectionMatrixTransposed(camera.zoom, float(WIDTH) / float(HEIGHT), 0.1f, 1000.0f);
		//модельная матрица, определяющая положение объекта в мировом пространстве
		float4x4 model; //начинаем с единичной матрицы
		
		int mode;
		if (def_vision) 
			mode = 1;
		else
			mode = 0;

		program.StartUseShader();

	//загружаем uniform-переменные в шейдерную программу (одинаковые для всех параллельно запускаемых копий шейдера)
		program.SetUniform("view", view);       
		GL_CHECK_ERRORS;
		program.SetUniform("projection", projection); 
		GL_CHECK_ERRORS;
		program.SetUniform("model", model);
		GL_CHECK_ERRORS;
		program.SetUniform("mode", mode);
		GL_CHECK_ERRORS;
		program.SetUniform("fog", fog);
		GL_CHECK_ERRORS;
		program.SetUniform("heights", min_max);
		GL_CHECK_ERRORS;
	//рисуем плоскость
		glBindVertexArray(vaoTriStrip);
		glDrawElements(GL_TRIANGLE_STRIP, triStripIndices, GL_UNSIGNED_INT, nullptr); 
		GL_CHECK_ERRORS;
		glBindVertexArray(0); 
		GL_CHECK_ERRORS;
		program.StopUseShader();
		
		sky_program.StartUseShader();
			sky_program.SetUniform("vPos", camera.pos);
			GL_CHECK_ERRORS;
			sky_program.SetUniform("view",       view);       
			GL_CHECK_ERRORS;
			sky_program.SetUniform("projection", projection); 
			GL_CHECK_ERRORS;
			sky_program.SetUniform("model",      model);
			GL_CHECK_ERRORS;
			sky_program.SetUniform("fog", fog);
			GL_CHECK_ERRORS;
			glBindTexture(GL_TEXTURE_CUBE_MAP, tex_cube);
			GL_CHECK_ERRORS;
			glBindVertexArray(vaoSkyBox);
			glDrawElements(GL_TRIANGLE_STRIP, skyBox, GL_UNSIGNED_INT, nullptr); 
			GL_CHECK_ERRORS;
			glBindVertexArray(0); 
			GL_CHECK_ERRORS;
		sky_program.StopUseShader();
		if (def_vision)
		{
		sea_plane_model_program.StartUseShader();
			sea_plane_model_program.SetUniform("view", view);       
			GL_CHECK_ERRORS;
			sea_plane_model_program.SetUniform("projection", projection); 
			GL_CHECK_ERRORS;
			float4x4 my_model_plane;
			my_model_plane = mul(my_model_plane, scale4x4(float3(0.1, 0.1, 0.1)));
			my_model_plane = mul(my_model_plane, rotate_Y_4x4(-3.14/2.0));
			sea_plane_model_program.SetUniform("model", my_model_plane);
			GL_CHECK_ERRORS;
			pos = float3(-63.0, 9.0, 122.0);
			sea_plane_model_program.SetUniform("position", pos);
			GL_CHECK_ERRORS;
			sea_plane_model_program.SetUniform("fog", fog);
			GL_CHECK_ERRORS;
			ourPlaneModel.Draw(sea_plane_model_program);
		sea_plane_model_program.StopUseShader();

		plane_model_program.StartUseShader();
			plane_model_program.SetUniform("view", view);       
			GL_CHECK_ERRORS;
			plane_model_program.SetUniform("projection", projection); 
			GL_CHECK_ERRORS;
			float4x4 my_model;
			my_model = mul(my_model, scale4x4(float3(0.1, 0.1, 0.1)));
			plane_model_program.SetUniform("model", my_model);
			GL_CHECK_ERRORS;
			pos = float3(120.0, 7.0, 130.0);
			plane_model_program.SetUniform("position", pos);
			GL_CHECK_ERRORS;
			plane_model_program.SetUniform("fog", fog);
			GL_CHECK_ERRORS;
			ourSeaPlaneModel.Draw(plane_model_program);
		plane_model_program.StopUseShader();

		boat_model_program.StartUseShader();
			boat_model_program.SetUniform("view", view);       
			GL_CHECK_ERRORS;
			boat_model_program.SetUniform("projection", projection); 
			GL_CHECK_ERRORS;
			float4x4 boat_model;
			boat_model = mul(boat_model, scale4x4(float3(0.05, 0.05, 0.05)));
			boat_model_program.SetUniform("model", boat_model);
			GL_CHECK_ERRORS;
			pos = float3(180.0, 21.2, 420.0);
			boat_model_program.SetUniform("position", pos);
			GL_CHECK_ERRORS;
			boat_model_program.SetUniform("fog", fog);
			GL_CHECK_ERRORS;
			ourBoatModel.Draw(boat_model_program);
		boat_model_program.StopUseShader();

		man_model_program.StartUseShader();
			man_model_program.SetUniform("view", view);       
			GL_CHECK_ERRORS;
			man_model_program.SetUniform("projection", projection); 
			GL_CHECK_ERRORS;
			float4x4 man_model;
			man_model = mul(man_model, scale4x4(float3(0.1, 0.1, 0.1)));
			man_model = mul(man_model, rotate_Y_4x4(3.14));
			man_model_program.SetUniform("model", man_model);
			GL_CHECK_ERRORS;
			pos = float3(-120.0, 15.5, -260.0);
			man_model_program.SetUniform("position", pos);
			GL_CHECK_ERRORS;
			man_model_program.SetUniform("fog", fog);
			GL_CHECK_ERRORS;
			ourManModel.Draw(man_model_program);
		man_model_program.StopUseShader();

		house_model_program.StartUseShader();
			house_model_program.SetUniform("view", view);       
			GL_CHECK_ERRORS;
			house_model_program.SetUniform("projection", projection); 
			GL_CHECK_ERRORS;
			float4x4 house_model;
			house_model = mul(house_model, scale4x4(float3(0.03, 0.03, 0.03)));
			house_model = mul(house_model, rotate_X_4x4(3.14/2.0));
			house_model_program.SetUniform("model", house_model);
			GL_CHECK_ERRORS;
			pos = float3(310.0, -800.0, 65.0);
			house_model_program.SetUniform("position", pos);
			GL_CHECK_ERRORS;
			house_model_program.SetUniform("fog", fog);
			GL_CHECK_ERRORS;
			ourHouseModel.Draw(house_model_program);
		house_model_program.StopUseShader();

		flower_model_program.StartUseShader();
			flower_model_program.SetUniform("view", view);       
			GL_CHECK_ERRORS;
			flower_model_program.SetUniform("projection", projection); 
			GL_CHECK_ERRORS;
			float4x4 flower_model;
			flower_model_program.SetUniform("model", flower_model);
			GL_CHECK_ERRORS;
			pos = float3(6.24, 1.11, 25.89);
			flower_model_program.SetUniform("position", pos);
			GL_CHECK_ERRORS;
			flower_model_program.SetUniform("fog", fog);
			GL_CHECK_ERRORS;
			ourFlowerModel.Draw(flower_model_program);
		flower_model_program.StopUseShader();

		flower_model_program.StartUseShader();
			flower_model_program.SetUniform("view", view);       
			GL_CHECK_ERRORS;
			flower_model_program.SetUniform("projection", projection); 
			GL_CHECK_ERRORS;
			flower_model_program.SetUniform("model", flower_model);
			GL_CHECK_ERRORS;
			pos = float3(15.36, 1, 13.89);
			flower_model_program.SetUniform("position", pos);
			GL_CHECK_ERRORS;
			flower_model_program.SetUniform("fog", fog);
			GL_CHECK_ERRORS;
			ourFlowerModel.Draw(flower_model_program);
		flower_model_program.StopUseShader();

		flower_model_program.StartUseShader();
			flower_model_program.SetUniform("view", view);       
			GL_CHECK_ERRORS;
			flower_model_program.SetUniform("projection", projection); 
			GL_CHECK_ERRORS;
			flower_model_program.SetUniform("model", flower_model);
			GL_CHECK_ERRORS;
			pos = float3(13.62, 1.22, 12.2);
			flower_model_program.SetUniform("position", pos);
			GL_CHECK_ERRORS;
			flower_model_program.SetUniform("fog", fog);
			GL_CHECK_ERRORS;
			ourFlowerModel.Draw(flower_model_program);
		flower_model_program.StopUseShader();

		flower_model_program.StartUseShader();
			flower_model_program.SetUniform("view", view);       
			GL_CHECK_ERRORS;
			flower_model_program.SetUniform("projection", projection); 
			GL_CHECK_ERRORS;
			flower_model_program.SetUniform("model", flower_model);
			GL_CHECK_ERRORS;
			pos = float3(2.22, 1.05, 12.77);
			flower_model_program.SetUniform("position", pos);
			GL_CHECK_ERRORS;
			flower_model_program.SetUniform("fog", fog);
			GL_CHECK_ERRORS;
			ourFlowerModel.Draw(flower_model_program);
		flower_model_program.StopUseShader();

		rocks_model_program.StartUseShader();
			rocks_model_program.SetUniform("view", view);       
			GL_CHECK_ERRORS;
			rocks_model_program.SetUniform("projection", projection); 
			GL_CHECK_ERRORS;
			float4x4 rock_model;
			rock_model = mul(rock_model, scale4x4(float3(0.01, 0.01, 0.01)));
			rocks_model_program.SetUniform("model", rock_model);
			GL_CHECK_ERRORS;
			pos = float3(0.0, 100.0, 0.0);
			rocks_model_program.SetUniform("position", pos);
			GL_CHECK_ERRORS;
			rocks_model_program.SetUniform("fog", fog);
			GL_CHECK_ERRORS;
			ourRocksModel.Draw(rocks_model_program);
		rocks_model_program.StopUseShader();

		rocks_model_program.StartUseShader();
			rocks_model_program.SetUniform("view", view);       
			GL_CHECK_ERRORS;
			rocks_model_program.SetUniform("projection", projection); 
			GL_CHECK_ERRORS;
			rocks_model_program.SetUniform("model", rock_model);
			GL_CHECK_ERRORS;
			pos = float3(1900.0, 20.0, 1300.0);
			rocks_model_program.SetUniform("position", pos);
			GL_CHECK_ERRORS;
			rocks_model_program.SetUniform("fog", fog);
			GL_CHECK_ERRORS;
			ourRocksModel.Draw(rocks_model_program);
		rocks_model_program.StopUseShader();
		}
		water_program.StartUseShader();
			GL_CHECK_ERRORS;
			glEnable(GL_BLEND);
			glActiveTexture(GL_TEXTURE3);
			glBindTexture(GL_TEXTURE_2D, texture_water);
			GL_CHECK_ERRORS;
			water_program.SetUniform("water_texture", 3);
			GL_CHECK_ERRORS;
			water_program.SetUniform("view",       view);       
			GL_CHECK_ERRORS;
			water_program.SetUniform("projection", projection); 
			GL_CHECK_ERRORS;
			water_program.SetUniform("model",      model);
			GL_CHECK_ERRORS;		
			water_program.SetUniform("fog", fog);
			GL_CHECK_ERRORS;
			glBindVertexArray(vaoWater);
			GL_CHECK_ERRORS;
			if (def_vision)
			glDrawElements(GL_TRIANGLE_STRIP, waterIndices, GL_UNSIGNED_INT, nullptr);
			GL_CHECK_ERRORS;
			glBindVertexArray(0);
			GL_CHECK_ERRORS;
			glDisable(GL_BLEND);
		water_program.StopUseShader();

		glfwSwapBuffers(window); 
	}

	//очищаем vao перед закрытием программы
	glDeleteVertexArrays(1, &vaoTriStrip);
	glDeleteVertexArrays(1, &vaoSkyBox);
	glDeleteVertexArrays(1, &vaoWater);

	//std::cout << std::endl;
	glfwTerminate();
	return 0;
}
