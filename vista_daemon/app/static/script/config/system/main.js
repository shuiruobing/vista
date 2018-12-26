var UserName = "";
var Password = "";
var Token = "";
var TasksIntervalTimer = -1;
var SelectedTaskIndex = -1;
var selectedChannelID = "";
var ComputerIntervalTimer = -1;
$(function() {
    $(document).ready(function() {
        UserName = getCookie('UserName');
        Password = getCookie('Password');
        Token = getCookie('Token');
        if (UserName == null || Password == null || UserName == "" || Password == "") {
            window.location.href = 'index.html';
        }
        $(".header-r .user label").html(UserName);
        $(".header-r .exit").click(function() {
            window.location.href = 'index.html';
        });

        $(".menu-1 .menu-2").each(function() {
            var $this = $(this);
            if ($this.attr("name") == "TasksManager") {
                $this.addClass("menu-hover menu-select");
                $this.siblings().removeClass("menu-hover menu-select");
            }
        });
        $(".layout-center .layout-center-inner .view .tabs-1 ul li").each(function() {
            var $this = $(this);
            if ($this.attr("module") == "TasksManager")
                $this.find("a").click();
        });
        TasksIntervalTimer = window.setInterval(TasksKeepAlive, 1000);
        ComputerIntervalTimer = window.setInterval(GetComputerInfo, 3000);
    });
    $(".menu-title-1").click(function() {
        var $this = $(this);
        var parentNode = $(this).parent();
        var parentClassName = parentNode.attr("class");
        if (parentClassName.indexOf("collapsed") != -1) {
            parentNode.removeClass("collapsed");
            parentNode.siblings().attr("class", "ng-scope menu menu-1 collapsed");
            parentNode.children(":first").attr("class", "menu-title menu-title-1");
            parentNode.children(":first").siblings("class", "menu menu-2");
            $this.siblings().attr("class", "menu menu-2");
            $this.next().addClass("collapsed menu-hover menu-select");
            var name = $this.next().attr("name");
            var aa = $(".layout-center .layout-center-inner .view #tabs ul").find('li[module="' + name + '"]');
            $(".layout-center .layout-center-inner .view #tabs ul").find('li[module="' + name + '"]').find("a").click();
        } else {
            $this.parent().addClass("collapsed");
            $this.attr("class", "menu-title menu-title-1");
            $this.siblings().attr("class", "menu menu-2");
            $this.next().addClass("collapsed menu-hover menu-select");
            var name = $this.next().attr("name");
            var aa = $(".layout-center .layout-center-inner .view #tabs ul").find('li[module="' + name + '"]');
            $(".layout-center .layout-center-inner .view #tabs ul").find('li[module="' + name + '"]').find("a").click();
        }



    });
    $(".menu-2").click(function() {
        var $this = $(this);
        $this.addClass("menu-hover menu-select");
        $this.siblings().removeClass("menu-hover menu-select");
        var name = $this.attr("name");
        $(".layout-center .layout-center-inner .view .tabs-1 ul li").each(function() {
            var $this = $(this);
            if ($this.attr("module") == name)
                $this.find("a").click();
        });
    });
});
var GetBaseInfos = function(obj) {
    $(".layout-center .layout-center-inner .view #tabs ul[id!='setting']").css("display", "none");
    $(".layout-center .layout-center-inner .view #tabs ul[id='setting']").css("display", "");
    $(".layout-center .layout-center-inner .view #tabs div[id='ui-tabs-1']").css("display", "");
    $(".layout-center .layout-center-inner .view #tabs div[id='ui-tabs-45']").css("display", "none");
    $(".layout-center .layout-center-inner .view #tabs div[id='ui-tabs-46']").css("display", "none");
    GetVesrsion();
    GetMachineCode();
}

function GetVesrsion() {
    $.ajax({
        type: 'get',
        url: WebVersionUrl + "?Token=" + Token,
        dataType: 'json',
        ContentType: "application/x-www-form-urlencoded",
        cache: false,
        success: function(msg) {
            //跳转地图页面
            if (msg) {
                if (msg.Result != 0) {
                    alert(msg.Message);
                    if (msg.Result == 105)
                        window.location.href = 'index.html';
                } else {

                    $("#setting .items .item span input[id='webVersion']").val(msg.Version);
                }
            }

        },
        error: function(err) {
            if (err.responseText != null) {
                $("#errotip").text(err.responseText).show();

            } else {
                $("#errotip").text("获取用户信息失败").show();
            }

        }
    });
}

function GetMachineCode() {
    $("#setting .items .item span input[id='MachineCode']").parent().parent().css("display", "none");
    $("#setting .items .item span input[id='RegisterCode']").parent().parent().css("display", "none");
    $.ajax({
        type: 'get',
        url: MachineCodeUrl + "?Token=" + Token,
        dataType: 'json',
        ContentType: "application/x-www-form-urlencoded",
        cache: false,
        success: function(msg) {
            //跳转地图页面
            if (msg) {
                if (msg.Result != 0) {

                    if (msg.Result == 105)
                        window.location.href = 'index.html';
                    if (msg.Result == 318) {
                        $("#setting .items .item span input[id='MachineCode']").parent().parent().css("display", "none");
                        $("#setting .items .item span input[id='RegisterCode']").parent().parent().css("display", "none");
                        GetRegisterStatus();
                    }
                    //alert(msg.Message);
                } else {
                    $("#setting .items .item span input[id='MachineCode']").parent().parent().css("display", "");
                    $("#setting .items .item span input[id='MachineCode']").val(msg.MachineCode);
                    $("#setting .items .item span input[id='RegisterCode']").parent().parent().css("display", "");
                }
            }
        },
        error: function(err) {
            if (err.responseText != null) {
                $("#errotip").text(err.responseText).show();

            } else {
                $("#errotip").text("获取用户信息失败").show();
            }
        }
    });
}

