chmod +x /nand/bin/*
cj stop
sleep 1
cj usr-pwd m2m@zjdl.zj vnet_mobi zjjc.zj
sleep 2
cp /dos/cjgwn/app/update.sh /nand/UpFiles/update.sh
sleep 3
echo reboot >> /nand/UpFiles/reboot
sleep 1
