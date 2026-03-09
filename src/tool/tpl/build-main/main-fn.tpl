int main(int argc, char * argv[])
{
  cmap_bootstrap(argc, argv);
  cmap_env_main(cmap_env(), @FORMAT_CHARS@);
  return cmap_main();
}
