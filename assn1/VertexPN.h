#pragma once
#include "matrix4.h"
#include "geometrymaker.h"

struct VertexPN {
	Cvec3f p, n;
	VertexPN() {}
	VertexPN(float x, float y, float z, float nx, float ny, float nz) : p(x, y, z), n(nx, ny, nz) {}
	VertexPN& operator = (const GenericVertex& v) {
		p = v.pos;
		n = v.normal;
		return *this;
	}
};

struct Transform {
	Quat rotationX, rotationY, rotationZ;
	Cvec3 scale;
	Cvec3 position;
	Transform() : scale(1.0f, 1.0f, 1.0f) {
	}
	Matrix4 createMatrix();
};
struct Geometry {
	GLuint vertexBO;
	GLuint indexBO;
	int numIndices;
	void Draw(GLuint positionAttribute, GLuint normalAttribute) {
		// bind buffer objects and draw
		glBindBuffer(GL_ARRAY_BUFFER, vertexBO);
		glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPN), (void*)offsetof(VertexPN, p));
		glEnableVertexAttribArray(positionAttribute);

		glVertexAttribPointer(normalAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPN), (void*)offsetof(VertexPN, n));
		glEnableVertexAttribArray(normalAttribute);
		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBO);
		glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_SHORT, 0);

	}
};
struct Entity {
	Transform transform;
	Geometry geometry;
	Entity* child;
	Entity* parent;
	void adoption(Entity* theChild) {
		theChild->parent = this;
		child = theChild;
	}
	void disown(Entity* theChild) {
		theChild->parent = nullptr;
		child = nullptr;
	}
	void Draw(Matrix4 &eyeInverse, GLuint positionAttribute, GLuint normalAttribute,
		GLuint modelviewMatrixUniformLocation, GLuint normalMatrixUniformLocation, GLuint colorUniformLocation, float r, float g, float b) {

		// create modelview matrix
		glUniform3f(colorUniformLocation, r, g, b);
		Matrix4 modelViewMatrix = eyeInverse;
		GLfloat glmatrix[16];
		modelViewMatrix.writeToColumnMajorMatrix(glmatrix);
		
		// create normal matrix
		Matrix4 normalMatrix;
		normalMatrix = transpose(inv(modelViewMatrix));
		GLfloat glmatrixNormal[16];
		normalMatrix.writeToColumnMajorMatrix(glmatrixNormal);

		// set the model view and normal matrices to the uniforms locations
		glUniformMatrix4fv(modelviewMatrixUniformLocation, 1, false, glmatrix);
		glUniformMatrix4fv(normalMatrixUniformLocation, 1, false, glmatrixNormal);
		
		geometry.Draw(positionAttribute, normalAttribute);
	}
};
