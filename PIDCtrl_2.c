/*******************************************************************
 * File: PIDCtrl_2.c
 *
 * Code generated for Simulink model 'PIDCtrl_2'.
 *
 * Model version                  : 1.208
 * Simulink Coder version         : 8.6 (R2014a) 27-Dec-2013
 * C/C++ source code generated on : Sat Jan 23 09:45:26 2016
 *
 * Target selection: idelink_ert.tlc
 * Embedded hardware selection: Texas Instruments->C2000
 * Code generation objectives:
 *    1. ROM efficiency
 *    2. RAM efficiency
 *    3. Execution efficiency
 *    4. Debugging
 *    5. Safety precaution
 * Validation result: Not run
*
 *    AUTHOR:
 *         LEO
 *
 *    HISTORY:
 *         PID_Ctrl_2.0-----roll/pitch/yaw三路通道双环PID控制
 *******************************************************************/
#include "PIDCtrl_2.h"
#include "PIDCtrl_2_private.h"

/* Block signals and states (auto storage) */
DW_PIDCtrl_2_T PIDCtrl_2_DW;

/* Model step function */
void PIDCtrl_2_step(void)
{
  /* local block i/o variables */
  real_T R_Derivative;           //roll控制临时变量
  real_T R_Integrator_DSTATE;
  real_T R_Integrator;
  real_T R_UD_DSTATE;
  real_T P_Derivative;          //pitch控制临时变量
  real_T P_Integrator_DSTATE;
  real_T P_Integrator;
  real_T P_UD_DSTATE;
  real_T Y_Derivative;          //yaw控制临时变量
  real_T Y_Integrator_DSTATE;
  real_T Y_Integrator;
  real_T Y_UD_DSTATE;
  int16_T i;
  uint16_T checksum1;           //原始数据帧校验标识位
  uint16_T checksum2;           //解算数据帧校验标识位

  /* ********************* 1-: '<Root>/IMU_R' ***************************** */
  {
	int i;
    int16_T read_cnt;             //读取字节数标志
    char recHead1[1];			  //帧头A5
    char recHead2[1];    	      //帧头5A
	char recbuff1[20];            //原始数据缓冲区
	char recbuff2[16];            //解算数据缓冲区
	char recbuff[4];              //帧头数据
	uint16_T chksum1;             //原始数据帧校验和
	uint16_T chksum2;             //解算数据帧校验和
	recHead1[0]=0;			      //初始化
	recHead2[0]=0;
    for (read_cnt = 0; read_cnt < 50; read_cnt++)  //循环50次读取帧头
	{
	    /*读取接收到的数据，用于判断是否为帧头A5,否则读取下一字节，继续判断*/
        scib_rcv(recHead1,1, SHORTLOOP);	     	   
        if (recHead1[0] == 0xA5) 
		{
      	   /*继续读取数据，并判断是否为帧头5A*/	 	  
      	   scib_rcv(recHead2,1, SHORTLOOP);
		   if (recHead2[0] == 0x5A)
		    {
			   scib_rcv(PIDCtrl_2_DW.IMU_head,2, SHORTLOOP);  //是5A则读取帧类型数据
			   if((PIDCtrl_2_DW.IMU_head[0] == 0x16) && (PIDCtrl_2_DW.IMU_head[1]==0xA2))
			    {
				   scib_rcv(recbuff1,20, LONGLOOP);   //读取原始数据到原始数据缓冲区
				   scib_rcv(recbuff,4, LONGLOOP);     //读取中间帧头数据到缓冲区
				   scib_rcv(recbuff2,16, LONGLOOP);   //读取解算数据到解算数据缓冲区
				   chksum1=0;                         //每次校验和之前清零
			       chksum2=0;
				   for(i=0;i<18;i++){
					   chksum1=chksum1+recbuff1[i];
				   }
				   chksum1=chksum1+0x16+0xA2;         //累加所有数据帧
				   if((chksum1%256)==recbuff1[18]&&recbuff1[19]==0xAA){   //%256取余即取低8位字节作为校验
				      checksum1=1;                    //并且校验最后一位是否是AA，校验通过置位原始数据帧标识位
					  for(i=0;i<20;i++){              //校验通过更新传感器原始数据
					  PIDCtrl_2_DW.IMU_1[i]=recbuff1[i];
					  }
				   }
				   else{
					   checksum1=0;}                  //校验未通过或者最后一位不是AA，清零原始数据帧标识位，并且不更新原始数据
				   for(i=0;i<14;i++){
					   chksum2=chksum2+recbuff2[i];
				   }
				   chksum2=chksum2+0x12+0xA1;
				   if((chksum2%256)==recbuff2[14]&&recbuff2[15]==0xAA){
				      checksum2=1;                    //校验通过置位解算数据帧标识位
					  for(i=0;i<16;i++){              //校验通过更新传感器解算数据
					  PIDCtrl_2_DW.IMU_2[i]=recbuff2[i];
					  }
					  break;                          //解算数据帧校验通过则跳出循环，否则放弃此两帧数据，
				   }                                  //重新读取，保证用到的解算数据是正确的
				   else{
					  checksum2=0;}                   //校验未通过清零解算数据帧标识位，并且不更新解算数据
				}
				if((PIDCtrl_2_DW.IMU_head[0] == 0x12) && (PIDCtrl_2_DW.IMU_head[1]==0xA1))
				{
				   scib_rcv(recbuff2,16, LONGLOOP);
				   scib_rcv(recbuff,4, LONGLOOP);
				   scib_rcv(recbuff1,20, LONGLOOP);
				   chksum1=0;                         //每次校验和之前清零
			       chksum2=0;
				   for(i=0;i<18;i++){
					   chksum1=chksum1+recbuff1[i];
				   }
				   chksum1=chksum1+0x16+0xA2;
				   if((chksum1%256)==recbuff1[18]&&recbuff1[19]==0xAA){
				      checksum1=1;
					  for(i=0;i<20;i++){
					  PIDCtrl_2_DW.IMU_1[i]=recbuff1[i];
					  }
				   }
				   else{
					   checksum1=0;}
				   for(i=0;i<14;i++){
					   chksum2=chksum2+recbuff2[i];
				   }
				   chksum2=chksum2+0x12+0xA1;
				   if((chksum2%256)==recbuff2[14]&&recbuff2[15]==0xAA){
				      checksum2=1;
					  for(i=0;i<16;i++){
					  PIDCtrl_2_DW.IMU_2[i]=recbuff2[i];
					  }
					  break;
				   }
				   else{
					  checksum2=0;}
				}
		    }
        }
    }
  }
 
  /* ********************* 2-: '<Root>/IMU_T' ***************************** */
  {
	  if(checksum1==1||checksum2==1){       //有一帧通过校验则更新上位机传感器数据
	  scic_xmit(PIDCtrl_2_DW.IMU_1,20);
	  scic_xmit(PIDCtrl_2_DW.IMU_2,16);
	  }
  }
  
  /* ********************* 3-: '<Root>/IMU_data' ************************** */
  {
	char temp1[9];        //临时变量
	char temp2[7];        //临时变量
	if(checksum1==1){     //原始数据通过校验，则解算取出此数据
		/*传感器原始数据处理部分*/
		for (i=0;i<9;i++)
		{
			temp1[i]=(uint16_T)((PIDCtrl_2_DW.IMU_1[i * 2]<<8)|(PIDCtrl_2_DW.IMU_1[i * 2+1]));
			if(temp1[i] & 0x8000)
				temp1[i]=0-(temp1[i] & 0x7fff);
			else
				temp1[i]=(temp1[i] & 0x7fff);
		}
		 PIDCtrl_2_DW.rtb_ax=(real_T)temp1[0];
		 PIDCtrl_2_DW.rtb_ay=(real_T)temp1[1];
		 PIDCtrl_2_DW.rtb_az=(real_T)temp1[2];
		 PIDCtrl_2_DW.rtb_gx=(real_T)temp1[3];
		 PIDCtrl_2_DW.rtb_gy=(real_T)temp1[4];
		 PIDCtrl_2_DW.rtb_gz=(real_T)temp1[5];
		 PIDCtrl_2_DW.rtb_hx=(real_T)temp1[6];
		 PIDCtrl_2_DW.rtb_hy=(real_T)temp1[7];
		 PIDCtrl_2_DW.rtb_hz=(real_T)temp1[8];
	}
	if(checksum2==1){      //解算数据通过校验，则解算取出此数据
		/*传感器解算数据处理部分*/
		for (i=0;i<7;i++)
		{
			temp2[i]=(uint16_T)((PIDCtrl_2_DW.IMU_2[i * 2]<<8)|(PIDCtrl_2_DW.IMU_2[i * 2+1]));
			if(temp2[i] & 0x8000)
				temp2[i]=0-(temp2[i] & 0x7fff);
			else
				temp2[i]=(temp2[i] & 0x7fff);
		}
		PIDCtrl_2_DW.rtb_yaw      =(real_T)temp2[0]/ 10.0f;
		PIDCtrl_2_DW.rtb_pitch    =(real_T)temp2[1]/ 10.0f * (-1);
		PIDCtrl_2_DW.rtb_roll     =(real_T)temp2[2]/ 10.0f;
		PIDCtrl_2_DW.rtb_alt      =(real_T)temp2[3]/ 10.0f;
		PIDCtrl_2_DW.rtb_tmprat   =(real_T)temp2[4]/ 10.0f;
		PIDCtrl_2_DW.rtb_press    =(real_T)temp2[5]* 10.0f;
		PIDCtrl_2_DW.rtb_imupersec=(real_T)temp2[6];
	}
  }
  
  /* ********************* 4-: '<Root>/GUI_R，GUI_data' ******************* */
  {
    int i;
    char recbuff[8];
	int errFlg = NOERROR;
    for (i = 0; i < 8; i++)
      recbuff[i] = 0;
    /* Receiving data */
    errFlg = scic_rcv(recbuff, 2, SHORTLOOP);
	if (errFlg != NOERROR){
	    goto RXERRB;
	}
	byteswap_L8cmp(PIDCtrl_2_DW.GUI_head, recbuff, 1, 2);
	if((PIDCtrl_2_DW.GUI_head[0]==0x06aa)||(PIDCtrl_2_DW.GUI_head[0]==0x07aa)||(PIDCtrl_2_DW.GUI_head[0]==0x08aa))
	{
		scic_rcv(recbuff, 8, LONGLOOP);
		byteswap_L8cmp(PIDCtrl_2_DW.GUI_Rpid, recbuff, 4, 2);
		if(PIDCtrl_2_DW.GUI_head[0]==0x06aa)
		{
			PIDCtrl_2_DW.roll_rate_p =(real_T)PIDCtrl_2_DW.GUI_Rpid[0]/1000;
			PIDCtrl_2_DW.roll_rate_i =(real_T)PIDCtrl_2_DW.GUI_Rpid[1]/1000;
			PIDCtrl_2_DW.roll_rate_d =(real_T)PIDCtrl_2_DW.GUI_Rpid[2]/1000;
			PIDCtrl_2_DW.roll_p	     =(real_T)PIDCtrl_2_DW.GUI_Rpid[3]/1000;
		}
		if(PIDCtrl_2_DW.GUI_head[0]==0x07aa)
		{
			PIDCtrl_2_DW.pitch_rate_p =(real_T)PIDCtrl_2_DW.GUI_Rpid[0]/1000;
			PIDCtrl_2_DW.pitch_rate_i =(real_T)PIDCtrl_2_DW.GUI_Rpid[1]/1000;
			PIDCtrl_2_DW.pitch_rate_d =(real_T)PIDCtrl_2_DW.GUI_Rpid[2]/1000;
			PIDCtrl_2_DW.pitch_p	  =(real_T)PIDCtrl_2_DW.GUI_Rpid[3]/1000;
		}
		if(PIDCtrl_2_DW.GUI_head[0]==0x08aa)
		{
			PIDCtrl_2_DW.yaw_rate_p =(real_T)PIDCtrl_2_DW.GUI_Rpid[0]/1000;
			PIDCtrl_2_DW.yaw_rate_i =(real_T)PIDCtrl_2_DW.GUI_Rpid[1]/1000;
			PIDCtrl_2_DW.yaw_rate_d =(real_T)PIDCtrl_2_DW.GUI_Rpid[2]/1000;
			PIDCtrl_2_DW.yaw_p 	    =(real_T)PIDCtrl_2_DW.GUI_Rpid[3]/1000;
		}
	}
	else if((PIDCtrl_2_DW.GUI_head[0]==0x01aa)||(PIDCtrl_2_DW.GUI_head[0]==0x02aa)||(PIDCtrl_2_DW.GUI_head[0]==0x03aa))
	{
		scic_rcv(recbuff, 2, LONGLOOP);
		byteswap_L8cmp(PIDCtrl_2_DW.GUI_Rpwm, recbuff, 1, 2);
		if(PIDCtrl_2_DW.GUI_head[0]==0x01aa)
		{
			PIDCtrl_2_DW.pwm_throttle =(real_T)((int16_T)PIDCtrl_2_DW.GUI_Rpwm[0]);
		}
		if(PIDCtrl_2_DW.GUI_head[0]==0x02aa)
		{
			PIDCtrl_2_DW.roll_design =(real_T)((int16_T)PIDCtrl_2_DW.GUI_Rpwm[0])/100;
		}
		if(PIDCtrl_2_DW.GUI_head[0]==0x03aa)
		{
			PIDCtrl_2_DW.pitch_design=(real_T)((int16_T)PIDCtrl_2_DW.GUI_Rpwm[0])/100;
		}
	}
	else if(PIDCtrl_2_DW.GUI_head[0]==0x04aa)
	{
		PIDCtrl_2_DW.stop=1;
		PIDCtrl_2_DW.start=0;
	}
	else if(PIDCtrl_2_DW.GUI_head[0]==0x05aa)
	{
		PIDCtrl_2_DW.stop=0;
		PIDCtrl_2_DW.start=1;
	}
	RXERRB:
	    asm(" NOP");
  }
  
  /* ********************* 5-: Update PID input parameters **************** */
  {
	{ /* %%%%%%%%%%%%%%%%%%%% rolL控制器参数更新  %%%%%%%%%%%%%%%%%%%%%% */
	 /* Sum: '<Root>/R_Sum' incorporates:*  Roll Error*/
	 PIDCtrl_2_DW.R_Err = PIDCtrl_2_DW.roll_design - PIDCtrl_2_DW.rtb_roll;
	 PIDCtrl_2_DW.R_rate_design=PIDCtrl_2_DW.R_Err*PIDCtrl_2_DW.roll_p;
	 if (PIDCtrl_2_DW.R_rate_design > 220.0) {
		PIDCtrl_2_DW.R_rate_design = 220.0;
	 } else {
	    if (PIDCtrl_2_DW.R_rate_design < -220.0) {
          PIDCtrl_2_DW.R_rate_design = -220.0;
          }
       }
	 PIDCtrl_2_DW.R_rate_err=PIDCtrl_2_DW.R_rate_design-PIDCtrl_2_DW.rtb_gx;
	 /* DiscreteIntegrator: '<S3>/R_Integrator' */
	 R_Integrator_DSTATE = PIDCtrl_2_DW.R_Integrator_DSTATE;
	 /* Update for UnitDelay: '<S6>/UD' */
	 R_UD_DSTATE = PIDCtrl_2_DW.R_UD_DSTATE;
	 /* SampleTimeMath: '<S6>/R_TSamp'：y = u * K where K = 1 / ( w * Ts ) */
     PIDCtrl_2_DW.R_TSamp = PIDCtrl_2_DW.R_rate_err * 100.0;
	 R_Derivative = PIDCtrl_2_DW.R_TSamp-R_UD_DSTATE;
	}
    { /* %%%%%%%%%%%%%%%%%%%% pitch控制器参数更新 %%%%%%%%%%%%%%%%%%%%%% */
	/* Sum: '<Root>/P_Sum' incorporates:*  Pitch Error*/
	 PIDCtrl_2_DW.P_Err = PIDCtrl_2_DW.pitch_design - PIDCtrl_2_DW.rtb_pitch;
	 PIDCtrl_2_DW.P_rate_design=PIDCtrl_2_DW.P_Err*PIDCtrl_2_DW.pitch_p;
	 if (PIDCtrl_2_DW.P_rate_design > 220.0) {
		PIDCtrl_2_DW.P_rate_design = 220.0;
	 } else {
	    if (PIDCtrl_2_DW.P_rate_design < -220.0) {
          PIDCtrl_2_DW.P_rate_design = -220.0;
          }
       }
	 PIDCtrl_2_DW.P_rate_err=PIDCtrl_2_DW.P_rate_design-PIDCtrl_2_DW.rtb_gy;
	 /* DiscreteIntegrator: '<S3>/R_Integrator' */
	 P_Integrator_DSTATE = PIDCtrl_2_DW.P_Integrator_DSTATE;
	 /* Update for UnitDelay: '<S5>/UD' */
	 P_UD_DSTATE = PIDCtrl_2_DW.P_UD_DSTATE;
	 /* SampleTimeMath: '<S5>/P_TSamp'：y = u * K where K = 1 / ( w * Ts ) */
     PIDCtrl_2_DW.P_TSamp = PIDCtrl_2_DW.P_rate_err * 100.0;
	 P_Derivative = PIDCtrl_2_DW.P_TSamp-P_UD_DSTATE;   
	}
	{ /* %%%%%%%%%%%%%%%%%%%% yaw控制器参数更新   %%%%%%%%%%%%%%%%%%%%%% */
	/* Sum: '<Root>/Y_Sum' incorporates:*  Yaw	Error*/
	 PIDCtrl_2_DW.Y_Err = PIDCtrl_2_DW.yaw_design - PIDCtrl_2_DW.rtb_yaw;
	 PIDCtrl_2_DW.Y_rate_design=PIDCtrl_2_DW.Y_Err*PIDCtrl_2_DW.yaw_p;
	 if (PIDCtrl_2_DW.Y_rate_design > 30.0) {
		PIDCtrl_2_DW.Y_rate_design = 30.0;
	 } else {
	    if (PIDCtrl_2_DW.Y_rate_design < -30.0) {
          PIDCtrl_2_DW.Y_rate_design = -30.0;
          }
       }
	 PIDCtrl_2_DW.Y_rate_err=PIDCtrl_2_DW.Y_rate_design-PIDCtrl_2_DW.rtb_gz;
	 /* DiscreteIntegrator: '<S3>/R_Integrator' */
	 Y_Integrator_DSTATE = PIDCtrl_2_DW.Y_Integrator_DSTATE;
	 /* Update for UnitDelay: '<S7>/UD' */
	 Y_UD_DSTATE = PIDCtrl_2_DW.Y_UD_DSTATE;
	 /* SampleTimeMath: '<S7>/Y_TSamp'：y = u * K where K = 1 / ( w * Ts ) */
     PIDCtrl_2_DW.Y_TSamp = PIDCtrl_2_DW.Y_rate_err * 100.0;
	 Y_Derivative = PIDCtrl_2_DW.Y_TSamp-Y_UD_DSTATE;
	}
  }

  /* ********************* 6-: PID controller  **************************** */
  {
	  /*增加积分作用条件：当误差在+-5°以外，比例和微分起主要作用，
	   *系统反应速度快，到达稳态的时间短。当误差在+-5°以内，才从
	   *零累加误差，加入积分作用，用以稳态下的微调，减小稳态误差*/

	  /* pid_roll_out */
	  if(PIDCtrl_2_DW.R_Err >= -5 && PIDCtrl_2_DW.R_Err <= 5 ){
      	R_Integrator=R_Integrator_DSTATE*PIDCtrl_2_DW.roll_rate_i;
	  	if (R_Integrator > 250.0) {
			R_Integrator = 250.0;
	  	} else {
	  	  if (R_Integrator < -250.0) {
        	  R_Integrator = -250.0;
          }
       	}
	  	PIDCtrl_2_DW.roll_out=PIDCtrl_2_DW.R_rate_err*PIDCtrl_2_DW.roll_rate_p+R_Integrator
			+R_Derivative * PIDCtrl_2_DW.roll_rate_d;
	  	R_Integrator_DSTATE += 0.01 * PIDCtrl_2_DW.R_rate_err; 
	  }
      else{
		PIDCtrl_2_DW.roll_out=PIDCtrl_2_DW.R_rate_err*PIDCtrl_2_DW.roll_rate_p
			+R_Derivative * PIDCtrl_2_DW.roll_rate_d;
	  }
	  if (PIDCtrl_2_DW.roll_out > 250.0) {
		PIDCtrl_2_DW.roll_out = 250.0;
	  } else {
	    if (PIDCtrl_2_DW.roll_out < -250.0) {
          PIDCtrl_2_DW.roll_out = -250.0;
        }
      }

	  /* pid_pitch_out */
	  if(PIDCtrl_2_DW.P_Err >= -5 && PIDCtrl_2_DW.P_Err <= 5 ){
     	 P_Integrator=P_Integrator_DSTATE*PIDCtrl_2_DW.pitch_rate_i;
	 	 if (P_Integrator > 250.0) {
			P_Integrator = 250.0;
	 	 } else {
	 	    if (P_Integrator < -250.0) {
      	 	   P_Integrator = -250.0;
		    }
		 }
	 	 PIDCtrl_2_DW.pitch_out=PIDCtrl_2_DW.P_rate_err*PIDCtrl_2_DW.pitch_rate_p+P_Integrator
			+P_Derivative * PIDCtrl_2_DW.pitch_rate_d;
		 P_Integrator_DSTATE += 0.01 * PIDCtrl_2_DW.P_rate_err; 
	  }
      else{
	 	 PIDCtrl_2_DW.pitch_out=PIDCtrl_2_DW.P_rate_err*PIDCtrl_2_DW.pitch_rate_p
			+P_Derivative * PIDCtrl_2_DW.pitch_rate_d;
	  }
	  if (PIDCtrl_2_DW.pitch_out > 250.0) {
		PIDCtrl_2_DW.pitch_out = 250.0;
	  } else {
	    if (PIDCtrl_2_DW.pitch_out < -250.0) {
          PIDCtrl_2_DW.pitch_out = -250.0;
          }
      }

	  /* pid_yaw_out */
	  if(PIDCtrl_2_DW.Y_Err >= -5 && PIDCtrl_2_DW.Y_Err <= 5 ){
   	   Y_Integrator=Y_Integrator_DSTATE*PIDCtrl_2_DW.yaw_rate_i;
	 	 if (Y_Integrator > 8.0) {
			Y_Integrator = 8.0;
		 } else {
		    if (Y_Integrator < -8.0) {
     	     Y_Integrator = -8.0;
     	    }
      	 }
	 	 PIDCtrl_2_DW.yaw_out=PIDCtrl_2_DW.Y_rate_err*PIDCtrl_2_DW.yaw_rate_p+Y_Integrator
			+Y_Derivative * PIDCtrl_2_DW.yaw_rate_d;
		 Y_Integrator_DSTATE += 0.01 * PIDCtrl_2_DW.Y_rate_err; 
	  }
      else{
	 	 PIDCtrl_2_DW.yaw_out=PIDCtrl_2_DW.Y_rate_err*PIDCtrl_2_DW.yaw_rate_p
			+Y_Derivative * PIDCtrl_2_DW.yaw_rate_d;
	  }
	  if (PIDCtrl_2_DW.yaw_out > 250.0) {
		PIDCtrl_2_DW.yaw_out = 250.0;
	  } else {
	    if (PIDCtrl_2_DW.yaw_out < -250.0) {
          PIDCtrl_2_DW.yaw_out = -250.0;
          }
      }

	  /* DiscreteIntegrator: '<S3>/R_Integrator' */
	  PIDCtrl_2_DW.R_Integrator_DSTATE = R_Integrator_DSTATE;
	  /* DiscreteIntegrator: '<S3>/P_Integrator' */
	  PIDCtrl_2_DW.P_Integrator_DSTATE = P_Integrator_DSTATE;
	  /* Update for DiscreteIntegrator: '<S3>/Y_Integrator' */
	  PIDCtrl_2_DW.Y_Integrator_DSTATE = Y_Integrator_DSTATE;
 	  /* Update for UnitDelay: '<S6>/UD' */
	  R_UD_DSTATE = PIDCtrl_2_DW.R_TSamp;
	  /* Update for UnitDelay: '<S7>/UD' */
	  Y_UD_DSTATE = PIDCtrl_2_DW.Y_TSamp;
	  /* Update for UnitDelay: '<S5>/UD' */
	  P_UD_DSTATE = PIDCtrl_2_DW.P_TSamp;
	  /* Update for UnitDelay: '<S6>/UD' */
	  PIDCtrl_2_DW.R_UD_DSTATE = R_UD_DSTATE;
	  /* Update for UnitDelay: '<S7>/UD' */
	  PIDCtrl_2_DW.P_UD_DSTATE = P_UD_DSTATE;
	  /* Update for UnitDelay: '<S5>/UD' */
	  PIDCtrl_2_DW.Y_UD_DSTATE = Y_UD_DSTATE;
  }

  /* ********************* 7-: '<Root>/PWM_data' ************************** */
  {
	/* 根据开机及GUI命令初始化PWM参数：开机置pwm1000，gui开始则输出模糊结果，紧急停机置为超量程停机  */
	if(PIDCtrl_2_DW.stop == 0 && PIDCtrl_2_DW.start == 0)
	{
	  PIDCtrl_2_DW.pwm_2=1000;       //dsp开机，输出低于启动信号的占空比，使电机进入等待触发工作信号阶段
	  PIDCtrl_2_DW.pwm_4=1000;
	  PIDCtrl_2_DW.pwm_1=1000;       
	  PIDCtrl_2_DW.pwm_3=1000;
	  PIDCtrl_2_DW.pwm_design  =1150;  //初始电机油门信号，大于1140，即启动
	  PIDCtrl_2_DW.roll_design =0;   //期望横滚角初始为零
	  PIDCtrl_2_DW.pitch_design=0;   //期望俯仰角初始为零
	  PIDCtrl_2_DW.yaw_design  =180; //期望偏航角初始为零
	  PIDCtrl_2_DW.roll_p  =4.5;    //未发送PID参数时初始内外环控制参数，默认为0.01,0.0,0.0
	  PIDCtrl_2_DW.roll_rate_p  =0.01;
	  PIDCtrl_2_DW.roll_rate_i  =0.0;
	  PIDCtrl_2_DW.roll_rate_d  =0.0;
	  PIDCtrl_2_DW.pitch_p =4.5;    //未发送PID参数时初始内外环控制参数，默认为0.01,0.0,0.0
	  PIDCtrl_2_DW.pitch_rate_p =0.01;
	  PIDCtrl_2_DW.pitch_rate_i =0.0;
	  PIDCtrl_2_DW.pitch_rate_d =0.0;
	  PIDCtrl_2_DW.yaw_p   =4.5;     //未发送PID参数时初始内外环控制参数，默认为0.0,0.0,0.0
	  PIDCtrl_2_DW.yaw_rate_p   =0.01;
	  PIDCtrl_2_DW.yaw_rate_i   =0.0;
	  PIDCtrl_2_DW.yaw_rate_d   =0.0;
	}
	if(PIDCtrl_2_DW.stop == 1 && PIDCtrl_2_DW.start == 0 )
	{
	  PIDCtrl_2_DW.pwm_2=2500;        //占空比大于22.75电机即停转
	  PIDCtrl_2_DW.pwm_4=2500;
	  PIDCtrl_2_DW.pwm_1=2500;        //占空比大于22.75电机即停转
	  PIDCtrl_2_DW.pwm_3=2500;
	}
	if(PIDCtrl_2_DW.stop == 0 && PIDCtrl_2_DW.start == 1 )   //可以输出PID的结果，更新至PWM
	{
	  //*************获得PID_PWM输出*******************************************//
	  /* '<S3>:1:2' pwm_2=pwm_throttle+R_out-Y_out; */
	  /* '<S3>:1:3' pwm_4=pwm_design-R_out-Y_out; */
	  /* '<S3>:1:4' pwm_1=pwm_design-P_out+Y_out; */
	  /* '<S3>:1:5' pwm_3=pwm_design+P_out+Y_out; */
	  PIDCtrl_2_DW.pwm_2 = PIDCtrl_2_DW.pwm_throttle + PIDCtrl_2_DW.roll_out;
	  PIDCtrl_2_DW.pwm_4 = PIDCtrl_2_DW.pwm_throttle - PIDCtrl_2_DW.roll_out;
	  PIDCtrl_2_DW.pwm_1 = PIDCtrl_2_DW.pwm_throttle - PIDCtrl_2_DW.pitch_out;
	  PIDCtrl_2_DW.pwm_3 = PIDCtrl_2_DW.pwm_throttle + PIDCtrl_2_DW.pitch_out;
	  /* Saturate: '<Root>/R_sat_wa' */
	  if (PIDCtrl_2_DW.pwm_2 > 2000) {
	    PIDCtrl_2_DW.pwm_2 = 2000;
	  } else if (PIDCtrl_2_DW.pwm_2 < 1000) {
	    PIDCtrl_2_DW.pwm_2 = 1000;
	  }
	  /* End of Saturate: '<Root>/R_sat_wa' */
	  /* Saturate: '<Root>/R_sat_wb' */
	  if (PIDCtrl_2_DW.pwm_4 > 2000) {
	    PIDCtrl_2_DW.pwm_4 = 2000;
	  } else if (PIDCtrl_2_DW.pwm_4 < 1000) {
	    PIDCtrl_2_DW.pwm_4 = 1000;
	  }
	  /* End of Saturate: '<Root>/R_sat_wb' */
	  /* Saturate: '<Root>/P_sat_wa' */
	  if (PIDCtrl_2_DW.pwm_1 > 2000) {
	    PIDCtrl_2_DW.pwm_1 = 2000;
	  } else if (PIDCtrl_2_DW.pwm_1 < 1000) {
	    PIDCtrl_2_DW.pwm_1 = 1000;
	  }
	  /* End of Saturate: '<Root>/P_sat_wa' */
	  /* Saturate: '<Root>/P_sat_wb' */
	  if (PIDCtrl_2_DW.pwm_3 > 2000) {
	    PIDCtrl_2_DW.pwm_3 = 2000;
	  } else if (PIDCtrl_2_DW.pwm_3 < 1000) {
	    PIDCtrl_2_DW.pwm_3 = 1000;
	  }
	  /* End of Saturate: '<Root>/P_sat_wb' */
	}
	/* S-Function (c280xpwm): '<Root>/ePWM1' */
	/*-- Update CMPA value for ePWM1 --*/
	EPwm1Regs.CMPA.half.CMPA = (uint16_T)((uint32_T)EPwm1Regs.TBPRD * PIDCtrl_2_DW.pwm_2 *0.005* 0.01);
	/*-- Update CMPB value for ePWM1 --*/
	EPwm1Regs.CMPB = (uint16_T)((uint32_T)EPwm1Regs.TBPRD * PIDCtrl_2_DW.pwm_4 *0.005* 0.01);
	/* S-Function (c280xpwm): '<Root>/ePWM3' */
	/*-- Update CMPA value for ePWM3 --*/
	EPwm3Regs.CMPA.half.CMPA = (uint16_T)((uint32_T)EPwm3Regs.TBPRD * PIDCtrl_2_DW.pwm_1 *0.005* 0.01);
	/*-- Update CMPB value for ePWM3 --*/
	EPwm3Regs.CMPB = (uint16_T)((uint32_T)EPwm3Regs.TBPRD * PIDCtrl_2_DW.pwm_3 *0.005* 0.01);

  }

}