function Registe() {
    var RegisterCode = $("#setting .items .item span input[id='RegisterCode']").val();
    if (!RegisterCode) {
        alert("请输入注册码");
        return;
    }
    if (RegisterCode == "") {
        alert("请输入注册码");
        return;
    }
    $.ajax({
        type: 'get',
        url: RegisteUrl + "?Token=" + Token + "&RegisterCode=" + RegisterCode,
        dataType: 'json',
        ContentType: "application/x-www-form-urlencoded",
        cache: false,
        success: function(msg) {
            //跳转地图页面
            if (msg) {
                if (msg.Result != 0) {
                    alert(msg.Message);
                } else {
                    if (msg.RegistrationStatus.status == "registered") {
                        alert("注册成功");
                        $("#setting .items .item span input[id='RegisterCode']").parent().parent().css("display", "none");
                        $("#setting .items .item span input[id='MachineCode']").parent().parent().css("display", "none");
                        $("#setting .items .item span input[id='RegisterStatus']").parent().parent().css("display", "");
                        $("#setting .items .item span input[id='RegisterStatus']").val(msg.RegistrationStatus.status);
                        $("#setting .items .item span input[id='Duetime']").parent().parent().css("display", "");
                        $("#setting .items .item span input[id='Duetime']").val(msg.RegistrationStatus.ExpireTime);
                    } else
                        alert("注册失败");

                }
            }
        },
        error: function(err) {
            if (err.responseText != null) {
                $("#errotip").text(err.responseText).show();

            } else {
                $("#errotip").text("获取用户信息失败").show();
            }
        }
    });
}

function GetRegisterStatus() {
    $.ajax({
        type: 'get',
        url: RegisterStatusUrl + "?Token=" + Token,
        dataType: 'json',
        ContentType: "application/x-www-form-urlencoded",
        cache: false,
        success: function(msg) {
            //跳转地图页面
            if (msg) {
                if (msg.Result != 0) {
                    if (msg.Result == 105)
                        window.location.href = 'index.html';
                    if (msg.Result == 321) {
                        $("#setting .items .item span input[id='RegisterStatus']").parent().parent().css("display", "none");
                        $("#setting .items .item span input[id='Duetime']").parent().parent().css("display", "none");
                        alert(msg.Message);

                    }
                    //alert(msg.Message);
                } else {
                    $("#setting .items .item span input[id='RegisterStatus']").parent().parent().css("display", "");
                    $("#setting .items .item span input[id='RegisterStatus']").val(msg.RegistrationStatus.status);
                    $("#setting .items .item span input[id='Duetime']").parent().parent().css("display", "");
                    $("#setting .items .item span input[id='Duetime']").val(msg.RegistrationStatus.ExpireTime);
                }
            }
        },
        error: function(err) {
            if (err.responseText != null) {
                $("#errotip").text(err.responseText).show();

            } else {
                $("#errotip").text("获取用户信息失败").show();
            }
        }
    });
}

function GetAllTasks(obj) {
    var taskManagerDiv = $("div .items #TasksManagerDiv");
    $(".layout-center .layout-center-inner .view #tabs ul[id!='operation']").css("display", "none");
    $(".layout-center .layout-center-inner .view #tabs ul[id='operation']").css("display", "");
    if (obj != null) {
        var id = $(obj).parent().attr("aria-controls");
        $(obj).parent().attr("class", "ui-state-default ui-corner-top ui-state-hover ui-tabs-active ui-state-active");
        $(obj).parent().siblings("li").attr("class", "ng-scope ui-state-default ui-corner-top");
        var module = $(obj).parent().attr("module");
        $(".menu-1 .menu-2").each(function() {
            var $this = $(this);
            if ($this.attr("name") == module) {
                $this.addClass("menu-hover menu-select");
                $this.siblings().removeClass("menu-hover menu-select");
            }
        });
        var taskDiv = $("#" + id);
        taskDiv.css("display", "block");
        taskDiv.siblings("div").css("display", "none");
    }
    TasksInfo = HttpTasks();
    CreatTasksListDiv(TasksInfo);

}

function GetTaskInfo(obj) {
    $(".layout-center .layout-center-inner .view #tabs ul[id!='operation']").css("display", "none");
    $(".layout-center .layout-center-inner .view #tabs #operation").css("display", "");
    if (obj != null) {
        var id = $(obj).parent().attr("aria-controls");
        $(obj).parent().attr("class", "ui-state-default ui-corner-top ui-state-hover ui-tabs-active ui-state-active");
        $(obj).parent().siblings("li").attr("class", "ng-scope ui-state-default ui-corner-top");
        var module = $(obj).parent().attr("module");
        $(".menu-1 .menu-2").each(function() {
            var $this = $(this);
            if ($this.attr("name") == module) {
                $this.addClass("menu-hover menu-select");
                $this.siblings().removeClass("menu-hover menu-select");
            }
        });
        var tableDiv = $("#" + id);
        tableDiv.css("display", "block");
        tableDiv.siblings("div").css("display", "none");
    }
    TaskEdit();
}

function TasksKeepAlive() {
    HttpTasks();
    var TasksManagerDiv = $("#TasksManager .items #TasksManagerDiv");
    UpdateTasksListDiv(TasksManagerDiv);
}

function GetComputerInfo() {
    $.ajax({
        type: 'Get',
        url: ComputerUrl,
        dataType: 'json',
        async: false,
        cache: false,
        success: function(msg) {
            //跳转地图页面
            if (msg.Result) {
                alert(msg.Message);
                if (msg.Result == 105)
                    window.location.href = 'index.html';
            } else {
                ComputerInfo = msg;

                SetComputerInfo();
            }
        },
        error: function(err) {
            if (err.responseText != null) {
                $("#errotip").text(err.responseText).show();

            } else {
                $("#errotip").text("获取用户信息失败").show();
            }

        }
    });
}

