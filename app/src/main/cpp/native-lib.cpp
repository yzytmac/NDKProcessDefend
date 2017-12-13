
#include "native-lib.h"
const char *userId;
const char *PATH = "/data/data/com.example.yzy.ndkprocessdefend/my.socket";
int m_child;


void child_do_work() {
    //开启socket  服务端
    if (child_create_channel()) {
        child_listen_msg();
    }


}

/**
 * 服务端读取
 */
void child_listen_msg() {
    fd_set rfds;
    struct timeval timout = {3,0};

    while (1) {
        FD_ZERO(&rfds);//清空内容
        FD_SET(m_child, &rfds);
        int r = select(m_child + 1, &rfds, NULL,NULL,&timout);
        LOGE("读消息%d",r);
        if (r>0){
            //缓冲区
            char pkg[256] = {0};
            //保证读的是客户端
            if (FD_ISSET(m_child,&rfds)){
                //读状态而已，并不读任何数据，阻塞式函数
                int result = read(m_child,pkg, sizeof(pkg));
                //开启服务
                LOGE("userId: %s",userId);
                execlp("am","am","startservice","--user",userId,
                       "com.example.yzy.ndkprocessdefend/com.example.yzy.ndkprocessdefend.ProcessService",(char*)NULL);
                break;
            }
        }
    }

}
/**
 * 创建服务端socket
 * @return
 */
int child_create_channel() {

    int listenfd = socket(AF_LOCAL, SOCK_STREAM, 0);//创建一个socket句柄
    unlink(PATH);
    struct sockaddr_un addr;//指向了某个内存地址
    memset(&addr, 0, sizeof(sockaddr_un));//清空内存
    addr.sun_family = AF_LOCAL;
    int connfd = 0;
    strcpy(addr.sun_path, PATH);
    int i = bind(listenfd, (const sockaddr *) &addr, sizeof(sockaddr_un));
    if (i < 0) {
        LOGE("绑定错误60");
        return 0;
    }

    listen(listenfd, 5);//最大监听5个
    //保证宿主进程连接成功
    while (1) {
        //阻塞式函数，返回客户端地址
        if ((connfd = accept(listenfd, NULL, NULL)) < 0) {//失败
            if (errno == EINTR) {
                continue;
            } else {
                LOGE("读取错误72");
                return 0;
            }
        }
        m_child = connfd;
        LOGE("连接成功了77");
        break;
    }
    return 1;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_yzy_ndkprocessdefend_Watcher_connectMonitor(JNIEnv *env, jobject instance) {
    //客户端 apk进程
    int socked;
    struct  sockaddr_un addr;
    while (1){
        LOGE("开始连接了89");
        socked = socket(AF_LOCAL,SOCK_STREAM,0);
        if (socked<0){
            LOGE("连接失败92");
            return;
        }

        memset(&addr,0, sizeof(sockaddr_un));
        addr.sun_family = AF_LOCAL;
        strcpy(addr.sun_path,PATH);

        if (connect(socked,(const sockaddr *)&addr, sizeof(sockaddr_un))<0){
            LOGE("连接失败102");
            close(socked);
            sleep(1);
            continue;
        }
        LOGE("连接成功107");
        break;
    }



}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_yzy_ndkprocessdefend_Watcher_createWatcher(JNIEnv *env,
                                                            jobject instance,
                                                            jstring userId_) {
    userId = env->GetStringUTFChars(userId_, 0);
    pid_t pid = fork();
    //fork后下面会调用两次
    if (pid < 0) {

    } else if (pid == 0) {
        //子进程 我们的守护进程
        child_do_work();
    } else if (pid > 0) {
        //父进程
    }


    env->ReleaseStringUTFChars(userId_, userId);
}



