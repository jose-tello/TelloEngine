TelloEngine is a vidiogame 3d engine, with a build in shader editor. Made by [Jose L. Redondo Tello](https://www.linkedin.com/in/jose-lu%C3%ADs-redondo-tello-44918b19b/).																			![Jose](https://raw.githubusercontent.com/x-mat-studio/HeartsOfGreed/Wiki/Welcome%20Page/Team%20Portraits/josePortrait.png) 

This project is made as a 3rd year assigment in Bachelor’s Degree in Video Games by UPC at CITM, Terrassa, under the supervision of lecturer [Marc Garrigó](www.linkedin.com/in/mgarrigo/). 



## Core sub-systems

#### Game Objects

·Drag and drop to reorder game objects in the hierarchy window.

·Delete game objects by pressing delete on the focused game object (while focusing the hierarchy window).

·Create default primitives, cameras (that will render to another window), and lights (now, only one light can affect the scene, this will be improved on a next release).



#### Components

**·Transform:** Objects can be moved / scaled / rotated using the inspector or using the pivot and the mouse.



**·Mesh:** 

-Can be picked using the mouse to select a game object.

-Debug display of vertex and face normals.

-Debug wire frame mode.



**·Material:** 

-Contains the color, texture and shader.

-The texture and the shader can be changed by dragging the texture to the engine window or using the load window.

-The shader can be edited inside the engine using the text editor, that can be opened by pressing the edit shader button in the inspector.

-Shader uniforms can be set from the inspector.



**·Camera:** 

-Creates a virtual camera, which is renders to another window.

-All cameras perform frustum culling.



**·Light source:** Sends its position to the default shader, where the color that will be drawn to the screen is calculated.



#### Resource manager

All the resources of assets are loaded into the folder Library.

All resources can be modified, added or deleted in runtime (must press the update state button in the load window to update the changes).

All resources use reference counting, and are only charged once.

Can drag and drop assets, and they will be loaded by the engine.



#### Shader pipeline

