@echo off

::set strIP="any"
::set nPort=4567
rem ��Ϣ�����߳�����
::set nThread=1
rem �ͻ�����������
::set nClient=2
rem ��־���Ŀ¼
::set logPath="G:\_projects\CppStudy\src\Server1.0\bin\log\"

set cmd="strIP=any"
set cmd=%cmd% nPort=4567
set cmd=%cmd% nThread=4
set cmd=%cmd% nMaxClient=10240
set cmd=%cmd% nSendBufSize=81920
set cmd=%cmd% nRecvBufSize=81920
set cmd=%cmd% logPath=G:\_projects\CppStudy\src\Server1.0\bin\log\

set cmd=%cmd% -sendback
set cmd=%cmd% -sendfull
set cmd=%cmd% -checkMsgID

EasyTcpServer %cmd%

pause