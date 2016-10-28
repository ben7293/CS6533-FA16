/*

Benson Tsai
CS-GY 6533
Prof. Ivan Safrin
Assignment 2
10/25/2016

This is a program with 3D rendering of three cube objects rotating in a heirarchy. 
The cubes are in increasing sizes.

*/


#include "glsupport.h"
#include <GL\freeglut.h>
#include "matrix4.h"
#include "quat.h"
#include "geometrymaker.h"
#include "VertexPN.h"


GLuint program; 
GLuint vertPositionVBO;
GLuint vertTexCoordVBO;
GLuint colorVBO;
GLuint normalVBO;
GLuint positionAttribute;
GLuint colorAttribute;
GLuint texCoordAttribute;
GLuint normalAttribute;
GLuint texture;
GLuint positionUniform;
GLuint timeUniform;
GLuint modelviewMatrixUniformLocation;
GLuint projectionMatrixUniformLocation;
GLuint colorUniformLocation;
GLuint normalUniformLocation;
 
float textureOffset = 0.0;

Entity sCube;
int ibLenS;
int vbLenS;

Entity mCube;
int ibLenM;
int vbLenM;

Entity lCube;
int ibLenL;
int vbLenL;
 
void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	int timeSinceStart = glutGet(GLUT_ELAPSED_TIME);
	glUniform1f(timeUniform, (float)timeSinceStart / 1000.0f);
	
	// Create eye matrix
	Matrix4 eyeMatrix;
	eyeMatrix = eyeMatrix.makeTranslation(Cvec3(0.00, 0.0, 3.00));

	// Create projection matrix
	Matrix4 projectionMatrix;
	projectionMatrix = projectionMatrix.makeProjection(100.0, 1.0, -0.1, -100.0);

	// Define modelview matrix for rotation
	Quat q1 = Quat::makeYRotation(0.001 * timeSinceStart * 70.0f);
	Quat q2 = Quat::makeZRotation(0.001 * timeSinceStart * 20.0f);
	Quat combined = q1 * q2;
	Matrix4 rotationMatrix = quatToMatrix(combined);
	Matrix4 modelViewMatrix = inv(eyeMatrix) * rotationMatrix;

	Quat q3 = Quat::makeXRotation(0.001 * timeSinceStart * 45.0f);
	Matrix4 modelViewMatrix2 = inv(eyeMatrix) * quatToMatrix(q3);

	GLfloat glmatrixProjection[16];
	projectionMatrix.writeToColumnMajorMatrix(glmatrixProjection);
	glUniformMatrix4fv(projectionMatrixUniformLocation, 1, false, glmatrixProjection);

	// Draw cubes
	sCube.Draw(modelViewMatrix, positionAttribute, normalAttribute, modelviewMatrixUniformLocation, normalUniformLocation, colorUniformLocation, 0.0, 0.0, 1.0);
	mCube.Draw(modelViewMatrix, positionAttribute, normalAttribute, modelviewMatrixUniformLocation, normalUniformLocation, colorUniformLocation, 1.0, 0.92, 0.0);
	lCube.Draw(modelViewMatrix, positionAttribute, normalAttribute, modelviewMatrixUniformLocation, normalUniformLocation, colorUniformLocation, 1.0, 0.0, 0.0);
	
	glBindBuffer(GL_ARRAY_BUFFER, vertPositionVBO);
	glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(positionAttribute);

	glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
	glVertexAttribPointer(colorAttribute, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(colorAttribute);

	glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
	glVertexAttribPointer(normalAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(normalAttribute);

	glDisableVertexAttribArray(positionAttribute);
	glDisableVertexAttribArray(colorAttribute);
	glDisableVertexAttribArray(normalAttribute);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0.0, 0.0, 0.0, 1.0);

    glutSwapBuffers();
}


