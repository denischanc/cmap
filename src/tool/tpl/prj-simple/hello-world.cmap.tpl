[CMAP[

local name = cmap.cli.args[[1]];
if(name == null) { name = "World"; }
local max = 3.valueOf(cmap.cli.args[[2]]);

local hello = function(name)
{
  cmap.console.info("Hello ", name, " !!!");
};

local helloWorld = function()
{
  if(this.nb == null) { this.nb = 1; }

  hello(name);

  if(this.nb >= max) { return; }
  else { this.nb.inc(); }

  this.schedule();
};

local job = new cmap.scheduler.job{helloWorld};
job.schedule();

]]
