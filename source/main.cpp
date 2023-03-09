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
  Ball(float px, float py, float direction = M_PI * 1.5, float speed = 0.1f, float gravity = 0.1f);
  void Draw(void);
  void Move(void);

private:
  float _px, _py, _direction, _speed, _gravity;
};

Ball::Ball(float px, float py, float direction, float speed, float gravity)
{
  _px = px;
  _py = py;
  _direction = direction;
  _speed = speed;
  _gravity = gravity;
}

void Ball::Draw(void)
{
  u32 clrWater = C2D_Color32(0x99, 0x99, 0xFF, 0xBB);
  C2D_DrawCircleSolid(_px, _py, 0, 1.0f, clrWater);
}

void Ball::Move(void)
{
  if (!_speed && _py == SCREEN_HEIGHT - 2)
    return;


  _direction = atan2(sin(_direction)*_speed, cos(_direction)*_speed - _gravity);
  _speed = sqrt((sin(_direction) * _speed) * (sin(_direction) * _speed) + (cos(_direction) * _speed - _gravity) * (cos(_direction) * _speed - _gravity));
  _px += sin(_direction) * _speed;
  _py -= cos(_direction) * _speed;
  if (_px < 1)
  {
    _px = 1;
  }
  if (SCREEN_WIDTH - 1 < _px)
  {
    _px = SCREEN_WIDTH - 1;
  }
  if (_py < 1)
  {
    _py = 1;
  }
  if (SCREEN_HEIGHT - 1 < _py)
  {
    _py = SCREEN_HEIGHT - 1;
    _speed = _speed - 0.1f < 0 ? 0 : _speed - 0.1;
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
      // balls.push_back(Ball(touch.px, touch.py, M_PI / 2, 5.0F));
      // balls.push_back(Ball(touch.px, touch.py, (touch.py * 1.0 / SCREEN_HEIGHT * 1.0) * M_PI * 2, 5.0F));
      for (size_t i = 0; i < 10; i++)
        balls.push_back(Ball(touch.px, touch.py, rand() % int(M_PI * 2 * 100) * 0.01f, 5.0F));
    }
    if (kDown & KEY_X)
      balls.clear();

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

    C3D_FrameEnd(0);
  }

  // Deinit libs
  C2D_Fini();

  C3D_Fini();

  gfxExit();

  return 0;
}
