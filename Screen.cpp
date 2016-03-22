#include "Screen.h"

PicasoSerial* ps = 0;

//-----------------------------------------------
Screen::Screen(PicasoSerial* device, word x, word y, word w, word h, Color bkgColor): myDevice(device),
  myXpos(x), myYpos(y), myWidth(w), myHeight(h), myColorBkg(bkgColor)
{
  myNbObjet = 0;

  for (int i = 0; i < NB_ELEMENT_MAX; i++)
  {
	myTableau[i] = 0;
  }
}
//----------------------------------------------
Screen::~Screen(void)
{
	for(int i=0;i<myNbObjet;i++)
	{
	if(myTableau[i] != 0)
		{
		delete(myTableau[i]);
		myTableau[i]=0;
		}
	}

}
//----------------------------------------------
word Screen::getTouchEvent(GraphicMessage* msg)
{
  word x = 0;
  word y = 0;
  word ev = EVENT_NO_ACTIVITY;

  ev = myDevice->touch_Get(TOUCH_STATUS);

  //
  if (ev > EVENT_NO_ACTIVITY)
  {
	x = myDevice->touch_Get(TOUCH_GETX);
	y = myDevice->touch_Get(TOUCH_GETY);

	msg->event = ev;
	msg->posiX = x;
	msg->posiY = y;
	msg->objetId = 0;

	return ev;
  }
  else
  {
	msg->event = EVENT_NO_ACTIVITY;
	msg->posiX = 0;
	msg->posiY = 0;
	msg->objetId = 0;
  }

  return ev;
}
//----------------------------------------------
bool Screen::isContent(Widget* pObj, word x, word y)
{
  word type=pObj->getType();
  word xd = pObj->getXposition();
  word w = pObj->getWidth();
  word xf = xd + w;

  word yd = pObj->getYposition();
  word h = pObj->getHeight();
  word yf = yd + h;

	//
	if(type==OBJ_PANEL)
	{
		return false;
	}
	//
	else if (x >= xd && x <= xf && y >= yd && y <= yf )
	{
	return true;
	}
  return false;
}
//----------------------------------------------
Widget* Screen::getWidgetPosition(word x, word y)
{
  for (int i = 0; i < myNbObjet; i++)
  {
	if (isContent(myTableau[i], x, y) )
	  return myTableau[i];
  }
  return 0;
}
//----------------------------------------------
void Screen::getProcessMessage(GraphicMessage* msg)
{
  word x = msg->posiX;
  word y = msg->posiY;
  word res = 0;

  Widget* pObj = 0;

  pObj = getWidgetPosition(x, y);

  if (pObj != 0 && pObj->getIsVisible() == true)
  {
	res = pObj->getId();
	msg->objetId = res;
  }
  else
  {
	msg->objetId = 0;
  }
}
//----------------------------------------------
void Screen::addWidget(Widget* pObj)
{

  if (myNbObjet > NB_ELEMENT_MAX)
	return;
  //
  myTableau[myNbObjet] = pObj;
  myNbObjet++;
}
//---------------------------------------------
void Screen::drawWidget(Widget* pObj)
{
  pObj->draw();
}
//----------------------------------------------
void Screen::drawAllWidget(void)
{

  Widget* pobj = 0;

  myDevice->gfx_RectangleFilled(myXpos, myYpos, myXpos + myWidth, myYpos + myHeight, myColorBkg);

  for (int i = 0; i < myNbObjet; i++)
  {
	pobj = myTableau[i];
	if (pobj != 0)
	{
	  drawWidget(pobj);
	}
  }

}
//----------------------------------------------
PicasoSerial* Screen::getPicasoSerial(void)
{
  return this->myDevice;
}
//------------------------------------------------
word Screen::getXpos(void) const
{
  return this->myXpos;
}
//-------------------------------------------------
word Screen::getYpos(void) const
{
  return this->myYpos;
}
//------------------------------------------------
word Screen::getWidth(void) const
{
  return this->myWidth;
}
//------------------------------------------------
word Screen::getHeight(void) const
{
  return this->myHeight;
}
//------------------------------------------------
Color Screen:: getColorBkg(void) const
{
  return this->myColorBkg;
}
//--------------------------------------------------
//--------------------------------------------------
Widget::Widget(Screen* screen, word id, word xpos, word ypos, word width, word height, Color color)
{
  myScreen = screen;

  myXpos = xpos;
  myYpos = ypos;
  myWidth = width;
  myHeight = height;

  myId = id;

  myType = OBJ_NONE;
  myState = STATE_ON;

  myIsVisible = true;

  myColor = color;
}
//--------------------------------------------------
Widget::~Widget(void)
{
}
//---------------------------------------------------
void Widget::draw(void)
{
}
//----------------------------------------------------
word Widget:: getXposition(void) const
{
  return this->myXpos;
}
//------------------------------------------------------
word Widget::getYposition(void) const
{
  return this->myYpos;
}
//----------------------------------------------------
word Widget::getWidth(void) const
{
  return this->myWidth;
}
//-----------------------------------------------------
word Widget::getHeight(void) const
{
  return this->myHeight;
}
//----------------------------------------------------
word Widget::getId(void) const
{
  return this->myId;
}
//------------------------------------------------------
word Widget::getIsVisible(void) const
{
  return this->myIsVisible;
}
//-------------------------------------------------------
word Widget::getState(void) const
{
  return this->myState;
}
//--------------------------------------------------------
word Widget::getType(void) const
{
  return this->myType;
}
//--------------------------------------------------------
Color Widget::getColor(void) const
{
  return this->myColor;
}
//---------------------------------------------------------
void Widget::setColor(Color color)
{
  myColor = color;
}
//---------------------------------------------------------
void Widget::setState(State state)
{
  myState = state;
}
//---------------------------------------------------------
void Widget::setId(word id)
{
  myId = id;
}
//----------------------------------------------------------
void Widget::setVisibility(bool visu)
{
  myIsVisible = visu;
}
//------------------------------------------------------------
//------------------------------------------------------------
GraphicLed::GraphicLed(Screen* screen, word xc, word yc, word radius, Color colorOn,
					   Color colorOff, State state): Widget(screen, 0, xc - radius, yc - radius,
							 radius + radius, radius + radius, colorOn)
{
  this->myColorOn = colorOn;
  this->myColorOff = colorOff;
  this->myRadius = radius;
  this->myCenterX = xc;
  this->myCenterY = yc;

  this->myState = state;
  this->myType = OBJ_LED;

  this->myScreen->addWidget((Widget*)this);
}
//--------------------------------------------------
void GraphicLed::draw(void)
{
  if (myIsVisible == false)
	return;
  //
  ps = myScreen->getPicasoSerial();

  State st = myState;

  switch (st)
  {
	//
	case LED_STATE_ON:
	  if (ps != 0)
	  {
		ps->gfx_CircleFilled(myCenterX, myCenterY, myRadius, myColorOn);
		ps->gfx_Circle(myCenterX, myCenterY, myRadius, BLACK);
	  }
	  return;

	//
	case LED_STATE_OFF:
	  if (ps != 0)
	  {
		ps->gfx_CircleFilled(myCenterX, myCenterY, myRadius, myColorOff);
		ps->gfx_Circle(myCenterX, myCenterY, myRadius, BLACK);
	  }
	  return;
	//
	default:
	  break;
  }
}
//---------------------------------------------------
GraphicLed::~GraphicLed(void)
{
}
//--------------------------------------------------------
void GraphicLed::setState(State state)
{
  if (myIsVisible == false)
	return;
  //
  if (myState == state)
	return;
  //
  myState = state;

  this->draw();
}
//---------------------------------------------------------
void GraphicLed::setInvisible(bool visu)
{
  if (visu == true)
  {
	if (myIsVisible == false)
	  return;

	myIsVisible = false;

	Color c = myScreen->getColorBkg();

	ps = myScreen->getPicasoSerial();

	if (ps != 0)
	  ps->gfx_CircleFilled(myCenterX, myCenterY, myRadius, c);
  }
  //
  else
  {
	myIsVisible = true;
	this->draw();
  }
}
//----------------------------------------------------------
Label::Label(Screen* screen, word xpos, word ypos, char* txt, Font font,
			 Color colorTxt, Color bkgColor): Widget(screen, 0, xpos, ypos, 0, 0, bkgColor)
{
  this->myText = txt;
  this->myFont = font;
  this->myColorText = colorTxt;

  this->myType = OBJ_LABEL;
  this->myState = STATE_ON;

  word largeur = 0;
  word hauteur = 16;
  word taille = 0;
  word incLargeur = 0;

  switch (myFont)
  {
	case FONT1:
	  incLargeur = 5;
	  hauteur = 7;
	  largeur = 5;
	  break;

	//
	case FONT2:
	  incLargeur = 8;
	  hauteur = 8;
	  largeur = 8;
	  break;
	//
	case FONT3:
	  incLargeur = 8;
	  hauteur = 12;
	  largeur = 8;
	  break;
	//
	default:
	  incLargeur = 12;
	  hauteur = 16;
	  largeur = 8;
	  break;
  }
  //
  taille = strlen(txt);
  /*while(*txt)
  {
	  taille++;
	  txt++;
  }
  */
  //
  largeur = incLargeur * taille;

  this->myWidth = largeur;
  this->myHeight = hauteur;

  this->myScreen->addWidget((Widget*)this);
}
//------------------------------------------------
Label::~Label(void)
{
}
//--------------------------------------------------
void Label::draw()
{
  if (myIsVisible == false)
	return;

  ps = myScreen->getPicasoSerial();

  if (ps != 0)
  {
	ps->txt_BGcolour(myColor);

	ps->txt_FGcolour(myColorText);

	ps->gfx_MoveTo(myXpos, myYpos);

	ps->putstr(myText);
  }
}
//---------------------------------------------------
void Label::setInvisible(bool visu)
{
  if (visu == true)
  {
	if (myIsVisible == false)
	  return;
	myIsVisible = false;
	Color c = myScreen->getColorBkg();

	ps = myScreen->getPicasoSerial();

	if (ps != 0)
	  ps->gfx_RectangleFilled(myXpos, myYpos, myXpos + myWidth, myYpos + myHeight, c);
  }
  else
  {
	myIsVisible = true;
	this->draw();
  }
}
//----------------------------------------------------
void Label::setNewText(char* txt)
{
  word largeur = 0;
  word hauteur = 16;
  word taille = 0;
  word incLargeur = 0;

  setInvisible(true);

  this->myText = txt;

  switch (this->myFont)
  {
	case FONT1:
	  incLargeur = 5;
	  hauteur = 7;
	  largeur = 5;
	  break;

	//
	case FONT2:
	  incLargeur = 8;
	  hauteur = 8;
	  largeur = 8;
	  break;
	//
	case FONT3:
	  incLargeur = 8;
	  hauteur = 12;
	  largeur = 8;
	  break;
	//
	default:
	  incLargeur = 12;
	  hauteur = 16;
	  largeur = 8;
	  break;
  }

  /*
  while(*txt)
  {
	  taille=0;
	  txt++;
  }
  txt--;
  txt=txt-taille;
  */
  taille = strlen(txt);
  largeur = incLargeur * taille;

  this->myWidth = largeur;
  this->myHeight = hauteur;

  setInvisible(false);
}
//-----------------------------------------------------
Button::Button(Screen* screen, word id, word xpos, word ypos, String txt, Font font, Color colorTxt,
			   Color colorBtn, State state): Widget(screen, id, xpos, ypos, 0, 0, colorBtn)
{
  this->myText = txt;
  this->myFont = font;
  this->myColorTxt = colorTxt;

  this->myType = OBJ_BUTTON;
  this->myState = state;

  word largeur = 0;
  word hauteur = 16;
  word taille = 0;
  word incLargeur = 0;

  switch (myFont)
  {
	case FONT1:
	  incLargeur = 5;
	  hauteur = 7;
	  largeur = 5;
	  break;

	//
	case FONT2:
	  incLargeur = 8;
	  hauteur = 8;
	  largeur = 8;
	  break;
	//
	case FONT3:
	  incLargeur = 8;
	  hauteur = 12;
	  largeur = 8;
	  break;
	//
	default:
	  incLargeur = 12;
	  hauteur = 16;
	  largeur = 8;
	  break;
  }
  //
  taille = this->myText.length();
  largeur = incLargeur * taille;

  this->myWidth = largeur + incLargeur + incLargeur;
  this->myHeight = hauteur + incLargeur + incLargeur;

  this->myScreen->addWidget((Widget*)this);
}
//--------------------------------------------
Button::~Button(void)
{
}
//-----------------------------------------------
void Button::draw(void)
{
  if (myIsVisible == false)
	return;
  //
  State st = myState;

  char buffer[20] = {'\0'};

  ps = myScreen->getPicasoSerial();

  for (word i = 0; i < myText.length(); i++)
  {
	buffer[i] = myText.charAt(i);
  }

  switch (st)
  {

	case BUTTON_STATE_PRESSED:
	  if (ps != 0)
		ps->gfx_Button(BUTTON_DOWN, myXpos, myYpos, myColor, myColorTxt, myFont, 1, 1, buffer);
	  return;

	//
	case BUTTON_STATE_RELEASED:
	  if (ps != 0)
		ps->gfx_Button(BUTTON_UP, myXpos, myYpos, myColorTxt, myColor, myFont, 1, 1, buffer);
	  return;
	//
	default:
	  break;
  }
}
//-----------------------------------------------
void Button::setInvisible(bool visu)
{
  if (visu == true)
  {
	if (myIsVisible == false)
	  return;
	myIsVisible = false;
	Color c = myScreen->getColorBkg();

	ps = myScreen->getPicasoSerial();

	if (ps != 0)
	  ps->gfx_RectangleFilled(myXpos, myYpos, myXpos + myWidth, myYpos + myHeight, c);
  }
  else
  {
	myIsVisible = true;
	this->draw();
  }
}
//-------------------------------------------------
void Button::setState(State state)
{
  if (myIsVisible == false)
	return;
  //
  if (myState == state)
	return;
  myState = state;

  this->draw();
}
//------------------------------------------------
void Button::changeState(GraphicMessage* msg)
{
  if (myIsVisible == false)
	return;
  //
  word ev = msg->event;

  if (ev == EVENT_PRESSED && myState == BUTTON_STATE_PRESSED)
  {
	this->setState(BUTTON_STATE_RELEASED);
	return;
  }
  //
  if (ev == EVENT_PRESSED && myState == BUTTON_STATE_RELEASED)
  {
	this->setState(BUTTON_STATE_PRESSED);
	return;
  }
  //
  if (ev == EVENT_MOVE || ev == EVENT_RELEASED)
	return;
}
//------------------------------------------------
Slider::Slider(Screen* screen, word id, word xpos, word ypos, word width, word height, int valMax,
			   int value, Color colorBkg): Widget(screen, id, xpos, ypos, width, height, colorBkg)
{
  myValueMax = valMax;
  myValue = value;

  myType = OBJ_SLIDER;
  myState = SLIDER_STATE_RELEASED;

  myScreen->addWidget((Widget*)this);
}
//---------------------------------------------------
Slider::~Slider(void)
{
}
//------------------------------------------------
void Slider::setValue(int value)
{
  if(value >myValueMax)
	value=myValueMax;
	
  if(value <0)
	value=0;
	
   this-> myValue=value;
	
	 this->draw();   
}
//------------------------------------------------
void Slider::draw(void)
{
  if (myIsVisible == false)
	return;
  //
  ps = myScreen->getPicasoSerial();

  if (ps != 0)
	ps->gfx_Slider(0x0000, myXpos, myYpos, myXpos + myWidth, myYpos + myHeight, myColor, myValueMax, myValue);
}
//------------------------------------------------
void Slider::setInvisible(bool visu)
{
  if (visu == true)
  {
	if (myIsVisible == false)
	  return;
	//
	myIsVisible = false;

	Color c = myScreen->getColorBkg();

	ps = myScreen->getPicasoSerial();

	if (ps != 0)
	  ps->gfx_RectangleFilled(myXpos, myYpos, myXpos + myWidth, myYpos + myHeight, c);
  }
  else
  {
	myIsVisible = true;

	this->draw();
  }
}
//-------------------------------------------------
int Slider::getValue(GraphicMessage* msg)
{
  if (myIsVisible == false)
	return 0;
  //
  word ev = msg->event;
  int xn = msg->posiX;

  if (ev == EVENT_RELEASED)
	myState = SLIDER_STATE_RELEASED;
  //
  if (ev == EVENT_MOVE)
  {
	myState = SLIDER_STATE_MOVE;
	int dis = xn - myXpos;
	int a = dis * myValueMax;

	myValue = a / myWidth;

	this->draw();
  }
  return myValue;
}
//--------------------------------------------------
Switcher::Switcher(Screen* screen, word id, word xpos, word ypos, word width, word height):
  Widget(screen, id, xpos, ypos, width, height, WHITE)
{
  myColorOn = GREEN;
  myColorOff = RED;
  myColorCursor = WHITE;

  this->myType = OBJ_SWITCHER;
  this->myState = SWITCHER_STATE_OFF;

  myScreen->addWidget((Widget*)this);
}
//----------------------------------------------------
Switcher::~Switcher(void)
{
}
//-------------------------------------------------------
void Switcher::draw(void)
{
  if (myIsVisible == false)
	return;
  //
  State st = myState;
  Color gauche = WHITE;
  Color droite = WHITE;

  switch (st)
  {
	//
	case SWITCHER_STATE_ON:
	  droite = myColorOn;
	  gauche = myColorCursor;
	  break;

	//
	case SWITCHER_STATE_OFF:
	  droite = myColorCursor;
	  gauche = myColorOff;
	  break;

	//
	default:
	  break;
  }
  //
  ps = myScreen->getPicasoSerial();

  //rec gauche
  if (ps != 0)
	ps->gfx_RectangleFilled(myXpos, myYpos, myXpos + myWidth / 2, myYpos + myHeight, gauche);

  //rec droite
  if (ps != 0)
	ps->gfx_RectangleFilled(myXpos + myWidth / 2, myYpos, myXpos + myWidth, myYpos + myHeight, droite);

  //cadre
  if (ps != 0)
  {
	ps->gfx_Rectangle(myXpos, myYpos, myXpos + myWidth, myYpos + myHeight, BLACK);
	ps->gfx_Rectangle(myXpos, myYpos, myXpos + myWidth / 2, myYpos + myHeight, BLACK);
  }
}
//--------------------------------------------------------
void Switcher::setInvisible(bool visu)
{
  if (visu == true)
  {

	if (myIsVisible == false)
	  return;
	myIsVisible = false;

	Color c = myScreen->getColorBkg();
	ps = myScreen->getPicasoSerial();

	if (ps != 0)
	  ps->gfx_RectangleFilled(myXpos, myYpos, myXpos + myWidth, myYpos + myHeight, c);
  }
  else
  {
	myIsVisible = true;

	this->draw();
  }
}
//---------------------------------------------------------
void Switcher::setState(State state)
{
  if (myIsVisible == false)
	return;

  if (myState == state)
	return;

  myState = state;

  this->draw();
}
//---------------------------------------------------------
void Switcher::changeState(GraphicMessage* msg)
{
  if (myIsVisible == false)
	return;
  //
  word ev = msg->event;

  if (ev == EVENT_PRESSED && myState == SWITCHER_STATE_OFF)
  {
	this->setState(SWITCHER_STATE_ON);
	return;
  }
  //
  if (ev == EVENT_PRESSED && myState == SWITCHER_STATE_ON)
  {
	this->setState(SWITCHER_STATE_OFF);
	return;
  }
  //
  if (ev == EVENT_MOVE || ev == EVENT_RELEASED)
	return;
}
//---------------------------------------------------------
Scaler::Scaler(Screen* screen, word id, word xpos, word ypos, word width,
			   word height, float valMin, float valInc, float valMax): Widget(screen, id, xpos, ypos, width, height, WHITE)
{
  myValMin = valMin;
  myValMax = valMax;
  myValInc = valInc;
  myValue = 0.0;

  myColorBkg = GRAY;
  myColorUp = BLUE;
  myColorDown = BLUE;

  myType = OBJ_SCALER;
  myState = SCALER_STATE_RELEASED;

  myScreen->addWidget((Widget*)this);
}
//------------------------------------------------------
Scaler::~Scaler(void)
{
}
//------------------------------------------------------
void Scaler::draw(void)
{
  if (myIsVisible == false)
	return;
  //
  ps = myScreen->getPicasoSerial();

  //rec up
  if (ps != 0)
	ps->gfx_RectangleFilled(myXpos, myYpos, myXpos + myWidth, myYpos + myHeight / 2, myColorBkg);

  //rec down
  if (ps != 0)
	ps->gfx_RectangleFilled(myXpos, myYpos + myHeight / 2 + 3, myXpos + myWidth, myYpos + myHeight + 3, myColorBkg);

  //fleche up
  word x1 = myXpos + myWidth / 2;
  word y1 = myYpos;

  word x2 = myXpos + myWidth;
  word y2 = myYpos + myHeight / 2;

  word x3 = myXpos;
  word y3 = myYpos + myHeight / 2;

  if (ps != 0)
	ps->gfx_TriangleFilled(x1, y1, x2, y2, x3, y3, myColorUp);

  //fleche down
  x1 = myXpos;
  y1 = myYpos + 3 + myHeight / 2;

  x2 = myXpos + myWidth;
  y2 = y1;

  x3 = myXpos + myWidth / 2;
  y3 = y2 + myHeight / 2;

  if (ps != 0)
	ps->gfx_TriangleFilled(x1, y1, x2, y2, x3, y3, myColorDown);

  //cadre
  if (ps != 0)
	ps->gfx_Rectangle(myXpos, myYpos, myXpos + myWidth, myYpos + myHeight + 3, BLACK);
}
//---------------------------------------------------------
void Scaler::setInvisible(bool visu)
{
  if (visu == true)
  {
	if (myIsVisible == false)
	  return;

	myIsVisible = false;

	Color c = myScreen->getColorBkg();

	ps = myScreen->getPicasoSerial();

	if (ps != 0)
	  ps->gfx_RectangleFilled(myXpos, myYpos, myXpos + myWidth, myYpos + myHeight + 3, c);

  }
  else
  {
	myIsVisible = true;

	this->draw();
  }
}
//----------------------------------------------------------
void Scaler::setColorBkg(Color colorBkg)
{
  myColorBkg = colorBkg;
  this->draw();
}
//---------------------------------------------------------
void Scaler::setColorCursorUp(Color colorUp)
{
  myColorUp = colorUp;
  this->draw();
}
//-------------------------------------------------------------
void Scaler::setColorCursorDown(Color colorDown)
{
  myColorDown = colorDown;
  this->draw();
}
//------------------------------------------------------------
float Scaler::getValue(GraphicMessage* msg)
{
  if (myIsVisible == false)
	return 0.0;
  //
  float val = 0.0;
  word ev = msg->event;
  word yn = msg->posiY;

  word y0 = myYpos;
  word ybut1 = y0 + myHeight / 2;
  word ybut2 = y0 + myHeight;

  Color couleurUp = myColorUp;
  Color couleurDown = myColorDown;

  if (ev == EVENT_PRESSED && myState == SCALER_STATE_RELEASED)
  {
	//up?
	if (yn >= y0 && yn <= ybut1)
	{
	  myState = SCALER_STATE_PRESSED_UP;
	  setColorCursorUp(WHITE);
	  delay(100);
	  setColorCursorUp(couleurUp);

	  val = myValue + myValInc;
	  if (val > myValMax)
		val = myValMax;

	  myValue = val;
	}
	//
	//down?
	if (yn >= ybut1 && yn <= ybut2)
	{
	  myState = SCALER_STATE_PRESSED_DOWN;
	  setColorCursorDown(WHITE);
	  delay(100);
	  setColorCursorDown(couleurDown);

	  val = myValue - myValInc;
	  if (val < myValMin)
		val = myValMin;

	  myValue = val;
	}
  }
  //
  if (ev == EVENT_RELEASED || ev == EVENT_MOVE)
  {
	myState = SCALER_STATE_RELEASED;
  }
  return myValue;
}
//---------------------------------------------------------------
DigitLed::DigitLed(Screen* screen, word xpos, word ypos,
				   bool ptVisible): Widget(screen, 0, xpos, ypos, 36, 39, WHITE)
{
  this->myValue = 0;
  this->myColorBkg = YELLOW;
  this->myColorSeg = RED;
  this->myIncX = 4; //9*4=36 pixels
  this->myIncY = 3; //3*13=39
  this->myPtVisible = ptVisible;

  this->myType = OBJ_DIGIT_LED;
  this->myState = LED_STATE_ON;

  myScreen->addWidget((Widget*)this);
}
//----------------------------------------------------------------
DigitLed::~DigitLed(void)
{
}
//-------------------------------------------------------------------
void DigitLed::draw(void)
{
  if (myIsVisible == false)
	return;
  //
  ps = myScreen->getPicasoSerial();
  //rectangle
  if (ps != 0)
	ps->gfx_RectangleFilled(myXpos, myYpos, myXpos + myWidth, myYpos + myHeight, myColorBkg);

  //
  if (myPtVisible)
	draw_seg_pt(ps);

  switch (myValue)
  {
	case 0:
	  draw_seg_a(ps);
	  draw_seg_b(ps);
	  draw_seg_c(ps);
	  draw_seg_d(ps);
	  draw_seg_e(ps);
	  draw_seg_f(ps);
	  break;
	//
	case 1:
	  draw_seg_b(ps);
	  draw_seg_c(ps);
	  break;
	//
	case 2:
	  draw_seg_a(ps);
	  draw_seg_b(ps);
	  draw_seg_d(ps);
	  draw_seg_e(ps);
	  draw_seg_g(ps);
	  break;
	//
	case 3:
	  draw_seg_a(ps);
	  draw_seg_b(ps);
	  draw_seg_c(ps);
	  draw_seg_d(ps);
	  draw_seg_g(ps);
	  break;
	//
	case 4:
	  draw_seg_b(ps);
	  draw_seg_c(ps);
	  draw_seg_f(ps);
	  draw_seg_g(ps);
	  break;
	//
	case 5:
	  draw_seg_a(ps);
	  draw_seg_c(ps);
	  draw_seg_d(ps);
	  draw_seg_f(ps);
	  draw_seg_g(ps);
	  break;
	//
	case 6:
	  draw_seg_a(ps);
	  draw_seg_c(ps);
	  draw_seg_d(ps);
	  draw_seg_e(ps);
	  draw_seg_f(ps);
	  draw_seg_g(ps);
	  break;
	//
	case 7:
	  draw_seg_a(ps);
	  draw_seg_b(ps);
	  draw_seg_c(ps);
	  break;
	//
	case 8:
	  draw_seg_a(ps);
	  draw_seg_b(ps);
	  draw_seg_c(ps);
	  draw_seg_d(ps);
	  draw_seg_e(ps);
	  draw_seg_f(ps);
	  draw_seg_g(ps);
	  break;
	//
	case 9:
	  draw_seg_a(ps);
	  draw_seg_b(ps);
	  draw_seg_c(ps);
	  draw_seg_d(ps);
	  draw_seg_f(ps);
	  draw_seg_g(ps);
	  break;
	//
	default:
	  break;
  }
}
//------------------------------------------------------------------
void DigitLed::setInvisible(bool visu)
{
  if (visu == true)
  {
	if (myIsVisible == false)
	  return;
	//
	myIsVisible = false;

	Color c = myScreen->getColorBkg();

	ps = myScreen->getPicasoSerial();

	if (ps != 0)
	  ps->gfx_RectangleFilled(myXpos, myYpos, myXpos + myWidth, myYpos + myHeight, c);
  }
  //
  else
  {
	myIsVisible = true;
	this->draw();
  }
}
//-------------------------------------------------------------------
void DigitLed::setColorBkg(Color colorBkg)
{
  if (myColorBkg == colorBkg)
	return;
  //
  myColorBkg = colorBkg;
  this->draw();
}
//-------------------------------------------------------------------
void DigitLed::setColorSeg(Color colorSeg)
{
  if (myColorSeg == colorSeg)
	return;
  //
  myColorSeg = colorSeg;
  this->draw();
}
//--------------------------------------------------------------------
void DigitLed::setValue(char value)
{
  if (myIsVisible == false)
	return;
  //
  if (myValue == value)
	return;
  //
  if (value > 9)
	value = 9;
  //
  myValue = value;
  this->draw();
}
//---------------------------------------------------------------------
void DigitLed::setPtVisible(bool state)
{
  if (myPtVisible == state)
	return;

  myPtVisible = state;
}
//----------------------------------------------------------------------
void DigitLed::draw_seg_a(PicasoSerial* ps)
{
  if (ps != 0)
	ps->gfx_RectangleFilled(myXpos + myIncX,
							myYpos,
							myXpos + myWidth - (2 * myIncX),
							myYpos + myIncY,
							myColorSeg);
}
//----------------------------------------------------------------------
void DigitLed::draw_seg_b(PicasoSerial* ps)
{
  if (ps != 0)
	ps->gfx_RectangleFilled(myXpos + myWidth - (3 * myIncX),
							myYpos,
							myXpos + myWidth - (2 * myIncX),
							myYpos + myHeight - (8 * myIncY),
							myColorSeg);
}
//----------------------------------------------------------------------
void DigitLed::draw_seg_c(PicasoSerial* ps)
{
  if (ps != 0)
	ps->gfx_RectangleFilled(myXpos + myWidth - (3 * myIncX),
							myYpos + myHeight - (5 * myIncY),
							myXpos + myWidth - (2 * myIncX),
							myYpos + myHeight,
							myColorSeg);
}
//----------------------------------------------------------------------
void DigitLed::draw_seg_d(PicasoSerial* ps)
{
  if (ps != 0)
	ps->gfx_RectangleFilled(myXpos + myIncX,
							myYpos + myHeight - myIncY,
							myXpos + myWidth - (2 * myIncX),
							myYpos + myHeight,
							myColorSeg);
}
//-----------------------------------------------------------------------
void DigitLed::draw_seg_e(PicasoSerial* ps)
{
  if (ps != 0)
	ps->gfx_RectangleFilled(myXpos + myIncX,
							myYpos + myHeight - (5 * myIncY),
							myXpos + 2 * myIncX,
							myYpos + myHeight,
							myColorSeg);
}
//------------------------------------------------------------------------
void DigitLed::draw_seg_f(PicasoSerial* ps)
{
  if (ps != 0)
	ps->gfx_RectangleFilled(myXpos + myIncX,
							myYpos,
							myXpos + (2 * myIncX),
							myYpos + myHeight - (8 * myIncY),
							myColorSeg);
}
//------------------------------------------------------------------------
void DigitLed::draw_seg_g(PicasoSerial* ps)
{
  if (ps != 0)
	ps->gfx_RectangleFilled(myXpos + myIncX,
							myYpos + myHeight - (7 * myIncY),
							myXpos + myWidth - (2 * myIncX),
							myYpos + myHeight - (6 * myIncY),
							myColorSeg);
}
//------------------------------------------------------------------------
void DigitLed::draw_seg_pt(PicasoSerial* ps)
{
  if (ps != 0)
	ps->gfx_RectangleFilled(myXpos + myWidth - myIncX,
							myYpos + myHeight - myIncY,
							myXpos + myWidth,
							myYpos + myHeight,
							myColorSeg);
}
//----------------------------------------------------------------------
Digit3Led::Digit3Led(Screen* screen, word xpos, word ypos,
					 char resolution): Widget(screen, 0, xpos, ypos, 110, 40, YELLOW)
{
  this->myColorBkg = YELLOW;
  this->myColorSeg = RED;
  this->myResolution = resolution;
  this->myValue = 0.0;

  this->myType = OBJ_DIGIT_3_LED;
  this->myState = LED_STATE_ON;

  this->myDigitLedCentaine = new DigitLed(screen, xpos, ypos, false);
  word x = myDigitLedCentaine->getXposition() + myDigitLedCentaine->getWidth();
  //
  this->myDigitLedDizaine = new DigitLed(screen, x, ypos, false);
  x = myDigitLedDizaine->getXposition() + myDigitLedDizaine->getWidth();

  this->myDigitLedUnite = new DigitLed(screen, x, ypos, false);

  myScreen->addWidget((Widget*)this);
}
//------------------------------------------------------------------------
Digit3Led::~Digit3Led(void)
{
  delete(myDigitLedDizaine);
  myDigitLedCentaine = 0;

  delete(myDigitLedDizaine);
  myDigitLedDizaine = 0;

  delete(myDigitLedUnite);
  myDigitLedUnite = 0;
}
//------------------------------------------------------------------------
void Digit3Led::draw(void)
{
  if (myIsVisible == false)
	return;
  //
  myDigitLedCentaine->draw();
  myDigitLedDizaine->draw();
  myDigitLedUnite->draw();
}
//------------------------------------------------------------------------
void Digit3Led::setInvisible(bool visu)
{
  if (visu == true)
  {
	if (myIsVisible == false)
	  return;

	//
	myIsVisible = false;
	Color c = myScreen->getColorBkg();

	ps = myScreen->getPicasoSerial();

	if (ps != 0)
	  ps->gfx_RectangleFilled(myXpos, myYpos, myXpos + myWidth, myYpos + myHeight, c);
  }
  else
  {
	myIsVisible = true;

	this->draw();
  }
}
//--------------------------------------------------------------------------
void Digit3Led::setColorBkg(Color colorBkg)
{
  if (myColorBkg == colorBkg)
	return;
  //
  myColorBkg = colorBkg;
  myDigitLedCentaine->setColorBkg(colorBkg);
  myDigitLedDizaine->setColorBkg(colorBkg);
  myDigitLedUnite->setColorBkg(colorBkg);

  this->draw();
}
//---------------------------------------------------------------------------
void Digit3Led::setColorSeg(Color colorSeg)
{
  if (myColorSeg == colorSeg)
	return;
  //
  myColorSeg = colorSeg;
  myDigitLedCentaine->setColorSeg(colorSeg);
  myDigitLedDizaine->setColorSeg(colorSeg);
  myDigitLedUnite->setColorSeg(colorSeg);

  this->draw();
}
//------------------------------------------
void Digit3Led::setResolution(int resolution)
{
	this->myResolution=resolution;
}
//---------------------------------------
void Digit3Led::setValue(float value)
{
  if (myIsVisible == false)
	return;
  //
  if (myValue == value)
	return;
  //
  myValue = value;
  word temp = 0;
  char centaine = 0;
  char dizaine = 0;
  char unite = 0;

  //pt ?
  switch (myResolution)
  {
	case 1: //xxx
	  temp = (uint16_t)value;
	  myDigitLedCentaine->setPtVisible(false);
	  myDigitLedDizaine->setPtVisible(false);
	  myDigitLedUnite->setPtVisible(false);
	  break;
	//
	case 10:    //xx.x
	  temp = (uint16_t)(value * 10);
	  myDigitLedCentaine->setPtVisible(false);
	  myDigitLedDizaine->setPtVisible(true);
	  myDigitLedUnite->setPtVisible(false);
	  break;
	//
	case 100:   //x.xx
	  temp = (uint16_t)(value * 100);
	  myDigitLedCentaine->setPtVisible(true);
	  myDigitLedDizaine->setPtVisible(false);
	  myDigitLedUnite->setPtVisible(false);
	  break;
	//
	default:
	  break;
  }
  /////////////////////
  if (temp < 100)
  {
	centaine = 0;
	//myDigitLedCentaine->setInvisible(true);
  }
  else
  {
	//myDigitLedCentaine->setInvisible(false);
	centaine = (char)(temp / 100);
	temp = temp - (centaine * 100);
  }
  ////////////////////////
  if (temp < 10)
  {
	dizaine = 0;
  }
  else
  {
	dizaine = (char)(temp / 10);
	temp = temp - (dizaine * 10);
  }
  ////////////////////////
  unite = (char)temp;

  myDigitLedCentaine->setValue(centaine);
  myDigitLedDizaine->setValue(dizaine);
  myDigitLedUnite->setValue(unite);

  this->draw();
}
//-----------------------------------------------------------------------------
Digit4Led::Digit4Led(Screen* screen, word xpos, word ypos, int resolution): Widget(screen, 0, xpos, ypos, 146, 40, YELLOW)
{
  this->myColorBkg = YELLOW;
  this->myColorSeg = RED;
  this->myResolution = resolution;
  this->myValue = 0.0;

  this->myType = OBJ_DIGIT_4_LED;
  this->myState = LED_STATE_ON;

  this->myDigitLedMillieme = new DigitLed(screen, xpos, ypos, false);
  word x = myDigitLedMillieme->getXposition() + myDigitLedMillieme->getWidth();

  this->myDigitLedCentaine = new DigitLed(screen, x, ypos, false);
  x = myDigitLedCentaine->getXposition() + myDigitLedCentaine->getWidth();
  //
  this->myDigitLedDizaine = new DigitLed(screen, x, ypos, false);
  x = myDigitLedDizaine->getXposition() + myDigitLedDizaine->getWidth();

  this->myDigitLedUnite = new DigitLed(screen, x, ypos, false);

  myScreen->addWidget((Widget*)this);
}
//-----------------------------------------------
 Digit4Led:: ~Digit4Led(void)
{
  delete(myDigitLedMillieme);
  myDigitLedMillieme=0;

  delete(myDigitLedDizaine);
  myDigitLedCentaine = 0;

  delete(myDigitLedDizaine);
  myDigitLedDizaine = 0;

  delete(myDigitLedUnite);
  myDigitLedUnite = 0;
}
//-----------------------------------------------
void Digit4Led::draw(void)
{
	if (myIsVisible == false)
		return;
  //
  myDigitLedMillieme->draw();
  myDigitLedCentaine->draw();
  myDigitLedDizaine->draw();
  myDigitLedUnite->draw();
}
//------------------------------------------------
void Digit4Led::setInvisible(bool visu)
{
	if (visu == true)
 	 {
   		 if (myIsVisible == false)
	  	return;

	//
	myIsVisible = false;
	Color c = myScreen->getColorBkg();

	ps = myScreen->getPicasoSerial();

	if (ps != 0)
	  ps->gfx_RectangleFilled(myXpos, myYpos, myXpos + myWidth, myYpos + myHeight, c);
  }
  else
  {
	myIsVisible = true;

	this->draw();
  }
}
//------------------------------------------------
void Digit4Led::setColorBkg(Color colorBkg)
{
if (myColorBkg == colorBkg)
	return;
  //
  myColorBkg = colorBkg;
  myDigitLedMillieme->setColorBkg(colorBkg);
  myDigitLedCentaine->setColorBkg(colorBkg);
  myDigitLedDizaine->setColorBkg(colorBkg);
  myDigitLedUnite->setColorBkg(colorBkg);

  this->draw();
}
//-------------------------------------------------
void Digit4Led::setColorSeg(Color colorSeg)
{
if (myColorSeg == colorSeg)
	return;
  //
  myColorSeg = colorSeg;
  myDigitLedMillieme->setColorSeg(colorSeg);
  myDigitLedCentaine->setColorSeg(colorSeg);
  myDigitLedDizaine->setColorSeg(colorSeg);
  myDigitLedUnite->setColorSeg(colorSeg);

  this->draw();
}
//-----------------------------------------------
void Digit4Led::setResolution(int resolution)
{
	this->myResolution=resolution;
}
//------------------------------------------------
void Digit4Led::setValue(float value)
{
if (myIsVisible == false)
	return;
  //
  if (myValue == value)
	return;
  //
  myValue = value;
  word temp = 0;
  char millieme = 0;
  char centaine = 0;
  char dizaine = 0;
  char unite = 0;

  //pt ?
  switch (myResolution)
  {
	case 1: //xxxx
	  temp = (uint16_t)value;
	  myDigitLedMillieme->setPtVisible(false);
	  myDigitLedCentaine->setPtVisible(false);
	  myDigitLedDizaine->setPtVisible(false);
	  myDigitLedUnite->setPtVisible(false);
	  break;
	//
	case 10:    //xxx.x
	  temp = (uint16_t)(value * 10);
	  myDigitLedMillieme->setPtVisible(false);
	  myDigitLedCentaine->setPtVisible(false);
	  myDigitLedDizaine->setPtVisible(true);
	  myDigitLedUnite->setPtVisible(false);
	  break;
	//
	case 100:   //xx.xx
	  temp = (uint16_t)(value * 100);
	  myDigitLedMillieme->setPtVisible(false);
	  myDigitLedCentaine->setPtVisible(true);
	  myDigitLedDizaine->setPtVisible(false);
	  myDigitLedUnite->setPtVisible(false);
	  break;
	//
	case 1000:	//x.xxx
	  temp = (uint16_t)(value * 1000);
	  myDigitLedMillieme->setPtVisible(true);
	  myDigitLedCentaine->setPtVisible(false);
	  myDigitLedDizaine->setPtVisible(false);
	  myDigitLedUnite->setPtVisible(false);
	  break;
	//
	default:
	  break;
  }
  //////////////////////
  if (temp < 1000)
  {
	 millieme = 0;
  }
  else
  {
	
	millieme = (char)(temp / 1000);
	temp = temp - (millieme * 1000);
  }
  /////////////////////
  if (temp < 100)
  {
	centaine = 0;
  }
  else
  {
	centaine = (char)(temp / 100);
	temp = temp - (centaine * 100);
  }
  ////////////////////////
  if (temp < 10)
  {
	dizaine = 0;
  }
  else
  {
	dizaine = (char)(temp / 10);
	temp = temp - (dizaine * 10);
  }
  ////////////////////////
  unite = (char)temp;

  myDigitLedMillieme->setValue(millieme);
  myDigitLedCentaine->setValue(centaine);
  myDigitLedDizaine->setValue(dizaine);
  myDigitLedUnite->setValue(unite);

  this->draw();
}
//----------------------------------------------
Panel::Panel(Screen* screen,word xpos, word ypos,word width,word height,Color color):Widget(screen, 0, xpos, ypos, width, height,color)
{
	this->myType = OBJ_PANEL;
	this->myState = STATE_ON;
	myScreen->addWidget((Widget*)this);
}
//------------
Panel:: ~Panel(void)
 {
 }
//---------------------
void Panel::draw(void)
{
	if (myIsVisible == false)
	return;
  //
  ps = myScreen->getPicasoSerial();

  ps->gfx_Panel(PANEL_RAISED,myXpos,myYpos,myWidth,myHeight,myColor);

}
//---------------------------------
void Panel::setInvisible(bool visu)
{
	if (visu == true)
	{
		if (myIsVisible == false)
		return;
		myIsVisible = false;
		Color c = myScreen->getColorBkg();

		ps = myScreen->getPicasoSerial();

		if (ps != 0)
		ps->gfx_RectangleFilled(myXpos, myYpos, myXpos + myWidth, myYpos + myHeight, c);
	}
  else
  {
	myIsVisible = true;
	this->draw();
  }
}
//----------------------------------------------
//fin
