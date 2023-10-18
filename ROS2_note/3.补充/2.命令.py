# ROS2中常用的命令如下：
# 补一个更常用的键盘控制：ros2 run teleop_twist_keyboard teleop_twist_keyboard
#
# ros2 node：节点相关命令行工具
# ros2 interface：接口(msg、srv、action)消息相关的命令行工具
# ros2 topic：话题通信相关的命令行工具
# ros2 service：服务通信相关的命令行工具
# ros2 action：动作通信相关的命令行工具
# ros2 param：参数服务相关的命令行工具
# 关于命令的使用一般都会提供帮助文档，帮助文档的获取方式如下：
#
# 可以通过命令 -h 或 命令 --help的方式查看命令帮助文档，比如：ros2 node -h或 ros2 node --help。
#
# 命令下参数的使用也可以通过命令 参数 -h 或 命令 参数 --help的方式查看帮助文档，比如：ros2 node list -h或 ros2 node list --help。
# 所有命令都可以通过-h -help获取帮助 比如先ros2 -h  然后看见有ros2 node 再继续ros2 node -h

# ros2 node的基本使用语法如下：
# ros2 node info 节点名 输出节点信息
# ros2 node list       输出运行中的节点的列表

# ros2 interface的基本使用语法如下：
# list      输出所有可用的接口消息
# package 包名  输出指定功能包下的接口消息
# packages  输出包含接口消息的功能包
# proto   接口名  输出接口消息原型（不显示注释）
# show      输出接口消息定义格式（会显示注释）

# ros2 topic的基本使用语法如下：
# bw 话题名      输出话题消息传输占用的带宽
# delay 话题名    输出带有 header 的话题延迟
# echo 话题名    输出某个话题下的消息
# find 消息类型    根据消息接口类型查找话题
# hz 话题名      输出消息发布频率
# info 话题名    输出话题相关信息--消息类型，发布者，订阅者信息
# list     输出运行中的话题列表
# pub -r 10(指定频率) 话题名 消息（table补齐--消息类型  table补齐知道要设置什么"{'linear':{'x':1.0},'angular':{'z':0.5}}"--json格式）     向指定话题发布消息
# type 话题名    输出话题使用的消息接口类型

# ros2 service的基本使用语法如下：
# call 服务名（list列出的） 消息类型(table) 数据(table) 向某个服务发送请求  数据组织都是"{'key':value}"类似
# find  接口类型 根据接口类型查找服务
# list  输出运行中的服务列表
# type  服务名 输出服务使用的接口类型

# ros2 action的基本使用语法如下：
# info 动作名（list输出的）      输出指定动作的相关信息
# list       输出运行中的动作的列表
# send_goal 动作名 消息类型 数据 --feedback(获取连续反馈信息) 向指定动作发送请求

# ros2 param的基本使用语法如下：
# delete 节点名 参数的键（名）   删除参数（有的参数不能删除）
# describe 节点名 参数的键（名） 输出参数的描述信息
# dump  节点名（打印到终端） --output-dir 路径（参数就存到路径了）     将节点参数写入到磁盘文件
# get 节点名 参数的键（名）       获取某个参数
# list      输出可用的参数的列表
# load 节点名 参数路径     从磁盘文件加载参数到节点
# set 节点名 参数的键（名） 目标值      设置参数
# 启动节点时也可以设置参数 ros2 run 包名 节点名 --ros-args -p 键:=值
#                                                    --params-file 文件路径

# rqt看一下讲义，摸索着用就行 注意topic的加号加到编辑区，以及方框打对勾开始下发，等等吧，摸索着用就行