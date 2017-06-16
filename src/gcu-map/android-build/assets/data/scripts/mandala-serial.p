//#include <float>
//native test(a)
new x{}="12345";
main(){
  new Float: xf=0.5,Float: f2=0.5;
  print "Hello world\n"

  new rv=0, cnt=1;
  for (new i = 0; i < cnt; i++) {
    rv += float(((i/xf)));
    //test();
    xf=cos((rv));
    xf+=f2
    xf=(f2*xf)/1.234;
    rv+= (xf)+x{i%5};
    printf("i=%d\n",i)
  }
  
  rv*=2.5*cos(0)
  rv=strfloat(x);
  printf("f:%.2f\n",f2);

  printf("idx=%d\n",f_cmd_altitude);
  printf("val=%.2f",get_var(f_cmd_altitude));
  set_var(f_cmd_altitude,123);
  printf("val=%.2f\n",get_var(f_cmd_altitude));
  set_var(f_mode,mode_LANDING,true);
  
  for (new i = 0; i < 10; i++){
    wait(i+1);
    //new Float: v=get_var(f_cmd_altitude,true);
    //printf("wait=%.2f\n",v);
    //set_var(f_airspeed,v*0.1,true);
  }

  print("serial_read...\n");
  new portID=1;
  cnt=serial_read(portID);
  printf("pid=%d, cnt=%d {%d,%d,%d}\n",portID,cnt,serial_byte(0),serial_byte(1),serial_byte(2));
  
  new str{10};
  cnt=serial_string(str);
  printf("str (%d): %s\n",cnt,str);
  return rv
}

new counter = 0;
@OnTask()
{
  counter++;
  if(counter%100==0){
    //printf("task: %d\n",counter);
  }  
}

@OnSerial(portID, cnt)
{
  new str{256}="";
  printf("rx: pid=%d, cnt=%d\n",portID,cnt);
  for(new i=0;i<cnt;i++){
    new strv{8};
    strformat(strv,sizeof(strv),true,"%x ",serial_byte(i));
    strcat(str,strv);
  }
  print(str);
}

//called by sending idx_vmexec,<@hello>
//gcu console command: vmexec '@hello'
forward @hello()
@hello()
{
	printf "Hello\n"
}