function SetComputerInfo() {
    if (ComputerInfo != null) {
        var os = $("#SystemInfoDiv #SystemInfo li #OS_Version");
        $("#SystemInfoDiv #SystemInfo li #OS_Version").html(ComputerInfo.OS_Version);
        $("#SystemInfoDiv #SystemInfo li #DiskSize").html(ComputerInfo.DiskSize);
        $("#SystemInfoDiv #SystemInfo li #PhisicalMemory").html(ComputerInfo.PhisicalMemory);
        $("#SystemInfoDiv #SystemInfo li #MemoryAvailable").html(ComputerInfo.MemoryAvailable);
        $("#SystemInfoDiv #SystemInfo li #CPULoad").html(ComputerInfo.CPULoad + "%");
        var gpuLi = $("#SystemInfoDiv #SystemInfo .GPU");
        var GPU = ComputerInfo.GPU;
        if (gpuLi.length == 0) {
            $.each(GPU, function(index, value) {
                var li = $("<li class='GPU' id=" + index + "></li>");
                var span = $("<span></span>");
                span.html("GPU " + index + ":");
                var ul1 = $("<ul style='margin-left:10%'></ul>");
                $.each(value, function(ind, val) {
                    var li1 = $("<li style='margin-left:10%'></li>");
                    var span1 = $("<span></span>");
                    span1.html(ind + ":");
                    var span2 = $("<span id=" + ind + "></span>");
                    span2.html(val + "%");
                    li1.append(span1, span2);
                    ul1.append(li1);
                });
                li.append(span, ul1);
                $("#SystemInfoDiv #SystemInfo").append(li);
            });
        } else {
            $.each(GPU, function(index, value) {
                var li = $("#SystemInfoDiv #SystemInfo #" + index);

                $.each(value, function(ind, val) {
                    li.find("ul li #" + ind).html(val + "%");
                });
            });
        }
    }

}

function HttpTasks() {
    var AcessTasksParam = {
        Token: Token
    };
    $.ajax({
        type: 'Get',
        url: AllTasksInfo,
        data: AcessTasksParam,
        dataType: 'json',
        async: false,
        cache: false,
        success: function(msg) {
            //跳转地图页面
            if (msg.Result) {
                alert(msg.Message);
                if (msg.Result == 105)
                    window.location.href = 'index.html';
            } else {
                TasksInfo = msg;
            }
        },
        error: function(err) {
            if (err.responseText != null) {
                $("#errotip").text(err.responseText).show();

            } else {
                $("#errotip").text("获取用户信息失败").show();
            }

        }
    });
    return TasksInfo;
}

function CreatTasksListDiv(TasksInfo) {
    var TasksManagerDiv = $("#TasksManager .items #TasksManagerDiv");
    TasksManagerDiv.html("");
    var ul = TasksManagerDiv.find("ul");
    ul = $("<ul style='width:100%'></ul>");
    var li = $("<li style='width:100%;height:auto'></li>");
    ul.append(li);
    $.each(TasksInfo, function(index, value) {
        var taskDiv = $("<div id=" + value.ChannelID + "></div>");
        taskDiv.addClass("Task");
        var taskDiv1 = $("<div id='ChannelID' style='position:relative;width:100%;'></div>");
        var span11 = $("<span>ChannelID:</span>");
        var span12 = $("<span>" + value.ChannelID + "</span>");
        taskDiv1.append(span11, span12);
        var taskDiv2 = $("<div id='ChannelName' style='position:relative;width:100%;'></div>");
        var span21 = $("<span>ChannelName:</span>");
        var span22 = $("<span>" + value.ChannelName + "</span>");
        taskDiv2.append(span21, span22);
        var taskDiv3 = $("<div id='Multicast' style='position:relative;width:100%;'></div>");
        var span31 = $("<span>Multicast:</span>");
        var span32 = $("<span>" + value.Multicast + "</span>");
        taskDiv3.append(span31, span32);
        var taskDiv4 = $("<div id='Status' style='position:relative;width:100%;'></div>");
        var span41 = $("<span>Status:</span>");
        var span42 = $("<span>" + value.Status + "</span>");
        taskDiv4.append(span41, span42);
        taskDiv.append(taskDiv1, taskDiv2, taskDiv3, taskDiv4);
        li.append(taskDiv);
        taskDiv.click((function(arg) {
            return function() {
                $(this).addClass("TaskSelected");
                $(this).siblings().removeClass("TaskSelected");
                SelectedTaskIndex = index;
                selectedChannelID = TasksInfo[SelectedTaskIndex].ChannelID;
            };
        })(index));
        if (value.ChannelID == selectedChannelID)
            taskDiv.addClass("TaskSelected");
    });
    TasksManagerDiv.append(ul);
}

function UpdateTasksListDiv(TasksManagerDiv) {
    if (TasksInfo == null)
        return;
    $.each(TasksInfo, function(index, value) {
        var channelID = value.ChannelID;
        var taskDiv = TasksManagerDiv.find("ul li #" + channelID);
        if (taskDiv) {
            if (taskDiv.length == 0) {
                taskDiv = AddTaskDiv(TasksManagerDiv, value);
            } else {
                UpdateTaskDiv(taskDiv, value);
                if (taskDiv.hasClass("TaskSelected"))
                    UpdateTaskInfo(value);
            }
        } else {
            taskDiv = AddTaskDiv(TasksManagerDiv, value);
        }
        taskDiv.click((function(arg) {
            return function() {
                $(this).addClass("TaskSelected");
                $(this).siblings().removeClass("TaskSelected");
                SelectedTaskIndex = index;
                selectedChannelID = TasksInfo[SelectedTaskIndex].ChannelID;
            };
        })(index));
    });
    /*for(var i=0;i<TasksInfo.length;i++)
    {
    	var channelID=TasksInfo[i].ChannelID;
    	var taskDiv=TasksManagerDiv.find("ul li #"+channelID);
    	if(taskDiv)
    	{
    		if(taskDiv.length==0)
    		{
    		taskDiv=AddTaskDiv(TasksManagerDiv,TasksInfo[i]);			
    		}
    		else
    		{
    			UpdateTaskDiv(taskDiv,TasksInfo[i]);
    			if(taskDiv.hasClass("TaskSelected"))
    				UpdateTaskInfo(TasksInfo[i]);
    		}
    	}
    	else
    	{
    		taskDiv=AddTaskDiv(TasksManagerDiv,TasksInfo[i]);			
    	}
    	taskDiv.click((function(arg){				
    		return function(){
    			$(this).addClass("TaskSelected");	
    			$(this).siblings().removeClass("TaskSelected");	
    			SelectedTaskIndex=i;
    			selectedChannelID=TasksInfo[SelectedTaskIndex].ChannelID;				
    		};
    		})(i));			
    }*/
}

