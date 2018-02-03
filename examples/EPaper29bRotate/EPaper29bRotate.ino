/**
 *  @filename   :   EPaper29bRotateDemo.ino
 *  @brief      :   2.9inch e-paper display (B) demostrating screen rotation
 *  @author     :   ScruffR (particle.io)
 *
 *  Copyright (C) Waveshare     October 03 2017
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documnetation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to  whom the Software is
 * furished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

SYSTEM_THREAD(ENABLED)

#include "WaveshareEPaper29b.h"

#define COLORED     0
#define UNCOLORED   1

#define WIDTH  128
#define HEIGHT 296

const int pRESET = A0;
const int pDC    = A1;
const int pCS    = A2;
const int pBUSY  = D7;

unsigned char black[(WIDTH * HEIGHT) / 8]; //[296*128];
unsigned char red[(WIDTH * HEIGHT) / 8]; //[296*128];

Epd   epd(SPI, pCS, pDC, pRESET, pBUSY);
Paint paintBlack(black, WIDTH, HEIGHT);
Paint paintRed(red, WIDTH, HEIGHT);

void setup() {
  if (!epd.Init()) 
  {
    Serial.print("e-Paper init failed");
    return;
  }
  else
    Serial.print("success");
  
  epd.ClearFrame();

  paintBlack.Clear(UNCOLORED);
  paintBlack.DrawStringAt(15, 5, "e-Paper Demo", &Font12, COLORED);
  paintBlack.DrawRectangle(1, 1, paintBlack.GetWidth()-2, paintBlack.GetHeight()-2, COLORED);
  paintBlack.DrawLine(0, 0, paintBlack.GetWidth(), paintBlack.GetHeight(), COLORED);
  paintBlack.DrawLine(paintBlack.GetWidth(), 0, 0, paintBlack.GetHeight(), COLORED);

  paintRed.Clear(UNCOLORED);
  paintRed.DrawCircle(paintRed.GetWidth()/2, paintRed.GetHeight()/2, 30, COLORED);
  paintRed.DrawStringAt(5, 64, "Hello world", &Font16, COLORED);

  epd.DisplayFrame(black, red);

  epd.WaitUntilIdle();
  
  //epd.Sleep();
}
  
void loop() {
  static int orient = 0;
  ORIENTATION po = PORTRAIT; // painting orientation

  if (!digitalRead(BTN))
  {
      switch((++orient & 0x06) >> 1)
      {
        case 0:
          po = PORTRAIT;
          break;
        case 1:
          po = PORTRAIT_FLIPPED;
          break;
        case 2:
          po = LANDSCAPE;
          break;
        case 3:
          po = LANDSCAPE_FLIPPED;
          break;
      }
      
      if (orient & 0x01) paintRed.SetRotate(po);
      else               paintBlack.SetRotate(po);
      
      epd.ClearFrame();
    
      paintBlack.Clear(UNCOLORED);
      paintBlack.DrawStringAt(15, 5, "e-Paper Demo", &Font12, COLORED);
      paintBlack.DrawRectangle(1, 1, paintBlack.GetWidth()-2, paintBlack.GetHeight()-2, COLORED);
      paintBlack.DrawLine(0, 0, paintBlack.GetWidth(), paintBlack.GetHeight(), COLORED);
      paintBlack.DrawLine(paintBlack.GetWidth(), 0, 0, paintBlack.GetHeight(), COLORED);
    
      paintRed.Clear(UNCOLORED);
      paintRed.DrawCircle(paintRed.GetWidth()/2, paintRed.GetHeight()/2, 30, COLORED);
      paintRed.DrawStringAt(5, 64, "Hello world", &Font16, COLORED);
    
      epd.DisplayFrame(black, red);
    
      epd.WaitUntilIdle();
  }
}

