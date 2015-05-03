/*
 * XmlParser.cpp
 *
 *  Created on: May 24, 2015
 *      Author: Bruno Gouveia
 */

#include "xml/XmlParser.h"
#include <GameWindow.h>
#include <Input.h>
#include <Cube.h>
#include <Scene.h>
#include <Script.h>
#include <Material.h>
#include <MeshObject.h>
#include <GBuffer.h>

XmlParser::~XmlParser() {
}

void XmlParser::loadFromXml(const char * fileName, int * argc, char ** argv) {
    XmlFile xmlFile(fileName);
    XmlDocument doc;
    doc.parse<0>(xmlFile.data());
    int W, H;

    XmlNode * rtNode = doc.first_node("ge");

    /*** Read window information ***/
    XmlNode * windowNode = rtNode->first_node("window");
    if (windowNode) {
        // Try to read window name
        XmlNode * name;
        xmlElement(name, windowNode);

        // Read width and height
        XmlNode * width, *height;
        xmlElement(width, windowNode);
        xmlElement(height, windowNode);

        W = atoi(width->value());
        H = atoi(height->value());

        // Set window parameters
        GameWindow::setWindow((name) ? name->value() : "GE", W, H);
    } else {
        W = 512;
        H = 512;

        GameWindow::setWindow("GE", W, H);
    }
    // Init gamewindow and light
    GameWindow::init(argc, argv);
    Light::init();
    Material::init();
    Input::init();
    GBuffer::init(W,H);
    // Load noise texture
    glActiveTexture(GL_TEXTURE11);
    LoadTexBMP("textures/noise.bmp");
    /*** Read Scene ***/
    // Reference to scene node
    XmlNode * sceneNode = rtNode->first_node("scene");
    // Instantiate scene    
    Scene * scene = new Scene();
    // Instantiate renderer
    Renderer * renderer = new Renderer();
    // Set renderer
    GameWindow::setRenderer(renderer);
    renderer->setScene(scene);

	// Create camera
	Camera * camera = new Camera();
	// Set camera
	renderer->setCamera(camera);
	camera->setAspectRatio(W/H);

	// Read Camera
	XmlNode * cameraNode = rtNode->first_node("camera");
	if (cameraNode) {
		readModel(camera, cameraNode->first_node("model"));
//		// Set position
//		XmlNode * position = cameraNode->first_node("position");
//		if (position) {
//			// Get coordinates
//			XmlAttr * x, *y, *z;
//			xmlAttribute(x, position);
//			xmlAttribute(y, position);
//			xmlAttribute(z, position);
//			camera->setPosition(atof(x->value()), atof(y->value()), atof(z->value()));
//		}
//
//		// Set direction
//		XmlNode * direction= cameraNode->first_node("direction");
//		if (direction) {
//			// Get coordinates
//			XmlAttr * x, *y, *z;
//			xmlAttribute(x, direction);
//			xmlAttribute(y, direction);
//			xmlAttribute(z, direction);
//			camera->setDirection(atof(x->value()), atof(y->value()),
//					atof(z->value()));
//		}
//
//
//		// Set direction
//		XmlNode * up = cameraNode->first_node("up");
//		if (up) {
//			// Get coordinates
//			XmlAttr * x, *y, *z;
//			xmlAttribute(x, up);
//			xmlAttribute(y, up);
//			xmlAttribute(z, up);
//			camera->setUp(atof(x->value()), atof(y->value()),
//					atof(z->value()));
//		}

		// Read scripts
		XmlNode * scriptsNode = cameraNode->first_node("scripts");
		readScripts(scene, camera, scriptsNode);
	}

    // Light shadow map must be initialized after renderer has a camera
    Light::initShadowMap(renderer->createShaderProg("shaders/shadow.vert", "shaders/shadow.frag"));

    // Read objects
    XmlNode * objectsNode = sceneNode->first_node("objects");
    if (objectsNode) {
        // Read all cubes
        XmlNode * cubeNode = objectsNode->first_node("cube");
        readCubes(renderer, scene, cubeNode);

        // Read all mesh from scene
        XmlNode * meshNode = objectsNode->first_node("mesh");
        readMeshs(renderer, scene, meshNode);
        
    } else {
        printf("No objects\n");
    }

    // Read Lights
    XmlNode * lights = sceneNode->first_node("lights");
    readLights(scene, lights);
}

