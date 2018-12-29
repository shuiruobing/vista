function creatpano(content)
{
	var $jqP3 = $('<div class="box"> <div class="stichitem"> <h1>'+content["text"]+'</h1> </div>	</div>');
	$("#water_grid").append($jqP3);
}
function itemWaterfull(){
    var margin = 0;  //每个item的外边距，因人需求而定
    var items = $(".box");
    console.log(items);
	var margin = parseInt(items.eq(0).css("left"));
	console.log("每个item的外边距:"+ margin);
    var item_width = parseInt(items.eq(0).css("width"));
    item_width +=  2*margin; //取区块的实际宽度
    console.log("区块的实际宽度:"+ item_width);
    $(".box").css("padding", "0");  //容器的起始内边距先设为0，按之后一行item的宽度再来设，保证所有item的居中
    var container_width = $("#body_main_content")[0].offsetWidth; //获取容器宽度
    console.log("容器宽度:"+ container_width);
    var n = parseInt(container_width / item_width);  //一行所允许放置的item个数
    console.log("一行所允许放置的item个数:"+n);
//	    var container_padding = (container_width - (n * item_width)) / 2; //一行宽度在容器中所剩余的宽度，设为容器的左右内边距
//	    console.log("容器的左右内边距:"+container_padding);
//	    $(".box").css("padding", "0 " + container_padding + "px");
    //寻找数组最小高度的下标
    function findMinIndex(arr)
    {
        var len = arr.length, min = 999999, index = -1;
        for(var i = 0; i < len; i++)
        {
            if(min > arr[i])
            {
                min = arr[i];
                index = i;
            }
        }
        return index;
	}
    //放置item
    function putItem()
    {
        var items_height = [];  //每个item的高度
        var len = items.length;  //获取item的个数
        for(var i = 0; i < len; i++)
        {
            var item_height1 = items[i].offsetHeight;  //获取每个item的高度	
            console.log(item_height1);
            var item_height = parseInt(items.eq(i).css("height"));
            console.log(item_height);
            // 放置在第一行的item
            if(i < n) 
            {
                items_height[i] = item_height;  //高度数组更新
                items.eq(i).css("top", 0);
                items.eq(i).css("left", i * item_width);

            } 
            else
            {  //放置在其他行的item
                var final_row_fir = parseInt(len / n) * n; //最后一行第一个item的下标
                //处于最后一行
		        if(final_row_fir <= i)
		        {
		                var index = i - final_row_fir;  //该item所应该放置的列
		                items.eq(i).css("top", items_height[index] + margin);
		                items.eq(i).css("left", index * item_width);
		                items_height[index] += item_height + margin;
		        } 
		        else 
		        {
	                var min_index = findMinIndex(items_height);  //寻找最小高度
	                if(min_index == -1) 
	                {
	                    console.log("高度计算出现错误");
	                    return ;
		        	}
	                items.eq(i).css("top", items_height[min_index] + margin);
	                items.eq(i).css("left", min_index * item_width);
	                items_height[min_index] += item_height + margin;  //高度数组更新
               }
            }
        }
    }

    putItem();
}
	

function OnAddTask(){
	$('#page').load('form.html');
	$.getScript("static/js/stitch_form.js", function(){});
}
function OnAddCamera()
{
	console.log("come here");
	var div = '<div class="layui-form-item" >'+'<div class="layui-inline">'+'<label class="layui-form-label">输入1</label>'+'<div class="layui-input-inline">'+'<input type="text" name="title" lay-verify="required|number" lay-verType="tips" placeholder="相机ID" autocomplete="off" class="layui-input layui-input-id"/>'+'</div>'+'</div>'+'<div class="layui-inline"">'+'<div class="layui-input-inline">'+'<input type="text" name="title" lay-verify="required|number" lay-verType="tips" placeholder="相机RTSP地址" autocomplete="off" class="layui-input layui-input-url"/>'+'</div>'+'</div>'+'</div>';
    $('#input_content2').append(div);
}

function reanderLayui()
{
	var $panoa = $('#pano-add').parent();
	
	//JavaScript代码区域
	layui.use('jquery', function(){
		var $ = layui.jquery;
  		var submit = function(){return false;};
		$('#test').on('submit', function(){return false;});
  		$('#test').on('submit', function(){return true;});
	});

	layui.use('element', function() {
		var element = layui.element;
	});

	layui.use('carousel', function(){
  		carousel = layui.carousel;
  		carousel.set({
  			elem: '#stichparam'
  			,width: $('#body_main_content')[0].offsetWidth+'px'
			,height: $('#body_main_content')[0].offsetHeight+'px'
			,arrow: 'hover'
    		,autoplay: false
    		,trigger: 'hover'
  		});
  		carousel.render({elem: '#stichparam',index:0});  
		$panoa.on("click","div",function(event){carousel.render({elem: '#stichparam',index:1});});
		$('.stichitem #modify').on("click",function(event){carousel.render({elem: '#stichparam',index:1});});
	});

	layui.use('layer', function(){
		//独立版的layer无需执行这一句
		var $ = layui.jquery, layer = layui.layer; //独立版的layer无需执行这一句
	  //触发事件
		var active = {
	    	setTop: function(){
	    		var that = this; 
	        	//多窗口模式，层叠置顶
	        	layer.open({
		        	type: 2 //此处以iframe举例
		        	,title: '当你选择该窗体时，即会在最顶端'
		        	,area: ['390px', '260px']
		        	,shade: 0
		        	,maxmin: true
		        	,offset: [ //为了演示，随机坐标
		          		Math.random()*($(window).height()-300)
		          		,Math.random()*($(window).width()-390)
		        	] 
		        	,content: '//layer.layui.com/test/settop.html'
		        	,btn: ['继续弹出', '全部关闭'] //只是为了演示
		        	,yes: function(){$(that).click();}
		        	,btn2: function(){layer.closeAll();}
		        	,zIndex: layer.zIndex //重点1
		        	,success: function(layero){	layer.setTop(layero);} //重点2    
		        	});
	    	}
	    };
	  	$('.layui-card-body .layui-btn').on('click', function(){
    		var othis = $(this), method = othis.data('method');
    		active[method] ? active[method].call(this, othis) : '';
			});
	});
	
}

$(document).ready(function(){
	
	reanderLayui();
	window.setTimeout(function(){itemWaterfull()},100);
	window.onresize = function(e){
		layui.carousel.render({
			elem: '#stichparam'
			,width: $('#body_main_content')[0].offsetWidth+'px'
			,height: $('#body_main_content')[0].offsetHeight+'px'})
		itemWaterfull();
	}
	
	$('#page').load('form.html');
	$.getScript("static/js/stitch_form.js", function(){});
	
	var rgb = $(".layui-badge-dot").css("background-color");
	$(".stichitem .layui-nav-bar").css("background-color",rgb);
	  	
	$('#layui-card-header-name').hover(function () {
		var value = $(this).text();
		$(this).attr('title',value);
	});
	$('.stichitem #delete').click(function(){
		
	})
	$('.stichitem #play').click(function(){
		
	})
		
});

	