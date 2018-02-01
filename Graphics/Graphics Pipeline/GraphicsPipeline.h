#pragma once

#include <vector>

#include "GraphicsModule.h"

class GraphicsPipeline
{
public:
	GraphicsPipeline();

	~GraphicsPipeline();

	void toggleModule(std::string moduleIdentifier);

	void intialisePipeline();
	void executeModules();

	void addModule(GraphicsModule* module);

private:
	std::vector<GraphicsModule*> modules;

};