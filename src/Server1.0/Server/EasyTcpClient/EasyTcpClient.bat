@echo off

set cmd="strIP=192.168.2.103"
set cmd=%cmd% nPort=4567
set cmd=%cmd% nThread=4
set cmd=%cmd% nClient=1000
set cmd=%cmd% nMsg=1
set cmd=%cmd% nSendSleep=1000
set cmd=%cmd% nSendBufSize=81920
set cmd=%cmd% nRecvBufSize=81920
set cmd=%cmd% logPath=G:\_projects\CppStudy\src\Server1.0\bin\log\

set cmd=%cmd% -checkMsgID

EasyTcpClient %cmd%

pause