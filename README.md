DOS版切水果
课程名称：程序设计综合实验
指导教师：白洪欢
组长：张扬
组员：周若昕
	  贾文珊
问题背景：
手机版的水果忍者游戏前段时间非常热门，我们也可以做一个电脑上的切水果游戏。屏幕上随机上升几个水果，玩家按下鼠标左键，然后划过这些水果，就可以得分。
游戏功能：
根据我们预期的游戏效果，本游戏需要有如下功能：（*为高级功能）
1.	显示背景图片。
2.	水果上升，旋转，符合运动学规律。
3.	鼠标单击左键，屏幕产生一段持续的划痕。
4.	水果被切时，产生爆炸效果。
5.	记分系统。
6.	水果被切声音，背景音乐*。
7.	有炸弹*。
8.	连击效果*。
9.	冰冻、狂热、X2奖励*。
10.	可以输入秘笈*。
11.	3种游戏模式：街机，禅模式,*。
游戏架构设计：
开发环境： Sublime文本编辑器编写源文件，传至github进行团队开发及版本控制，DOSBOX+TC编译，运行于1024x768 8bit分辨率。
模块：
根据以上预期的功能，我们将本游戏分为以下模块：
1.	图形显示
根据物体位置、角度将物体图片显示到屏幕上。
显示背景图片。
冰冻效果。
鼠标轨迹。
2.	物理引擎
根据运动学规律计算物体位置、角度。
3.	游戏逻辑
随机产生水果、炸弹、奖励。
切到水果、炸弹、奖励香蕉时产生相应的事件。
冰冻、狂热、X2奖励。
鼠标轨迹控制。
计分。
FPS控制。
连击。
4.	声音。
背景音乐。
切到物体是产生声音。
5.	鼠标、键盘控制
鼠标中断
键盘中断
6.	游戏UI
游戏welcome UI
游戏进行UI
数据结构：
1.	全局变量
int USER_POINT;
int FPS;
short int CURRENT_UI;  enum UITYPE{WELCOME_UI,GAME_UI};
struct objects *SCREEN_OBJ;
struct events *GAME_EVENTS;
2.	single object（水果，炸弹）struct
struct object{
		short int type;    enum OBJTYPE
		int x;
		int y;
		int angle;	
		int rateX;
int rateY;
int angleRate;
char *title;
}
3.	鼠标轨迹（链表）
4.	objects池（链表）
5.	events池（链表）
游戏流程图：
 
系统层次结构：
 
游戏物理坐标定义

宏定义：
	#define FPS_HIGH
	#define FPS_NORMAL
	#define FPS_LOW
头文件：
main.h
int main(int argc,char *argv[]);
void init(void);
void exit(void);
void circulation(void);
void welcomeUI(void);
void gameUI(void);
graphics.h
logic.h
model.h
IO.h
sound.h
相关算法及系统调用

技术规范
	严格遵守缩进格式。
	变量命名
		•全局变量全部大写