void XmlParser::readCubes(Renderer * renderer ,Scene * scene, XmlNode * cubeNode) {
    // Read all cubes from scene
    while(cubeNode) {
        // Create cube
        Cube * c = new Cube();
        // Read shaders and init it
        int basicShader = renderer->getDefaultBasicShader();
        int lightShader = renderer->getDefaultLightShader();
        XmlNode * basicShaderNode = cubeNode->first_node("basic-shader");
        XmlNode * lightShaderNode = cubeNode->first_node("light-shader");
        
        // If there is a basic shader node
        if (basicShaderNode) {
            XmlAttr * vert, * frag;
            xmlAttribute(vert, basicShaderNode);
            xmlAttribute(frag, basicShaderNode);
            // Create new shader
            basicShader = renderer->createShaderProg(vert->value(), frag->value());
        }

        // If there is a light shader node
        if (lightShaderNode) {
            XmlAttr * vert, * frag;
            xmlAttribute(vert, lightShaderNode);
            xmlAttribute(frag, lightShaderNode);
            // Create new shader
            lightShader = renderer->createShaderProg(vert->value(), frag->value());
        }

        // Init cube
        c->init(basicShader, lightShader);
        
        // Try to read model
        readModel(c, cubeNode->first_node("model"));


        // Add c to scene
        scene->addGameObject(c);

        // Set texture
        XmlNode * texture = cubeNode->first_node("texture");
        if (texture) {
            // Get file name
            XmlAttr * file;
            xmlAttribute(file, texture);

            // Create texture
            Texture texture(GL_TEXTURE0, std::string(file->value()));

            // Set texture
            c->setTexture(texture);
        }

        // Set material
        XmlNode * material = cubeNode->first_node("material");
        if (material) {
        	// Create material
        	Material * m = new Material();

        	// Set emissive
        	XmlNode * emissive = material->first_node("emissive");
			if (emissive) {
				XmlAttr * r, *g, *b;
				xmlAttribute(r, emissive);
				xmlAttribute(g, emissive);
				xmlAttribute(b, emissive);
				// Set diffuse
				m->setEmissive(atof(r->value()), atof(g->value()),
						atof(b->value()));
			}

			// Read ambient
			XmlNode * ambient = material->first_node("ambient");
			if (ambient) {
				XmlAttr * r, *g, *b;
				xmlAttribute(r, ambient);
				xmlAttribute(g, ambient);
				xmlAttribute(b, ambient);
				// Set diffuse
				m->setAmbient(atof(r->value()), atof(g->value()),
						atof(b->value()));
			}

			// Read diffuse
			XmlNode * diffuse = material->first_node("diffuse");
			if (diffuse) {
				XmlAttr * r, *g, *b;
				xmlAttribute(r, diffuse);
				xmlAttribute(g, diffuse);
				xmlAttribute(b, diffuse);
				// Set diffuse
				m->setDiffuse(atof(r->value()), atof(g->value()),
						atof(b->value()));
			}

			// Read specular
			XmlNode * specular = material->first_node("specular");
			if (specular) {
				XmlAttr * r, *g, *b;
				xmlAttribute(r, specular);
				xmlAttribute(g, specular);
				xmlAttribute(b, specular);
				// Set diffuse
				m->setSpecular(atof(r->value()), atof(g->value()),
						atof(b->value()));
			}

			// Read specular
			XmlNode * shininess = material->first_node("shininess");
			if (shininess) {
				XmlAttr * value;
				xmlAttribute(value, shininess);
				// Set diffuse
				m->setShininess(atof(value->value()));
			}

			// Set material
			c->setMaterial(m);
        }

        // Read Scripts
        XmlNode * scripts = cubeNode->first_node("scripts");
        readScripts(scene, c, scripts);

        // Get next cube
        cubeNode = cubeNode->next_sibling("cube");
    }
}