function TaskInfoRefresh() {
    var taskInfo = TasksInfo[SelectedTaskIndex];
    UpdateTaskInfo(taskInfo);
}

function UpdateTaskInfo(TaskInfo) {
    var taskDetailInfoItems = $(".layout-center .layout-center-inner .view #tabs").find("div").find("#TaskInfo #setting .items");
    if (!taskDetailInfoItems)
        return;
    if (taskDetailInfoItems.length == 0)
        return;
    taskDetailInfoItems.find("#ChannelID span input").val(TaskInfo.ChannelID);
    taskDetailInfoItems.find("#ChannelName span input").val(TaskInfo.ChannelName);
    taskDetailInfoItems.find("#Multicast span input").val(TaskInfo.Multicast);
    var StitchParam = JSON.parse(TaskInfo.StitchParam);
    taskDetailInfoItems.find("#StitchParam #Hardware span input").val(StitchParam.Hardware);
    taskDetailInfoItems.find("#StitchParam #HardwareNo span input").val(StitchParam.HardwareNo);
    var Devices = StitchParam.Input.Devices;
    var m = -1;
    for (var i = 0; i < Devices.length; i++) {
        var Device = Devices[i];
        $.each(Device, function(index, value) {
            m = m + 1;
            taskDetailInfoItems.find("#StitchParam #Input #Devices div").eq(m).find("span input").val(value);
        });
    }
    taskDetailInfoItems.find("#StitchParam #Input #Hardware span input").val(StitchParam.Input.Hardware);
    taskDetailInfoItems.find("#StitchParam #Input #MaxFrameCached span input").val(StitchParam.Input.MaxFrameCached);
    taskDetailInfoItems.find("#StitchParam #Input #MaxGopCached span input").val(StitchParam.Input.MaxGopCached);
    taskDetailInfoItems.find("#StitchParam #Output #Encode #Bitrate span input").val(StitchParam.Output.Encode.Bitrate);
    taskDetailInfoItems.find("#StitchParam #Output #Encode #CodecId span input").val(StitchParam.Output.Encode.CodecId);
    taskDetailInfoItems.find("#StitchParam #Output #Encode #GOP span input").val(StitchParam.Output.Encode.GOP);
    taskDetailInfoItems.find("#StitchParam #Output #Encode #Profile span input").val(StitchParam.Output.Encode.Profile);
    taskDetailInfoItems.find("#StitchParam #Output #Encode #QP span input").val(StitchParam.Output.Encode.QP);
    taskDetailInfoItems.find("#StitchParam #Output #Encode #hardware span input").val(StitchParam.Output.Encode.hardware);
    taskDetailInfoItems.find("#StitchParam #Output #Multicast  span input").val(StitchParam.Output.Multicast);
    taskDetailInfoItems.find("#StitchParam #Stitch #Mode  span input").val(StitchParam.Stitch.Mode);
    taskDetailInfoItems.find("#StitchPID span input").val(TaskInfo.StitchPID);
    taskDetailInfoItems.find("#StitchPort span input").val(TaskInfo.StitchPort);
    taskDetailInfoItems.find("#RtspPID span input").val(TaskInfo.RtspPID);
    taskDetailInfoItems.find("#RtspPort span input").val(TaskInfo.RtspPort);
    taskDetailInfoItems.find("#Status span input").val(TaskInfo.Status);
    taskDetailInfoItems.find("#RtspParamOut span input").val(TaskInfo.RtspParamOut);
    taskDetailInfoItems.find("#RtspParamIn span input").val(TaskInfo.RtspParamIn);
    taskDetailInfoItems.find("#RtspWorkDir span input").val(TaskInfo.RtspWorkDir);
    taskDetailInfoItems.find("#StitchWorkDir span input").val(TaskInfo.StitchWorkDir);
    taskDetailInfoItems.show();
}

function UpdateTaskDiv(TaskDiv, taskInfo) {
    if (taskInfo == null)
        return;
    var channelID = taskInfo.ChannelID;
    TaskDiv.find("#ChannelID").find("span").eq(1).html(channelID);
    var channelName = taskInfo.ChannelName;
    TaskDiv.find("#ChannelName").find("span").eq(1).html(channelName);
    var multiCast = taskInfo.Multicast;
    TaskDiv.find("#Multicast").find("span").eq(1).html(multiCast);
    var Status = taskInfo.Status;
    TaskDiv.find("#Status").find("span").eq(1).html(Status);
    TaskDiv.show();
}

function AddTaskDiv(TasksManagerDiv, taskInfo) {
    var ul = TasksManagerDiv.find("ul");
    var li;
    if (!ul) {
        ul = $("<ul style='width:100%'></ul>");
        li = $("<li style='width:100%;height:auto'></li>");
        ul.append(li);
    }
    li = ul.find("li");
    var taskDiv = $("<div id=" + taskInfo.ChannelID + "></div>");
    taskDiv.addClass("Task");
    var taskDiv1 = $("<div id='ChannelID' style='position:relative;width:100%;'></div>");
    var span11 = $("<span>ChannelID:</span>");
    var span12 = $("<span>" + taskInfo.ChannelID + "</span>");
    taskDiv1.append(span11, span12);
    var taskDiv2 = $("<div id='ChannelName' style='position:relative;width:100%;'></div>");
    var span21 = $("<span>ChannelName:</span>");
    var span22 = $("<span>" + taskInfo.ChannelName + "</span>");
    taskDiv2.append(span21, span22);
    var taskDiv3 = $("<div id='Multicast' style='position:relative;width:100%;'></div>");
    var span31 = $("<span>Multicast:</span>");
    var span32 = $("<span>" + taskInfo.Multicast + "</span>");
    taskDiv3.append(span31, span32);
    var taskDiv4 = $("<div id='Status' style='position:relative;width:100%;'></div>");
    var span41 = $("<span>Status:</span>");
    var span42 = $("<span>" + taskInfo.Status + "</span>");
    taskDiv4.append(span41, span42);
    taskDiv.append(taskDiv1, taskDiv2, taskDiv3, taskDiv4);
    li.append(taskDiv);

    TasksManagerDiv.show();
    return taskDiv;
}

