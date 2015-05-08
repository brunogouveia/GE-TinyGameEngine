#include <MeshObject.h>
#include <obj/tiny_obj_loader.h>
#include <iostream>
#include <GameWindow.h>

MeshObject::MeshObject(glm::mat4 modelMatrix) {
	vertexArrayObj = -1;
	vertexBuffer = -1;
	numVertices = -1;
	this->modelMatrix = modelMatrix;
}

MeshObject::~MeshObject() {
	for (unsigned int i = 0; i < shapeVector.size(); i++) {
		delete shapeVector[i];
	}
}

void MeshObject::init(int basicShader, int lightShader) {
    // Set shaders
    this->basicShader = basicShader;
    this->lightShader = lightShader;

    // Create vertex array object
    glGenVertexArrays(1, &vertexArrayObj);
    glBindVertexArray(vertexArrayObj);

    // Create buffer
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    float d[] = {
        0,0,0,+1,   0, 0,+1,   1,1,
        0,0,0,+1,   0, 0,+1,   0,1,
        0,0,0,+1,   0, 0,+1,   1,0
    };
    numVertices = 3;
    // Copy data to 
    glBufferData(GL_ARRAY_BUFFER, sizeof(d), d, GL_STATIC_DRAW);

    // Unbind vao and buffer
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void MeshObject::shadowPass() {
    // Set program
    glUseProgram(Light::getShadowShader());

    // Set model matrix
    GameWindow::getRenderer()->getCamera()->setModelMatrix(modelMatrix);

    // Bind vao and buffer
    glBindVertexArray(vertexArrayObj);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

    // Set attribute 0 - vertex (vec4)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 9*sizeof(float), (void*)0);

    // Draw cube
    for (unsigned int i = 0; i < shapeVector.size(); ++i) {
    	shapeVector[i]->material->activeMaterial();
		glDrawArrays(GL_TRIANGLES, shapeVector[i]->begin, shapeVector[i]->end - shapeVector[i]->begin + 1);
	}

    // Disable attributes
    glDisableVertexAttribArray(0);

    // Unbind everything
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER,0);

    // Unbind shader
    glUseProgram(0);
}

void MeshObject::rendererPass(bool useLight) {
    // Set program
    glUseProgram(useLight ? lightShader : basicShader);

    // Set model matrix
    GameWindow::getRenderer()->getCamera()->setModelMatrix(modelMatrix);

    // Bind vao and buffer
    glBindVertexArray(vertexArrayObj);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

    // Set attribute 0 - vertex (vec4)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 9*sizeof(float), (void*)0);
    // Set attribute 1 - normal (vec3)
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9*sizeof(float), (void*)(4*sizeof(float)));
    // Set attribute 2 - texture (vec2)
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9*sizeof(float), (void*)(7*sizeof(float)));

//    // Activate texture
    texture.active();
    // Bind texture
    texture.bind();
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_CUBE_MAP, Light::shadowCubeMap);
    glTexGendv(GL_S,GL_EYE_PLANE,Light::Svec);
    glTexGendv(GL_T,GL_EYE_PLANE,Light::Tvec);
    glTexGendv(GL_R,GL_EYE_PLANE,Light::Rvec);
    glTexGendv(GL_Q,GL_EYE_PLANE,Light::Qvec);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_COMPARE_MODE,GL_COMPARE_R_TO_TEXTURE);
