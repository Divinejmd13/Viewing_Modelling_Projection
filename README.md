# Viewing_Modelling_Projection


Consider the world coordinate space with X axis to the right, Y axis up and Z axis comingout of the screen plane. The given program renders a scaled cube (a cuboid) with the
bottom face centered at the world space origin and a scale factor of 2 in the Y direction.Compile and run it.

Study the program and understand how The viewing and projection matrices are constructed with GLM and are copied to the vertex shader as Uniform variables.

1. Program the arrow keys on your keyboard to change the camera view of the scene.
  (a) The camera center should change as follows:
    - Left and right arrows move the camera along -X and +X of the camera axes.
    - Down and up arrows move the camera along the -Y and +Y of the camera axes.
    - Shift + down/up arrows move the camera along the -Z and +Z of the camera axes. This will give the effect of Zoom out/in.
    Keep the look-at point to be the world space origin (0,0,0) and the camera upvector to the world vector (0, 1, 0). Do not forget to update the viewing matrix after
    every change in camera center. Note that arrow keys should change camera center coordinates in the camera space (and not in the world space), which will result in a
    camera motion on a sphere around the origin. The glm::lookAt() in setupViewTransformation() requires camera center in world space.

  (b) Move the camera to specific positions and generate one-point perspective, twopoint perspective, and the two three-point perspective views (bird’s eye view and rat’s eye view).



2. The given program performs a perspective projection using GLMs perspective()
   function. Keep the arrow key movement from the previous question and
  (a) Program the ‘o’/‘O’ and the ‘p’/‘P’ keys on the keyboard to switch between orthographic and perspective projections respectively.
    You can use 1 ImGui::IsKeyPressed() function to detect keyboard keys.
   
  (b) Using arrow keys, move the camera to specific positions to generate top view, front elevation, and side elevation. Use a modifier key on your keyboard like Ctrl (control)
    to snap camera center to appropriate axes/lines in order to accurately generate such views.
