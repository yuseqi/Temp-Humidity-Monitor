<template>
	<!-- 思路：先画头部页面（ctrl+/）
1、一个盒子里面套三个盒子
2、两个标签
3、一个标签+图片
4、一个标签 -->
	<view class="head-pages">
		<!-- 快捷键：alt+shift+下键，快速复制一行内容 -->
		<!-- 第一行 -->
		<view class="head-title">
			<view class="">空气质量-良</view>
			<view class="">江西-赣州</view>
		</view>
		<!-- 第二行 -->
		<view class="head-values">
			<view class="values-data">30℃</view>
			<image src="/static/sun.png" mode=""></image>
		</view>
		<!-- 第三行 -->
		<view class="head-advice">今天天气真好，适合出游！</view>

	</view>
	<!-- 思路：先画一个大盒子
1、大盒子
2、大盒子卡片盒子
3、卡片盒子，分左右两个盒子
4、右边的盒子上下盒子 -->
	<view class="body-pages">
		<!-- 温度卡片 -->
		<view class="body-cards">
			<view class="body-cards-left">
				<image src="/static/temp.png" mode=""></image>
			</view>
			<view class="body-cards-right">
				<view class="body-cards-right-up">
					温度
				</view>
				<view class="body-cards-right-down">
					{{temp}}℃
				</view>
			</view>
		</view>
		<!-- 湿度卡片 -->
		<view class="body-cards">
			<view class="body-cards-left">
				<image src="/static/humi.png" mode=""></image>
			</view>
			<view class="body-cards-right">
				<view class="body-cards-right-up">
					湿度
				</view>
				<view class="body-cards-right-down">
					{{humi}}%
				</view>
			</view>
		</view>
		<!-- LED卡片 -->
		<view class="body-cards">
			<view class="body-cards-left">
				<image src="/static/led.png" mode=""></image>
			</view>
			<view class="body-cards-right">
				<view class="body-cards-right-up">
					LED
				</view>
				<view class="body-cards-right-down">
					<switch checked="{{led}}" @change="onledchang" color="#1296db" />
				</view>
			</view>
		</view>
		<!-- 蜂鸣器卡片 -->
		<view class="body-cards">
			<view class="body-cards-left">
				<image src="/static/beep.png" mode=""></image>
			</view>
			<view class="body-cards-right">
				<view class="body-cards-right-up">
					蜂鸣器
				</view>
				<view class="body-cards-right-down">
					<switch checked="{{beep}}" @change="onbeepchange" color="#1296db" />
				</view>
			</view>
		</view>
	</view>

</template>