void XmlParser::readMeshs(Renderer * renderer ,Scene * scene, XmlNode * meshNode) {
    // Read mesh
    while(meshNode) {
        // Create cube
        glm::mat4 id;
        MeshObject * mesh = new MeshObject(id);
        // Read shaders and init it
        int basicShader = renderer->getDefaultBasicShader();
        int lightShader = renderer->getDefaultLightShader();
        XmlNode * basicShaderNode = meshNode->first_node("basic-shader");
        XmlNode * lightShaderNode = meshNode->first_node("light-shader");
        
        // If there is a basic shader node
        if (basicShaderNode) {
            XmlAttr * vert, * frag;
            xmlAttribute(vert, basicShaderNode);
            xmlAttribute(frag, basicShaderNode);
            // Create new shader
            basicShader = renderer->createShaderProg(vert->value(), frag->value());
        }

        // If there is a light shader node
        if (lightShaderNode) {
            XmlAttr * vert, * frag;
            xmlAttribute(vert, lightShaderNode);
            xmlAttribute(frag, lightShaderNode);
            // Create new shader
            lightShader = renderer->createShaderProg(vert->value(), frag->value());
        }

        // Init mesh
        mesh->init(basicShader, lightShader);

        // Load file (it must exist)
        XmlAttr * file;
        xmlAttribute(file, meshNode);
        mesh->loadFromFile(file->value());
        
        // Try to read model
        readModel(mesh, meshNode->first_node("model"));

        // Add c to scene
        scene->addGameObject(mesh);

        // Set texture
        XmlNode * textureNode = meshNode->first_node("texture");
        if (textureNode) {
            // Get file name
            XmlAttr * file;
            xmlAttribute(file, textureNode);

            // Create texture
            Texture texture(GL_TEXTURE0, std::string(file->value()));

            // Set texture
            mesh->setTexture(texture);
        }

        // Set normal map
        XmlNode * normalMapNode = meshNode->first_node("normal-map");
        if (normalMapNode) {
            // Get file name
            XmlAttr * file;
            xmlAttribute(file, normalMapNode);

            // Create texture
            Texture normalMap(GL_TEXTURE2, std::string(file->value()));

            // Set texture
            mesh->setNormalMap(normalMap);
        }

        // Set material
		XmlNode * material = meshNode->first_node("material");
		if (material) {
			// Create material
			Material * m = new Material();

			// Set emissive
			XmlNode * emissive = material->first_node("emissive");
			if (emissive) {
				XmlAttr * r, *g, *b;
				xmlAttribute(r, emissive);
				xmlAttribute(g, emissive);
				xmlAttribute(b, emissive);
				// Set diffuse
				m->setEmissive(atof(r->value()), atof(g->value()),
						atof(b->value()));
			}

			// Read ambient
			XmlNode * ambient = material->first_node("ambient");
			if (ambient) {
				XmlAttr * r, *g, *b;
				xmlAttribute(r, ambient);
				xmlAttribute(g, ambient);
				xmlAttribute(b, ambient);
				// Set diffuse
				m->setAmbient(atof(r->value()), atof(g->value()),
						atof(b->value()));
			}

			// Read diffuse
			XmlNode * diffuse = material->first_node("diffuse");
			if (diffuse) {
				XmlAttr * r, *g, *b;
				xmlAttribute(r, diffuse);
				xmlAttribute(g, diffuse);
				xmlAttribute(b, diffuse);
				// Set diffuse
				m->setDiffuse(atof(r->value()), atof(g->value()),
						atof(b->value()));
			}

			// Read specular
			XmlNode * specular = material->first_node("specular");
			if (specular) {
				XmlAttr * r, *g, *b;
				xmlAttribute(r, specular);
				xmlAttribute(g, specular);
				xmlAttribute(b, specular);
				// Set diffuse
				m->setSpecular(atof(r->value()), atof(g->value()),
						atof(b->value()));
			}

			// Read specular
			XmlNode * shininess = material->first_node("shininess");
			if (shininess) {
				XmlAttr * value;
				xmlAttribute(value, shininess);
				// Set diffuse
				m->setShininess(atof(value->value()));
			}

			// Set material
			mesh->setMaterial(m);
		}

        // Read Scripts
        XmlNode * scripts = meshNode->first_node("scripts");
        readScripts(scene, mesh, scripts);

        // Get next cube
        meshNode = meshNode->next_sibling("mesh");
    }
}

