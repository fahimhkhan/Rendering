/*
 * SceneShader.cpp
 *
 *  Created on: Nov 17, 2015
 *      Author: acarocha
 */

#include "SceneShader.h"
static float PI = 3.14159265359;


SceneShader::SceneShader(): Shader()
{
	_programPlane = 0;
	_programMesh = 0;
	_planeVertexArray = -1;
	_planeVertexArray = -1;
	_mvUniform = -1;
	_projUniform = -1;
	_zTranslation = 1.0;
	_aspectRatio = 1.0;
	_xRot = 0.0;
	_yRot = 0.0;
	lightPosition = glm::vec3(0.5, 0.5, 0.5);
        ObjectColor = glm::vec3(0.75, 0.75, 0.75);
        k_blue = glm::vec3(0.0, 0.0, 0.6);
        k_yellow = glm::vec3(0.6, 0.6, 0.0);
        alpha = 0.45;
        beta = 0.45;
        scale = 2.0;
        ModelID = 1;
}


void SceneShader::readMesh( std::string filename )
{
	_mesh = trimesh::TriMesh::read(filename);

	_mesh->need_bbox();
	_mesh->need_faces();
	_mesh->need_normals();
	_mesh->need_bsphere();

	for(unsigned int i = 0; i < _mesh->faces.size(); i++)
	{
   	  _triangleIndices.push_back(_mesh->faces[i][0]);
	  _triangleIndices.push_back(_mesh->faces[i][1]);
	  _triangleIndices.push_back(_mesh->faces[i][2]);
	}
}



void SceneShader::createVertexBuffer()
{
	//create plane geometry
	static const GLfloat quadData[] =
	{
                        -1.0f, 0.0f, -1.0f,
                        -1.0f, 0.0f, 1.0f,
                        1.0f, 0.0f, -1.0f,
                        1.0f, 0.0f, 1.0f,
	};

	//passing model attributes to the GPU
	//plane
	glGenVertexArrays(1, &_planeVertexArray);
	glBindVertexArray(_planeVertexArray);

	glGenBuffers(1, &_planeVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, _planeVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof (quadData), quadData, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);

        if(ModelID==1)
        {
            readMesh("./models/bunny.ply"); //normalized vertices coordinates
            scale = 4.0;
        }
        if(ModelID==2)
        {
            readMesh("./models/dragon.ply"); //normalized vertices coordinates
            scale = 1.0;
        }
        if(ModelID==3)
        {
            readMesh("./models/teapot.ply"); //normalized vertices coordinates
            scale = 0.2;
        }

	//triangle mesh
	glGenVertexArrays(1, &_meshVertexArray);
	glBindVertexArray(_meshVertexArray);

	glGenBuffers(1, &_meshVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, _meshVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER,  _mesh->vertices.size() * sizeof (trimesh::point), _mesh->vertices.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &_meshNormalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, _meshNormalBuffer);
	glBufferData(GL_ARRAY_BUFFER, _mesh->normals.size() * sizeof(trimesh::vec), _mesh->normals.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(1);

	glGenBuffers(1, &_meshIndicesBuffer );
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _meshIndicesBuffer );
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _triangleIndices.size()*sizeof(unsigned int), _triangleIndices.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);

}


void SceneShader::startup()
{
        if(ModelID==1)
            _programPlane = compile_shaders("./shaders/plane.vert", "./shaders/plane.frag");

	_programMesh = compile_shaders("./shaders/mesh.vert", "./shaders/mesh.frag");

	_programLight = compile_shaders("./shaders/light.vert", "./shaders/light.frag");

	createVertexBuffer();

}

void SceneShader::renderPlane()
{
	glBindVertexArray(_planeVertexArray);

	glUseProgram(_programPlane);

	//scene matrices and camera setup
	glm::vec3 eye(0.0f,0.3f, 2.0f);
	glm::vec3 up(0.0f, 1.0f, 0.0f);
	glm::vec3 center(0.0f, 0.0f, 0.0f);

	_modelview = glm::lookAt( eye, center, up);

	glm::mat4 identity(1.0f);
	_projection = glm::perspective( 45.0f, _aspectRatio, 0.01f, 100.0f);

	glm::mat4 rotationX = glm::rotate(identity, _yRot  * PI/180.0f, glm::vec3(0.0f, 1.0f, 0.0f));

        _modelview *=  rotationX;
	
	//Uniform variables
	glUniformMatrix4fv(glGetUniformLocation(_programPlane, "modelview"), 1, GL_FALSE, glm::value_ptr(_modelview));
	glUniformMatrix4fv(glGetUniformLocation(_programPlane, "projection"), 1, GL_FALSE, glm::value_ptr(_projection));

	glUniform3fv(glGetUniformLocation(_programPlane, "lightPosition"), 1, glm::value_ptr(lightPosition) );

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glBindVertexArray(0);
}

