#define TINYOBJLOADER_IMPLEMENTATION

/*
Benson Tsai
CS-GY 6533
Prof. Ivan Safrin
Assignment 3
11/22/2016

This is a program with 3D rendering of two monk models with three lighting sources.

*/


#include "glsupport.h"
#include <GL/freeglut.h>
#include "tiny_obj_loader.h"
#include "quat.h"
#include "matrix4.h"
#include "cvec.h"
#include "VertexPNTBTG.h"

GLuint program;

GLuint vertPositionVBO;
GLuint vertColorVBO;
GLuint vertTexCoordVBO;

GLuint positionAttribute;
GLuint colorAttribute;
GLuint texCoordAttribute;
GLuint normalAttribute;
GLuint binormalAttribute;
GLuint tangentAttribute;

GLuint diffuseTexture;
GLuint specularTexture;
GLuint normalTexture;

GLuint timeUniform;
GLuint positionUniform;
GLuint modelviewMatrixUniformLocation;
GLuint projectionMatrixUniformLocation;
GLuint normalMatrixUniformLocation;

GLuint normalTextureUniformLocation;
GLuint diffuseTextureUniformLocation;
GLuint specularTextureUniformLocation;

GLuint light1PositionUniformLocation;
GLuint light2PositionUniformLocation;
GLuint light3PositionUniformLocation;
GLuint light1DirectionUniformLocation;
GLuint light2DirectionUniformLocation;
GLuint light3DirectionUniformLocation;
GLuint light1ColorUniformLocation;
GLuint light2ColorUniformLocation;
GLuint light3ColorUniformLocation;
GLuint light1SpecularColorUniformLocation;
GLuint light2SpecularColorUniformLocation;
GLuint light3SpecularColorUniformLocation;

Entity object1;
Entity object2;
float textureOffset = 0.0;

std::vector<VertexPNTBTG> model1MeshVertices, model2MeshVertices;
std::vector<unsigned short> model1MeshIndices, model2MeshIndices;



void loadObjFile(const std::string &fileName, std::vector<VertexPNTBTG> &outVertices, std::vector<unsigned short> &outIndices) {
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string err;
	bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, fileName.c_str(), NULL, true);
	if (ret) {
		for (int i = 0; i < shapes.size(); i++) {
			for (int j = 0; j < shapes[i].mesh.indices.size(); j++) {
				unsigned int vertexOffset = shapes[i].mesh.indices[j].vertex_index * 3;
				unsigned int normalOffset = shapes[i].mesh.indices[j].normal_index * 3;
				unsigned int texOffset = shapes[i].mesh.indices[j].texcoord_index * 2;
				VertexPNTBTG v;
				v.p[0] = attrib.vertices[vertexOffset];
				v.p[1] = attrib.vertices[vertexOffset + 1];
				v.p[2] = attrib.vertices[vertexOffset + 2];
				v.n[0] = attrib.normals[normalOffset];
				v.n[1] = attrib.normals[normalOffset + 1];
				v.n[2] = attrib.normals[normalOffset + 2];
				v.t[0] = attrib.texcoords[texOffset];
				v.t[1] = 1.0 - attrib.texcoords[texOffset + 1];
				outVertices.push_back(v);
				outIndices.push_back(outVertices.size() - 1);
			}
		}
	}
	else {
		std::cout << err << std::endl;
		assert(false);
	}
}

void calculateFaceTangent(const Cvec3f &v1, const Cvec3f &v2, const Cvec3f &v3, const Cvec2f &texCoord1, const Cvec2f &texCoord2, const Cvec2f &texCoord3, Cvec3f &tangent, Cvec3f &binormal) {
	Cvec3f side0 = v1 - v2;
	Cvec3f side1 = v3 - v1;
	Cvec3f normal = cross(side1, side0);
	normalize(normal);
	float deltaV0 = texCoord1[1] - texCoord2[1];
	float deltaV1 = texCoord3[1] - texCoord1[1];
	tangent = side0 * deltaV1 - side1 * deltaV0;
	normalize(tangent);
	float deltaU0 = texCoord1[0] - texCoord2[0];
	float deltaU1 = texCoord3[0] - texCoord1[0];
	binormal = side0 * deltaU1 - side1 * deltaU0;
	normalize(binormal);
	Cvec3f tangentCross = cross(tangent, binormal);
	if (dot(tangentCross, normal) < 0.0f) {
		tangent = tangent * -1;
	}
}



