#ifndef XMLPARSERSCENE_H__
#define XMLPARSERSCENE_H__

#include <Scene.h>
#include <Renderer.h>
#include <Transform.h>
#ifndef RAPIDXML_HPP_INCLUDED
#include "rapidxml.hpp"
#endif
#ifndef RAPIDXML_UTILS_HPP_INCLUDED
#include "rapidxml_utils.hpp"
#endif

using namespace rapidxml;

#define xmlElement(node, parent) \
    node = parent->first_node(#node);    \
    if (!node) {\
        printf("Missing '"#node"' into %s\n", parent->name());\
        exit(1);\
    }

#define xmlAttribute(attribute, node) \
    attribute = node->first_attribute(#attribute);    \
    if (!attribute) {\
        printf("Missing '"#attribute"' into %s\n", node->name());\
        exit(1);\
    }

typedef file<> XmlFile;
typedef xml_document<> XmlDocument;
typedef xml_node<> XmlNode;
typedef xml_attribute<> XmlAttr;

class XmlParser {
    public:
        virtual ~XmlParser();

        static void loadFromXml(const char * fileName, int * argc, char ** argv);
        static void readCubes(Renderer * renderer ,Scene * scene, XmlNode * cubeNode);
        static void readMeshs(Renderer * renderer ,Scene * scene, XmlNode * meshNode);
        static void readLights(Scene * scene, XmlNode * lights);
        static void readModel(Transform * transform, XmlNode * modelNode);
        static void readScripts(Scene * scene, Transform * transform, XmlNode * scripts);
    private:
        XmlParser();
};

#endif /* XMLPARSERSCENE_H__ */
