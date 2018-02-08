#include "XMLParser.h"
#include <fstream>


XMLParser::XMLParser()
{
}


XMLParser::~XMLParser()
{
}



void XMLParser::loadFile(std::string filename)
{
	std::ifstream inFile(filename);

	if (!inFile) 
	{
		throw "Could not load File" + filename;
	}

	std::string xmlDump;

	std::string line;
	while (std::getline(inFile, line)) 
	{
		xmlDump += line;
	}
	std::vector<char> xmlData;
	xmlData = std::vector<char>(xmlDump.begin(), xmlDump.end());
	xmlData.push_back('\0');
	rapidxml::xml_document<> doc;
	doc.parse<rapidxml::parse_no_data_nodes>(&xmlData[0]);
	rapidxml::xml_node<>* firstNode = doc.first_node();
	


	recursivelyParse(firstNode, &parsedXml);
	
	

}

//PLEASE DO NOT TOUCH
void XMLParser::recursivelyParse(rapidxml::xml_node<>* unParsedNode, node** parsedNode)
{
	if (unParsedNode) 
	{
		//store name, type, value if any
		(*parsedNode)->nodeType = unParsedNode->name();
		(*parsedNode)->name = unParsedNode->first_attribute()->value();
		(*parsedNode)->value = unParsedNode->value();
		for(unParsedNode = unParsedNode->first_node(); unParsedNode != nullptr; unParsedNode = unParsedNode->next_sibling())
		{
			node* child = NULL;
			(*parsedNode)->children.push_back(child);
			recursivelyParse(unParsedNode, &child);
		}
			
			//(*parsedNode)->children.push_back(recursivelyParse(currentNode->first_node()));
	}
	else 
	{
		
		

	}
}
