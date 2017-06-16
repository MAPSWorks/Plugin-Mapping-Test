
//#include <float>
//native test(a)

main(){
  printf("CAN EXAMPLE");
}

@OnCAN(cnt)
{
  printf("CANrx: %d", cnt)
}

@OnTask()
{
  new msg[4+1+8];
  msg[0]=100; //ID_0_7
  msg[1]=0; //ID_8_15
  msg[2]=0; //ID_16_23
  msg[3]=0; //ID_24_31
  msg[4]=4; //DLC - data length
  //msg[4]|=0x80; //IDE (bit 7) 1=ext,0=std

  //data
  msg[5]=0x31;
  msg[6]=0x32;
  msg[7]=0x33;
  msg[8]=0x34;

  can_write(msg,4+1+msg[4]);
  //printf("can sent..");

  return 1000; //repeat every second
}


