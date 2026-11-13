#include <graphics.h>
#include <math.h>

struct ButtonStates
{
	bool enable;	/* 可用状态 */
	bool visible;	/* 可见状态 */
	bool pressed;	/* 按下状态 */
	bool checkable; /* 可选状态 */
	bool checked;	/* 选中状态 */
	bool hovered;	/* 悬停状态 */
	bool focused;	/* 焦点状态 */
};

// 圆形按钮
struct CircleButton
{
	unsigned int id;
	int x, y;		/* 圆心*/
	int radius;		/* 半径*/
	color_t normalColor;
	color_t pressedColor;
	const char* text;
	ButtonStates state;
};

void initButton(CircleButton* button, int x, int y, int radius);

void initAllButtons();

//初始化按钮状态
void initButtonState(CircleButton* button);

// 判断点(x, y)是否在按钮点击区域内部
bool insideButton(const CircleButton* button, int x, int y);


CircleButton* findButton(unsigned int id);

// 绘制按钮
void drawButton(const CircleButton* buttonArray);

// 绘制所有按钮
void drawAllButtons();

// 查找 (x, y) 所在的按钮，返回按钮ID, 没有返回 -1
int searchButton(int x, int y);

// 绘制
void draw();

// 颜色插值, t 范围为 (0.0 ~ 1.0)，分别对应colorA和colorB
color_t interpColor(color_t colorA, color_t colorB, float t);

#define BUTTON_SIZE		8
#define BUTTON_ID_ZERO	((unsigned int)0)
#define BUTTON_MIN_ID	1
#define BUTTON_MAX_ID	BUTTON_SIZE

//定义按钮，确定区域
CircleButton buttonArray[BUTTON_SIZE];
int buttonNum = BUTTON_SIZE;

// 按下按钮ID
int pressButtonId   = BUTTON_ID_ZERO;
int hoveredButtonId = BUTTON_ID_ZERO;
int focusedButtonId = BUTTON_ID_ZERO;

int windowWidth = 640, windowHeight = 480;

int main()
{
	
	initgraph(windowWidth, windowHeight, INIT_RENDERMANUAL);
	setbkcolor(WHITE);
	setbkmode(TRANSPARENT);
	ege_enable_aa(true);
	
	initAllButtons();
	
	int xMouse = 0, yMouse = 0;
	bool redraw = true;
	
	for (; is_run(); delay_fps(60)) {
		
		bool mouseMoved = false;
		
		while (mousemsg()) {
			mouse_msg msg = getmouse();
			
			// 判断鼠标左键按下（左键按下确定位置，同时判断是否为按钮区域
			// 抬起则解除按下状态
			if (msg.is_left()) {
				if (msg.is_down()) {
					
					int btnId = BUTTON_ID_ZERO;
					
					// 检查位置对应的按钮ID
					btnId = searchButton(msg.x, msg.y);
					
					if ((btnId != BUTTON_ID_ZERO) && (findButton(btnId)->state.enable)) {
						// 将被按下的按钮设置为按下状态
						if (btnId != BUTTON_ID_ZERO) {
							pressButtonId = btnId;
							findButton(pressButtonId)->state.pressed = true;
							redraw = true;
						}
					}
				}
				else {
					//左键抬起，如果有按钮被按下
					if (pressButtonId != BUTTON_ID_ZERO) {
						unsigned btnId = pressButtonId;
						CircleButton* button = findButton(btnId);
						
						// 取消之前按钮的焦点状态
						if (focusedButtonId != BUTTON_ID_ZERO)
						{
							findButton(focusedButtonId)->state.focused = false;
						}
						
						//设置当前按钮为焦点状态
						button->state.focused = true;
						focusedButtonId = btnId;
						
						// 解除按下状态
						button->state.pressed = false;
						pressButtonId = BUTTON_ID_ZERO;
						
						// 切换选中
						if (button->state.checkable)
							button->state.checked = !(button->state.checked);
						
						redraw = true;
					}
				}
			}
			else if (msg.is_move()) {
				mouseMoved = true;
				xMouse = msg.x;
				yMouse = msg.y;
			}
		}
		
		//当鼠标移动
		if (mouseMoved) {
			//查找悬停的按钮
			int btnId = searchButton(xMouse, yMouse);
			
			// 悬停按钮改变
			if (btnId != hoveredButtonId) {
				// 标记原先的按钮为非悬停状态
				if (hoveredButtonId != BUTTON_ID_ZERO) {
					findButton(hoveredButtonId)->state.hovered = false;
				}
				
				// 标记当前悬停的按钮为悬停状态
				if (btnId != BUTTON_ID_ZERO) {
					findButton(btnId)->state.hovered = true;
				}
				
				hoveredButtonId = btnId;
				redraw = true;
			}
		}
		
		// 判断是否需要重绘，减少不必要的绘制操作
		if (redraw) {
			cleardevice();
			draw();
			redraw = false;
		}
	}
	
	return 0;
}


bool insideButton(const CircleButton* button, int x, int y)
{
	int dx = x - button->x, dy = y - button->y;
	return (dx * dx + dy * dy) <= (button->radius * button->radius);
}

unsigned int generateId()
{
	static unsigned int id = 0;
	return ++id;
}

CircleButton* findButton(unsigned int id)
{
	return (id == BUTTON_ID_ZERO) ? NULL : &buttonArray[id - 1];
}

unsigned int getButtonId(const CircleButton* button)
{
	return button->id;
}

