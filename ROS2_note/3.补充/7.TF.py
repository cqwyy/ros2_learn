#静态广播器（本质是发布消息（发布相对关系），静态和动态都各有固定的话题）----推荐用命令实现静态广播
# tf2_ros功能包中提供了一个名为static_transform_publisher的可执行文件，通过该文件可以直接广播静态坐标系关系
# ros2 run tf2_ros static_transform_publisher --x x --y y --z z --yaw yaw --pitch pitch --roll roll(roll、pitch、yaw滚动，俯仰，偏航)
# 旋转单位是弧度，逆时针为正（此外还要像下面这样指定父子坐标系）
# ros2 run tf2_ros static_transform_publisher --frame-id base_link --child-frame-id laser --x x --y y --z z --yaw yaw --pitch pitch --roll roll
# 给定一个父子级坐标系关系，坐标系不存在则创建坐标系，坐标系存在则创建父子关系
# 终端打开rviz2，指定Global Options下的Fixed Frame为父级坐标系，然后添加TF，然后Show Names


# 动态广播：启动乌龟节点，动态发布乌龟坐标系和世界坐标系的关系（广播变换的数据）

# 坐标点发布案例：模拟发布无人车上激光雷达扫描的障碍物坐标（广播是发布关系，这里是发布具体的坐标点）
# TF是订阅坐标系与坐标系的关系，发布坐标点是坐标点相对于坐标系的关系，所以发布坐标点看不见坐标系，要有广播才能显示坐标系

# 坐标变换监听：监听可以实现坐标点在不同坐标系之间的变换或者不同坐标系之间的变换（求变换关系）
# TODO 监听是监听所有变换关系（广播的时候发布的话题是两个固定话题），然后用转换算法求转换关系
# 监听这里需要接收多个消息的发布，进而组织坐标关系树，所以要一个缓存来存储数据
# 还需要一个异常处理，比如要进行两个坐标系的变换，但是此时只收到一个坐标系的消息

# 坐标点变换：实现坐标点在不同坐标系下的变换。目前已知坐标系相对关系，以及坐标点到雷达的关系
# 逻辑：现订阅发布的数据（同如上坐标变换监听，可以监听坐标系关系。并需要另外创建一个订阅对象，订阅坐标点数据）。用过滤器整合两部分数据，求解结果
# 求解逻辑：按照之前逻辑创建缓存以及坐标系监听器，这里不同的是，缓存还要有一个定时器，用来统一缓存的坐标系和外部的座标点。
# 订阅座标点，创建过滤器。过滤器创建时候传入了坐标系变换关系以及座标点订阅对象，内部自动整合数据。然后在过滤器回调函数中直接处理数据，指定把点转换到哪个坐标系。