//
//    // Activate normalMap
    normalMap.active();
    // Bind normal Map
    normalMap.bind();

    GameWindow::getRenderer()->bindDeferredTextures(useLight ? lightShader : basicShader);

    // Set uniform value
    int id = glGetUniformLocation(useLight ? lightShader : basicShader, "text");
    if (id >= 0) glUniform1i(id, 0);
    id = glGetUniformLocation(useLight ? lightShader : basicShader, "depthText");
    if (id >= 0) glUniform1i(id, 1);
    id = glGetUniformLocation(useLight ? lightShader : basicShader, "normalMap");
    if (id >= 0) glUniform1i(id, 2);

    // Active material
    material->activeMaterial();

    // Draw cube
    for (unsigned int i = 0; i < shapeVector.size(); ++i) {
    	shapeVector[i]->material->activeMaterial();
    	shapeVector[i]->texture.active();
    	shapeVector[i]->texture.bind();
    	shapeVector[i]->normalMap.active();
    	shapeVector[i]->normalMap.bind();
		glDrawArrays(GL_TRIANGLES, shapeVector[i]->begin, shapeVector[i]->end - shapeVector[i]->begin + 1);
	}

    // Disable attributes
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);

    // Unbind everything
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glUseProgram(0);
}

void MeshObject::deferredPass() {
    // Set model matrix
    GameWindow::getRenderer()->getCamera()->setModelMatrix(modelMatrix);

    // Bind vao and buffer
    glBindVertexArray(vertexArrayObj);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

    // Set attribute 0 - vertex (vec4)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 9*sizeof(float), (void*)0);
    // Set attribute 1 - normal (vec3)
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9*sizeof(float), (void*)(4*sizeof(float)));

    // Draw cube
	for (unsigned int i = 0; i < shapeVector.size(); ++i) {
		shapeVector[i]->texture.active();
		shapeVector[i]->texture.bind();
		shapeVector[i]->normalMap.active();
		shapeVector[i]->normalMap.bind();
		glDrawArrays(GL_TRIANGLES, shapeVector[i]->begin, shapeVector[i]->end - shapeVector[i]->begin + 1);
	}

    // Disable attributes
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    // Unbind everything
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER,0);
}

void MeshObject::setTexture(Texture newTexure) {
	GameObject::setTexture(newTexure);
	for (unsigned int i = 0; i < shapeVector.size(); i++) {
		shapeVector[i]->texture = newTexure;
	}
}

void MeshObject::setNormalMap(Texture newNormalMap) {
	GameObject::setNormalMap(newNormalMap);
	for (unsigned int i = 0; i < shapeVector.size(); i++) {
		shapeVector[i]->normalMap = newNormalMap;
	}
}

