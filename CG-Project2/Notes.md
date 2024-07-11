# Notes

 - [X] Put the camera in an init file. This file creates a list of variables used by all project (maybe fixed)
 - [ ] The camera doesn't zoom
 - [ ] Shaders are not copied right in the IGEntity panel
 - [ ] Use material::NONE to check if the entity has a material
 - [X] Apparently it doesn't send the model matrix to the shader I don't know why (apparently the problem was the view matrix)
 - [ ] Modify scene system: try using an unordered map<Shader, vector<Entity*>>, use a shader and go through all the entities using that shader.
 - [ ] General code refactoring
 - [ ] Make a scene for all IGMenus
