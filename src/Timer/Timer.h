#ifndef TIMER_H
#define TIMER_H


class Timer
{
    public:
        Timer();
        ~Timer();

        void start();
        void stop();
        void paused();
        void unpause();

        int get_ticks();
        bool ispaused();
        bool isstarted();
   private:
       int start_tick;
       int paused_tick;

       bool is_paused;
       bool is_started;
};

#endif // TIMER_H
