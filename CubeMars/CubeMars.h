/*
 * CubeMars.h
 *
 *  Created on: Jul 21, 2022
 *      Author: sidiyer27
 */

#ifndef CUBEMARS_H_
#define CUBEMARS_H_

#include "main.h"

typedef enum {
CAN_PACKET_SET_DUTY = 0, //Duty cycle mode
CAN_PACKET_SET_CURRENT = 1, //Current loop mode 
CAN_PACKET_SET_CURRENT_BRAKE = 2, // Current brake mode
CAN_PACKET_SET_RPM = 3, //Velocity mode
CAN_PACKET_SET_POS = 4, // Position mode
CAN_PACKET_SET_ORIGIN_HERE = 5, //Set origin mode
CAN_PACKET_SET_POS_SPD = 6, //Position velocity loop mode
} CAN_PACKET_ID;


typedef struct Axis
{
	uint8_t controller_id;
	float motor_pos;
	float motor_spd;
	float motor_cur;
	int8_t motor_temp;
	int8_t motor_error;
	CAN_HandleTypeDef *motor_can; //hcan1, hcan2
}Axis;


void motor_receive(Axis *AXIS);
void cuomm_can_transmit_eid(uint32_t id, uint8_t *data, uint8_t len, CAN_HandleTypeDef *hcan);
void buffer_append_int32(uint8_t* buffer, int32_t number, int32_t *index);
void buffer_append_int16(uint8_t* buffer, int16_t number, int16_t *index);
void comm_can_set_duty(Axis Axis, float duty);
void comm_can_set_current(Axis Axis, float current);
void comm_can_set_cb(Axis Axis, float current);
void comm_can_set_rpm(Axis Axis, float rpm);
void comm_can_set_pos(Axis Axis, float pos);
void comm_can_set_origin(Axis Axis, uint8_t set_origin_mode);
void comm_can_set_pos_spd(Axis Axis, float pos,int16_t spd, int16_t RPA);




#endif /* CUBEMARS_H_ */
