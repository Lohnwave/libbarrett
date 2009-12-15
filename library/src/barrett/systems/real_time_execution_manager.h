/*
 * real_time_execution_manager.h
 *
 *  Created on: Dec 10, 2009
 *      Author: dc
 */

#ifndef REAL_TIME_EXECUTION_MANAGER_H_
#define REAL_TIME_EXECUTION_MANAGER_H_


//#include <native/task.h>

#include "../detail/ca_macro.h"
#include "../threading/real_time_mutex.h"
#include "./abstract/execution_manager.h"


// forward declarations from Xenomai's <native/task.h>
struct rt_task_placeholder;
typedef struct rt_task_placeholder RT_TASK;


namespace barrett {
namespace systems {


namespace detail {
extern "C" {

void rtemEntryPoint(void* cookie);

}
}


class RealTimeExecutionManager : public ExecutionManager {
public:
	explicit RealTimeExecutionManager(unsigned long period_ns = 2000000);
	virtual ~RealTimeExecutionManager();

	void start();
	bool isRunning();
	void stop();

protected:
	RT_TASK* task;
	unsigned long period;
	bool running, stopRunning;

	threading::RealTimeMutex mutex;

private:
	friend void detail::rtemEntryPoint(void* cookie);

	DISALLOW_COPY_AND_ASSIGN(RealTimeExecutionManager);
};


}
}


#endif /* REAL_TIME_EXECUTION_MANAGER_H_ */