/*
 * SceneShader.cpp
 *
 *  Created on: Nov 17, 2015
 *      Author: acarocha
 */

#include "SceneShader.h"
#include "lodepng.h"

static float PI = 3.14159265359;

std::string imageFilename[15] = {"textures/fig-7b.png",
                                "textures/fig-7c.png",
                                "textures/fig-7d.png",
                                "textures/fig-8a.png",
                                "textures/fig-8b.png",
                                "textures/fig-9b.png",
                                "textures/fig-9c.png",
                                "textures/fig-9e.png",
                                "textures/fig-9f.png",
                                "textures/fig-10b.png",
                                "textures/fig-10c.png",
                                "textures/fig-10d.png",
                                "textures/fig-11b.png",
                                "textures/fig-11c.png",
                                "textures/fig-11d.png"};


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
        tex_no = 0;
        r = 0;
        scale = 2.0;
        ModelID = 1;

}

void SceneShader::calculateCylindricalUVCoordinates()
{
        for( unsigned int i = 0; i < _mesh->vertices.size(); i++)
        {
            glm::vec3 vertex(_mesh->vertices[i][0],_mesh->vertices[i][1],_mesh->vertices[i][2]);

            float theta = glm::atan(vertex.z,vertex.x)/PI;

            _uvs.push_back(glm::vec2(theta, vertex.y*8.0));
        }
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
        std::vector<GLubyte> colormap;


        //red
        colormap.push_back(255);
        colormap.push_back(0);
        colormap.push_back(0);
        colormap.push_back(255);

        //blue
        colormap.push_back(0);
        colormap.push_back(0);
        colormap.push_back(255);
        colormap.push_back(255);

        //creating texture
        _textureID = _texture.create1DTexture(colormap);


        //create plane geometry
	static const GLfloat quadData[] =
	{
                        -1.0f, 0.0f, -1.0f,
                        -1.0f, 0.0f, 1.0f,
                        1.0f, 0.0f, -1.0f,
                        1.0f, 0.0f, 1.0f,
	};

        //create plane geometry
        static const GLfloat uvsPlane[] =
        {
                        0.0f, 0.0f,
                        0.0f, 1.0f,
                        1.0f, 0.0f,
                        1.0f, 1.0f,
        };


        //creating texture 2D

        unsigned int error;

        for(int i = 0; i < 15; i++){
            error = lodepng::decode( _image[i], _imageWidth, _imageHeight, imageFilename[i].c_str());
            _textureImageID[i] = _texture.create2DTexture(_image[i], _imageWidth, _imageHeight);
        }

        if(error)
        {
        std::cout << "reading error "<<error<<":"<<lodepng_error_text(error) <<std::endl;
        }

       //passing model attributes to the GPU
	//plane
	glGenVertexArrays(1, &_planeVertexArray);
	glBindVertexArray(_planeVertexArray);

	glGenBuffers(1, &_planeVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, _planeVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof (quadData), quadData, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

        glGenBuffers(1, &_planeTextureBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, _planeTextureBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof (uvsPlane), uvsPlane, GL_STATIC_DRAW);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(1);

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
            readMesh("./models/sphere.ply"); //normalized vertices coordinates
            scale = 0.5;
        }
        if(ModelID==4)
        {
            readMesh("./models/horse.ply"); //normalized vertices coordinates
            scale = 5.0;
        }
        calculateCylindricalUVCoordinates();
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


        glGenBuffers(1, &_meshTextureBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, _meshTextureBuffer);
        glBufferData(GL_ARRAY_BUFFER, _uvs.size() * sizeof(glm::vec2), _uvs.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(2);


	glGenBuffers(1, &_meshIndicesBuffer );
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _meshIndicesBuffer );
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _triangleIndices.size()*sizeof(unsigned int), _triangleIndices.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);

}


void SceneShader::startup()
{
	_programPlane = compile_shaders("./shaders/plane.vert", "./shaders/plane.frag");

	_programMesh = compile_shaders("./shaders/mesh.vert", "./shaders/mesh.frag");

	_programLight = compile_shaders("./shaders/light.vert", "./shaders/light.frag");

	createVertexBuffer();

        printInfo();

}

void SceneShader::renderPlane()
{
	glBindVertexArray(_planeVertexArray);

	glUseProgram(_programPlane);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


        //activating texture
        _texture.bind1DTexture(_programPlane, _textureID, std::string("colormap"));

        _texture.bind2DTexture(_programPlane, _textureImageID[int(tex_no/2)], std::string("image"));

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

        _texture.unbind1DTexture();
        _texture.unbind2DTexture();

        glDisable(GL_BLEND);

	glBindVertexArray(0);
}

