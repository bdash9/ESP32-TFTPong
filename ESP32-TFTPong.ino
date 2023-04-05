/*
 TFT Pong

 This example for the Arduino screen reads the values
 of 2 potentiometers to move a rectangular platform
 on the x and y axes. The platform can intersect
 with a ball causing it to bounce.

 This example code is in the public domain.

 Created by Tom Igoe December 2012
 Modified 15 April 2013 by Scott Fitzgerald

 http://www.arduino.cc/en/Tutorial/TFTPong

 */
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

int paddleWidth = 5;
int paddleHeight =20;
int paddleStartY = 65;
int paddleGap = 30; // distance between the two paddles

int paddle1Y = (135 - paddleHeight - paddleGap) / 2;
int paddle2Y = (135 + paddleGap) / 2;

int oldPaddle1Y = 0;
int oldPaddle2Y = 0;
int oldPaddleX = 0;
int oldPaddleY = 0;

int paddleX = 240 - paddleWidth;
int paddleY = 0;

int ballDirectionX = 1;
int ballDirectionY = 1;

int ballSpeed = 10;
int ballSize = 5;
int ballX, ballY, oldBallX, oldBallY;

int scoreLeft = 0;
int scoreRight = 0;

void setup() {
  pinMode(TFT_BACKLITE, OUTPUT);
  digitalWrite(TFT_BACKLITE, HIGH);

  pinMode(TFT_I2C_POWER, OUTPUT);
  digitalWrite(TFT_I2C_POWER, HIGH);
  delay(10);

  tft.init(135, 240);
  tft.setRotation(3);
  tft.fillScreen(ST77XX_BLACK);

  tft.setCursor(10, 5);
tft.setTextColor(ST77XX_WHITE);
tft.setTextSize(2);
tft.print(scoreLeft);

tft.setCursor(200, 5);
tft.setTextColor(ST77XX_WHITE);
tft.setTextSize(2);
tft.print(scoreRight);


  ballX = 60;
  ballY = 60;

  // draw the paddles at the starting position
tft.fillRect(paddleX, paddle1Y, paddleWidth, paddleHeight, ST77XX_WHITE);
tft.fillRect(paddleY, paddle2Y, paddleWidth, paddleHeight, ST77XX_WHITE);

}


void loop() {
  int myWidth = 240;
  int myHeight = 135;

  // erase the previous position of the paddles
  tft.fillRect(oldPaddle1Y, oldPaddleY, paddleWidth, paddleHeight, ST77XX_BLACK);
  tft.fillRect(oldPaddle2Y, oldPaddleY, paddleWidth, paddleHeight, ST77XX_BLACK);

  // update the position of the paddles based on the potentiometer readings
paddle1Y = map(analogRead(A0), 0, 1023, 0, myHeight - paddleHeight - paddleGap);
paddle2Y = map(analogRead(A1), 0, 1023, paddleGap, myHeight - paddleHeight);


  // draw the paddles at the new position
tft.fillRect(paddleX, paddle1Y, paddleWidth, paddleHeight, ST77XX_WHITE);
tft.fillRect(paddleX, paddle2Y, paddleWidth, paddleHeight, ST77XX_WHITE);

  // save the current position of the paddles as the old position
  oldPaddle1Y = paddle1Y;
  oldPaddle2Y = paddle2Y;
  oldPaddleY = paddleY;

  // update the ball's position and draw it on screen
  if (millis() % ballSpeed == 0) {
    moveBall();
  }
}

void moveBall() {
  // erase the previous position of the ball
  tft.fillRect(oldBallX, oldBallY, ballSize, ballSize, ST77XX_BLACK);

  // update the ball's position
  ballX += ballDirectionX;
  ballY += ballDirectionY;

  // check if the ball hits the left or right wall
  if (ballX <= 0 || ballX >= (240 - ballSize)) {
    ballDirectionX = -ballDirectionX;
  }

  // check if the ball hits the top or bottom wall
  if (ballY <= 0 || ballY >= (135 - ballSize)) {
    ballDirectionY = -ballDirectionY;
  }

  // check if the ball hits the paddles
  if ((ballX + ballSize >= paddle1Y && ballX <= paddle1Y + 20 && ballY + ballSize >= paddleY && ballY <= paddleY + 5) ||
      (ballX + ballSize >= paddle2Y && ballX <= paddle2Y + 20 && ballY + ballSize >= paddleY && ballY <= paddleY + 5)) {
    ballDirectionY = -ballDirectionY;
  }

  // draw the ball in its new position
  tft.fillRect(ballX, ballY, ballSize, ballSize, ST77XX_WHITE);

  // save the current position of the ball as the old position
  oldBallX = ballX;
  oldBallY = ballY;
  
  tft.drawFastVLine(120, 0, 135, ST77XX_WHITE);

  if (ballX <= 0) {
  scoreRight++;
  tft.fillRect(200, 5, 30, 20, ST77XX_BLACK); // erase old score
  tft.setCursor(200, 5);
  tft.print(scoreRight);
} else if (ballX >= (240 - ballSize)) {
  scoreLeft++;
  tft.fillRect(10, 5, 30, 20, ST77XX_BLACK); // erase old score
  tft.setCursor(10, 5);
  tft.print(scoreLeft);
  }
}

