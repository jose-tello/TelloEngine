Last release of an engine made for a university project.

Link to the website: https://jose-tello.github.io/TelloEngine/
Link to the repository: https://github.com/jose-tello/TelloEngine

Made by:
	-Jose L. Redondo Tello, guithub account: jose-tello
	-Linkedin: https://www.linkedin.com/in/joseluisredondotello/
	-Portfolio: https://jose-tello.github.io/Portfolio/

Usage:
-Camera movement:
	Right click to enable navigation camara controls (WASD to move and mouse motion to rotate camera).
	Press SHIFT to move the camara faster.
	Use the mouse wheel to zoom in an out.
	Press the mouse wheel to move sideways.
	Press F to look at the currentlly focused object.
	Right click + ALT enables orvital camera movement.

-Load scene window:
	You can open the window by pressing the "File" button on the window.
	In this window the user can rename the scene, save it, or load a scene.
	Note that several demo scenes are created in order to test non euclidean spaces.
	
-The hierarchy window will show all the gameObjects in the scene. When clicking one, the inspector window will open, showing 
the selected gameObject's components. 
	·You can press backspace to delete the selected game objects and its childs.
	·You can drag and drop game objects to change their hierarchy.


-You select any game object using left click.

-You can modify game object propertyes in the inspector or using the gizmos (in the inspector the button add lets you add an empty child).

-The toolbar has submenus that allow to create primitives, create cameras, create lights, create empty game objects, load/delete resources and assets, 
load/save the scene, start/pause/stop/continue the game simulation, show/hide editor windows, links about the project and an exit button.

-To see the reference counting, go to the window Application state and open the reference conting section.

-To see the debug camera options, go to the window Application state and open the camera option
	Warning, debug frustum cull onlly shows the cull of the lowest camera in the hierarchy (if there are no scene cameras, it will show nothing).

-Game simulation can be started, stopped, paused stepped one frame and continued (time can be seen in the Application state window).

-The console window outputs information about the application, loaded meshes and textures state.

-You can drag and drop fbx files, wich will be loaded with its respective resource (from any part of the hard drive).

-You can drag and drop png and dds diles, wich will be added as a texture to the material component of the selected game object, 
or will change the texture of the material if the game object allready has one. 

-You can drag and drop shaders (txt), wich will be added as a shader to the material component of the selected game object, 
or will change the shader of the material if the game object allready has one.

-You can edit the shader uniforms form the inspector.
*Note that the build in uniforms will not be affected if they are edited.

-Shaders can be editted using a text editor by pressing the edit shader button in the inspector, and will pe updated automatically when saved (using the text editor save button).
*Note that the text editor highlighting is from outdated glsl versions.


Additional features:
-Scene cameras are rendered in their own window (as they have the same name, if there are more than one camera it will be rendered in the same window but you will have to scroll down to see it
this is due to ImGui limitations os pushing ids).

-Default shader includes basic lightning. The light of the scene can be moved arround, and the ilumination will update acordinglly.

-Assets, library and resources will not be updated unless the button "update" in the Load window is pressed;

To be noted:
Note that the streen environment game objects are using the default shader, but don't have a shader in its material, so unless you set it by drag and drop or load window, you will not be able to open it.