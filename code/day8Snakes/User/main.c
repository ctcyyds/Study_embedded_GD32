#include "gd32f4xx.h"
#include "systick.h"
#include <stdio.h>
#include "main.h"
#include "usart.h"
#include "adc.h"
#include "Gui.h"
#include "pic.h"
#include "key.h"
#include "Lcd.h"
#include "link.h"
#include "dac.h"

//*************************[��Ϸ����]*************************
//����ɫ
#define DARK_GREEN (0x0320)
//����ɫ
#define LIGHT_GREEN (0x9772)

//ҡ��x��ʼֵ ��:4090 ��:0
int X_IN_VALUE = 1986;
//ҡ��y��ʼֵ ��:0 ��:3990
int Y_IN_VALUE = 2086;
// ��Ļ����
const int WINDOWWIDTH = 220;
// ��Ļ�߶�
const int WINDOWHEIGHT = 240;
// С����Ĵ�С
const int CELLSIZE = 20;
//���������ķ������
int CELLWIDTH = (int) (WINDOWWIDTH / CELLSIZE);
int CELLHEIGHT = (int) (WINDOWHEIGHT / CELLSIZE);

//̰���ߵķ���ö��
enum Direction {
    UP = 1, DOWN, LEFT, RIGHT
};

//ƻ��λ�ýṹ��
struct ApplePosition {
    int x;
    int y;
} apple_position;
//̰���ߵķ���
int direction = RIGHT;

//̰���ߵ�x��y����������
SListNode *snake_xs;
SListNode *snake_ys;
//̰����ɾ����β������������(�ߵ�ʱ����Ҫ�޸ĳɺ�ɫ)
int tail_x = -1;
int tail_y = -1;
//===================================================

//*************************[��������]*************************
//��Ϸ��ӭҳ��
void start_page(void);

//��Ϸ����ҳ��
void run_page(void);

//��Ϸ����ҳ��
void GameOver(void);

//��������
void draw_grid(void);

//ƻ���������ַ
void draw_apple_addr(void);

//�ߵ������ַ
void draw_snake_addr(void);

//����ƻ��
void draw_apple(void);

//������
void draw_worm(void);

//��Ϸ���ݳ�ʼ��
void GameReset(void);
//===================================================

int main(void) {
//*************************[��������]*************************
    //NVIC���ȼ�����,����һ��ȫ�־���  0~3   0~3
    nvic_priority_group_set(NVIC_PRIGROUP_PRE2_SUB2);
    systick_config();
    usart_run();

    //Rocker init
    adc_config();
    //Rocker scan
    adc_key_scan();

    //Key init
    key_led_run();

    //Lcd init
    Lcd_Init();
    //Music init
    dac_audio_init(8000);
    //Game homepage
    start_page();
    while (1) {
        //The game runs page
        run_page();
        //Game end page
        GameOver();
    }
}

//*************************[��Ϸ��ӭҳ��]*************************
void start_page(void) {
    //Picture display
    LCD_ShowPicture((LCD_W - 240) / 2, 0, 240, 280, gImage_snakes);
	start_play_audio(standby,sizeof(standby),0);
    while (1) {			
        printf("key:%d\n", key_B_pressed);
        if (key_B_pressed == 1) {
            return;
        }
    }
}
//===================================================

//*************************[��Ϸ����ҳ��]*************************
void run_page(void) {
    //��Ϸ���ݳ�ʼ��
    GameReset();
    //��ɫ����
    LCD_Fill(0, 0, LCD_W, LCD_H, COLOR_BLACK);
    //��������
    draw_grid();
    //��ƻ��һ�������ַ
    draw_apple_addr();
    //����һ�������ַ
    draw_snake_addr();
    //̰�����ƶ���ʱ�����ͷλ��
    int newx = 0;
    int newy = 0;
    //ѭ����Ⱦ
    while (1) {
        //ҡ��ɨ��
        adc_key_scan();
        //�ж���������
        if (abs(adcXValue - 1986) < 300 && adcYValue < 200 && direction != DOWN) {//��
            direction = UP;
        } else if (abs(adcXValue - 1986) < 300 && (3990 - adcYValue) < 200 && direction != UP) {//��
            direction = DOWN;
        } else if ((4090 - adcXValue) < 200 && abs(2086 - adcYValue) < 300 && direction != RIGHT) {//��
            direction = LEFT;
        } else if (adcXValue < 200 && abs(2086 - adcYValue) < 300 && direction != LEFT) {//��
            direction = RIGHT;
        }

        //���ݷ���,����һ���µ���ͷ,�����ַ�ʽ���ƶ�̰����
        //��Ҫ������µ���ͷx��y����
        if (direction == UP) {
            newx = snake_xs->data;
            newy = snake_ys->data - 1;
        } else if (direction == DOWN) {
            newx = snake_xs->data;
            newy = snake_ys->data + 1;
        } else if (direction == LEFT) {
            newx = snake_xs->data - 1;
            newy = snake_ys->data;
        } else if (direction == RIGHT) {
            newx = snake_xs->data + 1;
            newy = snake_ys->data;
        }
        //�����µ���ͷ��������ǰ��
        SListAddHead(&snake_xs, newx);
        SListAddHead(&snake_ys, newy);

        //���̰�����Ƿ�Ե�ƻ������û�Ե�����ɾ��β�ˣ�����ǰ��һ�� apple_position
        if ((snake_xs->data == apple_position.x) && (snake_ys->data == apple_position.y)) {
            printf("eat a apple\n");
            //�����һ��ƻ����λ��
            draw_apple_addr();
        } else {
            //ɾ��β��
            tail_x = SListDelTail(&snake_xs);
            tail_y = SListDelTail(&snake_ys);
        }

        //���̰�����Ƿ�ײ��ײ���߽磬�������ͷ������
        if (snake_xs->data < 0 || snake_xs->data > CELLWIDTH - 1 || snake_ys->data < 0 ||
                snake_ys->data > CELLHEIGHT - 1) {
            return;
        }

        /*�ж��Ƿ�ײ���Լ�*/
        SListNode *x_node = snake_xs;
        SListNode *y_node = snake_ys;
        int x_head = x_node->data;
        int y_head = y_node->data;
        //���̰�����Ƿ�ײ���Լ����������ͷ�������Ƿ��������������
        x_node = x_node->next;
        y_node = y_node->next;
        while (x_node != NULL) {
            int x = x_node->data;
            int y = y_node->data;
            if (x == x_head && y == y_head) {
                //ײ���Լ�
                return;
            }
            x_node = x_node->next;
            y_node = y_node->next;
        }

        //����ƻ��
        draw_apple();
        //������
        draw_worm();
        //˯��,����֡��
        delay_1ms(600);
    }
}
//===================================================

