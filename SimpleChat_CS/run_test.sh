#!/bin/bash

cd /home/chasen/lcz/learn_linux/linux应用开发/class/SimpleChat_CS

echo "========== 开始测试 =========="
echo ""

# 启动服务器
echo "[1/4] 启动服务器..."
./server/server &
SERVER_PID=$!
sleep 2

# 检查服务器是否启动
if ps -p $SERVER_PID > /dev/null; then
    echo "✓ 服务器启动成功 (PID: $SERVER_PID)"
else
    echo "✗ 服务器启动失败"
    exit 1
fi

echo ""
echo "[2/4] 启动客户端..."

# 启动客户端
echo -e "TestUser\nhello\nexit" | ./client/client &
CLIENT_PID=$!
sleep 2

echo ""
echo "[3/4] 等待客户端执行完毕..."
wait $CLIENT_PID 2>/dev/null || true

echo ""
echo "[4/4] 清理进程..."
kill $SERVER_PID 2>/dev/null || true
wait $SERVER_PID 2>/dev/null || true

echo ""
echo "========== 测试完成 =========="
