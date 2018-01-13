## mixServer
a event loop base tcp server.
## dependent：
### google protobuf python
  https://github.com/google/protobuf/releases/download/v3.5.1/protobuf-python-3.5.1.tar.gz
### python2.7 src:
  https://www.python.org/ftp/python/2.7.14/Python-2.7.14rc1.tgz
  
## build:
using SCons(makefile not update):http://scons.org/

cd mixSerever

scons

## features
* Linux use epoll.
* Macos use kqueue.
* python script embed.
* http server,websoekct server.