//*************************[��Ϸ����ҳ��]*************************
void GameOver(void) {
    //Character display
    LCD_ShowString(50, 100, "Game Over", COLOR_WHITE, COLOR_BLACK, 32, 0);  // ��ʾ32�ַ���
    LCD_ShowString(35, 220, "Press a KEY_B to play!", COLOR_RED, COLOR_BLACK, 16, 0);  // ��ʾ32�ַ���
    while (1) {
        printf("key:%d\n", key_B_pressed);
        if (key_B_pressed == 1) {
            return;
        }
    }
}
//===================================================

//*************************[�ߺ�ƻ���������ַ]*************************
void draw_apple_addr(void) {
    //��ƻ��һ�����λ��
    apple_position.x = rand() % CELLWIDTH;
    apple_position.y = rand() % CELLHEIGHT;
}

void draw_snake_addr(void) {
    // �����ʼ������һ������Ϊ̰���ߵ����
    int startx = rand() % (CELLWIDTH - 6) + 5;
    int starty = rand() % (CELLHEIGHT - 6) + 5;
    //�����������鱣��̰���ߵ�ͷ������x��y����
    snake_xs = SListCreateNode(startx);
    snake_ys = SListCreateNode(starty);
    //�������������ڵ��x��y����
    SListAddTail(&snake_xs, startx - 1);
    SListAddTail(&snake_ys, starty);
    SListAddTail(&snake_xs, startx - 2);
    SListAddTail(&snake_ys, starty);
}
//===================================================

//*************************[��������]*************************
void draw_grid(void) {
    int x = 0, y = 0;
    //����
    for (x = 10; x <= LCD_W - 10; x += CELLSIZE) {
        LCD_DrawLine(x, 20, x, LCD_H - 20, COLOR_WHITE);
    }

    //����
    for (y = 20; y <= LCD_H - 20; y += CELLSIZE) {
        LCD_DrawLine(10, y, LCD_W - 10, y, COLOR_WHITE);
    }
}
//===================================================

//*************************[����ƻ��]*************************
void draw_apple(void) {
    //���ƻ����Ӧ������
    int xs = apple_position.x * CELLSIZE + 11;
    int ys = apple_position.y * CELLSIZE + 21;
    LCD_Fill(xs, ys, xs + CELLSIZE - 2, ys + CELLSIZE - 2, COLOR_RED);
}
//===================================================

//*************************[������]*************************
//�����ߵ�һ����Ԫ
void draw_worm_ele(int x, int y) {
    int xs = x * CELLSIZE + 11;
    int ys = y * CELLSIZE + 21;

    LCD_Fill(xs, ys, xs + CELLSIZE - 2, ys + CELLSIZE - 2, DARK_GREEN);
    LCD_Fill(xs + 4, ys + 4, xs + CELLSIZE - 10, ys + CELLSIZE - 10, LIGHT_GREEN);
}

//�����ߵ�β��(��Ҫɾ��)
void draw_worm_tail() {
    int xs = tail_x * CELLSIZE + 10;
    int ys = tail_y * CELLSIZE + 20;

    LCD_Fill(xs + 2, ys + 2, xs + CELLSIZE - 2, ys + CELLSIZE - 2, COLOR_BLACK);
}

//������
void draw_worm(void) {
    SListNode *x_head = snake_xs;
    SListNode *y_head = snake_ys;
    while (x_head != NULL && y_head != NULL) {
        int x = x_head->data;
        int y = y_head->data;
        //����һ����Ԫ
        draw_worm_ele(x, y);
        //ָ����һ���ڵ�
        x_head = x_head->next;
        y_head = y_head->next;
    }
    //����β���Ŀ�Ԫ��
    draw_worm_tail();
}
//===================================================

//*************************[��Ϸ����]*************************
void GameReset(void) {
    snake_xs = NULL;
    snake_ys = NULL;
    tail_x = -1;
    tail_y = -1;
    direction = RIGHT;
}
//===================================================