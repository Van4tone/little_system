#ifndef _CAN_FUN_H
#define _CAN_FUN_H
#include "common.h"


void CanIntRecvFun();

void PackCanSendMsg(u8 *buf,u8 len);

void CanSendMsgFun(u8 *buf);

void CanMsgPack2Send(u8 *buf,u8 len);

u8 CanRecvMsgCheck(u8 *buf, u8 len);

u8 CanCheckRecvMsg(u8 *buf,u8 len);

void CanReadMsgDeal();

u8 CanFunctionInit();












#endif