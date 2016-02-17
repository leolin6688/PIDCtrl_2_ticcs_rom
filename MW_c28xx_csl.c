void rt_OneStep(void);

#include "DSP2833x_Device.h"
#include "DSP2833x_Examples.h"
#include "DSP2833x_GlobalPrototypes.h"
#include "rtwtypes.h"
#include "PIDCtrl_2.h"
#include "PIDCtrl_2_private.h"

/*-----------------------------------------------------------------------------
 * void config_schedulerTimer(void)
 *
 * Abstract:
 *      This function configures scheduler timer
 */
void config_schedulerTimer(void)
{
  InitCpuTimers();

  /* Configure CPU-Timer 0 to interrupt every 0.01 sec. */
  /* Parameters:  Timer Pointer, CPU Freq in MHz, Period in usec. */
  ConfigCpuTimer(&CpuTimer0, 150.0, 0.01 * 1000000);
  StartCpuTimer0();
}

void disableWatchdog(void)
{
  int *WatchdogWDCR = (void *) 0x7029;
  asm(" EALLOW ");
  *WatchdogWDCR = 0x0068;
  asm(" EDIS ");
}

interrupt void TINT0_isr(void)
{
  volatile unsigned int PIEIER1_stack_save = PieCtrlRegs.PIEIER1.all;
  PieCtrlRegs.PIEIER1.all &= ~64;      /*disable group1 lower/equal priority interrupts*/
  asm(" RPT #5 || NOP");               /*wait 5 cycles        */
  IFR &= ~1;                           /*eventually disable lower/equal priority pending interrupts*/
  PieCtrlRegs.PIEACK.all = 1;          /*ACK to allow other interrupts from the same group to fire*/
  IER |= 1;
  EINT;                                /*global interrupt enable*/
  rt_OneStep();
  DINT;                                /* disable global interrupts during context switch, CPU will enable global interrupts after exiting ISR    */
  PieCtrlRegs.PIEIER1.all = PIEIER1_stack_save;/*restore PIEIER register that was modified*/
}

void enable_interrupts()
{
  EALLOW;
  PieVectTable.TINT0 = &TINT0_isr;     /* Hook interrupt to the ISR*/
  EDIS;
  PieCtrlRegs.PIEIER1.bit.INTx7 = 1;   /* Enable interrupt TINT0*/
  IER |= M_INT1;

  /* Enable global Interrupts and higher priority real-time debug events:*/
  EINT;                                /* Enable Global interrupt INTM*/
  ERTM;                                /* Enable Global realtime interrupt DBGM*/
}

void disable_interrupts()
{
  IER &= M_INT1;                       /* Disable Global INT1 (CPU Interrupt 1)*/
  DINT;                                /* Disable Global interrupt INTM*/
}

