[INCLUDE[

#include "random.h"
#include "snake.h"
#include "screen.h"

]]
[CMAP[

@PROC(random_global_env);
@PROC(snake);
@PROC(screen);

timeEnd = 0#global#.time().add(30#local#);

LOCAL display = function()#global#
{
  this.screen.up().display();

  if(0#local#.time() > timeEnd) { this.delete(); }
  else { this.schedule(); }
};

LOCAL shift = function(job, snake, periodMs)#global#
{
  LOCAL timeCur = 0#local#.time();
  LOCAL timeCurMs = 0#local#.timeUs().div(1000#local#);

  if(timeCur > timeEnd)
  {
    job.deepDeleteNoRef();
    snake.deepDeleteNoRef();
    return;
  }
  else if((job.timeOkMs == null) || (timeCurMs > job.timeOkMs))
  {
    snake.shiftLines();
    if(job.timeOkMs == null) { job.timeOkMs = 0; }
    job.timeOkMs.set(timeCurMs).add(periodMs);
  }
  job.schedule();
};

LOCAL init = function()#global#
{
  this.screen = new screen{100#local#, 30#local#}#screen#;

  LOCAL snake_ = new snake{this.screen, 0#local#, 50#local#, "32;40"#local#};
  new cmap.scheduler.job{function()#global#{
    shift(this, snake_, 100#local#); }}.schedule();

  snake_ = new snake{this.screen, 50#local#, 100#local#, "31;40"#local#};
  new cmap.scheduler.job{function()#global#{
    shift(this, snake_, 300#local#); }}.schedule();

  this.screen.display();

  LOCAL jobDisplay = new cmap.scheduler.job{display}.schedule();
  jobDisplay.screen = this.screen;

  this.delete();
};

new cmap.scheduler.job{init}.schedule();

]]
