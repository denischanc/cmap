
@IMPORT("random.cmap", random_);
@IMPORT("snake.cmap");
@IMPORT("screen.cmap");

snake1TimeMs = 100.valueOf(cmap.cli.args[[1]]);
snake2TimeMs = 300.valueOf(cmap.cli.args[[2]]);
screenTimeMs = 40.valueOf(cmap.cli.args[[3]]);

timeEnd = 0.time() + 30;

report = "";
reportNb = 0;

local display = function() {
  if(0.time() > timeEnd) {
    if(reportNb < 2) { this.schedule(); }
    else {
      this.screen.showCursor();
      report += "[screen] " + this.screen.nbLoop + " loop(s)\n";
      cmap.console.print(report);
    }
  }
  else {
    this.schedule();

    local timeCurMs = 0.timeUs() / 1000;
    if((!this.timeOkMs) || (timeCurMs > this.timeOkMs)) {
      this.screen.up().display();
      if(!this.timeOkMs) { this.timeOkMs = timeCurMs + screenTimeMs; }
      else { this.timeOkMs += screenTimeMs; }
    }
  }
};

local shift = function(job, snake, periodMs) {
  local timeCur = 0.time();
  local timeCurMs = 0.timeUs() / 1000;

  if(timeCur > timeEnd) {
    report += "[snake:" + snake + "] " + snake.nbLoop + " loop(s)\n";
    reportNb++;
    return;
  }
  else if((!job.timeOkMs) || (timeCurMs > job.timeOkMs)) {
    snake.shiftLines();
    if(!job.timeOkMs) { job.timeOkMs = timeCurMs + periodMs; }
    else { job.timeOkMs += periodMs; }
  }
  job.schedule();
};

local init = function() {
  local screen = new screen(100, 30).hideCursor().display();

  local snake_ = new snake(screen, 0, 50, "32;40");
  new cmap.scheduler.job(function(){
    shift(this, snake_, snake1TimeMs); }).schedule();

  snake_ = new snake(screen, 50, 100, "31;40");
  new cmap.scheduler.job(function(){
    shift(this, snake_, snake2TimeMs); }).schedule();

  new cmap.scheduler.job(display).schedule().screen = screen;
};

new cmap.scheduler.job(init).schedule();
