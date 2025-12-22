
snake = function(screen, start, stop, ctl) {
  this.screen = screen;
  this.ctl = ctl;

  this.start = 0.valueOf(start);
  this.stop = 10.valueOf(stop);

  this.initLine();

  this.nbLoop = 0;
};

snake.prototype = {
  SNAKE_PARTS: [ "/", "|", "\\" ],

  SPACE: " ",

  RND_MAX_INIT: 10,
  RND_MAX_NEXT: 4,
  RND_MAX_NEW: 40,

  initLine: function() {
    local line = this.screen.height() - 1;
    local rnd = 0;
    for(local i = this.start.clone(); i < this.stop; i++) {
      random(rnd, this.RND_MAX_INIT);

      local pt = this.screen.get(i, line);
      if(rnd == 0) { pt.set(this.SNAKE_PARTS[[1]], this.ctl); }
      else { pt.set(this.SPACE, this.ctl); }
    }
  },

  follow: function(col, nextLine) {
    local rnd = 0;
    random(rnd, this.RND_MAX_NEXT);
    if(rnd < this.SNAKE_PARTS.size()) {
      local nextCol = col - 1 + rnd;
      if((nextCol >= this.start) && (nextCol < this.stop)) {
        this.screen.get(nextCol, nextLine).set(this.SNAKE_PARTS[rnd], null);
      }
    }
  },

  maybeLife: function(col, nextLine) {
    local rnd = 0;
    random(rnd, this.RND_MAX_NEW);
    if(rnd == 0) {
      this.screen.get(col, nextLine).set(this.SNAKE_PARTS[[1]], null);
    }
  },

  eraseLine: function(line) {
    for(local i = this.start.clone(); i < this.stop; i++) {
      this.screen.get(i, line).set(this.SPACE, null);
    }
  },

  nextLine: function(line) {
    local nextLine = line + 1;
    this.eraseLine(nextLine);

    for(local i = this.start.clone(); i < this.stop; i++) {
      local pt = this.screen.get(i, line);
      if(pt.let != this.SPACE) { this.follow(i, nextLine); }
      else { this.maybeLife(i, nextLine); }
    }
  },

  copyLine: function(line) {
    local prevLine = line - 1;
    for(local i = this.start.clone(); i < this.stop; i++) {
      local pt = this.screen.get(i, line);
      this.screen.get(i, prevLine).set(pt.let, pt.ctl);
    }
  },

  shiftLines: function() {
    local height = this.screen.height();
    for(local i = 1; i < height; i++) { this.copyLine(i); }
    this.nextLine(height - 2);
    this.nbLoop++;
  }
};
