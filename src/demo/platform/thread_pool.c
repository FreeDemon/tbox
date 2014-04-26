/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "../demo.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * globals
 */ 
static tb_atomic_t g_stop = 0;

/* //////////////////////////////////////////////////////////////////////////////////////
 * test
 */ 
static tb_void_t tb_demo_task_time_done(tb_cpointer_t priv)
{
	// trace
	tb_trace_i("done: %u ms", tb_p2u32(priv));
	
	// wait some time
	tb_size_t time = tb_p2u32(priv);
	tb_size_t count = (time / 500) + 1;
	while (!tb_atomic_get(&g_stop) && count--)
	{
		tb_msleep(500);
	}
}
static tb_void_t tb_demo_task_time_exit(tb_cpointer_t priv)
{
	// trace
	tb_trace_i("exit: %u ms", tb_p2u32(priv));
}

/* //////////////////////////////////////////////////////////////////////////////////////
 * main
 */ 
tb_int_t tb_demo_platform_thread_pool_main(tb_int_t argc, tb_char_t** argv)
{
#if 0
	// post task: 60s
	tb_thread_pool_task_post(tb_thread_pool(), "60000ms", tb_demo_task_time_done, tb_null, (tb_cpointer_t)60000, tb_false);

	// post task: 10s
	tb_thread_pool_task_post(tb_thread_pool(), "10000ms", tb_demo_task_time_done, tb_null, (tb_cpointer_t)10000, tb_false);

	// post task: 1s
	tb_thread_pool_task_post(tb_thread_pool(), "1000ms", tb_demo_task_time_done, tb_demo_task_time_exit, (tb_cpointer_t)1000, tb_false);

	// wait some time
	getchar();

#else

	// done
	tb_size_t count = tb_rand_uint32(1, 16);
	tb_size_t total = count;
	while (count-- && total < 1000)
	{
		// the time
		tb_size_t time = tb_rand_uint32(0, 1000);

		// trace
		tb_trace_i("post: %lu ms, total: %lu", time, total);
	
		// post task: time ms
		tb_thread_pool_task_post(tb_thread_pool(), tb_null, tb_demo_task_time_done, tb_demo_task_time_exit, (tb_pointer_t)time, !(time & 15)? tb_true : tb_false);

		// finished? wait some time and update count
		if (!count) 
		{
			// wait some time
			tb_msleep(500);

			// update count
			count = tb_rand_uint32(1, 16);
			total += count;
		}
	}
#endif

	// wait all
	tb_thread_pool_task_wait_all(tb_thread_pool(), -1);

	// trace
	tb_trace_i("end");
	return 0;
}