//void keyboard(unsigned char key, int x, int y) {
//	switch (key) {
//	case 'a':
//		textureOffset += 0.02;
//		break;
//	case 'd':
//		textureOffset -= 0.02;
//		break;
//	}
//}
//
//void mouse(int button, int state, int x, int y) {
//	float newPositionX = (float)x / 250.0f - 1.0f;
//	float newPositionY = (1.0 - (float)y / 250.0);
//	glUniform2f(positionUniform, newPositionX, newPositionY);
//}
//
//void mouseMove(int x, int y) {
//	float newPositionX = (float)x / 250.0f - 1.0f;
//	float newPositionY = (1.0 - (float)y / 250.0);
//	glUniform2f(positionUniform, newPositionX, newPositionY);
//}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
	glUseProgram(program);

	int timeSinceStart = glutGet(GLUT_ELAPSED_TIME);
	glUniform1f(timeUniform, (float)timeSinceStart / 1000.0f);

	// Eye matrix
	Matrix4 eyeMatrix;
	eyeMatrix = eyeMatrix.makeTranslation(Cvec3(-0.5, 4.0, 10.0));
	eyeMatrix = eyeMatrix * eyeMatrix.makeXRotation(5.0);

	// Projection Matrix
	Matrix4 projectionMatrix;
	projectionMatrix = projectionMatrix.makeProjection(90.0, 1.0, -0.1, -100.0);

	GLfloat glmatrixProjection[16];
	projectionMatrix.writeToColumnMajorMatrix(glmatrixProjection);
	glUniformMatrix4fv(projectionMatrixUniformLocation, 1, false, glmatrixProjection);

	// Draw the monks
	glUniform1i(diffuseTextureUniformLocation, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, object1.diffuseTexture);
	glUniform1i(specularTextureUniformLocation, 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, object1.specularTexture);
	glUniform1i(normalTextureUniformLocation, 2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, object1.normalTexture);
	glUniform1i(diffuseTextureUniformLocation, 3);
	glActiveTexture(GL_TEXTURE3);
	object1.transform.rotation = Quat::makeYRotation(0.001 * timeSinceStart * 40.0f);
	object1.transform.position = Cvec3(-5.0, 0.0, 0.0);
	object1.Draw(inv(eyeMatrix), positionAttribute, texCoordAttribute, normalAttribute, binormalAttribute, tangentAttribute, modelviewMatrixUniformLocation, normalMatrixUniformLocation);

	glBindTexture(GL_TEXTURE_2D, object2.diffuseTexture);
	glUniform1i(specularTextureUniformLocation, 4);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, object2.specularTexture);
	glUniform1i(normalTextureUniformLocation, 5);
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, object2.normalTexture);
	object2.transform.rotation = Quat::makeYRotation(0.001 * timeSinceStart * -40.0f);
	object2.transform.position = Cvec3(5.0, 0.0, 0.0);
	object2.Draw(inv(eyeMatrix), positionAttribute, texCoordAttribute, normalAttribute, binormalAttribute, tangentAttribute, modelviewMatrixUniformLocation, normalMatrixUniformLocation);

	glutSwapBuffers();
}

