#include "video_main.h"
struct buffer {
        void *                  start;
        size_t                  length;
};//定义一个结构体来映射每个缓冲帧。

struct buffer *        buffers        = NULL;
static unsigned int    n_buffers      = 0;
int fd;
int video_init()  //USB摄像头初始化
{
	struct v4l2_capability cap;  //v4l2_capability：linux中设备属性相关结构体
	struct v4l2_format fmt;      //设置当前格式结构体
	unsigned int i;
	enum v4l2_buf_type type; //enum为枚举 限定变量额范围，在这里限定v4l2_buf由type决定    帧类型
    fd=open("/dev/video0",O_RDWR,0);
		if(fd==-1)
		{
			perror("open");
			return 0;
		}		
	int ff=ioctl (fd, VIDIOC_QUERYCAP, &cap);              //获取摄像头参数
	if(ff<0)
	printf("failture VIDIOC_QUERYCAP\n");

/**********查询摄像头所支持的输出格式*********/
	struct v4l2_fmtdesc fmt1; //v4l2_fmtdesc : 帧格式结构体
	int ret;
	memset(&fmt1, 0, sizeof(fmt1));//将fmt1结构体填充为0
	fmt1.index = 0;            //初始化为0，要查询的格式序号
	fmt1.type = V4L2_BUF_TYPE_VIDEO_CAPTURE; // 数据流类型，必须永远是V4L2_BUF_TYPE_VIDEO_CAPTURE
	while ((ret = ioctl(fd, VIDIOC_ENUM_FMT, &fmt1)) == 0) //显示所有支持的格式
	{
		fmt1.index++;
		printf("{ pixelformat = '%c%c%c%c', description = '%s' }\n",fmt1.pixelformat & 0xFF, 
		(fmt1.pixelformat >> 8) & 0xFF,(fmt1.pixelformat >> 16) & 0xFF, 
		(fmt1.pixelformat >> 24) & 0xFF,fmt1.description); //  pixelformat;格式32位   description[32];// 格式名称8位
	}
 /**************************设置当前帧格式************************/
  /*fmt.fmt是调用v4l2_format结构体中的共用体fmt，
   该共用体含有一个名为pix的结构体  第一个fmt是v4l2_format结构体名，
	后一个fmt是里面的一个共用体名*/
    CLEAR (fmt);
    fmt.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;//数据流类型
    fmt.fmt.pix.width       = 320;  //16的倍数
    fmt.fmt.pix.height      = 240;//高必须是16的倍数
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_JPEG;//视频数据存储类型
    fmt.fmt.pix.field       = V4L2_FIELD_INTERLACED;
    if(ioctl (fd, VIDIOC_S_FMT, &fmt)<0)//设置图像格式
    printf("failture VIDIOC_S_FMT\n");
 /**************************读取当前帧格式*************************/
     fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	 ioctl(fd, VIDIOC_G_FMT, &fmt);//读取当前驱动的频捕获格式
	 printf("\nCurrent data format information:\n twidth:%d \n theight:%d \n",
                                     fmt.fmt.pix.width,fmt.fmt.pix.height);
	 printf(" pixelformat = '%c%c%c%c'\n",fmt1.pixelformat & 0xFF, 
    (fmt1.pixelformat >> 8) & 0xFF,(fmt1.pixelformat >> 16) & 0xFF, 
    (fmt1.pixelformat >> 24) & 0xFF); //  pixelformat;格式32位  	
     //file_length = fmt.fmt.pix.bytesperline * fmt.fmt.pix.height; //计算图片大小
/*********************申请物理内存*******************************/
    struct v4l2_requestbuffers req;
    CLEAR (req);
    req.count              = 4;// 缓冲区内缓冲帧的数目  
    req.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;// 缓冲帧数据格式  
    req.memory              = V4L2_MEMORY_MMAP;//内存映射(memorymapping)方式
    if(ioctl (fd, VIDIOC_REQBUFS, &req)<0)//申请缓冲，count是申请的数量
        printf("failture VIDIOC_REQBUFS\n");
    if (req.count < 1)
		printf("Insufficient buffer memory\n");
    buffers = calloc (req.count, sizeof (*buffers));//内存中建立对应空间
//void* calloc (size_t num, size_t size); 在内存中动态地分配 num 个长度为 size 的连续空间，并将每一个字节都初始化为 0。所以它的结果是分配了 num*size 个字节长度的内存空间，并且每个字节的值都是0
    for (n_buffers = 0; n_buffers < req.count; ++n_buffers) 
    {
        struct v4l2_buffer buf;  //驱动中的一帧
        CLEAR (buf);
        buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;//buffer类型
        buf.memory      = V4L2_MEMORY_MMAP;//IO方式，内存映射
        buf.index      = n_buffers;//序号
        //映射用户空间，把VIDIOC_REQBUFS中分配的数据缓存转换成物理地址
        if (-1 == ioctl (fd, VIDIOC_QUERYBUF, &buf)) 
		{   printf ("VIDIOC_QUERYBUF error\n");
	        return -1;
		}
         buffers[n_buffers].length = buf.length;
         buffers[n_buffers].start =mmap (NULL /* start anywhere */,    //通过mmap建立映射关系
                                  buf.length,
                                  PROT_READ | PROT_WRITE /* required */,
                                  MAP_SHARED /* recommended */,
                                  fd, buf.m.offset);//转换为相对地址

        if (MAP_FAILED == buffers[n_buffers].start)
		{
            printf ("mmap failed\n");
		    return -1;
		}
		if(ioctl(fd,VIDIOC_QBUF,&buf)==-1)//申请到的缓冲进入列队
		{   
		    printf ("VIDIOC_QBUF failed\n");
			return -1;
		}
    }
 /**********************开始捕捉图像数据***************************************/           
    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (-1 == ioctl (fd, VIDIOC_STREAMON, &type)) //开始捕捉图像数据
		printf ("VIDIOC_STREAMON failed\n");
	return 1;
}

