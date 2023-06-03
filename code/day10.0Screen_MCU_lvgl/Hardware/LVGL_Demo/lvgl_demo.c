#include "lvgl_demo.h"

void first_screen(void) {
    //��������
    lv_obj_t * obj = lv_obj_create(lv_scr_act());

    //lv_obj_set_pos(obj, 0, 0);

    lv_obj_set_size(obj,300, 400);

    lv_obj_align(obj, LV_ALIGN_CENTER, 0, 0);

}

//*************************[��ʽ]*************************
void obj_style(void) {
    //��ʽ����(static�ı��������������)
    static lv_style_t style;
    //��ʼ����ʽ
    lv_style_init(&style);
    //���ÿ�Ⱥ͸߶�
    lv_style_set_width(&style,400);
    lv_style_set_height(&style,400);
    //����Բ�ǰ뾶
    lv_style_set_radius(&style, 20);
    //����x,y����
    lv_style_set_x(&style,100);
    lv_style_set_y(&style,100);
    //���ÿؼ������ٶ���100���ڷŶ���
    lv_style_set_pad_top(&style, 100);

    //�ؼ�
    lv_obj_t * obj = lv_obj_create(lv_scr_act());
    //�����ʽ
    lv_obj_add_style(obj, &style, 0);
    lv_obj_t * obj1 = lv_obj_create(obj);
    lv_obj_set_size(obj1,100,100);
}
//===================================================

//*************************[�ı�]*************************
void obj_label(void) {
    //�����ı�����
    lv_obj_t * label = lv_label_create(lv_scr_act());
    //�����ı�����
    lv_label_set_text(label, "hello");
    //����λ��
    lv_obj_set_pos(label, 100,100);
    //���ÿ��&�߶�
    lv_obj_set_width(label, 150);
    lv_obj_set_height(label, 150);
}
//===================================================

//*************************[��ť]*************************
//����¼��Ļص�����
void event_callback(lv_event_t * e) {
    //�¼��ı���LV_EVENT_CLICKED | LV_EVENT_VALUE_CHANGED
    lv_event_code_t code = lv_event_get_code(e);
    //���ݵ�����
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
    //������ť����
    lv_obj_t * btn = lv_btn_create(lv_scr_act());
    //�����ı����󲢷��ڰ�ť��
    lv_obj_t * label = lv_label_create(btn);
    //���ð�ť����
    lv_label_set_text(label, "Btn1");
    //���ð�ť���ı���λ��
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
    //���ô�С
    lv_obj_set_size(btn,100, 100);
    //���ð�ťλ��
    lv_obj_align(btn, LV_ALIGN_CENTER, 0, 0);

    //���õڶ�����ť�����Ա�ѡ�У�
    lv_obj_t * btn1 = lv_btn_create(lv_scr_act());
    lv_obj_align(btn1, LV_ALIGN_BOTTOM_MID, 0, 0);
    //���ð�ť���Ա�ѡ��
    lv_obj_add_flag(btn1, LV_OBJ_FLAG_CHECKABLE);
    label = lv_label_create(btn1);
    lv_label_set_text(label, "Btn2");
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_size(btn1,100, 100);

    //��ť�ؼ��¼�
    lv_obj_add_event_cb(btn, event_callback, LV_EVENT_CLICKED, &a);
    //ѡ�а�ť�ؼ��¼�
    lv_obj_add_event_cb(btn1, event_callback, LV_EVENT_VALUE_CHANGED, &b);
}
//===================================================

//*************************[��ť����]*************************
//����¼��Ļص�����
void btn_matrix_event(lv_event_t * e) {
    //�¼��ı���LV_EVENT_CLICKED | LV_EVENT_VALUE_CHANGED
    lv_event_code_t code = lv_event_get_code(e);
    //���ݵ�����
    int *d = (int *)lv_event_get_user_data(e);
    //��ť�¼�
    lv_obj_t * obj = lv_event_get_target(e);

    if(code == LV_EVENT_VALUE_CHANGED) {
        //��ȡ��ť�����е���İ�ť��ID
        uint32_t id = lv_btnmatrix_get_selected_btn(obj);
        //����ID���Ի�ȡ��ť����
        const char * txt = lv_btnmatrix_get_btn_text(obj, id);
        printf("%s\n",txt);
    }
}

//��������ť����
static const char * btnm_map[] = {"1", "2", "3", "4", "5", "\n",
                                  "6", "7", "8", "9", "0", "\n",
                                  "Action1", "Action2"
                                 };

void obj_btn_matrix(void) {
    //������ť����ؼ�����
    lv_obj_t * btn_matrix = lv_btnmatrix_create(lv_scr_act());
    //������ť������ÿ����ť���ı�����
    lv_btnmatrix_set_map(btn_matrix, btnm_map);
    //���õ�ʮ����ť�Ŀ�͸�
    lv_btnmatrix_set_btn_width(btn_matrix, 10, 2);
    //����ĳһ����ť������
    lv_btnmatrix_set_btn_ctrl(btn_matrix, 10, LV_BTNMATRIX_CTRL_CHECKABLE);
    lv_btnmatrix_set_btn_ctrl(btn_matrix, 11, LV_BTNMATRIX_CTRL_CHECKED);
    //���ð�ť�����ڴ����е�λ��
    lv_obj_align(btn_matrix, LV_ALIGN_CENTER, 0, 0);

    //��ť�ؼ��¼�
    lv_obj_add_event_cb(btn_matrix, btn_matrix_event, LV_EVENT_VALUE_CHANGED, NULL);
}
//===================================================

