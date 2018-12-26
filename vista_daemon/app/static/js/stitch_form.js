//function addData(layui) {
	layui.use('jquery', function() {
		var $ = layui.jquery;
		var submit = function() {
			return false;
		};
		$('#test').on('submit', function() {
			return false
		});
		$('#test').on('submit', function() {
			return true
		});
	});

	layui.use('element', function() {
		var element = layui.element;

	});

	layui.use('form', function() {
		var form = layui.form;

		//自定义验证规则
		form.verify({
			title: function(value) {
				if(value.length < 5) {
					return '标题也太短了吧';
				}
			},
			pass: [/(.+){6,12}$/, '密码必须6到12位'],
			money: [
				/^\d+\.\b\d{2}\b$/, '金额必须为小数保留两位'
			]
		});

		//初始赋值
		form.val('first', {
//			'title': '测试',
			'phone': 11111111111,
			'email': 'xu@sentsin.com',
			'password': 123123
				//,'quiz': 2
				,
			'interest': 2,
			'like[write]': true
				//,'open': false
				,
			'sex': '男',
			'desc': 'form 是我们非常看重的一块'
		});

		//事件监听
		form.on('select', function(data) {
			console.log('select: ', this, data);
		});

		form.on('select(quiz)', function(data) {
			console.log('select.quiz：', this, data);
		});

		form.on('select(interest)', function(data) {
			console.log('select.interest: ', this, data);
		});

		form.on('checkbox', function(data) {
			console.log(this.checked, data.elem.checked);
		});

		form.on('switch', function(data) {
			console.log(data);
		});

		form.on('radio', function(data) {
			console.log(data);
		});

		//监听提交
		form.on('submit(*)', function(data) {
			console.log(data)
			alert(JSON.stringify(data.field));
			return false;
		});

	});
	layui.use(['element', 'layer'], function(){
		var element = layui.element;
		var layer = layui.layer;
		  
		//监听折叠
		element.on('collapse(test)', function(data){
		    layer.msg('展开状态：'+ data.show);
		 });
	});
//}