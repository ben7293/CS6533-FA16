#define TINYOBJLOADER_IMPLEMENTATION
#include "glsupport.h"
#include <GL/freeglut.h>
#include "tiny_obj_loader.h"
#include "quat.h"
#include "VertexPN.h"

GLuint program;

GLuint vertPositionVBO;
//GLuint vertColorVBO;
GLuint vertTexCoordVBO;
GLuint positionAttribute;
GLuint colorAttribute;
GLuint texCoordAttribute;
GLuint emojiTexture;
GLuint timeUniform;
GLuint positionUniform;

float textureOffset = 0.0;

std::vector<VertexPN> meshVertices;
std::vector<unsigned short> meshIndices;




void loadObjFile(const std::string &fileName, std::vector<VertexPN> &outVertices, std::vector<unsigned
	short> &outIndices) {
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string err;
	bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, fileName.c_str(), NULL, true);
	if (ret) {
		for (int i = 0; i < attrib.vertices.size(); i += 3) {
			VertexPN v;
			v.p[0] = attrib.vertices[i];
			v.p[1] = attrib.vertices[i + 1];
			v.p[2] = attrib.vertices[i + 2];
			v.n[0] = attrib.normals[i];
			v.n[1] = attrib.normals[i + 1];
			v.n[2] = attrib.normals[i + 2];
			outVertices.push_back(v);
		}
		for (int i = 0; i < shapes.size(); i++) {
			for (int j = 0; j < shapes[i].mesh.indices.size(); j++) {
				outIndices.push_back(shapes[i].mesh.indices[j].vertex_index);
			}
		}
	}
	else {
		std::cout << err << std::endl;
		assert(false);
	}
}


void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'a':
		textureOffset += 0.02;
		break;
	case 'd':
		textureOffset -= 0.02;
		break;
	}
}

void mouse(int button, int state, int x, int y) {
	float newPositionX = (float)x / 250.0f - 1.0f;
	float newPositionY = (1.0 - (float)y / 250.0);
	glUniform2f(positionUniform, newPositionX, newPositionY);
}

void mouseMove(int x, int y) {
	float newPositionX = (float)x / 250.0f - 1.0f;
	float newPositionY = (1.0 - (float)y / 250.0);
	glUniform2f(positionUniform, newPositionX, newPositionY);
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    
	//glUseProgram(program);

	int timeSinceStart = glutGet(GLUT_ELAPSED_TIME);
	glUniform1f(timeUniform, textureOffset);

	glBindBuffer(GL_ARRAY_BUFFER, vertPositionVBO);
	glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(positionAttribute);

	//glBindBuffer(GL_ARRAY_BUFFER, vertColorVBO);
	//glVertexAttribPointer(colorAttribute, 4, GL_FLOAT, GL_FALSE, 0, 0);
	//glEnableVertexAttribArray(colorAttribute);

	glBindBuffer(GL_ARRAY_BUFFER, vertTexCoordVBO);
	glVertexAttribPointer(texCoordAttribute, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(texCoordAttribute);
	glBindTexture(GL_TEXTURE_2D, emojiTexture);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0.5, 0.5, 0.5, 1.0); // set the clear color

	glUniform2f(positionUniform, -0.5, 0.0);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glUniform2f(positionUniform, 0.5, 0.0);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(positionAttribute);
	//glDisableVertexAttribArray(colorAttribute);
	glDisableVertexAttribArray(texCoordAttribute);

	
	//glClearDepth(0.0f);
	//glCullFace(GL_BACK);
	//glEnable(GL_CULL_FACE);
	//glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_GREATER);
	//glReadBuffer(GL_BACK);
	
	glutSwapBuffers();
}

void init() {

	program = glCreateProgram();
	readAndCompileShader(program, "vertex_textured.glsl", "fragment_textured.glsl");

	glUseProgram(program);
	positionAttribute = glGetAttribLocation(program, "position");
	texCoordAttribute = glGetAttribLocation(program, "texCoord");
	//colorAttribute = glGetAttribLocation(program, "color");
	timeUniform = glGetUniformLocation(program, "time");

	glGenBuffers(1, &vertPositionVBO);
	glBindBuffer(GL_ARRAY_BUFFER, vertPositionVBO);
	GLfloat sqVerts[12] = {
		-0.5f, -0.5f,
		0.5f, 0.5f,
		0.5f, -0.5f,
		-0.5f, -0.5f,
		-0.5f, 0.5f,
		0.5f, 0.5f
	};
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), sqVerts, GL_STATIC_DRAW);


	//For color in square
	//glGenBuffers(1, &vertColorVBO);
	//glBindBuffer(GL_ARRAY_BUFFER, vertColorVBO);
	//GLfloat sqColors[24] = {
	//	1.0f, 0.0f, 0.0f, 1.0f,
	//	0.0f, 1.0f, 1.0f, 1.0f,
	//	0.0f, 0.0f, 1.0f, 1.0f,
	//	1.0f, 0.0f, 0.0f, 1.0f,
	//	0.0f, 1.0f, 0.0f, 1.0f,
	//	0.0f, 1.0f, 1.0f, 1.0f
	//};
	//glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(GLfloat), sqColors, GL_STATIC_DRAW);

	//For image texture coordinates
	glGenBuffers(1, &vertTexCoordVBO);
	glBindBuffer(GL_ARRAY_BUFFER, vertTexCoordVBO);
	GLfloat sqTexCoords[12] = {
		0.0f, 1.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,

		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,
	};
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), sqTexCoords, GL_STATIC_DRAW);

	emojiTexture = loadGLTexture("path5670.png");

	//loadObjFile("model/Monk_Giveaway.obj", meshVertices, meshIndices);



}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
}

void idle(void) {
    glutPostRedisplay();
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutCreateWindow("CS-6533");

    glewInit();
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);

	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutMotionFunc(mouseMove);
    
    init();
    glutMainLoop();
    return 0;
}