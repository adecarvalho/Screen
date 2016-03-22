/**
*Version 28.2.2016 
* add digit4Led 
* add panel
* add setResolution
* change isContent with Panel
* 
* author A.DeCarvalho

*/

#ifndef SCREEN_H
#define SCREEN_H

#include <Arduino.h>
#include "PicasoSerial.h"

#define NB_ELEMENT_MAX	100

class Widget;
class Screen;
//----------------------------------
//-------------
// Objet Screen
//-------------
class Screen
{
  public:
	// constructor
	Screen(PicasoSerial* device, word x, word y, word w, word h, Color bkgColor);
	//
	virtual ~Screen(void);
	//
	void addWidget(Widget* pObj);
	//
	void drawWidget(Widget* pObj);
	//
	void drawAllWidget(void);

	//getter setter
	PicasoSerial* getPicasoSerial(void);
	//
	word getXpos(void) const;
	//
	word getYpos(void) const;
	//
	word getWidth(void) const;
	//
	word getHeight(void) const;
	//
	Color getColorBkg(void) const;
	//
	bool isContent(Widget* pObj, word x, word y);
	//
	Widget* getWidgetPosition(word x, word y);
	//
	//traitement message graphique
	void getProcessMessage(GraphicMessage* msg);
	//
	word getTouchEvent(GraphicMessage* msg);

  protected:
	PicasoSerial* myDevice;
	//
	word myXpos, myYpos, myWidth, myHeight;
	//
	Color myColorBkg;

	word myNbObjet;

	Widget* myTableau[NB_ELEMENT_MAX];

};
//----------------------------------
//----------------------
//Widget the mother
//----------------------
class Widget
{

  public:
	Widget(Screen* screen, word id, word xpos, word ypos, word width, word height, Color color);

	//
	virtual ~Widget(void);
	//
	virtual void draw(void);
	//
	word getXposition(void) const;
	//
	word getYposition(void) const;
	//
	word getWidth(void) const;
	//
	word getHeight(void) const;
	//
	word getId(void) const;
	//
	word getIsVisible(void) const;
	//
	word getState(void) const;
	//
	word getType(void) const;
	//
	Color getColor(void) const;
	//
	void setColor(Color color);
	//
	void setState(State state);
	//
	void setId(word id);
	//
	void setVisibility(bool visu);
	//
	//

  protected:
	Screen* myScreen;
	word myXpos, myYpos, myWidth, myHeight;
	word myId;
	word myType;
	State myState;
	bool myIsVisible;
	Color myColor;

};
//-------------------------------------------------
// GraphicLed
//------------------------------
class GraphicLed: public Widget
{
  public:
	GraphicLed(Screen* screen, word xc, word yc, word radius, Color colorOn, Color colorOff, State state);

	virtual void draw(void);
	//
	virtual ~GraphicLed(void);
	//
	void setState(State state);
	//
	void setInvisible(bool visu);

  protected:
	Color myColorOn;
	Color myColorOff;
	word myRadius;
	word myCenterX;
	word myCenterY;
};
//--------------------------------------------
// Label
//----------------------------
class Label: public Widget
{
  public:
	Label(Screen* screen, word xpos, word ypos, char* txt, Font font, Color colorTxt, Color bkgColor);

	virtual ~Label(void);
	//
	virtual void draw();
	//
	void setInvisible(bool visu);
	//
	void setNewText(char* txt);

  protected:
	char* myText;
	Font myFont;
	Color myColorText;

};
//------------------------------------
//Button
//------------------------------------
class Button: public Widget
{
  public:
	Button(Screen* screen, word id, word xpos, word ypos, String txt, Font font, Color colorTxt, Color colorBtn, State state);

	virtual ~Button(void);
	//
	virtual void draw(void);
	//
	void setInvisible(bool visu);
	//
	void setState(State state);
	//
	void changeState(GraphicMessage* msg);

  protected:
	String myText;
	Font myFont;
	Color myColorTxt;
};
//-----------------------------------
//Slider
//------------------------------------
class Slider: public Widget
{
  public:
	Slider(Screen* screen, word id, word xpos, word ypos, word width, word height, int valMax, int value, Color colorBkg);

	//
	virtual ~Slider(void);
	//
	virtual void draw(void);
	//
	void setInvisible(bool visu);
	//
	int getValue(GraphicMessage* msg);
	//
	void setValue(int value);