//*************************[�ı������]*************************
//����¼��Ļص�����
void text_btn(lv_event_t * e) {
    //��ť
    lv_obj_t * obj = lv_event_get_target(e);
    //�ı������
    lv_obj_t * ta = lv_event_get_user_data(e);
    //��ť����
    const char * txt = lv_btnmatrix_get_btn_text(obj, lv_btnmatrix_get_selected_btn(obj));
    //ɾ����ť
    if(strcmp(txt, LV_SYMBOL_BACKSPACE) == 0) lv_textarea_del_char(ta);
    //�س���ť
    else if(strcmp(txt, LV_SYMBOL_NEW_LINE) == 0) lv_event_send(ta, LV_EVENT_READY, NULL);
    //������ͨ�ַ�
    else lv_textarea_add_text(ta, txt);
}

//�ı������ؼ��Ļص�����
void textarea_event_handler(lv_event_t * e) {
    printf("sned data\n");
}

//��������ť�������ı����������������
static const char * ta_btn[] = {"1", "2", "3", "\n",
                                "4", "5", "6", "\n",
                                "7", "8", "9", "\n",
                                LV_SYMBOL_BACKSPACE, "0", LV_SYMBOL_NEW_LINE, ""
                               };

void obj_text_area(void) {
    //�����ı���������
    lv_obj_t * ta = lv_textarea_create(lv_scr_act());
    //�����ı�������λ��
    lv_obj_align(ta, LV_ALIGN_TOP_MID, 0, 10);
    //�����ı�������¼�
    lv_obj_add_event_cb(ta, textarea_event_handler, LV_EVENT_READY, ta);

    //��������ť����
    lv_obj_t * btnm = lv_btnmatrix_create(lv_scr_act());
    //���þ���ť��С
    lv_obj_set_size(btnm, 400, 300);
    //���þ���ťλ��
    lv_obj_align(btnm, LV_ALIGN_BOTTOM_MID, 0, -10);
    //���ð�ť�����¼�
    lv_obj_add_event_cb(btnm, text_btn, LV_EVENT_VALUE_CHANGED, ta);
    //���þ���ť���Ա�ѡ��
    lv_obj_clear_flag(btnm, LV_OBJ_FLAG_CLICK_FOCUSABLE);
    //������ť������ÿ����ť���ı�����
    lv_btnmatrix_set_map(btnm, ta_btn);
}
//===================================================

//*************************[���뷨]*************************
		//����ָ��
    lv_obj_t * ta = NULL;
		lv_obj_t * tb = NULL;
//�ص�����
void text_kb(lv_event_t * e) {
    //�¼���
    lv_event_code_t code = lv_event_get_code(e);
    //��������¼��Ŀؼ�
    lv_obj_t * ta = lv_event_get_target(e);
    //���̿ؼ�
    lv_obj_t * kb = lv_event_get_user_data(e);
    if(code == LV_EVENT_FOCUSED) {//���񵽹��
        printf("focused\n");
				//Ϊ���̰������
        lv_keyboard_set_textarea(kb, ta);
				//ȡ������
        lv_obj_clear_flag(kb, LV_OBJ_FLAG_HIDDEN);
    } else if(code == LV_EVENT_DEFOCUSED) { //��ʧ���
        printf("defocused\n");
        lv_keyboard_set_textarea(kb, NULL);
        //���ؼ���
				lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
    }
}

//��ť�¼��ص�����
void btn_kb(lv_event_t * e) {
	//�¼���
    lv_event_code_t code = lv_event_get_code(e);
    //��������¼��Ŀؼ�
    lv_obj_t * target = lv_event_get_target(e);
    //���̿ؼ�
    lv_obj_t * kb = lv_event_get_user_data(e);
	if(code == LV_EVENT_CLICKED){
		printf("Linke start\n");
		//��ȡ���������
		const char * str1 = lv_textarea_get_text(ta);
		const char * str2 = lv_textarea_get_text(tb);
		printf("Text1=%s\n",str1);
		printf("Text2=%s\n",str2);
	}
}

void obj_keyboard(void) {
    //�������뷨
    lv_obj_t * kb = lv_keyboard_create(lv_scr_act());
		lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
    //�����1
    //1.���崰��
    ta = lv_textarea_create(lv_scr_act());
    //2.����λ��
    lv_obj_align(ta, LV_ALIGN_TOP_LEFT, 10, 10);
    //3.��С
    lv_obj_set_size(ta, 140, 80);
    //4.������ı�����
    lv_textarea_set_placeholder_text(ta, "Enter text");
    //�������뷨�¼�
    lv_obj_add_event_cb(ta, text_kb, LV_EVENT_ALL, kb);
    //�����2
    tb = lv_textarea_create(lv_scr_act());
    lv_obj_align(tb, LV_ALIGN_TOP_RIGHT, -10, 10);
    lv_obj_set_size(tb, 140, 80);
    lv_textarea_set_placeholder_text(tb, "Enter text");
    lv_obj_add_event_cb(tb, text_kb, LV_EVENT_ALL, kb);
		
		//������ť����
    lv_obj_t * btn = lv_btn_create(lv_scr_act());
    //�����ı����󲢷��ڰ�ť��
    lv_obj_t * label = lv_label_create(btn);
    //���ð�ť����
    lv_label_set_text(label, "Sender");
    //���ð�ť���ı���λ��
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
    //���ô�С
    lv_obj_set_size(btn,100, 50);
    //���ð�ťλ��
    lv_obj_align(btn, LV_ALIGN_TOP_MID, 0, 30);
		//���ð�ť�¼�
		lv_obj_add_event_cb(btn, btn_kb, LV_EVENT_CLICKED, NULL);
}
//===================================================