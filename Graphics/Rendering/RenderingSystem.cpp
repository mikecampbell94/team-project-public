#include "RenderingSystem.h"

#include "../Communication/Message.h"
#include "../Communication/MessageProcessor.h"
#include "../Communication/DeliverySystem.h"

#include "../Communication/Message.h"

#include <queue>
#include "../../Communication/Messages/PlayerInputMessage.h"
#include "../../Communication/Messages/RelativeTransformMessage.h"
#include "../Resource Management/Database/Database.h"
#include "../../Gameplay/GameObject.h"
#include "../../Communication/Messages/TextMessage.h"

RenderingSystem::RenderingSystem(Window* window, Camera* camera, Vector2 resolution)
	: Subsystem("RenderingSystem")
{
	renderer = std::make_unique<Renderer>(window, camera, resolution);
}

RenderingSystem::~RenderingSystem()
{
}

void RenderingSystem::initialise(Database* database)
{

	std::vector<MessageType> types = { MessageType::TEXT, MessageType::PLAYER_INPUT, MessageType::RELATIVE_TRANSFORM };

	incomingMessages = MessageProcessor(types, DeliverySystem::getPostman()->getDeliveryPoint("RenderingSystem"));

	incomingMessages.addActionToExecuteOnMessage(MessageType::TEXT, [](Message* message)
	{
		TextMessage* textMessage = static_cast<TextMessage*>(message);
		std::cout << textMessage->text << std::endl;
	});

	incomingMessages.addActionToExecuteOnMessage(MessageType::PLAYER_INPUT, [](Message* message)
	{
		PlayerInputMessage* playerMessage = static_cast<PlayerInputMessage*>(message);
		std::cout << "Player : " << playerMessage->player->getPlayerID() << std::endl;
		std::cout << "key : " << playerMessage->data.key << std::endl;
		std::cout << "State : " << playerMessage->data.currentState << std::endl;
	});
	Initialise();

	GameObject* gameObject = static_cast<GameObject*>(
		database->getTable("GameObjects")->getAllResources()->getResource("playerBall"));

	incomingMessages.addActionToExecuteOnMessage(MessageType::RELATIVE_TRANSFORM, [database = database](Message* message)
	{
		RelativeTransformMessage* translationMessage = static_cast<RelativeTransformMessage*>(message);
		GameObject* gameObject = static_cast<GameObject*>(
			database->getTable("GameObjects")->getAllResources()->getResource(translationMessage->resourceName));

		gameObject->getSceneNode()->SetTransform(gameObject->getSceneNode()->GetTransform()
			* translationMessage->transform);

	});
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



void RenderingSystem::Initialise()
{
	graphicsconfigParser.loadFile("../Data/Resources/Config/Graphics/graphicsConfigXML.xml");
	Node* node = graphicsconfigParser.parsedXml;
	pipeline.intialisePipeline();
	for (int i = 0; i < node->children[1]->children.size(); i++) {
		graphicsConfig.insert(std::pair<std::string, bool>(node->children[1]->children[i]->name, stob(node->children[1]->children[i]->value)));
		pipeline.toggleModule(node->children[1]->children[i]->name);
	}
	//-then in the initialise function of renderin system, get the pipeline from the renderer, and enable / disable modules if they exist


	/*graphicsConfig.resolution.y = stof(node->children[0]->children[1]->value);
	graphicsConfig.basicgeometryEnabled = stob(node->children[1]->value);
	graphicsConfig.bloomEnabled = stob(node->children[2]->value);
	graphicsConfig.lightingEnabled = stob(node->children[3]->value);
	graphicsConfig.bplightingEnabled = stob(node->children[4]->value);
	graphicsConfig.gbufferEnabled = stob(node->children[5]->value);
	graphicsConfig.motionblurEnabled = stob(node->children[6]->value);
	graphicsConfig.particlesEnabled = stob(node->children[7]->value);
	graphicsConfig.shadowmappingEnabled = stob(node->children[8]->value);
	graphicsConfig.skyboxEnabled = stob(node->children[9]->value);
	graphicsConfig.ssaoEnabled = stob(node->children[10]->value);*/
}
