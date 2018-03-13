# Game Engine

## Description
A c++ data driven game engine consisting of various subsystems including graphics, physics and multi-threading.

## How To Use
**IMPORTANT**
Please install OpenAL with Audio/OpenAL 1.1 SDK/redist/oalinst.exe.

## Level Editor and Developer's Console
### Current Limitations
* Inputs are case sensitive.
* Captials must be used with the CapsLock button, shift is not implemented.
* Divide on numpad is used for a forward slash.

### Commands
#### General Commands
1. Toggle free camera movement
`debugcamera`

2. Create a game object

`spawngameobject GameObjectName`

* gameobjectname is the resource identifier.

3. Add a scene node to a game object

`addscenenode GameObjectName MeshIdentifier colour=1,1,1,1`

*adds a scene node to an existing game object.
* MeshIdentifier is the resource name of a mesh already loaded into the resource manager.
* colour must have 4 components.
