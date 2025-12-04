#!/bin/bash
# 手动交互式测试脚本

cd /home/chasen/lcz/learn_linux/linux应用开发/class/SimpleChat_CS

echo "======================================="
echo "      SimpleChat C/S 系统测试"
echo "======================================="
echo ""
echo "按以下步骤进行手动测试："
echo ""
echo "1. 在终端1中启动服务器："
echo "   cd /home/chasen/lcz/learn_linux/linux应用开发/class/SimpleChat_CS"
echo "   ./server/server"
echo ""
echo "2. 在终端2中启动客户端1："
echo "   cd /home/chasen/lcz/learn_linux/linux应用开发/class/SimpleChat_CS"
echo "   ./client/client"
echo "   输入用户名: Alice"
echo "   输入消息: hello"
echo ""
echo "3. 在终端3中启动客户端2："
echo "   cd /home/chasen/lcz/learn_linux/linux应用开发/class/SimpleChat_CS"
echo "   ./client/client"
echo "   输入用户名: Bob"
echo "   输入消息: hi there"
echo ""
echo "4. 观察消息广播效果"
echo ""
echo "======================================="
echo ""
echo "自动化测试（两个客户端）："
echo ""

# 启动服务器
./server/server > /tmp/server_output.log 2>&1 &
SERVER_PID=$!
sleep 2

# 启动第一个客户端
(
  sleep 0.5
  echo "Alice"
  sleep 0.5
  echo "Hi, everyone!"
  sleep 0.5
  echo "exit"
) | ./client/client > /tmp/client1_output.log 2>&1 &
CLIENT1_PID=$!

sleep 1

# 启动第二个客户端
(
  sleep 0.5
  echo "Bob"
  sleep 0.5
  echo "Hello Alice!"
  sleep 0.5
  echo "exit"
) | ./client/client > /tmp/client2_output.log 2>&1 &
CLIENT2_PID=$!

# 等待客户端完成
wait $CLIENT1_PID 2>/dev/null || true
wait $CLIENT2_PID 2>/dev/null || true

# 清理服务器
sleep 1
kill $SERVER_PID 2>/dev/null || true
wait $SERVER_PID 2>/dev/null || true

echo "========== 服务器日志 =========="
cat /tmp/server_output.log
echo ""
echo "========== 客户端1日志 =========="
cat /tmp/client1_output.log
echo ""
echo "========== 客户端2日志 =========="
cat /tmp/client2_output.log
