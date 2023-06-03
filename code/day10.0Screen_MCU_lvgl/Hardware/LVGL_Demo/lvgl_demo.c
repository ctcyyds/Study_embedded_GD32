#include "lvgl_demo.h"

void first_screen(void) {
    //创建桌面
    lv_obj_t * obj = lv_obj_create(lv_scr_act());

    //lv_obj_set_pos(obj, 0, 0);

    lv_obj_set_size(obj,300, 400);

    lv_obj_align(obj, LV_ALIGN_CENTER, 0, 0);

}

//*************************[样式]*************************
void obj_style(void) {
    //样式定义(static改变变量的生命周期)
    static lv_style_t style;
    //初始化样式
    lv_style_init(&style);
    //设置宽度和高度
    lv_style_set_width(&style,400);
    lv_style_set_height(&style,400);
    //设置圆角半径
    lv_style_set_radius(&style, 20);
    //设置x,y坐标
    lv_style_set_x(&style,100);
    lv_style_set_y(&style,100);
    //设置控件不能再顶部100以内放东西
    lv_style_set_pad_top(&style, 100);

    //控件
    lv_obj_t * obj = lv_obj_create(lv_scr_act());
    //添加样式
    lv_obj_add_style(obj, &style, 0);
    lv_obj_t * obj1 = lv_obj_create(obj);
    lv_obj_set_size(obj1,100,100);
}
//===================================================

//*************************[文本]*************************
void obj_label(void) {
    //创建文本对象
    lv_obj_t * label = lv_label_create(lv_scr_act());
    //设置文本内容
    lv_label_set_text(label, "hello");
    //设置位置
    lv_obj_set_pos(label, 100,100);
    //设置宽度&高度
    lv_obj_set_width(label, 150);
    lv_obj_set_height(label, 150);
}
//===================================================

//*************************[按钮]*************************
//点击事件的回调函数
void event_callback(lv_event_t * e) {
    //事件的编码LV_EVENT_CLICKED | LV_EVENT_VALUE_CHANGED
    lv_event_code_t code = lv_event_get_code(e);
    //传递的数据
    int *d = (int *)lv_event_get_user_data(e);

    if(code == LV_EVENT_CLICKED) {
        printf("btn1 %d\n",*d);
    } else if(code == LV_EVENT_VALUE_CHANGED) {
        printf("btn2 %d\n",*d);
    }
}

void obj_btn(void) {
    static int a = 10;
    static int b = 20;
    //创建按钮对象
    lv_obj_t * btn = lv_btn_create(lv_scr_act());
    //创建文本对象并放在按钮中
    lv_obj_t * label = lv_label_create(btn);
    //设置按钮文字
    lv_label_set_text(label, "Btn1");
    //设置按钮中文本的位置
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
    //设置大小
    lv_obj_set_size(btn,100, 100);
    //设置按钮位置
    lv_obj_align(btn, LV_ALIGN_CENTER, 0, 0);

    //设置第二个按钮（可以被选中）
    lv_obj_t * btn1 = lv_btn_create(lv_scr_act());
    lv_obj_align(btn1, LV_ALIGN_BOTTOM_MID, 0, 0);
    //设置按钮可以被选中
    lv_obj_add_flag(btn1, LV_OBJ_FLAG_CHECKABLE);
    label = lv_label_create(btn1);
    lv_label_set_text(label, "Btn2");
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_size(btn1,100, 100);

    //按钮控件事件
    lv_obj_add_event_cb(btn, event_callback, LV_EVENT_CLICKED, &a);
    //选中按钮控件事件
    lv_obj_add_event_cb(btn1, event_callback, LV_EVENT_VALUE_CHANGED, &b);
}
//===================================================

//*************************[按钮矩阵]*************************
//点击事件的回调函数
void btn_matrix_event(lv_event_t * e) {
    //事件的编码LV_EVENT_CLICKED | LV_EVENT_VALUE_CHANGED
    lv_event_code_t code = lv_event_get_code(e);
    //传递的数据
    int *d = (int *)lv_event_get_user_data(e);
    //按钮事件
    lv_obj_t * obj = lv_event_get_target(e);

    if(code == LV_EVENT_VALUE_CHANGED) {
        //获取按钮矩阵中点击的按钮的ID
        uint32_t id = lv_btnmatrix_get_selected_btn(obj);
        //根据ID可以获取按钮文字
        const char * txt = lv_btnmatrix_get_btn_text(obj, id);
        printf("%s\n",txt);
    }
}

//创建矩阵按钮数组
static const char * btnm_map[] = {"1", "2", "3", "4", "5", "\n",
                                  "6", "7", "8", "9", "0", "\n",
                                  "Action1", "Action2"
                                 };

void obj_btn_matrix(void) {
    //创建按钮矩阵控件对象
    lv_obj_t * btn_matrix = lv_btnmatrix_create(lv_scr_act());
    //创建按钮矩阵中每个按钮的文本内容
    lv_btnmatrix_set_map(btn_matrix, btnm_map);
    //设置第十个按钮的宽和高
    lv_btnmatrix_set_btn_width(btn_matrix, 10, 2);
    //控制某一个按钮的属性
    lv_btnmatrix_set_btn_ctrl(btn_matrix, 10, LV_BTNMATRIX_CTRL_CHECKABLE);
    lv_btnmatrix_set_btn_ctrl(btn_matrix, 11, LV_BTNMATRIX_CTRL_CHECKED);
    //设置按钮矩阵在窗口中的位置
    lv_obj_align(btn_matrix, LV_ALIGN_CENTER, 0, 0);

    //按钮控件事件
    lv_obj_add_event_cb(btn_matrix, btn_matrix_event, LV_EVENT_VALUE_CHANGED, NULL);
}
//===================================================

