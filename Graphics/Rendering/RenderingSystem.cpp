#include "RenderingSystem.h"

#include "../Communication/Message.h"
#include "../Communication/MessageProcessor.h"
#include "../Communication/DeliverySystem.h"

#include "../Communication/Message.h"

#include <queue>
#include "../../Communication/Messages/PlayerInputMessage.h"

RenderingSystem::RenderingSystem(Window* window, Camera* camera, Vector2 resolution)
	: Subsystem("RenderingSystem")
{
	renderer = std::make_unique<Renderer>(window, camera, resolution);

	std::vector<MessageType> types = { MessageType::DUMMY_TYPE, MessageType::PLAYER_INPUT };

	incomingMessages = MessageProcessor(types, DeliverySystem::getPostman()->getDeliveryPoint("RenderingSystem"));

	incomingMessages.addActionToExecuteOnMessage(MessageType::DUMMY_TYPE, [](Message* message)
	{
		std::cout << "Renderer recieved dummy message" << std::endl;
	});

	incomingMessages.addActionToExecuteOnMessage(MessageType::PLAYER_INPUT, [](Message* message)
	{
		PlayerInputMessage* playerMessage = static_cast<PlayerInputMessage*>(message);
		std::cout << "Player : " << playerMessage->player->getPlayerID() << std::endl;
		std::cout << "key : " << playerMessage->data.key << std::endl;
		std::cout << "State : " << playerMessage->data.currentState << std::endl;
	});
}

RenderingSystem::~RenderingSystem()
{
}

void RenderingSystem::SetSceneToRender(SceneManager* scene)
{
	renderer->initialise(scene);
}

void RenderingSystem::updateSubsystem(const float& deltaTime)
{
	renderer->update(deltaTime);
}

bool RenderingSystem::stob(std::string string)
{
	std::transform(string.begin(), string.end(), string.begin(), ::tolower);
	std::istringstream is(string);
	bool b;
	is >> std::boolalpha >> b;
	return b;
}

void RenderingSystem::getGraphicsConfig()
{
	graphicsconfigParser.loadFile("../Data/Resources/Config/Graphics/graphicsConfigXML.xml");
	Node* node = graphicsconfigParser.parsedXml;
	graphicsConfig.resolution.x = stof(node->children[0]->children[0]->value);
	graphicsConfig.resolution.y = stof(node->children[0]->children[1]->value);
	graphicsConfig.basicgeometryEnabled = stob(node->children[1]->value);
	graphicsConfig.bloomEnabled = stob(node->children[2]->value);
	graphicsConfig.lightingEnabled = stob(node->children[3]->value);
	graphicsConfig.bplightingEnabled = stob(node->children[4]->value);
	graphicsConfig.gbufferEnabled = stob(node->children[5]->value);
	graphicsConfig.motionblurEnabled = stob(node->children[6]->value);
	graphicsConfig.particlesEnabled = stob(node->children[7]->value);
	graphicsConfig.shadowmappingEnabled = stob(node->children[8]->value);
	graphicsConfig.skyboxEnabled = stob(node->children[9]->value);
	graphicsConfig.ssaoEnabled = stob(node->children[10]->value);
}
