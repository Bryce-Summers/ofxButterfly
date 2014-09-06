#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif
#include <SDL/SDL.h>
#include <iostream>
#include "cube.hpp"

#define RUN_GRAPHICS_DISPLAY 0x00;


Uint32 Display(Uint32 interval, void* param)
{
  (void)param; // unused

  SDL_Event event;
  event.type = SDL_USEREVENT;
  event.user.code = RUN_GRAPHICS_DISPLAY;
  event.user.data1 = 0;
  event.user.data2 = 0;
  SDL_PushEvent(&event);
  return interval;
}

void Display(bool toggle)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();

  // fov
  static GLfloat fov = -5.0f;
  glTranslatef(0.0f, 0.0f, fov);

  static shapes::Cube cube;


  // rotate
  static GLfloat angle = 0.0f;
  glRotatef(angle, 1.0f, 1.0f, 1.0f);

  // subdivide
  static int i = 900;

  // move or stay
  static bool run = true;

  if (toggle)
    run = (run) ? false : true;

  if (run)
  {
    angle += 2.0f;
    if (i++ < 1600 && (i % 200) == 0) 
      cube.ButterflySubdivide();
  }

  cube.Draw();

  SDL_GL_SwapBuffers();
}

int main(int argc, char** argv)
{
  (void)argc;
  (void)argv; // unused
  
  SDL_Surface* surf;
  Uint32 width = 720;
  Uint32 height = 680;
  Uint32 colourDepth = 16;
  Uint32 delay = 1000/30;
  
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER)
    < 0)
  {
    std::cout << "Failed to initalize SDL: " << SDL_GetError() << std::endl;
    return 1;
  }

  atexit(SDL_Quit);

  if (!(surf = SDL_SetVideoMode(width, height, colourDepth, SDL_OPENGL)))
  {
    std::cout << "Failed to initalize VideoMode: " << SDL_GetError() << std::endl;
    return 1;
  }

  glViewport(0, 0, (GLsizei)width, (GLsizei)height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  gluPerspective(45.0f, (GLfloat)width/(GLfloat)height, 1.0f, 1000.0f);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glShadeModel(GL_SMOOTH);
  glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
  glClearDepth(1.0f);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

  SDL_AddTimer(delay, Display, NULL);

  SDL_Event event;
  while (SDL_WaitEvent(&event))
  {
    switch (event.type)
    {
      case SDL_QUIT:
        std::cout << "Quiting." << std::endl;
        return 0;
        break;
      case SDL_MOUSEBUTTONDOWN:
        Display(true);
        break;
      case SDL_USEREVENT:
        Display(false);
        break;
    }
  }

  // should never get here
  return 0;
}
