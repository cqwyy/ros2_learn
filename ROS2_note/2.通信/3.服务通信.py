# ros2 pkg create cpp02_service --build-type ament_cmake --dependencies rclcpp base_interfaces_demo --node-name demo01_server
# 创建功能包，依赖于base_interfaces_demo，因为会把消息载体放在这个包，并且顺便创建节点，手动创建第一个节点很麻烦
# TODO 奇怪的一点：功能包不是生成了cpp源程序吗，导入了源程序源文件，为什么要依赖这个功能包---解释应该是不止依赖源程序，还有其他的东西，比如cmakelists
# TODO 创建.srv文件名首字母要大写
# ---分割，上面是请求数据，下面是响应数据2
# 编译完，在install/base_interfaces_demo/include/base_interfaces_demo下面有源码
# 客户端不用一起挂起，因为客户端请求发送一次，服务完就行了，没必要挂起。（ros2里也没法挂起，spin轮询处理回调进而产生挂起，客户端里没有回调函数，所以没法挂起）
# 配置文件还是cmakelist，跟之前一样三个地方
# 在发布订阅，没有发布方，订阅方就挂起，没有订阅方，发布方的数据就丢失。但是服务通信，客户端发送请求就是要响应，所以没有服务端就要等待服务端链接（不等的话发给
# 服务端的数据就丢了，自然也不会有响应）