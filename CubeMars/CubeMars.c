#include "CubeMars.h"

CAN_TxHeaderTypeDef TxHeader;
CAN_RxHeaderTypeDef RxHeader;

uint8_t TxData[8];
uint8_t RxData[8];

uint32_t TxMailbox;

void motor_receive(Axis *Axis){

	int32_t ID = 0;
	HAL_CAN_GetRxMessage(Axis->motor_can, CAN_RX_FIFO0, &RxHeader, RxData);
	ID = RxHeader.ExtId;
	uint8_t controller_id = ID & 0xFF;
	if(controller_id == Axis->controller_id){
		int16_t pos_int = RxData[0] << 8 | RxData[1];
		int16_t spd_int = RxData[2] << 8 | RxData[3];
		int16_t cur_int = RxData[4] << 8 | RxData[5];
		Axis->motor_pos= (float)( pos_int * 0.1f); //motor position 
		Axis->motor_spd= (float)( spd_int * 10.0f);//motor speed
		Axis->motor_cur= (float) ( cur_int * 0.01f);//motor current
		Axis->motor_temp = RxData[6] ;//motor temperature
		Axis->motor_error = RxData[7] ;//motor error mode
	}
}




void comm_can_transmit_eid(uint32_t id, uint8_t *data, uint8_t len, CAN_HandleTypeDef *hcan) {

if (len > 8) {
len = 8;
}
TxHeader.IDE = CAN_ID_EXT;
TxHeader.ExtId = id;
TxHeader.RTR = CAN_RTR_DATA;
TxHeader.DLC = len;

HAL_CAN_AddTxMessage(hcan, &TxHeader, data, &TxMailbox);
}

void buffer_append_int32(uint8_t* buffer, int32_t number, int32_t *index) {
buffer[(*index)++] = number >> 24;
buffer[(*index)++] = number >> 16;
buffer[(*index)++] = number >> 8;
buffer[(*index)++] = number;
}

void buffer_append_int16(uint8_t* buffer, int16_t number, int16_t *index) {
buffer[(*index)++] = number >> 8;
buffer[(*index)++] = number;
}

void comm_can_set_duty(Axis Axis, float duty) {

int32_t send_index = 0;
uint8_t buffer[4];
buffer_append_int32(buffer, (int32_t)(duty * 100000.0), &send_index);
comm_can_transmit_eid(Axis.controller_id |((uint32_t)CAN_PACKET_SET_DUTY << 8), buffer, send_index, Axis.motor_can);
}

void comm_can_set_current(Axis Axis, float current) {
int32_t send_index = 0;
uint8_t buffer[4];
buffer_append_int32(buffer, (int32_t)(current * 1000.0), &send_index);
comm_can_transmit_eid(Axis.controller_id |((uint32_t)CAN_PACKET_SET_CURRENT << 8), buffer, send_index, Axis.motor_can);
}

void comm_can_set_cb(Axis Axis, float current) {
int32_t send_index = 0;
uint8_t buffer[4];
buffer_append_int32(buffer, (int32_t)(current * 1000.0), &send_index);
comm_can_transmit_eid(Axis.controller_id | ((uint32_t)CAN_PACKET_SET_CURRENT_BRAKE << 8), buffer, send_index, Axis.motor_can);
}

void comm_can_set_rpm(Axis Axis, float rpm) {
int32_t send_index = 0;
uint8_t buffer[4];
buffer_append_int32(buffer, (int32_t)rpm, &send_index);
comm_can_transmit_eid(Axis.controller_id |((uint32_t)CAN_PACKET_SET_RPM << 8), buffer, send_index, Axis.motor_can);
}

void comm_can_set_pos(Axis Axis, float pos) {
int32_t send_index = 0;
uint8_t buffer[4];
buffer_append_int32(buffer, (int32_t)(pos * 1000000.0), &send_index);
comm_can_transmit_eid(Axis.controller_id |((uint32_t)CAN_PACKET_SET_POS << 8), buffer, send_index, Axis.motor_can);
}

void comm_can_set_origin(Axis Axis, uint8_t set_origin_mode) {
  int32_t send_index = 1;
  uint8_t buffer[1];
  buffer[0] = set_origin_mode;
comm_can_transmit_eid(Axis.controller_id |((uint32_t) CAN_PACKET_SET_ORIGIN_HERE << 8), buffer, send_index, Axis.motor_can);
}

void comm_can_set_pos_spd(Axis Axis, float pos,int16_t spd, int16_t RPA){
	//spd scaled by 10. i.e input 200 to get 2000
	//acc.
int32_t send_index = 0;
int16_t send_index1 = 0;
uint8_t buffer[8];
buffer_append_int32(buffer, (int32_t)(pos * 10000.0), &send_index);
send_index1 = send_index;
buffer_append_int16(buffer,spd, & send_index1);
buffer_append_int16(buffer,RPA, & send_index1);
comm_can_transmit_eid(Axis.controller_id |((uint32_t)CAN_PACKET_SET_POS_SPD << 8), buffer, 8, Axis.motor_can);
}





