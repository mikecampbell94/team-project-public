#pragma once

#include <vector>

#include "GraphicsModule.h"

class GraphicsPipeline
{
public:
	GraphicsPipeline();
	~GraphicsPipeline();

	void toggleModule(std::string moduleIdentifier);
	void toggleModule(std::string moduleIdentifier, bool enabled);

	void intialisePipeline();
	void executeModules();

	void addModule(GraphicsModule* module);
	GraphicsModule* getGraphicsModule(std::string moduleIdentifier);

private:
	std::vector<GraphicsModule*> modules;

};