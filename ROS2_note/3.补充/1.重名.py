# 用python编写launch
# cd到工作空间下的src----ros2 pkg create cpp05_names --build-type ament_cmake --node-name demo01_names --dependencies rclcpp
# 功能包下新建launch文件夹
# 配置功能包cmake 加入install(DIRECTORY launch DESTINATION share/${PROJECT_NAME})
# 命名python文件最后以_launch.py或者.launch.py结尾
# 编写文件
# 保存
# 回到工作空间编译
# ros2 launch cpp05_names demo01_names_launch.py 启动launch

# 命名空间也会作用到节点下的话题上
# 为话题起别名
# 语法： ros2 run 包名 节点名 --ros-args --remap 原话题名称:=新话题名称
# ros2 run turtlesim turtlesim_node --ros-args --remap /turtle1/cmd_vel:=/cmd_vel
#例子：ros2有个键盘控制的话题，乌龟有自己的键盘控制话题，但是这俩话题名不一样，要是想用ros2的键盘控制话题控制乌龟就需要把话题名改为相同


# 节点名称组成 根：/  命名空间（可有多级）  节点名
# 全局话题挂在根下，与命名空间平级，相对话题在命名空间下，与节点名平级，私有话题挂在节点名称下，是节点的子级
# /话题名（面向全部）            /命名空间/话题名（面向 同命名空间）  /命名空间/节点名/话题名（面向同节点）
# 创建语法(在创建的时候)
# /xxx                 xxx（到时候自动就加上了命名空间--节点所在的，因为他与节点平级）    ~/话题名（自动与节点名）
# TODO 话题也可以多级  xxx/xxx类型的多级话题名也可，不过是话题名的多级，与命名空间和节点无关
# TODO 以上话题名称设置适用于编码以及launch和ros2 run