void XmlParser::readLights(Scene * scene, XmlNode * lights) {
    if (lights) {
        // Read all lights from scene
        XmlNode * light = lights->first_node("light");
        while (light) {
            // Create light
            Light * l = new Light();

            // Get position from xml
            XmlNode * position;
            xmlElement(position, light);
            // Get coordinates
            XmlAttr * x, * y, * z;
            xmlAttribute(x, position);
            xmlAttribute(y, position);
            xmlAttribute(z, position);
            // Set position
            l->setPosition(atof(x->value()), atof(y->value()), atof(z->value()));

            // Get direction from xml
			XmlNode * direction;
			xmlElement(direction, light);
			// Get coordinates
			xmlAttribute(x, direction);
			xmlAttribute(y, direction);
			xmlAttribute(z, direction);
			// Set position
			l->setDirection(atof(x->value()), atof(y->value()), atof(z->value()));

			// Check angle
			XmlNode * angle = light->first_node("angle");
			if (angle) {
				XmlAttr * value;
				xmlAttribute(value, angle);
				l->setLightAngle(atof(value->value()));
			}

            // Try to get diffuse
            XmlNode * diffuse = light->first_node("diffuse");
            if (diffuse) {
                // Get coordinates
                XmlAttr * r, * g, * b;
                xmlAttribute(r, diffuse);
                xmlAttribute(g, diffuse);
                xmlAttribute(b, diffuse);
                // Set diffuse
                l->setDiffuse(atof(r->value()), atof(g->value()), atof(b->value()));
            }

            // Try to get specular
            XmlNode * specular = light->first_node("specular");
            if (specular) {
                // Get coordinates
                XmlAttr * r, * g, * b;
                xmlAttribute(r, specular);
                xmlAttribute(g, specular);
                xmlAttribute(b, specular);
                // Set specular
                l->setSpecular(atof(r->value()), atof(g->value()), atof(b->value()));
            }

            // Read scripts
            XmlNode * scripts = light->first_node("scripts");
            readScripts(scene, l, scripts);

            // Add to scene
            scene->addLight(l);

            // Get next light
            light = light->next_sibling("light");
        }
    } else {
        printf("No lights\n");
    }
}

void XmlParser::readScripts(Scene * scene, Transform * transform, XmlNode * scripts) {
    if (scripts) {
        // Get each script
        XmlNode * script = scripts->first_node("script");
        while (script) {
            XmlAttr * name;
            xmlAttribute(name, script);

            // Crate script and bind it to transform
            scene->addScript(Script::GetScriptByName(transform, name->value()));

            // Get next script
            script = script->next_sibling("script");
        }
    }
}


void XmlParser::readModel(Transform * transform, XmlNode * modelNode) {
    // Create model
    glm::mat4 model;

    if (modelNode) {
        // Get each transformation
        XmlNode * transformNode = modelNode->first_node();

        while (transformNode) {
            std::string type(transformNode->name());

            // Translate
            if (type == "translate") {
                XmlAttr * x, * y, * z;
                xmlAttribute(x, transformNode);
                xmlAttribute(y, transformNode);
                xmlAttribute(z, transformNode);

                // Apply transformation
                transform->translate(atof(x->value()), atof(y->value()), atof(z->value()));
            }
            // Rotate
            else if (type == "rotate") {
                // Read angle
                XmlAttr * angle;
                xmlAttribute(angle, transformNode);

                // Check normal
                glm::vec3 normal(0, 1, 0);
                XmlNode * normalNode = transformNode->first_node("normal");
                if (normalNode) {
                    XmlAttr * x, * y, * z;
                    xmlAttribute(x, normalNode);
                    xmlAttribute(y, normalNode);
                    xmlAttribute(z, normalNode);

                    // Set new normal
                    normal = glm::vec3(atof(x->value()), atof(y->value()), atof(z->value()));
                }

                // Apply transformation
                transform->rotate(atof(angle->value()), normal);
            }
            // Scale
            else if (type == "scale") {
                XmlAttr * x, * y, * z;
                xmlAttribute(x, transformNode);
                xmlAttribute(y, transformNode);
                xmlAttribute(z, transformNode);

                // Apply transformation
                transform->scale(atof(x->value()), atof(y->value()), atof(z->value()));
            }

            // Get next transformation
            transformNode = transformNode->next_sibling();
        }
    }
}



// Camera * XmlParser::xmlToCamera(XmlNode * camera) {
//  Camera * c = new Camera();
//  REAL x, y, z;

//  //parametros obrigatorios
//  XmlNode * position, *to, *up;
//  xmlElement(position, camera);
//  xmlElement(to, camera);
//  xmlElement(up, camera);
//  {
//      sscanf(position->value(), "%lf %lf %lf", &x, &y, &z);
//      c->setPosition(Vec3(x, y, z));
//      sscanf(to->value(), "%lf %lf %lf", &x, &y, &z);
//      c->setDirectionOfProjection((Vec3(x, y, z) - c->getPosition()));
//      sscanf(up->value(), "%lf %lf %lf", &x, &y, &z);
//      c->setViewUp(Vec3(x, y, z));
//  }

