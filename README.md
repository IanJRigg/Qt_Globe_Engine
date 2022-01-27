# Qt Globe Engine
![Image of the application running](https://github.com/IanJRigg/Qt_Globe_Engine/blob/master/images/screenshot.PNG)

## Brief
This is conversion of my OpenGL Globe Engine to use Qt Version 6 instead of GLFW. The Globe is rendered using the subdivided cube algorithm, with a transformed equirectangular image from the Blue Marble dataset rendered as a cubemap texture. In order to make the conversion, the application had to be re-written from the ground up due to differences between Qt and GLFW. That being said, the grand majority of OpenGL function calls remain the same.

## Features
The engine allows users to rotate around the earth using the arrow keys and also allows the user to zoom in and out via the mouse wheel. The globe can be rendered as a wireframe via a checkable menu item in the "Edit" menu, and the application can be closed via the X button or from the quit item in the "File" menu.

## Future Considerations
- The perspective camera is the wrong camera to be using for an application with a single globe. Ideally the camera class would be refactored to use an orthographic projection matrix.
- When zooming in, the level of detail remains the same. Multiple references on Globe Engine design have mentioned the need for a QuadTree implementation to allow for changes to the texture quality as one zooms in.
- The textures, as large as they are, rapidly show their lack of detail when zooming in. Adding a set of more detailed textures would go a long way to fix this issue.

## Cubemap Textures
Due to the size of the cubemap face textures, I opted to not include them in this application. in order for the application to work, six textures need to be added to the project in a textures/ folder. These textures align to the following names: asia.png, americas.png, arctic.png, antarctica.png, africa.png, and pacific.png. 