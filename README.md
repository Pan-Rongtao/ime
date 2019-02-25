README
===========================

`IME` is a platform-cross c++ components for windows, linux, qnx, mac osx, ios.
the components include `pinyin`, `associate`, `edic`, `stroke`, `wubi`

****
	
|Author|Pan.瞳|
|---|---
|E-mail|panrongtao@qq.com

# Depends
* c++11
* sqlite3
* cmake

	for sqlite3, you needn't build sqlite3 for windows yourself, extern/ dir contains the header file and libs.  
but for linux, qnx, osx, ios, you need to prepare sqlite3 yourself if system doesn't contain.  
please shell "sudo apt-get install libsqlite3-dev" to install sqlite3 dev, others please try yourself.

# Build（some platform build cost long time, wait minutes please)
------
### msvc
	cd ime
	./build_msvc14.bat

### linux
	cd ime
	mkdir build
	cd build
	cmake -DCMAKE_INSTALL_PREFIX=../dist ..
	make
	make install

### qnx
	. <qnxroot>/qnxsdp-env.sh
	cd ime
	mkdir build
	cd build
	cmake -DQNX_ARCH=[x86_64|aarch64] -DCMAKE_TOOLCHAIN_FILE=../qnx.cmake -DCMAKE_INSTALL_PREFIX=../dist ..
	make
	make install

### mac osx
	cd ime
	mkdir build
	cd build
	cmake -DCMAKE_INSTALL_PREFIX=../dist ..
	make
	make install

### ios(to be continue)

# Example
------

### pinyin
```cpp
#include "pinyin/Pinyin.h"

ime::pinyin::Pinyin m_pinyin;
bool b = m_pinyin.init("../../etc/system-py.db", "../../etc/learn-py.db");
m_pinyin.enableAICombineCandidate(true);
m_pinyin.enableAssociateCandidate(true);
m_pinyin.setCandidatePageSize(9);
m_pinyin.search("woaini");
std::vector<std::string> cans;
m_pinyin.getCandidateByPage(0, cans);	//"我爱你" ... 
```
### other components
	view tests in dir example/ for each component, they are build follow the project.  
