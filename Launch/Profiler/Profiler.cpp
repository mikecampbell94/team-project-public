#include "Profiler.h"

#include "../Resource Management/Database/Database.h"
#include "../Utilities/GameTimer.h"
#include "../Input/Devices/Keyboard.h"
#include "FPSCounter.h"

const float Y_OFFSET = 320.0f;
const float NEXT_LINE_OFFSET = -12.9f;
const Vector3 TEXT_SIZE = Vector3(12.9f, 12.9f, 12.9f);
const Vector3 TEXT_COLOUR = Vector3(0, 1, 0);

Profiler::Profiler(Keyboard* keyboard, Database* database, FPSCounter* fpsCounter) : Subsystem("Profiler")
{
	this->fpsCounter = fpsCounter;
	this->database = database;
	this->keyboard = keyboard;

	incomingMessages = MessageProcessor::MessageProcessor(std::vector<MessageType>{}, DeliverySystem::getPostman()->getDeliveryPoint("Profiler"));

	memoryWatcher = MemoryWatcher(database->MaxSize(), database);
}

void Profiler::updateSubsystem(const float& deltatime)
{
	fpsCounter->calculateFPS(deltatime);

	if (keyboard->keyTriggered(KEYBOARD_F5))
	{
		profilerEnabled = !profilerEnabled;
		depth = 0;
	}

	if (keyboard->keyTriggered(KEYBOARD_F6))
	{
		++depth;
	}

	if (profilerEnabled)
	{
		++numFramesUpdated;

		if (numFramesUpdated >= 30)
		{
			numFramesUpdated = 0;
			messages.clear();

			memoryWatcher.Update();
			updateProfiling();
		}

		displayChildTimers();
	}
}

void Profiler::addSubsystemTimer(string name, GameTimer* timer)
{
	timers.insert({ name, timer });
}

void Profiler::updateProfiling()
{
	nextLine = Y_OFFSET;

	messages.push_back(TextMeshMessage("RenderingSystem", "FPS : " + std::to_string(fpsCounter->fps),
		Vector3(-500.0f, nextLine, 0), TEXT_SIZE, TEXT_COLOUR, true, true));
	nextLine += NEXT_LINE_OFFSET;

	messages.push_back(TextMeshMessage("RenderingSystem", "Memory Usage : " + std::to_string(memoryWatcher.percent),
		Vector3(-500.0f, nextLine, 0), TEXT_SIZE, TEXT_COLOUR, true, true));
	nextLine += NEXT_LINE_OFFSET;

	for (std::pair<std::string, GameTimer*> subsystemTimer : timers)
	{
		nextLine += NEXT_LINE_OFFSET;

		std::string profilerText = subsystemTimer.first +"	" + std::to_string(subsystemTimer.second->getTimeTakenForSection());
		Vector3 position(-500.0f, nextLine, 0);

		messages.push_back(TextMeshMessage("RenderingSystem", profilerText,
			position, TEXT_SIZE, TEXT_COLOUR, true, true));

		saveProfilingInfo(subsystemTimer.second, 1, -500.0f);
	}
}

void Profiler::displayChildTimers()
{
	for (TextMeshMessage& text : messages)
	{
		DeliverySystem::getPostman()->insertMessage(text);
	}
}

void Profiler::saveProfilingInfo(GameTimer* parentTimer, int currentDepth, float parentXOffset)
{
	std::vector<GameTimer*> childTimers = parentTimer->getAllChildTimers();

	float xOffset = parentXOffset + 20.0f;

	if (currentDepth <= depth)
	{
		for (GameTimer* childTimer : childTimers)
		{
			nextLine += NEXT_LINE_OFFSET;

			std::string profilerText = childTimer->getTimerName() + "	" + std::to_string(childTimer->getTimeTakenForSection());
			messages.push_back(TextMeshMessage("RenderingSystem", profilerText,
				Vector3(xOffset, nextLine, 0), TEXT_SIZE, TEXT_COLOUR, true, true));

			saveProfilingInfo(childTimer, ++currentDepth, xOffset);
		}
	}
}
