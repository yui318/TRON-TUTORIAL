#include <tk/tkernel.h>
#include <tm/tmonitor.h>

/*
TA_TFIFO	待ちタスクの並びは先着順（FIFO順）(※1)
TA_TPRI	待ちタスクの並びは優先度順(※1)
TA_WSGL	複数のタスクの待ちを許さない(※2)
TA_WMUL	複数のタスクの待ちを許す(※2)
（※1）TA_TFIFOとTA_TPRIのいずれか一方を必ず指定する必要があります
（※2）TA_WSGLとTA_WMULのいずれか一方を必ず指定する必要があります
*/

LOCAL ID flgid_a;
LOCAL T_CFLG cflg_a =
    {
        .flgatr = TA_TFIFO | TA_WMUL,
        .iflgptn = 0,
}

#define FLG_SW_ON (1 << 0)

LOCAL void task_sw(INT stacd, void *exinf) {
  while (1) {
    if (condition) {
      tk_set_flg(flgid_a, FLG_SW_ON);
    }

    tk_dly_tsk(100);
  }

  tk_ext_tsk(); // never reached
}

LOCAL void task_led(INT stacd, void *exinf) {
  UINT flgptn;
  while (1) {
    tk_wai_flg(flgid_a, FLG_SW_ON, (TWF_ANDW | TWF_BITCLR), &flgptn, TMO_FEVR);
    led_ctl(1);
    tk_dly_tsk(3000);
    led_ctl(0);
  }

  tk_ext_tsk(); // never reached
}

// EXPORT INT usermain(void);