//*************************[文本输入框]*************************
//点击事件的回调函数
void text_btn(lv_event_t * e) {
    //按钮
    lv_obj_t * obj = lv_event_get_target(e);
    //文本输入框
    lv_obj_t * ta = lv_event_get_user_data(e);
    //按钮文字
    const char * txt = lv_btnmatrix_get_btn_text(obj, lv_btnmatrix_get_selected_btn(obj));
    //删除按钮
    if(strcmp(txt, LV_SYMBOL_BACKSPACE) == 0) lv_textarea_del_char(ta);
    //回车按钮
    else if(strcmp(txt, LV_SYMBOL_NEW_LINE) == 0) lv_event_send(ta, LV_EVENT_READY, NULL);
    //输入普通字符
    else lv_textarea_add_text(ta, txt);
}

//文本输入框控件的回调函数
void textarea_event_handler(lv_event_t * e) {
    printf("sned data\n");
}

//创建矩阵按钮用于向文本输入框中输入数据
static const char * ta_btn[] = {"1", "2", "3", "\n",
                                "4", "5", "6", "\n",
                                "7", "8", "9", "\n",
                                LV_SYMBOL_BACKSPACE, "0", LV_SYMBOL_NEW_LINE, ""
                               };

void obj_text_area(void) {
    //创建文本输入框对象
    lv_obj_t * ta = lv_textarea_create(lv_scr_act());
    //设置文本输入框的位置
    lv_obj_align(ta, LV_ALIGN_TOP_MID, 0, 10);
    //设置文本输入框事件
    lv_obj_add_event_cb(ta, textarea_event_handler, LV_EVENT_READY, ta);

    //创建矩阵按钮对象
    lv_obj_t * btnm = lv_btnmatrix_create(lv_scr_act());
    //设置矩阵按钮大小
    lv_obj_set_size(btnm, 400, 300);
    //设置矩阵按钮位置
    lv_obj_align(btnm, LV_ALIGN_BOTTOM_MID, 0, -10);
    //设置按钮矩阵事件
    lv_obj_add_event_cb(btnm, text_btn, LV_EVENT_VALUE_CHANGED, ta);
    //设置矩阵按钮可以被选中
    lv_obj_clear_flag(btnm, LV_OBJ_FLAG_CLICK_FOCUSABLE);
    //创建按钮矩阵中每个按钮的文本内容
    lv_btnmatrix_set_map(btnm, ta_btn);
}
//===================================================

//*************************[输入法]*************************
		//定义指针
    lv_obj_t * ta = NULL;
		lv_obj_t * tb = NULL;
//回调函数
void text_kb(lv_event_t * e) {
    //事件码
    lv_event_code_t code = lv_event_get_code(e);
    //出现这个事件的控件
    lv_obj_t * ta = lv_event_get_target(e);
    //键盘控件
    lv_obj_t * kb = lv_event_get_user_data(e);
    if(code == LV_EVENT_FOCUSED) {//捕获到光标
        printf("focused\n");
				//为键盘绑定输入框
        lv_keyboard_set_textarea(kb, ta);
				//取消隐藏
        lv_obj_clear_flag(kb, LV_OBJ_FLAG_HIDDEN);
    } else if(code == LV_EVENT_DEFOCUSED) { //丢失光标
        printf("defocused\n");
        lv_keyboard_set_textarea(kb, NULL);
        //隐藏键盘
				lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
    }
}

//按钮事件回调函数
void btn_kb(lv_event_t * e) {
	//事件码
    lv_event_code_t code = lv_event_get_code(e);
    //出现这个事件的控件
    lv_obj_t * target = lv_event_get_target(e);
    //键盘控件
    lv_obj_t * kb = lv_event_get_user_data(e);
	if(code == LV_EVENT_CLICKED){
		printf("Linke start\n");
		//获取输入框数据
		const char * str1 = lv_textarea_get_text(ta);
		const char * str2 = lv_textarea_get_text(tb);
		printf("Text1=%s\n",str1);
		printf("Text2=%s\n",str2);
	}
}

void obj_keyboard(void) {
    //创建输入法
    lv_obj_t * kb = lv_keyboard_create(lv_scr_act());
		lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
    //输入框1
    //1.定义窗口
    ta = lv_textarea_create(lv_scr_act());
    //2.设置位置
    lv_obj_align(ta, LV_ALIGN_TOP_LEFT, 10, 10);
    //3.大小
    lv_obj_set_size(ta, 140, 80);
    //4.输入框文本设置
    lv_textarea_set_placeholder_text(ta, "Enter text");
    //设置输入法事件
    lv_obj_add_event_cb(ta, text_kb, LV_EVENT_ALL, kb);
    //输入框2
    tb = lv_textarea_create(lv_scr_act());
    lv_obj_align(tb, LV_ALIGN_TOP_RIGHT, -10, 10);
    lv_obj_set_size(tb, 140, 80);
    lv_textarea_set_placeholder_text(tb, "Enter text");
    lv_obj_add_event_cb(tb, text_kb, LV_EVENT_ALL, kb);
		
		//创建按钮对象
    lv_obj_t * btn = lv_btn_create(lv_scr_act());
    //创建文本对象并放在按钮中
    lv_obj_t * label = lv_label_create(btn);
    //设置按钮文字
    lv_label_set_text(label, "Sender");
    //设置按钮中文本的位置
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
    //设置大小
    lv_obj_set_size(btn,100, 50);
    //设置按钮位置
    lv_obj_align(btn, LV_ALIGN_TOP_MID, 0, 30);
		//设置按钮事件
		lv_obj_add_event_cb(btn, btn_kb, LV_EVENT_CLICKED, NULL);
}
//===================================================