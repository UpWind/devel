#include <QTimer>
#include <QTime>

/**
  * Delaytimer class is used to handle the paused time of the timers.
  */
class DelayTimer: public QTimer{
    Q_OBJECT
protected:
    //time in milliseconds after last pause call
    int pausedTime;
    //time of last pause call
    QTime time;
public:
    /**
      * Constructor of the DelayTimer.
      */
    DelayTimer(): pausedTime(0){}
    /**
      * Pauses the timer and marks the pause time.
      */
    void pause(){
        stop();
        pausedTime = 0;
        time.restart();
    }
    /**
      * Resumes the timer with the given delay and returns the paused time.
      * @param delay - Delay of the timer in milliseconds.
      * @return Returns the time in milliseconds since the pause was called.
      */
    int resume(int delay){
        start(delay);
        if(time.isValid())
            pausedTime = time.elapsed();
        return pausedTime;
    }
    /**
      * Getter method for getting the paused time.
      * @return Returns the time in milliseconds since the pause was last called.
      */
    int getPausedTime(){
        return time.elapsed();
    }
};
