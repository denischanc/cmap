[CMAP[

snake = function(screen, start, stop, ctl)
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
  SNAKE_PARTS: [ "/", "|", "\\" ],

  SPACE: " ",

  RND_MAX_INIT: 10,
  RND_MAX_NEXT: 4,
  RND_MAX_NEW: 40,

  initLine: function()
  {
    local line = this.screen.height().dec();
    local rnd = 0;
    for(local i = 0.set(this.start);; i < this.stop; i.inc();)
    {
      random(rnd, this.RND_MAX_INIT);

      local pt = this.screen.get(i, line);
      if(rnd == 0) { pt.set(this.SNAKE_PARTS[[1]], this.ctl); }
      else { pt.set(this.SPACE, this.ctl); }
    }
  },

  follow: function(col, nextLine)
  {
    local rnd = 0;
    random(rnd, this.RND_MAX_NEXT);
    if(rnd < this.SNAKE_PARTS.size())
    {
      local nextCol = 0.set(col).dec().add(rnd);
      if((nextCol >= this.start) && (nextCol < this.stop))
      {
        this.screen.get(nextCol, nextLine).set(this.SNAKE_PARTS[rnd], null);
      }
    }
  },

  maybeLife: function(col, nextLine)
  {
    local rnd = 0;
    random(rnd, this.RND_MAX_NEW);
    if(rnd == 0)
    {
      this.screen.get(col, nextLine).set(this.SNAKE_PARTS[[1]], null);
    }
  },

  eraseLine: function(line)
  {
    for(local i = 0.set(this.start);; i < this.stop; i.inc();)
    {
      this.screen.get(i, line).set(this.SPACE, null);
    }
  },

  nextLine: function(line)
  {
    local nextLine = 0.set(line).inc();
    this.eraseLine(nextLine);

    for(local i = 0.set(this.start);; i < this.stop; i.inc();)
    {
      local pt = this.screen.get(i, line);
      if(pt.let != this.SPACE) { this.follow(i, nextLine); }
      else { this.maybeLife(i, nextLine); }
    }
  },

  copyLine: function(line)
  {
    local prevLine = 0.set(line).dec();
    for(local i = 0.set(this.start);; i < this.stop; i.inc();)
    {
      local pt = this.screen.get(i, line);
      this.screen.get(i, prevLine).set(pt.let, pt.ctl);
    }
  },

  shiftLines: function()
  {
    local height = this.screen.height();
    for(local i = 1;; i < height; i.inc();) { this.copyLine(i); }
    this.nextLine(height.dec().dec());
  }
};

]]
