#pragma once

#include "../Extrernal Libs/rapidxml-1.13/rapidxml.hpp"

#include <string>
#include <vector>

struct node
{
	std::string nodeType;
	std::string name;
	std::string value;
	std::vector<node*> children;
};

class XMLParser
{
public:
	XMLParser();
	~XMLParser();

	std::string loadFile(std::string filename);

	void recursivelyParse(rapidxml::xml_node<>* unParsedXml, node** parsedNode);
	void deleteAllNodes(node* currentNode);

	node* parsedXml;
};

