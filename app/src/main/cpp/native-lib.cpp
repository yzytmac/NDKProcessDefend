#include <jni.h>
#include <string>
#include "native-lib.h"

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

}

/**
 * 创建服务端socket
 * @return
 */
int child_create_channel() {
    int connfd = 0;
    int listenfd = socket(AF_LOCAL, SOCK_STREAM, 0);//创建一个socket句柄
    struct sockaddr_un addr;//指向了某个内存地址
    memset(&addr, 0, sizeof(sockaddr_un));//清空内存
    addr.sun_family = AF_LOCAL;
    strcpy(addr.sun_path, PATH);
    int i = bind(listenfd, (const sockaddr *) &addr, sizeof(sockaddr_un));
    if (i < 0) {
        LOGE("绑定错误");
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
                LOGE("读取错误");
                return 0;
            }
        }
        m_child = connfd;
        LOGE("连接成功了");
        break;
    }
    return 1;
}


JNIEXPORT void JNICALL
Java_com_example_yzy_ndkprocessdefend_Watcher_createWatcher(JNIEnv *env,
                                                            jobject instance,
                                                            jstring userId_) {
    const char *userId = env->GetStringUTFChars(userId_, 0);


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

extern "C"
JNIEXPORT jstring JNICALL
Java_com_example_yzy_ndkprocessdefend_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}
