# Notes

 - [X] Put the camera in an init file. This file creates a list of variables used by all project (maybe fixed)
 - [X] The camera doesn't zoom
 - [X] Shaders are not copied right in the IGEntity panel
 - [ ] Use material::NONE to check if the entity has a material
 - [X] Apparently it doesn't send the model matrix to the shader I don't know why (apparently the problem was the view matrix)
 - [ ] Modify scene system: try using an unordered map<Shader, vector<Entity*>>, use a shader and go through all the entities using that shader.
    > Actually i wanna change how the engine manages shaders
 - [ ] General code refactoring
 - [ ] Make a scene for all IGMenus
 - [ ] Refactor for loops (don't allocate variables inside loops
 - [ ] Fix some warning in Light and Entity for Shader (now it uses pointers)
 - [ ] Consider if using a Shader Storage Buffer is useful for lights computing (at the moment I don't think so)
 - [ ] Improve light caster light effect
 - [ ] Instead of setting a CustomView matrix, you can send the reference of the static camera and should be fine using a Camera* object in the lights scene and not in every light
 - [ ] Fix ImGui Popups level, to understand try creating a new popup (IGMenuPopup)
    - [X] Add a static event queue to implement selected action in IGMenuPopup
 - [ ] Implement a ResourceManager
 - [ ] Implement a better method to manage Debug Logs, also find a way to implement time stamps
 - [ ] Create a timeline using buttons with value the number of the frame and next to it the value in second
