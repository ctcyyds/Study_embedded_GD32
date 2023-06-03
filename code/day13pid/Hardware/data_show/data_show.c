#include "data_show.h"
#include "bluetooth.h"
#include <stdio.h>

//////////// PUSH ///////////////
#define TX_FRAME_HEAD	0x7a
#define TX_FRAME_END	0x7b
#define TX_CMD_CHN		0x01

//////////// PULL //////////////
#define RX_FRAME_HEAD	0x7b
#define RX_FRAME_END	0x7a
#define RX_CMD_PID		0x01

#define MAX_BUF		128
static unsigned char buf[MAX_BUF];
static unsigned char buf_idx = 0;
static unsigned char buf_len = 0;

//////////////////////////// ����ʵ������޸����´�����÷�ʽ////////////////
static void data_show_send(unsigned char dat) {
    blue_usart_send_data(dat);
}

void data_show_on_config_pid(unsigned char chn, unsigned char id, float kp, float ki, float kd){
	printf("%d %d %f %f %f \r\n",chn, id, kp, ki, kd);
}

////////////////////////////////////////////////////////////////////////////

//////////////////////////// ����ת������ͬƽ̨���ܴ��ڴ�С������ /////////
////////////////////// ��ǰ�Ǵ�ˣ��޸�˳�򼴿ɸ�ΪС�� ///////////////////
typedef union {
    float f;
    unsigned char b[4];
} FloatBytes;

static void floatToBytes(float value, unsigned char *bytes) {
    FloatBytes fb;
    fb.f = value;
    bytes[0] = fb.b[0];
    bytes[1] = fb.b[1];
    bytes[2] = fb.b[2];
    bytes[3] = fb.b[3];
}

static float bytesToFloat(unsigned char *bytes) {
    FloatBytes fb;
    fb.b[0] = bytes[0];
    fb.b[1] = bytes[1];
    fb.b[2] = bytes[2];
    fb.b[3] = bytes[3];
    return fb.f;
}
///////////////////////////////////////////////////////////////

void data_show_init() {

}

void data_show_push(float *chns, int chn_cnt) {
    //֡ͷ  |   ����  |   ���ݳ���   |   ����  |   У���� | ֡β
    //1�ֽ� |   1�ֽ� |   1�ֽ�      |  n�ֽ�  |  1�ֽ�   | 1�ֽ�
    //����: �������͵ı�ʶ
    //���ݳ���: ��ʾ���� ���� ���ֽڸ���
    //У����:  ���� + ���ݳ��� + ����, ȡ��λ
    unsigned char i;
    float chn;
    unsigned char tmp[4];
    unsigned short sum = 0;

    data_show_send(TX_FRAME_HEAD);
    data_show_send(TX_CMD_CHN);
    data_show_send(chn_cnt * 4);
    sum += TX_CMD_CHN;
    sum += chn_cnt * 4;
    for(i = 0; i < chn_cnt; i++) {
        floatToBytes(chns[i], tmp);

        data_show_send(tmp[0]);
        sum += tmp[0];
        data_show_send(tmp[1]);
        sum += tmp[1];
        data_show_send(tmp[2]);
        sum += tmp[2];
        data_show_send(tmp[3]);
        sum += tmp[3];
    }
    data_show_send((sum >> 8) & 0xFF);
    data_show_send(TX_FRAME_END);
}

static void parse_config_pid() {
	//[123, 1, 14, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 122]
		float kp, ki, kd;
		kp = bytesToFloat(&buf[(buf_idx + 5) % MAX_BUF]);
		ki = bytesToFloat(&buf[(buf_idx + 9) % MAX_BUF]);
		kd = bytesToFloat(&buf[(buf_idx + 13) % MAX_BUF]);
		data_show_on_config_pid(buf[(buf_idx + 3) % MAX_BUF], buf[(buf_idx + 4) % MAX_BUF], kp, ki, kd);
}

//////////////////// ���Ҫȥ������������Ľ����������޸�������� ////////////
static void on_cmd() {
		unsigned char cmd;
	
		cmd = buf[(buf_idx + 1) % MAX_BUF];
		if(cmd == RX_CMD_PID) {
				parse_config_pid();
		}
}
//////////////////////////////////////////////////////////////////////////////

void data_show_parse() {
    unsigned char dat_len, i;
    unsigned short sum = 0;
    //////////////// check cmd
    if(buf_len < 3) return;

    // ��Ϣ���
    // ֡ͷ  |   ����  |   ���ݳ���    |   ����  |   У���� | ֡β
    // 1�ֽ� |   1�ֽ� |   1�ֽ�       |  n�ֽ�  |  1�ֽ�   | 1�ֽ�
    // ����: �������͵ı�ʶ
    // ���ݳ���: ��ʾ���� ���� ���ֽڸ���
    // У����:  ���� + ���ݳ��� + ����, ȡ��λ

    // check frame head
    if(buf[buf_idx] != RX_FRAME_HEAD) {
        // drop one
        buf_idx++;
        buf_idx %= MAX_BUF;
				if(buf_len != 0) buf_len--;

        data_show_parse();
        return;
    }
		
    // check dat len
    dat_len = buf[(buf_idx + 2) % MAX_BUF];
    // check cmd total len
    if(buf_len < dat_len + 5) return;

    // check sum
    sum += buf[(buf_idx + 1) % MAX_BUF];
    sum += buf[(buf_idx + 2) % MAX_BUF];
    for(i = 0; i < dat_len; i++) {
			sum += buf[(buf_idx + 3 + i) % MAX_BUF];
    }
	
		if(((sum >> 8) & 0xFF) != buf[(buf_idx + dat_len + 3) % MAX_BUF]) {
				// drop
				buf_idx++;
        buf_idx %= MAX_BUF;
        if(buf_len != 0) buf_len--;

        data_show_parse();
        return;
		}
		// check frame end
		if(buf[(buf_idx + dat_len + 4) % MAX_BUF] != RX_FRAME_END) {
				// drop
				buf_idx++;
        buf_idx %= MAX_BUF;
        if(buf_len != 0) buf_len--;

        data_show_parse();
        return;
		}
		// parse success
		/**
		if(buf[(buf_idx + 1) % MAX_BUF] == RX_CMD_PID) {
				// config pid
				parse_config_pid();
		}
		**/
		on_cmd();
		
		// drop
		buf_idx += dat_len + 5;
    buf_idx %= MAX_BUF;
		if(buf_len >= dat_len + 5) {
				buf_len -= dat_len + 5;
		} else {
				buf_len = 0;
		}
		
		data_show_parse();
}

void data_show_pull(unsigned char dat) {
		unsigned char idx = buf_idx + buf_len;
		idx %= MAX_BUF;
    // config dat
    buf[idx] = dat;
    buf_len ++;
		
		if(buf_len > MAX_BUF) {
			buf_idx = 0;
			buf_len = 0;
		}
}


void data_show_pull_array(uint8_t* datas, uint16_t length){
	for(uint8_t i=0; i < length; i++){
		data_show_pull(datas[i]);
	}
}