function TaskStart() {
    var selectdTask = GetSelectedTask();
    if (!selectdTask) {
        alert("请选择任务");
        return;
    }
    if (selectdTask.length == 0) {
        alert("请选择任务");
        return;
    }
    var TaskInfo = TasksInfo[SelectedTaskIndex];
    var channelID = "";
    if (TaskInfo != null)
        channelID = TaskInfo.ChannelID;
    $.ajax({
        url: TaskStartUrl + "?Token=" + Token + "&ChannelID=" + channelID,
        async: false,
        type: "get",
        dataType: 'json',
        contentType: 'application/json',
        //data:JSON.stringify({Token:Token,ChannelID:channelID}),	
        //data:JSON.stringify(changeMessage),
        success: function(data) {
            if (data) {
                if (data.Result == 0)
                    alert("任务启动成功");
                else {
                    alert(data.Message);
                    if (data.Result == 105)
                        window.location.href = 'index.html';

                }
            }
        },
        error: function(err) {
            console.log(err);
        }
    });
}

function TaskRemove() {
    var selectdTask = GetSelectedTask();
    if (!selectdTask) {
        alert("请选择任务");
        return;
    }
    if (selectdTask.length == 0) {
        alert("请选择任务");
        return;
    }
    var TaskInfo = TasksInfo[SelectedTaskIndex];
    var channelID = "";
    if (TaskInfo != null)
        channelID = TaskInfo.ChannelID;
    $.ajax({
        url: TaskRemoveUrl + "?Token=" + Token + "&ChannelID=" + channelID,
        async: false,
        type: "get",
        dataType: 'json',
        contentType: 'application/json',
        success: function(data) {
            if (data) {
                if (data.Result == 0) {
                    alert("任务移除成功");
                    GetAllTasks(null);
                    var taskInfoItemsDiv = $(".layout-center .layout-center-inner .view #tabs div #TaskInfo #setting .items");
                    DrawTaskDetailInfo(taskInfoItemsDiv, null);
                } else {
                    alert(data.Message);
                    if (data.Result == 105)
                        window.location.href = 'index.html';
                }
            }
        },
        error: function(err) {
            console.log(err);
        }
    });
}

function TaskRefresh() {
    GetAllTasks(null);
}

function TaskStop() {
    var selectdTask = GetSelectedTask();
    if (!selectdTask) {
        alert("请选择任务");
        return;
    }
    if (selectdTask.length == 0) {
        alert("请选择任务");
        return;
    }
    var TaskInfo = TasksInfo[SelectedTaskIndex];
    var channelID = "";
    if (TaskInfo != null)
        channelID = TaskInfo.ChannelID;
    $.ajax({
        url: TaskStopUrl + "?Token=" + Token + "&ChannelID=" + channelID,
        async: false,
        type: "get",
        dataType: 'json',
        contentType: 'application/json',
        success: function(data) {
            if (data) {
                if (data.Result == 0)
                    alert("任务停止成功");
                else {
                    alert(data.Message);
                    if (data.Result == 105)
                        window.location.href = 'index.html';
                }
            }
        },
        error: function(err) {
            console.log(err);
        }
    });

}

function GetSelectedTask() {
    var TasksManagerDiv = $("#TasksManager .items #TasksManagerDiv");
    var selectdTask = TasksManagerDiv.find(".TaskSelected");
    return selectdTask;
}

function TaskEdit(obj) {
    var selectdTask = GetSelectedTask();
    if (!selectdTask) {
        alert("请选择任务");
        return;
    }
    if (selectdTask.length == 0) {
        alert("请选择任务");
        return;
    }
    if (obj != null) {
        $("#ui-tabs-45").css("display", "none");
    }
    var TaskInfo = TasksInfo[SelectedTaskIndex];
    $(".layout-center .layout-center-inner .view #tabs #operation  #TaskInfo").attr("class", "ui-state-default ui-corner-top ui-state-hover ui-tabs-active ui-state-active");
    $(".layout-center .layout-center-inner .view #tabs #operation  #TaskInfo").siblings().attr("class", "ng-scope ui-state-default ui-corner-top");
    $(".layout-center .layout-center-inner .view #tabs #operation").css("display", "");
    var taskInfoDiv = $(".layout-center .layout-center-inner .view #tabs").find("div").find("#TaskInfo");
    taskInfoDiv.parent().css("display", "");
    //taskInfoDiv.parent().siblings().css("display","none");
    $(".layout-west .sdmenu div[name='operation'] div[name='TaskInfo']").addClass("menu-hover menu-select");
    $(".layout-west .sdmenu div[name='operation'] div[name='TaskInfo']").siblings().removeClass("menu-hover menu-select");
    taskInfoItems = taskInfoDiv.find("#setting .items");
    DrawTaskDetailInfo(taskInfoItems, TaskInfo);
}

