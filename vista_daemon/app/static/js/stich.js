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
	
window.onresize = function() {
	itemWaterfull();
}; //在窗口大小改变后，item重新放置


$(document).ready(function(){
	
//	creatpano({text:'沁园春-长沙'});
//	creatpano({text:'沁园春-望西楼'});
	
	window.setTimeout(function(){itemWaterfull()},100);
	
	//内容赋值title
	$('#layui-card-header-name').hover(function () {
	    var value = $(this).text();
	    $(this).attr('title',value);
	});
	
	$('#page').load('form.html');
	$.getScript("static/js/stitch_form.js", function(){});


	
//	$(".layui-nav-item").on("click", "a", function(event){
//	console.log("点击事件处理");
//	if ($(this).hasClass("stich")) 
//		{ 
//			console.log($(this).attr('class'));
//			$('#page').load('form.html');
//		    $.getScript("js/stitch_form.js", function(){
////		     		addData(layui);
//		     	});
//        }
//		else
//		{
//			console.log($(this).attr('class'));
//		}
//
//	});
		
		
});

	