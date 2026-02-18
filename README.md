CMAP
====

![Build](https://github.com/denischanc/cmap/actions/workflows/build.yml/badge.svg)

This project introduce natively map, list into c. It defines
**cmap** language related to javascript, nodejs.

## How to install version x.y.z

```shell
tar xf cmap-x.y.z.tar.xz
cd cmap-x.y.z
./configure --prefix=/cmap/install/dir
make
make install
```

## Quick start

```shell
/cmap/install/dir/bin/cmap-tool prj hello-world
cd hello-world
make
export LD_LIBRARY_PATH=/cmap/install/dir/lib:$LD_LIBRARY_PATH
./hello-world yourname 5
```

* have a look to [hello-world.cmap](src/tool/tpl/prj-simple/hello-world.cmap.tpl)
* Makefile allow to build hello-world : **make**, clean project : **make clean**