function DrawTaskDetailInfo(taskInfoItemsDiv, TaskDetailInfo) {
    taskInfoItemsDiv.html("");
    var n = 0;
    var m = 0;
    if (!TaskDetailInfo)
        return;
    $.each(TaskDetailInfo, function(key, value) {
        n++;
        if (key == "StitchParam") {
            var item = $("<div id=" + key + "></div>");
            item.addClass("item");
            item.css("marginBottom", "0.5%");
            item.css("height", "auto");
            var spanParamName = $("<span>" + TaskParamChinaName[key][key] + "</span>");
            spanParamName.addClass('first ng-binding');
            item.append(spanParamName);
            var stitchParams = JSON.parse(value);
            $.each(stitchParams, function(k, val) {
                m++;
                if (k == "Input") {
                    var item1 = $("<div id=" + k + "></div>");
                    item1.addClass("item");
                    item1.css("marginBottom", "0.5%");
                    item1.css("height", "auto");
                    item1.css("marginLeft", "100px");
                    var spanParamName = $("<span>" + TaskParamChinaName[key][k][k] + "</span>");
                    spanParamName.addClass('first ng-binding');
                    item1.append(spanParamName);
                    $.each(val, function(inputKey, inputVal) {
                        if (inputKey == "Devices") {
                            var item2 = $("<div id=" + inputKey + "></div>");
                            item2.addClass("item");
                            item2.css("marginBottom", "0.5%");
                            item2.css("height", "auto");
                            item2.css("marginLeft", "100px");
                            var spanParamName = $("<span>" + TaskParamChinaName[key][k][inputKey][inputKey] + "</span>");
                            spanParamName.addClass('first ng-binding');
                            //var br=$("<br></br>");
                            item2.append(spanParamName);
                            var f = 0;
                            for (var i = 0; i < inputVal.length; i++) {

                                $.each(inputVal[i], function(cameraIndex, cameraVal) {
                                    f++;
                                    var item3 = $("<div id=" + cameraIndex + "></div>");
                                    item3.addClass("item");
                                    item3.css("marginBottom", "0.5%");
                                    item3.css("height", "auto");
                                    item3.css("width", "100%");
                                    item3.css("marginLeft", "100px");
                                    var spanParamName = $("<span>" + TaskParamChinaName[key][k][inputKey][cameraIndex] + "</span>");
                                    spanParamName.addClass('first ng-binding');
                                    var spandParamValue = $("<span><input id='" + key + "_" + k + "_" + inputKey + "_" + i + "' type='text' class='text-long ng-pristine ng-valid' disabled='disabled'>" + "</input></span>");
                                    spandParamValue.find("input").val(cameraVal);
                                    item3.append(spanParamName, spandParamValue);
                                    item2.append(item3);
                                });

                            }
                        } else {
                            var item2 = $("<div id=" + inputKey + "></div>");
                            item2.addClass("item");
                            item2.css("marginBottom", "0.5%");
                            item2.css("height", "auto");
                            item2.css("width", "100%");
                            item2.css("marginLeft", "100px");
                            var spanParamName = $("<span>" + TaskParamChinaName[key][k][inputKey] + "</span>");
                            spanParamName.addClass('first ng-binding');
                            var spandParamValue;
                            if (inputKey == "HardwareNo") {
                                /*spandParamValue=$("<span><select id='"+key+"_"+k+"_"+inputKey+"' class='text-long ng-pristine ng-valid'><option>-1</option><option>0</option><option>1</option><option>2</option><option>3</option>"+"</select></span>");*/
                                spandParamValue = $("<span><input id='" + key + "_" + k + "_" + inputKey + "' type='text' class='text-long ng-pristine ng-valid' disabled='disabled'>" + "</input></span>");
                            } else
                                spandParamValue = $("<span><input id='" + key + "_" + k + "_" + inputKey + "' type='text' class='text-long ng-pristine ng-valid' disabled='disabled'>" + "</input></span>");
                            spandParamValue.find("input").val(inputVal);
                            item2.append(spanParamName, spandParamValue);
                        }
                        item1.append(item2);

                    });
                    item.append(item1);
                } else if (k == "Output" || k == "Stitch") {
                    var item1 = $("<div id=" + k + "></div>");
                    item1.addClass("item");
                    item1.css("marginBottom", "0.5%");
                    item1.css("height", "auto");
                    item1.css("marginLeft", "100px");
                    var spanParamName = $("<span>" + TaskParamChinaName[key][k][k] + "</span>");
                    spanParamName.addClass('first ng-binding');
                    item1.append(spanParamName);
                    var v = 0;
                    $.each(val, function(outputKey, outputVal) {
                        v++;
                        if (outputKey == "Encode") {
                            var item2 = $("<div id=" + outputKey + "></div>");
                            item2.addClass("item");
                            item2.css("marginBottom", "0.5%");
                            item2.css("height", "auto");
                            item2.css("marginLeft", "100px");
                            var spanParamName = $("<span>" + TaskParamChinaName[key][k][outputKey][outputKey] + "</span>");
                            spanParamName.addClass('first ng-binding');
                            //var br=$("<br></br>");
                            item2.append(spanParamName);
                            var b = 0;
                            $.each(outputVal, function(encodeKey, encodeVal) {
                                b++;
                                var item3 = $("<div id=" + encodeKey + "></div>");
                                item3.addClass("item");
                                item3.css("marginBottom", "0.5%");
                                item3.css("height", "auto");
                                item3.css("width", "100%");
                                item3.css("marginLeft", "100px");
                                var spanParamName = $("<span>" + TaskParamChinaName[key][k][outputKey][encodeKey] + "</span>");
                                spanParamName.addClass('first ng-binding');
                                var spandParamValue = $("<span><input id='" + key + "_" + k + "_" + outputKey + "_" + encodeKey + "' type='text' class='text-long ng-pristine ng-valid' disabled='disabled'>" + "</input></span>");
                                spandParamValue.find("input").val(encodeVal);
                                item3.append(spanParamName, spandParamValue);
                                item2.append(item3);
                            });
                            item1.append(item2);
                        } else {
                            if (outputKey != "Unicast") {
                                var item2 = $("<div id=" + outputKey + "></div>");
                                item2.addClass("item");
                                item2.css("marginBottom", "0.5%");
                                item2.css("height", "auto");
                                if (v == 1)
                                    item2.css("marginLeft", "0px");
                                else
                                    item2.css("marginLeft", "100px");
                                var spanParamName = $("<span>" + TaskParamChinaName[key][k][outputKey] + "</span>");
                                spanParamName.addClass('first ng-binding');
                                var spandParamValue = $("<span><input id='" + key + "_" + k + "_" + outputKey + "' type='text' class='text-long ng-pristine ng-valid' disabled='disabled'>" + "</input></span>");
                                spandParamValue.find("input").val(outputVal);
                                item2.append(spanParamName, spandParamValue);
                                item1.append(item2);
                            }

                        }
                    });
                    item.append(item1);
                } else {
                    var item1 = $("<div id=" + k + "></div>");
                    item1.addClass("item");
                    item1.css("marginBottom", "0.5%");
                    item1.css("height", "auto");
                    item1.css("width", "100%");
                    item1.css("marginLeft", "100px");
                    var spanParamName = $("<span>" + TaskParamChinaName[key][k] + "</span>");
                    spanParamName.addClass('first ng-binding');
                    var spandParamValue;
                    if (k == "Hardware")
                        spandParamValue = $("<span><input id='" + key + "_" + k + "' type='text' class='text-long ng-pristine ng-valid' disabled='disabled'>" + "</input></span>");
                    else if (k == "HardwareNo") {
                        /*spandParamValue=$("<span><select id='"+key+"_"+k+"' class='text-long ng-pristine ng-valid'><option>-1</option><option>0</option><option>1</option><option>2</option><option>3</option>"+"</select></span>");*/
                        spandParamValue = $("<span><input id='" + key + "_" + k + "' type='text' class='text-long ng-pristine ng-valid' disabled='disabled'>" + "</input></span>");
                    } else
                        spandParamValue = $("<span><input id='" + key + "_" + k + "' type='text' class='text-long ng-pristine ng-valid' disabled='disabled'>" + "</input></span>");
                    spandParamValue.find("input").val(val);
                    item1.append(spanParamName, spandParamValue);
                    item.append(item1);
                }
            });
            taskInfoItemsDiv.append(item);
        } else {
            if (key != "ID") {
                var item = $("<div id=" + key + "></div>");
                item.addClass("item");
                item.css("marginBottom", "0.5%");
                item.css("height", "auto");
                item.css("width", "100%");
                var spanParamName;
                spanParamName = $("<span>" + TaskParamChinaName[key] + "</span>");
                spanParamName.addClass('first ng-binding');
                var spandParamValue;
                if (key == "StitchWorkDir" || key == "RtspWorkDir")
                    spandParamValue = $("<span><input id='" + key + "' type='text' class='text-long2 ng-pristine ng-valid' disabled='disabled'>" + "</input></span>");
                else if (key == "ChannelID" || key == "StitchPID" || key == "RtspPID" || key == "Status")
                    spandParamValue = $("<span><input id='" + key + "' type='text' class='text-long ng-pristine ng-valid' disabled='disabled'>" + "</input></span>");
                else if (key == "RtspParamOut" || key == "RtspParamIn")
                    spandParamValue = $("<span><input id='" + key + "' type='text' class='text-long2 ng-pristine ng-valid' disabled='disabled'>" + "</input></span>");
                else
                    spandParamValue = $("<span><input id='" + key + "' type='text' class='text-long ng-pristine ng-valid' disabled='disabled'>" + "</input></span>");
                spandParamValue.find("input").val(value);
                item.append(spanParamName, spandParamValue);
                taskInfoItemsDiv.append(item);
            }
        }
    });
}

