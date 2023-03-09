#include <3ds.h>
#include <citro2d.h>

#include <iostream>
#include <cmath>
#include <vector>

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

class Ball
{
public:
  Ball(float px, float py, float direction = M_PI * 1.5, float speed = 0.1f, float gravity = 0.05f);
  void Draw(void);
  void Move(void);

private:
  float _px, _py, _direction, _speed, _gravity;
};

Ball::Ball(float px, float py, float direction, float speed, float gravity)
{
  this->_px = px;
  this->_py = py;
  this->_direction = direction;
  this->_speed = speed;
  this->_gravity = gravity;
}

void Ball::Draw(void)
{
  u32 clrWater = C2D_Color32(0x99, 0x99, 0xFF, 0xBB);
  C2D_DrawCircleSolid(this->_px, this->_py, 0, 1.0f, clrWater);
}

void Ball::Move(void)
{
  if (_speed != 0)
  {
    if (_direction < M_PI)
      _speed -= _gravity;
    else
      _speed += _gravity;
  }

  if (M_PI / 2 < _direction && _direction < M_PI * 1.5)
    _direction += _gravity;
  else if (_direction != M_PI / 2)
  {
    _direction -= _gravity;
  }

  _px += cos(_direction) * _speed;
  _py -= sin(_direction) * _speed;
  if (_px < 1)
  {
    _px = 1;
    // direction = abs(M_PI - direction);
    // speed = abs(speed - 0.5f);
  }
  if (SCREEN_WIDTH - 1 < _px)
  {
    _px = SCREEN_WIDTH - 1;
    // direction = abs(M_PI - direction);
    // speed = abs(speed - 0.5f);
  }
  if (_py < 1)
  {
    _py = 1;
    // direction = direction < M_PI ? direction + M_PI : M_PI - direction;
    // speed = abs(speed - 0.5f);
  }
  if (SCREEN_HEIGHT - 1 < _py)
  {
    _py = SCREEN_HEIGHT - 1;
    _speed = _speed - 0.1f < 0 ? 0 : _speed - 0.1f;
    // direction = abs(M_PI - direction);
    // speed = abs(speed - 0.5f);
  }
}

int main(int argc, char **argv)
{
  gfxInitDefault();
  C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
  C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
  C2D_Prepare();
  consoleInit(GFX_TOP, nullptr);

  C3D_RenderTarget *top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
  C3D_RenderTarget *bottom = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);

  touchPosition touch;
  u32 clrClear = C2D_Color32(0xFF, 0xD8, 0xB0, 0x68);

  std::vector<Ball> balls;

  // Main loop
  while (aptMainLoop())
  {
    hidScanInput();

    u32 kDown = hidKeysDown();
    u32 kHeld = hidKeysHeld();

    if (kDown & KEY_START)
      break; // break in order to return to hbmenu

    if (kHeld & KEY_TOUCH)
    {
      hidTouchRead(&touch);
      balls.push_back(Ball(touch.px, touch.py, M_PI, 5.0F));
      // balls.push_back(Ball(touch.px, touch.py, rand() % 11 * M_PI / 6, 5.0F));
    }
    if(kDown & KEY_X)
      balls.clear();

    // Render the scene
    C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
    C2D_TargetClear(bottom, clrClear);
    C2D_SceneBegin(bottom);

    consoleClear();
    printf("%d\n", balls.size());

    for (auto &&ball : balls)
    {
      ball.Move();
      ball.Draw();
    }
    // u32 clrWater = C2D_Color32(0x99, 0x99, 0xFF, 0x99);
    // C2D_DrawCircleSolid(100, 100, 0, 1.0f, clrWater);
    // Ball(100, 100).Draw();

    C3D_FrameEnd(0);
  }

  // Deinit libs
  C2D_Fini();

  C3D_Fini();

  gfxExit();

  return 0;
}
