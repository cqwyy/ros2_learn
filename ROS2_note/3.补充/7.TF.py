#静态广播器----推荐用命令
# tf2_ros功能包中提供了一个名为static_transform_publisher的可执行文件，通过该文件可以直接广播静态坐标系关系
# ros2 run tf2_ros static_transform_publisher --x x --y y --z z --yaw yaw --pitch pitch --roll roll(roll、pitch、yaw滚动，俯仰，偏航)
# 旋转单位是弧度，逆时针为正（此外还要像下面这样指定父子坐标系）
# ros2 run tf2_ros static_transform_publisher --frame-id base_link --child-frame-id laser --x x --y y --z z --yaw yaw --pitch pitch --roll roll
# 给定一个父子级坐标系关系，坐标系不存在则创建坐标系，坐标系存在则创建父子关系
# 终端打开rviz2，指定Global Options下的Fixed Frame为父级坐标系，然后添加TF，然后Show Names


# 动态广播：启动乌龟节点，动态发布乌龟坐标系和世界坐标系的关系（广播变换的数据）

# 坐标点发布案例：模拟发布无人车上激光雷达扫描的障碍物坐标（广播是发布关系，这里是发布具体的坐标点）
# TF是订阅坐标系与坐标系的关系，发布坐标点是坐标点相对于坐标系的关系，所以要有广播才能显示坐标系