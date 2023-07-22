@echo off

set cmd="strIP=127.0.0.1"
set cmd=%cmd% nPort=4567
set cmd=%cmd% nThread=1
set cmd=%cmd% nClient=10000
set cmd=%cmd% nMsg=100
set cmd=%cmd% nSendSleep=1000
set cmd=%cmd% nSendBufSize=81920
set cmd=%cmd% nRecvBufSize=81920
set cmd=%cmd% -checkMsgID
set cmd=%cmd% logPath=G:\_projects\CppStudy\src\Server1.0\bin\log\

EasyTcpClient %cmd%

pause