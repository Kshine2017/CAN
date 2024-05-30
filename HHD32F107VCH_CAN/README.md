
# 1.初始化
```
    ZL_CanInit(1);
    ZL_CanInit(2);
```
# 2.发送
```
static void CAN_ACK_0x4D(void)
{
    ZL_CANX_Send8Byte(1,0x4D ,can_pkg0.array,8);
    ZL_CANX_Send8Byte(1,0x4D ,can_pkg1.array,8);
    ZL_CANX_Send8Byte(1,0x4D ,can_pkg2.array,8);
    ZL_CANX_Send8Byte(1,0x4D ,can_pkg3.array,8);
    ZL_CANX_Send8Byte(1,0x4D ,can_pkg4.array,8);
    ZL_CANX_Send8Byte(1,0x4D ,can_pkg5.array,8);
    ZL_CANX_Send8Byte(1,0x4D ,can_pkg6.array,8);
}    

```

# 3.接收，从FIFO中读取数据
```
void CAN(void)
{ 
    int i=1;
    if(CurrentStatus==WORK_MODE_STANDBY)
        return;
    feeddog_H();
    #if 1
    for(i=1;i<=2;i++)
    {
       uint8_t* frame = (uint8_t*) ZL_CanFIFORead16Bytes(i);
      
        //读到数据
       if(frame)
        {
//          uint32_t srcID =*((uint32_t*)(frame+2));
//          uint8_t code  = *((uint8_t*)(frame+6));//byte0 //0x4Dh
            uint8_t mode  = *((uint8_t*)(frame+7));//byte1 //0X1
            switch(mode)//模式字	SWO1
            {
                case 0x01:
                {
                  CAN_ACK_0x4D();
                }
                break;
        
                default:
                
                break;
            }
        }
    }
    #endif
    feeddog_L();
        
}

```
