#ifndef JNIPROXY_H_
#define JNIPROXY_H_

#include <jni.h>

class JniProxy {
public:
	virtual JNIEnv* getJNIEnv() = 0;
	virtual jclass getClass() = 0;
	virtual jobject getInstance() = 0;
};

#endif /* JNIPROXY_H_ */
