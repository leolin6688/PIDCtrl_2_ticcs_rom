/*
 * File: PIDCtrl_2.h
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
 */

#ifndef RTW_HEADER_PIDCtrl_2_h_
#define RTW_HEADER_PIDCtrl_2_h_
#include <string.h>
#include <stddef.h>
#ifndef PIDCtrl_2_COMMON_INCLUDES_
# define PIDCtrl_2_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "DSP2833x_Device.h"
#include "DSP28xx_SciUtil.h"
#include "DSP2833x_Examples.h"
#include "IQmathLib.h"
#endif                                 /* PIDCtrl_2_COMMON_INCLUDES_ */

#include "PIDCtrl_2_types.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetStopRequested
# define rtmGetStopRequested(rtm)      ((void*) 0)
#endif

/* Block signals and states (auto storage) for system '<Root>' */
typedef struct {
  /* '<Root>/GUI_data' */
  unsigned int GUI_head[1];	           /*GUI帧类型的寄存器*/
  unsigned int GUI_Rpid[4];            /* '<Root>/GUI_R' pid 参数*/
  unsigned int GUI_Rpwm[1];            /* '<Root>/GUI_R' 油门参数*/
  real_T pwm_throttle;               
  real_T pitch_design;         
  real_T roll_design;
  real_T yaw_design; 
  real_T roll_p;
  real_T roll_rate_p;
  real_T roll_rate_i;
  real_T roll_rate_d;            
  real_T pitch_p;
  real_T pitch_rate_p;
  real_T pitch_rate_i;
  real_T pitch_rate_d;
  real_T yaw_p;
  real_T yaw_rate_p;
  real_T yaw_rate_i;                     
  real_T yaw_rate_d;
  real_T stop; 
  real_T start;
  /* '<Root>/PWM_data' */
  real_T roll_out;    
  real_T pitch_out; 
  real_T yaw_out;
  real_T pwm_design;
  real_T pwm_1;
  real_T pwm_2;
  real_T pwm_3;
  real_T pwm_4;
  /* '<Root>/IMU_data' */
  char IMU_head[2];      //IMU帧头寄存器
  char IMU_1[20];        //原始数据寄存器
  char IMU_2[16];        //解算数据寄存器
  real_T rtb_roll;       //定义三个姿态角寄存器
  real_T rtb_pitch;
  real_T rtb_yaw;
  real_T rtb_press;      //定义温度压力等值寄存器
  real_T rtb_tmprat;
  real_T rtb_alt;
  real_T rtb_imupersec;
  real_T rtb_ax;         //定义三个加速度值寄存器
  real_T rtb_ay;
  real_T rtb_az;
  real_T rtb_gx;         //定义三个陀螺仪寄存器
  real_T rtb_gy;
  real_T rtb_gz;
  real_T rtb_hx;         //定义三个磁力计寄存器
  real_T rtb_hy;
  real_T rtb_hz;
  /* '<Root>/R_Sum，P_Sum' */
  real_T R_Err;                        /* '<Root>/R_Sum' */
  real_T R_rate_err;
  real_T R_rate_design;
  real_T P_Err;                        /* '<Root>/P_Sum' */
  real_T P_rate_err;
  real_T P_rate_design;
  real_T Y_Err;
  real_T Y_rate_err;
  real_T Y_rate_design;
  real_T R_TSamp;
  real_T P_TSamp;
  real_T Y_TSamp;
  real_T R_Integrator_DSTATE;          /* '<S3>/R_Integrator' */
  real_T Y_Integrator_DSTATE;          /* '<S3>/Y_Integrator' */
  real_T P_Integrator_DSTATE;          /* '<S3>/P_Integrator' */
  real_T R_UD_DSTATE;                  /* '<S6>/UD' */
  real_T Y_UD_DSTATE;                  /* '<S7>/UD' */
  real_T P_UD_DSTATE;                  /* '<S5>/UD' */
} DW_PIDCtrl_2_T;

/* Block signals and states (auto storage) */
extern DW_PIDCtrl_2_T PIDCtrl_2_DW;

/* Model entry point functions */
extern void PIDCtrl_2_initialize(void);
extern void PIDCtrl_2_step(void);

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Use the MATLAB hilite_system command to trace the generated code back
 * to the model.  For example,
 *
 * hilite_system('<S3>')    - opens system 3
 * hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'PIDCtrl_2'
 * '<S1>'   : 'PIDCtrl_2/Att_Ctrl'
 * '<S2>'   : 'PIDCtrl_2/GUI_data'
 * '<S3>'   : 'PIDCtrl_2/IMU_data'
 * '<S4>'   : 'PIDCtrl_2/PWM_data'
 * '<S5>'   : 'PIDCtrl_2/Att_Ctrl/P_Derivative'
 * '<S6>'   : 'PIDCtrl_2/Att_Ctrl/R_Derivative'
 * '<S7>'   : 'PIDCtrl_2/Att_Ctrl/Y_Derivative'
 */

/*-
 * Requirements for '<Root>': PIDCtrl_2
 */
#endif                                 /* RTW_HEADER_PIDCtrl_2_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
