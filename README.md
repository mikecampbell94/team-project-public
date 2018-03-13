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

* adds a scene node to an existing game object.
* MeshIdentifier is the resource name of a mesh already loaded into the resource manager.
* colour must have 4 components.

4. Add a physics node to a game object

`addphysicsnode GameObjectName shape=Box invMass=0.5 static=true`

* shape is the collision shape of the physics node. It must be either 'Box' or 'Sphere'.
* static will dictate whether it is moveable or not. Static nodes will not collide with eachother. Must be either 'true' or 'false'.
* **IMPORTANT. A SCALE MUST BE SET FOR A GAMEOBJECT BEFORE ADDING A PHYSICS NODE**

5. Load a mesh into the resource manager. 

`loadmesh NewMeshIdentifier mesh=.../Directory/Example/someMesh.obj`
`loadmesh NewMeshIdentifier mesh=.../Directory/Example/someMesh.obj texture=../Directory/AnotherExample/someTexture.png`

* NewMeshIdentifier is the meshes identifier to be used for adding it to a game object.
* A texture is optional.
* **WARNING MAY BE SLOW DEPENDING ON SIZE OF THE MESH**

6.