/* Model initialize function */
void PIDCtrl_2_initialize(void)
{
  /* Registration code */

  /* states (dwork) */
  (void) memset((void *)&PIDCtrl_2_DW, 0,
                sizeof(DW_PIDCtrl_2_T));

  /* Start for S-Function (c280xpwm): '<Root>/ePWM1' */

  /*** Initialize ePWM1 modules ***/
  {
    /*-- Setup Time-Base (TB) Submodule --*/
    EPwm1Regs.TBPRD = 46875;

    /* // Time-Base Control Register
       EPwm1Regs.TBCTL.bit.CTRMODE    = 0;          // Counter Mode
       EPwm1Regs.TBCTL.bit.SYNCOSEL   = 3;          // Sync output select
       EPwm1Regs.TBCTL.bit.PRDLD      = 0;          // Shadow select
       EPwm1Regs.TBCTL.bit.PHSEN      = 0;          // Phase load enable
       EPwm1Regs.TBCTL.bit.PHSDIR     = 0;          // Phase Direction
       EPwm1Regs.TBCTL.bit.HSPCLKDIV  = 0;          // High speed time pre-scale
       EPwm1Regs.TBCTL.bit.CLKDIV     = 6;          // Timebase clock pre-scale
     */
    EPwm1Regs.TBCTL.all = (EPwm1Regs.TBCTL.all & ~0x3FBF) | 0x1830;

    /* // Time-Base Phase Register
       EPwm1Regs.TBPHS.half.TBPHS     = 0;          // Phase offset register
     */
    EPwm1Regs.TBPHS.all = (EPwm1Regs.TBPHS.all & ~0xFFFF0000) | 0x0;
    EPwm1Regs.TBCTR = 0x0000;          /* Clear counter*/

    /*-- Setup Counter_Compare (CC) Submodule --*/
    /* // Counter-Compare Control Register
       EPwm1Regs.CMPCTL.bit.SHDWAMODE = 0;  // Compare A block operating mode.
       EPwm1Regs.CMPCTL.bit.SHDWBMODE = 0;  // Compare B block operating mode.
       EPwm1Regs.CMPCTL.bit.LOADAMODE = 0;          // Active compare A
       EPwm1Regs.CMPCTL.bit.LOADBMODE = 0;          // Active compare A
     */
    EPwm1Regs.CMPCTL.all = (EPwm1Regs.CMPCTL.all & ~0x5F) | 0x0;
    EPwm1Regs.CMPA.half.CMPA = 4687;
    EPwm1Regs.CMPB = 4687;

    /*-- Setup Action-Qualifier (AQ) Submodule --*/
    EPwm1Regs.AQCTLA.all = 24;
    EPwm1Regs.AQCTLB.all = 264;

    /* // Action-Qualifier Software Force Register
       EPwm1Regs.AQSFRC.bit.RLDCSF    = 0;          // Reload from Shadow options
     */
    EPwm1Regs.AQSFRC.all = (EPwm1Regs.AQSFRC.all & ~0xC0) | 0x0;

    /* // Action-Qualifier Continuous S/W Force Register Set
       EPwm1Regs.AQCSFRC.bit.CSFA     = 0;          // Continuous Software Force on output A
       EPwm1Regs.AQCSFRC.bit.CSFB     = 0;          // Continuous Software Force on output B
     */
    EPwm1Regs.AQCSFRC.all = (EPwm1Regs.AQCSFRC.all & ~0xF) | 0x0;

    /*-- Setup Dead-Band Generator (DB) Submodule --*/
    /* // Dead-Band Generator Control Register
       EPwm1Regs.DBCTL.bit.OUT_MODE   = 0;          // Dead Band Output Mode Control
       EPwm1Regs.DBCTL.bit.IN_MODE    = 0;          // Dead Band Input Select Mode Control
       EPwm1Regs.DBCTL.bit.POLSEL     = 0;          // Polarity Select Control
     */
    EPwm1Regs.DBCTL.all = (EPwm1Regs.DBCTL.all & ~0x3F) | 0x0;
    EPwm1Regs.DBRED = 0;
    EPwm1Regs.DBFED = 0;

    /*-- Setup Event-Trigger (ET) Submodule --*/
    /* // Event-Trigger Selection and Event-Trigger Pre-Scale Register
       EPwm1Regs.ETSEL.bit.SOCAEN     = 0;          // Start of conversion A Enable
       EPwm1Regs.ETSEL.bit.SOCASEL    = 1;          // Start of conversion A Select
       EPwm1Regs.ETPS.bit.SOCAPRD     = 1;          // EPWM1SOCA Period Select
       EPwm1Regs.ETSEL.bit.SOCBEN     = 0;          // Start of conversion B Enable
       EPwm1Regs.ETSEL.bit.SOCBSEL    = 1;          // Start of conversion B Select
       EPwm1Regs.ETPS.bit.SOCBPRD     = 1;          // EPWM1SOCB Period Select
       EPwm1Regs.ETSEL.bit.INTEN      = 0;          // EPWM1INTn Enable
       EPwm1Regs.ETSEL.bit.INTSEL     = 1;          // EPWM1INTn Select
       EPwm1Regs.ETPS.bit.INTPRD      = 1;          // EPWM1INTn Period Select
     */
    EPwm1Regs.ETSEL.all = (EPwm1Regs.ETSEL.all & ~0xFF0F) | 0x1101;
    EPwm1Regs.ETPS.all = (EPwm1Regs.ETPS.all & ~0x3303) | 0x1101;

    /*-- Setup PWM-Chopper (PC) Submodule --*/
    /* // PWM-Chopper Control Register
       EPwm1Regs.PCCTL.bit.CHPEN      = 0;          // PWM chopping enable
       EPwm1Regs.PCCTL.bit.CHPFREQ    = 0;          // Chopping clock frequency
       EPwm1Regs.PCCTL.bit.OSHTWTH    = 0;          // One-shot pulse width
       EPwm1Regs.PCCTL.bit.CHPDUTY    = 0;          // Chopping clock Duty cycle
     */
    EPwm1Regs.PCCTL.all = (EPwm1Regs.PCCTL.all & ~0x7FF) | 0x0;

    /*-- Set up Trip-Zone (TZ) Submodule --*/
    EALLOW;
    EPwm1Regs.TZSEL.all = 0;

    /* // Trip-Zone Control Register
       EPwm1Regs.TZCTL.bit.TZA        = 3;          // TZ1 to TZ6 Trip Action On EPWM1A
       EPwm1Regs.TZCTL.bit.TZB        = 3;          // TZ1 to TZ6 Trip Action On EPWM1B
     */
    EPwm1Regs.TZCTL.all = (EPwm1Regs.TZCTL.all & ~0xF) | 0xF;

    /* // Trip-Zone Enable Interrupt Register
       EPwm1Regs.TZEINT.bit.OST       = 0;          // Trip Zones One Shot Int Enable
       EPwm1Regs.TZEINT.bit.CBC       = 0;          // Trip Zones Cycle By Cycle Int Enable
     */
    EPwm1Regs.TZEINT.all = (EPwm1Regs.TZEINT.all & ~0x6) | 0x0;
    EDIS;
  }

  /* Start for S-Function (c280xpwm): '<Root>/ePWM3' */

  /*** Initialize ePWM3 modules ***/
  {
    /*-- Setup Time-Base (TB) Submodule --*/
    EPwm3Regs.TBPRD = 46875;

    /* // Time-Base Control Register
       EPwm3Regs.TBCTL.bit.CTRMODE    = 0;          // Counter Mode
       EPwm3Regs.TBCTL.bit.SYNCOSEL   = 3;          // Sync output select
       EPwm3Regs.TBCTL.bit.PRDLD      = 0;          // Shadow select
       EPwm3Regs.TBCTL.bit.PHSEN      = 0;          // Phase load enable
       EPwm3Regs.TBCTL.bit.PHSDIR     = 0;          // Phase Direction
       EPwm3Regs.TBCTL.bit.HSPCLKDIV  = 0;          // High speed time pre-scale
       EPwm3Regs.TBCTL.bit.CLKDIV     = 6;          // Timebase clock pre-scale
     */
    EPwm3Regs.TBCTL.all = (EPwm3Regs.TBCTL.all & ~0x3FBF) | 0x1830;

    /* // Time-Base Phase Register
       EPwm3Regs.TBPHS.half.TBPHS     = 0;          // Phase offset register
     */
    EPwm3Regs.TBPHS.all = (EPwm3Regs.TBPHS.all & ~0xFFFF0000) | 0x0;
    EPwm3Regs.TBCTR = 0x0000;          /* Clear counter*/

    /*-- Setup Counter_Compare (CC) Submodule --*/
    /* // Counter-Compare Control Register
       EPwm3Regs.CMPCTL.bit.SHDWAMODE = 0;  // Compare A block operating mode.
       EPwm3Regs.CMPCTL.bit.SHDWBMODE = 0;  // Compare B block operating mode.
       EPwm3Regs.CMPCTL.bit.LOADAMODE = 0;          // Active compare A
       EPwm3Regs.CMPCTL.bit.LOADBMODE = 0;          // Active compare A
     */
    EPwm3Regs.CMPCTL.all = (EPwm3Regs.CMPCTL.all & ~0x5F) | 0x0;
    EPwm3Regs.CMPA.half.CMPA = 4687;
    EPwm3Regs.CMPB = 4687;

    /*-- Setup Action-Qualifier (AQ) Submodule --*/
    EPwm3Regs.AQCTLA.all = 24;
    EPwm3Regs.AQCTLB.all = 264;

    /* // Action-Qualifier Software Force Register
       EPwm3Regs.AQSFRC.bit.RLDCSF    = 0;          // Reload from Shadow options
     */
    EPwm3Regs.AQSFRC.all = (EPwm3Regs.AQSFRC.all & ~0xC0) | 0x0;

    /* // Action-Qualifier Continuous S/W Force Register Set
       EPwm3Regs.AQCSFRC.bit.CSFA     = 0;          // Continuous Software Force on output A
       EPwm3Regs.AQCSFRC.bit.CSFB     = 0;          // Continuous Software Force on output B
     */
    EPwm3Regs.AQCSFRC.all = (EPwm3Regs.AQCSFRC.all & ~0xF) | 0x0;

    /*-- Setup Dead-Band Generator (DB) Submodule --*/
    /* // Dead-Band Generator Control Register
       EPwm3Regs.DBCTL.bit.OUT_MODE   = 0;          // Dead Band Output Mode Control
       EPwm3Regs.DBCTL.bit.IN_MODE    = 0;          // Dead Band Input Select Mode Control
       EPwm3Regs.DBCTL.bit.POLSEL     = 0;          // Polarity Select Control
     */
    EPwm3Regs.DBCTL.all = (EPwm3Regs.DBCTL.all & ~0x3F) | 0x0;
    EPwm3Regs.DBRED = 0;
    EPwm3Regs.DBFED = 0;

    /*-- Setup Event-Trigger (ET) Submodule --*/
    /* // Event-Trigger Selection and Event-Trigger Pre-Scale Register
       EPwm3Regs.ETSEL.bit.SOCAEN     = 0;          // Start of conversion A Enable
       EPwm3Regs.ETSEL.bit.SOCASEL    = 1;          // Start of conversion A Select
       EPwm3Regs.ETPS.bit.SOCAPRD     = 1;          // EPWM3SOCA Period Select
       EPwm3Regs.ETSEL.bit.SOCBEN     = 0;          // Start of conversion B Enable
       EPwm3Regs.ETSEL.bit.SOCBSEL    = 1;          // Start of conversion B Select
       EPwm3Regs.ETPS.bit.SOCBPRD     = 1;          // EPWM3SOCB Period Select
       EPwm3Regs.ETSEL.bit.INTEN      = 0;          // EPWM3INTn Enable
       EPwm3Regs.ETSEL.bit.INTSEL     = 1;          // EPWM3INTn Select
       EPwm3Regs.ETPS.bit.INTPRD      = 1;          // EPWM3INTn Period Select
     */
    EPwm3Regs.ETSEL.all = (EPwm3Regs.ETSEL.all & ~0xFF0F) | 0x1101;
    EPwm3Regs.ETPS.all = (EPwm3Regs.ETPS.all & ~0x3303) | 0x1101;

    /*-- Setup PWM-Chopper (PC) Submodule --*/
    /* // PWM-Chopper Control Register
       EPwm3Regs.PCCTL.bit.CHPEN      = 0;          // PWM chopping enable
       EPwm3Regs.PCCTL.bit.CHPFREQ    = 0;          // Chopping clock frequency
       EPwm3Regs.PCCTL.bit.OSHTWTH    = 0;          // One-shot pulse width
       EPwm3Regs.PCCTL.bit.CHPDUTY    = 0;          // Chopping clock Duty cycle
     */
    EPwm3Regs.PCCTL.all = (EPwm3Regs.PCCTL.all & ~0x7FF) | 0x0;

    /*-- Set up Trip-Zone (TZ) Submodule --*/
    EALLOW;
    EPwm3Regs.TZSEL.all = 0;

    /* // Trip-Zone Control Register
       EPwm3Regs.TZCTL.bit.TZA        = 3;          // TZ1 to TZ6 Trip Action On EPWM3A
       EPwm3Regs.TZCTL.bit.TZB        = 3;          // TZ1 to TZ6 Trip Action On EPWM3B
     */
    EPwm3Regs.TZCTL.all = (EPwm3Regs.TZCTL.all & ~0xF) | 0xF;

    /* // Trip-Zone Enable Interrupt Register
       EPwm3Regs.TZEINT.bit.OST       = 0;          // Trip Zones One Shot Int Enable
       EPwm3Regs.TZEINT.bit.CBC       = 0;          // Trip Zones Cycle By Cycle Int Enable
     */
    EPwm3Regs.TZEINT.all = (EPwm3Regs.TZEINT.all & ~0x6) | 0x0;
    EDIS;
  }
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
