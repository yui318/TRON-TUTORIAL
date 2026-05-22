/*
タスクAの生成情報と関連データ
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

LOCAL void task_a(INT stacd, void *exinf);
LOCAL ID tskid_a;

// typedef struct t_ctsk {
// 	void	*exinf;		/* Extended information */
// 	ATR	tskatr;		/* Task attribute */
// 	FP	task;		/* Task startup address */
// 	PRI	itskpri;	/* Priority at task startup */
// 	SZ	stksz;		/* User stack size (byte) */
// 	void	*bufptr;	/* User buffer */
// } T_CTSK;

LOCAL T_CTSK ctsk_a = {
    .itskpri = 10,
    .stksz = 1024,
    .task = task_a,
    .tskatr = TA_HLNG | TA_RNG3, // High-level language task, User mode
};

LOCAL void task_a(INT stacd, void *exinf) {
  tm_printf((UB *)"Start Task A\n");
  tk_ext_tsk();
}

EXPORT INT usermain(void) {
  tskid_a = tk_cre_tsk(&ctsk_a);
  tk_sta_tsk(tskid_a, 0);

  tk_slp_tsk(TMO_FEVR); // Sleep forever
  return 0;             // Never reached
}