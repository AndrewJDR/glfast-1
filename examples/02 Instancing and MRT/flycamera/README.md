**Description**: Single-file C implementation of a first-person quaternion 3D fly camera.

**License**: Public domain

**Quaternion Layout**: xyzw

**3x3 Matrix Order**: column-major (GLSL)

**Coordinate System**: right-handed (OpenGL)

**Dependencies**: libm

Example for SDL2
----------------

<img width="600px" src="https://i.imgur.com/W7hiaAK.gif" />

```c
#include "flycamera.h"

int main()
{
  // float cam_vec3[3] = {0, 0, 0}
  // float cam_quat[4] = {0, 0, 0, 1}
  // float cam_m3x3[9] = {1, 0, 0,
  //                      0, 1, 0,
  //                      0, 0, 1}

  SDL_SetRelativeMouseMode(1);
  
  Uint32 t_prev = SDL_GetTicks();

  while(1)
  {
    Uint32 t_curr = SDL_GetTicks();
    double dt = ((t_curr - t_prev) * 60.0) / 1000.0;

    SDL_PumpEvents();
    int mouse_x_rel = 0;
    int mouse_y_rel = 0;
    SDL_GetRelativeMouseState(&mouse_x_rel, &mouse_y_rel);
    const Uint8 * key = SDL_GetKeyboardState(NULL);
    
    flycamera(
      cam_vec3,
      cam_quat,
      cam_m3x3,
      0.10,
      0.05 * dt,
      -mouse_x_rel,
      -mouse_y_rel,
      key[SDL_SCANCODE_W],
      key[SDL_SCANCODE_A],
      key[SDL_SCANCODE_S],
      key[SDL_SCANCODE_D],
      key[SDL_SCANCODE_E],
      key[SDL_SCANCODE_Q]
    );

    /* Now cam_vec3, cam_quat and cam_m3x3 can be passed to a shader */

    /* The rest of the loop */

    t_prev = t_curr;
  }
}
```

Example for GLFW3
-----------------

<img width="600px" src="https://i.imgur.com/4WLSYTQ.gif" />

```c
#include "flycamera.h"

int main()
{
  // float cam_vec3[3] = {0, 0, 0}
  // float cam_quat[4] = {0, 0, 0, 1}
  // float cam_m3x3[9] = {1, 0, 0,
  //                      0, 1, 0,
  //                      0, 0, 1}

  glfwSetInputMode(glfw_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  double t_prev = glfwGetTime();

  while(!glfwWindowShouldClose(glfw_window))
  {
    double t_curr = glfwGetTime();
    double dt = (t_curr - t_prev) * 60.0;

    glfwPollEvents();
    double mouse_x = 0;
    double mouse_y = 0;
    static double mouse_x_prev = 0;
    static double mouse_y_prev = 0;
    glfwGetCursorPos(glfw_window, &mouse_x, &mouse_y);

    flycamera(
      cam_vec3,
      cam_quat,
      cam_m3x3,
      0.10,
      0.05 * dt,
      mouse_x_prev - mouse_x,
      mouse_y_prev - mouse_y,
      glfwGetKey(glfw_window, GLFW_KEY_W),
      glfwGetKey(glfw_window, GLFW_KEY_A),
      glfwGetKey(glfw_window, GLFW_KEY_S),
      glfwGetKey(glfw_window, GLFW_KEY_D),
      glfwGetKey(glfw_window, GLFW_KEY_E),
      glfwGetKey(glfw_window, GLFW_KEY_Q)
    );

    mouse_x_prev = mouse_x;
    mouse_y_prev = mouse_y;

    /* Now cam_vec3, cam_quat and cam_m3x3 can be passed to a shader */

    /* The rest of the loop */

    t_prev = t_curr;
  }
}
```

