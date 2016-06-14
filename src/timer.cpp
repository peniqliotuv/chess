//timer.cpp
#include "timer.h"
#include "sys/time.h"
#include <ctime>

int getTime(){
  timeval timer;
  gettimeofday(&timer, NULL);
  return timer.tv_sec*1000 + timer.tv_usec/1000;
}
