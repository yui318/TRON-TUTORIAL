/*
スイッチ制御タスクの生成情報と関連データ
スイッチ制御タスクの実行関数task_swのプロトタイプ宣言、スイッチ制御タスクのID番号を格納するための変数tskid_sw、スイッチ制御タスクの生成情報の変数ctsk_swを記述しています。
LED制御タスクの生成情報と関連データ
LED制御タスクの実行関数task_ledのプロトタイプ宣言、LED制御タスクのID番号を格納するための変数tskid_led
、LED制御タスクの生成情報の変数ctsk_ledを記述しています。
スイッチ制御タスクの実行関数
関数task_swは、スイッチ制御タスクの実行関数です。
関数の最後に自タスク終了API
tk_ext_tskを記述していますが、本関数はwhile文による無限ループから抜けませんので、このAPIが実行されることはありません。
LED制御タスクの実行関数
関数task_ledは、LED制御タスクの実行関数です。
関数の最後に自タスク終了API
tk_ext_tskを記述していますが、本関数はwhile文による無限ループから抜けませんので、このAPIが実行されることはありません。
usermain関数
usermain関数では、スイッチ制御タスクとLED制御タスクの生成および実行を行います。
最後にusermain関数は、タスクの起床待ちAPI
tk_slp_tskを実行し、起床待ち状態となります。これはusermain関数の終了によりμT-Kernelのシステム全体が終了することを防ぐためです。tk_slp_tskの呼び出しによって初期タスクが待ち状態になると、LED制御タスクとスイッチ制御タスクが実行されます。タスクAの生成情報と関連データ
タスクAの実行関数task_aのプロトタイプ宣言、タスクAのID番号を格納するための変数tskid_a、タスクAの生成情報の変数ctsk_aを記述しています。
タスクAの実行関数
関数task_aは、タスクAの実行関数です。本プログラムでは、デバッグ用出力関数tm_printfでメッセージ「Start
Task-A」を出力したのち、自タスク終了API
tk_ext_tskを呼び出してタスクAを終了します。 usermain関数
usermain関数は、タスク生成API
tk_cre_tskを呼び出してタスクAを生成します。続いてタスク起動API
tk_sta_tskを呼び出して生成したタスクAの実行を開始します。
最後にusermain関数は、タスク起床待ちAPI
tk_slp_tskを実行し、起床待ち状態となります。これは、usermain関数の終了によりμT-Kernelのシステム全体が終了することを防ぐためです。
なお、usermain関数を実行している初期タスクの優先度は最高優先度（優先度1）ですので、usermain関数の実行中は優先度10のタスクAは実行されません。
tk_slp_tskの呼び出しにより初期タスクが待ち状態になると、タスクAが実行されます。
*/

#include <tk/tkernel.h>
#include <tm/tmonitor.h>

LOCAL void task_sw(INT stacd, void *exinf);
LOCAL ID tskid_sw;

LOCAL T_CTSK ctsk_sw = {
    .itskpri = 10,
    .stksz = 1024,
    .task = task_sw,
    .tskatr = TA_HLNG | TA_RNG3, // High-level language task, User mode
};

LOCAL void task_led(INT stacd, void *exinf);
LOCAL ID tskid_led;

LOCAL T_CTSK ctsk_led = {
    .itskpri = 10,
    .stksz = 1024,
    .task = task_led,
    .tskatr = TA_HLNG | TA_RNG3, // High-level language task, User mode
};

LOCAL void task_sw(INT stacd, void *exinf) {
  UW sw_data, pre_sw_data;

  pre_sw_data = get_sw();
  while (1) {
    sw_data = get_sw();
    if (pre_sw_data != sw_data) {
      if (sw_data == 0) {
        tm_printf((UB *)"SW-ON\n");
      }
      pre_sw_data = sw_data;
    }
    tk_dly_tsk(100);
  }

  tk_ext_tsk(); // never reached
}

LOCAL void task_led(INT stacd, void *exinf) {
  UW data_reg;

  while (1) {
    led_ctl(1);
    tk_dly_tsk(500);
    led_ctl(0);
    tk_dly_tsk(500);
  }

  tk_ext_tsk(); // never reached
}

EXPORT INT usermain(void) {
  tskid_sw = tk_cre_tsk(&ctsk_sw);
  tk_sta_tsk(tskid_sw, 0);

  tskid_led = tk_cre_tsk(&ctsk_led);
  tk_sta_tsk(tskid_led, 0);

  tk_slp_tsk(TMO_FEVR); // Sleep forever
  return 0;             // Never reached
}