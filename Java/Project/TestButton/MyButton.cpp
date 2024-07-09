#include "MyButton.h"

void CircleButton::newButton(int x,int y,int radius,const char* String){
	struct Circle btn;
	btn.id = button.size();
	btn.x = x;
	btn.y = y;
	btn.radius = radius;
	btn.normalColor  = EGEARGB(0xFF, 0x40, 0xE0, 0xD0);
	btn.pressedColor = EGEARGB(0xFF, 0x1E, 0x90, 0xFF);
	btn.text = String;
	button.push_back(btn);
	initButtonState(button.back());
}

void CircleButton::initButtonState(struct Circle& button){
	button.state.pressed = false;
	button.state.hovered = false;
	button.state.focused = false;
}

bool CircleButton::insideButton(struct Circle& button,int x,int y){
	int dx = x - button.x, dy = y - button.y;
	return (dx * dx + dy * dy) <= (button.radius * button.radius);
}

void CircleButton::DrawButton(){
	setfillcolor(EGEARGB(0xFF, 0x1E, 0x90, 0xFF));
	setcolor(WHITE);
	settextjustify(CENTER_TEXT, CENTER_TEXT);
	setfont(24, 0, "");
	color_t color;
	for (auto buttons:button){
		if (buttons.state.pressed) color = buttons.pressedColor;
		else color = buttons.normalColor;
		if (buttons.state.hovered) color = interpColor(color, BLACK, 0.125f);
		setfillcolor(color);
		ege_fillellipse(buttons.x - buttons.radius, buttons.y - buttons.radius,2 * buttons.radius, 2 * buttons.radius);
		if (buttons.state.focused) {
			setcolor(interpColor(color, BLACK, 0.25f));
			setlinewidth(4);
			int borderRadius = buttons.radius + 4;
			ege_ellipse(buttons.x - borderRadius, buttons.y - borderRadius,2 * borderRadius, 2 * borderRadius);
		}
		setcolor(WHITE);
		xyprintf(buttons.x, buttons.y, "%s", buttons.text);
	}
}

color_t CircleButton::interpColor(color_t colorA, color_t colorB, float t){
	if (t > 1) t = 1;
	else if (t < 0) t = 0;
	int r = roundf((1 - t) * EGEGET_R(colorA) + t * EGEGET_R(colorB));
	int g = roundf((1 - t) * EGEGET_G(colorA) + t * EGEGET_G(colorB));
	int b = roundf((1 - t) * EGEGET_B(colorA) + t * EGEGET_B(colorB));
	return EGERGB(r, g, b);
}

int CircleButton::SearchButton(int x,int y){
	int n=button.size();
	for (int i=0;i<n;i++){
		if (insideButton(button[i],x,y)) return i;
	}
	return -1;
}

void CircleButton::PutButton(){
	bool mouseMoved = false;
	while (mousemsg()){
		mouse_msg msg=getmouse();
		if (msg.is_left()){
			if (msg.is_down()){
				int btnId=0;
				btnId=SearchButton(msg.x,msg.y);
				if (btnId>-1){
					PressId=btnId;
					button[btnId].state.pressed=true;
					redraw=1;
				}
			}
			else {
				if(PressId>-1){
					int btn=PressId;
					if (FocusId>-1) button[FocusId].state.focused=false;
					button[btn].state.focused=true;
					FocusId=btn;
					button[btn].state.pressed=false;
					PressId=-1;
					redraw=1;
				}
			}
		}
		else if (msg.is_move()) {
			mouseMoved = true;
			xMouse = msg.x;
			yMouse = msg.y;
		}
		if (mouseMoved) {
			int btn=SearchButton(xMouse,yMouse);
			if (btn!=HoverId) {
				button[HoverId].state.hovered=false;
				if (btn>-1) button[btn].state.hovered=true;
				HoverId=btn;
				redraw=1;
			}
		}
		if (redraw) {
			cleardevice();
			DrawButton();
			redraw = false;
		}
	}
}

void RectangleButton::newButton(int x,int y,int height,int width,const char* String){
	struct NormalRectangle btn;
	btn.id = button.size();
	btn.x=x;
	btn.y=y;
	btn.height=height;
	btn.width=width;
	btn.normalColor = EGEARGB(0xFF, 0x40, 0xE0, 0xD0);
	btn.pressedColor = EGEARGB (0xFF, 0x1E, 0x90, 0xFF);
	btn.text=String;
	button.push_back(btn);
	initButtonState(button.back());
}

void RectangleButton::initButtonState(struct NormalRectangle& button){
	button.state.pressed = false;
	button.state.hovered = false;
	button.state.focused = false;
}

bool RectangleButton::insideButton(struct NormalRectangle& button,int x,int y){
	return (x>=button.x&&y>=button.y&&x<=button.x+button.width&&y<=button.y+button.height);
}

void RectangleButton::DrawButton(){
	setfillcolor(EGEARGB(0xFF, 0x1E, 0x90, 0xFF));
	setcolor(WHITE);
	settextjustify(CENTER_TEXT, CENTER_TEXT);
	setfont(24, 0, "");
	color_t color;
	for (auto buttons:button){
		if (buttons.state.pressed) color = buttons.pressedColor;
		else color = buttons.normalColor;
		if (buttons.state.hovered) color = interpColor(color, BLACK, 0.125f);
		setfillcolor(color);
		ege_fillrect(buttons.x,buttons.y,buttons.x+buttons.width,buttons.y+buttons.height);
		if (buttons.state.focused) {
			setcolor(interpColor(color, BLACK, 0.25f));
			setlinewidth(4);
			ege_fillrect(buttons.x-4,buttons.y-4,buttons.x+buttons.width+4,buttons.y+buttons.height+4);
		}
		setcolor(WHITE);
		xyprintf(buttons.x+buttons.width, buttons.y+buttons.height, "%s", buttons.text);
	}
}
