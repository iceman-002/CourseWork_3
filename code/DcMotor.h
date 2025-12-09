#ifndef DC_MOTOR_H
#define DC_MOTOR_H

void DcMotorInit(void);
void DcMotorStart(unsigned char motorId, unsigned char direction, unsigned char speed);
void DcMotorStop(unsigned char motorId);

#endif