//  XmlNode * angle = camera->first_node("angle");
//  if (angle) {
//      sscanf(angle->value(), "%lf", &x);
//      c->setViewAngle(x);
//      c->setHeight(2 * tan(Math::toRadians<REAL>(x * 0.5)) * c->getDistance());
//  } else {
//      c->setViewAngle(90);
//      c->setHeight(20);
//  }

//  XmlNode * aspect = camera->first_node("aspect");
//  if (aspect) {
//      sscanf(aspect->value(), "%lf:%lf", &x, &y);
//      c->setAspectRatio(x / y);
//  } else {
//      c->setAspectRatio(1);
//  }

//  XmlNode * projection = camera->first_node("projection");
//  if (projection) {
//      if (strcmp(projection->value(), "perspective") == 0)
//          c->setProjectionType(Camera::Perspective);
//      else
//          c->setProjectionType(Camera::Parallel);
//  } else {
//      c->setProjectionType(Camera::Perspective);
//  }
//  return c;
// }

// Scene * XmlParser::xmlToScene(XmlNode * scene) {
//  Scene * s;
//  char * nameScene = (scene->first_attribute("name")) ? scene->first_attribute("name")->value() : 0;
//  s = (nameScene != 0) ? new Scene(nameScene) : Scene::New();

//  REAL x, y, z;

//  XmlNode * background;
//  xmlElement(background, scene);
//  {
//      sscanf(background->value(), "%lf %lf %lf", &x, &y, &z);
//      s->backgroundColor = Color(x, y, z);
//  }

//  XmlNode * ambient;
//  xmlElement(ambient, scene);
//  {
//      sscanf(ambient->value(), "%lf %lf %lf", &x, &y, &z);
//      s->ambientLight = Color(x, y, z);
//  }

//  XmlNode * mesh = scene->first_node("mesh");
//  while (mesh) {
//      XmlAttr * file;
//      xmlAttribute(file, mesh);

//      TriangleMeshShape * shape = MeshSweeper::loadFromObj(file->value());
//      xmlCheckTransMaterials(shape, mesh);

//      s->addActor(new Actor(*shape));

//      mesh = mesh->next_sibling("mesh");
//  }

//  mesh = scene->first_node("sphere");
//  while (mesh) {
//      TriangleMeshShape * shape = xmlToSphere(mesh);

//      s->addActor(new Actor(*shape));

//      mesh = mesh->next_sibling("sphere");
//  }

//  mesh = scene->first_node("box");
//  while (mesh) {
//      TriangleMeshShape * shape = xmlToBox(mesh);

//      s->addActor(new Actor(*shape));

//      mesh = mesh->next_sibling("box");
//  }

//  mesh = scene->first_node("cone");
//  while (mesh) {
//      TriangleMeshShape * shape = xmlToCone(mesh);

//      s->addActor(new Actor(*shape));

//      mesh = mesh->next_sibling("cone");
//  }

//  mesh = scene->first_node("cylinder");
//  while (mesh) {
//      TriangleMeshShape * shape = xmlToCylinder(mesh);

//      s->addActor(new Actor(*shape));

//      mesh = mesh->next_sibling("cylinder");
//  }

//  mesh = scene->first_node("screw");
//  while (mesh) {
//      TriangleMeshShape * shape = xmlToScrew(mesh);

//      s->addActor(new Actor(*shape));

//      mesh = mesh->next_sibling("screw");
//  }

//  XmlNode *light = scene->first_node("light");
//  while (light) {
//      s->addLight(xmlToLight(light));

//      light = light->next_sibling("light");
//  }

//  return s;
// }

// Light * XmlParser::xmlToLight(XmlNode * light) {
//  Vec3 pos;
//  Color color = Color::white;
//  int falloff = 0;
//  int pontual = 1;

//  XmlNode * position;
//  xmlElement(position, light);
//  sscanf(position->value(), "%lf %lf %lf %d", &pos.x, &pos.y, &pos.z, &pontual);

//  XmlNode * node = light->first_node("color");
//  if (node)
//      sscanf(node->value(), "%lf %lf %lf", &color.r, &color.g, &color.b);

//  node = light->first_node("falloff");
//  if (node)
//      falloff = atoi(node->value());

