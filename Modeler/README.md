# modeler

0. How to use Lsystem: first set the controller "draw Lsystem" to be 1; then set the Lsystem level. Otherwise nothing will hapen if you set the level directly.

1. Texture Map: Used stb_image.h to read image file.

2. Animation and Mood cycling: Together, a walking human is simulated, and by adjusting the mood slider, frequency and amplitude of some / all parts of the human are moving faster/slower.

3. Up vector: This function is implemented in an intuitive way just like the sample modeler .exe we were shown on the project page, that both twisting & zooming take effect in the same time.

4. frame all: By setting the dolly variable from camera class to be -20, and the camera will pull away from the object if they are close to each other.

5. About the third whistle: we add a controler called "sit down" to make the man sit down.  

6. Plygonal faces: we added pyramids as the man's feet.  

7. The individual looking is controled by bodyx, bodyy, bodyz  

8. The new primitive is a torus.