function TaskInfoSave(taskInfoItemsDiv) {
    if (!taskInfoItemsDiv)
        taskInfoItemsDiv = $(".layout-center .layout-center-inner .view #tabs div #TaskInfo #setting .items");

    var taskInfo = {};
    var ChannelID = taskInfoItemsDiv.find("#ChannelID span input").val();
    taskInfo.ChannelID = ChannelID;
    var ChannelName = taskInfoItemsDiv.find("#ChannelName span input").val();
    taskInfo.ChannelName = ChannelName;
    var Multicast = taskInfoItemsDiv.find("#Multicast span input").val();
    taskInfo.Multicast = Multicast;
    var RtspWorkDir = taskInfoItemsDiv.find("#RtspWorkDir span input").val();
    taskInfo.RtspWorkDir = RtspWorkDir;
    var StitchWorkDir = taskInfoItemsDiv.find("#StitchWorkDir span input").val();
    taskInfo.StitchWorkDir = StitchWorkDir;
    var StitchParam = {};
    StitchParam.Hardware = taskInfoItemsDiv.find("#StitchParam #Hardware span input").val();
    StitchParam.HardwareNo = taskInfoItemsDiv.find("#StitchParam #HardwareNo span select").val();
    var Devices = [];
    taskInfoItemsDiv.find("#StitchParam #Input #Devices #CameraChannelID").each(function(index, deviceObj) {
        var Device = {};
        var CameraChannelID = $(deviceObj).find("span input").val();
        Device.CameraChannelID = CameraChannelID;
        var RtspUrl = $(deviceObj).next().find("span input").val();
        Device.RtspUrl = RtspUrl;
        Devices.push(Device);
    });
    var Input = {};
    Input.Devices = Devices;
    Input.Hardware = taskInfoItemsDiv.find("#StitchParam #Input #Hardware span input").val();
    Input.MaxFrameCached = taskInfoItemsDiv.find("#StitchParam #Input #MaxFrameCached span input").val();
    Input.MaxGopCached = taskInfoItemsDiv.find("#StitchParam #Input #MaxGopCached span input").val();
    StitchParam.Input = Input;
    var Output = {};
    var Encode = {};
    Encode.Bitrate = taskInfoItemsDiv.find("#StitchParam #Output #Encode #Bitrate span input").val();
    Encode.CodecId = taskInfoItemsDiv.find("#StitchParam #Output #Encode #CodecId span input").val();
    Encode.GOP = taskInfoItemsDiv.find("#StitchParam #Output #Encode #GOP span input").val();
    Encode.Profile = taskInfoItemsDiv.find("#StitchParam #Output #Encode #Profile span input").val();
    Encode.QP = taskInfoItemsDiv.find("#StitchParam #Output #Encode #QP span input").val();
    Encode.hardware = taskInfoItemsDiv.find("#StitchParam #Output #Encode #hardware span input").val();
    Output.Encode = Encode;
    Output.Multicast = taskInfoItemsDiv.find("#StitchParam #Output #Multicast span input").val();
    Output.Unicast = JSON.parse(TasksInfo[SelectedTaskIndex].StitchParam).Output.Unicast;
    StitchParam.Output = Output;
    var Stitch = {};
    Stitch.Mode = taskInfoItemsDiv.find("#StitchParam #Stitch #Mode span input").val();
    StitchParam.Stitch = Stitch;
    taskInfo.StitchParam = JSON.stringify(StitchParam);
    taskInfo.StitchPID = taskInfoItemsDiv.find("#StitchPID  span input").val();
    taskInfo.RtspPID = taskInfoItemsDiv.find("#RtspPID  span input").val();
    taskInfo.Status = taskInfoItemsDiv.find("#Status  span input").val();
    taskInfo.RtspParamOut = taskInfoItemsDiv.find("#RtspParamOut  span input").val();
    taskInfo.RtspParamIn = taskInfoItemsDiv.find("#RtspParamIn  span input").val();
    taskInfo.RtspWorkDir = taskInfoItemsDiv.find("#StitchWorkDir  span input").val();
    SaveTaskInfo(taskInfo);
}

