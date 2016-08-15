WASD - move forward, left, backward, right
Arrow keys - pitch, yaw camera
IJKLOP - light movement
1,2 - toggle front face culling
3,4 - toggle wireframe mode
5,6,7 - toggle point, directional, spotlight
R- reset fire

particles are 3D billboarded
discarded all alphha < 0.1 hence fixing alpha blending based on distance prob
farmhouse uses different texture when on fire and not on fire

FrameWork:
all render done in render manager
scene only manages variables and render manager automatically does render
billboards 3d billboard lightenabled shadows are all boolean toggles in the scene, while managed and done in render manager
any object to be render just inherits from Renderable class