void MeshObject::loadFromFile(char * fileName) {
	std::string basePath = "obj/crytek-sponza/";
    // Shapes, materials
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    // Load from file
    std::string err = LoadObj(shapes, materials, fileName, basePath.c_str());

    // Check for error
    if (!err.empty()) {
        std::cerr << err << std::endl;
        return;
    }

    // Compute size
    numVertices = 0;
    for (unsigned int i = 0; i < shapes.size(); ++i) {
        numVertices += shapes[i].mesh.indices.size();
    }

    // Initialize data
    std::vector<float> data;
    data.reserve(numVertices * 9);

    // Create textures for each material
    vector<Material*> materialsPtr;
	vector<Texture> textures;
	vector<Texture> normalMaps;
	for (unsigned int i = 0; i < materials.size(); ++i) {
		std::cout << "Loading material: " << materials[i].name << std::endl;
		// Load material
		Material * newMaterial = new Material();
		newMaterial->setEmissive(materials[i].emission);
		newMaterial->setAmbient(materials[i].ambient);
		newMaterial->setDiffuse(materials[i].diffuse);
		newMaterial->setSpecular(materials[i].specular);
		newMaterial->setShininess(materials[i].shininess);
		// Add to materialsPtr
		materialsPtr.push_back(newMaterial);

		// Load texture
		if (!materials[i].diffuse_texname.empty())
			textures.push_back(Texture(GL_TEXTURE0, basePath + materials[i].diffuse_texname));
		else
			textures.push_back(Texture(GL_TEXTURE0));
		// Load normal map
		if (!materials[i].unknown_parameter["map_bump"].empty())
			normalMaps.push_back(Texture(GL_TEXTURE2, basePath + materials[i].unknown_parameter["map_bump"]));
		else
			normalMaps.push_back(Texture(GL_TEXTURE2, true));
	}

	// Used to create shapes
    int currentTriangle = 0; // Current vertex
    for (unsigned int i = 0; i < shapes.size(); ++i) {
    	// Create shape
    	Shape * currentShape = new Shape((currentTriangle) * 3, (currentTriangle) * 3);

        // For each triangle
        for (unsigned int j = 0; j < shapes[i].mesh.indices.size()/3; ++j) {
            // For each point
            for (unsigned int k = 0; k < 3; ++k) {
                // Point index
                int index = shapes[i].mesh.indices[3*j + k];

                // Set vertex
                data[27 * currentTriangle + 9 * k]     = shapes[i].mesh.positions[3 * index];
                data[27 * currentTriangle + 9 * k + 1] = shapes[i].mesh.positions[3 * index + 1];
                data[27 * currentTriangle + 9 * k + 2] = shapes[i].mesh.positions[3 * index + 2];
                data[27 * currentTriangle + 9 * k + 3] = 1.0;

                // Set normal
                if (shapes[i].mesh.normals.size() > 0) {
                    data[27 * currentTriangle + 9 * k + 4] = shapes[i].mesh.normals[3 * index];
                    data[27 * currentTriangle + 9 * k + 5] = shapes[i].mesh.normals[3 * index + 1];
                    data[27 * currentTriangle + 9 * k + 6] = shapes[i].mesh.normals[3 * index + 2];
                }

                // Set texture
                if (shapes[i].mesh.texcoords.size() > 0) {
                    data[27 * currentTriangle + 9 * k + 7] = shapes[i].mesh.texcoords[2 * index];
                    data[27 * currentTriangle + 9 * k + 8] = shapes[i].mesh.texcoords[2 * index + 1];
                } else {
                    data[27 * currentTriangle + 9 * k + 7] = 0.0;
                    data[27 * currentTriangle + 9 * k + 8] = 0.0;
                }
            }

            // If obj has no normals, we need to compute them
            if (shapes[i].mesh.normals.size() == 0) {
                // Vectors of triangle
                glm::vec3 a = glm::vec3(data[27*currentTriangle + 9] - data[27*currentTriangle], data[27*currentTriangle + 10] - data[27*currentTriangle + 1], data[27*currentTriangle + 11] - data[27*currentTriangle + 2]);
                glm::vec3 b = glm::vec3(data[27*currentTriangle + 18] - data[27*currentTriangle], data[27*currentTriangle + 19] - data[27*currentTriangle + 1], data[27*currentTriangle + 20] - data[27*currentTriangle + 2]);

                // Compute normal
                glm::vec3 n = glm::normalize(glm::cross(a,b));

                // Copy normal to data
                for (int k = 0; k < 3; ++k) {
                    data[27 * currentTriangle + 9 * k + 4] = n[0];
                    data[27 * currentTriangle + 9 * k + 5] = n[1];
                    data[27 * currentTriangle + 9 * k + 6] = n[2];
                }
            }
            // Incremente curVert
            currentTriangle++;
        }

        // Update end index of shape
        currentShape->end += shapes[i].mesh.indices.size() - 1;

		if (shapes[i].mesh.material_ids[0] >= 0) {
			std::cout << "Maior" << std::endl;
			// Set shape's material
			currentShape->material = materialsPtr[shapes[i].mesh.material_ids[0]];
			// Set shape's texture
			currentShape->texture = textures[shapes[i].mesh.material_ids[0]];
			currentShape->normalMap = normalMaps[shapes[i].mesh.material_ids[0]];
		}

		// Add to shapesVector
        shapeVector.push_back(currentShape);
    }

    // Bind vertex array object
    glBindVertexArray(vertexArrayObj);
    // Bind buffer
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    // Copy data to 
    glBufferData(GL_ARRAY_BUFFER, (numVertices * 9 * sizeof(float)), &data[0], GL_STATIC_DRAW);

    // Unbind vao and buffer
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    ErrCheck("obj");
}
