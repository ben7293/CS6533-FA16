#pragma once
#include "matrix4.h"
#include "geometrymaker.h"

struct VertexPNTBTG {
	Cvec3f p, n, b, tg;
	Cvec2f t;
	VertexPNTBTG() {}
	VertexPNTBTG(float x, float y, float z, float nx, float ny, float nz) : p(x, y, z), n(nx, ny, nz) {}
	VertexPNTBTG& operator = (const GenericVertex& v) {
		p = v.pos;
		n = v.normal;
		t = v.tex;
		b = v.binormal;
		tg = v.tangent;
		return *this;
	}
};

struct Transform {
	Quat rotation;
	Cvec3 scale;
	Cvec3 position;
	Transform() : scale(1.0f, 1.0f, 1.0f) {}
	Matrix4 createMatrix() {
		Matrix4 matrix;
		matrix = matrix.makeTranslation(position) * quatToMatrix(rotation) * matrix.makeScale(scale);
		return matrix;
	}
};

struct Geometry {
	GLuint vertexBO;
	GLuint indexBO;
	int numIndices;
	void Draw(GLuint positionAttribute, GLuint texCoordAttribute, GLuint normalAttribute, GLuint binormalAttribute, GLuint tangentAttribute) {
		
		// bind buffer objects and draw
		glBindBuffer(GL_ARRAY_BUFFER, vertexBO);
		glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPNTBTG), (void*)offsetof(VertexPNTBTG, p));
		glEnableVertexAttribArray(positionAttribute);

		//glBindBuffer(GL_ARRAY_BUFFER, vertTexCoordVBO);
		
		glEnableVertexAttribArray(texCoordAttribute);
		glVertexAttribPointer(texCoordAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(VertexPNTBTG), (void*)offsetof(VertexPNTBTG, t));
		
		glEnableVertexAttribArray(normalAttribute);
		glVertexAttribPointer(normalAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPNTBTG), (void*)offsetof(VertexPNTBTG, n));

		glEnableVertexAttribArray(binormalAttribute);
		glVertexAttribPointer(binormalAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPNTBTG), (void*)offsetof(VertexPNTBTG, b));

		glEnableVertexAttribArray(tangentAttribute);
		glVertexAttribPointer(tangentAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPNTBTG), (void*)offsetof(VertexPNTBTG, tg));

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBO);
		glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_SHORT, 0);

		glDisableVertexAttribArray(positionAttribute);
		glDisableVertexAttribArray(texCoordAttribute);
		glDisableVertexAttribArray(normalAttribute);
		glDisableVertexAttribArray(binormalAttribute);
		glDisableVertexAttribArray(tangentAttribute);


	}
};

struct Entity {
	Transform transform;
	Geometry geometry;
	Entity* parent;

	void Draw(Matrix4 &eyeInverse, GLuint positionAttribute, GLuint texCoordAttribute, GLuint normalAttribute, GLuint binormalAttribute, GLuint tangentAttribute, GLuint modelviewMatrixUniformLocation, GLuint normalMatrixUniformLocation) {

		// create modelview matrix
		Matrix4 modelViewMatrix;
		if (parent == nullptr) {
			modelViewMatrix = eyeInverse * transform.createMatrix();
		}
		else {
			Matrix4 modelViewMatrix = eyeInverse;
		}
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
		
		geometry.Draw(positionAttribute, texCoordAttribute, normalAttribute, binormalAttribute, tangentAttribute);
	}
};
