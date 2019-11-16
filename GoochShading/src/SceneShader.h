/*
 * SceneShader.h
 *
 *  Created on: Nov 17, 2015
 *      Author: acarocha
 */

#ifndef SCENESHADER_H_
#define SCENESHADER_H_

#include "Shader.h"
#include <vector>

#define GLM_FORCE_RADIANS

#include "TriMesh.h"
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class SceneShader : public Shader
{
public:

	SceneShader();
	~SceneShader();

	void startup ();
	void shutdown ();
	void render();

	void renderPlane();
	void renderMesh();
	void renderLight();

	void setAspectRatio( float ratio );
	void setZTranslation(float z);
	void setRotationX( float x );
	void setRotationY( float y );

	void updateLightPositionX( float x );
	void updateLightPositionY( float y );
	void updateLightPositionZ( float z );
        void updateKBlue( float z );
        void updateKYellow( float x );
        void updateAlpha( float x );
        void updateBeta( float x );
        void updateRC( float x );
        void updateGC( float y );
        void updateBC( float z );
        void updateZoom( float z );

        void updateObjectColor( float rc, float gc, float bc );
        void setGoochVars(float bl, float yl, float alpha1, float beta1, int ModelID1);

private:

	/*methods*/

	void readMesh( std::string filename );
	void createVertexBuffer();


	/*variables*/
	GLuint _programLight;
	GLuint _programPlane;
	GLuint _programMesh;


	GLuint _planeVertexArray;
	GLuint _planeVertexBuffer;

	GLuint _meshVertexArray;
	GLuint _meshVertexBuffer;
	GLuint _meshNormalBuffer;
	GLuint _meshIndicesBuffer;

        GLint _mvUniform, _projUniform;

	/* Matrices */
	glm::mat4 _modelview;
	glm::mat4 _projection;

	float _zTranslation;
	float _xRot;
	float _yRot;
	float _aspectRatio;

	/* Textures */
	GLuint _textureID;

	/*vertices*/
	std::vector<float> _vertices;

	trimesh::TriMesh* _mesh;
	std::vector<unsigned int> _triangleIndices;

	glm::vec3 lightPosition;

        /* Gooch Properties */
        glm::vec3 ObjectColor;
        glm::vec3 k_blue;
        glm::vec3 k_yellow;
        float alpha;
        float beta;
        float scale;
        int ModelID;

};

#endif /* SCENESHADER_H_ */