void SceneShader::renderMesh()
{
	glBindVertexArray(_meshVertexArray);

	glUseProgram(_programMesh);

	//scene matrices and camera setup
        glm::vec3 eye(0.0f,0.3f, 2.0f);
        glm::vec3 up(0.0f, 1.0f, 0.0f);
        glm::vec3 center(0.0f, 0.0f, 0.0f);

        _modelview = glm::lookAt( eye, center, up);

        glm::mat4 identity(1.0f);
        _projection = glm::perspective( 45.0f, _aspectRatio, 0.01f, 100.0f);

        glm::mat4 rotationX = glm::rotate(identity, _yRot  * PI/180.0f, glm::vec3(0.0f, 1.0f, 0.0f));

        _modelview *=  rotationX;

	//uniform variables
	glUniformMatrix4fv(glGetUniformLocation(_programMesh, "modelview"), 1, GL_FALSE, glm::value_ptr(_modelview));
	glUniformMatrix4fv(glGetUniformLocation(_programMesh, "projection"), 1, GL_FALSE, glm::value_ptr(_projection));

	glUniform3fv(glGetUniformLocation(_programMesh, "lightPosition"), 1, glm::value_ptr(lightPosition) );

        glUniform3fv(glGetUniformLocation(_programMesh, "kd"), 1, glm::value_ptr(ObjectColor) );
        glUniform3fv(glGetUniformLocation(_programMesh, "k_blue"), 1, glm::value_ptr(k_blue) );
        glUniform3fv(glGetUniformLocation(_programMesh, "k_yellow"), 1, glm::value_ptr(k_yellow) );
        glUniform1f(glGetUniformLocation(_programMesh, "alpha"), alpha );
        glUniform1f(glGetUniformLocation(_programMesh, "beta"), beta );
        glUniform1f(glGetUniformLocation(_programMesh, "scale"), scale );

	glDrawElements( GL_TRIANGLES, _mesh->faces.size()*3, GL_UNSIGNED_INT, 0 );

	glBindVertexArray(0);
}


void SceneShader::renderLight()
{
	glUseProgram(_programLight);

	//scene matrices and camera setup
        glm::vec3 eye(0.0f,0.3f, 2.0f);
        glm::vec3 up(0.0f, 1.0f, 0.0f);
        glm::vec3 center(0.0f, 0.0f, 0.0f);

        _modelview = glm::lookAt( eye, center, up);

        glm::mat4 identity(1.0f);
        _projection = glm::perspective( 45.0f, _aspectRatio, 0.01f, 100.0f);

        glm::mat4 rotationX = glm::rotate(identity, _yRot  * PI/180.0f, glm::vec3(0.0f, 1.0f, 0.0f));

        _modelview *=  rotationX;

	//uniform variables
	glUniformMatrix4fv(glGetUniformLocation(_programLight, "modelview"), 1, GL_FALSE, glm::value_ptr(_modelview));
	glUniformMatrix4fv(glGetUniformLocation(_programLight, "projection"), 1, GL_FALSE, glm::value_ptr(_projection));

	glUniform3fv(glGetUniformLocation(_programLight, "lightPosition"), 1, glm::value_ptr(lightPosition) );

        glPointSize(50.0f);
	glDrawArrays( GL_POINTS, 0, 1);

}

void SceneShader::render()
{
	renderPlane();
	renderMesh();
	renderLight();
}

void SceneShader::setZTranslation(float z)
{
	_zTranslation = z;
}

void SceneShader::setAspectRatio(float ratio)
{
	_aspectRatio = ratio;
}

void SceneShader::setRotationX( float x )
{
	_xRot = x;
}

void SceneShader::setRotationY( float y )
{
	_yRot = y;
}

void SceneShader::setGoochVars(float bl, float yl, float alpha1, float beta1, int ModelID1)
{
        k_blue = glm::vec3(0.0, 0.0, bl);
        k_yellow = glm::vec3(yl, yl, 0.0);
        alpha = alpha1;
        beta = beta1;
        ModelID = ModelID1;
}

void SceneShader::shutdown()
{
	glDeleteBuffers(1, &_meshVertexBuffer);
	glDeleteBuffers(1, &_meshNormalBuffer);
	glDeleteBuffers(1, &_meshIndicesBuffer );
	glDeleteVertexArrays(1, &_meshVertexArray);
	glDeleteVertexArrays(1, &_planeVertexArray);
}

void SceneShader::updateLightPositionX(float x)
{
	lightPosition.x += x;
}

void SceneShader::updateLightPositionY(float y)
{
	lightPosition.y += y;
}

void SceneShader::updateLightPositionZ(float z)
{
	lightPosition.z += z;
}

void SceneShader::updateKBlue(float z)
{
            k_blue.z += z;
}

void SceneShader::updateKYellow(float x)
{
            k_yellow.x += x;
            k_yellow.y += x;

}

void SceneShader::updateAlpha(float x)
{
        alpha += x;
}

void SceneShader::updateBeta(float x)
{
        beta += x;
}

void SceneShader::updateRC(float x)
{
        ObjectColor.x += x;
}

void SceneShader::updateGC(float y)
{
        ObjectColor.y += y;
}

void SceneShader::updateBC(float z)
{
        ObjectColor.z += z;
}


void SceneShader::updateZoom(float z)
{
        scale += z;
}

void SceneShader::updateObjectColor(float rc, float gc, float bc)
{
        ObjectColor.x = rc;
        ObjectColor.y = gc;
        ObjectColor.z = bc;
}

SceneShader::~SceneShader()
{
	shutdown();
}
