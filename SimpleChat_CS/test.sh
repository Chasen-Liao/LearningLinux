#!/bin/bash
# 测试脚本

# 启动服务器
cd /home/chasen/lcz/learn_linux/linux应用开发/class/SimpleChat_CS

echo "=== 启动服务器 ==="
./server/server &
SERVER_PID=$!
sleep 1

echo ""
echo "=== 启动客户端 ==="
echo "user1" | timeout 3 ./client/client

sleep 1
kill $SERVER_PID 2>/dev/null
wait $SERVER_PID 2>/dev/null

echo ""
echo "=== 测试完成 ==="
