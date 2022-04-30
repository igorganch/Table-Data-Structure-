#include "timer.h"
/*
  start();  starts the clock running from the current time
  currtime();  returns the current time
  stop();  stops the clock without reseting its value
  reset();  stops the clock and resets the timer to 0
*/

Timer::Timer() {
    elapsedTime_ = 0;
    isstop_ = true;

#if PLATFORM == WINDOWS
    starttime_ = 0;
    stoptime_ = 0;
#else

    stoptime_ = starttime_;
#endif


}
void Timer::reset() {
#if PLATFORM == WINDOWS
    starttime_ = 0;
    stoptime_ = 0;
#else

    stoptime_ = starttime_;
#endif
    elapsedTime_ = 0;
    isstop_ = true;
}
void Timer::start() {
#if PLATFORM == WINDOWS
    clock_t curr = clock();
    if (isstop_) {
        starttime_ = (stoptime_ == 0) ? curr : starttime_ + (curr - stoptime_);
        isstop_ = false;
    }
    else {
        starttime_ = curr;   //its like hitting reset and start
    }

    
#endif

}
void Timer::stop() {
#if PLATFORM == WINDOWS
    stoptime_ = clock();

#endif
    isstop_ = true;
}
double Timer::currtime() {
    double rc;
#if PLATFORM == WINDOWS
    clock_t curr = clock();
    clock_t elapse = (isstop_) ? stoptime_ - starttime_ : curr - starttime_;
    rc = double(elapse) / CLOCKS_PER_SEC;

#endif
    return rc;
}
double Timer::starttime() {
#if PLATFORM == WINDOWS
    return (double)starttime_;
#else
    return starttime_.tv_nsec / 1000000000.0 + starttime_.tv_sec;
#endif
}
