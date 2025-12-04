#!/bin/bash
# 测试服务器断开连接时客户端的行为

cd /home/chasen/lcz/learn_linux/linux应用开发/class/SimpleChat_CS

echo "========== 服务器断开连接测试 =========="
echo ""
echo "此测试演示："
echo "1. 启动服务器"
echo "2. 启动客户端"
echo "3. 客户端发送消息"
echo "4. 服务器在10秒后断开连接"
echo "5. 客户端应该收到提醒并退出"
echo ""
echo "=========================================="
echo ""

# 启动服务器
./server/server > /tmp/server_test.log 2>&1 &
SERVER_PID=$!
sleep 2

echo "[$(date '+%H:%M:%S')] 服务器已启动 (PID: $SERVER_PID)"
echo ""

# 启动客户端，设置延时等待服务器断开
(
  sleep 0.5
  echo "TestUser"
  sleep 1
  echo "Message 1"
  sleep 1
  echo "Message 2"
  sleep 3
  # 等待服务器断开，此时应该看到错误提示
  sleep 5
  echo "这行不应该被看到"
) | timeout 15 ./client/client &
CLIENT_PID=$!

echo "[$(date '+%H:%M:%S')] 客户端已启动 (PID: $CLIENT_PID)"
echo ""

# 给客户端一点时间连接
sleep 3

echo "[$(date '+%H:%M:%S')] 现在杀死服务器以测试客户端的断开连接处理..."
kill $SERVER_PID 2>/dev/null

echo "[$(date '+%H:%M:%S')] 服务器已被终止"
echo ""
echo "等待客户端反应..."
sleep 3

echo ""
echo "========== 测试结果 =========="
echo ""
echo "客户端应该在服务器断开后显示:"
echo "  ❌ 服务器已断开连接！"
echo "  💤 程序即将退出..."
echo ""
echo "然后自动退出程序。"
echo ""

# 清理
kill $CLIENT_PID 2>/dev/null || true
wait $CLIENT_PID 2>/dev/null || true

echo "========== 测试完成 =========="