void init() {
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glReadBuffer(GL_BACK);

	program = glCreateProgram();
	readAndCompileShader(program, "vertex_textured.glsl", "fragment_textured.glsl");
	glUseProgram(program);

	positionAttribute = glGetAttribLocation(program, "position");
	colorAttribute = glGetAttribLocation(program, "color");
	timeUniform = glGetUniformLocation(program, "time");
	normalAttribute = glGetAttribLocation(program, "normal");

	normalUniformLocation = glGetUniformLocation(program, "normalMatrix");
	colorUniformLocation = glGetUniformLocation(program, "uColor");
	modelviewMatrixUniformLocation = glGetUniformLocation(program, "modelViewMatrix");
	projectionMatrixUniformLocation = glGetUniformLocation(program, "projectionMatrix");

	lCube.adoption(&mCube);
	mCube.adoption(&sCube);


	// Small cube
	getCubeVbIbLen(vbLenS, ibLenS);
	sCube.geometry.indexBO = ibLenS;
	sCube.geometry.vertexBO = vbLenS;
	sCube.geometry.numIndices = ibLenS;

	std::vector<VertexPN> vtxS(vbLenS);
	std::vector<unsigned short> idxS(ibLenS);

	makeCube(1, vtxS.begin(), idxS.begin());

	glGenBuffers(1, &sCube.geometry.vertexBO);
	glBindBuffer(GL_ARRAY_BUFFER, sCube.geometry.vertexBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexPN) * vtxS.size(), vtxS.data(), GL_STATIC_DRAW);
	glGenBuffers(1, &sCube.geometry.indexBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sCube.geometry.indexBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * idxS.size(), idxS.data(), GL_STATIC_DRAW);


	// Medium cube
	getCubeVbIbLen(vbLenM, ibLenM);
	mCube.geometry.indexBO = ibLenM;
	mCube.geometry.vertexBO = vbLenM;
	mCube.geometry.numIndices = ibLenM;

	std::vector<VertexPN> vtxM(vbLenM);
	std::vector<unsigned short> idxM(ibLenM);

	makeCube(2, vtxM.begin(), idxM.begin());

	glGenBuffers(1, &mCube.geometry.vertexBO);
	glBindBuffer(GL_ARRAY_BUFFER, mCube.geometry.vertexBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexPN) * vtxM.size(), vtxM.data(), GL_STATIC_DRAW);
	glGenBuffers(1, &mCube.geometry.indexBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mCube.geometry.indexBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * idxM.size(), idxM.data(), GL_STATIC_DRAW);

	// Large cube
	getCubeVbIbLen(vbLenL, ibLenL);
	lCube.geometry.indexBO = ibLenL;
	lCube.geometry.vertexBO = vbLenL;
	lCube.geometry.numIndices = ibLenL;

	std::vector<VertexPN> vtxL(vbLenL);
	std::vector<unsigned short> idxL(ibLenL);

	makeCube(3, vtxL.begin(), idxL.begin());

	glGenBuffers(1, &lCube.geometry.vertexBO);
	glBindBuffer(GL_ARRAY_BUFFER, lCube.geometry.vertexBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexPN) * vtxL.size(), vtxL.data(), GL_STATIC_DRAW);
	glGenBuffers(1, &lCube.geometry.indexBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lCube.geometry.indexBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * idxL.size(), idxL.data(), GL_STATIC_DRAW);


	glGenBuffers(1, &normalVBO);
	glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
	GLfloat cubeNormals[] = {
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		0.0f, 0.0f,-1.0f,
		0.0f, 0.0f,-1.0f,
		0.0f, 0.0f,-1.0f,
		0.0f,-1.0f,0.0f,
		0.0f,-1.0f,0.0f,
		0.0f,-1.0f,0.0f,
		0.0f, 0.0f,-1.0f,
		0.0f, 0.0f,-1.0f,
		0.0f, 0.0f,-1.0f,
		-1.0f, 0.0f,0.0f,
		-1.0f, 0.0f,0.0f,
		-1.0f, 0.0f,0.0f,
		0.0f,-1.0f, 0.0f,
		0.0f,-1.0f, 0.0f,
		0.0f,-1.0f,0.0f,
		0.0f, 0.0f, 1.0f,
		0.0f,0.0f, 1.0f,
		0.0f,0.0f, 1.0f,
		1.0f, 0.0f, 0.0f,
		1.0f,0.0f,0.0f,
		1.0f, 0.0f,0.0f,
		1.0f,0.0f,0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f,0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f,0.0f,
		0.0f, 1.0f,0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f
	};

	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeNormals), cubeNormals, GL_STATIC_DRAW);

	

} 


void reshape(int w, int h) {
    glViewport(0, 0, w, h);
}

void idle(void) {
    glutPostRedisplay();
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutCreateWindow("CS-6533");

    glewInit();
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);

    
    init();
    glutMainLoop();
    return 0;
}