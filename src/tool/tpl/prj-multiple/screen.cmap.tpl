
screen = function(nbCol, nbLine) {
  this.lines = [];
  for(local i = 0; i < nbLine; i++) {
    this.lines.push(new screen.line(nbCol));
  }
  this.nbLoop = 0;
};

/* https://en.wikipedia.org/wiki/ANSI_escape_code */

screen.ESCAPE = "\e[";
screen.M = "m";
screen.A = "A";
screen.HIDE_CURSOR = "?25l";
screen.SHOW_CURSOR = "?25h";
screen.RESET = "\e[0m";

screen.prototype = {
  ctl: function(ctl) {
    cmap.console.print(screen.ESCAPE, ctl);
  },

  hideCursor: function() {
    this.ctl(screen.HIDE_CURSOR);
    return this;
  },

  showCursor: function() {
    this.ctl(screen.SHOW_CURSOR);
    return this;
  },

  up: function() {
    this.ctl("" + this.height() + screen.A);
    return this;
  },

  display: function() {
    this.lines.apply(function(line){ line.display(); });
    this.nbLoop++;
    return this;
  },

  get: function(col, line) {
    return this.lines[line].get(col);
  },

  length: function() {
    return this.lines[[0]].length();
  },

  height: function() {
    return this.lines.size();
  }
};

screen.line = function(nbCol) {
  this.pts = [];
  for(local i = 0; i < nbCol; i++) {
    this.pts.push(new screen.line.pt());
  }
};

screen.line.prototype = {
  display: function() {
    local line = "";
    this.pts.apply(function(pt) {
      line.append(screen.ESCAPE, pt.ctl, screen.M, pt.let, screen.RESET);
    });
    cmap.console.info(line);
  },

  get: function(col) {
    return this.pts[col];
  },

  length: function() {
    return this.pts.size();
  }
};

screen.line.pt = function() {
  this.let = this.LET_DFT;
  this.ctl = this.CTL_DFT;
};

screen.line.pt.prototype = {
  LET_DFT: "#",
  CTL_DFT: "30;47",

  set: function(let, ctl) {
    if(let) { this.let = let; }
    if(ctl) { this.ctl = ctl; }
  }
};
