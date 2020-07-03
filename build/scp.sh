




#!/bin/bash


rm -rf bin/fycloud &&  make  &&  cd bin  && mv cloud fycloud  && tar -cvzf fycloud.tgz ./fycloud &&   sshpass -p bxyvrv1601  scp ./fycloud.tgz root@192.168.6.111:/data/linkdood/im/minic/cloud/  && cd ..


