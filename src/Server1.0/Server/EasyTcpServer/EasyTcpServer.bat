@echo off

::set strIP="any"
::set nPort=4567
rem 消息处理线程数量
::set nThread=1
rem 客户端连接上限
::set nClient=2
rem 日志输出目录
::set logPath="G:\_projects\CppStudy\src\Server1.0\bin\log\"

set cmd="strIP=any"
set cmd=%cmd% nPort=4567
set cmd=%cmd% nThread=2
set cmd=%cmd% nClient=3
set cmd=%cmd% logPath=G:\_projects\CppStudy\src\Server1.0\bin\log\

EasyTcpServer %cmd%

pause