void SceneShader::renderMesh()
{
	glBindVertexArray(_meshVertexArray);

	glUseProgram(_programMesh);

        //glBindTexture(_programMesh, _textureID);

        glEnable(GL_BLEND);

        _texture.bind1DTexture(_programMesh, _textureID, std::string("colormap"));

        _texture.bind2DTexture(_programMesh, _textureImageID[int(tex_no/2)], std::string("image"));


	//scene matrices and camera setup
	glm::vec3 eye(0.0f, 0.3f, 2.0f);
	glm::vec3 up(0.0f, 1.0f, 0.0f);
	glm::vec3 center(0.0f, 0.0f, 0.0f);

	_modelview = glm::lookAt( eye, center, up);

	_projection = glm::perspective( 45.0f, _aspectRatio, 0.01f, 100.0f);

	glm::mat4 identity(1.0f);

	glm::mat4 rotationX = glm::rotate(identity, _yRot  * PI/180.0f, glm::vec3(0.0f, 1.0f, 0.0f));

        _modelview *=  rotationX;

	//uniform variables
	glUniformMatrix4fv(glGetUniformLocation(_programMesh, "modelview"), 1, GL_FALSE, glm::value_ptr(_modelview));
	glUniformMatrix4fv(glGetUniformLocation(_programMesh, "projection"), 1, GL_FALSE, glm::value_ptr(_projection));

	glUniform3fv(glGetUniformLocation(_programMesh, "lightPosition"), 1, glm::value_ptr(lightPosition) );
        glUniform1f(glGetUniformLocation(_programMesh, "scale"), scale );
        glUniform1f(glGetUniformLocation(_programMesh, "r"), r );

	glDrawElements( GL_TRIANGLES, _mesh->faces.size()*3, GL_UNSIGNED_INT, 0 );

        //Texture
        _texture.unbind1DTexture();
        _texture.unbind2DTexture();

        glDisable(GL_BLEND);

	glBindVertexArray(0);
}


void SceneShader::renderLight()
{
	glUseProgram(_programLight);

	//scene matrices and camera setup
	glm::vec3 eye(0.0f, 0.3f, 2.0f);
	glm::vec3 up(0.0f, 1.0f, 0.0f);
	glm::vec3 center(0.0f, 0.0f, 0.0f);

	_modelview = glm::lookAt( eye, center, up);

	_projection = glm::perspective( 45.0f, _aspectRatio, 0.01f, 100.0f);

	glm::mat4 identity(1.0f);

	glm::mat4 rotationX = glm::rotate(identity, _yRot  * PI/180.0f, glm::vec3(0.0f, 1.0f, 0.0f));

        _modelview *=  rotationX;

	//uniform variables
	glUniformMatrix4fv(glGetUniformLocation(_programLight, "modelview"), 1, GL_FALSE, glm::value_ptr(_modelview));
	glUniformMatrix4fv(glGetUniformLocation(_programLight, "projection"), 1, GL_FALSE, glm::value_ptr(_projection));

	glUniform3fv(glGetUniformLocation(_programLight, "lightPosition"), 1, glm::value_ptr(lightPosition) );

	glPointSize(30.0f);
	glDrawArrays( GL_POINTS, 0, 1);

}

void SceneShader::render()
{
        if (ModelID==1) renderPlane();
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

void SceneShader::setXtoonVars(float r1, int tex_no1, int ModelID1)
{
        r = r1;
        tex_no = tex_no1;
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

void SceneShader::updateTexture(int i)
{
        printInfo();
        if(tex_no >= 0 && tex_no < 29){
            tex_no += i;
        }
        else
            tex_no = 0;
}

void SceneShader::updateZoom(float z)
{
        printInfo();
        scale += z;
}

void SceneShader::update_r(float x)
{
        printInfo();
        r += x;
        if(r < 0)
            r = 0;
}

void SceneShader::printInfo()
{
        system("clear");
        printf("\n==============================================================================");
        printf("\n=======================    RENDERING PARAMETERS   ============================");
        printf("\n==============================================================================");
        printf("\n\tTexture \n\t=> Toggle using keyboard << R and T >> \t: %s", imageFilename[int(tex_no/2)].c_str());
        printf("\n\tModel size \n\t=> Change using keyboard << O and P >> \t: %f", scale);
        printf("\n\tMax factor (r) \n\t=> Change using keyboard << N and M >> \t: %f", r);
        printf("\n==============================================================================");
        printf("\n");
}

SceneShader::~SceneShader()
{
	shutdown();
}