//  Light * l = new Light(pos, color);
//  switch (falloff) {
//      case 1:
//          l->flags.enable(Light::Squared, false);
//          l->flags.enable(Light::Linear, true);
//          break;
//      case 2:
//          l->flags.enable(Light::Squared, true);
//          l->flags.enable(Light::Linear, false);
//          break;
//      default:
//          l->flags.enable(Light::Squared, false);
//          l->flags.enable(Light::Linear, false);

//  }
//  l->setDirectional((pontual) ? false : true);

//  return l;
// }

// void XmlParser::xmlCheckTransMaterials(TriangleMeshShape * shape, XmlNode * mesh) {
//  XmlNode * node;
//  node = mesh->first_node("transform");
//  if (node) {
//      shape->transform(xmlToTransf(node));
//  }

//  node = mesh->first_node("material");
//  if (node) {
//      shape->setMaterial(*xmlToMaterial(node));
//  }
// }

// TriangleMeshShape* XmlParser::xmlToSphere(XmlNode * sphere) {
//  Vec3 cen;
//  REAL rad;
//  int mer;

//  TriangleMeshShape * shape;
//  XmlNode * center, *radius, *meridians;

//  xmlElement(center, sphere);
//  xmlElement(radius, sphere);

//  sscanf(center->value(), "%lf %lf %lf", &cen.x, &cen.y, &cen.z);
//  rad = atof(radius->value());

//  meridians = sphere->first_node("meridians");
//  if (meridians) {
//      mer = atoi(meridians->value());

//      shape = MeshSweeper::makeSphere(cen, rad, mer);
//  } else {
//      shape = MeshSweeper::makeSphere(cen, rad);
//  }

//  xmlCheckTransMaterials(shape, sphere);

//  return shape;
// }

// TriangleMeshShape* XmlParser::xmlToCone(XmlNode * cone) {
//  Vec3 cen(0, 0, 0);
//  Vec3 norm(0, 1, 0);
//  REAL rad = 1;
//  REAL heig = 2;
//  int seg = 16;

//  TriangleMeshShape * shape;
//  XmlNode * node;

//  node = cone->first_node("center");
//  if (node)
//      sscanf(node->value(), "%lf %lf %lf", &cen.x, &cen.y, &cen.z);

//  node = cone->first_node("normal");
//  if (node)
//      sscanf(node->value(), "%lf %lf %lf", &norm.x, &norm.y, &norm.z);

//  node = cone->first_node("radius");
//  if (node)
//      rad = atof(node->value());

//  node = cone->first_node("height");
//  if (node)
//      heig = atof(node->value());

//  node = cone->first_node("segments");
//  if (node)
//      seg = atoi(node->value());

//  cen.print("Center: ", stdout);
//  norm.print("Normal: ", stdout);
//  printf("%lf %lf %d\n", rad, heig, seg);

//  shape = MeshSweeper::makeCone(cen, norm, rad, heig, seg);
//  xmlCheckTransMaterials(shape, cone);
//  return shape;
// }

// TriangleMeshShape* XmlParser::xmlToCylinder(XmlNode * cylinder) {
//  Vec3 cen(0, 0, 0);
//  Vec3 norm(0, 1, 0);
//  REAL rad = 1;
//  REAL heig = 2;
//  int seg = 16;

//  TriangleMeshShape * shape;
//  XmlNode * node;

//  node = cylinder->first_node("center");
//  if (node)
//      sscanf(node->value(), "%lf %lf %lf", &cen.x, &cen.y, &cen.z);

//  node = cylinder->first_node("normal");
//  if (node)
//      sscanf(node->value(), "%lf %lf %lf", &norm.x, &norm.y, &norm.z);

//  node = cylinder->first_node("radius");
//  if (node)
//      rad = atof(node->value());

//  node = cylinder->first_node("height");
//  if (node)
//      heig = atof(node->value());

//  node = cylinder->first_node("segments");
//  if (node)
//      seg = atoi(node->value());

//  shape = MeshSweeper::makeCylinder(cen, rad, norm, heig, seg);
//  xmlCheckTransMaterials(shape, cylinder);
//  return shape;
// }

// TriangleMeshShape* XmlParser::xmlToScrew(XmlNode * screw) {
//  Vec3 cen(2, 0, 0);
//  Vec3 first(1, 0, 0);
//  Vec3 norm(0, 0, 1);
//  REAL distance = 1;
//  REAL angle = 360;
//  REAL delta = 0;
//  REAL heig = 2;
//  int steps = 36;

