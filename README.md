# BMPglue
Pure C to glue *.bmps into a new image.

Q1:
```
gcc bmp_info.c A2_bmp_helpers.c -o bmp_info
./bmp_info mario_16_bit.bmp
// 命令行会打印图片信息
```

Q2:
```
gcc bmp_mask.c A2_bmp_helpers.c -o bmp_mask
./bmp_mask mario_16_bit.bmp mario_masked.bmp 150 150 200 200 0 0 0
// 会生成一个马里奥加一个黑色正方形的图片
```

Q3：
作业提供了一个脚本，里面有好几条指令，运行后生成蒙特利尔地铁图
脚本需要添加运行权限
```
chmod a+x ./create_metro_collage.bash
./create_metro_collage.bash
```

这道题需要注意的地方：
理解BMP图片格式，作业一开始提供了一些信息，但是不够全。这是一个比较好的教程。
http://blog.csdn.net/u012877472/article/details/50272771
BMP图片，你用sublime或者记事本打开，可以看到一堆字符串。最前面的54个byte（一个byte是你看到的四个数字）记录了图片的信息，然后从offset位置开始存储图片信息，按blue,green,blue,blue,green,blue.....存储。每个颜色大小范围是0-255，对应十六进制就是00-ff.
有两点格外注意：
1. 数据第一个点是图片的左下角第一个点，然后是左下角第二个点.....倒数第二行第一个点，倒数第二行第二个点....一直到右上角。
2. 最坑的是：BMP格式要求数据每行必须是4byte的倍数，不然要用0补齐。举个例子，假如一行5个像素点，总共是5*3=15个byte的数据（每个像素3个颜色，共3byte).显然15不是4的倍数，那么就要补一个byte的0. 这就是为什么第一题里面要显示一个padding的意义！


>现在总结下bmp图片大小：mario_16_bit.bmp，236行，253列，offset是54.那么大小是236*253*3+54=179178byte，你可以右键查看图片属性检查，刚好是179178字节（byte）

>metro_mid_left:147行，303列, offset=122. 147*3=441，不足4，那么填充3byte的0，所以每一行的大小是444byte， 444*303+122=134654，你也可以查看图片属性确认。

附注：一个byte是8个bit，char的大小刚好是一个byte的大小。
