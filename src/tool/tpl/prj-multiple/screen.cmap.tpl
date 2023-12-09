[CMAP[

screen = function(nbCol, nbLine)#global#
{
  this.lines = []#screen#;
  for(LOCAL i = 0#local#;; i < nbLine; i.inc();)
  {
    this.lines.push(new screen.line{nbCol}#screen#);
  }
};

screen.ESCAPE = "\e["#global#;
screen.M = "m"#global#;
screen.A = "A"#global#;
screen.RESET = "\e[0m"#global#;

screen.prototype =
{
  ctl: function(ctl)#global#
  {
    cmap.console.info(screen.ESCAPE, ctl);
  },

  up: function()#global#
  {
    this.ctl(""#local#.append(this.height().inc()).append(screen.A));
    return this;
  },

  display: function()#global#
  {
    this.lines.apply(function(line)#local#{ line.display(); });
    return this;
  },

  get: function(col, line)#global#
  {
    return this.lines[line].get(col);
  },

  length: function()#global#
  {
    return this.lines[[0]].length();
  },

  height: function()#global#
  {
    return this.lines.size();
  }
}#global#;

screen.line = function(nbCol)#global#
{
  this.pts = []#screen#;
  for(LOCAL i = 0#local#;; i < nbCol; i.inc();)
  {
    this.pts.push(new screen.line.pt{}#screen#);
  }
};

screen.line.prototype =
{
  display: function()#global#
  {
    LOCAL line = ""#local#;
    this.pts.apply(function(pt)#local#
    {
      line.append(screen.ESCAPE).append(pt.ctl).append(screen.M)
        .append(pt.let).append(screen.RESET);
    });
    cmap.console.info(line);
  },

  get: function(col)#global#
  {
    return this.pts[col];
  },

  length: function()#global#
  {
    return this.pts.size();
  }
}#global#;

screen.line.pt = function()#global#
{
  this.let = ""#screen#.append(this.LET_DFT);
  this.ctl = ""#screen#.append(this.CTL_DFT);
};

screen.line.pt.prototype =
{
  LET_DFT: "#"#global#,
  CTL_DFT: "30;47"#global#,

  set: function(let, ctl)#global#
  {
    if(let != null) { this.let.clear().append(let); }
    if(ctl != null) { this.ctl.clear().append(ctl); }
  }
}#global#;

]]