static int video(void)
{
	struct v4l2_buffer buf;
	unsigned int i;
	CLEAR (buf);
	buf.type    = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	buf.memory  = V4L2_MEMORY_MMAP;
	buf.index   = 0;
	while(1)
	{
		if(ioctl (fd, VIDIOC_DQBUF, &buf)<0)//把数据从缓存中读取出来
		{
			printf("failture\n"); //出列采集的帧缓冲
			return -1;
		}
		memcpy(databuf->buf,buffers[buf.index].start,buffers[buf.index].length);
		databuf->datasize=buf.bytesused;
		pthread_cond_signal(&g_cond);
		usleep(10000);
		/* assert (buf.index < n_buffers);//其作用是如果它的条件返回错误，则终止程序执行，即是buf.index >= n_buffers,程序终止
		printf ("buf.index dq is %d,\n",buf.index); */
		if(ioctl (fd, VIDIOC_QBUF, &buf)<0)//再将其入列，把数据放回缓存队列
		{  
			printf("failture VIDIOC_QBUF\n");
			return -1;
		}
	}	
	return 1;
}
void close_video()
{   
    unsigned char i=0;
    enum v4l2_buf_type type;
unmap:
	for (i = 0; i < n_buffers; ++i)
		if (-1 == munmap (buffers[i].start, buffers[i].length))//解除内存映射关系
	printf ("munmap error");
	type = V4L2_BUF_TYPE_VIDEO_CAPTURE;  
    if (-1 == ioctl(fd, VIDIOC_STREAMOFF, &type))  //结束视频显示函数
        printf("VIDIOC_STREAMOFF"); 
	close (fd);
}

void *pthread_video(void *arg)
{
		pthread_detach(pthread_self());
		databuf=(buf_t *)malloc(sizeof(buf_t)+buffers[0].length);
		while(1)
		{
			video();
		}
		close_video();
		return NULL;
}