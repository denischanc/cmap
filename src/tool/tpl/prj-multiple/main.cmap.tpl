[INCLUDE[

#include "random.h"
#include "snake.h"
#include "screen.h"

]]
[CMAP[

@PROC(random_global_env);
@PROC(snake);
@PROC(screen);

timeEnd = 0.time().add(30);

local display = function()
{
  this.screen.up().display();

  if(0.time() > timeEnd) { this.delete(); }
  else { this.schedule(); }
};

local shift = function(job, snake, periodMs)
{
  local timeCur = 0.time();
  local timeCurMs = 0.timeUs().div(1000);

  if(timeCur > timeEnd) { return; }
  else if((job.timeOkMs == null) || (timeCurMs > job.timeOkMs))
  {
    snake.shiftLines();
    if(job.timeOkMs == null) { job.timeOkMs = 0; }
    job.timeOkMs.set(timeCurMs).add(periodMs);
  }
  job.schedule();
};

local init = function()
{
  this.screen = new screen(100, 30);

  local snake_ = new snake(this.screen, 0, 50, "32;40");
  new cmap.scheduler.job(function(){
    shift(this, snake_, 100); }).schedule();

  snake_ = new snake(this.screen, 50, 100, "31;40");
  new cmap.scheduler.job(function(){
    shift(this, snake_, 300); }).schedule();

  this.screen.display();

  local jobDisplay = new cmap.scheduler.job(display).schedule();
  jobDisplay.screen = this.screen;

  this.delete();
};

new cmap.scheduler.job(init).schedule();

]]
