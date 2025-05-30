[INCLUDE[

#include "random.h"
#include "snake.h"
#include "screen.h"

]]
[CMAP[

@PROC(random_global_env);
@PROC(snake);
@PROC(screen);

snake1TimeUs = 100.valueOf(cmap.cli.args[[1]]);
snake2TimeUs = 300.valueOf(cmap.cli.args[[2]]);
screenTimeUs = 40.valueOf(cmap.cli.args[[3]]);

timeEnd = 0.time().add(30);

local display = function()
{
  if(0.time() > timeEnd) { this.screen.showCursor(); }
  else
  {
    this.schedule();

    local timeCurMs = 0.timeUs().div(1000);
    if((this.timeOkMs == null) || (timeCurMs > this.timeOkMs))
    {
      this.screen.up().display();
      if(this.timeOkMs == null) { this.timeOkMs = 0; }
      this.timeOkMs.set(timeCurMs).add(screenTimeUs);
    }
  }
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
  local screen = new screen(100, 30).hideCursor().display();

  local snake_ = new snake(screen, 0, 50, "32;40");
  new cmap.scheduler.job(function(){
    shift(this, snake_, snake1TimeUs); }).schedule();

  snake_ = new snake(screen, 50, 100, "31;40");
  new cmap.scheduler.job(function(){
    shift(this, snake_, snake2TimeUs); }).schedule();

  new cmap.scheduler.job(display).schedule().screen = screen;
};

new cmap.scheduler.job(init).schedule();

]]