// 绘制按钮
void drawButton(const CircleButton* button)
{
	if (button != NULL) {
		if (button->state.visible) {
			setfillcolor(EGEARGB(0xFF, 0x1E, 0x90, 0xFF));
			setcolor(WHITE);
			settextjustify(CENTER_TEXT, CENTER_TEXT);
			setfont(24, 0, "");
			
			//根据状态进行不同的绘制
			color_t color;

			if (!(button->state.enable)) {
				color = GRAY;
			}
			else
			{
				if (button->state.pressed) {
					color = button->pressedColor;
				}
				else {
					color = button->normalColor;
				}
				
				if (button->state.hovered) {
					color = interpColor(color, BLACK, 0.125f);
				}
			}
			
			setfillcolor(color);
			
			// 绘制按钮
			ege_fillellipse(button->x - button->radius, button->y - button->radius,
				2 * button->radius, 2 * button->radius
				);
			
			// 选中则绘制外环边框
			if (button->state.focused)
			{
				setcolor(interpColor(color, BLACK, 0.25f));
				setlinewidth(4);
				
				int borderRadius = button->radius + 4;
				
				ege_ellipse(button->x - borderRadius, button->y - borderRadius,
					2 * borderRadius, 2 * borderRadius
					);
			}
			
			
			int radiusCheckBox = 14;
			int xCheckBox = button->x - button->radius - 2 * radiusCheckBox;
			int yCheckBox = button->y;
			
			if (button->state.checked) {
				setfillcolor(button->pressedColor);
				ege_fillellipse(xCheckBox - radiusCheckBox, yCheckBox - radiusCheckBox,
					2 * radiusCheckBox, 2 * radiusCheckBox
					);
			}
			else {
				setcolor(button->pressedColor);
				setlinewidth(2);
				ege_ellipse(xCheckBox - radiusCheckBox, yCheckBox - radiusCheckBox,
					2 * radiusCheckBox, 2 * radiusCheckBox
					);
			}
			
			setcolor(WHITE);
			xyprintf(button->x, button->y, "%s", button->text);
		}
		else {
			setcolor(BLACK);
			settextjustify(CENTER_TEXT, CENTER_TEXT);
			setfont(24, 0, "");
			xyprintf(button->x, button->y, "不可见");
		}
	}
}


// 绘制所有按钮
void drawAllButtons()
{
	for (int i = 0; i < buttonNum; i++) {
		unsigned int btnId = BUTTON_MIN_ID + i;
		drawButton(findButton(btnId));
	}
}

int searchButton(int x, int y)
{
	int buttonId = BUTTON_ID_ZERO;
	
	for (int i = 0; i < buttonNum; i++) {
		unsigned int btnId = BUTTON_MIN_ID + i;
		CircleButton* button = findButton(btnId);
		
		if ((button != NULL) && (insideButton(button, x, y))) {
			buttonId = btnId;
			break;   //退出，已经检测到，后面的按钮不再检测
		}
	}
	
	return buttonId;
}

void draw()
{
	//绘制
	drawAllButtons();
	setcolor(BLACK);
	setfont(24, 0, "");
	settextjustify(LEFT_TEXT, TOP_TEXT);
	xyprintf(240, 360, "按下按钮ID:%d", pressButtonId);
}

color_t interpColor(color_t colorA, color_t colorB, float t)
{
	if (t > 1)
		t = 1;
	else if (t < 0)
		t = 0;
	
	int r = roundf((1 - t) * EGEGET_R(colorA) + t * EGEGET_R(colorB));
	int g = roundf((1 - t) * EGEGET_G(colorA) + t * EGEGET_G(colorB));
	int b = roundf((1 - t) * EGEGET_B(colorA) + t * EGEGET_B(colorB));
	
	return EGERGB(r, g, b);
}


void initButtonState(CircleButton* button)
{
	button->state.enable = true;
	button->state.visible = true;
	button->state.pressed = false;
	button->state.checkable = true;
	button->state.checked = false;
	button->state.hovered = false;
	button->state.focused = false;
}

void initButton(CircleButton* button, int x, int y, int radius)
{
	if (button != NULL) {
		button->id = generateId();
		button->x = x;
		button->y = y;
		button->radius = radius;
		button->normalColor  = EGEARGB(0xFF, 0x40, 0xE0, 0xD0);
		button->pressedColor = EGEARGB(0xFF, 0x1E, 0x90, 0xFF);
		button->text = "Button";
		
		initButtonState(button);
	}
}

void initAllButtons()
{
	int colNum = 2;
	
	for (int i = 0; i < buttonNum; i++) {
		int id = BUTTON_MIN_ID + i;
		int x = (int)(((i % colNum) + 0.5f) * (windowWidth / colNum));
		int y = (int)(((i / colNum) + 0.5f) * (windowHeight / ((buttonNum + colNum - 1) / colNum)));
		
		initButton(findButton(id), x, y, 50);
	}
	
	CircleButton* btn;
	btn = findButton(BUTTON_MIN_ID);
	btn->state.enable = false;
	btn->text = "不可用";
	
	btn = findButton(BUTTON_MIN_ID + 1);
	btn->state.visible = false;
	
	btn = findButton(BUTTON_MIN_ID + 2);
	btn->state.checkable = false;
	btn->state.checked = false;
	btn->state.focused = true;
	btn->text = "不可选";
	
	btn = findButton(BUTTON_MIN_ID + 3);
	btn->state.checkable = false;
	btn->state.checked = true;
	btn->text = "不可选";
	
	btn = findButton(BUTTON_MIN_ID + 4);
	btn->state.pressed = true;
	
	btn = findButton(BUTTON_MIN_ID + 5);
	btn->state.checked = true;
	
	btn = findButton(BUTTON_MIN_ID + 6);
	btn->state.pressed = true;
	btn->state.checked = true;
	
	focusedButtonId = BUTTON_MIN_ID + 2;
}
