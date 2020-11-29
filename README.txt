First release of an engine made for a university project.

Link to the repository: https://github.com/jose-tello/TelloEngine

Made by:
	-Jose L. Redondo Tello, guithub account: jose-tello

Usage:
-Camera movement:
	Right click to enable navigation camara controls (WASD to move and mouse motion to rotate camera).
	Press SHIFT to move the camara faster.
	Use the mouse wheel to zoom in an out.
	Press the mouse wheel to move sideways.
	Press F to look at the currentlly focused object.
	Right click + ALT enables orvital camera movement.
	
-The hierarchy window will show all the gameObjects in the scene. When clicking one, the inspector window will open, showing 
the selected gameObject's components. 
	·You can press backspace to delete the selected game objects and its childs.
	·You can drag and drop game objects to change their hierarchy.

-You select any game object using left click;

-You can modify game object propertyes in the inspector or using the gizmos (in the inspector the button add lets you add an empty child).

-The toolbar has submenus that allow to create primitives, create cameras, create empty game objects, load/delete resources and assets, 
load/save the scene, start/pause/stop/continue the game simulation, show/hide editor windows, links about the project and an exit button.

-To see the reference counting, go to the window Application state and open the reference conting section.

-To see the debug camera options, go to the window Application state and open the camera option
	Warning, debug frustum cull onlly shows the cull of the lowest camera in the hierarchy (if there are no scene cameras, it will show nothing).

-Game simulation can be started, stopped, paused stepped one frame and continued (time can be seen in the Application state window).

-The console window outputs information about the application, loaded meshes and textures state.

-You can drag and drop fbx files, wich will be loaded with its respective resource (from any part of the hard drive).

-You can drag and drop png and dds diles, wich will be added as material component to the selected game objects, 
or will change the texture of the material if the game object allready has one. 

-Load window: Can charge any asset/resource into the scene by double clicking it, or delete it using backspace. To update the state, press the button update (add new resources, or change them).

Additional features:
-Scene cameras are rendered in their own window (as they have the same name, if there are more than one camera it will be rendered in the same window but you will have to scroll down to see it
this is due to ImGui limitations os pushing ids).