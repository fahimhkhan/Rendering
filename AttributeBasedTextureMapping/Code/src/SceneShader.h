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
#include "texture.h"

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
        void calculateCylindricalUVCoordinates();
        void updateTexture(int i);
        void setXtoonVars(float r1, int tex_no1, int ModelID1);
        void updateZoom( float z );
        void update_r( float  );
        void printInfo();

private:

	/*methods*/

	void readMesh( std::string filename );
	void createVertexBuffer();


// 

std::vector<unsigned char> _image[15];
//std::vector<unsigned char> _image1;
unsigned int _imageWidth;
unsigned int _imageHeight;


	/*variables*/
	GLuint _programLight;
	GLuint _programPlane;
	GLuint _programMesh;


	GLuint _planeVertexArray;
	GLuint _planeVertexBuffer;
        GLuint _planeTextureBuffer;

	GLuint _meshVertexArray;
	GLuint _meshVertexBuffer;
	GLuint _meshNormalBuffer;
	GLuint _meshIndicesBuffer;
        GLuint _meshTextureBuffer;

        GLint _mvUniform, _projUniform;

	/* Matrices */
	glm::mat4 _modelview;
	glm::mat4 _projection;

	float _zTranslation;
	float _xRot;
	float _yRot;
	float _aspectRatio;

	/* Textures */
        Texture _texture;

	GLuint _textureID;
        GLuint _textureImageID[15];
        //GLuint _textureImageID1;

	/*vertices*/
        std::vector<glm::vec2> _uvs;
	std::vector<float> _vertices;

	trimesh::TriMesh* _mesh;
	std::vector<unsigned int> _triangleIndices;

	glm::vec3 lightPosition;

        float scale;
        float r;
        int ModelID;
        int tex_no;

};

#endif /* SCENESHADER_H_ */
