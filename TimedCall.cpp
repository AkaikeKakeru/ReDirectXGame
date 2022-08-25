#include "TimedCall.h"

TimedCall::TimedCall(std::function<void(void)> f,uint32_t time)
{
	f_ = f;
	time_ = time;
};

void TimedCall::Update() {
	if (isFinished_ == true) {
		return;
	}
	time_--;
	if (time_ <= 0) {
		isFinished_ = true;
		//コールバック関数呼び出し
		(f_)();
	}
}