#include <Arduino.h>
#include <M5Core2.h>

extern const unsigned char wifi[0x78];

String Input_data;

ButtonColors on_clrs = {YELLOW, WHITE, WHITE};
ButtonColors off_clrs = {BLACK, WHITE, WHITE};
ButtonColors on_clrs_dial = {BLACK, WHITE, WHITE};
ButtonColors off_clrs_dial = {GREEN, WHITE, WHITE};
ButtonColors on_clrs_hang = {BLACK, WHITE, WHITE};
ButtonColors off_clrs_hang = {RED, WHITE, WHITE};
Button tl(0, 0, 0, 0, false , "1", off_clrs, on_clrs, MC_DATUM);
Button t2(0, 0, 0, 0, false, "2", off_clrs, on_clrs, MC_DATUM);
Button t3(0, 0, 0, 0, false, "3", off_clrs, on_clrs, MC_DATUM);
//Button t4(0, 0, 0, 0, false, "QQ", off_clrs, on_clrs, MC_DATUM);
Button t5(0, 0, 0, 0, false , "4", off_clrs, on_clrs, MC_DATUM);
Button t6(0, 0, 0, 0, false, "5", off_clrs, on_clrs, MC_DATUM);
Button t7(0, 0, 0, 0, false, "6", off_clrs, on_clrs, MC_DATUM);
//Button t8(0, 0, 0, 0, false, "WX", off_clrs, on_clrs, MC_DATUM);
Button t9(0, 0, 0, 0, false , "7", off_clrs, on_clrs, MC_DATUM);
Button t10(0, 0, 0, 0, false, "8", off_clrs, on_clrs, MC_DATUM);
Button t11(0, 0, 0, 0, false, "9", off_clrs, on_clrs, MC_DATUM);
Button t12(0, 0, 0, 0, false, "HANG", off_clrs_hang, on_clrs_hang, MC_DATUM);
//Button t13(0, 0, 0, 0, false , "0", off_clrs, on_clrs, MC_DATUM);
Button t14(0, 0, 0, 0, false, "0", off_clrs, on_clrs, MC_DATUM);
Button t15(0, 0, 0, 0, false, "DEL", off_clrs, on_clrs, MC_DATUM);
Button t16(0, 0, 0, 0, false, "DIAL", off_clrs_dial, on_clrs_dial, MC_DATUM);

String sendMsg(String command) 
{
    Serial2.print(command);
    // canvas.printf("Sent: %s", command);
    // canvas.pushSprite(0, 0);
    return command;
}


String getMsg() 
{
    String restr ="";
    while(1){
        if(Serial2.available()) {
          String str = Serial2.readString();
                restr += str;
          //restr = restr + (char)Serial2.read();
        }else{
            break;
        }
    }
    Serial.print(restr);
    // canvas.printf("Received: %s\r\n", restr);
    // canvas.printf("\r\n");
    // canvas.pushSprite(0, 0);
    return restr;
}   

String send_and_getMsg(String str) 
{
    sendMsg(str);
    return getMsg();
}

void doButtons() {
  uint8_t but_w = 75;
  uint8_t but_h = 43;
  uint8_t but_w_big = 75;
  uint8_t but_h_big = 91;
  tl.set(0, 48, but_w, but_h);
  t2.set(80, 48, but_w, but_h);
  t3.set(160, 48, but_w, but_h);
  //t4.set(240, 48, but_w, but_h);
  t5.set(0, 96, but_w, but_h);
  t6.set(80, 96, but_w, but_h);
  t7.set(160, 96, but_w, but_h);
  //t8.set(240, 96, but_w, but_h);
  t9.set(0, 144, but_w, but_h);
  t10.set(80, 144, but_w, but_h);
  t11.set(160, 144, but_w, but_h);
  t12.set(240, 48, but_w_big, but_h_big);
  //t13.set(0, 192, but_w, but_h);
  t14.set(80, 192, but_w, but_h);
  t15.set(160, 192, but_w, but_h);
  t16.set(240, 144, but_w_big, but_h_big);
  M5.Buttons.draw();
}

