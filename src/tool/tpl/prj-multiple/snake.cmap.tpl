[CMAP[

snake = function(screen, start, stop, ctl)#global#
{
  this.screen = screen;
  if(ctl != null) { this.ctl = "".append(ctl); }

  this.start = 0;
  if(start != null) { this.start.set(start); }
  this.stop = 10;
  if(stop != null) { this.stop.set(stop); }

  this.initLine();
};

snake.prototype =
{
  SNAKE_PARTS: [ "/"#global#, "|"#global#, "\\"#global# ]#global#,

  SPACE: " "#global#,

  VAL_0: 0#global#,

  RND_MAX_INIT: 10#global#,
  RND_MAX_NEXT: 4#global#,
  RND_MAX_NEW: 40#global#,

  initLine: function()#global#
  {
    LOCAL line = this.screen.height().dec();
    LOCAL rnd = 0#local#;
    for(LOCAL i = 0#local#.set(this.start);; i < this.stop; i.inc();)
    {
      random(rnd, this.RND_MAX_INIT);

      LOCAL pt = this.screen.get(i, line);
      if(rnd == this.VAL_0) { pt.set(this.SNAKE_PARTS[[1]], this.ctl); }
      else { pt.set(this.SPACE, this.ctl); }
    }
  },

  follow: function(col, nextLine)#global#
  {
    LOCAL rnd = 0#local#;
    random(rnd, this.RND_MAX_NEXT);
    if(rnd < this.SNAKE_PARTS.size())
    {
      LOCAL nextCol = 0#local#.set(col).dec().add(rnd);
      if((nextCol >= this.start) && (nextCol < this.stop))
      {
        this.screen.get(nextCol, nextLine).set(this.SNAKE_PARTS[rnd], null);
      }
    }
  },

  maybeLife: function(col, nextLine)#global#
  {
    LOCAL rnd = 0#local#;
    random(rnd, this.RND_MAX_NEW);
    if(rnd == this.VAL_0)
    {
      this.screen.get(col, nextLine).set(this.SNAKE_PARTS[[1]], null);
    }
  },

  eraseLine: function(line)#global#
  {
    for(LOCAL i = 0#local#.set(this.start);; i < this.stop; i.inc();)
    {
      this.screen.get(i, line).set(this.SPACE, null);
    }
  },

  nextLine: function(line)#global#
  {
    LOCAL nextLine = 0#local#.set(line).inc();
    this.eraseLine(nextLine);

    for(LOCAL i = 0#local#.set(this.start);; i < this.stop; i.inc();)
    {
      LOCAL pt = this.screen.get(i, line);
      if(pt.let != this.SPACE) { this.follow(i, nextLine); }
      else { this.maybeLife(i, nextLine); }
    }
  },

  copyLine: function(line)#global#
  {
    LOCAL prevLine = 0#local#.set(line).dec();
    for(LOCAL i = 0#local#.set(this.start);; i < this.stop; i.inc();)
    {
      LOCAL pt = this.screen.get(i, line);
      this.screen.get(i, prevLine).set(pt.let, pt.ctl);
    }
  },

  shiftLines: function()#global#
  {
    LOCAL height = this.screen.height();
    for(LOCAL i = 1#local#;; i < height; i.inc();) { this.copyLine(i); }
    this.nextLine(height.dec().dec());
  }
}#global#;

]]