  protected:
	int myValueMax;
	int myValue;
};
//----------------------------------------
// Switch
//---------------------------------------
class Switcher: public Widget
{
  public:
	Switcher(Screen* screen, word id, word xpos, word ypos, word width, word height);

	//
	virtual ~Switcher(void);
	//
	virtual void draw(void);
	//
	void setInvisible(bool visu);
	//
	void setState(State state);
	//
	void changeState(GraphicMessage* msg);

  protected:
	Color myColorOn;
	Color myColorOff;
	Color myColorCursor;
};
//----------------------------------------
//Input
//-----------------------------------------
class Scaler: public Widget
{
  public:
	Scaler(Screen* screen, word id, word xpos, word ypos, word width, word height, float valMin, float valInc, float valMax);

	//
	virtual ~Scaler(void);
	//
	virtual void draw(void);
	//
	void setInvisible(bool visu);
	//
	void setColorBkg(Color colorBkg);
	//
	void setColorCursorUp(Color colorUp);
	//
	void setColorCursorDown(Color colorDown);
	//
	float getValue(GraphicMessage* msg);


  protected:
	float myValMin;
	float myValMax;
	float myValInc;
	float myValue;

	Color myColorBkg;
	Color myColorUp;
	Color myColorDown;
};
//---------------------------------------
//DigitLed
//----------------------------------------
class DigitLed: public Widget
{
  public:
	DigitLed(Screen* screen, word xpos, word ypos, bool ptVisible);

	//
	virtual ~DigitLed(void);
	//
	virtual void draw(void);
	//
	void setInvisible(bool visu);
	//
	void setColorBkg(Color colorBkg);
	//
	void setColorSeg(Color colorSeg);
	//
	void setValue(char value);
	//
	void setPtVisible(bool state);

  protected:
	char myValue;
	Color myColorBkg;
	Color myColorSeg;
	word myIncX;
	word myIncY;
	bool myPtVisible;

	//
	void draw_seg_a(PicasoSerial* ps);
	void draw_seg_b(PicasoSerial* ps);
	void draw_seg_c(PicasoSerial* ps);
	void draw_seg_d(PicasoSerial* ps);
	void draw_seg_e(PicasoSerial* ps);
	void draw_seg_f(PicasoSerial* ps);
	void draw_seg_g(PicasoSerial* ps);
	void draw_seg_pt(PicasoSerial* ps);
};
//----------------------------------------
//Digit3Led
//-----------------------------------------
class Digit3Led: public Widget
{
  public:
	Digit3Led(Screen* screen, word xpos, word ypos, char resolution);

	//
	virtual ~Digit3Led(void);
	//
	virtual void draw(void);
	//
	void setInvisible(bool visu);
	//
	void setColorBkg(Color colorBkg);
	//
	void setColorSeg(Color colorSeg);
	//
	void setValue(float value);
	//
	void setResolution(int resolution);

  protected:
	float myValue;
	char myResolution;
	Color myColorBkg;
	Color myColorSeg;

	DigitLed* myDigitLedCentaine;
	DigitLed* myDigitLedDizaine;
	DigitLed* myDigitLedUnite;

};
//--------------------------------------
//Digit4Led
//--------------------------------------
class Digit4Led: public Widget
{
  public:
	Digit4Led(Screen* screen, word xpos, word ypos, int resolution);

	//
	virtual ~Digit4Led(void);
	//
	virtual void draw(void);
	//
	void setInvisible(bool visu);
	//
	void setColorBkg(Color colorBkg);
	//
	void setColorSeg(Color colorSeg);
	//
	void setValue(float value);
	//
	void setResolution(int resolution);

  protected:
	float myValue;
	int myResolution;
	Color myColorBkg;
	Color myColorSeg;

	DigitLed* myDigitLedMillieme;
	DigitLed* myDigitLedCentaine;
	DigitLed* myDigitLedDizaine;
	DigitLed* myDigitLedUnite;

};
//--------------------------------------
//Panel
//--------------------------------------
class Panel: public Widget
{
public:
	Panel(Screen* screen,word xpos, word ypos,word width,word height, Color color);

	virtual ~Panel(void);
	//
	virtual void draw(void);
	//
	void setInvisible(bool visu);
	//

};
#endif // GRAPHIC_OBJET_H_
