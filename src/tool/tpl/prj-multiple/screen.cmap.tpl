[CMAP[

screen = function(nbCol, nbLine)
{
  this.lines = [];
  for(local i = 0;; i < nbLine; i.inc();)
  {
    this.lines.push(new screen.line{nbCol});
  }
};

screen.ESCAPE = "\e[";
screen.M = "m";
screen.A = "A";
screen.RESET = "\e[0m";

screen.prototype =
{
  ctl: function(ctl)
  {
    cmap.console.info(screen.ESCAPE, ctl);
  },

  up: function()
  {
    this.ctl("".append(this.height().inc()).append(screen.A));
    return this;
  },

  display: function()
  {
    this.lines.apply(function(line){ line.display(); });
    return this;
  },

  get: function(col, line)
  {
    return this.lines[line].get(col);
  },

  length: function()
  {
    return this.lines[[0]].length();
  },

  height: function()
  {
    return this.lines.size();
  }
};

screen.line = function(nbCol)
{
  this.pts = [];
  for(local i = 0;; i < nbCol; i.inc();)
  {
    this.pts.push(new screen.line.pt{});
  }
};

screen.line.prototype =
{
  display: function()
  {
    local line = "";
    this.pts.apply(function(pt)
    {
      line.append(screen.ESCAPE).append(pt.ctl).append(screen.M)
        .append(pt.let).append(screen.RESET);
    });
    cmap.console.info(line);
  },

  get: function(col)
  {
    return this.pts[col];
  },

  length: function()
  {
    return this.pts.size();
  }
};

screen.line.pt = function()
{
  this.let = "".append(this.LET_DFT);
  this.ctl = "".append(this.CTL_DFT);
};

screen.line.pt.prototype =
{
  LET_DFT: "#",
  CTL_DFT: "30;47",

  set: function(let, ctl)
  {
    if(let != null) { this.let.clean().append(let); }
    if(ctl != null) { this.ctl.clean().append(ctl); }
  }
};

]]