void eventDisplay(Event& e) {
  Serial.printf("%-12s finger%d  %-18s (%3d, %3d) --> (%3d, %3d)   ",
                e.typeName(), e.finger, e.objName(), e.from.x, e.from.y,
                e.to.x, e.to.y);
  Serial.printf("( dir %d deg, dist %d, %d ms )\n", e.direction(),
                e.distance(), e.duration);
  if (String(e.objName()).equals(String("BtnA"))) {
    //执行BtnA事件
    delay(50);
  } else {
    if (String(e.objName()).equals(String("BtnB"))) {
      //执行BtnB事件
      delay(50);
    } else {
      if (String(e.objName()).equals(String("BtnC"))) {
        //执行BtnC事件
        delay(50);
      } else {
        if (String(e.typeName()).equals(String("E_RELEASE"))) { //检测是否释放屏幕
          if (!String(e.objName()).equals(String("background"))) { //检测是否为已注册按键区域

            if (String(e.objName()).equals(String("HANG"))) { //检测是否为按下HANG按键
              Serial.println("HANG");
              //M5.Lcd.drawBitmap(262, 6, 30, 31, QQ_icon); //显示QQ图标
              send_and_getMsg("AT+CHUP\r\n");
              M5.Lcd.fillRoundRect(5, 0, 260, 43, 5, BLACK); //清空显示字符串
              M5.Lcd.drawRoundRect(0, 0, 315, 43, 5, WHITE); //重新绘制文本显示框
              M5.Lcd.setCursor(5, 33); //设置文本显示坐标
              Input_data = "";
              delay(50);
            } else {
              if (String(e.objName()).equals(String("DIAL"))) { //检测是否为按下DIAL按键
                Serial.println("DIAL");
                //M5.Lcd.drawBitmap(262, 6, 30, 30, WX_icon); //显示微信图标
                if (!Input_data.equals(String("")) && Input_data.toFloat() > 0) { //当输入不为空且数字大于0生成订单
                  send_and_getMsg("AT+CSDVC=1\r\n");
                  delay(50);
                  send_and_getMsg("AT+CLVL?\r\n");
                  delay(50);
                  send_and_getMsg("AT+CLVL=5\r\n");
                  delay(50);
                  String callNum = "ATD" + Input_data + ";\r\n";
                  send_and_getMsg(callNum);
                  M5.Lcd.fillRoundRect(5, 0, 260, 43, 5, BLACK); //清空显示字符串
                  M5.Lcd.drawRoundRect(0, 0, 315, 43, 5, WHITE); //重新绘制文本显示框
                  M5.Lcd.setCursor(5, 33); //设置文本显示坐标
                  Input_data = "";
                  M5.Lcd.printf("DIALING...");
                }
                delay(50);
                  } else {
                    if (String(e.objName()).equals(String("DEL"))) { //检测是否为按下DEL删除按键，按下删除末尾字符
                      Input_data = String(Input_data).substring(0, (String(Input_data).length() - 1)); //删除输入字符串末尾字符
                    //M5.Lcd.fillRoundRect(40, 0, 180, 43, 5, BLACK); //清空显示字符串
                      M5.Lcd.fillRoundRect(5, 0, 260, 43, 5, BLACK); //清空显示字符串
                      M5.Lcd.drawRoundRect(0, 0, 315, 43, 5, WHITE); //重新绘制文本显示框
                      //M5.Lcd.setCursor(40, 33); //设置文本显示坐标
                      M5.Lcd.setCursor(5, 33); //设置文本显示坐标
                      M5.Lcd.printf(Input_data.c_str()); //显示输入文本
                      delay(50);
                    } else {
                      if (String(Input_data).length() <= 10) { //检测到数字按键及小数点
                        Input_data = String(Input_data) + String(e.objName()); //连接字符串
                        // if (Input_data.startsWith(".")) { //输入首位字符为小数点
                        //   Input_data = ""; //清空字符串
                        // }
                        // M5.Lcd.fillRoundRect(40, 0, 180, 43, 5, BLACK);
                        Serial.println(Input_data);
                        M5.Lcd.fillRoundRect(5, 0, 260, 43, 5, BLACK);
                        M5.Lcd.drawRoundRect(0, 0, 315, 43, 5, WHITE);
                        //M5.Lcd.setCursor(40, 33);
                        M5.Lcd.setCursor(5, 33);
                        M5.Lcd.printf(Input_data.c_str());
                        delay(50);
                      }
                    }
                  }
                }
              }
            }
          }
        }
    }
}

void checkConnect()
{   
    M5.Lcd.drawXBitmap(265, 6, wifi, 30, 30, RED);
    //Serial.println(send_and_getMsg("AT+CSQ\r\n").lastIndexOf(",99"));
    while (send_and_getMsg("AT+CSQ\r\n").lastIndexOf(",99") == -1) 
    {
        Serial.println("Connecting...");
        send_and_getMsg("AT+CSQ\r\n");
        delay(1000);
    } 
    M5.Lcd.drawXBitmap(265, 6, wifi, 30, 30, GREEN);
    Serial.println("Connected");
    send_and_getMsg("AT+CSQ\r\n");
}

void setup() {
  M5.begin();

  M5.Lcd.fillScreen(BLACK); //设置背景颜色
  M5.Buttons.addHandler(eventDisplay, E_ALL - E_MOVE); //注册按键动作检测
  M5.Lcd.drawRoundRect(0, 0, 315, 43, 5, WHITE); //绘制显示文本框
  M5.Lcd.setTextColor(WHITE, BLACK); //设置显示文本颜色
  M5.Lcd.setTextSize(2); //设置显示字体大小
  doButtons(); //设置按钮属性及绘制按钮

  Serial.begin(115200);
  Serial.print("Serial OK\r\n");
  //Serial2.begin(115200, SERIAL_8N1, 5, 13);
  Serial2.begin(115200, SERIAL_8N1, 13, 14);
  Serial.print("Serial2 OK\r\n");
  checkConnect();
}

void loop() {
  M5.update();
}