//  TriangleMeshShape * shape;
//  XmlNode * node;

//  node = screw->first_node("center");
//  if (node)
//      sscanf(node->value(), "%lf %lf %lf", &cen.x, &cen.y, &cen.z);

//  node = screw->first_node("first");
//  if (node)
//      sscanf(node->value(), "%lf %lf %lf", &first.x, &first.y, &first.z);

//  node = screw->first_node("normal");
//  if (node)
//      sscanf(node->value(), "%lf %lf %lf", &norm.x, &norm.y, &norm.z);

//  node = screw->first_node("distance");
//  if (node)
//      distance = atof(node->value());

//  node = screw->first_node("angle");
//  if (node)
//      angle = atof(node->value());

//  node = screw->first_node("delta");
//  if (node)
//      delta = atof(node->value());

//  node = screw->first_node("height");
//  if (node)
//      heig = atof(node->value());

//  node = screw->first_node("steps");
//  if (node)
//      steps = atoi(node->value());

//  shape = MeshSweeper::makeScrew(cen, first, norm, distance, angle, heig, delta, steps);
//  xmlCheckTransMaterials(shape, screw);
//  return shape;
// }

// TriangleMeshShape* XmlParser::xmlToBox(XmlNode * box) {
//  Vec3 cen(0, 0, 0);
//  Vec3 ori(0, 0, 0);
//  Vec3 sca(1, 1, 1);

//  TriangleMeshShape * shape;
//  XmlNode * node;

//  node = box->first_node("center");
//  if (node)
//      sscanf(node->value(), "%lf %lf %lf", &cen.x, &cen.y, &cen.z);

//  node = box->first_node("orientation");
//  if (node)
//      sscanf(node->value(), "%lf %lf %lf", &ori.x, &ori.y, &ori.z);

//  node = box->first_node("scale");
//  if (node)
//      sscanf(node->value(), "%lf %lf %lf", &sca.x, &sca.y, &sca.z);

//  shape = MeshSweeper::makeBox(cen, ori, sca);
//  xmlCheckTransMaterials(shape, box);
//  return shape;
// }

// Transf3 XmlParser::xmlToTransf(XmlNode * transformations) {
//  XmlNode * transformation = transformations->first_node();

//  Transf3 t;
//  t.identity();
//  while (transformation) {
//      char * type = transformation->name();

//      if (strcmp(type, "scale") == 0) {
//          Transf3 temp;
//          temp.scale(atof(transformation->value()));
//          t.compose(temp);
//      } else if (strcmp(type, "rotation") == 0) {
//          XmlNode * axis, *angle;
//          xmlElement(axis, transformation);
//          xmlElement(angle, transformation);

//          Vec3 vec;
//          sscanf(axis->value(), "%lf %lf %lf", &vec.x, &vec.y, &vec.z);

//          Transf3 temp;
//          temp.rotation(vec, Math::toRadians<REAL>(atof(angle->value())));
//          t.compose(temp);
//      } else if (strcmp(type, "translation") == 0) {
//          Transf3 temp;

//          Vec3 vec;
//          sscanf(transformation->value(), "%lf %lf %lf", &vec.x, &vec.y, &vec.z);
//          temp.translation(vec);
//          t.compose(temp);
//      }
//      transformation = transformation->next_sibling();
//  }
//  return t;
// }

// Material* XmlParser::xmlToMaterial(XmlNode * material) {
//  Material * m = MaterialFactory::New();

//  XmlNode * node;
//  Color color;

//  node = material->first_node("ambient");
//  if (node) {
//      sscanf(node->value(), "%lf %lf %lf", &color.r, &color.g, &color.b);
//      m->surface.ambient = color;
//  }

//  node = material->first_node("diffuse");
//  if (node) {
//      sscanf(node->value(), "%lf %lf %lf", &color.r, &color.g, &color.b);
//      m->surface.diffuse = color;
//  }

//  node = material->first_node("spot");
//  if (node) {
//      sscanf(node->value(), "%lf %lf %lf", &color.r, &color.g, &color.b);
//      m->surface.spot = color;
//  }

//  node = material->first_node("spot");
//  if (node) {
//      m->surface.shine = atof(node->value());
//  }

//  node = material->first_node("specular");
//  if (node) {
//      sscanf(node->value(), "%lf %lf %lf", &color.r, &color.g, &color.b);
//      m->surface.specular = color;
//  }
//  return m;
// }
