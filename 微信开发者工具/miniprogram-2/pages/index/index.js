Page({
  data: {
    temp:0,
    humi:0,
    fire:0,
    mq:0,
    switch1_checked: false,// 开关的状态
    switch2_checked: false,
    switch3_checked: false,
  },
  handleSwitch1(event) {
    const isChecked = event.detail.value;
    console.log("第一个开关状态改变为：", isChecked);
    const data = isChecked ? "1" : "0"; 
    console.log("data:" + data);
    
    this.setData({
      switch1: data ,
      switch1_checked: isChecked,
    })
    console.log("switch1:" + this.data.switch1);
   
    this.updateSwitchData("switch1", data);
  },
  
  handleSwitch2(event) {
    const isChecked = event.detail.value;
    console.log("第二个开关状态改变为：", isChecked);
    const data = isChecked ? "1" : "0"; // 将 true 转换为 "1"，将 false 转换为 "0"
    console.log("data:" + data);
    
    this.setData({
      switch2: data ,
      switch2_checked: isChecked,
    })
    console.log("switch2:" + this.data.switch2);
    this.updateSwitchData("switch2", data);
  },
  
  handleSwitch3(event) {
    const isChecked = event.detail.value;
    console.log("第三个开关状态改变为：", isChecked);
    const data = isChecked ? "1" : "0"; // 将 true 转换为 "1"，将 false 转换为 "0"
    console.log("data:" + data);
    
    this.setData({
      switch3: data ,
      switch3_checked: isChecked,
    })
    console.log("switch3:" + this.data.switch3);
    this.updateSwitchData("switch3", data);
  },
  updateSwitchData(switchName, data) {
    console.log("更新开关状态数据：", switchName, data);
    const devId = "1072230738"; 
    const apiKey = "RtORRhWxpUn=YIP05x4lTJapexs="; 
    const url = `https://api.heclouds.com/devices/1072230738/datapoints`;
    const sendCommandURL = "https://api.heclouds.com/cmds"
    const headers = {
      "api-key": apiKey,
    };
    console.log("updateSwitchData switchName:" + switchName + " data:" + data);
    //按钮发送命令控制硬件
    wx.request({
      url: sendCommandURL + "?device_id=" + devId,
      method: 'POST',
      header: {
        'content-type': 'application/x-www-form-urlencoded',
        "api-key": "RtORRhWxpUn=YIP05x4lTJapexs="
      },
      data: switchName + ":" + data ,
      data: switchName + ":" + data ,
      success(res) {
        console.log("控制成功")
        console.log(res)
      }
    })
  },
  getinfo(){
    var that = this
    wx.request({
    url: "https://api.heclouds.com/devices/1072230738/datapoints",   
    //设备ID
    header: {
      "api-key": "RtORRhWxpUn=YIP05x4lTJapexs=" //api-key
    },
    method: "GET",
    success: function (e) {
      console.log("获取成功",e)
      that.setData({
        temp:e.data.data.datastreams[10].datapoints[0].value,
        humi:e.data.data.datastreams[6].datapoints[0].value,
        fire:e.data.data.datastreams[4].datapoints[0].value,
        mq:e.data.data.datastreams[2].datapoints[0].value,
      })
      console.log("temp==",that.data.temp)
      console.log("humi==",that.data.humi)
      console.log("fire==",that.data.fire )
      console.log("mq==",that.data.mq)
    }
   });
  },
 
  onLoad() {
    var that = this
    setInterval(function(){
      that.getinfo()
    },1200)
   
  }
})