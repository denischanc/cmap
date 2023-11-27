[CMAP[

LOCAL name = cmap.cli.args[[1]];
if(name == null) { name = "World"#t#; }
LOCAL max = 3#t#.valueOf(cmap.cli.args[[2]]);

LOCAL hello = function(name)#t#
{
  cmap.console.info("Hello "#t#, name, " !!!"#t#);
};

LOCAL helloWorld = function()#t#
{
  if(this.nb == null) { this.nb = 1; }

  hello(name);

  if(this.nb >= max) { this.deepDeleteNoRef(); return; }
  else { this.nb.inc(); }

  this.schedule();
};

LOCAL job = new cmap.scheduler.job{helloWorld};
job.schedule();

]]
