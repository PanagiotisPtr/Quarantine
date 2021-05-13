# Quarantine
A 3D game made with OpenGL and C++

## Game Description
In this very basic game you need to place boxes around the sphere/player to protect it from getting infected with Covid. Take advantage of the trees in the environment
to use as few boxes as possible. 

## Making new levels
If you would like to create more levels, you can easily do so by adding more files in the `assets/levels` folder. 

## Useful concepts about the code
The code uses an event bus on which all events are registered. Instead of listening for GLFW events, all events go on the event bus and the user can register listeners/handlers on each event.
Also there are a number of objects and object structures. Those allow for easy modification and creation of new models. All setup is abstracted away by the base classes so all you
have to do is describe the structure of the object.

## Animations
An animation library is also included in this piece of code that makes it very easy to create keyframe and animate between them.

Everything is written in plain C++ and was compiled with Visual Studio. The only libraries included are glfw and glm which are already inside the repository.