void init_SCI(void)
{                                      /* initialize SCI & FIFO registers */
  EALLOW;

  /*
   * Initialize SCI_B with following parameters:
   *    BaudRate              : 114329
   *    CharacterLengthBits   : 8
   *    EnableLoopBack        : 0
   *    NumberOfStopBits      : 1
   *    ParityMode            : None
   *    SuspensionMode        : Free_run
   *    CommMode              : Raw_data
   */
  ScibRegs.SCICCR.bit.STOPBITS = 0;    /*Number of stop bits. (0: One stop bit, 1: Two stop bits)*/
  ScibRegs.SCICCR.bit.PARITY = 0;      /*Parity mode (0: Odd parity, 1: Even parity)*/
  ScibRegs.SCICCR.bit.PARITYENA = 0;   /*Enable Pary Mode */
  ScibRegs.SCICCR.bit.LOOPBKENA = 0;   /*Loop Back enable*/
  ScibRegs.SCICCR.bit.ADDRIDLE_MODE = 0;/*ADDR/IDLE Mode control*/
  ScibRegs.SCICCR.bit.SCICHAR = 7;     /*Character length*/
  ScibRegs.SCICTL1.bit.RXERRINTENA = 0;/*Disable receive error interrupt*/
  ScibRegs.SCICTL1.bit.SWRESET = 1;    /*Software reset*/
  ScibRegs.SCICTL1.bit.TXENA = 0;      /* SCI transmitter enable*/
  ScibRegs.SCICTL1.bit.RXENA = 1;      /* SCI receiver enable*/
  ScibRegs.SCIHBAUD = 0U;
  ScibRegs.SCILBAUD = 40U;

  /*Free run, continue SCI operation regardless of suspend*/
  ScibRegs.SCIPRI.bit.FREE = 1;        /* Free emulation mode control*/
  ScibRegs.SCIPRI.bit.SOFT = 0;        /* Interrupt priority select*/
  ScibRegs.SCIFFCT.bit.ABDCLR = 0;
  ScibRegs.SCIFFCT.bit.CDC = 0;
  ScibRegs.SCIFFTX.bit.SCIRST = 1;     /* SCI reset rx/tx channels*/
  ScibRegs.SCIFFTX.bit.SCIFFENA = 1;   /* SCI FIFO enhancements are enabled.*/
  ScibRegs.SCIFFTX.bit.TXFIFOXRESET = 1;/* Re-enable transmit FIFO operation.*/
  ScibRegs.SCIFFRX.bit.RXFIFORESET = 1;/* Re-enable receive FIFO operation.*/

  /*
   * Initialize SCI_C with following parameters:
   *    BaudRate              : 114329
   *    CharacterLengthBits   : 8
   *    EnableLoopBack        : 0
   *    NumberOfStopBits      : 1
   *    ParityMode            : None
   *    SuspensionMode        : Free_run
   *    CommMode              : Raw_data
   */
  ScicRegs.SCICCR.bit.STOPBITS = 0;    /*Number of stop bits. (0: One stop bit, 1: Two stop bits)*/
  ScicRegs.SCICCR.bit.PARITY = 0;      /*Parity mode (0: Odd parity, 1: Even parity)*/
  ScicRegs.SCICCR.bit.PARITYENA = 0;   /*Enable Pary Mode */
  ScicRegs.SCICCR.bit.LOOPBKENA = 0;   /*Loop Back enable*/
  ScicRegs.SCICCR.bit.ADDRIDLE_MODE = 0;/*ADDR/IDLE Mode control*/
  ScicRegs.SCICCR.bit.SCICHAR = 7;     /*Character length*/
  ScicRegs.SCICTL1.bit.RXERRINTENA = 0;/*Disable receive error interrupt*/
  ScicRegs.SCICTL1.bit.SWRESET = 1;    /*Software reset*/
  ScicRegs.SCICTL1.bit.TXENA = 1;      /* SCI transmitter enable*/
  ScicRegs.SCICTL1.bit.RXENA = 1;      /* SCI receiver enable*/
  ScicRegs.SCIHBAUD = 0U;
  ScicRegs.SCILBAUD = 40U;

  /*Free run, continue SCI operation regardless of suspend*/
  ScicRegs.SCIPRI.bit.FREE = 1;        /* Free emulation mode control*/
  ScicRegs.SCIPRI.bit.SOFT = 0;        /* Interrupt priority select*/
  ScicRegs.SCIFFCT.bit.ABDCLR = 0;
  ScicRegs.SCIFFCT.bit.CDC = 0;
  ScicRegs.SCIFFTX.bit.SCIRST = 1;     /* SCI reset rx/tx channels*/
  ScicRegs.SCIFFTX.bit.SCIFFENA = 1;   /* SCI FIFO enhancements are enabled.*/
  ScicRegs.SCIFFTX.bit.TXFIFOXRESET = 1;/* Re-enable transmit FIFO operation.*/
  ScicRegs.SCIFFRX.bit.RXFIFORESET = 1;/* Re-enable receive FIFO operation.*/
  EDIS;
}

void init_SCI_GPIO(void)
{
  EALLOW;
  GpioCtrlRegs.GPAPUD.bit.GPIO19 = 0;  /*Enable pull-up for GPIO19*/
  GpioCtrlRegs.GPAMUX2.bit.GPIO19 = 2; /*Configure GPIO19 as SCIRXDB*/
  GpioCtrlRegs.GPBPUD.bit.GPIO62 = 0;  /*Enable pull-up for GPIO62*/
  GpioCtrlRegs.GPBMUX2.bit.GPIO62 = 1; /*Configure GPIO62 as SCIRXDA*/
  GpioCtrlRegs.GPBPUD.bit.GPIO63 = 0;  /*Enable pull-up for GPIO63*/
  GpioCtrlRegs.GPBMUX2.bit.GPIO63 = 1; /*Configure GPIO63 as SCITXDA*/
  EDIS;
}
