
@IMPORT("random.cmap", random_);
@IMPORT("snake.cmap");
@IMPORT("screen.cmap");

snake1TimeMs = 100.valueOf(cmap.cli.args[[1]]);
snake2TimeMs = 300.valueOf(cmap.cli.args[[2]]);
screenTimeMs = 40.valueOf(cmap.cli.args[[3]]);

local infos = function(title, o) {
  cmap.console.info("[", title, ":", o, "] ", o.nbLoop, " loop(s); max ",
    o.timeMaxUs / 1000, "ms");
};

local end = function() {
  this.screen.showCursor();
  this.jobs.apply(function(job) {
    job.stop();
    job.infos();
  });
};

local init = function() {
  local jobs = [];

  local screen = new screen(100, 30).hideCursor().display();
  job = new cmap.job(function(){ screen.up().display(); })
    .scheduleMs(0, screenTimeMs);
  job.infos = function(){ infos("screen", screen); };
  jobs.push(job);

  local snake_ = new snake(screen, 0, 50, "32;40");
  local job = new cmap.job(function(){ snake_.shiftLines(); })
    .scheduleMs(0, snake1TimeMs);
  job.infos = function(){ infos("snake1", snake_); };
  jobs.push(job);

  snake_ = new snake(screen, 50, 100, "31;40");
  job = new cmap.job(function(){ snake_.shiftLines(); })
    .scheduleMs(0, snake2TimeMs);
  job.infos = function(){ infos("snake2", snake_); };
  jobs.push(job);

  job = new cmap.job(end).scheduleMs(30000);
  job.jobs = jobs;
  job.screen = screen;
};

new cmap.job(init).scheduleMs();