function SaveTaskInfo(taskInfo) {
    $.ajax({
        url: SaveTaskInfoUrl,
        async: false,
        type: "post",
        dataType: 'json',
        contentType: 'application/json',
        data: JSON.stringify({
            Token: Token,
            TaskDetailInfo: taskInfo
        }),
        //data:JSON.stringify(changeMessage),
        success: function(data) {
            if (data) {
                if (data.Result == 0)
                    GetAllTasks(null);
                else {
                    alert(data.Message);
                    if (data.Result == 105)
                        window.location.href = 'index.html';
                }
            }
        },
        error: function(err) {
            console.log(err);
        }
    });
}

function showAttDiv() {

    var usersAttrDiv = document.getElementById("usersAttrDiv");
    var mou_head = document.getElementById("mou_head");
    usersAttrDiv.style.display = "block";
    usersAttrDiv.style.left = (document.documentElement.clientWidth - usersAttrDiv.clientWidth) / 2 + document.documentElement.scrollLeft + "px";
    usersAttrDiv.style.top = (document.documentElement.clientHeight - usersAttrDiv.clientHeight) / 2 + document.documentElement.scrollTop - 50 + "px";
    mou_head.onmousedown = function(e) {
        if (!e) e = window.event;
        posX = e.clientX - parseInt(usersAttrDiv.style.left);
        posY = e.clientY - parseInt(usersAttrDiv.style.top);
        document.onmousemove = mousemove;
    }
    document.onmouseup = function() {
        document.onmousemove = null;
    }
}

function mousemove(ev) {
    var usersAttrDiv = $("#usersAttrDiv");
    if (ev == null) ev = window.event;
    var iWidth = document.documentElement.clientWidth;
    var iHeight = document.documentElement.clientHeight;
    //限制移动范围
    var destX = (ev.clientX - posX);
    var destY = (ev.clientY - posY);
    if (destX > 0 && (destX + usersAttrDiv[0].offsetWidth < iWidth) && destY > 0 && (destY + usersAttrDiv[0].offsetHeight < iHeight)) {
        usersAttrDiv[0].style.left = (ev.clientX - posX) + "px";
        usersAttrDiv[0].style.top = (ev.clientY - posY) + "px";
    }
}
var ShowUsersInfos = function(data) {
    showAttDiv();
    new GetOtherUsers(data.controllerID);
}
var GetOtherUsers = function(userID) {
    var AcessUser = {
        UserName: UserName,
        Password: Password,
        ID: userID
    };
    var OtherUsers = [];
    var OtherUserInfoUrl = WebServer.ip + "api/v1/FusionServer/GetOtherUsersInfo";
    $.ajax({
        type: 'post',
        url: OtherUserInfoUrl,
        data: AcessUser,
        dataType: 'json',
        ContentType: "application/x-www-form-urlencoded",
        cache: false,
        success: function(msg) {
            //跳转地图页面
            if (msg == "error") {
                $("#errotip").text("输入的用户名或密码有误").show();

            } else {
                $.each(msg, function(index, value) {
                    var otherUser = {};
                    otherUser = value;
                    if (value.state == "0") {
                        otherUser.operateStatus = "正在控制";
                    } else if (value.state == "1") {
                        otherUser.operateStatus = "正在观看";
                    } else if (value.state == "2") {
                        otherUser.operateStatus = "等待中";
                    } else if (value.state == "3") {
                        otherUser.operateStatus = "被抢占";
                    } else if (value.state == "4") {
                        otherUser.operateStatus = "剔除";
                    }
                    OtherUsers.push(otherUser);
                });
                var userSelectTable = $("#userSelectTable");
                var leftbootstrap = $(".userSelect .bootstrap-table");
                if (leftbootstrap.length > 0) {
                    userSelectTable.bootstrapTable('load', OtherUsers);
                } else {
                    var oTable1 = new OtherUsersTableInit(userSelectTable, OtherUsers);
                }
                userSelectTable.find("tbody tr .bs-checkbox input[type=radio]").attr("name", "userSelectItem");
            }
        },
        error: function(err) {
            if (err.responseText != null) {
                $("#errotip").text(err.responseText).show();

            } else {
                $("#errotip").text("获取用户信息失败").show();
            }

        }
    });
}