<script>
	// 安装一个MQTT包,用于连接serverless服务器
	// 1 先安装node.js,将node.js的安装路径放置到电脑环境变量里面
	// 2 检查命令node -v 查看是否安装完成。
	// 3 安装mqtt包，输入命令npm install mqtt@3.0.0
	// 4 通过命令 npm list 查看是否安装完成mqtt

	//导入mqtt.js包
	import mqtt from "mqtt/dist/mqtt.js"

	const devicesub = '/mcu/sub' //设备订阅主题（小程序APP发布主题）
	const devicepub = '/mcu/pub' //设备发布主题（小程序APP订阅主题）
	const mpsub = '/mcu/pub' //小程序APP订阅主题
	const mppub = '/mcu/sub' //小程序APP发布主题

	export default {
		data() {
			return {
				// 定义变量
				client: null, //客户端

				//数据的双向绑定，只需要标签使用2个大括号，{{}}，就可以数据绑定
				temp: 50,
				humi: 80,
				led: false,
				beep: false,

				// 1 定义服务器的变量
				client: null, //定义创建连接MQTT服务器的对象
				mqttHost: '122.51.25.139', //MQTT服务器的地址
				mqttport: 8083 //端口号
			}
		},
		onLoad() {

		},
		// 当页面刷新的调用
		// 连接MQTT的方法,来源mqtt.js包,所有必须导入import mqtt from "mqtt/dist/mqtt.js"
		// 1 配置服务器参数方法:client = mqtt.connect(url,{配置参数},回调函数)
		// 2 连接MQTT服务器方法:client.on('connect',回调函数)
		onShow() {
			console.log('页面刷新');
			var that = this;
			// 1配置服务器参数
			that.client = mqtt.connect('ws://122.51.25.139:8083/mqtt', {
				clientId: '20231294', //用户ID
				username: 'qyx', //用户名
				password: '123' //用户密码	
			}, function(err) {
				console.log('连接错误:' + err);
			});
			// 2连接服务器
			that.client.on('connect', function(err) {
				console.log('成功连接服务器');
				// 调用弹窗方法,提示连接成功
				uni.showToast({
					title:'成功连接服务器',
				})
			});
			//3小程序APP订阅一个主题
			//client.subscribe('订阅的主题'，回调函数);
			that.client.subscribe(mpsub, function(err) {
				if (!err) {
					console.log('订阅主题:' + mpsub + '成功');
				}
			});
			// 4 接收订阅的消息
			//client.on('message',function(topic,message));
			that.client.on('message', function(topic, message) {
				console.log(topic);
				console.log(message);
				let datafromdevice = {};
				datafromdevice = JSON.parse(message); //把message转换成json格式:key:value
				console.log(datafromdevice);
				// 解析数据
				that.temp = datafromdevice.temp;
				that.humi = datafromdevice.humi;

			});

		},
		//方法
		methods: {
			// 当按键切换时候调用
			onledchang(event) {
				var that = this;
				console.log("LED按键切换");
				console.log(event.detail.value);
				let ledstate = event.detail.value;
				// 5 通过按键发布消息
				// client.publish(发布的主题,"发布的消息",function(){});
				that.client.publish(mppub, JSON.stringify({
					led: ledstate
				}), function(err) {
					if (!err) {
						console.log('发布主题：' + mppub + '成功');
					}
				});
			},
			onbeepchange(event) {
				console.log("beep按键切换");
				var that = this;
				console.log(event.detail.value);
				let beepstate = event.detail.value;
				// 5 通过按键发布消息
				// client.publish(发布的主题,"发布的消息",function(){});
				that.client.publish(mppub, JSON.stringify({
					beep: beepstate
				}), function(err) {
					if (!err) {
						console.log('发布主题：' + mppub + '成功');
					}
				});
			}
		}
	}
</script>

<style>
	.head-pages {
		/* 上下左右边距 */
		margin: 0 32rpx;
		/* 背景颜色 */
		background-color: #1296db;
		/* 圆角 */
		border-radius: 64rpx;
	}

	.head-pages .head-title {
		/* 字体颜色 */
		color: white;
		/* 弹性盒子方式显示 */
		display: flex;
		/* 居中布局 */
		justify-content: space-between;
		/* 上下左右内边距 */
		padding: 32rpx 64rpx;
		/* 字体放大 */
		font-size: larger;
	}

	.head-pages .head-values {
		display: flex;
		color: white;
		font-size: xxx-large;
		/* 居中对齐排列 */
		align-items: center;
		padding: 0rpx 64rpx;
		justify-content: space-between;
	}

	.head-pages .head-values image {
		height: 200rpx;
		width: 200rpx;
	}

	.head-pages .head-advice {
		color: white;
		padding: 32rpx 64rpx;
	}

	/* body部分样式 */
	.body-pages {
		/* background-color: red; */
		margin-top: 32rpx;
		/* 卡片的高度 */
		height: 200rpx;
		/* 网格方式显示 */
		display: grid;
		/* 网格排列方式 */
		grid-template-columns: repeat(auto-fill, 320rpx);
		/* 网格之间的间隙 */
		grid-gap: 18rpx;
		/* 居中排列 */
		justify-content: center;
	}

	/* 小盒子样式 */
	.body-pages .body-cards {
		/* margin-top: 32rpx; */
		/* background-color: blue; */
		border-radius: 32rpx;
		box-shadow: 0 0 16rpx #a9a9a9;
		display: flex;
		justify-content: space-between;
		padding: 0rpx 20rpx;
		align-items: center;
	}

	.body-pages .body-cards .body-cards-left image {
		height: 100rpx;
		width: 100rpx;
	}

	.body-pages .body-cards .body-cards-right {
		/* background-color: pink; */
	}

	.body-pages .body-cards .body-cards-right .body-cards-right-up {
		/* background-color: red; */
		font-size: large;
		padding: 16rpx 0;
		font-weight: bold;
		color: gray;
	}

	.body-pages .body-cards .body-cards-right .body-cards-right-down {
		font-size: x-large;
		font-weight: bold;
		padding-bottom: 16rpx;
	}
</style>