void init() {

	program = glCreateProgram();

	glClearDepth(0.0f);
	//glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_GREATER);
	glReadBuffer(GL_BACK);
	
	readAndCompileShader(program, "vertex_textured.glsl", "fragment_textured.glsl");

	positionAttribute = glGetAttribLocation(program, "position");
	texCoordAttribute = glGetAttribLocation(program, "texCoord");
	//colorAttribute = glGetAttribLocation(program, "color");
	timeUniform = glGetUniformLocation(program, "time");
	normalAttribute = glGetAttribLocation(program, "normal");
	binormalAttribute = glGetAttribLocation(program, "binormal");
	tangentAttribute = glGetAttribLocation(program, "tangent");

	modelviewMatrixUniformLocation = glGetUniformLocation(program, "modelViewMatrix");
	projectionMatrixUniformLocation = glGetUniformLocation(program, "projectionMatrix");
	normalMatrixUniformLocation = glGetUniformLocation(program, "normalMatrix");
	diffuseTextureUniformLocation = glGetUniformLocation(program, "diffuseTexture");
	specularTextureUniformLocation = glGetUniformLocation(program, "specularTexture");
	normalTextureUniformLocation = glGetUniformLocation(program, "normalTexture");

	loadObjFile("model/Monk_Giveaway_Fixed.obj", model1MeshVertices, model1MeshIndices);
	loadObjFile("model/Monk_Giveaway_Fixed.obj", model2MeshVertices, model2MeshIndices);

	// Monk 1
	object1.diffuseTexture = loadGLTexture("model/Monk_D.tga");
	object1.specularTexture = loadGLTexture("model/Monk_S.tga");
	object1.normalTexture = loadGLTexture("model/Monk_N.tga");


	// Monk 1
	for (int i = 0; i < model1MeshVertices.size(); i += 3) {
		Cvec3f tangent;
		Cvec3f binormal;
		calculateFaceTangent(model1MeshVertices[i].p, model1MeshVertices[i + 1].p, model1MeshVertices[i + 2].p, model1MeshVertices[i].t, model1MeshVertices[i + 1].t, model1MeshVertices[i + 2].t, tangent, binormal);
		
		model1MeshVertices[i].tg = tangent;
		model1MeshVertices[i + 1].tg = tangent;
		model1MeshVertices[i + 2].tg = tangent;

		model1MeshVertices[i].b = binormal;
		model1MeshVertices[i + 1].b = binormal;
		model1MeshVertices[i + 2].b = binormal;
	}

	// Monk 1
	object1.geometry.numIndices = model1MeshIndices.size();
	object1.geometry.vertexBO = model1MeshVertices.size();
	object1.geometry.indexBO = model1MeshIndices.size();

	// Monk 1
	glGenBuffers(1, &object1.geometry.vertexBO);
	glBindBuffer(GL_ARRAY_BUFFER, object1.geometry.vertexBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexPNTBTG) * model1MeshVertices.size(), model1MeshVertices.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &object1.geometry.indexBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, object1.geometry.indexBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * model1MeshIndices.size(), model1MeshIndices.data(), GL_STATIC_DRAW);

	// Monk 2
	object2.diffuseTexture = loadGLTexture("model/Monk_D.tga");
	object2.specularTexture = loadGLTexture("model/Monk_S.tga");
	object2.normalTexture = loadGLTexture("model/Monk_N.tga");

	// Monk 2
	for (int i = 0; i < model2MeshVertices.size(); i += 3) {
		Cvec3f tangent;
		Cvec3f binormal;
		calculateFaceTangent(model2MeshVertices[i].p, model2MeshVertices[i + 1].p, model2MeshVertices[i + 2].p,	model2MeshVertices[i].t, model2MeshVertices[i + 1].t, model2MeshVertices[i + 2].t, tangent, binormal);
		
		model2MeshVertices[i].tg = tangent;
		model2MeshVertices[i + 1].tg = tangent;
		model2MeshVertices[i + 2].tg = tangent;

		model2MeshVertices[i].b = binormal;
		model2MeshVertices[i + 1].b = binormal;
		model2MeshVertices[i + 2].b = binormal;
	}

	// Monk 2
	object2.geometry.numIndices = model2MeshIndices.size();
	object2.geometry.vertexBO = model2MeshVertices.size();
	object2.geometry.indexBO = model2MeshIndices.size();

	// Monk 2
	glGenBuffers(1, &object2.geometry.vertexBO);
	glBindBuffer(GL_ARRAY_BUFFER, object2.geometry.vertexBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexPNTBTG) * model2MeshVertices.size(), model2MeshVertices.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &object2.geometry.indexBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, object2.geometry.indexBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * model2MeshIndices.size(), model2MeshIndices.data(), GL_STATIC_DRAW);

	// Lighting
	light1PositionUniformLocation = glGetUniformLocation(program, "lights[0].lightPosition");
	light1DirectionUniformLocation = glGetUniformLocation(program, "lights[0].lightDirection"); 
	light1ColorUniformLocation = glGetUniformLocation(program, "lights[0].lightColor");
	light1SpecularColorUniformLocation = glGetUniformLocation(program, "lights[0].specularLightColor");

	light2PositionUniformLocation = glGetUniformLocation(program, "lights[1].lightPosition");
	light2DirectionUniformLocation = glGetUniformLocation(program, "lights[1].lightDirection"); 
	light2ColorUniformLocation = glGetUniformLocation(program, "lights[1].lightColor");
	light2SpecularColorUniformLocation = glGetUniformLocation(program, "lights[1].specularLightColor");

	light3PositionUniformLocation = glGetUniformLocation(program, "lights[2].lightPosition");
	light3DirectionUniformLocation = glGetUniformLocation(program, "lights[2].lightDirection"); 
	light3ColorUniformLocation = glGetUniformLocation(program, "lights[2].lightColor");
	light3SpecularColorUniformLocation = glGetUniformLocation(program, "lights[2].specularLightColor");

	glUniform3f(light1PositionUniformLocation, 0.0, 2.0, 0.0);
	glUniform3f(light1DirectionUniformLocation, 0.0, 2.0, 0.0);
	glUniform3f(light1ColorUniformLocation, 1.0, 1.0, 1.0);
	glUniform3f(light1SpecularColorUniformLocation, 1.0, 1.0, 1.0);

	glUniform3f(light2PositionUniformLocation, 0.0, 2.0, 0.0);
	glUniform3f(light2DirectionUniformLocation, 0.0, 2.0, 0.0);
	glUniform3f(light2ColorUniformLocation, 1.0, 1.0, 1.0);
	glUniform3f(light2SpecularColorUniformLocation, 1.0, 1.0, 1.0);

	glUniform3f(light3PositionUniformLocation, 0.0, 2.0, 0.0);
	glUniform3f(light3DirectionUniformLocation, 0.0, 2.0, 0.0);
	glUniform3f(light3ColorUniformLocation, 1.0, 1.0, 1.0);
	glUniform3f(light3SpecularColorUniformLocation, 1.0, 1.0, 1.0);


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

	//glutKeyboardFunc(keyboard);
	//glutMouseFunc(mouse);
	//glutMotionFunc(mouseMove);
    
    init();
    glutMainLoop();
    return 0;
}